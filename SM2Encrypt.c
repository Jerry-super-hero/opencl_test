#include"SM2Encrypt.h"
//#include <stdio.h>




//----------------------------------------------------------------------------
//opencl.c

//#include <stdio.h>
//#include <assert.h>

//#include<string.h>

//自己写的memset函数


//void* memset(void* dest, int c, size_t count)
//{
//    assert(dest != NULL && count >= 0);
//    char* p = (char*)dest;
//
//    unsigned  int i = 0;
//    while (i < count)
//    {
//        p[i++] = c;
//    }
//    return (void*)p;
//}


//c语言实现memset
void* memset_cl(void* ptr, int value, int num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

//c语言实现memcpy
void* memcpy_cl(void* dest, const void* src, int n) {
    char* dp = dest;
    const char* sp = src;
    while (n--) {
        *dp++ = *sp++;
    }
    return dest;
}



//----------------------------------------------------------------------------
//ecc.c

//#if defined(__SIZEOF_INT128__) || ((__clang_major__ * 100 + __clang_minor__) >= 302)
//#define SUPPORTS_INT128 0
//#else
//#define SUPPORTS_INT128 1
//#endif

//#if SUPPORTS_INT128
//typedef unsigned __int128 uint128_t;
//#else

//#endif



#define CONCAT1(a, b) a##b
#define CONCAT(a, b) CONCAT1(a, b)

#define Curve_P_16 {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFDFFFFFFFF}
#define Curve_P_24 {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull, 0xFFFFFFFFFFFFFFFFull}
//#define Curve_P_32 {0xFFFFFFFFFFFFFFFFull, 0x00000000FFFFFFFFull, 0x0000000000000000ull, 0xFFFFFFFF00000001ull}
#define Curve_P_32	{0x722EDB8B08F1DFC3ULL, 0x457283915C45517DULL, 0xE8B92435BF6FF7DEULL, 0x8542D69E4C044F18ULL}
#define Curve_P_48 {0x00000000FFFFFFFF, 0xFFFFFFFF00000000, 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}

#define Curve_B_16 {0xD824993C2CEE5ED3, 0xE87579C11079F43D}
#define Curve_B_24 {0xFEB8DEECC146B9B1ull, 0x0FA7E9AB72243049ull, 0x64210519E59C80E7ull}
//#define Curve_B_32 {0x3BCE3C3E27D2604Bull, 0x651D06B0CC53B0F6ull, 0xB3EBBD55769886BCull, 0x5AC635D8AA3A93E7ull}
#define Curve_B_32 {0x6E12D1DA27C5249Aull, 0xF61D59A5B16BA06Eull, 0x9CF84241484BFE48ull, 0x63E4C6D3B23B0C84ull}
#define Curve_B_48 {0x2A85C8EDD3EC2AEF, 0xC656398D8A2ED19D, 0x0314088F5013875A, 0x181D9C6EFE814112, 0x988E056BE3F82D19, 0xB3312FA7E23EE7E4}


#define Curve_G_16 { \
    {0x0C28607CA52C5B86, 0x161FF7528B899B2D}, \
    {0xC02DA292DDED7A83, 0xCF5AC8395BAFEB13}}

#define Curve_G_24 { \
    {0xF4FF0AFD82FF1012ull, 0x7CBF20EB43A18800ull, 0x188DA80EB03090F6ull}, \
    {0x73F977A11E794811ull, 0x631011ED6B24CDD5ull, 0x07192B95FFC8DA78ull}}

//#define Curve_G_32 { \
//    {0xF4A13945D898C296ull, 0x77037D812DEB33A0ull, 0xF8BCE6E563A440F2ull, 0x6B17D1F2E12C4247ull}, \
//    {0xCBB6406837BF51F5ull, 0x2BCE33576B315ECEull, 0x8EE7EB4A7C0F9E16ull, 0x4FE342E2FE1A7F9Bull}}
#define Curve_G_32 {\
	{0x4C4E6C147FEDD43D, 0x32220B3BADD50BDC, 0x746434EBC3CC315E, 0x421DEBD61B62EAB6},\
	{0xA85841B9E46E09A2, 0xE5D7FDFCBFA36EA1, 0xD47349D2153B70C4, 0x0680512BCBB42C07}}
#define Curve_G_48 { \
    {0x3A545E3872760AB7, 0x5502F25DBF55296C, 0x59F741E082542A38, 0x6E1D3B628BA79B98, 0x8EB1C71EF320AD74, 0xAA87CA22BE8B0537}, \
    {0x7A431D7C90EA0E5F, 0x0A60B1CE1D7E819D, 0xE9DA3113B5F0B8C0, 0xF8F41DBD289A147C, 0x5D9E98BF9292DC29, 0x3617DE4A96262C6F}}

#define Curve_N_16 {0x75A30D1B9038A115, 0xFFFFFFFE00000000}
#define Curve_N_24 {0x146BC9B1B4D22831ull, 0xFFFFFFFF99DEF836ull, 0xFFFFFFFFFFFFFFFFull}
//#define Curve_N_32 {0xF3B9CAC2FC632551ull, 0xBCE6FAADA7179E84ull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF00000000ull}
#define Curve_N_32 {0x5AE74EE7C32E79B7,0x297720630485628D,0xE8B92435BF6FF7DD,0x8542D69E4C044F18}
#define Curve_N_48 {0xECEC196ACCC52973, 0x581A0DB248B0A77A, 0xC7634D81F4372DDF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}

static uint64_t curve_p[NUM_ECC_DIGITS] = CONCAT(Curve_P_, ECC_CURVE);
static uint64_t curve_b[NUM_ECC_DIGITS] = CONCAT(Curve_B_, ECC_CURVE);
static EccPoint curve_G = CONCAT(Curve_G_, ECC_CURVE);
static uint64_t curve_n[NUM_ECC_DIGITS] = CONCAT(Curve_N_, ECC_CURVE);

#if (defined(_WIN32) || defined(_WIN64))
/* Windows */

#define WIN32_LEAN_AND_MEAN
// #include <windows.h>
// #include <wincrypt.h>

//static int getRandomNumber(uint64_t *p_vli)
//{
//    HCRYPTPROV l_prov;
//    if(!CryptAcquireContext(&l_prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
//    {
//        return 0;
//    }

//    CryptGenRandom(l_prov, ECC_BYTES, (BYTE *)p_vli);
//    CryptReleaseContext(l_prov, 0);
//
//    return 1;
//}

#else /* _WIN32 */

///* Assume that we are using a POSIX-like system with /dev/urandom or /dev/random. */
//#include <sys/types.h>
//#include <fcntl.h>
//#include <unistd.h>

#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

//static int getRandomNumber(uint64_t *p_vli)
//{
//    int l_fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
//    if(l_fd == -1)
//    {
//        l_fd = open("/dev/random", O_RDONLY | O_CLOEXEC);
//        if(l_fd == -1)
//        {
//            return 0;
//        }
//    }
//
//    char *l_ptr = (char *)p_vli;
//    size_t l_left = ECC_BYTES;
//    while(l_left > 0)
//    {
//        int l_read = read(l_fd, l_ptr, l_left);
//        if(l_read <= 0)
//        { // read failed
//            close(l_fd);
//            return 0;
//        }
//        l_left -= l_read;
//        l_ptr += l_read;
//    }
//
//    close(l_fd);
//    return 1;
//}

#endif /* _WIN32 */



//大数清零
static void vli_clear(uint64_t* p_vli)
{
    uint i;
    for (i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        p_vli[i] = 0;
    }
}

/* Returns 1 if p_vli == 0, 0 otherwise. */
static int vli_isZero(uint64_t* p_vli)
{
    uint i;
    for (i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        if (p_vli[i])
        {
            return 0;
        }
    }
    return 1;
}

/* Returns nonzero if bit p_bit of p_vli is set. */
static uint64_t vli_testBit(uint64_t* p_vli, uint p_bit)
{
    return (p_vli[p_bit / 64] & ((uint64_t)1 << (p_bit % 64)));
}

/* Counts the number of 64-bit "digits" in p_vli. */
static uint vli_numDigits(uint64_t* p_vli)
{
    int i;
    /* Search from the end until we find a non-zero digit.
       We do it in reverse because we expect that most digits will be nonzero. */
    for (i = NUM_ECC_DIGITS - 1; i >= 0 && p_vli[i] == 0; --i)
    {
    }

    return (i + 1);
}

/* Counts the number of bits required for p_vli. */
static uint vli_numBits(uint64_t* p_vli)
{
    uint i;
    uint64_t l_digit;

    uint l_numDigits = vli_numDigits(p_vli);
    if (l_numDigits == 0)
    {
        return 0;
    }

    l_digit = p_vli[l_numDigits - 1];
    for (i = 0; l_digit; ++i)
    {
        l_digit >>= 1;
    }

    return ((l_numDigits - 1) * 64 + i);
}

/* Sets p_dest = p_src. */
static void vli_set(uint64_t* p_dest, uint64_t* p_src)
{
    uint i;
    for (i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        p_dest[i] = p_src[i];
    }
}

/* Returns sign of p_left - p_right. */
int vli_cmp(uint64_t* p_left, uint64_t* p_right)
{
    int i;
    for (i = NUM_ECC_DIGITS - 1; i >= 0; --i)
    {
        if (p_left[i] > p_right[i])
        {
            return 1;
        }
        else if (p_left[i] < p_right[i])
        {
            return -1;
        }
    }
    return 0;
}

/* Computes p_result = p_in << c, returning carry. Can modify in place (if p_result == p_in). 0 < p_shift < 64. */
static uint64_t vli_lshift(uint64_t* p_result, uint64_t* p_in, uint p_shift)
{
    uint64_t l_carry = 0;
    uint i;
    for (i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        uint64_t l_temp = p_in[i];
        p_result[i] = (l_temp << p_shift) | l_carry;
        l_carry = l_temp >> (64 - p_shift);
    }

    return l_carry;
}

/* Computes p_vli = p_vli >> 1. */
static void vli_rshift1(uint64_t* p_vli)
{
    uint64_t* l_end = p_vli;
    uint64_t l_carry = 0;

    p_vli += NUM_ECC_DIGITS;
    while (p_vli-- > l_end)
    {
        uint64_t l_temp = *p_vli;
        *p_vli = (l_temp >> 1) | l_carry;
        l_carry = l_temp << 63;
    }
}

/* Computes p_result = p_left + p_right, returning carry. Can modify in place. */
uint64_t vli_add(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right)
{
    uint64_t l_carry = 0;
    uint i;
    for (i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        uint64_t l_sum = p_left[i] + p_right[i] + l_carry;
        if (l_sum != p_left[i])
        {
            l_carry = (l_sum < p_left[i]);
        }
        p_result[i] = l_sum;
    }
    return l_carry;
}

/* Computes p_result = p_left - p_right, returning borrow. Can modify in place. */
uint64_t vli_sub(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right)
{
    uint64_t l_borrow = 0;
    uint i;
    for (i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        uint64_t l_diff = p_left[i] - p_right[i] - l_borrow;
        if (l_diff != p_left[i])
        {
            l_borrow = (l_diff > p_left[i]);
        }
        p_result[i] = l_diff;
    }
    return l_borrow;
}

#if SUPPORTS_INT128

/* Computes p_result = p_left * p_right. */
static void vli_mult(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right)
{
    uint128_t r01 = 0;
    uint64_t r2 = 0;

    uint i, k;

    /* Compute each digit of p_result in sequence, maintaining the carries. */
    for (k = 0; k < NUM_ECC_DIGITS * 2 - 1; ++k)
    {
        uint l_min = (k < NUM_ECC_DIGITS ? 0 : (k + 1) - NUM_ECC_DIGITS);
        for (i = l_min; i <= k && i < NUM_ECC_DIGITS; ++i)
        {
            uint128_t l_product = (uint128_t)p_left[i] * p_right[k - i];
            r01 += l_product;
            r2 += (r01 < l_product);
        }
        p_result[k] = (uint64_t)r01;
        r01 = (r01 >> 64) | (((uint128_t)r2) << 64);
        r2 = 0;
    }

    p_result[NUM_ECC_DIGITS * 2 - 1] = (uint64_t)r01;
}

/* Computes p_result = p_left^2. */
static void vli_square(uint64_t* p_result, uint64_t* p_left)
{
    uint128_t r01 = 0;
    uint64_t r2 = 0;

    uint i, k;
    for (k = 0; k < NUM_ECC_DIGITS * 2 - 1; ++k)
    {
        uint l_min = (k < NUM_ECC_DIGITS ? 0 : (k + 1) - NUM_ECC_DIGITS);
        for (i = l_min; i <= k && i <= k - i; ++i)
        {
            uint128_t l_product = (uint128_t)p_left[i] * p_left[k - i];
            if (i < k - i)
            {
                r2 += l_product >> 127;
                l_product *= 2;
            }
            r01 += l_product;
            r2 += (r01 < l_product);
        }
        p_result[k] = (uint64_t)r01;
        r01 = (r01 >> 64) | (((uint128_t)r2) << 64);
        r2 = 0;
    }

    p_result[NUM_ECC_DIGITS * 2 - 1] = (uint64_t)r01;
}

#else /* #if SUPPORTS_INT128 */

static uint128_t mul_64_64(uint64_t p_left, uint64_t p_right)
{
    uint128_t l_result;

    uint64_t a0 = p_left & 0xffffffffull;
    uint64_t a1 = p_left >> 32;
    uint64_t b0 = p_right & 0xffffffffull;
    uint64_t b1 = p_right >> 32;

    uint64_t m0 = a0 * b0;
    uint64_t m1 = a0 * b1;
    uint64_t m2 = a1 * b0;
    uint64_t m3 = a1 * b1;

    m2 += (m0 >> 32);
    m2 += m1;
    if (m2 < m1)
    {
        // overflow
        m3 += 0x100000000ull;
    }

    l_result.m_low = (m0 & 0xffffffffull) | (m2 << 32);
    l_result.m_high = m3 + (m2 >> 32);

    return l_result;
}

static uint128_t add_128_128(uint128_t a, uint128_t b)
{
    uint128_t l_result;
    l_result.m_low = a.m_low + b.m_low;
    l_result.m_high = a.m_high + b.m_high + (l_result.m_low < a.m_low);
    return l_result;
}

void vli_mult(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right)
{
    uint128_t r01 = { 0, 0 };
    uint64_t r2 = 0;

    uint i, k;

    /* Compute each digit of p_result in sequence, maintaining the carries. */
    for (k = 0; k < NUM_ECC_DIGITS * 2 - 1; ++k)
    {
        uint l_min = (k < NUM_ECC_DIGITS ? 0 : (k + 1) - NUM_ECC_DIGITS);
        for (i = l_min; i <= k && i < NUM_ECC_DIGITS; ++i)
        {
            uint128_t l_product = mul_64_64(p_left[i], p_right[k - i]);
            r01 = add_128_128(r01, l_product);
            r2 += (r01.m_high < l_product.m_high);
        }
        p_result[k] = r01.m_low;
        r01.m_low = r01.m_high;
        r01.m_high = r2;
        r2 = 0;
    }

    p_result[NUM_ECC_DIGITS * 2 - 1] = r01.m_low;
}

//static void vli_square(uint64_t* p_result, uint64_t* p_left)
//{
//    uint128_t r01 = { 0, 0 };
//    uint64_t r2 = 0;
//
//    uint i, k;
//    for (k = 0; k < NUM_ECC_DIGITS * 2 - 1; ++k)
//    {
//        uint l_min = (k < NUM_ECC_DIGITS ? 0 : (k + 1) - NUM_ECC_DIGITS);
//        for (i = l_min; i <= k && i <= k - i; ++i)
//        {
//            uint128_t l_product = mul_64_64(p_left[i], p_left[k - i]);
//            if (i < k - i)
//            {
//                r2 += l_product.m_high >> 63;
//                l_product.m_high = (l_product.m_high << 1) | (l_product.m_low >> 63);
//                l_product.m_low <<= 1;
//            }
//            r01 = add_128_128(r01, l_product);
//            r2 += (r01.m_high < l_product.m_high);
//        }
//        p_result[k] = r01.m_low;
//        r01.m_low = r01.m_high;
//        r01.m_high = r2;
//        r2 = 0;
//    }
//
//    p_result[NUM_ECC_DIGITS * 2 - 1] = r01.m_low;
//}

#endif /* SUPPORTS_INT128 */


/* Computes p_result = (p_left + p_right) % p_mod.
   Assumes that p_left < p_mod and p_right < p_mod, p_result != p_mod. */
void vli_modAdd(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right, uint64_t* p_mod)
{
    uint64_t l_carry = vli_add(p_result, p_left, p_right);
    while (l_carry || vli_cmp(p_result, p_mod) >= 0)
    {
        if (l_carry == 1 && vli_cmp(p_result, p_mod) == -1) //本次减法需要借位。l_carry == 1和后面的判断其实是永远相等的，这样可以提高运行速度
        {
            l_carry = 0;
        }
        /* p_result > p_mod (p_result = p_mod + remainder), so subtract p_mod to get remainder. */
        vli_sub(p_result, p_result, p_mod);
    }
}

/* Computes p_result = (p_left - p_right) % p_mod.
   Assumes that p_left < p_mod and p_right < p_mod, p_result != p_mod. */
void vli_modSub(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right, uint64_t* p_mod)
{
    uint64_t l_borrow = vli_sub(p_result, p_left, p_right);
    if (l_borrow)
    {
        /* In this case, p_result == -diff == (max int) - diff.
           Since -x % d == d - x, we can get the correct result from p_result + p_mod (with overflow). */
        vli_add(p_result, p_result, p_mod);
    }
}

//#if ECC_CURVE == secp128r1
//
///* Computes p_result = p_product % curve_p.
//   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
//static void vli_mmod_fast(uint64_t* p_result, uint64_t* p_product)
//{
//    uint64_t l_tmp[NUM_ECC_DIGITS];
//    int l_carry;
//
//    vli_set(p_result, p_product);
//
//    l_tmp[0] = p_product[2];
//    l_tmp[1] = (p_product[3] & 0x1FFFFFFFFull) | (p_product[2] << 33);
//    l_carry = vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = (p_product[2] >> 31) | (p_product[3] << 33);
//    l_tmp[1] = (p_product[3] >> 31) | ((p_product[2] & 0xFFFFFFFF80000000ull) << 2);
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = (p_product[2] >> 62) | (p_product[3] << 2);
//    l_tmp[1] = (p_product[3] >> 62) | ((p_product[2] & 0xC000000000000000ull) >> 29) | (p_product[3] << 35);
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = (p_product[3] >> 29);
//    l_tmp[1] = ((p_product[3] & 0xFFFFFFFFE0000000ull) << 4);
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = (p_product[3] >> 60);
//    l_tmp[1] = (p_product[3] & 0xFFFFFFFE00000000ull);
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = 0;
//    l_tmp[1] = ((p_product[3] & 0xF000000000000000ull) >> 27);
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    while (l_carry || vli_cmp(curve_p, p_result) != 1)
//    {
//        l_carry -= vli_sub(p_result, p_result, curve_p);
//    }
//}
//
//#elif ECC_CURVE == secp192r1
//
///* Computes p_result = p_product % curve_p.
//   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
//static void vli_mmod_fast(uint64_t* p_result, uint64_t* p_product)
//{
//    uint64_t l_tmp[NUM_ECC_DIGITS];
//    int l_carry;
//
//    vli_set(p_result, p_product);
//
//    vli_set(l_tmp, &p_product[3]);
//    l_carry = vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = 0;
//    l_tmp[1] = p_product[3];
//    l_tmp[2] = p_product[4];
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    l_tmp[0] = l_tmp[1] = p_product[5];
//    l_tmp[2] = 0;
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    while (l_carry || vli_cmp(curve_p, p_result) != 1)
//    {
//        l_carry -= vli_sub(p_result, p_result, curve_p);
//    }
//}
//
//#elif ECC_CURVE == secp256r1
//
///* Computes p_result = p_product % curve_p
//   from http://www.nsa.gov/ia/_files/nist-routines.pdf */
//static void vli_mmod_fast(uint64_t* p_result, uint64_t* p_product)
//{
//    uint64_t l_tmp[NUM_ECC_DIGITS];
//    int l_carry;
//
//    /* t */
//    vli_set(p_result, p_product);//p_result[3:0] = p_product[3:0]
//
//    /* s1 */
//    l_tmp[0] = 0;
//    l_tmp[1] = p_product[5] & 0xffffffff00000000ull;
//    l_tmp[2] = p_product[6];
//    l_tmp[3] = p_product[7];
//    l_carry = vli_lshift(l_tmp, l_tmp, 1);//tmp[3:1] = (p_product[7]|p_product[6]|(p_product[5]_H))<<1;    l_carry = p_product.MSB
//    l_carry += vli_add(p_result, p_result, l_tmp);//p_result = p_result + l_tmp;        l_carry = l_carry + carry(p_result + l_tmp)
//
//    /* s2 */
//    l_tmp[1] = p_product[6] << 32;
//    l_tmp[2] = (p_product[6] >> 32) | (p_product[7] << 32);
//    l_tmp[3] = p_product[7] >> 32;
//    l_carry += vli_lshift(l_tmp, l_tmp, 1);//tmp[3:1] = product[7]_H, product[6]_H + product[7]_L, product[6]_L
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    /* s3 */
//    l_tmp[0] = p_product[4];
//    l_tmp[1] = p_product[5] & 0xffffffff;
//    l_tmp[2] = 0;
//    l_tmp[3] = p_product[7];
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    /* s4 */
//    l_tmp[0] = (p_product[4] >> 32) | (p_product[5] << 32);
//    l_tmp[1] = (p_product[5] >> 32) | (p_product[6] & 0xffffffff00000000ull);
//    l_tmp[2] = p_product[7];
//    l_tmp[3] = (p_product[6] >> 32) | (p_product[4] << 32);
//    l_carry += vli_add(p_result, p_result, l_tmp);
//
//    /* d1 */
//    l_tmp[0] = (p_product[5] >> 32) | (p_product[6] << 32);
//    l_tmp[1] = (p_product[6] >> 32);
//    l_tmp[2] = 0;
//    l_tmp[3] = (p_product[4] & 0xffffffff) | (p_product[5] << 32);
//    l_carry -= vli_sub(p_result, p_result, l_tmp);
//
//    /* d2 */
//    l_tmp[0] = p_product[6];
//    l_tmp[1] = p_product[7];
//    l_tmp[2] = 0;
//    l_tmp[3] = (p_product[4] >> 32) | (p_product[5] & 0xffffffff00000000ull);
//    l_carry -= vli_sub(p_result, p_result, l_tmp);
//
//    /* d3 */
//    l_tmp[0] = (p_product[6] >> 32) | (p_product[7] << 32);
//    l_tmp[1] = (p_product[7] >> 32) | (p_product[4] << 32);
//    l_tmp[2] = (p_product[4] >> 32) | (p_product[5] << 32);
//    l_tmp[3] = (p_product[6] << 32);
//    l_carry -= vli_sub(p_result, p_result, l_tmp);
//
//    /* d4 */
//    l_tmp[0] = p_product[7];
//    l_tmp[1] = p_product[4] & 0xffffffff00000000ull;
//    l_tmp[2] = p_product[5];
//    l_tmp[3] = p_product[6] & 0xffffffff00000000ull;
//    l_carry -= vli_sub(p_result, p_result, l_tmp);
//
//    if (l_carry < 0)
//    {
//        do
//        {
//            l_carry += vli_add(p_result, p_result, curve_p);
//        } while (l_carry < 0);
//    }
//    else
//    {
//        while (l_carry || vli_cmp(curve_p, p_result) != 1)
//        {
//            l_carry -= vli_sub(p_result, p_result, curve_p);
//        }
//    }
//}
//
//#elif ECC_CURVE == secp384r1
//
//static void omega_mult(uint64_t* p_result, uint64_t* p_right)
//{
//    uint64_t l_tmp[NUM_ECC_DIGITS];
//    uint64_t l_carry, l_diff;
//
//    /* Multiply by (2^128 + 2^96 - 2^32 + 1). */
//    vli_set(p_result, p_right); /* 1 */
//    l_carry = vli_lshift(l_tmp, p_right, 32);
//    p_result[1 + NUM_ECC_DIGITS] = l_carry + vli_add(p_result + 1, p_result + 1, l_tmp); /* 2^96 + 1 */
//    p_result[2 + NUM_ECC_DIGITS] = vli_add(p_result + 2, p_result + 2, p_right); /* 2^128 + 2^96 + 1 */
//    l_carry += vli_sub(p_result, p_result, l_tmp); /* 2^128 + 2^96 - 2^32 + 1 */
//    l_diff = p_result[NUM_ECC_DIGITS] - l_carry;
//    if (l_diff > p_result[NUM_ECC_DIGITS])
//    {
//        /* Propagate borrow if necessary. */
//        uint i;
//        for (i = 1 + NUM_ECC_DIGITS; ; ++i)
//        {
//            --p_result[i];
//            if (p_result[i] != (uint64_t)-1)
//            {
//                break;
//            }
//        }
//    }
//    p_result[NUM_ECC_DIGITS] = l_diff;
//}
//
///* Computes p_result = p_product % curve_p
//    see PDF "Comparing Elliptic Curve Cryptography and RSA on 8-bit CPUs"
//    section "Curve-Specific Optimizations" */
//static void vli_mmod_fast(uint64_t* p_result, uint64_t* p_product)
//{
//    uint64_t l_tmp[2 * NUM_ECC_DIGITS];
//
//    while (!vli_isZero(p_product + NUM_ECC_DIGITS)) /* While c1 != 0 */
//    {
//        uint64_t l_carry = 0;
//        uint i;
//
//        vli_clear(l_tmp);
//        vli_clear(l_tmp + NUM_ECC_DIGITS);
//        omega_mult(l_tmp, p_product + NUM_ECC_DIGITS); /* tmp = w * c1 */
//        vli_clear(p_product + NUM_ECC_DIGITS); /* p = c0 */
//
//        /* (c1, c0) = c0 + w * c1 */
//        for (i = 0; i < NUM_ECC_DIGITS + 3; ++i)
//        {
//            uint64_t l_sum = p_product[i] + l_tmp[i] + l_carry;
//            if (l_sum != p_product[i])
//            {
//                l_carry = (l_sum < p_product[i]);
//            }
//            p_product[i] = l_sum;
//        }
//    }
//
//    while (vli_cmp(p_product, curve_p) > 0)
//    {
//        vli_sub(p_product, p_product, curve_p);
//    }
//    vli_set(p_result, p_product);
//}
//
//#endif
//
///* Computes p_result = (p_left * p_right) % curve_p. */
//void vli_modMult_fast(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right)
//{
//    uint64_t l_product[2 * NUM_ECC_DIGITS];
//    vli_mult(l_product, p_left, p_right);
//    vli_mmod_fast(p_result, l_product);
//}
//
///* Computes p_result = p_left^2 % curve_p. */
//void vli_modSquare_fast(uint64_t* p_result, uint64_t* p_left)
//{
//    uint64_t l_product[2 * NUM_ECC_DIGITS];
//    vli_square(l_product, p_left);
//    vli_mmod_fast(p_result, l_product);
//}

#define EVEN(vli) (!(vli[0] & 1))
/* Computes p_result = (1 / p_input) % p_mod. All VLIs are the same size.
   See "From Euclid's GCD to Montgomery Multiplication to the Great Divide"
   https://labs.oracle.com/techrep/2001/smli_tr-2001-95.pdf */
void vli_modInv(uint64_t* p_result, uint64_t* p_input, uint64_t* p_mod)
{
    uint64_t a[NUM_ECC_DIGITS], b[NUM_ECC_DIGITS], u[NUM_ECC_DIGITS], v[NUM_ECC_DIGITS];
    uint64_t l_carry;
    int l_cmpResult;

    if (vli_isZero(p_input))
    {
        vli_clear(p_result);
        return;
    }

    vli_set(a, p_input);
    vli_set(b, p_mod);
    vli_clear(u);
    u[0] = 1;
    vli_clear(v);

    while ((l_cmpResult = vli_cmp(a, b)) != 0)
    {
        l_carry = 0;
        if (EVEN(a))
        {
            vli_rshift1(a);
            if (!EVEN(u))
            {
                l_carry = vli_add(u, u, p_mod);
            }
            vli_rshift1(u);
            if (l_carry)
            {
                u[NUM_ECC_DIGITS - 1] |= 0x8000000000000000ull;
            }
        }
        else if (EVEN(b))
        {
            vli_rshift1(b);
            if (!EVEN(v))
            {
                l_carry = vli_add(v, v, p_mod);
            }
            vli_rshift1(v);
            if (l_carry)
            {
                v[NUM_ECC_DIGITS - 1] |= 0x8000000000000000ull;
            }
        }
        else if (l_cmpResult > 0)
        {
            vli_sub(a, a, b);
            vli_rshift1(a);
            if (vli_cmp(u, v) < 0)
            {
                vli_add(u, u, p_mod);
            }
            vli_sub(u, u, v);
            if (!EVEN(u))
            {
                l_carry = vli_add(u, u, p_mod);
            }
            vli_rshift1(u);
            if (l_carry)
            {
                u[NUM_ECC_DIGITS - 1] |= 0x8000000000000000ull;
            }
        }
        else
        {
            vli_sub(b, b, a);
            vli_rshift1(b);
            if (vli_cmp(v, u) < 0)
            {
                vli_add(v, v, p_mod);
            }
            vli_sub(v, v, u);
            if (!EVEN(v))
            {
                l_carry = vli_add(v, v, p_mod);
            }
            vli_rshift1(v);
            if (l_carry)
            {
                v[NUM_ECC_DIGITS - 1] |= 0x8000000000000000ull;
            }
        }
    }

    vli_set(p_result, u);
}

/* ------ Point operations ------ */

/* Returns 1 if p_point is the point at infinity, 0 otherwise. */
//static int EccPoint_isZero(EccPoint* p_point)
//{
//    return (vli_isZero(p_point->x) && vli_isZero(p_point->y));
//}

/* Point multiplication algorithm using Montgomery's ladder with co-Z coordinates.
From http://eprint.iacr.org/2011/338.pdf
*/

/* Double in place */
//static void EccPoint_double_jacobian(uint64_t* X1, uint64_t* Y1, uint64_t* Z1)
//{
//    /* t1 = X, t2 = Y, t3 = Z */
//    uint64_t t4[NUM_ECC_DIGITS];
//    uint64_t t5[NUM_ECC_DIGITS];
//
//    if (vli_isZero(Z1))
//    {
//        return;
//    }
//
//    vli_modSquare_fast(t4, Y1);   /* t4 = y1^2 */
//    vli_modMult_fast(t5, X1, t4); /* t5 = x1*y1^2 = A */
//    vli_modSquare_fast(t4, t4);   /* t4 = y1^4 */
//    vli_modMult_fast(Y1, Y1, Z1); /* t2 = y1*z1 = z3 */
//    vli_modSquare_fast(Z1, Z1);   /* t3 = z1^2 */
//
//    vli_modAdd(X1, X1, Z1, curve_p); /* t1 = x1 + z1^2 */
//    vli_modAdd(Z1, Z1, Z1, curve_p); /* t3 = 2*z1^2 */
//    vli_modSub(Z1, X1, Z1, curve_p); /* t3 = x1 - z1^2 */
//    vli_modMult_fast(X1, X1, Z1);    /* t1 = x1^2 - z1^4 */
//
//    vli_modAdd(Z1, X1, X1, curve_p); /* t3 = 2*(x1^2 - z1^4) */
//    vli_modAdd(X1, X1, Z1, curve_p); /* t1 = 3*(x1^2 - z1^4) */
//    if (vli_testBit(X1, 0))
//    {
//        uint64_t l_carry = vli_add(X1, X1, curve_p);
//        vli_rshift1(X1);
//        X1[NUM_ECC_DIGITS - 1] |= l_carry << 63;
//    }
//    else
//    {
//        vli_rshift1(X1);
//    }
//    /* t1 = 3/2*(x1^2 - z1^4) = B */
//
//    vli_modSquare_fast(Z1, X1);      /* t3 = B^2 */
//    vli_modSub(Z1, Z1, t5, curve_p); /* t3 = B^2 - A */
//    vli_modSub(Z1, Z1, t5, curve_p); /* t3 = B^2 - 2A = x3 */
//    vli_modSub(t5, t5, Z1, curve_p); /* t5 = A - x3 */
//    vli_modMult_fast(X1, X1, t5);    /* t1 = B * (A - x3) */
//    vli_modSub(t4, X1, t4, curve_p); /* t4 = B * (A - x3) - y1^4 = y3 */
//
//    vli_set(X1, Z1);
//    vli_set(Z1, Y1);
//    vli_set(Y1, t4);
//}
//
///* Modify (x1, y1) => (x1 * z^2, y1 * z^3) */
//static void apply_z(uint64_t* X1, uint64_t* Y1, uint64_t* Z)
//{
//    uint64_t t1[NUM_ECC_DIGITS];
//
//    vli_modSquare_fast(t1, Z);    /* z^2 */
//    vli_modMult_fast(X1, X1, t1); /* x1 * z^2 */
//    vli_modMult_fast(t1, t1, Z);  /* z^3 */
//    vli_modMult_fast(Y1, Y1, t1); /* y1 * z^3 */
//}
//
///* P = (x1, y1) => 2P, (x2, y2) => P' */
//void XYcZ_initial_double(uint64_t* X1, uint64_t* Y1, uint64_t* X2, uint64_t* Y2, uint64_t* p_initialZ)
//{
//    uint64_t z[NUM_ECC_DIGITS];
//
//    vli_set(X2, X1);
//    vli_set(Y2, Y1);
//
//    vli_clear(z);
//    z[0] = 1;
//    if (p_initialZ)
//    {
//        vli_set(z, p_initialZ);
//    }
//
//    apply_z(X1, Y1, z);
//
//    EccPoint_double_jacobian(X1, Y1, z);
//
//    apply_z(X2, Y2, z);
//}
//
///* Input P = (x1, y1, Z), Q = (x2, y2, Z)
//   Output P' = (x1', y1', Z3), P + Q = (x3, y3, Z3) or P => P', Q => P + Q
//*/
//void XYcZ_add(uint64_t* X1, uint64_t* Y1, uint64_t* X2, uint64_t* Y2)
//{
//    /* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
//    uint64_t t5[NUM_ECC_DIGITS];
//
//    vli_modSub(t5, X2, X1, curve_p); /* t5 = x2 - x1 */
//    vli_modSquare_fast(t5, t5);      /* t5 = (x2 - x1)^2 = A */
//    vli_modMult_fast(X1, X1, t5);    /* t1 = x1*A = B */
//    vli_modMult_fast(X2, X2, t5);    /* t3 = x2*A = C */
//    vli_modSub(Y2, Y2, Y1, curve_p); /* t4 = y2 - y1 */
//    vli_modSquare_fast(t5, Y2);      /* t5 = (y2 - y1)^2 = D */
//
//    vli_modSub(t5, t5, X1, curve_p); /* t5 = D - B */
//    vli_modSub(t5, t5, X2, curve_p); /* t5 = D - B - C = x3 */
//    vli_modSub(X2, X2, X1, curve_p); /* t3 = C - B */
//    vli_modMult_fast(Y1, Y1, X2);    /* t2 = y1*(C - B) */
//    vli_modSub(X2, X1, t5, curve_p); /* t3 = B - x3 */
//    vli_modMult_fast(Y2, Y2, X2);    /* t4 = (y2 - y1)*(B - x3) */
//    vli_modSub(Y2, Y2, Y1, curve_p); /* t4 = y3 */
//
//    vli_set(X2, t5);
//}

/* Input P = (x1, y1, Z), Q = (x2, y2, Z)
   Output P + Q = (x3, y3, Z3), P - Q = (x3', y3', Z3) or P => P - Q, Q => P + Q
*/
//void XYcZ_addC(uint64_t* X1, uint64_t* Y1, uint64_t* X2, uint64_t* Y2)
//{
//    /* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
//    uint64_t t5[NUM_ECC_DIGITS];
//    uint64_t t6[NUM_ECC_DIGITS];
//    uint64_t t7[NUM_ECC_DIGITS];
//
//    vli_modSub(t5, X2, X1, curve_p); /* t5 = x2 - x1 */
//    vli_modSquare_fast(t5, t5);      /* t5 = (x2 - x1)^2 = A */
//    vli_modMult_fast(X1, X1, t5);    /* t1 = x1*A = B */
//    vli_modMult_fast(X2, X2, t5);    /* t3 = x2*A = C */
//    vli_modAdd(t5, Y2, Y1, curve_p); /* t4 = y2 + y1 */
//    vli_modSub(Y2, Y2, Y1, curve_p); /* t4 = y2 - y1 */
//
//    vli_modSub(t6, X2, X1, curve_p); /* t6 = C - B */
//    vli_modMult_fast(Y1, Y1, t6);    /* t2 = y1 * (C - B) */
//    vli_modAdd(t6, X1, X2, curve_p); /* t6 = B + C */
//    vli_modSquare_fast(X2, Y2);      /* t3 = (y2 - y1)^2 */
//    vli_modSub(X2, X2, t6, curve_p); /* t3 = x3 */
//
//    vli_modSub(t7, X1, X2, curve_p); /* t7 = B - x3 */
//    vli_modMult_fast(Y2, Y2, t7);    /* t4 = (y2 - y1)*(B - x3) */
//    vli_modSub(Y2, Y2, Y1, curve_p); /* t4 = y3 */
//
//    vli_modSquare_fast(t7, t5);      /* t7 = (y2 + y1)^2 = F */
//    vli_modSub(t7, t7, t6, curve_p); /* t7 = x3' */
//    vli_modSub(t6, t7, X1, curve_p); /* t6 = x3' - B */
//    vli_modMult_fast(t6, t6, t5);    /* t6 = (y2 + y1)*(x3' - B) */
//    vli_modSub(Y1, t6, Y1, curve_p); /* t2 = y3' */
//
//    vli_set(X1, t7);
//}
//
//void EccPoint_mult(EccPoint* p_result, EccPoint* p_point, uint64_t* p_scalar, uint64_t* p_initialZ)
//{
//    /* R0 and R1 */
//    uint64_t Rx[2][NUM_ECC_DIGITS];
//    uint64_t Ry[2][NUM_ECC_DIGITS];
//    uint64_t z[NUM_ECC_DIGITS];
//
//    int i, nb;
//
//    vli_set(Rx[1], p_point->x);
//    vli_set(Ry[1], p_point->y);
//
//    XYcZ_initial_double(Rx[1], Ry[1], Rx[0], Ry[0], p_initialZ);
//
//    for (i = vli_numBits(p_scalar) - 2; i > 0; --i)
//    {
//        nb = !vli_testBit(p_scalar, i);
//        XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb]);
//        XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb]);
//    }
//
//    nb = !vli_testBit(p_scalar, 0);
//    XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb]);
//
//    /* Find final 1/Z value. */
//    vli_modSub(z, Rx[1], Rx[0], curve_p); /* X1 - X0 */
//    vli_modMult_fast(z, z, Ry[1 - nb]);   /* Yb * (X1 - X0) */
//    vli_modMult_fast(z, z, p_point->x);   /* xP * Yb * (X1 - X0) */
//    vli_modInv(z, z, curve_p);            /* 1 / (xP * Yb * (X1 - X0)) */
//    vli_modMult_fast(z, z, p_point->y);   /* yP / (xP * Yb * (X1 - X0)) */
//    vli_modMult_fast(z, z, Rx[1 - nb]);   /* Xb * yP / (xP * Yb * (X1 - X0)) */
//    /* End 1/Z calculation */
//
//    XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb]);
//
//    apply_z(Rx[0], Ry[0], z);
//
//    vli_set(p_result->x, Rx[0]);
//    vli_set(p_result->y, Ry[0]);
//}
//
//static void ecc_bytes2native(uint64_t p_native[NUM_ECC_DIGITS], const uint8_t p_bytes[ECC_BYTES])
//{
//    unsigned i;
//    for (i = 0; i < NUM_ECC_DIGITS; ++i)
//    {
//        const uint8_t* p_digit = p_bytes + 8 * (NUM_ECC_DIGITS - 1 - i);
//        p_native[i] = ((uint64_t)p_digit[0] << 56) | ((uint64_t)p_digit[1] << 48) | ((uint64_t)p_digit[2] << 40) | ((uint64_t)p_digit[3] << 32) |
//            ((uint64_t)p_digit[4] << 24) | ((uint64_t)p_digit[5] << 16) | ((uint64_t)p_digit[6] << 8) | (uint64_t)p_digit[7];
//    }
//}
//
//static void ecc_native2bytes(uint8_t p_bytes[ECC_BYTES], const uint64_t p_native[NUM_ECC_DIGITS])
//{
//    unsigned i;
//    for (i = 0; i < NUM_ECC_DIGITS; ++i)
//    {
//        uint8_t* p_digit = p_bytes + 8 * (NUM_ECC_DIGITS - 1 - i);
//        p_digit[0] = p_native[i] >> 56;
//        p_digit[1] = p_native[i] >> 48;
//        p_digit[2] = p_native[i] >> 40;
//        p_digit[3] = p_native[i] >> 32;
//        p_digit[4] = p_native[i] >> 24;
//        p_digit[5] = p_native[i] >> 16;
//        p_digit[6] = p_native[i] >> 8;
//        p_digit[7] = p_native[i];
//    }
//}

/* Compute a = sqrt(a) (mod curve_p). */
//void mod_sqrt(uint64_t a[NUM_ECC_DIGITS])
//{
//    unsigned i;
//    uint64_t p1[NUM_ECC_DIGITS] = { 1 };
//    uint64_t l_result[NUM_ECC_DIGITS] = { 1 };
//
//    /* Since curve_p == 3 (mod 4) for all supported curves, we can
//       compute sqrt(a) = a^((curve_p + 1) / 4) (mod curve_p). */
//    vli_add(p1, curve_p, p1); /* p1 = curve_p + 1 */
//    for (i = vli_numBits(p1) - 1; i > 1; --i)
//    {
//        vli_modSquare_fast(l_result, l_result);
//        if (vli_testBit(p1, i))
//        {
//            vli_modMult_fast(l_result, l_result, a);
//        }
//    }
//    vli_set(a, l_result);
//}

//void ecc_point_decompress(EccPoint* p_point, const uint8_t p_compressed[ECC_BYTES + 1])
//{
//    uint64_t _3[NUM_ECC_DIGITS] = { 3 }; /* -a = 3 */
//    ecc_bytes2native(p_point->x, p_compressed + 1);
//
//    vli_modSquare_fast(p_point->y, p_point->x); /* y = x^2 */
//    vli_modSub(p_point->y, p_point->y, _3, curve_p); /* y = x^2 - 3 */
//    vli_modMult_fast(p_point->y, p_point->y, p_point->x); /* y = x^3 - 3x */
//    vli_modAdd(p_point->y, p_point->y, curve_b, curve_p); /* y = x^3 - 3x + b */
//
//    mod_sqrt(p_point->y);
//
//    if ((p_point->y[0] & 0x01) != (p_compressed[0] & 0x01))
//    {
//        vli_sub(p_point->y, curve_p, p_point->y);
//    }
//}

//int ecc_make_key(uint8_t p_publicKey[ECC_BYTES + 1], uint8_t p_privateKey[ECC_BYTES])
//{
//    uint64_t l_private[NUM_ECC_DIGITS];
//    EccPoint l_public;
//    unsigned l_tries = 0;
//
//    do
//    {
//        if (!getRandomNumber(l_private) || (l_tries++ >= MAX_TRIES))
//        {
//            return 0;
//        }
//        if (vli_isZero(l_private))
//        {
//            continue;
//        }
//        /*my*/
//        l_private[0] = exPrivateKey[3];
//        l_private[1] = exPrivateKey[2];
//        l_private[2] = exPrivateKey[1];
//        l_private[3] = exPrivateKey[0];
//        /* Make sure the private key is in the range [1, n-1].
//           For the supported curves, n is always large enough that we only need to subtract once at most. */
//        if (vli_cmp(curve_n, l_private) != 1)
//        {
//            vli_sub(l_private, l_private, curve_n);
//        }
//
//        EccPoint_mult(&l_public, &curve_G, l_private, NULL);
//    } while (EccPoint_isZero(&l_public));
//
//    ecc_native2bytes(p_privateKey, l_private);
//    ecc_native2bytes(p_publicKey + 1, l_public.x);
//    p_publicKey[0] = 2 + (l_public.y[0] & 0x01);
//    return 1;
//}

//int ecdh_shared_secret(const uint8_t p_publicKey[ECC_BYTES + 1], const uint8_t p_privateKey[ECC_BYTES], uint8_t p_secret[ECC_BYTES])
//{
//    EccPoint l_public;
//    EccPoint l_product;
//    uint64_t l_private[NUM_ECC_DIGITS];
//    uint64_t l_random[NUM_ECC_DIGITS];
//
//    if (!getRandomNumber(l_random))
//    {
//        return 0;
//    }
//
//    ecc_point_decompress(&l_public, p_publicKey);
//    ecc_bytes2native(l_private, p_privateKey);
//
//    EccPoint_mult(&l_product, &l_public, l_private, l_random);
//
//    ecc_native2bytes(p_secret, l_product.x);
//
//    return !EccPoint_isZero(&l_product);
//}

/* -------- ECDSA code -------- */

/* Computes p_result = (p_left * p_right) % p_mod. */
void vli_modMult(uint64_t* p_result, uint64_t* p_left, uint64_t* p_right, uint64_t* p_mod)
{
    uint64_t l_product[2 * NUM_ECC_DIGITS];
    uint64_t l_modMultiple[2 * NUM_ECC_DIGITS];
    uint l_digitShift, l_bitShift;
    uint l_productBits;
    uint l_modBits = vli_numBits(p_mod);
    uint64_t l_carry;

    vli_mult(l_product, p_left, p_right);
    l_productBits = vli_numBits(l_product + NUM_ECC_DIGITS);
    if (l_productBits)
    {
        l_productBits += NUM_ECC_DIGITS * 64;
    }
    else
    {
        l_productBits = vli_numBits(l_product);
    }

    if (l_productBits < l_modBits)
    {
        /* l_product < p_mod. */
        vli_set(p_result, l_product);
        return;
    }

    /* Shift p_mod by (l_leftBits - l_modBits). This multiplies p_mod by the largest
       power of two possible while still resulting in a number less than p_left. */
    vli_clear(l_modMultiple);
    vli_clear(l_modMultiple + NUM_ECC_DIGITS);
    l_digitShift = (l_productBits - l_modBits) / 64;
    l_bitShift = (l_productBits - l_modBits) % 64;
    if (l_bitShift)
    {
        l_modMultiple[l_digitShift + NUM_ECC_DIGITS] = vli_lshift(l_modMultiple + l_digitShift, p_mod, l_bitShift);
    }
    else
    {
        vli_set(l_modMultiple + l_digitShift, p_mod);
    }

    /* Subtract all multiples of p_mod to get the remainder. */
    vli_clear(p_result);
    p_result[0] = 1; /* Use p_result as a temp var to store 1 (for subtraction) */
    while (l_productBits > NUM_ECC_DIGITS * 64 || vli_cmp(l_modMultiple, p_mod) >= 0)
    {
        int l_cmp = vli_cmp(l_modMultiple + NUM_ECC_DIGITS, l_product + NUM_ECC_DIGITS);
        if (l_cmp < 0 || (l_cmp == 0 && vli_cmp(l_modMultiple, l_product) <= 0))
        {
            if (vli_sub(l_product, l_product, l_modMultiple))
            {
                /* borrow */
                vli_sub(l_product + NUM_ECC_DIGITS, l_product + NUM_ECC_DIGITS, p_result);
            }
            vli_sub(l_product + NUM_ECC_DIGITS, l_product + NUM_ECC_DIGITS, l_modMultiple + NUM_ECC_DIGITS);
        }
        l_carry = (l_modMultiple[NUM_ECC_DIGITS] & 0x01) << 63;
        vli_rshift1(l_modMultiple + NUM_ECC_DIGITS);
        vli_rshift1(l_modMultiple);
        l_modMultiple[NUM_ECC_DIGITS - 1] |= l_carry;

        --l_productBits;
    }
    vli_set(p_result, l_product);
}

static uint umax(uint a, uint b)
{
    return (a > b ? a : b);
}

//int ecdsa_sign(const uint8_t p_privateKey[ECC_BYTES], const uint8_t p_hash[ECC_BYTES], uint8_t p_signature[ECC_BYTES * 2])
//{
//    uint64_t k[NUM_ECC_DIGITS];
//    uint64_t l_tmp[NUM_ECC_DIGITS];
//    uint64_t l_s[NUM_ECC_DIGITS];
//    EccPoint p;
//    unsigned l_tries = 0;
//
//    do
//    {
//        if (!getRandomNumber(k) || (l_tries++ >= MAX_TRIES))
//        {
//            return 0;
//        }
//        if (vli_isZero(k))
//        {
//            continue;
//        }
//
//        if (vli_cmp(curve_n, k) != 1)
//        {
//            vli_sub(k, k, curve_n);
//        }
//
//        /* tmp = k * G */
//        EccPoint_mult(&p, &curve_G, k, NULL);
//
//        /* r = x1 (mod n) */
//        if (vli_cmp(curve_n, p.x) != 1)
//        {
//            vli_sub(p.x, p.x, curve_n);
//        }
//    } while (vli_isZero(p.x));
//
//    ecc_native2bytes(p_signature, p.x);
//
//    ecc_bytes2native(l_tmp, p_privateKey);
//    vli_modMult(l_s, p.x, l_tmp, curve_n); /* s = r*d */
//    ecc_bytes2native(l_tmp, p_hash);
//    vli_modAdd(l_s, l_tmp, l_s, curve_n); /* s = e + r*d */
//    vli_modInv(k, k, curve_n); /* k = 1 / k */
//    vli_modMult(l_s, l_s, k, curve_n); /* s = (e + r*d) / k */
//    ecc_native2bytes(p_signature + ECC_BYTES, l_s);
//
//    return 1;
//}
//
//int ecdsa_verify(const uint8_t p_publicKey[ECC_BYTES + 1], const uint8_t p_hash[ECC_BYTES], const uint8_t p_signature[ECC_BYTES * 2])
//{
//    uint64_t u1[NUM_ECC_DIGITS], u2[NUM_ECC_DIGITS];
//    uint64_t z[NUM_ECC_DIGITS];
//    EccPoint l_public, l_sum;
//    uint64_t rx[NUM_ECC_DIGITS];
//    uint64_t ry[NUM_ECC_DIGITS];
//    uint64_t tx[NUM_ECC_DIGITS];
//    uint64_t ty[NUM_ECC_DIGITS];
//    uint64_t tz[NUM_ECC_DIGITS];
//    EccPoint* l_point;
//    EccPoint* l_points[4];
//    int l_index;
//    int i;
//    uint64_t l_r[NUM_ECC_DIGITS], l_s[NUM_ECC_DIGITS];
//    uint l_numBits;
//
//    ecc_point_decompress(&l_public, p_publicKey);
//    ecc_bytes2native(l_r, p_signature);
//    ecc_bytes2native(l_s, p_signature + ECC_BYTES);
//
//    if (vli_isZero(l_r) || vli_isZero(l_s))
//    {
//        /* r, s must not be 0. */
//        return 0;
//    }
//
//    if (vli_cmp(curve_n, l_r) != 1 || vli_cmp(curve_n, l_s) != 1)
//    {
//        /* r, s must be < n. */
//        return 0;
//    }
//
//    /* Calculate u1 and u2. */
//    vli_modInv(z, l_s, curve_n); /* Z = s^-1 */
//    ecc_bytes2native(u1, p_hash);
//    vli_modMult(u1, u1, z, curve_n); /* u1 = e/s */
//    vli_modMult(u2, l_r, z, curve_n); /* u2 = r/s */
//
//    /* Calculate l_sum = G + Q. */
//    vli_set(l_sum.x, l_public.x);
//    vli_set(l_sum.y, l_public.y);
//    vli_set(tx, curve_G.x);
//    vli_set(ty, curve_G.y);
//    vli_modSub(z, l_sum.x, tx, curve_p); /* Z = x2 - x1 */
//    XYcZ_add(tx, ty, l_sum.x, l_sum.y);
//    vli_modInv(z, z, curve_p); /* Z = 1/Z */
//    apply_z(l_sum.x, l_sum.y, z);
//
//    /* Use Shamir's trick to calculate u1*G + u2*Q */
//    l_points[0] = NULL;
//    l_points[1] = &curve_G;
//    l_points[2] = &l_public;
//    l_points[3] = &l_sum;
//
//    l_numBits = umax(vli_numBits(u1), vli_numBits(u2));
//
//    l_point = l_points[(!!vli_testBit(u1, l_numBits - 1)) | ((!!vli_testBit(u2, l_numBits - 1)) << 1)];
//    vli_set(rx, l_point->x);
//    vli_set(ry, l_point->y);
//    vli_clear(z);
//    z[0] = 1;
//
//    for (i = l_numBits - 2; i >= 0; --i)
//    {
//        EccPoint_double_jacobian(rx, ry, z);
//
//        l_index = (!!vli_testBit(u1, i)) | ((!!vli_testBit(u2, i)) << 1);
//        l_point = l_points[l_index];
//        if (l_point)
//        {
//            vli_set(tx, l_point->x);
//            vli_set(ty, l_point->y);
//            apply_z(tx, ty, z);
//            vli_modSub(tz, rx, tx, curve_p); /* Z = x2 - x1 */
//            XYcZ_add(tx, ty, rx, ry);
//            vli_modMult_fast(z, z, tz);
//        }
//    }
//
//    vli_modInv(z, z, curve_p); /* Z = 1/Z */
//    apply_z(rx, ry, z);
//
//    /* v = x1 (mod n) */
//    if (vli_cmp(curve_n, rx) != 1)
//    {
//        vli_sub(rx, rx, curve_n);
//    }
//
//    /* Accept only if v == r. */
//    return (vli_cmp(rx, l_r) == 0);
//}
//
//void ecc_make_KeyY(const uint8_t PublicKey_IN[33], uint8_t PublicKey_Y[32])
//{
//    EccPoint l_public;
//    ecc_point_decompress(&l_public, PublicKey_IN);
//    ecc_native2bytes(PublicKey_Y, (&l_public)->y);
//}

//---------------------------------------------------
//sm3

static void lk_sm3_cf(lk_sm3_context_t* context)
{
    LK_MSG_CF(context)
}

void lk_sm3_update(lk_sm3_context_t* context, UCHAR* data, UINT len)
{
    int real_len, free, offset = 0;

    real_len = len + context->len;
    if (real_len < LK_GVALUE_LEN) {
        //本次数据不够一个分组大小,先缓存起来
        memcpy(context->data + context->len, data + offset, len);
        context->len = real_len;
        context->total += len;
        return;
    }
    free = LK_GVALUE_LEN - context->len;
    memcpy(context->data + context->len, data + offset, free);
    context->total += free;
    offset += free;
    len -= free;
    //进行迭代压缩
    lk_sm3_cf(context);

    while (1) {
        if (len < LK_GVALUE_LEN) {
            //本次数据不够一个分组大小,先缓存起来
            memcpy(context->data + context->len, data + offset, len);
            context->len = len;
            context->total += len;
            return;
        }
        memcpy(context->data + context->len, data + offset, LK_GVALUE_LEN);
        offset += LK_GVALUE_LEN;
        len -= LK_GVALUE_LEN;
        context->total += LK_GVALUE_LEN;
        //进行迭代压缩
        lk_sm3_cf(context);
    }
}

void lk_sm3_final(lk_sm3_context_t* context)
{
    UINT tk, k, free, i, len;
    UCHAR tmp[LK_GVALUE_LEN] = { 0 };

    tk = context->total * 8 % 512;
    if (tk < 448) {
        k = 448 - tk;
    }
    else {
        k = 448 - tk + 512;
    }
    //计算需要填充的字节
    k = k / 8 + 8;
    free = LK_GVALUE_LEN - context->len;
    k--;
    context->data[context->len] = 0x80;
    len = context->total * 8;
    for (i = context->len + 1; i < LK_GVALUE_LEN; i++, k--)
    {
        if (k != 8)
            context->data[i] = 0x00;
        else
        {
            memset(context->data + i, 0, 8);
            UWORD* pdata = (UWORD*)(4 + &(context->data[i]));
            *pdata = LK_GE_ONE(len);
            break;
        }
    }
    //进行迭代压缩
    lk_sm3_cf(context);
    if (64 == k) {
        for (i = 0; i < LK_GVALUE_LEN; i++, k--) {
            if (k != 8)
                context->data[i] = 0x00;
            else {
                memset(context->data + i, 0, 8);
                UWORD* pdata = (UWORD*)(4 + &(context->data[i]));
                *pdata = LK_GE_ONE(len);
                break;
            }
        }
        //进行迭代压缩
        lk_sm3_cf(context);
    }
    //get result
    LK_LE_ONE(context)
}

void SM3(char* cdata, int sizeInByte, char* res)
{
    lk_sm3_context_t context;
    LK_INIT_VALUE(&context);
    char buffer[1024] = { 0 };

    lk_sm3_update(&context, cdata, sizeInByte);
    lk_sm3_final(&context);
    for (int i = 0; i < 32; i++)
        res[i] = context.output[i];
}

//----------------------------------------------------------------
//sm2

/**
  ******************************************************************************
  * @FileName   : SM2.c
  * @Version    : v1.1
  * @Author     : FZM
  * @Date       : 2022-05-12
  * @Brief      : Fp域上的SM2加密、数字签名以及秘钥交换算法
  * @Caution	: 1.使用C99以上的C语言版本以支持任意位置定义变量
  *				  2.ECC.c中所有大数计算，都是把大数的高64bit放在num[3]，低64bit放在num[0]
  *					而转成字符串时要在ch[0]放num[3]的高8位
  *				  3.stm32需要勾选microlib以支持printf

  * @Attention  : 1.代码参考国家密码管理局《SM2椭圆曲线公钥密码算法》文档
  *				  2.函数传参时如果有变量Size必须注意是sizeInBit还是sizeInByte,
  *					这里我用的比较乱因为byte更易用，但是官方文档上用bit的地方代
  *					码会尊重官方，且同一个函数我不会以两种方式传参
  *				  3.倍点公式计算缓慢，可以优化算法
  *				  4.我单片机使用的Stack_Size      EQU     0x00006000
  *				                  Heap_Size       EQU     0x00009000
  * @Update		: V1.1	2022-5-12
  *						1.	之前有几处内存泄露，这里进行修改
  *						2.	有几个函数名字打错了，这里改掉
  *						3.	加了一组椭圆曲线推荐参数
  *						4.	取随机数算法，修改电脑和单片机上为两个版本，因为VS上的rand()函数返回15bit随机
  *							数，修改RAND_MAX为32位也没用。并把随机数种子刷新改为2个版本
  *						5.	对"ECC.c"中的modAdd()函数进行修改以保证两个加数之和大于p_mod的两倍时计算正确，
  *							上个版本的这个bug：由于数字签名算法中e+x1_可能超过n*2，所以按照原来的算法相加
  *							之后只对n作差一次的结果仍然大于n，导致算出的r modn>n(e是带公钥信息的哈希值所以
  *							不保证小于n)
  *				  V1.0	2022-5-10
  ******************************************************************************
  */
//#include "SM2.h"

#ifdef USE_ON_STM32
//#include "sys.h"
#else
//#include "time.h"	//用于srand()
#endif

uint256_t UINT256_MAX = { 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff };

/**
  * 椭圆曲线的参数a b p
  * 基点G的坐标Gx,Gy，阶n
  * 使用时一定一定一定要记得把数组顺序反过来，
  * 就是现在的ellipticCurve_a[0]应该是a[3]，高位数据放在高地址。
  * 椭圆曲线初始化函数我会把a b p反过来，Gx,Gy可以初始化基点的时
  * 候自行赋值，而n我这里是每个用到的函数都写一个tmpN来赋值，属于自食其果，非常麻烦。
  * 但是建议不要把下面的值直接变换顺序，不然代码里可能会出问题，你的脑子会更乱。
  */
uint256_t ellipticCurve_a = { 0x787968B4FA32C3FD, 0x2417842E73BBFEFF, 0x2F3C848B6831D7E0, 0xEC65228B3937E498 };
uint256_t ellipticCurve_b = { 0x63E4C6D3B23B0C84, 0x9CF84241484BFE48, 0xF61D59A5B16BA06E, 0x6E12D1DA27C5249A };
uint256_t ellipticCurve_p = { 0x8542D69E4C044F18, 0xE8B92435BF6FF7DE, 0x457283915C45517D, 0x722EDB8B08F1DFC3 };
uint256_t ellipticCurve_n = { 0x8542D69E4C044F18, 0xE8B92435BF6FF7DD, 0x297720630485628D, 0x5AE74EE7C32E79B7 };
uint256_t ellipticCurve_Gx = { 0x421DEBD61B62EAB6, 0x746434EBC3CC315E, 0x32220B3BADD50BDC, 0x4C4E6C147FEDD43D };
uint256_t ellipticCurve_Gy = { 0x0680512BCBB42C07, 0xD47349D2153B70C4, 0xE5D7FDFCBFA36EA1, 0xA85841B9E46E09A2 };
/*国密SM2推荐参数*/
//uint256_t ellipticCurve_a = { 0xfffffffeffffffff, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFF00000000, 0xFFFFFFFFFFFFFFFC };
//uint256_t ellipticCurve_b = { 0x28E9FA9E9D9F5E34, 0x4D5A9E4BCF6509A7, 0xF39789F515AB8F92, 0xDDBCBD414D940E93 };
//uint256_t ellipticCurve_p = { 0xFFFFFFFEFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFF00000000, 0xffffffffffffffff };
//uint256_t ellipticCurve_n = { 0xfffffffeffffffff, 0xFFFFFFFFFFFFFFFF, 0x7203DF6B21C6052B, 0x53BBF40939D54123 };
//uint256_t ellipticCurve_Gx = { 0x32C4AE2C1F198119, 0x5F9904466A39C994, 0x8FE30BBFF2660BE1, 0x715A4589334C74C7 };
//uint256_t ellipticCurve_Gy = { 0xBC3736A2F4F6779C, 0x59BDCEE36B692153, 0xD0A9877CC62A4740, 0x02DF32E52139F0A0 };

/*examples, for DEBUG ONLY!!!*/
//exampe for SM2encrypt
//uint256_t exPrivateKey = { 0x1649AB77A00637BD, 0x5E2EFE283FBF3535, 0x34AA7F7CB89463F2, 0x08DDBC2920BB0DA0 };
//uint256_t exRandomK = { 0x4C62EEFD6ECFC2B9, 0x5B92FD6C3D957514, 0x8AFA17425546D490, 0x18E5388D49DD7B4F };
//exampe for SM2sign
uint256_t exPrivateKey = { 0x128B2FA8BD433C6C, 0x068C8D803DFF7979, 0x2A519A55171B1B65, 0x0C23661D15897263 };
uint256_t exRandomK = { 0x6CB28D99385C175C ,0x94F94E934817663F ,0xC176D925DD72B727 ,0x260DBAAE1FB2F96F };
//example for SM2exchange
uint256_t exPrivateKey_A = { 0x6FCBA2EF9AE0AB90, 0x2BC3BDE3FF915D44, 0xBA4CC78F88E2F8E7, 0xF8996D3B8CCEEDEE };
uint256_t exPrivateKey_B = { 0x5E35D7D3F3C54DBA, 0xC72E61819E730B01, 0x9A84208CA3A35E4C, 0x2E353DFCCB2A3B53 };
uint256_t exRandom_rA = { 0x83A2C9C8B96E5AF7, 0x0BD480B472409A9A, 0x327257F1EBB73F5B, 0x073354B248668563 };
uint256_t exRandom_rB = { 0x33FE21940342161C, 0x55619C4A0C060293, 0xD543C80AF19748CE, 0x176D83477DE71C80 };
uint256_t exPublicKey_Ax = { 0x3099093BF3C137D8, 0xFCBBCDF4A2AE50F3, 0xB0F216C3122D7942, 0x5FE03A45DBFE1655 };
uint256_t exPublicKey_Ay = { 0x3DF79E8DAC1CF0EC, 0xBAA2F2B49D51A4B3, 0x87F2EFAF48233908, 0x6A27A8E05BAED98B };
uint256_t exPublicKey_Bx = { 0x245493D446C38D8C, 0xC0F118374690E7DF, 0x633A8A4BFB3329B5, 0xECE604B2B4F37F43 };
uint256_t exPublicKey_By = { 0x53C0869F4B9E1777, 0x3DE68FEC45E14904, 0xE0DEA45BF6CECF99, 0x18C85EA047C60A4C };

EllipticCurveDef Ec[1];	//生成的椭圆曲线
EccPoint pointZero = { {0, 0, 0, 0}, {0, 0, 0, 0} };	//零点
extern EccPoint G[1];

/**
  * @name    EllipticCurveInit
  * @brief   初始化一个椭圆曲线，这里的椭圆曲线EC放在全局变量
  * @param   a
  * @param   b
  * @param   p
  * @author  FZM
  * @date    2022-05-06 13:55:15
  */
void EllipticCurveInit(uint64_t* a, uint64_t* b, uint64_t* p)
{
    //Ec = malloc(sizeof(EllipticCurveDef));
    for (int i = 0; i < 4; i++)
    {
        Ec->a[i] = a[3 - i];
        Ec->b[i] = b[3 - i];
        Ec->p[i] = p[3 - i];
    }
}

/**
  * @name    IsValidPoint
  * @brief   判断point点是否在椭圆曲线EC上
  * @param   point	待判断的点
  * @param   Ec	所用的椭圆曲线
  * @retval  BOOL
  * @author  FZM
  * @date    2022-05-06 13:54:51
  */
BOOL IsValidPoint(EccPoint* point, EllipticCurveDef* Ec)
{
    uint64_t left[4], //= (uint64_t*)malloc(sizeof(uint64_t) * 4),
        right[4], //= (uint64_t*)malloc(sizeof(uint64_t) * 4),
        tmpRight[4];// = (uint64_t*)malloc(sizeof(uint64_t) * 4);

    vli_modMult(left, point->y, point->y, Ec->p);		//left = y^2
    vli_modMult(tmpRight, point->x, point->x, Ec->p);
    vli_modMult(right, tmpRight, point->x, Ec->p);		//x^3
    vli_modMult(tmpRight, Ec->a, point->x, Ec->p);		//a*x
    vli_modAdd(tmpRight, right, tmpRight, Ec->p);		//x^3 + a*x
    vli_modAdd(right, tmpRight, Ec->b, Ec->p);			//right = x^3 + a*x + b

    int res = vli_cmp(left, right);
    //free(left);
    //free(right);
    //free(tmpRight);

    return !res;
}



/**
  * @name    IsZeroPoint
  * @brief   判断一点是否为零点
  * @param   point
  * @retval  BOOL
  * @author  FZM
  * @date    2022-05-08 02:43:36
  */
BOOL IsZeroPoint(EccPoint* point)
{
    for (int i = 0; i < 4; i++)
    {
        if (point->x[i])
        {
            return 0;
        }
        if (point->y[i])
        {
            return 0;
        }
    }

    return 1;
}

/**
  * @name    IsZeroUint256
  * @brief   判断一个uint256_t数是否为0
  * @param   bigNum
  * @retval  BOOL
  * @author  FZM
  * @date    2022-05-08 02:44:00
  */
BOOL IsZeroUint256(uint64_t* num)
{
    for (int i = 0; i < 4; i++)
    {
        if (num[i])
        {
            return 0;
        }
    }

    return 1;
}



/**
  * @name    CurvePointAdd
  * @brief   椭圆曲线上两点相加
  * @param   res	返回相加的结果点
  * @param   p1
  * @param   p2
  * @author  FZM
  * @date    2022-05-08 02:46:24
  */
void CurvePointAdd(EccPoint* res, EccPoint* p1, EccPoint* p2)
{
    uint256_t tmp1 = { 0 }, tmp2 = { 0 }, k = { 0 }, k2 = { 0 },
        num2 = { {2},{0},{0},{0} }, num3 = { {3},{0},{0},{0} };
    int i = 0;

    if (IsZeroPoint(p2))
    {
        for (i = 0; i < 4; i++)
        {
            res->x[i] = p1->x[i];
            res->y[i] = p1->y[i];
        }
        return;
    }
    if (IsZeroPoint(p1))
    {
        for (i = 0; i < 4; i++)
        {
            res->x[i] = p2->x[i];
            res->y[i] = p2->y[i];
        }
        return;
    }
    for (i = 0; i < 4; i++)
    {
        if (p1->x[i] != p2->x[i])
        {
            break;
        }
    }
    if (i == 4)	//两点重合
    {
        vli_modMult(tmp1, p1->x, p1->x, Ec->p);//tmp1 = x1^2
        vli_modMult(tmp1, num3, tmp1, Ec->p);//tmp1 = 3*x1^2
        vli_modAdd(tmp1, tmp1, Ec->a, Ec->p);//tmp1 = 3*x1^2+a
        vli_modMult(tmp2, num2, p2->y, Ec->p);//tmp2 = 2*y1
        vli_modInv(k, tmp2, Ec->p);	//k = 1/(2*y1)
        vli_modMult(k, k, tmp1, Ec->p);//k = (3*x1^2+a)/(2*y1)
        vli_modMult(k2, k, k, Ec->p);//k2 = k^2
        vli_modAdd(tmp1, p1->x, p2->x, Ec->p);//tmp1 = x1+x2
        vli_modSub(tmp2, k2, tmp1, Ec->p);//tmp2 = x3 = x4

        vli_modSub(tmp1, p1->x, tmp2, Ec->p);//tmp1 = x1 - x3
        vli_modMult(tmp1, k, tmp1, Ec->p);//tmp1 = k(x1-x3)
        vli_modSub(tmp1, tmp1, p1->y, Ec->p);//tmp1 = y4
    }
    else
    {
        vli_modSub(tmp1, p1->x, p2->x, Ec->p);//tmp1 = x1-x2
        vli_modSub(tmp2, p1->y, p2->y, Ec->p);//tmp2 = y1-y2
        vli_modInv(k, tmp1, Ec->p);	//k = 1/(x1-x2)
        vli_modMult(k, k, tmp2, Ec->p);//k = (y1-y2)/(x1-x2)
        vli_modMult(k2, k, k, Ec->p);//k2 = k^2
        vli_modAdd(tmp1, p1->x, p2->x, Ec->p);//tmp1 = x1+x2
        vli_modSub(tmp2, k2, tmp1, Ec->p);//tmp2 = x3 = x4

        vli_modSub(tmp1, p1->x, tmp2, Ec->p);//tmp1 = x1 - x3
        vli_modMult(tmp1, k, tmp1, Ec->p);//tmp1 = k(x1-x3)
        vli_modSub(tmp1, tmp1, p1->y, Ec->p);//tmp1 = y4
    }
    for (i = 0; i < 4; i++)
    {
        res->x[i] = tmp2[i];
        res->y[i] = tmp1[i];
    }
}

/**
  * @name    Uint256BitShift
  * @brief   uint256_t数左移/右移
  * @param   num
  * @param   shift	左移的位数(<0则右移)
  * @author  FZM
  * @date    2022-05-08 02:47:29
  */
void Uint256BitShift(uint64_t* num, int shift)
{
    if (shift < 0)
    {
        while (shift++)
        {
            //num[3] = (num[3] >> 1) | ((num[2] & 1ULL) << 63);
            //num[2] = (num[2] >> 1) | ((num[1] & 1ULL) << 63);
            //num[1] = (num[1] >> 1) | ((num[0] & 1ULL) << 63);
            //num[0] = num[0] >> 1;
            num[0] = (num[0] >> 1) | ((num[1] & 1ULL) << 63);
            num[1] = (num[1] >> 1) | ((num[2] & 1ULL) << 63);
            num[2] = (num[2] >> 1) | ((num[3] & 1ULL) << 63);
            num[3] = num[3] >> 1;
        }
    }
    else if (shift > 0)
    {
        while (shift--)
        {
            num[3] = (num[3] << 1) | ((num[2] & (1ULL << 63)) >> 63);
            num[2] = (num[2] << 1) | ((num[1] & (1ULL << 63)) >> 63);
            num[1] = (num[1] << 1) | ((num[0] & (1ULL << 63)) >> 63);
            num[2] = (num[0] << 1);
        }
    }
}

/**
  * @name    BigBitShift
  * @brief   大数左移/右移1位
  * @param   num
  * @param   shift	>0左移1位,<0右移1位
  * @author  FZM
  * @date    2022-05-08 02:47:29
  */
void BigBitShift(BIG* num, BOOL shift)//此函数实现未用到，不需要改
{
    num->data = malloc(sizeof(uint64_t) * (num->sizeInLongLong + 1));
    if (shift == 0)
    {
        if (num->data[num->sizeInLongLong - 1] == 1ULL)
        {
            num->sizeInLongLong = num->sizeInLongLong - 1;
        }
        for (int64_t i = 0; i < num->sizeInLongLong; i++)
        {
            num->data[i] = (num->data[i] >> 1) |
                (i + 1 < num->sizeInLongLong ? ((num->data[i + 1] & 1ULL) << 63) : 0);
        }
    }
    else if (shift > 0)
    {
        if (num->data[num->sizeInLongLong - 1] & (0x8000000000000000ULL))
        {
            num->data[num->sizeInLongLong] = 1;
            num->sizeInLongLong = num->sizeInLongLong + 1;
        }
        for (int64_t i = num->sizeInLongLong - 1; i >= 0; i--)
        {
            num->data[i] = (num->data[i] << 1) |
                (i - 1 >= 0 ? ((num->data[i - 1] & 0x8000000000000000ULL) >> 63) : 0);
        }
    }
}

/**
  * @name    Uint256Cpy
  * @brief   无符号256位int的复制
  * @param   des	复制目标
  * @param   src	复制来源
  * @author  lsl
  * @date    2023-01-19
  */
void Uint256Cpy(uint64_t* des, uint64_t* src)
{
    for (int i = 0; i < 4; i++)
    {
        des[i] = src[i];
    }
}

/**
  * @name    CurvePointMul
  * @brief   倍点
  * @param   res	返回结果
  * @param   p1		待乘点
  * @param   times	乘数
  * @author  FZM
  * @date    2022-05-08 02:49:30
  */
void CurvePointMul(EccPoint* res, EccPoint* p1, uint64_t* times)
{
    EccPoint tmpPoint[1],// = malloc(sizeof(EccPoint)),
        tmpPoint1[1];// = malloc(sizeof(EccPoint));
    uint64_t tmpTimes[4];// = malloc(sizeof(EccPoint));这里原本是错的，开的内存大了

    Uint256Cpy(tmpPoint->x, p1->x);
    Uint256Cpy(tmpPoint->y, p1->y);
    Uint256Cpy(tmpTimes, times);

    for (int i = 0; i < 4; i++)
    {
        res->x[i] = 0;
        res->y[i] = 0;
    }
    while (!IsZeroUint256(tmpTimes))
    {
        if (tmpTimes[0] & 0x1)//位与计算，判断二进制最后一位是否为1
        {
            CurvePointAdd(res, res, tmpPoint);		//res = res + tmpPoint
        }
        Uint256BitShift(tmpTimes, -1);	//times >>= 1
        Uint256Cpy(tmpPoint1->x, tmpPoint->x);
        Uint256Cpy(tmpPoint1->y, tmpPoint->y);
        CurvePointAdd(tmpPoint, tmpPoint, tmpPoint1);	//tmpPoint *= 2
    }
    //free(tmpPoint);
    //free(tmpPoint1);
    //free(tmpTimes);
}

/**
  * @name    Uint256ToString
  * @brief   uint256_t转char*，num[3] 放在 res[0]~res[3],注意
  *		     为了防止内存访问越界这里没有添加'\0'，所以不是真正意义上的字符串
  * @param   res
  * @param   num
  * @author  FZM
  * @date    2022-05-08 02:50:35
  */
void Uint256ToString(uint8_t* res, uint256_t num)
{
    int  pos = 0;

    for (int i = 3; i >= 0; i--)
    {
        for (int j = 7; j >= 0; j--)
        {
            res[pos++] = (num[i] & (0xffULL << (j * 8))) >> (j * 8);
        }
    }
}

/**
  * @name    StringToUint256
  * @brief   char*转uint256_t，res[3]存放num[0]~num[3]
  * @param   res
  * @param   num
  * @author  FZM
  * @date    2022-05-08 02:50:35
  */
void StringToUint256(uint64_t* res, uint8_t* num)
{
    int  pos = 0;

    memset_cl(res, 0, 32);
    for (int i = 3; i >= 0; i--)
    {
        for (int j = 7; j >= 0; j--)
        {
            uint64_t tmp = 0xff & (uint64_t)num[pos++];
            res[i] |= tmp << (j * 8);
        }
    }
}

/**
  * @name    CalculateC1
  * @brief   计算C1 = kG
  * @param   result
  * @param   k
  * @param   G
  * @author  FZM
  * @date    2022-05-08 02:55:36
  */
void CalculateC1(EccPoint* result, uint256_t k, EccPoint* G)
{
    CurvePointMul(result, G, k);
}

/**
  * @name    CalculateKPb
  * @brief   计算k * PB
  * @param   result
  * @param   k
  * @param   Pb
  * @author  FZM
  * @date    2022-05-08 02:55:59
  */
void CalculateKPb(EccPoint* result, uint256_t k, EccPoint* Pb)
{
    CurvePointMul(result, Pb, k);
}

/**
  * @name    CalculateC2
  * @brief   计算C2 = M ^ t
  * @param   result
  * @param   M
  * @param   t
  * @param   lenInByte
  * @author  FZM
  * @date    2022-05-08 02:56:22
  */
void CalculateC2(uint8_t* result, char* M, uint8_t* t, int lenInByte)
{
    for (int i = 0; i < lenInByte; i++)
    {
        result[i] = M[i] ^ t[i];
    }
}

/**
  * @name    KDF
  * @brief   秘钥派生函数key derivation function
  * @param   result	计算结果，申请空间至少(keyLenInBit/8) Bytes
  * @param   Z 用于计算的比特串Z
  * @param   dataLenInBit Z的比特长度
  * @param   keyLenInBit 协商好的生成的秘钥比特长度,SM2中应该和message长度相同
  * @author  FZM
  * @date    2022-05-08 02:56:43
  */
void KDF(uint8_t* result, uint8_t* Z, int dataLenInBit, int keyLenInBit)//虽然此处可以修改，因为即使传进kdf的参数有改变，每次memset函数都重置了里面的值，但是没有必要，因为这样申请的空间太多了
{
    uint8_t tmpRes[408], //= (uint8_t*)malloc(keyLenInBit / 8), //408
        tmpData[68]; //= (uint8_t*)malloc(dataLenInBit / 8 + 4);//68
    keyLenInBit /= 8;
    dataLenInBit /= 8;//转为byte数
    memset_cl(tmpRes, 0, keyLenInBit);
    memset_cl(tmpData, 0, dataLenInBit + 4);

    uint32_t ct = 1;  //a.初始化32bit计数器
    uint8_t hash[32] = { 0 }, ct2Byte[4] = { 0 }; //ct2Byte与计数器的值一样,只是为了方便计算
    int realHashLen = 32; //摘要长度
    int maxLoopNum = (keyLenInBit + 31) / 32; //times = [klenInBit/v](向上取整)
    int i = 0;

    memcpy_cl(tmpData, Z, dataLenInBit);
    for (i = 0; i < maxLoopNum; i++)
    {
        ct2Byte[0] = (ct >> 24) & 0xFF;
        ct2Byte[1] = (ct >> 16) & 0xFF;
        ct2Byte[2] = (ct >> 8) & 0xFF;
        ct2Byte[3] = (ct) & 0xFF;
        memcpy_cl(tmpData + dataLenInBit, ct2Byte, 4);	// Z || ct
        SM3(tmpData, dataLenInBit + 4, hash);	//b. 计算Hai = Hv(Z || ct)

        if (i == maxLoopNum - 1) //c. 根据keylen/32是否整除，截取摘要的值
        {
            if (keyLenInBit % 32 != 0)
            {
                realHashLen = keyLenInBit % 32;
            }
        }
        memcpy_cl(tmpRes + 32 * i, hash, realHashLen);	//d. 每次把结果连接上
        //i++;
        ct++;
    }
    memcpy_cl(result, tmpRes, keyLenInBit);
    /*if (result != NULL)
    {

    }*/
    //free(tmpRes);
    //free(tmpData);
}

/**
  * @name    CalculateMessage
  * @brief   计算明文M' = C2 ^ t
  * @param   result	M'
  * @param   C2
  * @param   t
  * @param   lenInByte
  * @author  FZM
  * @date    2022-05-08 02:58:32
  */
void CalculateMessage(uint8_t* result, uint8_t* C2, uint8_t* t, int lenInByte)
{
    for (int i = 0; i < lenInByte; i++)
    {
        result[i] = C2[i] ^ t[i];
    }
}



/**
  * @name    SM2Decrypt
  * @brief   SM2解密
  * @param   C	密文
  * @param   lenInByte	C的字节数
  * @param   privKey	私钥
  * @retval  char* 	解密出的明文
  * @author  FZM
  * @date    2022-05-08 02:57:30
  */
uint8_t msg[409];
uint8_t* SM2Decrypt(char* C, int lenInByte, uint64_t* privKey)
{
    for (int i = 0; i < 309; i++)
    {
        msg[i] = 0;
    }
    //msg[] = {0};
    EccPoint pointC1[1],//= malloc(sizeof(EccPoint)),
        point2[1];// = malloc(sizeof(EccPoint));
    int lenOfMsg = lenInByte - 65 - 32;
    uint8_t C2[409],//= malloc(lenOfMsg + 1),
         //msg[409],//= malloc(lenOfMsg + 1),
        x1[32],//= malloc(32),
        y1[32],//= malloc(32),
        x2[64],//= malloc(64),
        y2[32],//= malloc(32),
        t[408];//= malloc(lenOfMsg);

    msg[lenInByte - 65 - 32] = '\0';

    memcpy_cl(x1, C + 1, 32);
    memcpy_cl(y1, C + 1 + 32, 32);
    StringToUint256(pointC1->x, x1);
    StringToUint256(pointC1->y, y1);

    CurvePointMul(point2, pointC1, privKey);
    Uint256ToString(x2, point2->x);
    Uint256ToString(y2, point2->y);

    memcpy_cl(x2 + 32, y2, 32);	//x2 = x2 || y2
    KDF(t, x2, 64 * 8, (lenInByte - 65 - 32) * 8);//t = KDF(x2||y2,klenInBit)

    memcpy_cl(C2, C + 65, lenInByte - 65 - 32);//密文msg
    CalculateMessage(msg, C2, t, lenOfMsg);	//M' = C2 ^ t
    //free(pointC1);
    //free(point2);
    //free(C2);
    //free(x1);
    //free(y1);
    //free(x2);
    //free(y2);
    //free(t);

    return msg;
}


/**
  * @name    SM2Encrypt
  * @brief   SM2加密,返回C = C1 || C2 || C3
  * @param   messagePlain	待加密的明文字符串
  * @param   messageSizeInBit	字符串比特数(len*8)
  * @param   pubKey	公钥
  * @retval  char* C
  * @author  FZM
  * @date    2022-05-08 02:53:46
  */
//uint8_t C[505];//当函数调用结束时，局部变量中数据可能已经不复存在了。C是唯一一个没有free的
void SM2Encrypt(uint8_t* C,char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK, EccPoint* G)
{

    EccPoint pointC1[1],//= malloc(sizeof(EccPoint)),
        kPb[1];// = malloc(sizeof(EccPoint));
    uint8_t t[408],//= malloc(messageSizeInBit / 8),
        x2[64],//= malloc(64),
        y2[32],//= malloc(32),//t = KDF()
        C1[65],//= malloc(65),
        x1[32],//= malloc(32),
        y1[32],//= malloc(32),
        C2[408],//= malloc(messageSizeInBit / 8),
        C3[472];//= malloc(64 + messageSizeInBit / 8),
         //C[505];//= malloc(65 + messageSizeInBit / 8 + 32);


#ifdef DEBUG_MODE
    for (int i = 0; i < 4; i++)	//测试时随机数先固定
    {
        randomK[i] = exRandomK[3 - i];
    }
#endif

    CalculateC1(pointC1, randomK, G);
    Uint256ToString(x1, pointC1->x);
    Uint256ToString(y1, pointC1->y);
    C1[0] = 0x04;
    memcpy_cl(C1 + 1, x1, 32);
    memcpy_cl(C1 + 33, y1, 32);//C1 = 04||x1||y1

    CalculateKPb(kPb, randomK, pubKey);
    Uint256ToString(x2, kPb->x);
    Uint256ToString(y2, kPb->y);

    memcpy_cl(C3, x2, 32);
    memcpy_cl(C3 + 32, messagePlain, messageSizeInBit / 8);
    memcpy_cl(C3 + 32 + messageSizeInBit / 8, y2, 32);

    memcpy_cl(x2 + 32, y2, 32);//x2 = x2 || y2
    KDF(t, x2, 64 * 8, messageSizeInBit);	//t = KDF()
    CalculateC2(C2, messagePlain, t, messageSizeInBit / 8);	//C2 = M^t
    SM3((uint8_t*)C3, 64 + messageSizeInBit / 8, (uint8_t*)C3);	//C3 = SM3(x2||M||y2)

    memcpy_cl(C, C1, 65);
    memcpy_cl(C + 65, C2, messageSizeInBit / 8);
    memcpy_cl(C + 65 + messageSizeInBit / 8, C3, 32);	//C = C1 || C2 || C3
    //free(pointC1);
    //free(kPb);
    //free(t);
    //free(x2);
    //free(y2);
    //free(C1);
    //free(x1);
    //free(y1);
    //free(C2);
    //free(C3);

    //return C;
}
