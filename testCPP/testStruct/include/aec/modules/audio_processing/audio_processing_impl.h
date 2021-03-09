/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MODULES_AUDIO_PROCESSING_AUDIO_PROCESSING_IMPL_H_
#define MODULES_AUDIO_PROCESSING_AUDIO_PROCESSING_IMPL_H_

#include <list>
#include <memory>
#include <vector>

#include "api/function_view.h"
#include "modules/audio_processing/aec3/echo_canceller3.h"
#include "modules/audio_processing/agc/agc_manager_direct.h"
#include "modules/audio_processing/agc/gain_control.h"
#include "modules/audio_processing/audio_buffer.h"
//#include "modules/audio_processing/echo_control_mobile_impl.h"
#include "modules/audio_processing/gain_control_impl.h"
//#include "modules/audio_processing/gain_controller2.h"
#include "modules/audio_processing/high_pass_filter.h"
//#include "modules/audio_processing/include/aec_dump.h"
#include "modules/audio_processing/include/audio_frame_proxies.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "modules/audio_processing/include/audio_processing_statistics.h"
#include "modules/audio_processing/level_estimator.h"
//#include "modules/audio_processing/ns/noise_suppressor.h"
#include "modules/audio_processing/render_queue_item_verifier.h"
#include "modules/audio_processing/residual_echo_detector.h"
#include "modules/audio_processing/rms_level.h"
//#include "modules/audio_processing/transient/transient_suppressor.h"
//#include "modules/audio_processing/voice_detection.h"
#include "rtc_base/critical_section.h"
#include "rtc_base/gtest_prod_util.h"
#include "rtc_base/ignore_wundef.h"
#include "rtc_base/swap_queue.h"
#include "rtc_base/thread_annotations.h"

namespace webrtc {

class ApmDataDumper;
class AudioFrame;
class AudioConverter;

class AudioProcessingImpl : public AudioProcessing {
 public:
  // Methods forcing APM to run in a single-threaded manner.
  // Acquires both the render and capture locks.
  explicit AudioProcessingImpl(const webrtc::Config& config);
  // AudioProcessingImpl takes ownership of capture post processor.
  AudioProcessingImpl(const webrtc::Config& config,
                      std::unique_ptr<CustomProcessing> capture_post_processor,
                      std::unique_ptr<CustomProcessing> render_pre_processor,
                      std::unique_ptr<EchoControlFactory> echo_control_factory,
                      rtc::scoped_refptr<EchoDetector> echo_detector,
                      std::unique_ptr<CustomAudioAnalyzer> capture_analyzer);
  ~AudioProcessingImpl() override;
  int Initialize() override;
  int Initialize(int capture_input_sample_rate_hz,
                 int capture_output_sample_rate_hz,
                 int render_sample_rate_hz,
                 ChannelLayout capture_input_layout,
                 ChannelLayout capture_output_layout,
                 ChannelLayout render_input_layout) override;
  int Initialize(const ProcessingConfig& processing_config) override;
  void ApplyConfig(const AudioProcessing::Config& config) override;
  void SetExtraOptions(const webrtc::Config& config) override;
  void UpdateHistogramsOnCallEnd() override;
#ifdef ENABLE_AECDUMP
  void AttachAecDump(std::unique_ptr<AecDump> aec_dump) override;
#endif
  void DetachAecDump() override;
  void AttachPlayoutAudioGenerator(
      std::unique_ptr<AudioGenerator> audio_generator) override;
  void DetachPlayoutAudioGenerator() override;

  void SetRuntimeSetting(RuntimeSetting setting) override;

  // Capture-side exclusive methods possibly running APM in a
  // multi-threaded manner. Acquire the capture lock.
  int ProcessStream(AudioFrame* frame) override {
    return ProcessAudioFrame(this, frame);
  }
  int ProcessStream(const int16_t* const src,
                    const StreamConfig& input_config,
                    const StreamConfig& output_config,
                    int16_t* const dest) override;
  int ProcessStream(const float* const* src,
                    const StreamConfig& input_config,
                    const StreamConfig& output_config,
                    float* const* dest) override;
  bool GetLinearAecOutput(
      rtc::ArrayView<std::array<float, 160>> linear_output) const override;
  void set_output_will_be_muted(bool muted) override;
  int set_stream_delay_ms(int delay) override;
  void set_stream_key_pressed(bool key_pressed) override;
  void set_stream_analog_level(int level) override;
  int recommended_stream_analog_level() const override;

  // Render-side exclusive methods possibly running APM in a
  // multi-threaded manner. Acquire the render lock.
  int ProcessReverseStream(AudioFrame* frame) override {
    return ProcessReverseAudioFrame(this, frame);
  }
  int ProcessReverseStream(const int16_t* const src,
                           const StreamConfig& input_config,
                           const StreamConfig& output_config,
                           int16_t* const dest) override;
  int AnalyzeReverseStream(const float* const* data,
                           const StreamConfig& reverse_config) override;
  int ProcessReverseStream(const float* const* src,
                           const StreamConfig& input_config,
                           const StreamConfig& output_config,
                           float* const* dest) override;

  // Methods only accessed from APM submodules or
  // from AudioProcessing tests in a single-threaded manner.
  // Hence there is no need for locks in these.
  int proc_sample_rate_hz() const override;
  int proc_split_sample_rate_hz() const override;
  size_t num_input_channels() const override;
  size_t num_proc_channels() const override;
  size_t num_output_channels() const override;
  size_t num_reverse_channels() const override;
  int stream_delay_ms() const override;

  AudioProcessingStats GetStatistics(bool has_remote_tracks) override {
    return GetStatistics();
  }
  AudioProcessingStats GetStatistics() override {
    return stats_reporter_.GetStatistics();
  }

  // TODO(peah): Remove MutateConfig once the new API allows that.
  void MutateConfig(rtc::FunctionView<void(AudioProcessing::Config*)> mutator);
  AudioProcessing::Config GetConfig() const override;

  void setAecMaskValue(float maskvalue) override;
    
 protected:
  // Overridden in a mock.
  virtual int InitializeLocked()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_, crit_capture_);

 private:
  // TODO(peah): These friend classes should be removed as soon as the new
  // parameter setting scheme allows.
  FRIEND_TEST_ALL_PREFIXES(ApmConfiguration, DefaultBehavior);
  FRIEND_TEST_ALL_PREFIXES(ApmConfiguration, ValidConfigBehavior);
  FRIEND_TEST_ALL_PREFIXES(ApmConfiguration, InValidConfigBehavior);

  // Class providing thread-safe message pipe functionality for
  // |runtime_settings_|.
  class RuntimeSettingEnqueuer {
   public:
    explicit RuntimeSettingEnqueuer(
        SwapQueue<RuntimeSetting>* runtime_settings);
    ~RuntimeSettingEnqueuer();
    void Enqueue(RuntimeSetting setting);

   private:
    SwapQueue<RuntimeSetting>& runtime_settings_;
  };

  std::unique_ptr<ApmDataDumper> data_dumper_;
  static int instance_count_;
  const bool use_setup_specific_default_aec3_config_;

  SwapQueue<RuntimeSetting> capture_runtime_settings_;
  SwapQueue<RuntimeSetting> render_runtime_settings_;

  RuntimeSettingEnqueuer capture_runtime_settings_enqueuer_;
  RuntimeSettingEnqueuer render_runtime_settings_enqueuer_;

  // EchoControl factory.
  std::unique_ptr<EchoControlFactory> echo_control_factory_;

  class SubmoduleStates {
   public:
    SubmoduleStates(bool capture_post_processor_enabled,
                    bool render_pre_processor_enabled,
                    bool capture_analyzer_enabled);
    // Updates the submodule state and returns true if it has changed.
    bool Update(bool high_pass_filter_enabled,
                bool mobile_echo_controller_enabled,
                bool residual_echo_detector_enabled,
                bool noise_suppressor_enabled,
                bool adaptive_gain_controller_enabled,
                bool gain_controller2_enabled,
                bool pre_amplifier_enabled,
                bool echo_controller_enabled,
                bool voice_detector_enabled,
                bool transient_suppressor_enabled);
    bool CaptureMultiBandSubModulesActive() const;
    bool CaptureMultiBandProcessingPresent() const;
    bool CaptureMultiBandProcessingActive(bool ec_processing_active) const;
    bool CaptureFullBandProcessingActive() const;
    bool CaptureAnalyzerActive() const;
    bool RenderMultiBandSubModulesActive() const;
    bool RenderFullBandProcessingActive() const;
    bool RenderMultiBandProcessingActive() const;
    bool HighPassFilteringRequired() const;

   private:
    const bool capture_post_processor_enabled_ = false;
    const bool render_pre_processor_enabled_ = false;
    const bool capture_analyzer_enabled_ = false;
    bool high_pass_filter_enabled_ = false;
    bool mobile_echo_controller_enabled_ = false;
    bool residual_echo_detector_enabled_ = false;
    bool noise_suppressor_enabled_ = false;
    bool adaptive_gain_controller_enabled_ = false;
    bool gain_controller2_enabled_ = false;
    bool pre_amplifier_enabled_ = false;
    bool echo_controller_enabled_ = false;
    bool voice_detector_enabled_ = false;
    bool transient_suppressor_enabled_ = false;
    bool first_update_ = true;
  };

  // Methods for modifying the formats struct that is used by both
  // the render and capture threads. The check for whether modifications are
  // needed is done while holding a single lock only, thereby avoiding that the
  // capture thread blocks the render thread.
  // Called by render: Holds the render lock when reading the format struct and
  // acquires both locks if reinitialization is required.
  int MaybeInitializeRender(const ProcessingConfig& processing_config)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);
  // Called by capture: Holds the capture lock when reading the format struct
  // and acquires both locks if reinitialization is needed.
  int MaybeInitializeCapture(const StreamConfig& input_config,
                             const StreamConfig& output_config);

  // Method for updating the state keeping track of the active submodules.
  // Returns a bool indicating whether the state has changed.
  bool UpdateActiveSubmoduleStates()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Methods requiring APM running in a single-threaded manner, requiring both
  // the render and capture lock to be acquired.
  int InitializeLocked(const ProcessingConfig& config)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_, crit_capture_);
  void InitializeResidualEchoDetector()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_, crit_capture_);
  void InitializeEchoController()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_, crit_capture_);

  // Initializations of capture-only submodules, requiring the capture lock
  // already acquired.
  void InitializeHighPassFilter(bool forced_reset)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializeVoiceDetector() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializeGainController1() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializeTransientSuppressor()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializeGainController2() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializeNoiseSuppressor() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializePreAmplifier() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializePostProcessor() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void InitializeAnalyzer() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Initializations of render-only submodules, requiring the render lock
  // already acquired.
  void InitializePreProcessor() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);

  // Sample rate used for the fullband processing.
  int proc_fullband_sample_rate_hz() const
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Empties and handles the respective RuntimeSetting queues.
  void HandleCaptureRuntimeSettings()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);
  void HandleRenderRuntimeSettings() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);

  void EmptyQueuedRenderAudio();
  void AllocateRenderQueue()
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_, crit_capture_);
  void QueueBandedRenderAudio(AudioBuffer* audio)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);
  void QueueNonbandedRenderAudio(AudioBuffer* audio)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);

  // Capture-side exclusive methods possibly running APM in a multi-threaded
  // manner that are called with the render lock already acquired.
  int ProcessCaptureStreamLocked() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Render-side exclusive methods possibly running APM in a multi-threaded
  // manner that are called with the render lock already acquired.
  // TODO(ekm): Remove once all clients updated to new interface.
  int AnalyzeReverseStreamLocked(const float* const* src,
                                 const StreamConfig& input_config,
                                 const StreamConfig& output_config)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);
  int ProcessRenderStreamLocked() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_render_);

  // Collects configuration settings from public and private
  // submodules to be saved as an audioproc::Config message on the
  // AecDump if it is attached.  If not |forced|, only writes the current
  // config if it is different from the last saved one; if |forced|,
  // writes the config regardless of the last saved.
  void WriteAecDumpConfigMessage(bool forced)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Notifies attached AecDump of current configuration and capture data.
  void RecordUnprocessedCaptureStream(const float* const* capture_stream)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  void RecordUnprocessedCaptureStream(const int16_t* const data,
                                      const StreamConfig& config)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Notifies attached AecDump of current configuration and
  // processed capture data and issues a capture stream recording
  // request.
  void RecordProcessedCaptureStream(
      const float* const* processed_capture_stream)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  void RecordProcessedCaptureStream(const int16_t* const data,
                                    const StreamConfig& config)
      RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // Notifies attached AecDump about current state (delay, drift, etc).
  void RecordAudioProcessingState() RTC_EXCLUSIVE_LOCKS_REQUIRED(crit_capture_);

  // AecDump instance used for optionally logging APM config, input
  // and output to file in the AEC-dump format defined in debug.proto.
//  std::unique_ptr<AecDump> aec_dump_;

  // Hold the last config written with AecDump for avoiding writing
  // the same config twice.
//  InternalAPMConfig apm_config_for_aec_dump_ RTC_GUARDED_BY(crit_capture_);

  // Critical sections.
  rtc::CriticalSection crit_render_ RTC_ACQUIRED_BEFORE(crit_capture_);
  rtc::CriticalSection crit_capture_;

  // Struct containing the Config specifying the behavior of APM.
  AudioProcessing::Config config_;

  // Class containing information about what submodules are active.
  SubmoduleStates submodule_states_;

  // Struct containing the pointers to the submodules.
  struct Submodules {
    Submodules(std::unique_ptr<CustomProcessing> capture_post_processor,
               std::unique_ptr<CustomProcessing> render_pre_processor,
               rtc::scoped_refptr<EchoDetector> echo_detector,
               std::unique_ptr<CustomAudioAnalyzer> capture_analyzer)
        : echo_detector(std::move(echo_detector)),
          capture_post_processor(std::move(capture_post_processor)),
          render_pre_processor(std::move(render_pre_processor)),
          capture_analyzer(std::move(capture_analyzer)) {}
    // Accessed internally from capture or during initialization.
    std::unique_ptr<AgcManagerDirect> agc_manager; // agc 控制器
    std::unique_ptr<GainControlImpl> gain_control; // agc 1代
    //std::unique_ptr<GainController2> gain_controller2; // agc 2代
    std::unique_ptr<HighPassFilter> high_pass_filter; // 带通滤波器
    rtc::scoped_refptr<EchoDetector> echo_detector; // 残留回声
    std::unique_ptr<EchoControl> echo_controller; // 回声消除 echo_canceller3
//    std::unique_ptr<EchoControlMobileImpl> echo_control_mobile; //EchoControlMobileImpl
//    std::unique_ptr<NoiseSuppressor> noise_suppressor; //噪声抑制
//    std::unique_ptr<TransientSuppressor> transient_suppressor; //瞬变现象
    std::unique_ptr<CustomProcessing> capture_post_processor;
    std::unique_ptr<CustomProcessing> render_pre_processor;
//    std::unique_ptr<GainApplier> pre_amplifier; //GainApplier 放大器
    std::unique_ptr<CustomAudioAnalyzer> capture_analyzer;
    std::unique_ptr<LevelEstimator> output_level_estimator; //能量 估计 LevelEstimator
//    std::unique_ptr<VoiceDetection> voice_detector; //vad VoiceDetection
  } submodules_;

  // State that is written to while holding both the render and capture locks
  // but can be read without any lock being held.
  // As this is only accessed internally of APM, and all internal methods in APM
  // either are holding the render or capture locks, this construct is safe as
  // it is not possible to read the variables while writing them.
  struct ApmFormatState {
    ApmFormatState()
        :  // Format of processing streams at input/output call sites.
          api_format({{{kSampleRate16kHz, 1, false},
                       {kSampleRate16kHz, 1, false},
                       {kSampleRate16kHz, 1, false},
                       {kSampleRate16kHz, 1, false}}}),
          render_processing_format(kSampleRate16kHz, 1) {}
    ProcessingConfig api_format;
    StreamConfig render_processing_format;
  } formats_;

  // APM constants.
  const struct ApmConstants {
    ApmConstants(bool multi_channel_render_support,
                 bool multi_channel_capture_support,
                 bool enforce_split_band_hpf)
        : multi_channel_render_support(multi_channel_render_support),
          multi_channel_capture_support(multi_channel_capture_support),
          enforce_split_band_hpf(enforce_split_band_hpf) {}
    bool multi_channel_render_support;
    bool multi_channel_capture_support;
    bool enforce_split_band_hpf;
  } constants_;

  struct ApmCaptureState {
    ApmCaptureState();
    ~ApmCaptureState();
    bool was_stream_delay_set;
    bool output_will_be_muted;
    bool key_pressed;
    std::unique_ptr<AudioBuffer> capture_audio;
    std::unique_ptr<AudioBuffer> capture_fullband_audio;
    std::unique_ptr<AudioBuffer> linear_aec_output;
    // Only the rate and samples fields of capture_processing_format_ are used
    // because the capture processing number of channels is mutable and is
    // tracked by the capture_audio_.
    StreamConfig capture_processing_format;
    int split_rate;
    bool echo_path_gain_change;
    int prev_analog_mic_level;
    float prev_pre_amp_gain;
    int playout_volume;
    int prev_playout_volume;
    AudioProcessingStats stats;
    struct KeyboardInfo {
      void Extract(const float* const* data, const StreamConfig& stream_config);
      size_t num_keyboard_frames = 0;
      const float* keyboard_data = nullptr;
    } keyboard_info;
    int cached_stream_analog_level_ = 0;
  } capture_ RTC_GUARDED_BY(crit_capture_);

  struct ApmCaptureNonLockedState {
    ApmCaptureNonLockedState()
        : capture_processing_format(kSampleRate16kHz),
          split_rate(kSampleRate16kHz),
          stream_delay_ms(0) {}
    // Only the rate and samples fields of capture_processing_format_ are used
    // because the forward processing number of channels is mutable and is
    // tracked by the capture_audio_.
    StreamConfig capture_processing_format;
    int split_rate;
    int stream_delay_ms;
    bool echo_controller_enabled = false;
  } capture_nonlocked_;

  struct ApmRenderState {
    ApmRenderState();
    ~ApmRenderState();
    std::unique_ptr<AudioConverter> render_converter;
    std::unique_ptr<AudioBuffer> render_audio;
  } render_ RTC_GUARDED_BY(crit_render_);

  // Class for statistics reporting. The class is thread-safe and no lock is
  // needed when accessing it.
  class ApmStatsReporter {
   public:
    ApmStatsReporter();
    ~ApmStatsReporter();

    // Returns the most recently reported statistics.
    AudioProcessingStats GetStatistics();

    // Update the cached statistics.
    void UpdateStatistics(const AudioProcessingStats& new_stats);

   private:
    rtc::CriticalSection crit_stats_;
    AudioProcessingStats cached_stats_ RTC_GUARDED_BY(crit_stats_);
    SwapQueue<AudioProcessingStats> stats_message_queue_;
  } stats_reporter_;

  std::vector<int16_t> aecm_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<int16_t> aecm_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  size_t agc_render_queue_element_max_size_ RTC_GUARDED_BY(crit_render_)
      RTC_GUARDED_BY(crit_capture_) = 0;
  std::vector<int16_t> agc_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<int16_t> agc_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  size_t red_render_queue_element_max_size_ RTC_GUARDED_BY(crit_render_)
      RTC_GUARDED_BY(crit_capture_) = 0;
  std::vector<float> red_render_queue_buffer_ RTC_GUARDED_BY(crit_render_);
  std::vector<float> red_capture_queue_buffer_ RTC_GUARDED_BY(crit_capture_);

  RmsLevel capture_input_rms_ RTC_GUARDED_BY(crit_capture_);
  RmsLevel capture_output_rms_ RTC_GUARDED_BY(crit_capture_);
  int capture_rms_interval_counter_ RTC_GUARDED_BY(crit_capture_) = 0;

  // Lock protection not needed.
//  std::unique_ptr<
//      SwapQueue<std::vector<int16_t>, RenderQueueItemVerifier<int16_t>>>
//      aecm_render_signal_queue_;
  std::unique_ptr<
      SwapQueue<std::vector<int16_t>, RenderQueueItemVerifier<int16_t>>>
      agc_render_signal_queue_;
  std::unique_ptr<SwapQueue<std::vector<float>, RenderQueueItemVerifier<float>>>
      red_render_signal_queue_;
};

}  // namespace webrtc

#endif  // MODULES_AUDIO_PROCESSING_AUDIO_PROCESSING_IMPL_H_
