#ifndef		    Elevoc_NS_H
#define		    Elevoc_NS_H
#include		"ElevocTypeDef.h"
#include        "ElevocNSDefine.h"

#define        MIN_HB_PSD_NOISE        2e-5f
#define        MIN_LB_PSD_NOISE        1e-6f

/* ==========================================================*/
/* ================ ns constant define ======================*/

#define        NB_DNN_SIZE               80
#define        WB_DNN_SIZE               160
#define        SWB_DNN_SIZE              160
#define        FB_DNN_SIZE               160

#define        NB_PSD_NUM                80
#define        WB_PSD_NUM                160
#define        SWB_PSD_NUM               192
#define        FB_PSD_NUM                200

#define        MAX_FREQ_SIZE             480
#define        MAX_DNN_FREQ_SIZE         161

#define        MAX_HB_PSD_SIZE           40
#define        MAX_BARK_SIZE             49

#define        NB_FREQ_SIZE              80
#define        WB_FREQ_SIZE              160
#define        SWB_FREQ_SIZE             320
#define        FB_FREQ_SIZE              480
/* ==========================================================*/
/* Elevoc NS algorithm struct information */


typedef struct _NS_PARAM_LSTM
{
    int              version;                 //    -ro    default:0
    short            enable;                  //    -rw    default:1
    short            state;                   //    -ro    default:0
    /*--------------- mode bit define -----------------------
     bit5     bit4      bit3     bit2    bit1     bit0
     HB_Mute | AFNS_EN  | AGC_En | DRC_En | PF_En | DNN_En
     ------------------------------------------------------ */
    short            mode;                    //    -rw    default:0x07
    short            gain_Q12;                //    -rw    default:0x
    short            floor_Q15[4];            //    -rw    default:0x140
    
    /* test mode */
    short            tst_vth;                 //    -rw    default:0x08
    short            tst_frm;                 //    -rw    default:0x50
    short            tst_hold;                //    -rw    default:0x190
    
    /* drc mod information */
    short            drc_vth;                 //    -rw    default:0x10
    short            drc_ratio_Q12;           //    -rw    default:0x3000
    short            drc_gain_Q15;            //    -rw    default:0x2000
    
    /* dnn information */
    struct     DNN_PARAM
    {
        int          dnn_version;             //    -ro    default:0x2000
        short        dnn_state;               //    -ro    default:0x0
        short        dnn_model_ix;            //    -rw    default:0x0
        short        dnn_scaleLB_Q14;         //    -rw    default:0x4000
        short        dnn_scaleHB_Q14;         //    -rw    default:0x4000
        short        dnn_scaleMin_Q14;        //    -rw    default:0x4000
        short        dnn_snrRatio_Q15;        //    -rw    default:0x7333
        short        dnn_snrMin_Q8;           //    -rw    default:0
        short        dnn_snrMax_Q8;           //    -rw    default:0x1e00
        short        dnn_rev[6];
    }dnn_param;
    //        DNN_PARAM        dnn_param;
    
    /* ns information */
    short            MCRA_rangeHB;            //    -rw    default:0xc8
    short            MCRA_alphaHB_Q15;        //    -rw    default:0x6666
    short            MCRA_gammaHB_Q12;        //    -rw    default:0x2000
    short            MCRA_rev;
    
    short            PF_scaleLB_Q14;          //    -rw    default:0x4000
    short            PF_scaleHB_Q14;          //    -rw    default:0x4000
    short            PF_alphaLB_Q15;          //    -rw    default:0x1999
    short            PF_alphaHB_Q15;          //    -rw    default:0x1999
    
    short            MMSE_alphaHB_Q15;        //    -rw    default:0xccd
    short            MMSE_gammaHB_Q12;        //    -rw    default:0x1000
    short            MMSE_ratioHB_Q14;        //    -rw    default:0x4000
    short            MMSE_scaleHB_Q14;        //    -rw    default:0x4000
    
    short            PF_vb_Q15;               //    -rw    default:0x0ccd
    short            PF_vmu_Q12;              //    -rw    default:0x1333
    short            PF_fbLB_Q15[2];          //    -rw    default:0x1999
    short            PF_fmuLB_Q12[2];         //    -rw    default:0x1333
    short            PF_fbHB_Q15;             //    -rw    default:0x1999
    short            PF_fmuHB_Q12;            //    -rw    default:0x1333
    short            ns_rev[12];
}NS_PARAM_LSTM;


typedef struct _Elevoc_NS
{
    NS_PARAM_LSTM     p_param;
    int               freq_size;
    int               LB_psd_size;
    int               HB_psd_size;
    int               LB_bark_size;
    
    unsigned int      sample_rate;
    int               band_mode;
    
    int               tst_state;
    int               tst_frm;
    int               mic_vad_frm;
    int               mic_sil_frm;
    int               mic_state;
    float             mic_Smin;
    float             mic_noise;
    
    int               drc_cnt;
    float             drc_noise;
    float             drc_lr;
    float             drc_gain;
    
    int               frm_cnt;
    int               MCRA_cnt;
    
    float             pFrame;
    float             noiseLB_ratio;
    
    float             LT_noiseLB;
    float             LT_vocalLB;
    float             LT_snrLB;
    float             floorHB;
    
    float             ns_filter[MAX_FREQ_SIZE];
    float             gain_floor[WB_FREQ_SIZE];
    
    float             feat[MAX_DNN_FREQ_SIZE];
    float             psd_weight[2];
    float             psd_acc[2];
    
    /* low-band buffer */
    float             dnn_mask[WB_FREQ_SIZE];
    float             psd_LB[WB_FREQ_SIZE];
    float             zeta_LB[WB_FREQ_SIZE];
    
    /* high-band denoise buffer */
    float             psd_HB[MAX_HB_PSD_SIZE];         // psd_HB[HB_psd_size]
    float             psd_vocalHB[MAX_HB_PSD_SIZE];    // psd_vocalHB[HB_psd_size]
    float             psd_noiseHB[MAX_HB_PSD_SIZE];    // psd_noiseHB[HB_psd_size]
    float             zeta_HB[MAX_HB_PSD_SIZE];        // zeta_HB[HB_psd_size]
    
    float             S_HB[MAX_HB_PSD_SIZE];           // S_HB[HB_psd_size]
    float             Smin_HB[MAX_HB_PSD_SIZE];        // Smin_HB[HB_psd_size]
    float             Stmp_HB[MAX_HB_PSD_SIZE];        // Stmp_HB[HB_psd_size]
}Elevoc_NS;

#ifdef __cplusplus
extern "C" {
#endif

void    Elevoc_NS_reset(Elevoc_NS *p_NS, unsigned int sample_rate);
void    Elevoc_NS_set_dnn_param(Elevoc_NS *p_NS, NSParamType type,  float param);
void    Elevoc_NS_process(Elevoc_NS *p_NS, ele_complex_float *fft_in, float *mask);
    
#ifdef __cplusplus
}
#endif

#endif
