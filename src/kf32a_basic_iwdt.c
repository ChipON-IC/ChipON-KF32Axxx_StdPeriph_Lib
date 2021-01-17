/**
  ******************************************************************************
  * 文件名  kf32a_basic_iwdt.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了独立看门狗(IWDT)相关的功能函数，包含：
  *          + 独立看门狗(IWDT)控制功能函数
  *
  *********************************************************************
  */

#include "kf32a_basic_iwdt.h"

/* 独立看门狗(IWDT)私有宏定义 --------------------------------------------------*/
/* IWDT_FD寄存器喂狗值 */
#define FEED_THE_DOG                    ((uint32_t)0x55AA55AA)

/**
  *   ##### 独立看门狗(IWDT)控制功能函数 #####
  */
/**
  * 描述  设置独立看门狗预分频，对INTLF进行分频。
  * 输入  Prescaler: 独立看门狗对INTLF的预分频值，取值为：
  *                    IWDT_PRESCALER_32: 32分频
  *                    IWDT_PRESCALER_64: 64分频
  *                    IWDT_PRESCALER_128: 128分频
  *                    IWDT_PRESCALER_256: 256分频
  *                    IWDT_PRESCALER_512: 512分频
  *                    IWDT_PRESCALER_1024: 1024分频
  *                    IWDT_PRESCALER_2048: 2048分频
  *                    IWDT_PRESCALER_4096: 4096分频
  *                    IWDT_PRESCALER_8192: 8192分频
  *                    IWDT_PRESCALER_16384: 16384分频
  *                    IWDT_PRESCALER_32768: 32768分频
  *                    IWDT_PRESCALER_65536: 65536分频
  * 返回  无。
  */
void
IWDT_Prescaler_Config (uint32_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_IWDT_PRESCALER(Prescaler));

    /*---------------- 配置IWDT_CTL寄存器IWDTPS位 ----------------*/
    IWDT_CTL = SFR_Config (IWDT_CTL, ~IWDT_CTL_IWDTPS, Prescaler);
}

/**
  * 描述  设置独立看门狗溢出值，溢出时复位。
  * 输入  Overflow: 独立看门狗溢出值，取值为0~0xFFF。
  * 返回  无。
  */
void
IWDT_Overflow_Config (uint32_t Overflow)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_IWDTPS_OVERFLOW(Overflow));

    /*---------------- 配置IWDT_CTL寄存器IWDTOVF位 ----------------*/
    tmpreg = Overflow << IWDT_CTL_IWDTOVF0_POS;
    IWDT_CTL = SFR_Config (IWDT_CTL, ~IWDT_CTL_IWDTOVF, tmpreg);
}

/**
  * 描述  设置独立看门狗使能。
  * 输入  NewState: 独立看门狗使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
IWDT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置IWDT_CTL寄存器IWDTEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能独立看门狗定时器 */
        SFR_SET_BIT_ASM(IWDT_CTL, IWDT_CTL_IWDTEN_POS);
    }
    else
    {
        /* 未使能独立看门狗 */
        SFR_CLR_BIT_ASM(IWDT_CTL, IWDT_CTL_IWDTEN_POS);
    }
}

/**
  * 描述  独立看门狗喂狗。
  * 输入  无。
  * 返回  无。
  */
void
IWDT_Feed_The_Dog (void)
{
    IWDT_FD = FEED_THE_DOG;
}
/**
  *   ##### 独立看门狗(IWDT)控制功能函数定义结束 #####
  */
