/**
  ******************************************************************************
  * 文件名  kf32a_basic_osc.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了振荡器(OSC)外设功能函数，包含：
  *          + 振荡器(OSC)外设初始化函数
  *          + 振荡器(OSC)功能配置函数
  *          + 振荡器(OSC)中断控制函数
  *
  *********************************************************************
  */

#include "kf32a_basic_osc.h"


/* 振荡器(OSC)私有定义 ----------------------------------------------------*/
/* OSC_CTL0寄存器SCK初始化配置掩码 */
#define OSC_CTL0_SCLK_MASK               (OSC_CTL0_SCKS \
                                       | OSC_CTL0_SCKDIV)

/* OSC_CTL0寄存器HFCK初始化配置掩码 */
#define OSC_CTL0_HFCK_MASK             (OSC_CTL0_HFCKS \
                                       | OSC_CTL0_HFCKDIV)

/* OSC_CTL0寄存器LFCK初始化配置掩码 */
#define OSC_CTL0_LFCK_MASK             (OSC_CTL0_LFCKS \
                                       | OSC_CTL0_LFCKDIV)

/* OSC_CTL1寄存器CK48M初始化配置掩码 */
#define OSC_CTL1_CK48M_MASK             (OSC_CTL1_CK48MS \
                                       | OSC_CTL1_CK48DIV)


/**
  *   ##### 振荡器(OSC)外设初始化函数 #####
  */
/**
  * 描述  配置振荡器(OSC)系统主时钟SCLK。
  * 输入  oscInitStruct: 振荡器(OSC)配置信息结构体指针。
  * 返回  无。
  */
void
OSC_SCLK_Configuration (OSC_InitTypeDef* oscInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpmask = 0;
    uint32_t tmpmask1 = 0;
    uint32_t sfraddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SCLK_SOURCE(oscInitStruct->m_InputSource));
    CHECK_RESTRICTION(CHECK_SCLK_DIVISION(oscInitStruct->m_CLKDivision));

    /*------------------ 设置OSC_CTL0寄存器 -----------------*/
    /* 根据参数m_InputSource，设置SCKS位域 */
    /* 根据参数m_CLKDivision，设置SCKDIV位域 */
    tmpreg = oscInitStruct->m_InputSource << OSC_CTL0_SCKS0_POS \
           | oscInitStruct->m_CLKDivision << OSC_CTL0_SCKDIV0_POS;
    tmpmask = OSC_CTL0_SCLK_MASK;

    switch (oscInitStruct->m_InputSource)
    {
        case SCLK_SOURCE_PLL:
        {
            /* 根据参数m_PLLInputSource，设置PLLxCKS位域 */
            tmpreg |= (oscInitStruct->m_PLLInputSource \
                                << OSC_CTL0_PLL0CKS_POS);
            tmpmask |= OSC_CTL0_PLL0CKS;
            /* PLL倍频DIVSEL位域寄存器地址 */
            sfraddr = (uint32_t)&PLL_CTL;
            /* PLL启动延时时间配置位域偏移 */
            tmpreg1 = OSC_CTL2_PLLDLY0_POS;
        }
            break;
        case SCLK_SOURCE_EXTHF:
        {
            tmpreg1 = OSC_CTL2_EHFDLY0_POS;
            tmpmask1 = OSC_CTL2_EHFDLY;
        }
            break;
        case SCLK_SOURCE_EXTLF:
        {
            tmpreg1 = OSC_CTL2_ELFDLY0_POS;
            tmpmask1 = OSC_CTL2_ELFDLY;
        }
            break;
        default:
            break;
    }
    /* PLL输入时钟源选择位 */
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~tmpmask, tmpreg);

    if (oscInitStruct->m_InputSource >= SCLK_SOURCE_PLL)
    {
        CHECK_RESTRICTION(CHECK_PLL_INPUT(oscInitStruct->m_PLLInputSource));
        CHECK_RESTRICTION(CHECK_PLL_START_DELAY(oscInitStruct->m_StartDelay));

        /* PLL启动延时时间配置 */
        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置PLLxDLY位域 */
        OSC_CTL2 = SFR_Config (OSC_CTL2,
                            ~(0xF << tmpreg1),
                            oscInitStruct->m_StartDelay << tmpreg1);
    }
    else if ((oscInitStruct->m_InputSource == SCLK_SOURCE_EXTHF)
         || (oscInitStruct->m_InputSource == SCLK_SOURCE_EXTLF))
    {
        /* 外部启动延时时间配置 */
        CHECK_RESTRICTION(CHECK_EXT_START_DELAY(oscInitStruct->m_StartDelay));

        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置EHFDLY/ELFDLY位域 */
        OSC_CTL2 = SFR_Config (OSC_CTL2,
                            ~tmpmask1,
                            oscInitStruct->m_StartDelay << tmpreg1);
    }
    else
    {
        ;
    }
}

/**
  * 描述  配置振荡器(OSC)高频外设时钟HFCLK。
  * 输入  oscInitStruct: 振荡器(OSC)配置信息结构体指针。
  * 返回  无。
  */
void
OSC_HFCK_Configuration (OSC_InitTypeDef* oscInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpmask = 0;
    uint32_t tmpmask1 = 0;
    uint32_t sfraddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_HFCK_SOURCE(oscInitStruct->m_InputSource));
    CHECK_RESTRICTION(CHECK_HFCK_DIVISION(oscInitStruct->m_CLKDivision));

    switch (oscInitStruct->m_InputSource)
    {
        case HFCK_SOURCE_PLL:
        {
            /* 根据参数m_PLLInputSource，设置PLLxCKS位域 */
            tmpreg = (oscInitStruct->m_PLLInputSource \
                                << OSC_CTL0_PLL0CKS_POS);
            tmpmask = OSC_CTL0_PLL0CKS;
            /* PLL倍频DIVSEL位域寄存器地址 */
            sfraddr = (uint32_t)&PLL_CTL;
            /* PLL启动延时时间配置位域偏移 */
            tmpreg1 = OSC_CTL2_PLLDLY0_POS;
        }
            break;
        case HFCK_SOURCE_INTHF:
            break;
        case HFCK_SOURCE_EXTHF:
        {
            tmpreg1 = OSC_CTL2_ELFDLY0_POS;
            tmpmask1 = OSC_CTL2_ELFDLY;
        }
            break;
        default:
            break;
    }
    /* PLL输入时钟源选择位 */
    /*------------------ 设置OSC_CTL0寄存器 -----------------*/
    /* 根据参数m_InputSource，设置HFCKS位域 */
    /* 根据参数m_CLKDivision，设置HFCKDIV位域 */
    tmpreg |= oscInitStruct->m_InputSource << OSC_CTL0_HFCKS0_POS \
           | oscInitStruct->m_CLKDivision << OSC_CTL0_HFCKDIV0_POS;
    tmpmask |= OSC_CTL0_HFCK_MASK;

    OSC_CTL0 = SFR_Config (OSC_CTL0, ~tmpmask, tmpreg);

    if (oscInitStruct->m_InputSource >= HFCK_SOURCE_PLL)
    {
        CHECK_RESTRICTION(CHECK_PLL_INPUT(oscInitStruct->m_PLLInputSource));
        CHECK_RESTRICTION(CHECK_PLL_START_DELAY(oscInitStruct->m_StartDelay));
        /* PLL启动延时时间配置 */
        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置PLLxDLY位域 */
        OSC_CTL2 = SFR_Config (OSC_CTL2,
                            ~(0xF << tmpreg1),
                            oscInitStruct->m_StartDelay << tmpreg1);
    }
    else if (oscInitStruct->m_InputSource == HFCK_SOURCE_EXTHF)
    {
        /* 外部启动延时时间配置 */
        CHECK_RESTRICTION(CHECK_EXT_START_DELAY(oscInitStruct->m_StartDelay));

        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置EHFDLY/ELFDLY位域 */
        OSC_CTL2 = SFR_Config (OSC_CTL2,
                            ~tmpmask1,
                            oscInitStruct->m_StartDelay << tmpreg1);
    }
    else
    {
        ;
    }
}

/**
  * 描述  配置振荡器(OSC)低频外设时钟LFCLK。
  * 输入  oscInitStruct: 振荡器(OSC)配置信息结构体指针。
  * 返回  无。
  */
void
OSC_LFCK_Configuration (OSC_InitTypeDef* oscInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LFCK_INPUT(oscInitStruct->m_InputSource));
    CHECK_RESTRICTION(CHECK_LFCK_DIVISION(oscInitStruct->m_CLKDivision));

    /*------------------ 设置OSC_CTL0寄存器 -----------------*/
    /* 根据参数m_InputSource，设置LFCKS位域 */
    /* 根据参数m_CLKDivision，设置LFCKDIV位域 */
    tmpreg = oscInitStruct->m_InputSource << OSC_CTL0_LFCKS_POS \
           | oscInitStruct->m_CLKDivision << OSC_CTL0_LFCKDIV0_POS;
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~OSC_CTL0_LFCK_MASK, tmpreg);

    if (oscInitStruct->m_InputSource == LFCK_INPUT_EXTLF)
    {
        /* 外部启动延时时间配置 */
        CHECK_RESTRICTION(CHECK_EXT_START_DELAY(oscInitStruct->m_StartDelay));

        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置ELFDLY位域 */
        tmpreg = oscInitStruct->m_StartDelay << OSC_CTL2_ELFDLY0_POS;
        OSC_CTL2 = SFR_Config (OSC_CTL2, ~OSC_CTL2_ELFDLY, tmpreg);
    }
    else
    {
        ;
    }
}

/**
  * 描述  配置振荡器(OSC)48MHz时钟CK48M。
  * 输入  oscInitStruct: 振荡器(OSC)配置信息结构体指针。
  * 返回  无。
  */
void
OSC_CK48M_Configuration (OSC_InitTypeDef* oscInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpmask = 0;
    uint32_t tmpmask1 = 0;
    uint32_t sfraddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CK48M_SOURCE(oscInitStruct->m_InputSource));
    CHECK_RESTRICTION(CHECK_CK48M_DIVISION(oscInitStruct->m_CLKDivision));

    switch (oscInitStruct->m_InputSource)
    {
        case CK48M_SOURCE_PLL:
        {
            /* 根据参数m_PLLInputSource，设置PLLxCKS位域 */
            tmpreg = (oscInitStruct->m_PLLInputSource \
                                << OSC_CTL0_PLL0CKS_POS);
            tmpmask = OSC_CTL0_PLL0CKS;
            /* PLL倍频DIVSEL位域寄存器地址 */
            sfraddr = (uint32_t)&PLL_CTL;
            /* PLL启动延时时间配置位域偏移 */
            tmpreg1 = OSC_CTL2_PLLDLY0_POS;
        }
            break;
        case CK48M_SOURCE_INTHF:
            break;
        case CK48M_SOURCE_EXTHF:
        {
            tmpreg1 = OSC_CTL2_ELFDLY0_POS;
            tmpmask1 = OSC_CTL2_ELFDLY;
        }
            break;
        default:
            break;
    }

    if (oscInitStruct->m_InputSource >= CK48M_SOURCE_PLL)
    {
        CHECK_RESTRICTION(CHECK_PLL_INPUT(oscInitStruct->m_PLLInputSource));
        CHECK_RESTRICTION(CHECK_PLL_START_DELAY(oscInitStruct->m_StartDelay));

        /* PLL输入时钟源选择位 */
        /*------------------ 设置OSC_CTL0寄存器 -----------------*/
        /* 根据参数m_PLLInputSource，设置PLLxCKS位域 */
        OSC_CTL0 = SFR_Config (OSC_CTL0, ~tmpmask, tmpreg);

        /* PLL启动延时时间配置 */
        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置PLLxDLY位域 */
        OSC_CTL2 = SFR_Config (OSC_CTL2,
                            ~(0xF << tmpreg1),
                            oscInitStruct->m_StartDelay << tmpreg1);
    }
    else if (oscInitStruct->m_InputSource == HFCK_SOURCE_EXTHF)
    {
        /* 外部启动延时时间配置 */
        CHECK_RESTRICTION(CHECK_EXT_START_DELAY(oscInitStruct->m_StartDelay));

        /*------------------ 设置OSC_CTL2寄存器 -----------------*/
        /* 根据参数m_StartDelay，设置EHFDLY/ELFDLY位域 */
        OSC_CTL2 = SFR_Config (OSC_CTL2,
                            ~tmpmask1,
                            oscInitStruct->m_StartDelay << tmpreg1);
    }
    else
    {
        ;
    }

    /* 振荡源选择和分频系数 */
    /*------------------ 设置OSC_CTL1寄存器 -----------------*/
    /* 根据参数m_InputSource，设置CK48MS位域 */
    /* 根据参数m_CLKDivision，设置CK48DIV位域 */
    tmpreg = oscInitStruct->m_InputSource << OSC_CTL1_CK48MS0_POS \
           | oscInitStruct->m_CLKDivision << OSC_CTL1_CK48DIV0_POS;
    OSC_CTL1 = SFR_Config (OSC_CTL1, ~OSC_CTL1_CK48M_MASK, tmpreg);
}

/**
  * 描述  初始化振荡器(OSC)配置信息结构体。
  * 输入  oscInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
OSC_Struct_Init (OSC_InitTypeDef* oscInitStruct)
{
    /* 振荡源选择 */
    oscInitStruct->m_InputSource = SCLK_SOURCE_INTHF;
    /* 时钟分频系数 */
    oscInitStruct->m_CLKDivision = SCLK_DIVISION_128;
    /* PLL输入时钟源选择 */
    oscInitStruct->m_PLLInputSource = PLL_INPUT_INTHF;
    /* PLL 或 外部启动延时时间配置 */
    oscInitStruct->m_StartDelay = PLL_START_DELAY_64;
}
/**
  *   ##### 振荡器(OSC)外设初始化函数定义结束 #####
  */


/**
  *   ##### 振荡器(OSC)功能配置函数 #####
  */
/**
  * 描述  配置低频外设时钟分频选择。
  * 输入  LFDivision: 低频外设时钟分频选择，取值范围为：
  *                         LFCK_DIVISION_1: 1/1分频,
  *                         LFCK_DIVISION_2: 1/2分频,
  *                         LFCK_DIVISION_4: 1/4分频,
  *                         LFCK_DIVISION_8: 1/8分频,
  *                         LFCK_DIVISION_16: 1/16分频,
  *                         LFCK_DIVISION_32: 1/32分频,
  *                         LFCK_DIVISION_64: 1/64分频,
  *                         LFCK_DIVISION_128: 1/128分频,
  * 返回  无。
  */
void
OSC_LFCK_Division_Config (uint32_t LFDivision)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LFCK_DIVISION(LFDivision));

    /*---------------- 设置OSC_CTL0寄存器LFCKDIV位 ----------------*/
    tmpreg = LFDivision << OSC_CTL0_LFCKDIV0_POS;
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~OSC_CTL0_LFCKDIV, tmpreg);
}

/**
  * 描述  配置高频外设时钟分频选择。
  * 输入  HFDivision: 高频外设时钟分频选择，取值范围为：
  *                         HFCK_DIVISION_1: 1/1分频,
  *                         HFCK_DIVISION_2: 1/2分频,
  *                         HFCK_DIVISION_4: 1/4分频,
  *                         HFCK_DIVISION_8: 1/8分频,
  *                         HFCK_DIVISION_16: 1/16分频,
  *                         HFCK_DIVISION_32: 1/32分频,
  *                         HFCK_DIVISION_64: 1/64分频,
  *                         HFCK_DIVISION_128: 1/128分频,
  *                         HFCK_DIVISION_256: 1/256分频,
  *                         HFCK_DIVISION_512: 1/512分频,
  * 返回  无。
  */
void
OSC_HFCK_Division_Config (uint32_t HFDivision)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_HFCK_DIVISION(HFDivision));

    /*---------------- 设置OSC_CTL0寄存器HFCKDIV位 ----------------*/
    tmpreg = HFDivision << OSC_CTL0_HFCKDIV0_POS;
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~OSC_CTL0_HFCKDIV, tmpreg);
}

/**
  * 描述  配置主时钟SCLK分频选择。
  * 输入  SclkDivision: 主时钟SCLK分频选择，取值范围为：
  *                         SCLK_DIVISION_1: 1/1分频,
  *                         SCLK_DIVISION_2: 1/2分频,
  *                         SCLK_DIVISION_4: 1/4分频,
  *                         SCLK_DIVISION_8: 1/8分频,
  *                         SCLK_DIVISION_16: 1/16分频,
  *                         SCLK_DIVISION_32: 1/32分频,
  *                         SCLK_DIVISION_64: 1/64分频,
  *                         SCLK_DIVISION_128: 1/128分频,
  * 返回  无。
  */
void
OSC_SCK_Division_Config (uint32_t SclkDivision)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SCLK_DIVISION(SclkDivision));

    /*---------------- 设置OSC_CTL0寄存器SCKDIV位 ----------------*/
    tmpreg = SclkDivision << OSC_CTL0_SCKDIV0_POS;
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~OSC_CTL0_SCKDIV, tmpreg);
}


/**
  * 描述  配置PLL输入时钟源选择。
  * 输入  NewState: PLL输入时钟源选择，取值范围为：
  *                   PLL_INPUT_INTHF: 选择INTHF作为PLL输入时钟，
  *                   PLL_INPUT_EXTHF: 选择EXTHF作为PLL输入时钟。
  * 返回  无。
  */
void
OSC_PLL_Input_Source_Config (uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PLL_INPUT(NewState));

    /*---------------- 设置OSC_CTL0寄存器PLL0CKS位 ----------------*/
    if (NewState != PLL_INPUT_INTHF)
    {
        /* 选择EXTHF作为PLL输入时钟 */
        SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PLL0CKS_POS);
    }
    else
    {
        /* 选择INTHF作为PLL输入时钟 */
        SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_PLL0CKS_POS);
    }
}

/**
  * 描述  配置高频外设时钟源选择。
  * 输入  HFSource: 高频外设时钟源选择，取值范围为：
  *                   HFCK_SOURCE_INTHF: 选择INTHF作为HFCLK时钟,
  *                   HFCK_SOURCE_EXTHF: 选择EXTHF作为HFCLK时钟,
  *                   HFCK_SOURCE_PLL: 选择PLL输出作为HFCLK时钟,
  *                   HFCK_SOURCE_PLL2: 选择PLL2输出作为HFCLK时钟,
  *                   HFCK_SOURCE_PLL1: 选择PLL1输出作为HFCLK时钟,
  * 返回  无。
  */
void
OSC_HFCK_Source_Config (uint32_t HFSource)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_HFCK_SOURCE(HFSource));

    /*---------------- 设置OSC_CTL0寄存器HFCKS位 ----------------*/
    tmpreg = HFSource << OSC_CTL0_HFCKS0_POS;
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~OSC_CTL0_HFCKS, tmpreg);
}

/**
  * 描述  配置高频外设时钟允许使能。
  * 输入  NewState: 高频外设时钟允许使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_HFCK_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL0寄存器HFCKEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* HFCLK时钟信号允许 */
        SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_HFCKEN_POS);
    }
    else
    {
        /* HFCLK时钟信号禁止 */
        SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_HFCKEN_POS);
    }
}

/**
  * 描述  配置低频外设时钟源选择。
  * 输入  NewState: 低频外设时钟源选择，取值范围为：
  *                   LFCK_INPUT_INTLF: 选择INTLF作为LFCLK时钟，
  *                   LFCK_INPUT_EXTLF: 选择EXTLF作为LFCLK时钟。
  * 返回  无。
  */
void
OSC_LFCK_Source_Config (uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LFCK_INPUT(NewState));

    /*---------------- 设置OSC_CTL0寄存器LFCKS位 ----------------*/
    if (NewState != LFCK_INPUT_INTLF)
    {
        /* 选择EXTLF作为LFCLK时钟 */
        SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_LFCKS_POS);
    }
    else
    {
        /* 选择INTLF作为LFCLK时钟 */
        SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_LFCKS_POS);
    }
}

/**
  * 描述  配置低频外设时钟允许使能。
  * 输入  NewState: 低频外设时钟允许使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_LFCK_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL0寄存器LFCKEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* LFCLK时钟信号允许 */
        SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_LFCKEN_POS);
    }
    else
    {
        /* LFCLK时钟信号禁止 */
        SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_LFCKEN_POS);
    }
}

/**
  * 描述  配置SCLK主时钟选择。
  * 输入  SclkSource: SCLK主时钟选择，取值范围为：
  *                     SCLK_SOURCE_INTHF: 选择INTHF作为系统时钟,
  *                     SCLK_SOURCE_INTLF: 选择INTLF作为系统时钟,
  *                     SCLK_SOURCE_EXTHF: 选择EXTHF作为系统时钟,
  *                     SCLK_SOURCE_EXTLF: 选择EXTLF作为系统时钟,
  *                     SCLK_SOURCE_PLL: 选择PLL输出作为系统时钟,
  *                     SCLK_SOURCE_LP4M:  选择 LP4M 输出作为系统时钟
  * 返回  无。
  */
void
OSC_SCK_Source_Config (uint32_t SclkSource)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SCLK_SOURCE(SclkSource));

    /*---------------- 设置OSC_CTL0寄存器SCKS位 ----------------*/
    tmpreg = SclkSource << OSC_CTL0_SCKS0_POS;
    OSC_CTL0 = SFR_Config (OSC_CTL0, ~OSC_CTL0_SCKS, tmpreg);
}

/**
  * 描述  配置备份区寄存器读写允许。
  * 输入  NewState: 备份区寄存器读写允许状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_Backup_Write_Read_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL0寄存器PMWREN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 备份区可读写 */
        SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
    }
    else
    {
        /* 备份区不可读写 */
        SFR_CLR_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
    }
}

/**
  * 描述  配置系统时钟输出使能。
  * 输入  NewState: 系统时钟输出使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_SCLK_Output_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL1寄存器SCLKOE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 允许SCLK时钟从IO口输出（CLKOUT） */
        SFR_SET_BIT_ASM(OSC_CTL1, OSC_CTL1_SCLKOE_POS);
    }
    else
    {
        /* 禁止SCLK时钟从IO口输出（CLKOUT） */
        SFR_CLR_BIT_ASM(OSC_CTL1, OSC_CTL1_SCLKOE_POS);
    }
}
/**
  * 描述  配置从引脚 CLKOUT 输出时钟选择。
  * 输入  SclkSource: 输出时钟选择，
  *                     CLKOUT_SCLK:  选择 SCLK 作为输出时钟
  *                     CLKOUT_EXTLF: 选择 EXTLF 作为输出时钟
  *                     CLKOUT_EXTHF: 选择 EXTHF 作为输出时钟
  *                     CLKOUT_INTLF: 选择 INTLF 作为输出时钟
  *                     CLKOUT_INTHF: 选择 INTHF 作为输出时钟
  *                     CLKOUT_PLL:   选择 PLL 作为输出时钟
  *                     CLKOUT_LP4M:  选择 LP4M 作为输出时钟
  * 返回  无。
  */
void
OSC_SCLK_Output_Select (uint32_t SclkSource)
{
    uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CLKOUT_SEL(SclkSource));

    /*---------------- 设置OSC_CTL0寄存器CLKOUTS位 ----------------*/
    tmpreg = SclkSource << OSC_CTL1_CLKOUTS0_POS;
    OSC_CTL1 = SFR_Config (OSC_CTL1, ~OSC_CTL1_CLKOUTS, tmpreg);
}
/**
  * 描述  配置SCLK输出时钟分频选择。
  * 输入  OutputDivision: SCLK输出时钟分频选择位，取值范围为：
  *                         SCLK_DIVISION_1: 1/1分频,
  *                         SCLK_DIVISION_2: 1/2分频,
  *                         SCLK_DIVISION_4: 1/4分频,
  *                         SCLK_DIVISION_8: 1/8分频,
  *                         SCLK_DIVISION_16: 1/16分频,
  *                         SCLK_DIVISION_32: 1/32分频,
  *                         SCLK_DIVISION_64: 1/64分频,
  *                         SCLK_DIVISION_128: 1/128分频,
  * 返回  无。
  */
void
OSC_SCLK_Output_Division_Config (uint32_t OutputDivision)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SCLK_DIVISION(OutputDivision));

    /*---------------- 设置OSC_CTL1寄存器SCLKOUTDIV位 ----------------*/
    tmpreg = OutputDivision << OSC_CTL1_SCLKOUTDIV0_POS;
    OSC_CTL1 = SFR_Config (OSC_CTL1, ~OSC_CTL1_SCLKOUTDIV, tmpreg);
}

/**
  * 描述  配置时钟故障检测使能。
  * 输入  NewState: 时钟故障检测使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_Clock_Failure_Check_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL1寄存器FSCM位 ----------------*/
    if (NewState != FALSE)
    {
        /* 时钟故障检测使能 */
        SFR_SET_BIT_ASM(OSC_CTL1, OSC_CTL1_FSCM_POS);
    }
    else
    {
        /* 时钟故障检测禁止 */
        SFR_CLR_BIT_ASM(OSC_CTL1, OSC_CTL1_FSCM_POS);
    }
}

/**
  * 描述  配置CK48M时钟分频选择。
  * 输入  CK48MDivision: CK48M时钟分频选择，取值范围为：
  *                        CK48M_DIVISION_1: 1/1分频,
  *                        CK48M_DIVISION_2: 1/2分频,
  *                        CK48M_DIVISION_4: 1/4分频,
  * 返回  无。
  */
void
OSC_CK48M_Division_Config (uint32_t CK48MDivision)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CK48M_DIVISION(CK48MDivision));

    /*---------------- 设置OSC_CTL1寄存器CK48DIV位 ----------------*/
    tmpreg = CK48MDivision << OSC_CTL1_CK48DIV0_POS;
    OSC_CTL1 = SFR_Config (OSC_CTL1, ~OSC_CTL1_CK48DIV, tmpreg);
}

/**
  * 描述  配置48M时钟输入时钟源选择。
  * 输入  CK48MSource: 48M时钟输入时钟源选择，取值范围为：
  *                      CK48M_SOURCE_INTHF: 选择INTHF作为CK48M输入时钟,
  *                      CK48M_SOURCE_EXTHF: 选择EXTHF作为CK48M输入时钟,
  *                      CK48M_SOURCE_PLL: 选择PLL输出作为CK48M输入时钟,
  * 返回  无。
  */
void
OSC_CK48M_Source_Config (uint32_t CK48MSource)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CK48M_SOURCE(CK48MSource));

    /*---------------- 设置OSC_CTL1寄存器CK48MS位 ----------------*/
    tmpreg = CK48MSource << OSC_CTL1_CK48MS0_POS;
    OSC_CTL1 = SFR_Config (OSC_CTL1, ~OSC_CTL1_CK48MS, tmpreg);
}

/**
  * 描述  配置48M时钟输入时钟允许使能。
  * 输入  NewState: 48M时钟输入时钟允许使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_CK48M_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL1寄存器CK48MEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* CK48M时钟信号使能 */
        SFR_SET_BIT_ASM(OSC_CTL1, OSC_CTL1_CK48MEN_POS);
    }
    else
    {
        /* CK48M时钟信号禁止 */
        SFR_CLR_BIT_ASM(OSC_CTL1, OSC_CTL1_CK48MEN_POS);
    }
}
/**
  * 描述  配置PLL倍频选择。
  * 输入  PLLmultiple: PLL倍频，取值范围为：
  *                PLLmultiple_M 0X04-0X3FFF范围(M要大于等于4, 并且满足200MHZ<（M*输入参考频率/N）<400MHZ)
  *                PLLmultiple_N 0X01-0XF范围,并且满足1MHZ<（输入参考频率/N）<50MHZ
  *                PLLmultiple_NO 取值，1/2/4/8选其中1个。
  *
  *					放大倍数=PLLmultiple_M/（PLLmultiple_N*PLLmultiple_NO）
  * 返回  无。
  */
void
OSC_PLL_Multiple_Value_Select (uint32_t PLLmultiple_M,uint32_t PLLmultiple_N,uint32_t PLLmultiple_NO)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PLL_MULTIPLE_MValue(PLLmultiple_M));
    CHECK_RESTRICTION(CHECK_PLL_MULTIPLE_NValue(PLLmultiple_N));
    CHECK_RESTRICTION(CHECK_PLL_MULTIPLE_NOValue(PLLmultiple_NO));



    /*---------------- 设置PLL_CTL寄存器OD位N位M位 ----------------*/
    tmpreg = PLLmultiple_M << PLL_CTL_M0_POS;
    PLL_CTL = SFR_Config (PLL_CTL, ~PLL_CTL_M, tmpreg);

    tmpreg = PLLmultiple_N << PLL_CTL_N0_POS;
    PLL_CTL = SFR_Config (PLL_CTL, ~PLL_CTL_N, tmpreg);

    if(PLLmultiple_NO==8){PLLmultiple_NO=3;}
    else if(PLLmultiple_NO==4){PLLmultiple_NO=2;}
    else if(PLLmultiple_NO==2){PLLmultiple_NO=1;}
    else if(PLLmultiple_NO==1){PLLmultiple_NO=0;}
    tmpreg = PLLmultiple_NO << PLL_CTL_OD0_POS;
    PLL_CTL = SFR_Config (PLL_CTL, ~PLL_CTL_OD, tmpreg);


}
/**
  * 描述  PLL复位控制。
  * 输入 :无
  * 返回 ：无
  */
void OSC_PLL_RST(void)
{
    /*---------------- 设置PLL_CTL寄存器PDRST位 ----------------*/
    SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_PDRST_POS);
}
/**
  * 描述  配置PLL启动延迟控制。
  * 输入  PLLDelay: PLL启动延迟控制，取值范围为：
  *                   PLL_START_DELAY_64: 延迟64个时钟
  *                   PLL_START_DELAY_128: 延迟128个时钟
  *                   PLL_START_DELAY_256: 延迟256个时钟
  *                   PLL_START_DELAY_512: 延迟512个时钟
  *                   PLL_START_DELAY_1024: 延迟1024个时钟
  *                   PLL_START_DELAY_2048: 延迟2048个时钟
  *                   PLL_START_DELAY_4096: 延迟4096个时钟
  *                   PLL_START_DELAY_8192: 延迟8192个时钟
  *                   PLL_START_DELAY_16384: 延迟16384个时钟
  *                   PLL_START_DELAY_32768: 延迟32768个时钟
  * 返回  无。
  */
void
OSC_PLL_Start_Delay_Config (uint32_t PLLDelay)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PLL_START_DELAY(PLLDelay));

    /*---------------- 设置OSC_CTL2寄存器PLLDLY位 ----------------*/
    tmpreg = PLLDelay << OSC_CTL2_PLLDLY0_POS;
    OSC_CTL2 = SFR_Config (OSC_CTL2, ~OSC_CTL2_PLLDLY, tmpreg);
}


/**
  * 描述  配置外部高频启动延迟控制。
  * 输入  ExternalDelay: 外部高频启动延迟控制，取值范围为：
  *                        EXT_START_DELAY_0: 延迟0个时钟
  *                        EXT_START_DELAY_2: 延迟2个时钟
  *                        EXT_START_DELAY_4: 延迟4个时钟
  *                        EXT_START_DELAY_8: 延迟8个时钟
  *                        EXT_START_DELAY_16: 延迟16个时钟
  *                        EXT_START_DELAY_32: 延迟32个时钟
  *                        EXT_START_DELAY_64: 延迟64个时钟
  *                        EXT_START_DELAY_128: 延迟128个时钟
  *                        EXT_START_DELAY_256: 延迟256个时钟
  *                        EXT_START_DELAY_512: 延迟512个时钟
  * 返回  无。
  */
void
OSC_EXTHF_Start_Delay_Config (uint32_t ExternalDelay)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_EXT_START_DELAY(ExternalDelay));

    /*---------------- 设置OSC_CTL2寄存器EHFDLY位 ----------------*/
    tmpreg = ExternalDelay << OSC_CTL2_EHFDLY0_POS;
    OSC_CTL2 = SFR_Config (OSC_CTL2, ~OSC_CTL2_EHFDLY, tmpreg);
}

/**
  * 描述  配置外部低频启动延迟控制。
  * 输入  ExternalDelay: 外部低频启动延迟控制，取值范围为：
  *                        EXT_START_DELAY_0: 延迟0个时钟
  *                        EXT_START_DELAY_2: 延迟2个时钟
  *                        EXT_START_DELAY_4: 延迟4个时钟
  *                        EXT_START_DELAY_8: 延迟8个时钟
  *                        EXT_START_DELAY_16: 延迟16个时钟
  *                        EXT_START_DELAY_32: 延迟32个时钟
  *                        EXT_START_DELAY_64: 延迟64个时钟
  *                        EXT_START_DELAY_128: 延迟128个时钟
  *                        EXT_START_DELAY_256: 延迟256个时钟
  *                        EXT_START_DELAY_512: 延迟512个时钟
  * 返回  无。
  */
void
OSC_EXTLF_Start_Delay_Config (uint32_t ExternalDelay)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_EXT_START_DELAY(ExternalDelay));

    /*---------------- 设置OSC_CTL2寄存器ELFDLY位 ----------------*/
    tmpreg = ExternalDelay << OSC_CTL2_ELFDLY0_POS;
    OSC_CTL2 = SFR_Config (OSC_CTL2, ~OSC_CTL2_ELFDLY, tmpreg);
}

/**
  * 描述  配置PLL软件使能。
  * 输入  NewState: PLL软件使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Software_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL2寄存器PLLSWEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能PLL工作 */
        SFR_SET_BIT_ASM(OSC_CTL2, OSC_CTL2_PLLSWEN_POS);
    }
    else
    {
        /* 无效 */
        SFR_CLR_BIT_ASM(OSC_CTL2, OSC_CTL2_PLLSWEN_POS);
    }
}

/**
  * 描述  配置外部高频振荡器软件使能。
  * 输入  NewState: 外部高频振荡器软件使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_EXTHF_Software_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL2寄存器EXTHFSWEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能外部高频振荡器工作 */
        SFR_SET_BIT_ASM(OSC_CTL2, OSC_CTL2_EXTHFSWEN_POS);
    }
    else
    {
        /* 无效 */
        SFR_CLR_BIT_ASM(OSC_CTL2, OSC_CTL2_EXTHFSWEN_POS);
    }
}

/**
  * 描述  配置外部低频振荡器软件使能。
  * 输入  NewState: 外部低频振荡器软件使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_EXTLF_Software_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL2寄存器EXTLFSWEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能外部低频振荡器工作 */
        SFR_SET_BIT_ASM(OSC_CTL2, OSC_CTL2_EXTLFSWEN_POS);
    }
    else
    {
        /* 无效 */
        SFR_CLR_BIT_ASM(OSC_CTL2, OSC_CTL2_EXTLFSWEN_POS);
    }
}

/**
  * 描述  配置内部高频振荡器软件使能。
  * 输入  NewState: 内部高频振荡器软件使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_INTHF_Software_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL2寄存器INTHFSWEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能内部高频振荡器工作 */
        SFR_SET_BIT_ASM(OSC_CTL2, OSC_CTL2_INTHFSWEN_POS);
    }
    else
    {
        /* 无效 */
        SFR_CLR_BIT_ASM(OSC_CTL2, OSC_CTL2_INTHFSWEN_POS);
    }
}

/**
  * 描述  配置内部低频振荡器软件使能。
  * 输入  NewState: 内部低频振荡器软件使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_INTLF_Software_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_CTL2寄存器INTLFSWEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能内部低频振荡器工作 */
        SFR_SET_BIT_ASM(OSC_CTL2, OSC_CTL2_INTLFSWEN_POS);
    }
    else
    {
        /* 无效 */
        SFR_CLR_BIT_ASM(OSC_CTL2, OSC_CTL2_INTLFSWEN_POS);
    }
}

/**
  * 描述  配置零温漂的绝对值校准权重位。
  * 输入  Scale: 零温漂电流档选择，取值为下列宏的一个或多个组合：
  *                ZERO_DRIFT_SCALEDIV32: 1/32uA电流档
  *                ZERO_DRIFT_SCALEDIV16: 1/16uA电流档
  *                ZERO_DRIFT_SCALEDIV8: 1/8uA电流档
  *                ZERO_DRIFT_SCALEDIV4: 1/4uA电流档
  *                ZERO_DRIFT_SCALEDIV2: 1/2uA电流档
  *                ZERO_DRIFT_SCALE1X: 1uA电流档
  *                ZERO_DRIFT_SCALE2X: 2uA电流档
  *                ZERO_DRIFT_SCALE4X: 4uA电流档
  *                ZERO_DRIFT_SCALE8X: 8uA电流档
  *                ZERO_DRIFT_SCALE16X0: 16uA电流档
  *                ZERO_DRIFT_SCALE16X1: 16uA电流档
  *                ZERO_DRIFT_SCALE16X2: 16uA电流档
  *                ZERO_DRIFT_SCALE16X3: 16uA电流档
  *                ZERO_DRIFT_SCALE16X4: 16uA电流档
  *                ZERO_DRIFT_SCALE32X: 32uA电流档
  *       NewState: 零温漂电流档状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_Zero_Drift_Config (uint32_t Scale, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ZERO_DRIFT(Scale));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_HFOSCCAL0寄存器 ----------------*/
    if (NewState != FALSE)
    {
        /* 指定电流档开启 */
        OSC_HFOSCCAL0 |= Scale;
    }
    else
    {
        /* 指定电流档关闭 */
        OSC_HFOSCCAL0 &= ~Scale;
    }
}

/**
  * 描述  配置内部高频振荡器的正温调节。
  * 输入  PositiveDrift: 内部高频振荡器的正温调节，取值范围为：0~31。
  * 返回  无。
  */
void
OSC_Positive_Drift_Config (uint32_t PositiveDrift)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_POSITIVE_DRIFT(PositiveDrift));

    /*---------------- 设置OSC_HFOSCCAL0寄存器PTAT位 ----------------*/
    tmpreg = PositiveDrift << OSC_HFOSCCAL0_PTAT0_POS;
    OSC_HFOSCCAL0 = SFR_Config (OSC_HFOSCCAL0, ~OSC_HFOSCCAL0_PTAT, tmpreg);
}

/**
  * 描述  配置内部高频振荡器的负温调节。
  * 输入  NegativeDrift: 内部高频振荡器的负温调节，取值范围为：0~31。
  * 返回  无。
  */
void
OSC_Negative_Drift_Config (uint32_t PositiveDrift)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_NEGATIVE_DRIFT(PositiveDrift));

    /*---------------- 设置OSC_HFOSCCAL0寄存器CTAT位 ----------------*/
    tmpreg = PositiveDrift << OSC_HFOSCCAL0_CTAT0_POS;
    OSC_HFOSCCAL0 = SFR_Config (OSC_HFOSCCAL0, ~OSC_HFOSCCAL0_CTAT, tmpreg);
}

/**
  * 描述  配置电流增益选择。
  * 输入  CurrentGain: 电流增益选择，取值范围为：0~3。
  * 返回  无。
  */
void
OSC_Current_Gain_Config (uint32_t PositiveDrift)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CURRENT_GAIN(PositiveDrift));

    /*---------------- 设置OSC_HFOSCCAL1寄存器SELGAIN位 ----------------*/
    tmpreg = PositiveDrift << OSC_HFOSCCAL1_SELGAIN0_POS;
    OSC_HFOSCCAL1 = SFR_Config (OSC_HFOSCCAL1, ~OSC_HFOSCCAL1_SELGAIN, tmpreg);
}

/**
  * 描述  配置高速选择。
  * 输入  NewState: 高速选择状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_High_Speed_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_HFOSCCAL1寄存器HSFSEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HSFSEN_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HSFSEN_POS);
    }
}

/**
  * 描述  配置外部高频晶振输入使能。
  * 输入  NewState: 外部高频晶振输入使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_External_Input_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_HFOSCCAL1寄存器HSXTALEXEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HSXTALEXEN_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HSXTALEXEN_POS);
    }
}

/**
  * 描述  配置反馈电阻调节。
  * 输入  NewState: 反馈电阻调节，取值范围为：
  *                   FREQUENCY_MORE_THAN_10M: 频率大于10MHz，
  *                   FREQUENCY_LESS_THAN_10M: 频率小于10MHz。
  * 返回  无。
  */
void
OSC_Feedback_Resistance_Config (uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FEEDBACK_RESISTANCE(NewState));

    /*---------------- 设置OSC_HFOSCCAL1寄存器HLSW位 ----------------*/
    if (NewState != FREQUENCY_MORE_THAN_10M)
    {
        /* 频率小于10MHz */
        SFR_SET_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HLSW_POS);
    }
    else
    {
        /* 频率大于10MHz */
        SFR_CLR_BIT_ASM(OSC_HFOSCCAL1, OSC_HFOSCCAL1_HLSW_POS);
    }
}
/**
  * 描述  配置PLL零温漂电流源输出到IO口使能。
  * 输入  NewState: PLL零温漂电流源输出到IO口使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Zero_Source_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSZEROIS位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开零温漂电流源到IO的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSZEROIS_POS);
    }
    else
    {
        /* 关闭零温漂电流源到IO的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSZEROIS_POS);
    }
}

/**
  * 描述  配置PLL内部参考电压2V输出到IO口的使能。
  * 输入  NewState: PLL内部参考电压2V输出到IO口的使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Vref2_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSVREF2V位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开内部参考电压2V到IO的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSVREF2V_POS);
    }
    else
    {
        /* 关闭内部参考电压2V到IO的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSVREF2V_POS);
    }
}

/**
  * 描述  配置PLL内部低功耗BG参考0.5V到内部buffer的使能。
  * 输入  NewState: PLL内部低功耗BG参考0.5V到内部buffer的使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Vref0p5_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSVREF0P5位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开内部低功耗BG参考0.5V到内部buffer的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSVREF0P5_POS);
    }
    else
    {
        /* 关闭内部低功耗BG参考0.5V到内部buffer的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSVREF0P5_POS);
    }
}

/**
  * 描述  配置PLL主BG到内部buffer的通道使能。
  * 输入  NewState: PLL主BG到内部buffer的通道使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Vref1p2_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSMRBG位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开主BG参考1.2V到内部buffer的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSMRBG_POS);
    }
    else
    {
        /* 关闭BG参考1.2V到内部buffer的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSMRBG_POS);
    }
}

/**
  * 描述  配置PLL内部低功耗BG的电流源20nA到IO口的选通使能。
  * 输入  NewState: PLL内部低功耗BG的电流源20nA到IO口的选通使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Low_Power_20nA_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSLSRCIS位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开内部低功耗BG的电流源20nA到IO口的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSLSRCIS_POS);
    }
    else
    {
        /* 关闭内部低功耗BG的电流源20nA到IO口的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSLSRCIS_POS);
    }
}

/**
  * 描述  配置PLL内部低功耗参考电压1.14V到内部buffer的选通使能。
  * 输入  NewState: PLL内部低功耗参考电压1.14V到内部buffer的选通使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Vref1p14_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSLPBG位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开内部低功耗参考电压1.14V到内部buffer的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSLPBG_POS);
    }
    else
    {
        /* 关闭内部低功耗参考电压1.14V到内部buffer的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSLPBG_POS);
    }
}

/**
  * 描述  配置PLL内部低功耗偏置电路的电流源100nA到IO口的选通使能。
  * 输入  NewState: PLL内部低功耗偏置电路的电流源100nA到IO口的选通使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_Low_Power_100nA_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSLCDIS位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开内部低功耗偏置电路的电流源100nA到IO口的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSLCDIS_POS);
    }
    else
    {
        /* 关闭内部低功耗偏置电路的电流源100nA到IO口的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSLCDIS_POS);
    }
}

/**
  * 描述  配置PLL内部高频LDO到内部buffer的选通使能。
  * 输入  NewState: PLL内部高频LDO到内部buffer的选通使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_LDO_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置PLL_CTL0寄存器TSHSILDO12位 ----------------*/
    if (NewState != FALSE)
    {
        /* 打开内部高频LDO到内部buffer的通道 */
        SFR_SET_BIT_ASM(PLL_CTL, PLL_CTL_TSHSILDO12_POS);
    }
    else
    {
        /* 关闭内部高频LDO到内部buffer的通道 */
        SFR_CLR_BIT_ASM(PLL_CTL, PLL_CTL_TSHSILDO12_POS);
    }
}

/**
  * 描述  配置PLL中断使能。
  * 输入  NewState: PLL中断使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_PLL_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_INT寄存器PLLIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_INT, OSC_INT_PLLIE_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_INT, OSC_INT_PLLIE_POS);
    }
}

/**
  * 描述  配置外部高频中断使能。
  * 输入  NewState: 外部高频中断使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_EXTHF_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_INT寄存器EHFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_INT, OSC_INT_EHFIE_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_INT, OSC_INT_EHFIE_POS);
    }
}

/**
  * 描述  配置外部低频中断使能。
  * 输入  NewState: 外部低频中断使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_EXTLF_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_INT寄存器ELFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_INT, OSC_INT_ELFIE_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_INT, OSC_INT_ELFIE_POS);
    }
}

/**
  * 描述  配置内部高频中断使能。
  * 输入  NewState: 内部高频中断使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_INTHF_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_INT寄存器IHFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_INT, OSC_INT_IHFIE_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_INT, OSC_INT_IHFIE_POS);
    }
}

/**
  * 描述  配置内部低频中断使能。
  * 输入  NewState: 内部低频中断使能状态，取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
OSC_INTLF_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置OSC_INT寄存器ILFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 中断使能 */
        SFR_SET_BIT_ASM(OSC_INT, OSC_INT_ILFIE_POS);
    }
    else
    {
        /* 中断禁止 */
        SFR_CLR_BIT_ASM(OSC_INT, OSC_INT_ILFIE_POS);
    }
}

/**
  * 描述  读取时钟故障标志。
  * 输入  无。
  * 返回  时钟故障状态，0：没有时钟故障，1：检测到时钟故障。
  */
FlagStatus
OSC_Get_Clock_Failure_INT_Flag (void)
{
    /*---------------- 设置OSC_INT寄存器CKFIF位 ----------------*/
    if (OSC_INT & OSC_INT_CKFIF)
    {
        /* 检测到时钟故障 */
        return SET;
    }
    else
    {
        /* 没有时钟故障 */
        return RESET;
    }
}

/**
  * 描述  读取PLL中断标志。
  * 输入  无。
  * 返回  中断状态，0：未发生中断，1：发生中断。
  */
FlagStatus
OSC_Get_PLL_INT_Flag (void)
{
    /*---------------- 设置OSC_INT寄存器PLLIF位 ----------------*/
    if (OSC_INT & OSC_INT_PLLIF)
    {
        /* PLL锁定 */
        return SET;
    }
    else
    {
        /* PLL未锁定 */
        return RESET;
    }
}

/**
  * 描述  读取外部高频中断标志。
  * 输入  无。
  * 返回  中断状态，0：未发生中断，1：发生中断。
  */
FlagStatus
OSC_Get_EXTHF_INT_Flag (void)
{
    /*---------------- 设置OSC_INT寄存器EHFIF位 ----------------*/
    if (OSC_INT & OSC_INT_EHFIF)
    {
        /* PLL锁定 */
        return SET;
    }
    else
    {
        /* PLL未锁定 */
        return RESET;
    }
}

/**
  * 描述  读取外部低频中断标志。
  * 输入  无。
  * 返回  中断状态，0：未发生中断，1：发生中断。
  */
FlagStatus
OSC_Get_EXTLF_INT_Flag (void)
{
    /*---------------- 设置OSC_INT寄存器ELFIF位 ----------------*/
    if (OSC_INT & OSC_INT_ELFIF)
    {
        /* PLL锁定 */
        return SET;
    }
    else
    {
        /* PLL未锁定 */
        return RESET;
    }
}

/**
  * 描述  读取内部高频中断标志。
  * 输入  无。
  * 返回  中断状态，0：未发生中断，1：发生中断。
  */
FlagStatus
OSC_Get_INTHF_INT_Flag (void)
{
    /*---------------- 设置OSC_INT寄存器IHFIF位 ----------------*/
    if (OSC_INT & OSC_INT_IHFIF)
    {
        /* PLL锁定 */
        return SET;
    }
    else
    {
        /* PLL未锁定 */
        return RESET;
    }
}

/**
  * 描述  读取内部低频中断标志。
  * 输入  无。
  * 返回  中断状态，0：未发生中断，1：发生中断。
  */
FlagStatus
OSC_Get_INTLF_INT_Flag (void)
{
    /*---------------- 设置OSC_INT寄存器ILFIF位 ----------------*/
    if (OSC_INT & OSC_INT_ILFIF)
    {
        /* PLL锁定 */
        return SET;
    }
    else
    {
        /* PLL未锁定 */
        return RESET;
    }
}
/**
  *   ##### 振荡器(OSC)中断控制函数定义结束 #####
  */

