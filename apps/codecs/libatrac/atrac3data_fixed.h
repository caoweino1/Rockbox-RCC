
/* tables for the scalefactor decoding */
/* scaled by 2^31*/
static const int32_t iMaxQuant_fix[8] = {
    0x0, 0x55555580, 0x33333340, 0x24924940, 0x1c71c720, 0x11111120, 0x8421080, 
    0x4104108
};

/* scaled by 2^16 */
static const int32_t SFTable_fixed[64] = {
    0x00000800, 0x00000a14, 0x00000cb3, 0x00001000, 0x00001429, 0x00001966, 
    0x00002000, 0x00002851, 0x000032cc, 0x00004000, 0x000050a3, 0x00006598, 
    0x00008000, 0x0000a145, 0x0000cb30, 0x00010000, 0x0001428a, 0x00019660, 
    0x00020000, 0x00028514, 0x00032cc0, 0x00040000, 0x00050a29, 0x00065980, 
    0x00080000, 0x000a1452, 0x000cb2ff, 0x00100000, 0x001428a3, 0x001965ff, 
    0x00200000, 0x00285146, 0x0032cbfd, 0x00400000, 0x0050a28c, 0x006597fb, 
    0x00800000, 0x00a14518, 0x00cb2ff5, 0x01000000, 0x01428a30, 0x01965fea, 
    0x02000000, 0x02851460, 0x032cbfd4, 0x04000000, 0x050a28c0, 0x06597fa8, 
    0x08000000, 0x0a145180, 0x0cb2ff50, 0x10000000, 0x1428a300, 0x1965fea0, 
    0x20000000, 0x28514600, 0x32cbfd40, 0x40000000, 0x50a28c00, 0x6597fa80, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000,
};

/* transform data */
/* floating point values scaled by 2^31  */
static const int32_t qmf_48tap_half_fix[24] = {
    0xffff855e, 0xfffcfbca, 0xfffe28eb, 0x9de6b,    0x7f028,    0xffe40d08, 
    0xffeef140, 0x42a692,   0x19ab1f,   0xff75dec7, 0xffe738f5, 0x100e928, 
    0xfffdfedf, 0xfe478b84, 0x50b279,   0x2c83f88,  0xff005ad7, 0xfba2ee80, 
    0x2685970,  0x6f42798,  0xfa6b6f10, 0xf3475f80, 0x10e7f7c0, 0x3b6c44c0 
};

/* mdct window scaled by 2^31 */
static const int32_t window_lookup[512] = {
    0xffffb10c, 0xfffd394b, 0xfff8494f, 0xfff0e025, 0xffe6fc5f, 0xffda9c15, 
    0xffcbbce6, 0xffba5bf4, 0xffa675e8, 0xff9006f0, 0xff770aba, 0xff5b7c7e, 
    0xff3d56f2, 0xff1c9452, 0xfef92e59, 0xfed31e45, 0xfeaa5cd5, 0xfe7ee247, 
    0xfe50a657, 0xfe1fa041, 0xfdebc6c1, 0xfdb5100d, 0xfd7b71d5, 0xfd3ee149, 
    0xfcff5311, 0xfcbcbb49, 0xfc770d99, 0xfc2e3d15, 0xfbe23c39, 0xfb92fd29, 
    0xfb407141, 0xfaea8989, 0xfa913661, 0xfa3467b1, 0xf9d40cd9, 0xf9701499, 
    0xf9086d41, 0xf89d04a9, 0xf82dc7f1, 0xf7baa3e1, 0xf74384b1, 0xf6c85611, 
    0xf6490321, 0xf5c576b1, 0xf53d9b21, 0xf4b15a01, 0xf4209ce1, 0xf38b4c71, 
    0xf2f15171, 0xf2529411, 0xf1aefbf1, 0xf10670a1, 0xf058d941, 0xefa61cc1, 
    0xeeee21c1, 0xee30cec1, 0xed6e0a41, 0xeca5ba61, 0xebd7c5c1, 0xeb041241, 
    0xea2a8601, 0xe94b0861, 0xe8657f61, 0xe779d241, 0xe687e861, 0xe58fa9e1, 
    0xe490fec1, 0xe38bd101, 0xe28009c1, 0xe16d93e1, 0xe0545ba1, 0xdf344dc1, 
    0xde0d5881, 0xdcdf6bc1, 0xdbaa7801, 0xda6e70c1, 0xd92b4ac1, 0xd7e0fc81, 
    0xd68f7ec1, 0xd536cd41, 0xd3d6e5c1, 0xd26fc901, 0xd10179c1, 0xcf8bff41, 
    0xce0f6301, 0xcc8bb241, 0xcb00fdc1, 0xc96f5b01, 0xc7d6e141, 0xc637af41, 
    0xc491e4c1, 0xc2e5a801, 0xc1332401, 0xbf7a8701, 0xbdbc0681, 0xbbf7da01, 
    0xba2e4181, 0xb85f7f81, 0xb68bde01, 0xb4b3a981, 0xb2d73781, 0xb0f6df01, 
    0xaf12ff01, 0xad2bfa81, 0xab423981, 0xa9562981, 0xa7683c01, 0xa578e701, 
    0xa388a681, 0xa197f801, 0x9fa75e81, 0x9db75f01, 0x9bc88201, 0x99db5301, 
    0x97f06001, 0x96083601, 0x94236601, 0x92427f81, 0x90661481, 0x8e8eb481, 
    0x8cbced01, 0x8af14d81, 0x892c5f81, 0x876eab01, 0x85b8b681, 0x840b0301, 
    0x82660c01, 0x80ca4a01, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 
    0x80ca4a01, 0x82660c01, 0x840b0301, 0x85b8b681, 0x876eab01, 0x892c5f81, 
    0x8af14d81, 0x8cbced01, 0x8e8eb481, 0x90661481, 0x92427f81, 0x94236601, 
    0x96083601, 0x97f06001, 0x99db5301, 0x9bc88201, 0x9db75f01, 0x9fa75e81, 
    0xa197f801, 0xa388a681, 0xa578e701, 0xa7683c01, 0xa9562981, 0xab423981, 
    0xad2bfa81, 0xaf12ff01, 0xb0f6df01, 0xb2d73781, 0xb4b3a981, 0xb68bde01, 
    0xb85f7f81, 0xba2e4181, 0xbbf7da01, 0xbdbc0681, 0xbf7a8701, 0xc1332401, 
    0xc2e5a801, 0xc491e4c1, 0xc637af41, 0xc7d6e141, 0xc96f5b01, 0xcb00fdc1, 
    0xcc8bb241, 0xce0f6301, 0xcf8bff41, 0xd10179c1, 0xd26fc901, 0xd3d6e5c1, 
    0xd536cd41, 0xd68f7ec1, 0xd7e0fc81, 0xd92b4ac1, 0xda6e70c1, 0xdbaa7801, 
    0xdcdf6bc1, 0xde0d5881, 0xdf344dc1, 0xe0545ba1, 0xe16d93e1, 0xe28009c1, 
    0xe38bd101, 0xe490fec1, 0xe58fa9e1, 0xe687e861, 0xe779d241, 0xe8657f61, 
    0xe94b0861, 0xea2a8601, 0xeb041241, 0xebd7c5c1, 0xeca5ba61, 0xed6e0a41, 
    0xee30cec1, 0xeeee21c1, 0xefa61cc1, 0xf058d941, 0xf10670a1, 0xf1aefbf1, 
    0xf2529411, 0xf2f15171, 0xf38b4c71, 0xf4209ce1, 0xf4b15a01, 0xf53d9b21, 
    0xf5c576b1, 0xf6490321, 0xf6c85611, 0xf74384b1, 0xf7baa3e1, 0xf82dc7f1, 
    0xf89d04a9, 0xf9086d41, 0xf9701499, 0xf9d40cd9, 0xfa3467b1, 0xfa913661, 
    0xfaea8989, 0xfb407141, 0xfb92fd29, 0xfbe23c39, 0xfc2e3d15, 0xfc770d99, 
    0xfcbcbb49, 0xfcff5311, 0xfd3ee149, 0xfd7b71d5, 0xfdb5100d, 0xfdebc6c1, 
    0xfe1fa041, 0xfe50a657, 0xfe7ee247, 0xfeaa5cd5, 0xfed31e45, 0xfef92e59, 
    0xff1c9452, 0xff3d56f2, 0xff5b7c7e, 0xff770aba, 0xff9006f0, 0xffa675e8, 
    0xffba5bf4, 0xffcbbce6, 0xffda9c15, 0xffe6fc5f, 0xfff0e025, 0xfff8494f, 
    0xfffd394b, 0xffffb10c, 
};

/* Gain tables scaled by 2^16 */
static const int32_t gain_tab1[16] = {
    0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000, 0x00008000, 
    0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 
    0x00000100, 0x00000080, 0x00000040, 0x00000020, 
};

static const int32_t gain_tab2[31] = {
    0x0003ab03, 0x00035d14, 0x0003159d, 0x0002d414, 0x000297fb, 0x000260e0, 
    0x00022e57, 0x00020000, 0x0001d582, 0x0001ae8a, 0x00018ace, 0x00016a0a, 
    0x00014bfe, 0x00013070, 0x0001172c, 0x00010000, 0x0000eac1, 0x0000d745, 
    0x0000c567, 0x0000b505, 0x0000a5ff, 0x00009838, 0x00008b96, 0x00008000, 
    0x00007560, 0x00006ba2, 0x000062b4, 0x00005a82, 0x000052ff, 0x00004c1c, 
    0x000045cb, 
 
};

/* Joint-Stereo related tables, scaled by 2^16 */
static const int32_t matrixCoeffs_fix[8] = {
    0x00000000, 0x00020000, 0x00020000, 0x00020000, 
    0x00000000, 0x00000000, 0x00010000, 0x00010000, 
};

