//
// Created by Elevoc on 2020/11/21 0021.
//
#include <iostream>
#include <fstream>

#include "speex_resampler.h"
#include "my_log.h"


using namespace std;

#define SAMPLE_RATE_441k 44100
#define SAMPLE_RATE_48k 48000
#define IN_CHANNEL 1
#define IN_BIT 16

#define NAME "44100_c1_b16"
#define FILE_NAME (NAME ".pcm")
#define OUT_FILE_NAME (NAME "_out.pcm")
#define OUT_REBACK_FILE_NAME (NAME "_out_back.pcm")

int resample(int oSampleRate, int dSampleRate, char *readFileName, char *writeFileName, int pre_frame_size) {
    cout << readFileName << "::" << writeFileName << endl;
    int err = 0;
    SpeexResamplerState *resampler44t48_in = elevoc_resampler_init(IN_CHANNEL, oSampleRate, dSampleRate, 9, &err);
    if (resampler44t48_in != NULL) {
        elevoc_resampler_set_rate(resampler44t48_in, oSampleRate, dSampleRate);
    } else {
        LOGE("resampler error 44to48 in");
    }

    FILE *fp = fopen(readFileName, "rb");
    FILE *fpw = fopen(writeFileName, "wb+");
    if (!fp) { exit(-1); }
    if (!fpw) { exit(-1); }

    if (pre_frame_size <= 0) { exit(-1); }

    // 每次 读取 (采样率 * channel * bit /8 )* 10 /1000
    char *input = (char *) malloc(pre_frame_size);
    memset(input, 0, pre_frame_size);
    char *output = (char *) malloc(pre_frame_size * 2);
    memset(output, 0, pre_frame_size * 2);
    // 读够 指定的字节数
    int count = 0;
    do {
        int reminder = pre_frame_size;
        while ((count = fread(input+(pre_frame_size-reminder), 1, reminder, fp)) > 0) {
            reminder -= count;
            if (reminder == 0) {
                break;
            }
            cout << "reminder:::" << reminder << ":::count:" << count << endl;
        }
        cout << reminder << ":::count:" << count << endl;
        if (reminder == pre_frame_size && count <= 0) {
            break;
        }
        unsigned int in_len = (pre_frame_size - reminder)/2;
        unsigned int out_len = pre_frame_size;
        cout << "in_len=" << in_len << ",out_len=" << out_len << endl;
        elevoc_resampler_process_int(resampler44t48_in, 0, (short *) input, &in_len, (short *) output, &out_len);
        cout << "out_len=" << out_len << endl;
        printf("\n");
        size_t writelen = fwrite(output, 2, out_len, fpw);
        cout << out_len << ":::" << writelen << endl;
    } while (true);

    free(input);
    free(output);
    fclose(fp);
    fclose(fpw);
    if (resampler44t48_in != NULL) {
        elevoc_resampler_destroy(resampler44t48_in);
    }
    return 0;
}

int main() {
    resample(SAMPLE_RATE_441k, SAMPLE_RATE_48k, FILE_NAME, OUT_FILE_NAME,
             ((SAMPLE_RATE_441k * IN_CHANNEL * IN_BIT / 8) * 10 / 1000));
    resample(SAMPLE_RATE_48k, SAMPLE_RATE_441k, OUT_FILE_NAME, OUT_REBACK_FILE_NAME, ((SAMPLE_RATE_48k*IN_CHANNEL*IN_BIT/8)*10/1000));
}