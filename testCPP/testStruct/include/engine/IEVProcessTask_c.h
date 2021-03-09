//
//  Created by elevoc on 2019/4/23.
//  Copyright © 2019年 elevoc. All rights reserved.
//

#ifndef IEVPROCESSTASK_C_H
#define IEVPROCESSTASK_C_H

#include "EVProcessEnumType.h"

#ifdef __cplusplus
extern "C" {
#endif

//=================================必要方法 Start========================================
/**
 * 获取降噪处理的对象实例
 * @param appid 渠道标志
 * @param secreatKey 渠道密钥
 * @return
 */
void *elevoc_new_evtask(const char *appid, const char *secreatKey);

/**
 * 释放降噪处理的对象实例
 * @param evTask
 * @return
 */
bool elevoc_delete_evtask(void *evTask);

/**
* 设置错误回调函数
* @param failCallBack 接收错误处理的函数指针
* @return true for success
*/
bool elevoc_set_error_callback(void *evTask, processFailCallBack failCallBack);

/**
 * 设置认证成功回调函数
 * @param evTask
 * @param successCallBack 接收成功消息的函数指针
 * @return true for success
 */
bool elevoc_set_success_callback(void *evTask, oauthSuccessCallBack successCallBack);

/*
 * 若不是自动认证，则需要主动调用to_verify去进行认证
 */
void elevoc_to_verify(void *evTask);

/**
 * 开启降噪任务
 * @param evTask
 * @return
 */
bool elevoc_start_task(void *evTask);


/**
 * 写入原始数据
 * @param evTask
 * @param audioData 数据指针
 * @param len 数据长度（byte）
 * @return
 */
bool elevoc_write_audio(void *evTask, const char *audioData, size_t len);


/**
 * 写入远端数据，仅当SDK支持AEC时才可使用此接口
 * @param evTask
 * @param audioData 数据指针
 * @param len 数据长度（byte）
 * @return
 */
bool elevoc_write_farend_audio(void *evTask, const char *audioData, size_t len);

/**
 * 获取降噪后的数据
 * @param evTask
 * @param buffer 数据指针
 * @param len 读取的长度
 * @param isLast 是否为最后一帧（如离线处理需要在最后一帧做一些关闭文件等操作）
 * @param isPaddingZero 是否在SDK处理已数据长度不足时padding 0，如为实时语音通话场景，建议设为true
 * @return
 */
size_t elevoc_read_buffer(void *evTask, char *buffer, size_t len, bool *isLast, bool isPaddingZero = true);


/**
 * 停止降噪
 * @param evTask
 * @param stopImmediately 是否立即停止（将丢弃队列中还未处理的数据，实时语音通话场景建议为true）
 * @return
 */
bool elevoc_stop_task(void *evTask, bool stopImmediately = true);


/**
 * 获取SDK的版本号
 * @param evTask
 * @return
 */
const char *elevoc_lib_ver(void *evTask);


//=================================必要方法 End========================================


//=================================配置参数 Start======================================

/*
* 加载模型
* model path
*/
void elevoc_load_model(void *evTask, const char *model_path);

/**
 * 加载配置文件
 * @param evTask
 * @param path
 * @return
 */
bool elevoc_load_config(void *evTask, const char *path);

/**
 * 加载参数
 */
bool elevoc_load_params(void *evTask, const char *content);

/**
 * 设置降噪比例
 * @param evTask
 * @param value 降噪比例 (0~1.0)
 * @return
 */
bool elevoc_set_mask_value(void *evTask, float value);

/**
 * 设置最大降噪db (-40db~0db)
 * @param evTask
 * @param db (-40db - 0db) default is -40db
 */
bool elevoc_set_noise_suppress_db(void *evTask, int db);
    
    
/**
 * 设置增益值
 * @param evTask
 * @param value 增益比例 (1~50)
 * @return
 */
bool elevoc_set_gain_value(void *evTask, int value);

/**
 * 设置是否开启降噪
 * @param evTask
 * @param enable
 * @return
 */
bool elevoc_set_enable_process(void *evTask, bool enable);

/**
 * 设置是否启用log日志，开启日志有助于定位问题
 * @param evTask
 * @param enableLog
 * @return
 */
bool elevoc_set_enable_log(void *evTask, bool enableLog);


/**
 * 设置log的文件指针，存储到文件
 * @param evTask
 * @param fp
 */
bool elevoc_set_file_log_handle(void *evTask, FILE *fp);

/**
 * 设置使用场景，如为VOIP，需设置为VOIP
 * @param evTask
 * @param mode Default/VOIP
 * @return
 */
bool elevoc_set_scene_mode(void *evTask, MODE mode);

/**
 * 设置dump语音数据的目录
 * @param evTask
 * @param dir
 */
void elevoc_set_dump_audio_dir(void *evTask, const char *dir);

/**
 * 设置是否开启语音数据dump, 默认是关闭
 * @param evTask
 * @param enable
 */
void elevoc_set_enable_dump_audio(void *evTask, bool enable);

/**
 * 设置network license save dir
 */
void elevoc_set_network_license_save_dir(void *evTask, const char *dir);

/*
 * check license
*/
bool elevoc_check_license(void *evTask, const char *license);

/**
* 设置采样率
@param sampleRate 8000 or 16000 or 32000 or 48000
*/
void elevoc_set_samplerate(void *evTask, int sampleRate);

/*
* 设置通道数 (1 or 2), 默认是单通道
*/
void elevoc_set_channels(void *evTask, int channels);


/**
 * 设置UUID for network request
 */
void elevoc_set_request_uuid(void *evTask, const char *uuid);

/**
 * 设置设备id和包名 for local license save
 */
bool elevoc_set_device_uuid_and_pkgname(void *evTask, const char *uuid, const char *pkgname);

/**
 * 设置网络状态 true for network work
 */
bool elevoc_set_network_status(void *evTask, bool status);

/**
 * @param evTask
 * @param mode HAND_SET/HAND_FREE
 * @return
 */
void elevoc_set_device_voice_mode(void *evTask, DEVICE_VOICE_MODE mode);

/**
 * 设置cacert文件路径（仅当需要开启网络认证时, 注意中文路径下需要进行ansi编码!!!）
 * @param evTask
 * @param capath cacert的路径
 * @return
 */
bool elevoc_set_cacert_path(void *evTask, const char *capath);

/**
 * 设置vad callback
 */
void elevoc_set_vad_callback(void *evTask, vadStartCallBack startBlock,vadStopCallBack stopBlock);

/**
 * 设置vad frameThreshold, 默认值0.9, 范围(0~1)
 * 值越大语音检测条件越苛刻.
 */
void elevoc_set_vad_frame_threshold(void *evTask, float frameThreshold);

/**
 *
 * 设置vad检测start最小时间间隔 默认(8k采样率是1s)
 */
void elevoc_set_vad_min_no_speech_time(void *evTask, float minNoSpeechTime);

/**
 *
 * 设置vad平滑窗长，默认是20，窗越长start事件检测越慢
 */
void elevoc_set_vad_smooth_window_len(void *evTask, int wlen);

/**
 *
 * 设置vad 最短语音结束时长 (单位是秒，默认值是 0.3s)
 */
void elevoc_set_vad_min_speech_dissolve_time(void *evTask, float minSpeechDissolveTime);

/**
 *	
 *	设置语音端点的能量阈值 (默认值为8.0)
 */
void elevoc_set_vad_speech_energy_threshold(void *evTask, float speechEnergyThreshold);
    
/**
 *
 *  当vad_start事件回调时，获取vad回溯的语音(最大15帧)
 */
size_t elevoc_fetch_vad_pre_buffer(void *evTask, char *buffer, size_t len);

//=================================配置参数 End======================================


/**
 * 设置是否启用啸叫检测（与降噪互斥）
 * @param evTask
 * @param enable
 * @return
 */
bool elevoc_set_enable_howling(void *evTask, bool enable);

/**
 * 设置啸叫检测回调，用于获得啸叫/非啸叫时间
 * @param evTask
 * @param howlingCallBack
 * @return
 */
bool elevoc_set_howling_detect_callback(void *evTask, howlingDetectCallBack howlingCallBack);

/**
 * 获取截至目前啸叫与非啸叫时长，单位s
 * @param evTask
 * @param howlingTimes 啸叫时长
 * @param unHowlingTimes 未啸叫时长
 * @return
 */
bool elevoc_get_howling_state(void *evTask, int *howlingTimes, int *unHowlingTimes);

#ifdef __cplusplus
}
#endif

#endif //NNSS_DEMO_IEVPROCESSTASK_C_H
