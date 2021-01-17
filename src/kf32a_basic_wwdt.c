/**
  ******************************************************************************
  * 文件名  kf32a_basic_wwdt.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了窗口看门狗(WWDT)相关的功能函数，包含：
  *          + 窗口看门狗(WWDT)控制功能函数
  *          + 窗口看门狗(WWDT)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_wwdt.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/**
  *   ##### 窗口看门狗(WWDT)控制功能函数 #####
  */
/**
  * 描述  复位WWDT模块，使能外设时钟。
  * 输入  无。
  * 返回  无。
  */
void
WWDT_Reset (void)
{
    /* 复位WWDT模块 */
    RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_WWDTRST, TRUE);
    RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_WWDTRST, FALSE);

    /* 使能WWDT时钟 */
    PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_WWDTCLKEN, TRUE);
}

/**
  * 描述  设置窗口看门狗可操作区下限值。
  * 输入  Threshold: 窗口看门狗可操作区下限值，取值为0~0x3F。
  * 返回  无。
  */
void
WWDT_Threshold_Config (uint32_t Threshold)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_WWDT_THRESHOLD(Threshold));

    /*---------------- 配置WWDT_CTL寄存器WW位 ----------------*/
    tmpreg = Threshold << WWDT_CTL_WW0_POS;
    WWDT_CTL = SFR_Config (WWDT_CTL, ~WWDT_CTL_WW, tmpreg);
}

/**
  * 描述  设置窗口看门狗预分频，对INTLF进行分频。
  * 输入  Prescaler: 窗口看门狗对INTLF的预分频值，取值为：
  *                    WWDT_PRESCALER_1: 不分频
  *                    WWDT_PRESCALER_2: 2分频
  *                    WWDT_PRESCALER_4: 4分频
  *                    WWDT_PRESCALER_8: 8分频
  *                    WWDT_PRESCALER_16: 16分频
  *                    WWDT_PRESCALER_32: 32分频
  *                    WWDT_PRESCALER_64: 64分频
  *                    WWDT_PRESCALER_128: 128分频
  *                    WWDT_PRESCALER_256: 256分频
  *                    WWDT_PRESCALER_512: 512分频
  *                    WWDT_PRESCALER_1024: 1024分频
  *                    WWDT_PRESCALER_2048: 2048分频
  *                    WWDT_PRESCALER_4096: 4096分频
  *                    WWDT_PRESCALER_8192: 8192分频
  *                    WWDT_PRESCALER_16384: 16384分频
  *                    WWDT_PRESCALER_32768: 32768分频
  *                    WWDT_PRESCALER_65536: 65536分频
  *                    WWDT_PRESCALER_131072: 131072分频
  *                    WWDT_PRESCALER_262144: 262144分频
  * 返回  无。
  */
void
WWDT_Prescaler_Config (uint32_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_WWDT_PRESCALER(Prescaler));

    /*---------------- 配置WWDT_CTL寄存器WPS位 ----------------*/
    WWDT_CTL = SFR_Config (WWDT_CTL, ~WWDT_CTL_WPS, Prescaler);
}

/**
  * 描述  设置窗口看门狗使能。
  * 输入  NewState: 窗口看门狗使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
WWDT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置WWDT_CTL寄存器WWDTEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能窗口看门狗定时器 */
        SFR_SET_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTEN_POS);
    }
    else
    {
        /* 未使能窗口看门狗 */
        SFR_CLR_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTEN_POS);
    }
}

/**
  * 描述  配置窗口看门狗计数值。
  * 输入  Counter: 窗口看门狗计数值，取值为7位有效数值。
  * 返回  无。
  */
void
WWDT_Counter_Config (uint32_t Counter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_WWDT_THRESHOLD(Counter));

    /*---------------- 配置WWDT_CNT寄存器WT位 ----------------*/
    WWDT_CNT = Counter;
}

/**
  * 描述  获取窗口看门狗计数值。
  * 输入  无。
  * 返回  窗口看门狗计数值，7位有效数值。
  */
uint32_t
WWDT_Get_Counter (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 获取WWDT_CNT寄存器WT位 ----------------*/
    tmpreg = WWDT_CNT & WWDT_CNT_WT;
    tmpreg >>= WWDT_CNT_WT_POS;

    return tmpreg;
}
/**
  *   ##### 窗口看门狗(WWDT)控制功能函数定义结束 #####
  */


/**
  *   ##### 窗口看门狗(WWDT)中断管理函数 #####
  */
/**
  * 描述  设置窗口看门狗中断使能。
  * 输入  NewState: 窗口看门狗中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
WWDT_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置WWDT_CTL寄存器WWDTIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能窗口看门狗中断 */
        SFR_SET_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIE_POS);
    }
    else
    {
        /* 禁止窗口看门狗中断 */
        SFR_CLR_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIE_POS);
    }
}

/**
  * 描述  获取窗口看门狗中断标志。
  * 输入  无。
  * 返回  1:产生了窗口看门狗中断；0:未产生窗口看门狗中断。
  */
FlagStatus
WWDT_Get_INT_Flag (void)
{
    /*---------------- 读取WWDT_CTL寄存器WWDTIF位 ----------------*/
    if (WWDT_CTL & WWDT_CTL_WWDTIF)
    {
        /* 产生了窗口看门狗中断 */
        return SET;
    }
    else
    {
        /* 未产生窗口看门狗中断 */
        return RESET;
    }
}

/**
  * 描述  清零窗口看门狗中断标志。
  * 输入  无。
  * 返回  无。
  */
void
WWDT_Clear_INT_Flag (void)
{
    /*---------------- 设置WWDT_CTL寄存器WWDTIC位 ----------------*/
    SFR_SET_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIC_POS);
    while(WWDT_CTL & WWDT_CTL_WWDTIF);
    SFR_CLR_BIT_ASM(WWDT_CTL, WWDT_CTL_WWDTIC_POS);
}
/**
  *   ##### 窗口看门狗(WWDT)中断管理函数定义结束 #####
  */
