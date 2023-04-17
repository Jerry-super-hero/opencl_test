#ifndef _OPENCL_H_
#define _OPENCL_H_

#include <stdio.h>
#include <CL/cl.h>
#include"SM2Encrypt.h"

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

void CalculateMessage_OPENCL(uint8_t* result, uint8_t* C2, uint8_t* t, int lenInByte);
void SM2Encrypt_OPENCL(uint8_t* C, char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK, EccPoint* G, int lenInByte);
void SM2Encrypt_OPENCL_NEW(uint8_t* C, char* messagePlain, int messageSizeInBit, EccPoint* pubKey, uint256_t randomK, EccPoint* G, int lenInByte);
char* read_txt_file(char* filename);
void* memcpy_c(void* dest, const void* src, size_t n);
void* memset_c(void* ptr, int value, size_t num);

#endif /* _OPENCL_H_ */
