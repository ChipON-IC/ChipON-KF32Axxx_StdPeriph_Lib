/**
  ******************************************************************************
  * 文件名  kf32a_basic_adc.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了模数转换模块(ADC)外设功能函数，包含：
  *          + 模数转换模块(ADC)初始化函数
  *          + 模数转换模块(ADC)模拟看门狗初始化函数
  *          + 模数转换模块(ADC)控制功能配置函数
  *          + 模数转换模块(ADC)常规通道功能配置函数
  *          + 模数转换模块(ADC)高优先级通道功能配置函数
  *          + 模数转换模块(ADC)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_adc.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* 模数转换模块(ADC)私有定义 -------------------------------------------------*/

/* ADCx_CTL0寄存器初始化配置掩码 */
#define ADC_CTL0_INIT_MASK             (ADC_CTL0_CLKS \
                                       | ADC_CTL0_ADCCLKS \
                                       | ADC_CTL0_SCANEN \
                                       | ADC_CTL0_CONT \
                                       | ADC_CTL0_LR)

/* ADCx_CTL0寄存器模拟看门狗配置掩码 */
#define ADC_CTL0_AWDMODE_MASK           (ADC_CTL0_WDSCH \
                                       | ADC_CTL0_HPWDEN \
                                       | ADC_CTL0_WDEN)

/* ADCx_CTL1寄存器初始化配置掩码 */
#define ADC_CTL1_INIT_MASK              (ADC_CTL1_EXTEN \
                                       | ADC_CTL1_HPEXTEN \
                                       | ADC_CTL1_NCH \
                                       | ADC_CTL1_HCH \
                                       | ADC_CTL1_REF)

/* ADC0_DELAY寄存器初始化配置掩码 */
#define ADC0_DELAY_INIT_MASK            (ADC0_DELAY_FDELAY \
                                       | ADC0_DELAY_SDELAY)


/**
  *   ##### 模数转换模块(ADC)初始化函数 #####
  */
/**
  * 描述  复位ADC外设，使能外设时钟。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  * 返回  无。
  */
void
ADC_Reset (ADC_SFRmap* ADCx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));

#ifdef KF32A_Periph_adc0
    if (ADCx == ADC0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_ADC0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_ADC0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_ADC0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_adc1
    if (ADCx == ADC1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_ADC1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_ADC1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_ADC1CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_adc2
    if (ADCx == ADC2_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_ADC2RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_ADC2RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_ADC2CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  模数转换模块(ADC)初始化配置。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       adcInitStruct: ADC模块配置信息结构体指针。
  * 返回  无。
  */
void
ADC_Configuration (ADC_SFRmap* ADCx, ADC_InitTypeDef* adcInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_CLK(adcInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_ADC_CLK_DIV(adcInitStruct->m_ClockDiv));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(adcInitStruct->m_ScanMode));
    CHECK_RESTRICTION(CHECK_ADC_CONV_MODE(adcInitStruct->m_ContinuousMode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(adcInitStruct->m_ExternalTrig_EN));
    CHECK_RESTRICTION(CHECK_ADC_EXT_TRIG(adcInitStruct->m_ExternalTrig));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(adcInitStruct->m_HPExternalTrig_EN));
    CHECK_RESTRICTION(CHECK_ADC_HPEXT_TRIG(adcInitStruct->m_HPExternalTrig));
    CHECK_RESTRICTION(CHECK_ADC_DATA_ALIGN(adcInitStruct->m_DataAlign));
    CHECK_RESTRICTION(CHECK_ADC_REF(adcInitStruct->m_VoltageRef));
    CHECK_RESTRICTION(CHECK_ADC_REGULAR_LENGTH(adcInitStruct->m_NumOfConv));
    CHECK_RESTRICTION(CHECK_ADC_HIGH_PRIORITY_LENGTH(adcInitStruct->m_NumOfHPConv));

    /*---------------- 设置ADC_CTL0寄存器 ----------------*/
    /* 根据结构体成员m_Clock，设置CLKS位域 */
    /* 根据结构体成员m_ClockDiv，设置ADCCLKS位域 */
    /* 根据结构体成员m_ScanMode，设置SCANEN位域 */
    /* 根据结构体成员m_ContinuousMode，设置CONT位域 */
    /* 根据结构体成员m_DataAlign，设置LR位域 */
    tmpreg = (uint32_t)(adcInitStruct->m_Clock | \
                        adcInitStruct->m_ClockDiv | \
                        ((uint32_t)adcInitStruct->m_ScanMode << 17) | \
                        adcInitStruct->m_ContinuousMode | \
                        adcInitStruct->m_DataAlign
                        );
    ADCx->CTL0 = SFR_Config (ADCx->CTL0, ~ADC_CTL0_INIT_MASK, tmpreg);

    /*---------------- 设置ADC_CTL1寄存器 ----------------*/
    /* 根据结构体成员m_ExternalTrig_EN，设置EXTEN位域 */
    /* 根据结构体成员m_HPExternalTrig_EN，设置HPEXTEN位域 */
    /* 根据结构体成员m_ExternalTrig，设置NCH位域 */
    /* 根据结构体成员m_HPExternalTrig，设置HCH位域 */
    /* 根据结构体成员m_VoltageRef，设置REF位域 */
    tmpreg = (uint32_t)(((uint32_t)adcInitStruct->m_ExternalTrig_EN << 11) | \
                         ((uint32_t)adcInitStruct->m_HPExternalTrig_EN << 27) | \
                         adcInitStruct->m_ExternalTrig | \
                         adcInitStruct->m_HPExternalTrig | \
                         adcInitStruct->m_VoltageRef);
    ADCx->CTL1 = SFR_Config (ADCx->CTL1, ~ADC_CTL1_INIT_MASK, tmpreg);

    /*---------------- 设置ADC_SCANSQ3寄存器 ----------------*/
    /* 根据结构体成员m_NumOfConv，设置SLEN位域 */
    tmpreg = (adcInitStruct->m_NumOfConv - 1) << 28;
    ADCx->SCANSQ3 = SFR_Config (ADCx->SCANSQ3,
                                  ~ADC_SCANSQ3_SLEN,
                                  tmpreg);

    /*---------------- 设置ADC_HSCANSQ寄存器 ----------------*/
    /* 根据结构体成员m_NumOfHPConv，设置HSLEN位域 */
    tmpreg = (adcInitStruct->m_NumOfHPConv - 1) << 28;
    ADCx->HSCANSQ = SFR_Config (ADCx->HSCANSQ,
                                  ~ADC_HSCANSQ_HSLEN,
                                  tmpreg);
}

/**
  * 描述  初始化ADC配置信息结构体。
  * 输入  adcInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
ADC_Struct_Init (ADC_InitTypeDef* adcInitStruct)
{
    /* 初始化 ADC时钟源选择 */
    adcInitStruct->m_Clock = ADC_SCLK;

    /* 初始化 ADC时钟分频 */
    adcInitStruct->m_ClockDiv = ADC_CLK_DIV_2;

    /* 初始化 ADC扫描模式使能 */
    adcInitStruct->m_ScanMode = FALSE;

    /* 初始化 ADC连续转换模式 */
    adcInitStruct->m_ContinuousMode = ADC_SINGLE_MODE;

    /* 初始化 ADC转换结果输出格式 */
    adcInitStruct->m_DataAlign = ADC_DATAALIGN_LEFT;

    /* 初始化 ADC常规通道外部触发转换模式使能 */
    adcInitStruct->m_ExternalTrig_EN = FALSE;

    /* 初始化 ADC常规通道外部触发事件 */
    adcInitStruct->m_ExternalTrig = ADC_EXTERNALTRIG_CCP1_CH1;

    /* 初始化 ADC高优先级通道外部触发转换模式使能 */
    adcInitStruct->m_HPExternalTrig_EN = FALSE;

    /* 初始化 高优先级通道外部触发事件 */
    adcInitStruct->m_HPExternalTrig = ADC_HPEXTERNALTRIG_CCP1_CH1;

    /* 初始化 ADC参考电压选择 */
    adcInitStruct->m_VoltageRef = ADC_REF_AVDD;

    /* 初始化 ADC常规通道扫描长度 */
    adcInitStruct->m_NumOfConv = 1;

    /* 初始化 ADC高优先级通道扫描长度 */
    adcInitStruct->m_NumOfHPConv = 1;
}

/**
  * 描述  ADC0_DELAY初始化配置。
  * 输入  adc0Delay: ADC0快慢交叉模式信息结构体。
  * 返回  无。
  */
void
ADC_Delay_Configuration (ADC0_DELAY_InitTypeDef* adc0Delay)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_FDELAY_MODE(adc0Delay->m_FastDelay));
    CHECK_RESTRICTION(CHECK_ADC_SDELAY_MODE(adc0Delay->m_SlowDelay));

    /*---------------- 设置ADC0_DELAY寄存器 ----------------*/
    /* 根据结构体成员m_SlowDelay，设置SDELAY位域 */
    /* 根据结构体成员m_FastDelay，设置FDELAY位域 */
    tmpreg = (((uint32_t)(adc0Delay->m_SlowDelay - 1) << 16) | \
              ((uint32_t)(adc0Delay->m_FastDelay - 1)));
#ifdef KF32A_Periph_adc0
    ADC0_DELAY = SFR_Config (ADC0_DELAY,
                               ~ADC0_DELAY_INIT_MASK,
                               tmpreg);
#endif
}

/**
  * 描述  初始化ADC0快慢交叉模式信息结构体。
  * 输入  adc0Delay: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
ADC_Delay_Struct_Init (ADC0_DELAY_InitTypeDef* adc0Delay)
{
    /* 初始化 ADC快速交叉模式延时时间 */
    adc0Delay->m_FastDelay = 0x06;

    /* 初始化 ADC慢速交叉模式延时时间 */
    adc0Delay->m_SlowDelay = 0x0D;
}

/**
  * 描述  配置ADC工作使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: ADC工作使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Cmd (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能A/D转换模块 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_EN_POS);
    }
    else
    {
        /* A/D转换器关闭且不消耗工作电流 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_EN_POS);
    }
}
/**
  *   ##### 模数转换模块(ADC)初始化函数定义结束 #####
  */


/**
  *   ##### 模数转换模块(ADC)模拟看门狗初始化函数 #####
  */
/**
  * 描述  ADC模拟看门狗初始化配置。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       adcAnalogWatchdog: ADC模拟看门狗信息结构体指针。
  * 返回  无。
  */
void
ADC_Analog_Watchdog_Configuration (ADC_SFRmap* ADCx,
                    ADC_WD_InitTypeDef * adcAnalogWatchdog)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_WDSCH(adcAnalogWatchdog->m_WDSingleCH));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(adcAnalogWatchdog->m_HPChannelWDEN));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(adcAnalogWatchdog->m_ChannelWDEN));
    CHECK_RESTRICTION(CHECK_ADC_WDCHANNEL(adcAnalogWatchdog->m_WDChannel));
    CHECK_RESTRICTION(CHECK_ADC_THRESHOLD(adcAnalogWatchdog->m_Threshold_H));
    CHECK_RESTRICTION(CHECK_ADC_THRESHOLD(adcAnalogWatchdog->m_Threshold_L));

    /*---------------- 设置ADC_CTL0寄存器 ----------------*/
    /* 根据结构体成员m_WDSingleCH，设置WDSCH位域 */
    /* 根据结构体成员m_HPChannelWDEN，设置HPWDEN位域 */
    /* 根据结构体成员m_ChannelWDEN，设置WDEN位域 */
    tmpreg = (uint32_t)(adcAnalogWatchdog->m_WDSingleCH | \
                  ((uint32_t)adcAnalogWatchdog->m_HPChannelWDEN << 30) | \
                  ((uint32_t)adcAnalogWatchdog->m_ChannelWDEN << 29));
    ADCx->CTL0 = SFR_Config (ADCx->CTL0,
                               ~ADC_CTL0_AWDMODE_MASK,
                               tmpreg);

    /*---------------- 设置ADC_SCANSQ3寄存器 ----------------*/
    /* 根据结构体成员m_WDChannel，设置WDCH位域 */
    tmpreg = adcAnalogWatchdog->m_WDChannel;
    ADCx->SCANSQ3 = SFR_Config (ADCx->SCANSQ3,
                               ~ADC_SCANSQ3_WDCH,
                               tmpreg);

    /*---------------- 设置ADC_WDH寄存器 ----------------*/
    /* 根据结构体成员m_Threshold_H，设置WDH位域 */
    tmpreg = adcAnalogWatchdog->m_Threshold_H;
    ADCx->WDH = SFR_Config (ADCx->WDH,
                              ~ADC_WDH_WDH,
                              tmpreg);

    /*---------------- 设置ADC_WDL寄存器 ----------------*/
    /* 根据结构体成员m_Threshold_L，设置WDL位域 */
    tmpreg = adcAnalogWatchdog->m_Threshold_L;
    ADCx->WDL = SFR_Config (ADCx->WDL,
                              ~ADC_WDL_WDL,
                              tmpreg);
}

/**
  * 描述  初始化ADC模拟看门狗信息结构体。
  * 输入  adcAnalogWatchdog: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
ADC_Analog_Watchdog_Struct_Init (ADC_WD_InitTypeDef* adcAnalogWatchdog)
{
    /* 初始化 ADC模拟看门狗单通道使能 */
    adcAnalogWatchdog->m_WDSingleCH = ADC_WD_ALL_CH;

    /* 初始化 ADC高优先级通道上看门狗使能 */
    adcAnalogWatchdog->m_HPChannelWDEN = FALSE;

    /* 初始化 ADC常规通道上看门狗使能 */
    adcAnalogWatchdog->m_ChannelWDEN = FALSE;

    /* 初始化 ADC模拟看门狗通道选择 */
    adcAnalogWatchdog->m_WDChannel = ADC_WDCH_0;

    /* 初始化 ADC模拟看门狗高阈值 */
    adcAnalogWatchdog->m_Threshold_H = 0x0;

    /* 初始化 ADC模拟看门狗低阈值 */
    adcAnalogWatchdog->m_Threshold_L = 0x0;
}
/**
  *   ##### 模数转换模块(ADC)模拟看门狗初始化函数定义结束 #####
  */


/**
  *   ##### 模数转换模块(ADC)控制功能配置函数 #####
  */
/**
  * 描述  配置扫描模式中模拟看门狗单一通道使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 模拟看门狗单一通道使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Watchdog_Single_Channel_Enable (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器WDSCH位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能单一通道 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_WDSCH_POS);
    }
    else
    {
        /* 使能所有通道 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_WDSCH_POS);
    }
}

/**
  * 描述  配置扫描模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 扫描模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Scan_Mode_Enable (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器SCANEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能扫描模式 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_SCANEN_POS);
    }
    else
    {
        /* 禁止扫描模式 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_SCANEN_POS);
    }
}

/**
  * 描述  配置比较器的运放输入参考电压二分之一校准。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 校准配置状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Bosssel_Comparator_Calibration (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器BOSSSELCOMP位 ----------------*/
    if (NewState != FALSE)
    {
        /* 校准，比较器输入端电压更准确 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_BOSSSELCOMP_POS);
    }
    else
    {
        /* 不校准 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_BOSSSELCOMP_POS);
    }
}

/**
  * 描述  配置参考电压二分之一运放电压校准。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 校准配置状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Bosssel_Calibration (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器BOSSSEL位 ----------------*/
    if (NewState != FALSE)
    {
        /* 校准，运放输出电压更准确 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_BOSSSEL_POS);
    }
    else
    {
        /* 不校准 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_BOSSSEL_POS);
    }
}

/**
  * 描述  配置偏置电流校准。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 校准配置状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Trim_Current_Intensity_Bias (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器TRIMIBIAS位 ----------------*/
    if (NewState != FALSE)
    {
        /* 校准，偏置电流变大 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_TRIMIBIAS_POS);
    }
    else
    {
        /* 不校准 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_TRIMIBIAS_POS);
    }
}

/**
  * 描述  配置ADC模拟部分时钟分频比选择。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       ClockSelect: ADC模拟部分时钟分频比，取值范围为:
  *                      ADC_CLK_DIV_1: T
  *                      ADC_CLK_DIV_2: 1/2T
  *                      ADC_CLK_DIV_4: 1/4T
  *                      ADC_CLK_DIV_8: 1/8T
  *                      ADC_CLK_DIV_16: 1/16T
  *                      ADC_CLK_DIV_32: 1/32T
  *                      ADC_CLK_500KHZ: 500KHz
  * 返回  无。
  */
void
ADC_Analog_Clock_Config (ADC_SFRmap* ADCx, uint32_t ClockSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_CLK_DIV(ClockSelect));

    /*------------- 设置ADCx_CTL0寄存器ADCCLKS位 -------------*/
    ADCx->CTL0 = SFR_Config (ADCx->CTL0,
                               ~ADC_CTL0_ADCCLKS,
                               ClockSelect);
}

/**
  * 描述  配置A/D转换结果输出格式。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       DataAlign: ADC转换结果对齐格式，取值范围为：
  *                    ADC_DATAALIGN_LEFT: 转换结果存储时左对齐
  *                    ADC_DATAALIGN_RIGHT: 转换结果存储时右对齐
  * 返回  无。
  */
void
ADC_Data_Align_Config (ADC_SFRmap* ADCx, uint32_t DataAlign)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_DATA_ALIGN(DataAlign));

    /*------------- 设置ADCx_CTL0寄存器LR位 -------------*/
    ADCx->CTL0 = SFR_Config (ADCx->CTL0,
                               ~ADC_CTL0_LR,
                               DataAlign);
}

/**
  * 描述  配置A/D工作时钟源。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       ClockSource: ADC工作时钟源，取值范围为：
  *                      ADC_SCLK: 选用SCLK作为A/D数字模块工作时钟
  *                      ADC_HFCLK: 选用HFCLK作为A/D数字模块工作时钟
  *                      ADC_LFCLK: 选用LFCLK作为A/D数字模块工作时钟
  * 返回  无。
  */
void
ADC_Clock_Source_Config (ADC_SFRmap* ADCx, uint32_t ClockSource)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_CLK(ClockSource));

    /*------------- 设置ADCx_CTL0寄存器CLKS位 -------------*/
    ADCx->CTL0 = SFR_Config (ADCx->CTL0,
                               ~ADC_CTL0_CLKS,
                               ClockSource);
}

/**
  * 描述  配置直接存储器访问模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 直接存储器访问模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Regular_Channel_DMA_Cmd (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使用DMA模式 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_NDMAEN_POS);
    }
    else
    {
        /* 不使用DMA模式 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_NDMAEN_POS);
    }
}

/**
  * 描述  配置直接存储器访问模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *          HPChannel: 高优先级通道选择，取值为：
  *                        ADC_HPCH0: 高优先级通道0
  *                        ADC_HPCH1: 高优先级通道1
  *                        ADC_HPCH2: 高优先级通道2
  *                        ADC_HPCH3: 高优先级通道3
  *       NewState: 直接存储器访问模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_High_Priority_Channel_DMA_Cmd (ADC_SFRmap* ADCx,
					uint32_t HPChannel,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_HPCH(HPChannel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));


    if (NewState != FALSE)
    {
    	ADCx->CTL0 |= HPChannel;
    }
    else
    {
    	ADCx->CTL0 &= ~HPChannel;
    }
}
/**
  * 描述  配置双ADC模式选择。
  * 输入  WorkMode: 双ADC模式，取值范围为：
  *                   ADC_SINGLE: 独立模式
  *                   ADC_REGULAR_HPRIORITY: 混合常规通道同步+高优先级通道同步模式
  *                   ADC_REGULAR_ALTERNATELY: 混合常规通道同步+交替触发模式
  *                   ADC_HPRIORITY_FAST_ALTERNATELY: 混合高优先级通道同步+快速交叉模式
  *                   ADC_HPRIORITY_SYNC: 高优先级同步模式
  *                   ADC_REGULAR_SYNC: 常规通道同步模式
  *                   ADC_FAST_ALTERNATELY_SYNC: 快速交叉同步
  *                   ADC_SLOW_ALTERNATELY_SYNC: 慢速交叉同步
  *                   ADC_ALTERNATELY_TRIGGER: 交替触发模式
  * 返回  无。
  */
void
ADC_Double_Mode_Config (uint32_t WorkMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_DOUBLE_MODE(WorkMode));

#ifdef KF32A_Periph_adc0
    /*------------- 设置ADC0_CTL1寄存器DOUMOD位 -------------*/
    ADC0_CTL1 = SFR_Config (ADC0_CTL1,
                               ~ADC_CTL1_DOUMOD,
                               WorkMode);
#endif
}

/**
  * 描述  配置ADC参考电压选择。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       RefVoltage: 双ADC模式，取值范围为：
  *                     ADC_REF_INTERNAL: 内部参考电压作为AD转换电压
  *                     ADC_REF_VREF: Vref+作为AD转换电压
  *                     ADC_REF_AVDD: AVDD作为AD转换电压
  * 返回  无。
  */
void
ADC_Reference_Voltage_Config (ADC_SFRmap* ADCx, uint32_t RefVoltage)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_REF(RefVoltage));

    /*------------- 设置ADCx_CTL1寄存器REF位 -------------*/
    ADCx->CTL1 = SFR_Config (ADCx->CTL1,
                               ~ADC_CTL1_REF,
                               RefVoltage);
}

/**
  * 描述  配置ADC模拟看门狗通道选择。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       Channel: ADC模拟看门狗通道，取值范围为ADC_WDCH_0~ADC_WDCH_25。
  * 返回  无。
  */
void
ADC_Analog_Watchdog_Channel_Config (ADC_SFRmap* ADCx, uint32_t Channel)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_WDCHANNEL(Channel));

    /*------------- 设置ADCx_CTL1寄存器WDCH位 -------------*/
    ADCx->SCANSQ3 = SFR_Config (ADCx->SCANSQ3,
                               ~ADC_SCANSQ3_WDCH,
                               Channel);
}
/**
  *   ##### 模数转换模块(ADC)控制功能配置函数定义结束 #####
  */


/**
  *   ##### 模数转换模块(ADC)常规通道功能配置函数 #####
  */
/**
  * 描述  配置常规通道上看门狗使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 常规通道上看门狗使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Regular_Channel_Watchdog_Enable (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器WDEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能常规通道上模拟看门狗 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_WDEN_POS);
    }
    else
    {
        /* 禁止常规通道上模拟看门狗 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_WDEN_POS);
    }
}

/**
  * 描述  配置常规通道外部触发转换模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 常规通道外部触发转换模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_External_Trig_Conv_Enable (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL1寄存器EXTEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能外部事件触发转换 */
        SFR_SET_BIT_ASM(ADCx->CTL1, ADC_CTL1_EXTEN_POS);
    }
    else
    {
        /* 禁止外部事件触发转换 */
        SFR_CLR_BIT_ASM(ADCx->CTL1, ADC_CTL1_EXTEN_POS);
    }
}

/**
  * 描述  配置常规通道外部触发事件。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       ExternalTrigEvent: 常规通道外部触发事件选择，取值为：
  *                            ADC_EXTERNALTRIG_CCP1_CH1: CCP1 通道 1
  *                            ADC_EXTERNALTRIG_CCP1_CH2: CCP1 通道 2
  *                            ADC_EXTERNALTRIG_CCP1_CH3: CCP1 通道 3
  *                            ADC_EXTERNALTRIG_CCP1_CH4: CCP1 通道 4
  *                            ADC_EXTERNALTRIG_CCP2_CH1: CCP2 通道 1
  *                            ADC_EXTERNALTRIG_CCP2_CH2: CCP2 通道 2
  *                            ADC_EXTERNALTRIG_CCP2_CH3: CCP2 通道 3
  *                            ADC_EXTERNALTRIG_CCP2_CH4: CCP2 通道 4
  *                            ADC_EXTERNALTRIG_CCP3_CH1: CCP3 通道 1
  *                            ADC_EXTERNALTRIG_CCP3_CH2: CCP3 通道 2
  *                            ADC_EXTERNALTRIG_CCP3_CH3: CCP3 通道 3
  *                            ADC_EXTERNALTRIG_CCP3_CH4: CCP3 通道 4
  *                            ADC_EXTERNALTRIG_CCP4_CH1: CCP4 通道 1
  *                            ADC_EXTERNALTRIG_CCP4_CH2: CCP4 通道 2
  *                            ADC_EXTERNALTRIG_CCP4_CH3: CCP4 通道 3
  *                            ADC_EXTERNALTRIG_CCP4_CH4: CCP4 通道 4
  *                            ADC_EXTERNALTRIG_CCP18_CH1: CCP18 通道 1
  *                            ADC_EXTERNALTRIG_CCP18_CH2: CCP18 通道 2
  *                            ADC_EXTERNALTRIG_CCP18_CH3: CCP18 通道 3
  *                            ADC_EXTERNALTRIG_CCP18_CH4: CCP18 通道 4
  *                            ADC_EXTERNALTRIG_CCP19_CH1: CCP19 通道 1
  *                            ADC_EXTERNALTRIG_CCP19_CH2: CCP19 通道 2
  *                            ADC_EXTERNALTRIG_CCP19_CH3: CCP19 通道 3
  *                            ADC_EXTERNALTRIG_CCP19_CH4: CCP19 通道 4
  *                            ADC_EXTERNALTRIG_CCP20_CH1: CCP20 通道 1
  *                            ADC_EXTERNALTRIG_CCP20_CH2: CCP20 通道 2
  *                            ADC_EXTERNALTRIG_CCP20_CH3: CCP20 通道 3
  *                            ADC_EXTERNALTRIG_CCP20_CH4: CCP20 通道 4
  *                            ADC_EXTERNALTRIG_CCP21_CH1: CCP21 通道 1
  *                            ADC_EXTERNALTRIG_CCP21_CH2: CCP21 通道 2
  *                            ADC_EXTERNALTRIG_CCP21_CH3: CCP21 通道 3
  *                            ADC_EXTERNALTRIG_CCP21_CH4: CCP21 通道 4
  *                            ADC_EXTERNALTRIG_CCP5_CH1: CCP5 通道 1
  *                            ADC_EXTERNALTRIG_CCP5_CH2: CCP5 通道 2
  *                            ADC_EXTERNALTRIG_CCP5_CH3: CCP5 通道 3
  *                            ADC_EXTERNALTRIG_CCP5_CH4: CCP5 通道 4
  *                            ADC_EXTERNALTRIG_CCP22_CH1: CCP22 通道 1
  *                            ADC_EXTERNALTRIG_CCP22_CH2: CCP22 通道 2
  *                            ADC_EXTERNALTRIG_CCP22_CH3: CCP22 通道 3
  *                            ADC_EXTERNALTRIG_CCP22_CH4: CCP22 通道 4
  *                            ADC_EXTERNALTRIG_T1TRGO: T1TRGO
  *                            ADC_EXTERNALTRIG_T2TRGO: T2TRGO
  *                            ADC_EXTERNALTRIG_T3TRGO: T3TRGO
  *                            ADC_EXTERNALTRIG_T4TRGO: T4TRGO
  *                            ADC_EXTERNALTRIG_T18TRGO: T18TRGO
  *                            ADC_EXTERNALTRIG_T19TRGO: T19TRGO
  *                            ADC_EXTERNALTRIG_T20TRGO: T20TRGO
  *                            ADC_EXTERNALTRIG_T21TRGO: T21TRGO
  *                            ADC_EXTERNALTRIG_T5TRGO: T5TRGO
  *                            ADC_EXTERNALTRIG_T9TRGO: T9TRGO
  *                            ADC_EXTERNALTRIG_T14TRGO: T14TRGO
  *                            ADC_EXTERNALTRIG_T15TRGO: T15TRGO
  *                            ADC_EXTERNALTRIG_T5_OVERFLOW
  *                            ADC_EXTERNALTRIG_T6_OVERFLOW
  *                            ADC_EXTERNALTRIG_T9_OVERFLOW
  *                            ADC_EXTERNALTRIG_T10_OVERFLOW
  *                            ADC_EXTERNALTRIG_CCP9_CH1
  *                            ADC_EXTERNALTRIG_CCP9_CH2
  *                            ADC_EXTERNALTRIG_CCP9_CH3
  *                            ADC_EXTERNALTRIG_CCP9_CH4
  *                            ADC_EXTERNALTRIG_EINT7: EINT7
  *                            ADC_EXTERNALTRIG_EINT15: EINT15
  *                            ADC_EXTERNALTRIG_CCP0_CH1
  *                            ADC_EXTERNALTRIG_CCP0_CH2
  *                            ADC_EXTERNALTRIG_CCP0_CH3
  *                            ADC_EXTERNALTRIG_CCP0_CH4
  *                            ADC_EXTERNALTRIG_CCP23_CH1
  *                            ADC_EXTERNALTRIG_CCP23_CH2
  *                            ADC_EXTERNALTRIG_CCP23_CH3
  *                            ADC_EXTERNALTRIG_CCP23_CH4
  * 返回  无。
  */
void
ADC_External_Trig_Conv_Config (ADC_SFRmap* ADCx,
                    uint32_t ExternalTrigEvent)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_EXT_TRIG(ExternalTrigEvent));

    /*------------- 设置ADCx_CTL1寄存器NCH位 -------------*/
    ADCx->CTL1 = SFR_Config (ADCx->CTL1,
                               ~ADC_CTL1_NCH,
                               ExternalTrigEvent);
}

/**
  * 描述  配置常规通道扫描序列。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       Channel: ADC通道选择，取值范围为:
  *                  ADC_CHANNEL_0: 通道0---- ADC_CHANNEL_63: 通道63
  *       Rank: 常规通道扫描AD输入的转换位置，取值为1~16。
  * 返回  无。
  */
void
ADC_Regular_Channel_Config (ADC_SFRmap* ADCx, uint32_t Channel, uint32_t Rank)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ADC_REGULAR_RANK(Rank));

    /* 配置第1次转换~第5次转换 */
    if (Rank < 6)
    {
        /*------------- 设置ADCx_SCANSQ0寄存器SSQx位 -------------*/
        tmpreg = (uint32_t)Channel << (6 * (Rank - 1));
        tmpmask = ADC_SCANSQ0_SSQ0 << (6 * (Rank - 1));
        ADCx->SCANSQ0 = SFR_Config (ADCx->SCANSQ0,
                              ~tmpmask,
                              tmpreg);
    }
    /* 配置第6次转换~第10次转换 */
    else if (Rank < 11)
    {
        /*------------- 设置ADCx_SCANSQ1寄存器SSQx位 -------------*/
        tmpreg = (uint32_t)Channel << (6 * (Rank - 6));
        tmpmask = ADC_SCANSQ1_SSQ5 << (6 * (Rank - 6));
        ADCx->SCANSQ1 = SFR_Config (ADCx->SCANSQ1,
                              ~tmpmask,
                              tmpreg);
    }
    /* 配置第11次转换~第15次转换 */
    else if (Rank < 16)
    {
        /*------------- 设置ADCx_SCANSQ2寄存器SSQx位 -------------*/
        tmpreg = (uint32_t)Channel << (6 * (Rank - 11));
        tmpmask = ADC_SCANSQ2_SSQ10 << (6 * (Rank - 11));
        ADCx->SCANSQ2 = SFR_Config (ADCx->SCANSQ2,
                              ~tmpmask,
                              tmpreg);
    }
    /* 配置第16次转换 */
    else
    {
        /*------------- 设置ADCx_SCANSQ3寄存器SSQx位 -------------*/
        tmpreg = (uint32_t)Channel << (6 * (Rank - 16));
        tmpmask = ADC_SCANSQ3_SSQ15 << (6 * (Rank - 16));
        ADCx->SCANSQ3 = SFR_Config (ADCx->SCANSQ3,
                              ~tmpmask,
                              tmpreg);
    }
}

/**
  * 描述  配置常规通道扫描长度。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       Length: 常规通道扫描长度，取值为1~16。
  * 返回  无。
  */
void
ADC_Regular_Sequencer_Length_Config (ADC_SFRmap* ADCx, uint32_t Length)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_REGULAR_LENGTH(Length));

    /*---------------- 设置ADC_SCANSQ3寄存器 ----------------*/
    /* 根据结构体成员m_NumOfConv，设置SLEN位域 */
    tmpreg = (Length - 1) << ADC_SCANSQ3_SLEN_0_POS;
    ADCx->SCANSQ3 = SFR_Config (ADCx->SCANSQ3,
                          ~ADC_SCANSQ3_SLEN,
                          tmpreg);
}

/**
  * 描述  配置常规优先级通道的 Tx_CCRy 触发使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       HPExternalTrigEvent: 常规优先级通道外部触发事件选择，取值为：
  *                              ADC_EXTERNALTRIG_T10_CCR0: 常规优先级通道的 T10_CCR0 触发使能
  *                              ADC_EXTERNALTRIG_T9_CCR1:  常规优先级通道的 T9_CCR1 触发使能
  *                              ADC_EXTERNALTRIG_T9_CCR0:  常规优先级通道的 T9_CCR0 触发使能
  *                              ADC_EXTERNALTRIG_T6_CCR0:  常规优先级通道的 T6_CCR0 触发使能
  *                              ADC_EXTERNALTRIG_T5_CCR1:  常规优先级通道的 T5_CCR1 触发使能
  *                              ADC_EXTERNALTRIG_T5_CCR0:  常规优先级通道的 T5_CCR0 触发使能
  *       NewState: ADC中断使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Regular_Channel_TxCCRy_Trig_Enable (ADC_SFRmap* ADCx,
							 uint32_t ExternalTrigEvent, FunctionalState NewState)
{
    uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));
    CHECK_RESTRICTION(CHECK_RegularChannel_TxCCRy_Trig(ExternalTrigEvent));

    /* 获取触发掩码 */
    tmpreg = ExternalTrigEvent;
    tmpreg = (uint32_t)0x01 << tmpreg;

    if (NewState != FALSE)
    {
        /* 使能触发 */
        ADCx->STATE |= tmpreg;
    }
    else
    {
        /* 禁止触发 */
        ADCx->STATE &= (~tmpreg);
    }
}

/**
  * 描述  软件启动A/D常规通道转换。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  * 返回  无。
  */
void
ADC_Software_Start_Conv(ADC_SFRmap* ADCx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));

    /*---------------- 配置ADC_CTL1寄存器START位 ----------------*/
    SFR_SET_BIT_ASM(ADCx->CTL1, ADC_CTL1_START_POS);
}

/**
  * 描述  配置连续转换使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 连续转换使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Continuous_Mode_Cmd (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 连续转换模式 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_CONT_POS);
    }
    else
    {
        /* 单次转换模式 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_CONT_POS);
    }
}

/**
  * 描述  配置间隔模式通道计数。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       Number: 间隔模式通道计数，取值范围为1~8。
  * 返回  无。
  */
void
ADC_Disc_Mode_Channel_Count_Config (ADC_SFRmap* ADCx, uint8_t Number)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_REGULAR_DISC_NUMBER(Number));

    /*---------------- 配置ADC_CTL0寄存器DSNUM位 ----------------*/
    tmpreg = (uint32_t)(Number - 1) << ADC_CTL0_DSNUM0_POS;
    ADCx->CTL0 = SFR_Config (ADCx->CTL0,
                               ~ADC_CTL0_DSNUM,
                               tmpreg);
}

/**
  * 描述  配置常规通道上的间隔模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 常规通道上的间隔模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Disc_Mode_Cmd (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能常规通道上的间隔模式 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_DSEN_POS);
    }
    else
    {
        /* 禁止常规通道上的间隔模式 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_DSEN_POS);
    }
}

/**
  * 描述  获取常规通道转换结果数据。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  * 返回  常规通道转换结果数据，16位有效数据。
  */
uint16_t
ADC_Get_Conversion_Value (ADC_SFRmap* ADCx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));

    /*---------------- 配置ADC_DATA寄存器DATA位 ----------------*/
    tmpreg = ADCx->DATA;

    return (uint16_t) tmpreg;
}
/**
  *   ##### 模数转换模块(ADC)常规通道功能配置函数定义结束 #####
  */


/**
  *   ##### 模数转换模块(ADC)高优先级通道功能配置函数 #####
  */
/**
  * 描述  配置高优先级通道上看门狗使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 高优先级通道上看门狗使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_High_Priority_Watchdog_Enable (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL0寄存器HPWDEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能高优先级通道上模拟看门狗 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_HPWDEN_POS);
    }
    else
    {
        /* 禁止高优先级通道上模拟看门狗 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_HPWDEN_POS);
    }
}

/**
  * 描述  配置高优先级通道外部触发转换模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 高优先级通道外部触发转换模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_HPExternal_Trig_Conv_Enable (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ADC_CTL1寄存器HPEXTEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能外部事件触发转换 */
        SFR_SET_BIT_ASM(ADCx->CTL1, ADC_CTL1_HPEXTEN_POS);
    }
    else
    {
        /* 禁止外部事件触发转换 */
        SFR_CLR_BIT_ASM(ADCx->CTL1, ADC_CTL1_HPEXTEN_POS);
    }
}

/**
  * 描述  配置高优先级通道扫描序列。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       Channel: ADC通道选择，取值范围为:
  *                  ADC_CHANNEL_0: 通道0--- ADC_CHANNEL_63: 通道63
  *       Rank: 高优先级通道扫描AD输入的转换位置，取值为1~4。
  * 返回  无。
  */
void
ADC_High_Priority_Channel_Config (ADC_SFRmap* ADCx,
                    uint32_t Channel, uint32_t Rank)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_HP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ADC_HP_RANK(Rank));

    /* 配置第1次转换~第4次转换 */

    /*------------- 设置ADCx_HSCANSQ寄存器HSSQx位 -------------*/
    tmpreg = (uint32_t)Channel << (6 * (Rank - 1));
    tmpmask = ADC_HSCANSQ_SSQ1 << (6 * (Rank - 1));
    ADCx->HSCANSQ = SFR_Config (ADCx->HSCANSQ,
                          ~tmpmask,
                          tmpreg);
}

/**
  * 描述  配置高优先级通道扫描长度。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       Length: 高优先级通道扫描长度，取值范围为1~4。
  * 返回  无。
  */
void
ADC_High_Priority_Sequencer_Length_Config (ADC_SFRmap* ADCx, uint32_t Length)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_HIGH_PRIORITY_LENGTH(Length));

    /*------------- 设置ADCx_HSCANSQ寄存器HSLEN位 -------------*/
    tmpreg = (Length - 1) << ADC_HSCANSQ_HSLEN_0_POS;
    ADCx->HSCANSQ = SFR_Config (ADCx->HSCANSQ,
                               ~ADC_HSCANSQ_HSLEN,
                               tmpreg);
}

/**
  * 描述  配置高优先级通道转换结果数据偏移。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       HPDoffChannel: 高优先级通道选择，取值为：
  *                        ADC_HPDOFF_0: 高优先级通道0
  *                        ADC_HPDOFF_1: 高优先级通道1
  *                        ADC_HPDOFF_2: 高优先级通道2
  *                        ADC_HPDOFF_3: 高优先级通道3
  *       Offset: 转换结果数据偏移，取值为12位有效数据。
  * 返回  无。
  */
void
ADC_Set_HPChannel_Conv_Value_Offset (ADC_SFRmap* ADCx,
                    uint32_t HPDoffChannel, uint32_t Offset)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_HPDOFF_OFFSET(HPDoffChannel));
    CHECK_RESTRICTION(CHECK_ADC_OFFSET(Offset));

    /*------------- 设置ADCx_HPDOFFy寄存器HPDOFF位 -------------*/
    /* 获取ADCx_HPDOFFy地址 */
    tmpreg = (uint32_t)ADCx;
    tmpreg += HPDoffChannel;

    /* 设置ADCx_HPDOFFy */
    *(volatile uint32_t *) tmpreg = Offset;
}

/**
  * 描述  配置高优先级通道外部触发事件。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       HPExternalTrigEvent: 高优先级通道外部触发事件选择，取值为：
  *                              ADC_HPEXTERNALTRIG_CCP1_CH1: CCP1 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP1_CH2: CCP1 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP1_CH3: CCP1 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP1_CH4: CCP1 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP2_CH1: CCP2 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP2_CH2: CCP2 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP2_CH3: CCP2 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP2_CH4: CCP2 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP3_CH1: CCP3 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP3_CH2: CCP3 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP3_CH3: CCP3 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP3_CH4: CCP3 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP4_CH1: CCP4 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP4_CH2: CCP4 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP4_CH3: CCP4 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP4_CH4: CCP4 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP18_CH1: CCP18 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP18_CH2: CCP18 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP18_CH3: CCP18 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP18_CH4: CCP18 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP19_CH1: CCP19 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP19_CH2: CCP19 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP19_CH3: CCP19 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP19_CH4: CCP19 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP20_CH1: CCP20 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP20_CH2: CCP20 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP20_CH3: CCP20 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP20_CH4: CCP20 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP21_CH1: CCP21 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP21_CH2: CCP21 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP21_CH3: CCP21 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP21_CH4: CCP21 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP5_CH1: CCP5 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP5_CH2: CCP5 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP5_CH3: CCP5 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP5_CH4: CCP5 通道 4
  *                              ADC_HPEXTERNALTRIG_CCP22_CH1: CCP22 通道 1
  *                              ADC_HPEXTERNALTRIG_CCP22_CH2: CCP22 通道 2
  *                              ADC_HPEXTERNALTRIG_CCP22_CH3: CCP22 通道 3
  *                              ADC_HPEXTERNALTRIG_CCP22_CH4: CCP22 通道 4
  *                              ADC_HPEXTERNALTRIG_T1TRGO: T1TRGO
  *                              ADC_HPEXTERNALTRIG_T2TRGO: T2TRGO
  *                              ADC_HPEXTERNALTRIG_T3TRGO: T3TRGO
  *                              ADC_HPEXTERNALTRIG_T4TRGO: T4TRGO
  *                              ADC_HPEXTERNALTRIG_T18TRGO: T18TRGO
  *                              ADC_HPEXTERNALTRIG_T19TRGO: T19TRGO
  *                              ADC_HPEXTERNALTRIG_T20TRGO: T20TRGO
  *                              ADC_HPEXTERNALTRIG_T21TRGO: T21TRGO
  *                              ADC_HPEXTERNALTRIG_T5TRGO: T5TRGO
  *                              ADC_HPEXTERNALTRIG_T9TRGO: T9TRGO
  *                              ADC_HPEXTERNALTRIG_T14TRGO: T14TRGO
  *                              ADC_HPEXTERNALTRIG_T15TRGO: T15TRGO
  *                              ADC_HPEXTERNALTRIG_T5_OVERFLOW: T5溢出
  *                              ADC_HPEXTERNALTRIG_T6_OVERFLOW: T6溢出
  *                              ADC_HPEXTERNALTRIG_T9_OVERFLOW: T9溢出
  *                              ADC_HPEXTERNALTRIG_T10_OVERFLOW: T10溢出
  *                              ADC_HPEXTERNALTRIG_CCP9_CH1: CCP9通道1
  *                              ADC_HPEXTERNALTRIG_CCP9_CH2: CCP9通道2
  *                              ADC_HPEXTERNALTRIG_CCP9_CH3: CCP9通道3
  *                              ADC_HPEXTERNALTRIG_CCP9_CH4: CCP9通道4
  *                              ADC_HPEXTERNALTRIG_EINT7: EINT7
  *                              ADC_HPEXTERNALTRIG_EINT15: EINT15
  *                              ADC_HPEXTERNALTRIG_CCP0_CH1_CMP: CCP0_CH1
  *                              ADC_HPEXTERNALTRIG_CCP0_CH2_CMP: CCP0_CH2
  *                              ADC_HPEXTERNALTRIG_CCP0_CH3: CCP0_CH3
  *                              ADC_HPEXTERNALTRIG_CCP0_CH4: CCP0_CH4
  *                              ADC_HPEXTERNALTRIG_CCP23_CH1: CCP23通道1
  *                              ADC_HPEXTERNALTRIG_CCP23_CH2: CCP23通道2
  *                              ADC_HPEXTERNALTRIG_CCP23_CH3: CCP23通道3
  *                              ADC_HPEXTERNALTRIG_CCP23_CH4: CCP23通道4
  * 返回  无。
  */
void
ADC_HPExternal_Trig_Conv_Config (ADC_SFRmap* ADCx,
                    uint32_t HPExternalTrigEvent)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_HPEXT_TRIG(HPExternalTrigEvent));

    /*------------- 设置ADCx_CTL1寄存器HCH位 -------------*/
    ADCx->CTL1 = SFR_Config (ADCx->CTL1,
                               ~ADC_CTL1_HCH,
                               HPExternalTrigEvent);
}

/**
  * 描述  软件启动A/D高优先级通道转换。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  * 返回  无。
  */
void
ADC_Software_HPStart_Conv (ADC_SFRmap* ADCx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));

    /*---------------- 配置ADC_CTL1寄存器HPSTART位 ----------------*/
    SFR_SET_BIT_ASM(ADCx->CTL1, ADC_CTL1_HPSTART_POS);
}

/**
  * 描述  配置自动高优先级通道组转换使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 自动高优先级通道组转换使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_HPAuto_Conv_Cmd (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能自动的高优先级通道转换 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_HPAUTO_POS);
    }
    else
    {
        /* 禁止自动的高优先级通道转换 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_HPAUTO_POS);
    }
}

/**
  * 描述  配置高优先级通道上的间隔模式使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       NewState: 高优先级通道上的间隔模式使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_HPDisc_Mode_Cmd (ADC_SFRmap* ADCx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能高优先级通道上的间隔模式 */
        SFR_SET_BIT_ASM(ADCx->CTL0, ADC_CTL0_HPDSEN_POS);
    }
    else
    {
        /* 禁止高优先级通道上的间隔模式 */
        SFR_CLR_BIT_ASM(ADCx->CTL0, ADC_CTL0_HPDSEN_POS);
    }
}

/**
  * 描述  获取高优先级通道转换结果数据。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       HPDataChannel: 高优先级通道选择，取值为：
  *                        ADC_HPDATA_0: 高优先级通道0
  *                        ADC_HPDATA_1: 高优先级通道1
  *                        ADC_HPDATA_2: 高优先级通道2
  *                        ADC_HPDATA_3: 高优先级通道3
  * 返回  高优先级通道转换结果数据，16位有效数据。
  */
uint16_t
ADC_Get_HPConversion_Data (ADC_SFRmap* ADCx, uint8_t HPDataChannel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_HPDATA_OFFSET(HPDataChannel));

    /*------------- 获取ADCx_HPDATAy寄存器HPDATA位 -------------*/
    /* 获取ADCx_HPDATAy地址 */
    tmpreg = (uint32_t)ADCx;
    tmpreg += HPDataChannel;

    /* 获取ADCx_HPDATAy的值 */
    tmpreg = *(volatile uint32_t *)  tmpreg;
    return (uint16_t) tmpreg;
}
/**
  *   ##### 模数转换模块(ADC)高优先级通道功能配置函数定义结束 #####
  */


/**
  *   ##### 模数转换模块(ADC)中断管理函数 #####
  */
/**
  * 描述  配置ADC中断使能。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       InterruptType: ADC中断类型，取值为：
  *                        ADC_INT_EOC: ADC一次转换结束中断
  *                        ADC_INT_AWD: ADC模拟看门狗中断
  *                        ADC_INT_HPEND: ADC高优先级通道转换结束中断
  *                        ADC_INT_END: ADC常规通道转换结束中断
  *       NewState: ADC中断使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
ADC_Set_INT_Enable (ADC_SFRmap* ADCx,
                    uint32_t InterruptType, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));
    CHECK_RESTRICTION(CHECK_ADC_INT(InterruptType));

    /* 获取中断类型掩码 */
    tmpreg = (InterruptType & 0x0F);
    tmpreg = (uint32_t)0x01 << tmpreg;

    if (NewState != FALSE)
    {
        /* 使能中断 */
        ADCx->STATE |= tmpreg;
    }
    else
    {
        /* 禁止中断 */
        ADCx->STATE &= (~tmpreg);
    }
}

/**
  * 描述  获取ADC中断标志。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       InterruptType: ADC中断类型，取值为：
  *                        ADC_INT_EOC: ADC一次转换结束中断
  *                        ADC_INT_AWD: ADC模拟看门狗中断
  *                        ADC_INT_HPEND: ADC高优先级通道转换结束中断
  *                        ADC_INT_END: ADC常规通道转换结束中断
  * 返回  中断标志，1：发生了中断，0：未发生中断。
  */
FlagStatus
ADC_Get_INT_Flag (ADC_SFRmap* ADCx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_INT(InterruptType));

    /*------------- 获取ADCx_STATE寄存器对应中断标志位 -------------*/
    tmpreg = ((InterruptType >> 8)&0x0F);
    tmpreg = (uint32_t)0x01 << tmpreg;
    if ((ADCx->STATE & tmpreg) != RESET)
    {
        /* ADC发生了中断 */
        return SET;
    }
    else
    {
        /* ADC未发生中断 */
        return RESET;
    }
}

/**
  * 描述  清除ADC中断标志。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       InterruptType: ADC中断类型，取值为：
  *                        ADC_INT_EOC: ADC一次转换结束中断
  *                        ADC_INT_AWD: ADC模拟看门狗中断
  *                        ADC_INT_HPEND: ADC高优先级通道转换结束中断
  *                        ADC_INT_END: ADC常规通道转换结束中断
  * 返回  无。
  */
void
ADC_Clear_INT_Flag (ADC_SFRmap* ADCx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpreg2 = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_INT(InterruptType));

    /*------------- 设置ADCx_STATE寄存器对应中断标志清零位 -------------*/
    tmpreg = InterruptType >> 16;
    tmpreg = (uint32_t)0x01 << tmpreg;

    /* 设置中断标志清零位 */
    ADCx->STATE |= tmpreg;

    tmpreg1 = (InterruptType >> 8)&0x0f;
    tmpreg2 = (ADCx->STATE) & ((uint32_t)(0x01 << tmpreg1));
    while(tmpreg2>>tmpreg1);
    /* 清零中断标志清零位 */
    ADCx->STATE &= ~tmpreg;
}

/**
  * 描述  获取ADC中断响应状态。
  * 输入  ADCx: 指向ADC内存结构的指针，取值为ADC0_SFR~ADC2_SFR。
  *       InterruptType: ADC中断类型，取值为：
  *                        ADC_INT_EOC: ADC一次转换结束中断
  *                        ADC_INT_AWD: ADC模拟看门狗中断
  *                        ADC_INT_HPEND: ADC高优先级通道转换结束中断
  *                        ADC_INT_END: ADC常规通道转换结束中断
  * 返回  中断响应状态，1：发生待响应中断，0：未发生中断或未使能。
  */
INTStatus
ADC_Get_INT_Status (ADC_SFRmap* ADCx, uint32_t InterruptType)
{
    uint32_t tmpmask = 0;
    uint32_t enablestatus = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ADC_ALL_PERIPH(ADCx));
    CHECK_RESTRICTION(CHECK_ADC_INT(InterruptType));

    /*------------- 获取ADCx_STATE寄存器 -------------*/
    tmpreg = ADCx->STATE;

    /* 获取中断使能状态 */
    enablestatus = (tmpreg & ((uint32_t)0x01 << InterruptType)) ;

    /* 获取中断标志状态 */
    tmpmask = tmpreg & ((uint32_t)0x01 << ((InterruptType >> 8)));

    /* 检查中断使能和标志 */
    if ((tmpmask != (uint32_t)RESET) && enablestatus)
    {
        /* 发生待响应中断 */
        return SET;
    }
    else
    {
        /* 未发生中断或未使能 */
        return RESET;
    }
}
/**
  *   ##### 模数转换模块(ADC)中断管理函数定义结束 #####
  */
