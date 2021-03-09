//
//  Elevoc_NS_Constant.h
//  iosdnn
//
//  Created by elevoc on 2019/3/7.
//  Copyright © 2019 蔡鹏. All rights reserved.
//

#ifndef Elevoc_NS_Constant_h
#define Elevoc_NS_Constant_h

static const int Bark_band_size_ns[4] = {32,40,46,49};

static const int Bark_left_ix_ns[480] = {
    0,   0,   0,   1,   2,   3,   3,   4,   5,   6,
    7,   8,   8,   9,  10,  11,  11,  12,  13,  13,
    14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
    19,  19,  20,  20,  21,  21,  21,  22,  22,  22,
    23,  23,  23,  24,  24,  24,  24,  25,  25,  25,
    25,  26,  26,  26,  26,  26,  27,  27,  27,  27,
    27,  28,  28,  28,  28,  28,  28,  29,  29,  29,
    29,  29,  29,  30,  30,  30,  30,  30,  30,  30,
    31,  31,  31,  31,  31,  31,  31,  31,  32,  32,
    32,  32,  32,  32,  32,  33,  33,  33,  33,  33,
    33,  33,  33,  33,  34,  34,  34,  34,  34,  34,
    34,  34,  34,  35,  35,  35,  35,  35,  35,  35,
    35,  35,  35,  36,  36,  36,  36,  36,  36,  36,
    36,  36,  36,  36,  37,  37,  37,  37,  37,  37,
    37,  37,  37,  37,  37,  37,  37,  38,  38,  38,
    38,  38,  38,  38,  38,  38,  38,  38,  38,  38,
    39,  39,  39,  39,  39,  39,  39,  39,  39,  39,
    39,  39,  39,  39,  39,  39,  39,  40,  40,  40,
    40,  40,  40,  40,  40,  40,  40,  40,  40,  40,
    40,  40,  40,  40,  40,  40,  41,  41,  41,  41,
    41,  41,  41,  41,  41,  41,  41,  41,  41,  41,
    41,  41,  41,  41,  41,  41,  41,  41,  41,  42,
    42,  42,  42,  42,  42,  42,  42,  42,  42,  42,
    42,  42,  42,  42,  42,  42,  42,  42,  42,  42,
    42,  42,  42,  42,  42,  42,  42,  43,  43,  43,
    43,  43,  43,  43,  43,  43,  43,  43,  43,  43,
    43,  43,  43,  43,  43,  43,  43,  43,  43,  43,
    43,  43,  43,  43,  43,  43,  43,  43,  43,  43,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47};

static const float Bark_filter_left_ns[480] = {
    0.0000000f,0.4513224f,0.6348936f,0.7256268f,0.8232305f,0.9299438f,0.0478605f,0.1789029f,0.3248029f,0.4870876f,
    0.6670725f,0.8658598f,0.0843415f,0.3232075f,0.5829568f,0.8639116f,0.1662327f,0.4899363f,0.8349112f,0.2009358f,
    0.5876941f,0.9947914f,0.4217684f,0.8681143f,0.3332787f,0.8166814f,0.3177219f,0.8357874f,0.3702588f,0.9205169f,
    0.4859471f,0.0659428f,0.6599089f,0.2672642f,0.8874432f,0.5198974f,0.1640966f,0.8195294f,0.4857036f,0.1621465f,
    0.8484044f,0.5440431f,0.2486469f,0.9618187f,0.6831792f,0.4123665f,0.1490353f,0.8928567f,0.6435171f,0.4007181f,
    0.1641752f,0.9336178f,0.7087882f,0.4894411f,0.2753433f,0.0662726f,0.8620177f,0.6623773f,0.4671601f,0.2761838f,
    0.0892752f,0.9062690f,0.7270081f,0.5513430f,0.3791311f,0.2102368f,0.0445310f,0.8818905f,0.7221981f,0.5653422f,
    0.4112163f,0.2597192f,0.1107542f,0.9642292f,0.8200565f,0.6781525f,0.5384375f,0.4008355f,0.2652740f,0.1316841f,
    1.0000000f,0.8530579f,0.7081339f,0.5651634f,0.4240849f,0.2848399f,0.1473725f,0.0116293f,0.8775597f,0.7451152f,
    0.6142499f,0.4849196f,0.3570827f,0.2306992f,0.1057311f,0.9821423f,0.8598982f,0.7389660f,0.6193144f,0.5009137f,
    0.3837354f,0.2677526f,0.1529394f,0.0392714f,0.9267253f,0.8152787f,0.7049104f,0.5956004f,0.4873293f,0.3800787f,
    0.2738313f,0.1685702f,0.0642797f,0.9609444f,0.8585500f,0.7570825f,0.6565287f,0.5568759f,0.4581120f,0.3602254f,
    0.2632050f,0.1670399f,0.0717201f,0.9772356f,0.8835769f,0.7907349f,0.6987007f,0.6074658f,0.5170220f,0.4273612f,
    0.3384757f,0.2503580f,0.1630008f,0.0763968f,0.9905393f,0.9054212f,0.8210361f,0.7373774f,0.6544387f,0.5722137f,
    0.4906963f,0.4098803f,0.3297597f,0.2503287f,0.1715814f,0.0935119f,0.0161145f,0.9393836f,0.8633135f,0.7878986f,
    0.7131333f,0.6390120f,0.5655293f,0.4926796f,0.4204575f,0.3488576f,0.2778743f,0.2075023f,0.1377362f,0.0685705f,
    1.0000000f,0.9356342f,0.8718216f,0.8085573f,0.7458362f,0.6836532f,0.6220032f,0.5608813f,0.5002824f,0.4402016f,
    0.3806338f,0.3215740f,0.2630172f,0.2049586f,0.1473931f,0.0903159f,0.0337219f,0.9776064f,0.9219643f,0.8667909f,
    0.8120813f,0.7578306f,0.7040341f,0.6506870f,0.5977845f,0.5453219f,0.4932944f,0.4416974f,0.3905262f,0.3397762f,
    0.2894427f,0.2395212f,0.1900071f,0.1408960f,0.0921833f,0.0438645f,0.9959353f,0.9483912f,0.9012279f,0.8544411f,
    0.8080265f,0.7619798f,0.7162967f,0.6709733f,0.6260052f,0.5813883f,0.5371187f,0.4931922f,0.4496048f,0.4063527f,
    0.3634318f,0.3208384f,0.2785685f,0.2366183f,0.1949841f,0.1536621f,0.1126487f,0.0719402f,0.0315330f,0.9914235f,
    0.9516082f,0.9120836f,0.8728462f,0.8338926f,0.7952194f,0.7568233f,0.7187010f,0.6808492f,0.6432647f,0.6059442f,
    0.5688848f,0.5320831f,0.4955362f,0.4592410f,0.4231945f,0.3873937f,0.3518357f,0.3165176f,0.2814366f,0.2465898f,
    0.2119744f,0.1775877f,0.1434269f,0.1094894f,0.0757726f,0.0422737f,0.0089902f,0.9759195f,0.9430592f,0.9104067f,
    0.8779596f,0.8457155f,0.8136720f,0.7818266f,0.7501771f,0.7187212f,0.6874566f,0.6563810f,0.6254923f,0.5947882f,
    0.5642666f,0.5339254f,0.5037625f,0.4737758f,0.4439632f,0.4143228f,0.3848525f,0.3555504f,0.3264145f,0.2974430f,
    0.2686339f,0.2399854f,0.2114956f,0.1831628f,0.1549850f,0.1269607f,0.0990880f,0.0713651f,0.0437905f,0.0163624f,
    0.9890792f,0.9619392f,0.9349409f,0.9080826f,0.8813627f,0.8547799f,0.8283324f,0.8020188f,0.7758376f,0.7497874f,
    0.7238666f,0.6980740f,0.6724080f,0.6468672f,0.6214504f,0.5961561f,0.5709830f,0.5459298f,0.5209951f,0.4961778f,
    0.4714764f,0.4468899f,0.4224169f,0.3980562f,0.3738066f,0.3496669f,0.3256360f,0.3017127f,0.2778958f,0.2541843f,
    0.2305769f,0.2070727f,0.1836705f,0.1603692f,0.1371679f,0.1140654f,0.0910607f,0.0681528f,0.0453407f,0.0226234f,
    1.0000000f,0.9763120f,0.9527207f,0.9292250f,0.9058239f,0.8825166f,0.8593020f,0.8361791f,0.8131472f,0.7902052f,
    0.7673522f,0.7445874f,0.7219099f,0.6993187f,0.6768130f,0.6543921f,0.6320549f,0.6098008f,0.5876289f,0.5655383f,
    0.5435283f,0.5215980f,0.4997468f,0.4779738f,0.4562782f,0.4346594f,0.4131165f,0.3916489f,0.3702558f,0.3489364f,
    0.3276901f,0.3065163f,0.2854141f,0.2643829f,0.2434220f,0.2225308f,0.2017086f,0.1809548f,0.1602686f,0.1396496f,
    0.1190969f,0.0986101f,0.0781886f,0.0578316f,0.0375386f,0.0173090f,0.9971423f,0.9770378f,0.9569949f,0.9370132f,
    0.9170921f,0.8972309f,0.8774292f,0.8576864f,0.8380019f,0.8183754f,0.7988061f,0.7792937f,0.7598376f,0.7404372f,
    0.7210922f,0.7018020f,0.6825661f,0.6633841f,0.6442554f,0.6251796f,0.6061563f,0.5871849f,0.5682651f,0.5493963f,
    0.5305781f,0.5118101f,0.4930918f,0.4744229f,0.4558029f,0.4372313f,0.4187078f,0.4002319f,0.3818033f,0.3634214f,
    0.3450861f,0.3267967f,0.3085530f,0.2903546f,0.2722010f,0.2540920f,0.2360271f,0.2180059f,0.2000282f,0.1820935f,
    0.1642014f,0.1463517f,0.1285440f,0.1107778f,0.0930530f,0.0753691f,0.0577258f,0.0401228f,0.0225598f,0.0050364f,
    0.9875522f,0.9701071f,0.9527006f,0.9353325f,0.9180025f,0.9007102f,0.8834553f,0.8662375f,0.8490567f,0.8319123f,
    0.8148043f,0.7977322f,0.7806958f,0.7636948f,0.7467289f,0.7297979f,0.7129015f,0.6960394f,0.6792114f,0.6624171f,
    0.6456563f,0.6289288f,0.6122344f,0.5955726f,0.5789434f,0.5623464f,0.5457814f,0.5292482f,0.5127465f,0.4962760f,
    0.4798366f,0.4634280f,0.4470500f,0.4307023f,0.4143848f,0.3980971f,0.3818391f,0.3656105f,0.3494111f,0.3332408f,
    0.3170992f,0.3009863f,0.2849016f,0.2688452f,0.2528167f,0.2368159f,0.2208427f,0.2048968f,0.1889781f,0.1730863f,
    0.1572212f,0.1413827f,0.1255706f,0.1097846f,0.0940246f,0.0782904f,0.0625818f,0.0468987f,0.0312408f,0.0156079f};

static const int Bark_right_ix_ns[480] = {
    0,   0,   1,   2,   3,   4,   4,   5,   6,   7,
    8,   9,   9,  10,  11,  12,  12,  13,  14,  14,
    15,  16,  16,  17,  17,  18,  18,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  23,
    24,  24,  24,  25,  25,  25,  25,  26,  26,  26,
    26,  27,  27,  27,  27,  27,  28,  28,  28,  28,
    28,  29,  29,  29,  29,  29,  29,  30,  30,  30,
    30,  30,  30,  31,  31,  31,  31,  31,  31,  31,
    32,  32,  32,  32,  32,  32,  32,  32,  33,  33,
    33,  33,  33,  33,  33,  34,  34,  34,  34,  34,
    34,  34,  34,  34,  35,  35,  35,  35,  35,  35,
    35,  35,  35,  36,  36,  36,  36,  36,  36,  36,
    36,  36,  36,  37,  37,  37,  37,  37,  37,  37,
    37,  37,  37,  37,  38,  38,  38,  38,  38,  38,
    38,  38,  38,  38,  38,  38,  38,  39,  39,  39,
    39,  39,  39,  39,  39,  39,  39,  39,  39,  39,
    40,  40,  40,  40,  40,  40,  40,  40,  40,  40,
    40,  40,  40,  40,  40,  40,  40,  41,  41,  41,
    41,  41,  41,  41,  41,  41,  41,  41,  41,  41,
    41,  41,  41,  41,  41,  41,  42,  42,  42,  42,
    42,  42,  42,  42,  42,  42,  42,  42,  42,  42,
    42,  42,  42,  42,  42,  42,  42,  42,  42,  43,
    43,  43,  43,  43,  43,  43,  43,  43,  43,  43,
    43,  43,  43,  43,  43,  43,  43,  43,  43,  43,
    43,  43,  43,  43,  43,  43,  43,  44,  44,  44,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    44,  44,  44,  44,  44,  44,  44,  44,  44,  44,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    45,  45,  45,  45,  45,  45,  45,  45,  45,  45,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
    46,  46,  46,  46,  46,  46,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
    48,  48,  48,  48,  48,  48,  48,  48,  48,  48,
    48,  48,  48,  48,  48,  48,  48,  48,  48,  48,
    48,  48,  48,  48,  48,  48,  48,  48,  48,  48,
    48,  48,  48,  48,  48,  48,  48,  48,  48,  48,
    48,  48,  48,  48,  48,  48,  48,  48,  48,  48,
    48,  48,  48,  48,  48,  48,  48,  48,  48,  48};

static const float Bark_filter_right_ns[480] = {
    0.0000000f,0.0000000f,0.3651064f,0.2743732f,0.1767695f,0.0700562f,0.9521395f,0.8210971f,0.6751971f,0.5129124f,
    0.3329275f,0.1341402f,0.9156585f,0.6767925f,0.4170432f,0.1360884f,0.8337673f,0.5100637f,0.1650888f,0.7990642f,
    0.4123059f,0.0052086f,0.5782316f,0.1318857f,0.6667213f,0.1833186f,0.6822781f,0.1642126f,0.6297412f,0.0794831f,
    0.5140529f,0.9340572f,0.3400911f,0.7327358f,0.1125568f,0.4801026f,0.8359034f,0.1804706f,0.5142964f,0.8378535f,
    0.1515956f,0.4559569f,0.7513531f,0.0381813f,0.3168208f,0.5876335f,0.8509647f,0.1071433f,0.3564829f,0.5992819f,
    0.8358248f,0.0663822f,0.2912118f,0.5105589f,0.7246567f,0.9337274f,0.1379823f,0.3376227f,0.5328399f,0.7238162f,
    0.9107248f,0.0937310f,0.2729919f,0.4486570f,0.6208689f,0.7897632f,0.9554690f,0.1181095f,0.2778019f,0.4346578f,
    0.5887837f,0.7402808f,0.8892458f,0.0357708f,0.1799435f,0.3218475f,0.4615625f,0.5991645f,0.7347260f,0.8683159f,
    0.0000000f,0.1469421f,0.2918661f,0.4348366f,0.5759151f,0.7151601f,0.8526275f,0.9883707f,0.1224403f,0.2548848f,
    0.3857501f,0.5150804f,0.6429173f,0.7693008f,0.8942689f,0.0178577f,0.1401018f,0.2610340f,0.3806856f,0.4990863f,
    0.6162646f,0.7322474f,0.8470606f,0.9607286f,0.0732747f,0.1847213f,0.2950896f,0.4043996f,0.5126707f,0.6199213f,
    0.7261687f,0.8314298f,0.9357203f,0.0390556f,0.1414500f,0.2429175f,0.3434713f,0.4431241f,0.5418880f,0.6397746f,
    0.7367950f,0.8329601f,0.9282799f,0.0227644f,0.1164231f,0.2092651f,0.3012993f,0.3925342f,0.4829780f,0.5726388f,
    0.6615243f,0.7496420f,0.8369992f,0.9236032f,0.0094607f,0.0945788f,0.1789639f,0.2626226f,0.3455613f,0.4277863f,
    0.5093037f,0.5901197f,0.6702403f,0.7496713f,0.8284186f,0.9064881f,0.9838855f,0.0606164f,0.1366865f,0.2121014f,
    0.2868667f,0.3609880f,0.4344707f,0.5073204f,0.5795425f,0.6511424f,0.7221257f,0.7924977f,0.8622638f,0.9314295f,
    0.0000000f,0.0643658f,0.1281784f,0.1914427f,0.2541638f,0.3163468f,0.3779968f,0.4391187f,0.4997176f,0.5597984f,
    0.6193662f,0.6784260f,0.7369828f,0.7950414f,0.8526069f,0.9096841f,0.9662781f,0.0223936f,0.0780357f,0.1332091f,
    0.1879187f,0.2421694f,0.2959659f,0.3493130f,0.4022155f,0.4546781f,0.5067056f,0.5583026f,0.6094738f,0.6602238f,
    0.7105573f,0.7604788f,0.8099929f,0.8591040f,0.9078167f,0.9561355f,0.0040647f,0.0516088f,0.0987721f,0.1455589f,
    0.1919735f,0.2380202f,0.2837033f,0.3290267f,0.3739948f,0.4186117f,0.4628813f,0.5068078f,0.5503952f,0.5936473f,
    0.6365682f,0.6791616f,0.7214315f,0.7633817f,0.8050159f,0.8463379f,0.8873513f,0.9280598f,0.9684670f,0.0085765f,
    0.0483918f,0.0879164f,0.1271538f,0.1661074f,0.2047806f,0.2431767f,0.2812990f,0.3191508f,0.3567353f,0.3940558f,
    0.4311152f,0.4679169f,0.5044638f,0.5407590f,0.5768055f,0.6126063f,0.6481643f,0.6834824f,0.7185634f,0.7534102f,
    0.7880256f,0.8224123f,0.8565731f,0.8905106f,0.9242274f,0.9577263f,0.9910098f,0.0240805f,0.0569408f,0.0895933f,
    0.1220404f,0.1542845f,0.1863280f,0.2181734f,0.2498229f,0.2812788f,0.3125434f,0.3436190f,0.3745077f,0.4052118f,
    0.4357334f,0.4660746f,0.4962375f,0.5262242f,0.5560368f,0.5856772f,0.6151475f,0.6444496f,0.6735855f,0.7025570f,
    0.7313661f,0.7600146f,0.7885044f,0.8168372f,0.8450150f,0.8730393f,0.9009120f,0.9286349f,0.9562095f,0.9836376f,
    0.0109208f,0.0380608f,0.0650591f,0.0919174f,0.1186373f,0.1452201f,0.1716676f,0.1979812f,0.2241624f,0.2502126f,
    0.2761334f,0.3019260f,0.3275920f,0.3531328f,0.3785496f,0.4038439f,0.4290170f,0.4540702f,0.4790049f,0.5038222f,
    0.5285236f,0.5531101f,0.5775831f,0.6019438f,0.6261934f,0.6503331f,0.6743640f,0.6982873f,0.7221042f,0.7458157f,
    0.7694231f,0.7929273f,0.8163295f,0.8396308f,0.8628321f,0.8859346f,0.9089393f,0.9318472f,0.9546593f,0.9773766f,
    0.0000000f,0.0236880f,0.0472793f,0.0707750f,0.0941761f,0.1174834f,0.1406980f,0.1638209f,0.1868528f,0.2097948f,
    0.2326478f,0.2554126f,0.2780901f,0.3006813f,0.3231870f,0.3456079f,0.3679451f,0.3901992f,0.4123711f,0.4344617f,
    0.4564717f,0.4784020f,0.5002532f,0.5220262f,0.5437218f,0.5653406f,0.5868835f,0.6083511f,0.6297442f,0.6510636f,
    0.6723099f,0.6934837f,0.7145859f,0.7356171f,0.7565780f,0.7774692f,0.7982914f,0.8190452f,0.8397314f,0.8603504f,
    0.8809031f,0.9013899f,0.9218114f,0.9421684f,0.9624614f,0.9826910f,0.0028577f,0.0229622f,0.0430051f,0.0629868f,
    0.0829079f,0.1027691f,0.1225708f,0.1423136f,0.1619981f,0.1816246f,0.2011939f,0.2207063f,0.2401624f,0.2595628f,
    0.2789078f,0.2981980f,0.3174339f,0.3366159f,0.3557446f,0.3748204f,0.3938437f,0.4128151f,0.4317349f,0.4506037f,
    0.4694219f,0.4881899f,0.5069082f,0.5255771f,0.5441971f,0.5627687f,0.5812922f,0.5997681f,0.6181967f,0.6365786f,
    0.6549139f,0.6732033f,0.6914470f,0.7096454f,0.7277990f,0.7459080f,0.7639729f,0.7819941f,0.7999718f,0.8179065f,
    0.8357986f,0.8536483f,0.8714560f,0.8892222f,0.9069470f,0.9246309f,0.9422742f,0.9598772f,0.9774402f,0.9949636f,
    0.0124478f,0.0298929f,0.0472994f,0.0646675f,0.0819975f,0.0992898f,0.1165447f,0.1337625f,0.1509433f,0.1680877f,
    0.1851957f,0.2022678f,0.2193042f,0.2363052f,0.2532711f,0.2702021f,0.2870985f,0.3039606f,0.3207886f,0.3375829f,
    0.3543437f,0.3710712f,0.3877656f,0.4044274f,0.4210566f,0.4376536f,0.4542186f,0.4707518f,0.4872535f,0.5037240f,
    0.5201634f,0.5365720f,0.5529500f,0.5692977f,0.5856152f,0.6019029f,0.6181609f,0.6343895f,0.6505889f,0.6667592f,
    0.6829008f,0.6990137f,0.7150984f,0.7311548f,0.7471833f,0.7631841f,0.7791573f,0.7951032f,0.8110219f,0.8269137f,
    0.8427788f,0.8586173f,0.8744294f,0.8902154f,0.9059754f,0.9217096f,0.9374182f,0.9531013f,0.9687592f,0.9843921f};

static const float Bark_scaling_ns[49] = {
    0.9206272f,0.9168145f,0.9110756f,0.8661205f,0.8325711f,0.8726765f,0.8603744f,
    0.8474684f,0.7793450f,0.7283288f,0.7938087f,0.6909955f,0.6850291f,0.6468679f,
    0.6443934f,0.5467870f,0.5602764f,0.5173278f,0.4827087f,0.4412360f,0.4073707f,
    0.3781771f,0.3453107f,0.3150882f,0.2804809f,0.2567482f,0.2287180f,0.2047690f,
    0.1830997f,0.1630629f,0.1460377f,0.1389736f,0.1347514f,0.1230076f,0.1127183f,
    0.1031600f,0.0941616f,0.0853784f,0.0768105f,0.0660130f,0.0559133f,0.0475741f,
    0.0398728f,0.0330024f,0.0271996f,0.0231789f,0.0201442f,0.0175081f,0.0328540f};

#endif /* Elevoc_NS_Constant_h */