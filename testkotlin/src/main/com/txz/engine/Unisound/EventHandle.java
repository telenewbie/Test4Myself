package com.txz.engine.Unisound;

public interface EventHandle {

    interface UNI_VOICE_COMMAND {

        //lasr
        int LASR_BASIC_ID = 0;
        int LASR_ENGINE_MODE = 1;
        int LASR_STD_THRESHOLD = 2;
        int LASR_LP_THRESHOLD = 3;
        int LASR_SKIP_FRAM = 4;
        int LASR_ENGINE_LOG_EN = 5;
        int LASR_STD_MODEL_ID = 6;
        int LASR_DOMAIN = 7;
        int LASR_MODEL_PATH = 8;
        int LASR_TIMEOUT = 9;
        int LASR_WAKEUP_WORD = 10;
        int LASR_NEXT_SCENE = 11;
        int LASR_DATABUF_TIMEMS = 12;
        int LASR_SAVE_RECORD = 13;
        int LASR_SAVE_PATH = 14;
        int LASR_MAX_ID = 100;
        //asr
        int RASR_BISIC_ID = 101;
        int ASR_SIL_TIMEOUT = 102;
        int RASR_APP_KEY = 103;
        int RASR_SEC_KEY = 104;
        int RASR_DOMAIN = 105;
        int RASR_TCP_CON_TPYE = 106;
        int RASR_TCP_TRANSLATE = 107;
        int RASR_OWNER_ID = 108;
        int RASR_DEVICE_ID = 109;
        int RASR_TOKEN = 110;
        int RASR_FILTER_URL = 111;
        int RASR_DP_NAME = 112;
        int RASR_ADDITIONAL_SERVICE = 113;
        int RASR_SCENARIO = 114;
        int RASR_SUB_DOMAIN = 115;
        int RASR_VOICE_FIELD = 116;
        int RASR_NLU_SCENARIO = 117;
        int RASR_CHIP_CLIENT = 118;
        int RASR_NET_TIMEOUT = 119;
        int RASR_PROTOCAL = 120;
        int RASR_PASS_THROUGH = 121;
        int RASR_CODEC = 122;
        int RASR_SESSION_ID = 123;
        int RASR_IP = 124;
        int RASR_PORT = 125;
        int RASR_TIMEOUT = 126;
        int RASR_SAVE_FILE = 127;
        int RASR_SAVE_PATH = 128;
        int RASR_DATABUF_LEN_MS = 129;
        int RASR_SELF_RES = 130;
        int RASR_MAX_ID = 200;
        // preproc
        int PREPROC_BASIC_ID = 201;
        int PREPROC_4MICL_EN = 202;
        int PREPROC_4MICL_AEC_EN = 203;
        int PREPROC_2MIC_EN = 204;
        int PREPROC_RESAMPLE_EN = 205;
        int PREPROC_RECOGN_MODE = 206;
        int PREPROC_TIME_LEN_STAMP = 207;
        int PREPROC_SAVE_FILE = 208;
        int PREPROC_SAVE_PATH = 209;
        int PREPROC_MULTIPLY = 210;
        int PREPROC_MIX_ID = 300;
        /*tts*/
        int TTS_BASIC_ID = 301;
        int TTS_HOST = 302;
        int TTS_PORT = 303;
        int TTS_PARAM_PTR = 304;
        int TTS_PROTOCAL = 305;
        int TTS_CODEC = 306;
        int TTS_APP_KEY = 307;
        int TTS_SEC_KEY = 308;
        int TTS_SPEAKER = 309;
        int TTS_SPD = 310;
        int TTS_VOL = 311;
        int TTS_PIT = 312;
        int TTS_EMT = 313;
        int TTS_SMT = 314;
        int TTS_SAVE_FILE = 315;
        int TTS_SAVE_PATH = 316;
        int TTS_MAX_ID = 400;
        //log
        int LOG_BASIC_ID = 401;
        int LOG_LEVEL = 402;
        int LOG_WAY = 403;
        int LOG_FILE = 404;
        int LOG_MAX_ID = 500;
        int SAV_REC_BASIC_ID = 501;
        int SAV_REC_FLG = 502;       /* 1: save record file 0: do not save record file */
        int SAV_ALS_REC_FLG = 503;   /* 1: save analysis record file 0: do not save analysis record file*/
        int SAV_SED_REC_FLG = 504;   /* 1: save send to engine record file 0: do not save send to engine record file*/
        int SAV_REC_PATH = 505;      /* set save record file path */
        int SAV_REC_MAX_ID = 600;
        //vad
        int VAD_BASIC_ID = 601;
        int VAD_ENABLE = 602;
        int VAD_MAXSILLEN = 603;
        int VAD_USE_LOW_VAD = 604;
        int VAD_ST_ENABLE = 605;
        int VAD_END_ENABLE = 606;
        int VAD_SILENCE_TIMEOUT = 607;
        int VAD_VOICE_TIMEOUT = 608;
        int VAD_PRE_TIME_MS = 609;
        int VAD_MAX_ID = 700;
        //         capture
        int CAPTURE_BASIC_ID = 701;
        int CAPTURE_TIMEOUT = 702;
        int CAPTURE_CUR_SESSION_TIMEMS = 703;
        int CAPTURE_MAX_ID = 800;
        int COMMAND_MAX = 1000;
    }

    /**
     * eventId:事件id，参考UNI_VOICE_COMMAND
     * time:时间（不支持，保留）
     * eventContent:内容
     * eventErrCode:错误妈
     * eventExtendInfo:(不支持，保留)
     * void AsrEventHandle(int eventId,int time,byte[] eventContent,int eventErrCode,long eventExtendInfo);
     */
    void AsrEventHandle(int eventId, int time, byte[] eventContent, int eventErrCode, long eventExtendInfo);

    /**
     * eventId:事件id，参考UNI_VOICE_COMMAND
     * time:时间（不支持，保留）
     * eventContent:内容
     * eventErrCode:错误妈
     * eventExtendInfo:(不支持，保留)
     * void TtsEventHandle(int eventId,int time,byte[] eventContent,int eventErrCode,long eventExtendInfo);
     */
    void TtsEventHandle(int eventId, int time, byte[] eventContent, int eventErrCode, long eventExtendInfo);
}
