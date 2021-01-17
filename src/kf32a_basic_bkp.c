/**
  ******************************************************************************
  * 文件名  kf32a_basic_bkp.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了备份域(BKP)外设功能函数，包含：
  *          + 备份域(BKP)复位及读写允许函数
  *          + 备份域(BKP)功能配置函数
  *          + 备份域(BKP)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_bkp.h"


/**
  *   ##### 备份域(BKP)复位及读写允许函数 #####
  */
/**
  * 描述  复位备份域(BKP)外设，该函数仅复位备份域寄存器组，而不复位RTC模块。
  * 输入  无。
  * 返回  无。
  */
void
BKP_Reset (void)
{
    /* 备份域寄存器组软件复位 */
    SFR_SET_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
    SFR_CLR_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
}

/**
  * 描述  备份域(BKP)寄存器组读写时，需要对如下寄存器位进行配置：
  *         1.  OSCCTL0寄存器的PMWREN位置1，允许整个备份域的读写操作；
  *         2.  PM_CTL0寄存器的BKPREGCLR位置1，寄存器组退出复位；
  *         3.  PM_CTL0寄存器的BKPWR位置1，允许备份域数据寄存器组和RTC模块内的寄存器读写操作。
  *       读写寄存器组前调用该函数，结束读写后必须再次调用该函数，若缺少或多次调用则会无法恢复原始配置。
  * 输入  NewState: 备份域寄存器组读写配置使能，取值范围为：
  *                   TRUE: 备份域寄存器组读写前的配置
  *                   FALSE: 备份域寄存器组读写后的配置恢复
  * 返回  无。
  */
void
BKP_Write_And_Read_Enable (FunctionalState NewState)
{
    uint32_t tmpreg = 0;
    static uint32_t pmwrenbkp = TRUE;
    static uint32_t bkpregclrbkp = TRUE;
    static uint32_t bkpwrbkp = TRUE;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 准备读写备份域数据 */
        /* 备份PMWREN位 */
        tmpreg = OSC_CTL0;
        if (OSC_CTL0_PMWREN & tmpreg)
        {
            pmwrenbkp = TRUE;
        }
        else
        {
            pmwrenbkp = FALSE;
            /* 允许整个备份域的读写操作 */
            SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
        }

        /* 备份BKPREGCLR位 */
        tmpreg = PM_CTL0;
        if (PM_CTL0_BKPREGCLR & tmpreg)
        {
            bkpregclrbkp = TRUE;
        }
        else
        {
            bkpregclrbkp = FALSE;
            /* 寄存器组退出复位 */
            SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
        }

        /* 备份BKPWR位 */
        tmpreg = PM_CTL0;
        if (PM_CTL0_BKPWR & tmpreg)
        {
            bkpwrbkp = TRUE;
        }
        else
        {
            bkpwrbkp = FALSE;
            /* 允许备份域数据寄存器组和RTC模块内的寄存器读写操作 */
            SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
        }
    }
    else
    {
        /* 结束读写备份域数据 */
        /* 恢复PMWREN位 */
        if (pmwrenbkp != FALSE)
        {
            pmwrenbkp = TRUE;
        }
        else
        {
            pmwrenbkp = FALSE;
            /* 禁止整个备份域的读写操作 */
            SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
        }

        /* 恢复BKPREGCLR位 */
        if (bkpregclrbkp != FALSE)
        {
            bkpregclrbkp = TRUE;
        }
        else
        {
            bkpregclrbkp = FALSE;
            /* 禁止整个备份域的读写操作 */
            SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
        }

        /* 恢复BKPWR位 */
        if (bkpwrbkp != FALSE)
        {
            bkpwrbkp = TRUE;
        }
        else
        {
            bkpwrbkp = FALSE;
            /* 禁止整个备份域的读写操作 */
            SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);
        }
    }
}
/**
  *   ##### 备份域(BKP)复位及读写允许函数定义结束 #####
  */


/**
  *   ##### 备份域(BKP)功能配置函数 #####
  */
/**
  * 描述  配置备份域寄存器组软件复位使能。
  * 输入  NewState: 备份域寄存器组软件复位，取值范围为：TRUE 或 FALSE。
  * 返回  无
  */
void
BKP_Reset_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置BKP_CTL寄存器BKDRST位 ----------------*/
    if (NewState != FALSE)
    {
        /* 备份域寄存器组复位 */
        SFR_SET_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
    }
    else
    {
        /* 无作用 */
        SFR_CLR_BIT_ASM(BKP_CTL, BKP_CTL_BKDRST_POS);
    }
}

/**
  * 描述  设置侵入检测引脚的有效电平。
  * 输入  PinSel: 侵入检测引脚选择，取值为：
  *                 BKP_PIN_RTC_TAMP1: 侵入检测引脚RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: 侵入检测引脚RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: 侵入检测引脚RTC_TAMP3
  *       Effective: 有效电平，取值为:
  *                    BKP_HIGH_LEVEL_EFFECTIVE: 高电平会清除所有的数据备份寄存器
  *                    BKP_LOW_LEVEL_EFFECTIVE: 低电平会清除所有的数据备份寄存器
  * 返回  无。
  */
void
BKP_Pin_Effective_Level_Config (uint32_t PinSel, uint32_t Effective)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));
    CHECK_RESTRICTION(CHECK_BKP_EFFECTIVE_LEVEL(Effective));

    /*------------- 配置BKP_CTL寄存器TAMPxLVL位 -------------*/
    tmpreg = BKP_CTL_TAMP1LVL << PinSel;
    if (BKP_HIGH_LEVEL_EFFECTIVE != Effective)
    {
        /* 侵入检测引脚上的低电平会清除所有的数据备份寄存器 */
        BKP_CTL |= tmpreg;
    }
    else
    {
        /* 侵入检测引脚上的高电平会清除所有的数据备份寄存器 */
        BKP_CTL &= ~tmpreg;
    }
}

/**
  * 描述  设置侵入检测引脚RTC_TAMP1使能。
  * 输入  PinSel: 侵入检测引脚选择，取值为：
  *                 BKP_PIN_RTC_TAMP1: 侵入检测引脚RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: 侵入检测引脚RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: 侵入检测引脚RTC_TAMP3
  *       NewState: 侵入检测引脚RTC_TAMP1使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
BKP_Pin_Enable (uint32_t PinSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------- 配置BKP_CTL寄存器TAMPxLVL位 -------------*/
    tmpreg = BKP_CTL_TAMP1EN << PinSel;
    if (NewState != FALSE)
    {
        /* 使能侵入引脚1功能 */
        BKP_CTL |= tmpreg;
    }
    else
    {
        /* 禁止侵入引脚1功能 */
        BKP_CTL &= ~tmpreg;
    }
}

/**
  * 描述  配置RTC时钟源。
  * 输入  Source: RTC时钟源选择，取值为：
                    BKP_RTC_NO_CLK: 无时钟
                    BKP_RTC_EXTLF: EXTLF作为RTC时钟
                    BKP_RTC_INTLF: INTLF作为RTC时钟
                    BKP_RTC_EXTHF_DIV_128: EXTHF经过128分频后作为RTC时钟
  * 返回  无。
  */
void
BKP_RTC_Clock_Config (uint32_t Source)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_RTC_CLK(Source));

    /*---------------- 配置BKP_CTL寄存器RTCCLKS位 ----------------*/
    BKP_CTL = SFR_Config (BKP_CTL, ~BKP_CTL_RTCCLKS, Source);
}

/**
  * 描述  设置外部时钟旁路控制使能。
  * 输入  Source: 时钟选择，取值为：
  *                 BKP_EXTHF: 外部高频时钟
  *                 BKP_EXTLF: 外部低频时钟
  *       NewState: 外部时钟旁路控制使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
BKP_External_Clock_Bypass_Enable (uint32_t Source, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_CLOCK_SOURCE(Source));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------- 配置BKP_CTL寄存器TAMPxLVL位 -------------*/
    tmpreg = BKP_CTL_EXTHFBYP << Source;
    if (FALSE != NewState)
    {
        /* 外部时钟被旁路 */
        BKP_CTL |= tmpreg;
    }
    else
    {
        /* 外部时钟未旁路 */
        BKP_CTL &= ~tmpreg;
    }
}

/**
  * 描述  写备份域备份数据寄存器。
  * 输入  OrderNumber: 备份数据寄存器编号，取值范围为0~31。
  *       WriteData: 写入的数据，取值范围为32位数据。
  * 返回  无。
  */
void
BKP_Data_Config (uint32_t OrderNumber, uint32_t WriteData)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_DATA_NUMBER(OrderNumber));

    /*------------- 配置BKP_DATA寄存器BKPDATA位 -------------*/
    BKP_SFR->DATA[OrderNumber] = WriteData;
}

/**
  * 描述  读备份域备份数据寄存器。
  * 输入  OrderNumber: 备份数据寄存器编号，取值范围为0~31。
  * 返回  备份数据寄存器的值，取值范围为32位数据。
  */
uint32_t
BKP_Get_Data (uint32_t OrderNumber)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_DATA_NUMBER(OrderNumber));

    /*------------- 读取BKP_DATA寄存器BKPDATA位 -------------*/
    tmpreg = BKP_SFR->DATA[OrderNumber];

    return tmpreg;
}
/**
  *   ##### 备份域(BKP)功能配置函数定义结束 #####
  */


/**
  *   ##### 备份域(BKP)中断管理函数 #####
  */
/**
  * 描述  配置侵入引脚中断使能。
  * 输入  PinSel: 侵入检测引脚选择，取值为：
  *                 BKP_PIN_RTC_TAMP1: 侵入检测引脚RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: 侵入检测引脚RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: 侵入检测引脚RTC_TAMP3
  *       NewState: 侵入引脚中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
BKP_Pin_TAMP_INT_Enable (uint32_t PinSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置BKP_INT寄存器TAMPxIE位 ----------------*/
    tmpreg = BKP_INT_TAMP1IE << PinSel;
    if (NewState != FALSE)
    {
        /* 使能侵入检测中断 */
        BKP_INT |= tmpreg;
    }
    else
    {
        /* 禁止侵入检测中断 */
        BKP_INT &= ~tmpreg;
    }
}

/**
  * 描述  获取侵入中断标志。
  * 输入  PinSel: 侵入检测引脚选择，取值为：
  *                 BKP_PIN_RTC_TAMP1: 侵入检测引脚RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: 侵入检测引脚RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: 侵入检测引脚RTC_TAMP3
  * 返回  1:发生侵入事件，0:无侵入事件发生。
  */
FlagStatus
BKP_Get_Pin_TAMP_INT_Flag (uint32_t PinSel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));

    /*---------------- 读取BKP_INT寄存器TAMPxIF位 ----------------*/
    tmpreg = BKP_INT_TAMP1IF << PinSel;
    if (BKP_INT & tmpreg)
    {
        /* 发生侵入事件 */
        return SET;
    }
    else
    {
        /* 无侵入事件发生 */
        return RESET;
    }
}

/**
  * 描述  清除侵入检测中断标志。
  * 输入  PinSel: 侵入检测引脚选择，取值为：
  *                 BKP_PIN_RTC_TAMP1: 侵入检测引脚RTC_TAMP1
  *                 BKP_PIN_RTC_TAMP2: 侵入检测引脚RTC_TAMP2
  *                 BKP_PIN_RTC_TAMP3: 侵入检测引脚RTC_TAMP3
  * 返回  无。
  */
void
BKP_Clear_Pin_TAMP_INT_Flag (uint32_t PinSel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BKP_PIN_RTC_TAMP(PinSel));

    /*---------------- 配置BKP_INT寄存器TAMPxIC位 ----------------*/
    tmpreg = BKP_INT_TAMP1IC << PinSel;
    tmpreg1 = BKP_INT_TAMP1IF << PinSel;
    BKP_INT |= tmpreg;
    while((BKP_INT & tmpreg1)>>(BKP_INT_TAMP1IF_POS+PinSel));
    BKP_INT &= ~tmpreg;
}
/**
  *   ##### 备份域(BKP)中断管理函数 #####
  */
