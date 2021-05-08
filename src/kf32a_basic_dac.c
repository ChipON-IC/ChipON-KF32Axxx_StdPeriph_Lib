/**
  ******************************************************************************
  * 文件名  kf32a_basic_dac.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了DAC外设功能函数，包含：
  *          + 初始化及配置函数
  *          + 数据读写函数
  *          + DAC校验寄存器配置函数
  *
  *********************************************************************
  */

#include "kf32a_basic_dac.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* DAC私有定义 ----------------------------------------------------*/
/* DACx_CTL寄存器配置掩码 */
#define DAC_CTL_INIT_MASK               (DAC_CTL_TRIGEN \
                                       | DAC_CTL_TREVT \
                                       | DAC_CTL_DE \
                                       | DAC_CTL_WAVE \
                                       | DAC_CTL_MAS)

/* DACx_CTL寄存器配置掩码 */
#define DAC_CTL1_INIT_MASK              (DAC_CTL1_CLK \
                                       | DAC_CTL1_RFS \
                                       | DAC_CTL1_BUFEN \
                                       | DAC_CTL1_SEL)

/* DACx_DAHD寄存器配置掩码 */
#define DAC_CAL_INIT_MASK               (DAC_CAL_PPOSITRIM \
                                       | DAC_CAL_PNEGATRIM \
                                       | DAC_CAL_NPOSITRIM \
                                       | DAC_CAL_NNEGATRIM \
                                       | DAC_CAL_BUFTRIM)


/**
  *   ##### 初始化及配置函数 #####
  */
/**
  * 描述  DAC外设复位，使能外设时钟。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  * 返回  无。
  */
void
DAC_Reset (DAC_SFRmap* DACx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));

#ifdef KF32A_Periph_dac0
    if (DACx == DAC0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_DAC0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_dac1
    if (DACx == DAC1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_DAC1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_DAC1CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  DAC外设配置。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       dacInitStruct: DAC配置信息
  * 返回  无。
  */
void
DAC_Configuration (DAC_SFRmap* DACx, DAC_InitTypeDef* dacInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacInitStruct->m_TriggerEnable));
    CHECK_RESTRICTION(CHECK_DAC_TRIGGER_EVENT(dacInitStruct->m_TriggerEvent));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacInitStruct->m_TriggerDMAEnable));
    CHECK_RESTRICTION(CHECK_DAC_WAVE(dacInitStruct->m_Wave));
    CHECK_RESTRICTION(CHECK_DAC_MAS(dacInitStruct->m_Mas));
    CHECK_RESTRICTION(CHECK_DAC_CLK(dacInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_DAC_CLK_DIV(dacInitStruct->m_ClockDiv));
    CHECK_RESTRICTION(CHECK_DAC_RFS(dacInitStruct->m_ReferenceVoltage));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacInitStruct->m_OutputBuffer));
    CHECK_RESTRICTION(CHECK_DAC_OUTPUT_PIN(dacInitStruct->m_OutputPin));
    CHECK_RESTRICTION(CHECK_DAC_OUTPUT(dacInitStruct->m_Output));

    /*---------------------------- DACx_CTL寄存器配置  -----------------*/
    /* 根据结构体成员m_TriggerEnable，设置TRIGEN位域 */
    /* 根据结构体成员m_TriggerEvent，设置TREVT位域 */
    /* 根据结构体成员m_TriggerDMAEnable，设置DE位域 */
    /* 根据结构体成员m_Wave，设置WAVE位域 */
    /* 根据结构体成员m_Mas，设置MAS位域 */
    tmpreg = ((dacInitStruct->m_TriggerEnable << DAC_CTL_TRIGEN_POS) | \
              (dacInitStruct->m_TriggerEvent) | \
              (dacInitStruct->m_TriggerDMAEnable << DAC_CTL_DE_POS) | \
              (dacInitStruct->m_Wave) | \
              (dacInitStruct->m_Mas));
    DACx->CTL = SFR_Config (DACx->CTL, ~DAC_CTL_INIT_MASK, tmpreg);

    /*---------------------------- DACx_CTL1寄存器配置 -----------------*/
    /* 根据结构体成员m_ClockDiv，设置CLKDIV位域 */
    /* 根据结构体成员m_Clock，设置CLK位域 */
    /* 根据结构体成员m_ReferenceVoltage，设置RFS位域 */
    /* 根据结构体成员m_OutputBuffer，设置BUFEN位域 */
    /* 根据结构体成员m_OutputPin，设置SEL位域 */
    tmpreg = (dacInitStruct->m_ClockDiv)\
    	   | (dacInitStruct->m_Clock) \
           | (dacInitStruct->m_ReferenceVoltage) \
           | (dacInitStruct->m_OutputBuffer << DAC_CTL1_BUFEN_POS) \
           | (dacInitStruct->m_OutputPin);
    DACx->CTL1 = SFR_Config (DACx->CTL1, ~DAC_CTL1_INIT_MASK, tmpreg);

    /*---------------------------- DACx_DAHD寄存器配置 -----------------*/
    /* 根据结构体成员m_Output，设置DAHD位域 */
    tmpreg = dacInitStruct->m_Output;
    DACx->DAHD = SFR_Config (DACx->DAHD, ~DAC_DAHD_DAHD, tmpreg);
}

/**
  * 描述  初始化DAC配置信息结构体。
  * 输入  dacInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
DAC_Struct_Init (DAC_InitTypeDef* dacInitStruct)
{
    /* 初始化 DAC通道触发使能 */
    dacInitStruct->m_TriggerEnable = FALSE;
    /* 初始化 DAC通道触发事件 */
    dacInitStruct->m_TriggerEvent = DAC_TRIGGER_T1_TRGO;
    /* 初始化 DAC触发DMA使能 */
    dacInitStruct->m_TriggerDMAEnable = FALSE;
    /* 初始化 DAC波形发生器使能 */
    dacInitStruct->m_Wave = DAC_WAVE_NONE;
    /* 初始化 DAC屏蔽/幅值选择器 */
    dacInitStruct->m_Mas = DAC_LFSR_UNMASK_BITS0_0;
    /* 初始化 DAC工作时钟 */
    dacInitStruct->m_Clock = DAC_CLK_SCLK;
    /* 初始化 DAC时钟分频 */
    dacInitStruct->m_ClockDiv = DAC_CLK_DIV_1;
    /* 初始化 DAC参考电压选择 */
    dacInitStruct->m_ReferenceVoltage = DAC_RFS_AVDD;
    /* 初始化 DAC输出缓冲使能 */
    dacInitStruct->m_OutputBuffer = FALSE;
    /* 初始化 DAC输出引脚 */
    dacInitStruct->m_OutputPin = DAC_OUTPUT_PIN_0;
    /* 初始化 DAC输出数据 */
    dacInitStruct->m_Output = 0;
}

/**
  * 描述  控制DAC使能位。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       NewState: DAC使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
DAC_Cmd (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能DAC */
        SFR_SET_BIT_ASM(DACx->CTL, DAC_CTL_EN_POS);
    }
    else
    {
        /* 禁止DAC */
        SFR_CLR_BIT_ASM(DACx->CTL, DAC_CTL_EN_POS);
    }
}

/**
  * 描述  控制DAC使能位。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       NewState: DAC使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
DAC_Software_Trigger_Cmd (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能DAC软件触发 */
        SFR_SET_BIT_ASM(DACx->CTL, DAC_CTL_SWTRIG_POS);
    }
    else
    {
        /* 禁止DAC软件触发 */
        SFR_CLR_BIT_ASM(DACx->CTL, DAC_CTL_SWTRIG_POS);
    }
}

/**
  * 描述  控制DAC通道DMA模式使能。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       NewState: DAC使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
DAC_DMA_Cmd (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能DAC通道DMA模式 */
        SFR_SET_BIT_ASM(DACx->CTL, DAC_CTL_DE_POS);
    }
    else
    {
        /* 禁止DAC通道DMA模式 */
        SFR_CLR_BIT_ASM(DACx->CTL, DAC_CTL_DE_POS);
    }
}

/**
  * 描述  控制DAC输出缓冲使能。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       NewState: DAC使能位配置信息，
  *                 取值为 TRUE 或 FALSE，分别表示输出带缓冲或不带缓冲。
  * 返回  无。
  */
void
DAC_Output_Buffer_Config (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能DAC输出缓冲 */
        SFR_SET_BIT_ASM(DACx->CTL1, DAC_CTL1_BUFEN_POS);
    }
    else
    {
        /* 禁止DAC输出缓冲 */
        SFR_CLR_BIT_ASM(DACx->CTL1, DAC_CTL1_BUFEN_POS);
    }
}

/**
  * 描述  控制DAC输出IO口选择。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       NewState: DAC使能位配置信息，
  *                 取值为 TRUE 或 FALSE，分别表示输出到IO口1，或输出到IO口0。
  * 返回  无。
  */
void
DAC_Output_Port_Config (DAC_SFRmap* DACx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* DAC输出到IO口1 */
        SFR_SET_BIT_ASM(DACx->CTL1, DAC_CTL1_SEL_POS);
    }
    else
    {
        /* DAC输出到IO口0 */
        SFR_CLR_BIT_ASM(DACx->CTL1, DAC_CTL1_SEL_POS);
    }
}

/**
  *   ##### 初始化及配置函数结束 #####
  */



/**
  *   ##### 数据读写函数 #####
  */
/**
  * 描述  写DAC输出数据，只能写如DACx_DAHD寄存器。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。
  *       Value: 写入数据寄存器的值，
  *              取值为12位数据。
  * 返回  无。
  */
void
DAC_Write_Output_Data (DAC_SFRmap* DACx, uint32_t Value)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_DAC_OUTPUT(Value));

    DACx->DAHD = Value;
}

/**
  * 描述  读DAC输出数据。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。。
  * 返回  32位类型数据的12位数据。
  */
uint32_t
DAC_Read_Output_Data (DAC_SFRmap* DACx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));

    return (DAC_DAHD_DAHD && DACx->DATA);
}

/**
  *   ##### 数据读写函数结束 #####
  */



/**
  *   ##### DAC校验寄存器配置函数 #####
  */
/**
  * 描述  DAC校验寄存器配置函数。
  * 输入  DACx: 指向DAC内存结构的指针，取值为DAC0_SFR、DAC1_SFR。。
  *       dacCalStruct: DAC校验寄存器配置信息
  * 返回  32位类型数据的12位数据。
  */
void
DAC_Calibration_Config (DAC_SFRmap* DACx, DAC_CALTypeDef* dacCalStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DAC_ALL_PERIPH(DACx));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_PPositrim));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_PNegatrim));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_NPositrim));
    CHECK_RESTRICTION(CHECK_DAC_CAL(dacCalStruct->m_NNegatrim));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dacCalStruct->m_Buftrim));

    /*---------------------------- DACx_CAL寄存器配置  -----------------*/
    /* 根据结构体成员m_PPositrim，设置PPOSITRIM位域 */
    /* 根据结构体成员m_PNegatrim，设置PNEGATRIM位域 */
    /* 根据结构体成员m_NPositrim，设置NPOSITRIM位域 */
    /* 根据结构体成员m_NNegatrim，设置NNEGATRIM位域 */
    tmpreg = (dacCalStruct->m_PPositrim << DAC_CAL_PPOSITRIM0_POS) | \
             (dacCalStruct->m_PNegatrim << DAC_CAL_PNEGATRIM0_POS) | \
             (dacCalStruct->m_NPositrim << DAC_CAL_NPOSITRIM0_POS) | \
             (dacCalStruct->m_NNegatrim << DAC_CAL_NNEGATRIM0_POS) | \
             (dacCalStruct->m_Buftrim << DAC_CAL_BUFTRIM_POS);
    DACx->CAL = SFR_Config (DACx->CAL, ~DAC_CAL_INIT_MASK, tmpreg);
}

/**
  * 描述  初始化DAC配置信息结构体。
  * 输入  dacCalStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
DAC_Calibration_Struct_Init (DAC_CALTypeDef* dacCalStruct)
{
    /* 初始化 DAC模块BUFFER输入P管正失调方向校准位 */
    dacCalStruct->m_PPositrim = DAC_CAL_0;
    /* 初始化 DAC模块BUFFER输入P管负失调方向校准位 */
    dacCalStruct->m_PNegatrim = DAC_CAL_0;
    /* 初始化 DAC模块BUFFER输入N管正失调方向校准位 */
    dacCalStruct->m_NPositrim = DAC_CAL_0;
    /* 初始化 DAC模块BUFFER输入N管负失调方向校准位 */
    dacCalStruct->m_NNegatrim = DAC_CAL_0;
    /* 初始化 DAC模块BUFFER失调校准使能位 */
    dacCalStruct->m_Buftrim = FALSE;
}

/**
  *   ##### DAC校验寄存器配置函数结束 #####
  */
