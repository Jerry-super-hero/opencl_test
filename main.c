/**
  ******************************************************************************
  * @FileName   : main.c
  * @Version    :
  * @Author     : 付泽铭 https://blog.csdn.net/XiaoMing_
  * @Date       : 2022-05-12
  * @Brief      : Fp域上的SM2加密、数字签名以及秘钥交换算法主流程
  ******************************************************************************
  */
#include <stdio.h>
//#include <string.h>
#include"SM2Encrypt.h"
#include"OPENCL.h"
//#include "SM2.h"
//#include "SM3.h"
//#include "ECC.h"

#ifdef USE_ON_STM32
#include "delay.h"
#include "usart.h"
#endif

#pragma warning(disable:4996)

EllipticCurveDef Ec[1];
EccPoint G[1];

#define TEST_ENCRYPT
//#define TEST_SIGN
//#define TEST_EXCHANGE

/**
  * @name    GetRandomNumber
  * @brief   获得一个uint256_t随机数,因为SM2要求maxVal的最高DWORD必须有值，
  *			 所以我这里直接令result[3] %= (maxVal[3]-1)保证结果小于maxVal
  * @param   result 为获取到的随机数
  * @author  FZM
  * @date    2022-05-08 02:42:59
  */
void GetRandomNumber(uint64_t* result, uint64_t* maxVal)
{
#ifndef DEBUG_MODE
#ifndef USE_ON_STM32
    srand(clock());//srand(随机数参数),clock()(记录从程序开始运行到执行clock函数的时间)
#else
    SysTick->LOAD = 0xffffffff;	//Systick->val作为随机数种子
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    uint32_t val = SysTick->VAL;
    srand(val);
#endif	//USE_ON_STM32
#endif	//DEBUG_MODE

    for (int i = 0; i < 4; i++)
    {
#ifdef USE_ON_STM32
        result[i] = ((uint64_t)rand() << 62) | ((uint64_t)rand() << 31) | rand();
#else
        result[i] = ((uint64_t)rand() << 60) | ((uint64_t)rand() << 45) | ((uint64_t)rand() << 30) | ((uint64_t)rand() << 15) | ((uint64_t)rand() << 0);//电脑上的rand()函数返回16bit随机数，修改RAND_MAX无效
#endif
    }
    //int tmp = GetMSB(maxVal);
    //Uint256WriteBits(result, 0, tmp, 255);
    result[3] %= (maxVal[3] - 1);
}

/**
  * @name    GenerateKeys
  * @brief   产生公钥及私钥
  * @param   privKey 返回私钥
  * @param   pubKey	返回公钥
  * @param   pointG	基点G
  * @retval  BOOL 是否成功
  * @author  FZM
  * @date    2022-05-08 02:45:32
  */
BOOL GenerateKeys(uint64_t* privKey, EccPoint* pubKey, EccPoint* pointG)
{
    int maxTries = 16;

    uint256_t tmp = { 0 };
    for (int i = 0; i < 4; i++)
    {
        tmp[i] = ellipticCurve_n[3 - i];
    }
    do {
        maxTries--;
        if (maxTries == 0)
        {
            return FALSE;
        }
        GetRandomNumber(privKey, Ec->p);
#ifdef DEBUG_MODE
        for (int i = 0; i < 4; i++)//方便测试所以固定随机数
        {
            privKey[i] = exPrivateKey[3 - i];
        }
#endif
        if (IsZeroUint256(privKey))
        {
            continue;
        }
        if (vli_cmp(tmp, privKey) != 1)	//Make sure that privKey is < n
        {
            vli_sub(privKey, privKey, tmp);
        }
        CurvePointMul(pubKey, pointG, privKey);
    } while (IsZeroPoint(pubKey));

    return TRUE;
}

int main()
{
        uint64_t privKey[4];
//        uint64_t* privKey = (uint64_t*)malloc(sizeof(uint64_t) * 4);
//        sizeof(uint64_t)=8
        EccPoint pubKey[1];
//        EccPoint* pubKey = (EccPoint*)malloc(sizeof(EccPoint));
//        sizeof(EccPoint)=64
        char* testMessage = "The Little Prince is a novella by French aristocrat, writer.\n\
It was first published in English and French in the US by Reynal & Hitchcock in April 1943.\
\nThe story follows a young prince who visits various planets in space, including Earth, and addresses themes of loneliness, friendship, love, \
and loss.Despite its style as a children's book, The Little Prince makes observations about life and human nature.";
        //这里的char是signed char （int8_t）
        int lenTestMessage = strlen(testMessage);
        //char a = "a";
        //printf("%d\n", a);
#ifdef USE_ON_STM32
        uart_init(115200);
        //	delay_init();
        printf("STM32 SM2 testing...\n");
#endif
        //G = malloc(sizeof(EccPoint));
        for (int i = 0; i < 4; i++)
        {
            G->x[i] = ellipticCurve_Gx[3 - i];
            G->y[i] = ellipticCurve_Gy[3 - i];
        }

        EllipticCurveInit(ellipticCurve_a, ellipticCurve_b, ellipticCurve_p);
        if (!IsValidPoint(G, Ec))
        {
            while (1);
        }

    GenerateKey:
        if (!GenerateKeys(privKey, pubKey, G))
        {
            while (1);
        }
        if (!IsValidPoint(pubKey, Ec))
        {
            goto GenerateKey;
        }
#ifdef TEST_ENCRYPT
        uint256_t randomK = { 0 };
        uint8_t resC[505];
        GetRandomNumber(randomK, Ec->p);
        //uint256_t* randomK1=randomK;
        printf("明文：\n");
        printf("%s\n",testMessage);
        //SM2Encrypt(resC,testMessage, lenTestMessage * 8, pubKey, randomK,G);
        //printf("常规加密后内容：\n");
        //printf("%s\n",resC);

        //uint8_t* resSM2Decrypt = SM2Decrypt((char*)resC, 65 + 32 + lenTestMessage * 8 / 8, privKey);
        //printf("常规解密后内容：\n");
        //printf("%s\n",resSM2Decrypt);

        uint8_t resB[505];
        SM2Encrypt_OPENCL_NEW(resB, testMessage, lenTestMessage * 8, pubKey, randomK, G, 1);
        printf("opencl加密后内容：\n");
        printf("%s\n", resB);

        uint8_t* resSM2Decrypt = SM2Decrypt((char*)resB, 65 + 32 + lenTestMessage * 8 / 8, privKey);
        printf("opencl解密后内容：\n");
        printf("%s\n",resSM2Decrypt);
        //printf("Decryption %s\n", strcmp(testMessage, resSM2Decrypt) ? "failed" : "passed");
        //free(resC);
        //free(resSM2Decrypt);
#endif  //TEST_ENCRYPT

#ifdef TEST_SIGN
        char* IDA = "ALICE123@YAHOO.COM";
        char* m = "message digest";
        SM2Signature* sign = SM2Sign(m, 8 * strlen(m), IDA, 0x0090, G, pubKey, privKey);
        int v = SM2SignatureVerify(m, 8 * strlen(m), sign, IDA, 0x0090, G, pubKey);
        printf("Signature verification %s \n", v ? "passed" : "failed");
        //free(sign);
#endif // TEST_SIGN

#ifdef TEST_EXCHANGE
        char* IDA2 = "ALICE123@YAHOO.COM";
        char* IDB2 = "BILL456@YAHOO.COM";
        int resSM2KeyExchange = SM2KeyExchange(IDA2, 0x0090, IDB2, 0x0088, G, 128);
        printf("Key exchange %s\n", resSM2KeyExchange ? "passed" : "failed");
#endif	//TEST_EXCHANGE

//        free(pubKey);
//        free(privKey);
        //free(Ec);
        //free(G);

}
