#ifndef _SM2Encrypt_H_
#define _SM2Encrypt_H_

//#include"OPENCL.h"

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

//typedef signed char        int_least8_t;
//typedef short              int_least16_t;
//typedef int                int_least32_t;
//typedef long long          int_least64_t;
//typedef unsigned char      uint_least8_t;
//typedef unsigned short     uint_least16_t;
//typedef unsigned int       uint_least32_t;
//typedef unsigned long long uint_least64_t;

//typedef signed char        int_fast8_t;
//typedef int                int_fast16_t;
//typedef int                int_fast32_t;
//typedef long long          int_fast64_t;
//typedef unsigned char      uint_fast8_t;
//typedef unsigned int       uint_fast16_t;
//typedef unsigned int       uint_fast32_t;
//typedef unsigned long long uint_fast64_t;

typedef long long          intmax_t;
typedef unsigned long long uintmax_t;

typedef unsigned int uint;

typedef struct
{
    uint64_t m_low;
    uint64_t m_high;
} uint128_t;
//--------------------------------------------------
//






//--------------------------------------------------
//opencl.h

void* memcpy_cl(void* dest, const void* src, int n);
void* memset_cl(void* ptr, int value, int num);

//--------------------------------------------------
//ecc.h

typedef uint64_t uint256_t[4];
extern uint256_t exPrivateKey;
/* Curve selection options. */
#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48
#ifndef ECC_CURVE
#define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
#error "Must define ECC_CURVE to one of the available curves"
#endif

#define ECC_BYTES ECC_CURVE

#ifdef __cplusplus
extern "C"
{
#endif

#define NUM_ECC_DIGITS (ECC_BYTES/8)
#define MAX_TRIES 16

    typedef struct
    {
        uint64_t x[NUM_ECC_DIGITS];
        uint64_t y[NUM_ECC_DIGITS];
    } EccPoint;

   
    //void ecc_make_KeyY(const uint8_t PublicKey_IN[33], uint8_t PublicKey_Y[32]);
    void vli_mult(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right);
    int vli_cmp(uint64_t* p_left, uint64_t* p_right);
    void vli_modAdd(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right, uint64_t* p_mod);
    /*求left*right%p_mod保存到p_result*/
    void vli_modMult(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right, uint64_t* p_mod);
    uint64_t vli_sub(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right);
    void EccPoint_mult(EccPoint* p_result, EccPoint* p_point, uint64_t* p_scalar, uint64_t* p_initialZ);
    void XYcZ_add(uint64_t* X1, uint64_t* Y1, uint64_t* X2, uint64_t* Y2);
    void vli_modSquare_fast(uint64_t* p_result, uint64_t* p_left);
    void vli_modSub(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right, uint64_t* p_mod);
    void vli_modMult_fast(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right);
    void mod_sqrt(uint64_t a[NUM_ECC_DIGITS]);
    uint64_t vli_add(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right);
    void XYcZ_addC(uint64_t* X1, uint64_t* Y1, uint64_t* X2, uint64_t* Y2);
    void XYcZ_initial_double(uint64_t* X1, uint64_t* Y1, uint64_t* X2, uint64_t* Y2, uint64_t* p_initialZ);
    void vli_modInv(uint64_t* p_result, uint64_t* p_input, uint64_t* p_mod);




    //--------------------------------------------------
    //sm3.h

#ifdef __cpluscplus
    extern "C" {
#endif

//#include <string.h>

#ifdef __cpluscplus
    }
#endif

#define LK_GVALUE_LEN       64
#define LK_WORD_SIZE        32
#define LK_GVALUE_BITLEN    256
#define LK_HASH_NMEMB       8

    typedef unsigned int        UINT;
#ifdef i386
    typedef unsigned long long   UWORD;
#else
    typedef unsigned long       UWORD;
#endif
    typedef unsigned char   UCHAR;

    //常量
    // 0 <= j <= 15
#define LK_T0   0x79cc4519
// 16 <= j <= 63
#define LK_T1   0x7a879d8a
//循环左移
#define LOOPSHFT(a, n) ( ((a) << (n)) | ((a) >> (LK_WORD_SIZE - (n))))
//布尔函数
#define LK_FF0(x, y, z) ((x)^(y)^(z))
#define LK_FF1(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define LK_GG0(x, y, z) ((x)^(y)^(z))
#define LK_GG1(x, y, z) (( (x) & (y) ) | ((~x) & (z)))

//置换函数
#define LK_P0(x) (\
    (x)^(LOOPSHFT((x), 9))^(LOOPSHFT((x), 17)) )
#define LK_P1(x) (\
    (x)^(LOOPSHFT((x), 15))^(LOOPSHFT((x), 23)) )

//标准中给出的IV初始值
#define LK_INIT_VALUE(t) {\
    lk_sm3_context_t *x = (t);\
    x->v[0] = 0x7380166f;\
    x->v[1] = 0x4914b2b9;\
    x->v[2] = 0x172442d7;\
    x->v[3] = 0xda8a0600;\
    x->v[4] = 0xa96f30bc;\
    x->v[5] = 0x163138aa;\
    x->v[6] = 0xe38dee4d;\
    x->v[7] = 0xb0fb0e4e;\
    memset(x->data, 0, LK_GVALUE_LEN);\
    x->total = 0;\
    x->len = 0;}

#define LK_LE_ONE(t) {\
    lk_sm3_context_t *x = (t);\
    UINT l_z, l_d;\
    for (l_z = 0; l_z < LK_HASH_NMEMB; l_z++) {\
        l_d = x->v[l_z];\
        x->output[l_z*4] = ((l_d >> 24) & 0x000000ff);\
        x->output[l_z*4 + 1] = ((l_d >> 16) & 0x000000ff);\
        x->output[l_z*4 + 2] = ((l_d >> 8) & 0x000000ff);\
        x->output[l_z*4 + 3] = (l_d & 0x000000ff);\
    }}

//大端转化
#define LK_GE_ONE(c) (\
    ((c&0x00000000000000ffUL) << 56) | (((c&0x000000000000ff00UL) << 40)) |\
    ((c&0x0000000000ff0000UL) << 24) | (((c&0x00000000ff000000UL) << 8)) |\
    ((c&0x000000ff00000000UL)  >> 8) | (((c&0x0000ff0000000000UL) >> 24)) |\
    ((c&0x00ff000000000000UL) >> 40) | (((c&0xff00000000000000UL) >> 56)) )
#define LK_GE(w, c) \
    int j2;\
    for (j = 0; j <= 15; j++) {\
        j2 = j*4;\
        w[j] = ((c[j2] << 24) | ((c[j2+1] << 16)) |\
         (c[j2+2] << 8) | (c[j2+3]));\
    }


//压缩计算摘要函数
#define LK_MSG_CF(t) {\
    UINT j;\
    lk_sm3_context_t *x = t;\
    UCHAR *data = x->data;\
    UINT W1[68];\
    UINT W2[64];\
    UINT a,b,c,d,e,f,g,h;\
    a = x->v[0];\
    b = x->v[1];\
    c = x->v[2];\
    d = x->v[3];\
    e = x->v[4];\
    f = x->v[5];\
    g = x->v[6];\
    h = x->v[7];\
    LK_GE(W1, data)\
for ( j = 16; j <= 67; j++ ) {\
        W1[j] = LK_P1(W1[j-16]^W1[j-9]^(LOOPSHFT(W1[j-3], 15))) ^ LOOPSHFT(W1[j-13], 7) ^ W1[j-6];\
    }\
    for ( j = 0; j <= 63; j++ ) {\
        W2[j] = W1[j] ^ W1[j+4];\
    }\
    for ( j = 0; j <= 63; j++ ) {\
       UINT T, ss1, ss2, tt1, tt2;\
       if ( j >= 0 && j <= 15 )\
           T = LK_T0;\
       else\
           T = LK_T1;\
        ss1 = LOOPSHFT( (LOOPSHFT(a, 12) + e + LOOPSHFT(T, j)), 7 );\
        ss2 = ss1 ^ LOOPSHFT(a, 12);\
        if ( j >= 0 && j <= 15 ) {\
            tt1 = LK_FF0(a, b, c) + d + ss2 + W2[j];\
            tt2 = LK_GG0(e, f, g) + h + ss1 + W1[j];\
        } else {\
            tt1 = LK_FF1(a, b, c) + d + ss2 + W2[j];\
            tt2 = LK_GG1(e, f, g) + h + ss1 + W1[j];\
        }\
        d = c;\
        c = LOOPSHFT(b, 9);\
        b = a;\
        a = tt1;\
        h = g;\
        g = LOOPSHFT(f, 19);\
        f = e;\
        e = LK_P0(tt2);\
    }\
    x->v[0] = a ^ x->v[0];\
    x->v[1] = b ^ x->v[1];\
    x->v[2] = c ^ x->v[2];\
    x->v[3] = d ^ x->v[3];\
    x->v[4] = e ^ x->v[4];\
    x->v[5] = f ^ x->v[5];\
    x->v[6] = g ^ x->v[6];\
    x->v[7] = h ^ x->v[7];\
    x->len = 0;\
}

    typedef struct lk_sm3_context_s
    {
        UINT    len;
        UINT    total;
        UCHAR   data[LK_GVALUE_LEN];
        UINT    v[LK_HASH_NMEMB];
        UCHAR   output[LK_WORD_SIZE];
    } lk_sm3_context_t;

#ifdef __cpluscplus
    extern "C" {
#endif
        void SM3(char* cdata, int sizeInByte, char* res);
        extern void lk_sm3_final(lk_sm3_context_t* context);
        extern void lk_sm3_update(lk_sm3_context_t* context, UCHAR* data, UINT len);

#ifdef __cpluscplus
    }
#endif

    //--------------------------------------------------
    //sm2.h


    //typedef uint64_t uint256_t[4];
    typedef int BOOL;

    //#define USE_ON_STM32	//打开这个宏定义以加入STM32中的硬件支持（uart，systick）
    //#define DEBUG_MODE	//打开这个宏定义以使用固定的随机数

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

    typedef struct
    {
        uint256_t a;
        uint256_t b;
        uint256_t p;
    }EllipticCurveDef;

    typedef struct
    {
        uint8_t r[32];
        uint8_t s[32];
    }SM2Signature;

    typedef struct
    {
        uint64_t* data;
        uint64_t sizeInLongLong;
    }BIG;

    typedef struct
    {
        uint8_t ZA[32];
        uint8_t ZB[32];
        uint256_t dA;
        EccPoint* PA;
        EccPoint* PB;
        uint32_t w;
        uint32_t h;
        uint256_t n;
        EccPoint* G;
    }KeyExchangeAOriginalInfoDef;//秘钥交换中的原始信息

    typedef struct
    {
        uint8_t ZA[32];//包含IDA的杂凑值
        uint8_t ZB[32];//包含IDB的杂凑值
        uint256_t dB;//私钥B
        EccPoint* PA;//公钥A
        EccPoint* PB;//公钥B
        uint32_t w;
        uint32_t h;
        uint256_t n;	//基点的阶
        EccPoint* G;	//基点
    }KeyExchangeBOriginalInfoDef;

    extern uint256_t ellipticCurve_a;
    extern uint256_t ellipticCurve_b;
    extern uint256_t ellipticCurve_p;
    extern uint256_t ellipticCurve_n;
    extern uint256_t ellipticCurve_Gx;
    extern uint256_t ellipticCurve_Gy;
    extern uint256_t exPrivateKey;

    void EllipticCurveInit(uint64_t* a, uint64_t* b, uint64_t* p);
    BOOL IsValidPoint(EccPoint* point, EllipticCurveDef* Ec);
    //void GetRandomNumber(uint64_t* result, uint64_t* maxVal);
    //BOOL GenerateKeys(uint64_t* privKey, EccPoint* pubKey, EccPoint* pointG);
    void CurvePointAdd(EccPoint* res, EccPoint* p1, EccPoint* p2);
    void CurvePointMul(EccPoint* res, EccPoint* p1, uint64_t* times);
    //void BigNumberCpy(uint64_t* des, uint64_t* src);
    void BigBitShift(BIG* num, BOOL shift);
    void Uint256ToString(uint8_t* res, uint256_t num);
    void StringToUint256(uint64_t* res, uint8_t* num);
    void KDF(uint8_t* result, uint8_t* cdata, int datalen, int keylen);
    //void Uint256WriteBits(uint64_t* num, BOOL bitNum, int startBitPos, int stopBitPos);
    //int GetMSB(uint64_t* num);

    ////---------------------uint8_t* SM2Encrypt(char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK);
    //uint8_t* SM2Decrypt(char* C, int lenInByte, uint64_t* privKey);
    void CalculateC1(EccPoint* result, uint256_t k, EccPoint* G);
    void CalculateKPb(EccPoint* result, uint256_t k, EccPoint* Pb);
    void CalculateC2(uint8_t* result, char* M, uint8_t* t, int len);
    //void CalculateMessage(uint8_t* result, uint8_t* C2, uint8_t* t, int lenInByte);

    //SM2Signature* SM2Sign(char* message, int messageSizeInBit, char* IDA, uint16_t ENTLAInBit, EccPoint* G, EccPoint* pubKey, uint64_t* privKey);
    //BOOL SM2SignatureVerify(char* message, int messageSizeInBit, SM2Signature* sign, char* IDA, uint16_t ENTLAInBit, EccPoint* G, EccPoint* pubKey);

    //BOOL SM2KeyExchange(char* IDA, uint32_t ENTLAInBit, char* IDB, uint32_t ENTLBInBit, EccPoint* G, int klenInBit);
    //void SM2KeyExchange_AStep1To3(EccPoint* RA, uint64_t* rA, KeyExchangeAOriginalInfoDef infoA);
    //void SM2KeyExchange_AStep4To5(uint64_t* tA, EccPoint* RA, uint64_t* rA, KeyExchangeAOriginalInfoDef infoA);
    //void SM2KeyExchange_AStep6To7(EccPoint* resultU, uint64_t* tA, EccPoint* RB, KeyExchangeAOriginalInfoDef infoA);
    //void SM2KeyExchange_AStep8To9(uint8_t* KA, uint8_t* S1, EccPoint U, int klenInBit, EccPoint RA, EccPoint RB, KeyExchangeAOriginalInfoDef infoA);
    //void SM2KeyExchange_AStep10(uint8_t* SA, EccPoint U, EccPoint RA, EccPoint RB, KeyExchangeAOriginalInfoDef infoA);
    //void SM2KeyExchange_BStep1To4(uint64_t* tB, EccPoint* RB, KeyExchangeBOriginalInfoDef infoB);
    //void SM2KeyExchange_BStep5To6(EccPoint* resultV, uint64_t* tB, EccPoint* RA, KeyExchangeBOriginalInfoDef infoB);
    //void SM2KeyExchange_BStep7To9(uint8_t* KB, uint8_t* SB, EccPoint V, int klenInBit, EccPoint RA, EccPoint RB, KeyExchangeBOriginalInfoDef infoB);
    //void SM2KeyExchange_BStep10(uint8_t* S2, EccPoint V, EccPoint RA, EccPoint RB, KeyExchangeBOriginalInfoDef infoB);




    //--------------------------------------------------
    void CalculateMessage(uint8_t* result, uint8_t* C2, uint8_t* t, int lenInByte);
uint8_t* SM2Decrypt(char* C, int lenInByte, uint64_t* privKey);
void SM2Encrypt(uint8_t* C, char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK, EccPoint* G);

#endif /* _SM2Encrypt_H_ */
