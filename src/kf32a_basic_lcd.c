/**
  ******************************************************************************
  * 文件名  kf32a_basic_lcd.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了液晶显示(LCD)相关的功能函数，包含：
  *          + 液晶显示(LCD)初始化函数
  *          + 液晶显示(LCD)功能配置函数
  *          + 液晶显示(LCD)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_lcd.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_lcd
/* 定时器私有定义 ----------------------------------------------------*/
/* LCD_CTL寄存器初始化掩码 */
#define LCD_CTL_INIT_MASK               (LCD_CTL_LCDENANALOG \
                                       | LCD_CTL_LMUX \
                                       | LCD_CTL_FR \
                                       | LCD_CTL_VLCDMD \
                                       | LCD_CTL_CS \
                                       | LCD_CTL_LP)

/**
  *   ##### 液晶显示(LCD)初始化函数 #####
  */
/**
  * 描述  复位LCD外设，使能外设时钟。
  * 输入  无。
  * 返回  无。
  */
void
LCD_Reset (void)
{
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_LCDRST, TRUE);
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_LCDRST, FALSE);
    PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_LCDCLKEN, TRUE);
}

/**
  * 描述  LCD液晶显示初始化。
  * 输入  lcdInitStruct: LCD液晶显示配置信息结构体。
  * 返回  无。
  */
void
LCD_Configuration(LCD_InitTypeDef* lcdInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;
    uint32_t tmploc = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(lcdInitStruct->m_SegPin));
    CHECK_RESTRICTION(CHECK_LCD_SEG_FUNCTION(lcdInitStruct->m_SegPinEn));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(lcdInitStruct->m_Analog));
    CHECK_RESTRICTION(CHECK_LCD_SOURCE(lcdInitStruct->m_ClockSource));
    CHECK_RESTRICTION(CHECK_LCD_SOURCE_DIVIDE(lcdInitStruct->m_SourcePrescaler));
    CHECK_RESTRICTION(CHECK_LCD_PRESCALER(lcdInitStruct->m_LCDPrescaler));
    CHECK_RESTRICTION(CHECK_LCD_COMMON_PORT(lcdInitStruct->m_CommonPort));
    CHECK_RESTRICTION(CHECK_LCD_VOLTAGE_SELECT(lcdInitStruct->m_VoltageSelect));

    /*---------------- 设置LCD_CTL寄存器 ----------------*/
    /* 根据结构体成员m_Analog，设置LCDENANALOG位域 */
    /* 根据结构体成员m_CommonPort，设置LMUX位域 */
    /* 根据结构体成员m_SourcePrescaler，设置FR位域 */
    /* 根据结构体成员m_VoltageSelect，设置VLCDMD位域 */
    /* 根据结构体成员m_ClockSource，设置CS位域 */
    /* 根据结构体成员m_LCDPrescaler，设置LP位域 */
    tmpreg = (lcdInitStruct->m_Analog << LCD_CTL_LCDENANALOG_POS) \
           | (lcdInitStruct->m_CommonPort) \
           | (lcdInitStruct->m_SourcePrescaler) \
           | (lcdInitStruct->m_VoltageSelect << LCD_CTL_VLCDMD_POS) \
           | (lcdInitStruct->m_ClockSource) \
           | (lcdInitStruct->m_LCDPrescaler);
    LCD_CTL = SFR_Config (LCD_CTL,
                          ~LCD_CTL_INIT_MASK,
                          tmpreg);

    /*---------------- 设置LCD_SEGx寄存器 ----------------*/
    /* 根据结构体成员m_SegPin和m_SegPinEn，设置SEGx位域 */
    tmpreg = (uint32_t)lcdInitStruct->m_SegPin;
    if (tmpreg != 0)
    {
        if (lcdInitStruct->m_SegPinEn != LCD_SEG_PIN_IO)
        {
            /* 使能引脚的段功能 */
            LCD_SEG0 |= tmpreg;
        }
        else
        {
            /* 使能引脚的I/O功能 */
            LCD_SEG0 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }

    tmpreg = (uint32_t)(lcdInitStruct->m_SegPin >> 32);
    if (tmpreg != 0)
    {
        if (lcdInitStruct->m_SegPinEn != LCD_SEG_PIN_IO)
        {
            /* 使能引脚的段功能 */
            LCD_SEG1 |= tmpreg;
        }
        else
        {
            /* 使能引脚的I/O功能 */
            LCD_SEG1 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }
}

/**
  * 描述  初始化LCD液晶显示配置信息结构体。
  * 输入  lcdInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
LCD_Struct_Init (LCD_InitTypeDef* lcdInitStruct)
{
    /* 初始化 SEG引脚编号掩码选择 */
    lcdInitStruct->m_SegPin = 0;
    /* 初始化 段引脚功能 */
    lcdInitStruct->m_SegPinEn = LCD_SEG_PIN_IO;
    /* 初始化 LCD模拟部分使能 */
    lcdInitStruct->m_Analog = FALSE;
    /* 初始化 LCD时钟源 */
    lcdInitStruct->m_ClockSource = LCD_SOURCE_HALF_SCLK;
    /* 初始化 LCD时钟源分频 */
    lcdInitStruct->m_SourcePrescaler = LCD_SOURCE_DIVIDE_2;
    /* 初始化 LCD预分频比 */
    lcdInitStruct->m_LCDPrescaler = LCD_PRESCALER_1;
    /* 初始化 LCD公共端选择 */
    lcdInitStruct->m_CommonPort = LCD_COMMON_PORT_DIV_8;
    /* 初始化 LCD内部/外部电源选择 */
    lcdInitStruct->m_VoltageSelect = LCD_VOLTAGE_INTERNAL;
}
/**
  *   ##### 液晶显示(LCD)初始化函数定义结束 #####
  */


/**
  *   ##### 液晶显示(LCD)功能配置函数 #####
  */
/**
  * 描述  配置LCD驱动使能。
  * 输入  NewState: LCD驱动使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Cmd_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置LCD_CTL寄存器LCDEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能LCD驱动模块 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_LCDEN_POS);
    }
    else
    {
        /* 禁止LCD驱动模块 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_LCDEN_POS);
    }
}

/**
  * 描述  配置LCD模拟部分使能。
  * 输入  NewState: LCD模拟部分使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Analog_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置LCD_CTL寄存器LCDENANALOG位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能LCD模拟部分 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_LCDENANALOG_POS);
    }
    else
    {
        /* 禁止LCD模拟部分 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_LCDENANALOG_POS);
    }
}

/**
  * 描述  配置内部偏压源电压。
  * 输入  Voltage: 内部偏压源电压，取值范围为:
  *                  LCD_BIAS_VOLTAGE_3P04: 3.04V
  *                  LCD_BIAS_VOLTAGE_2P89: 2.89V
  *                  LCD_BIAS_VOLTAGE_2P76: 2.76V
  *                  LCD_BIAS_VOLTAGE_2P62: 2.62V
  *                  LCD_BIAS_VOLTAGE_3P62: 3.62V
  *                  LCD_BIAS_VOLTAGE_3P46: 3.46V
  *                  LCD_BIAS_VOLTAGE_3P32: 3.32V
  *                  LCD_BIAS_VOLTAGE_3P19: 3.19V
  * 返回  无。
  */
void
LCD_Internal_Bias_Voltage_Config (uint32_t Voltage)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_BIAS_VOLTAGE(Voltage));

    /*---------------- 配置LCD_CTL寄存器TRIMCPVOL位 ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_TRIMCPVOL, Voltage);
}

/**
  * 描述  配置公共端。
  * 输入  CommonPort: 公共端选择，取值范围为:
  *                     LCD_COMMON_PORT_STATIC: 静态（COM0）
  *                     LCD_COMMON_PORT_DIV_2: 1/2 （COM<1:0>）
  *                     LCD_COMMON_PORT_DIV_3: 1/3 （COM<2:0>）
  *                     LCD_COMMON_PORT_DIV_4: 1/4 （COM<3:0>）
  *                     LCD_COMMON_PORT_DIV_8: 1/8 （COM<7:0>）
  * 返回  无。
  */
void
LCD_Common_Port_Config (uint32_t CommonPort)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_COMMON_PORT(CommonPort));

    /*---------------- 配置LCD_CTL寄存器LMUX位 ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_LMUX, CommonPort);
}

/**
  * 描述  配置LCD时钟源分频。
  * 输入  ClkDivider: LCD时钟源分频，取值范围为:
  *                     LCD_SOURCE_DIVIDE_2: 1:2
  *                     LCD_SOURCE_DIVIDE_4: 1:4
  *                     LCD_SOURCE_DIVIDE_8: 1:8
  *                     LCD_SOURCE_DIVIDE_16: 1:16
  *                     LCD_SOURCE_DIVIDE_32: 1:32
  *                     LCD_SOURCE_DIVIDE_64: 1:64
  *                     LCD_SOURCE_DIVIDE_128: 1:128
  *                     LCD_SOURCE_DIVIDE_256: 1:256
  *                     LCD_SOURCE_DIVIDE_512: 1:512
  *                     LCD_SOURCE_DIVIDE_1024: 1:1024
  *                     LCD_SOURCE_DIVIDE_2048: 1:2048
  *                     LCD_SOURCE_DIVIDE_4096: 1:4096
  *                     LCD_SOURCE_DIVIDE_8192: 1:8192
  *                     LCD_SOURCE_DIVIDE_16384: 1:16384
  *                     LCD_SOURCE_DIVIDE_32768: 1:32768
  * 返回  无。
  */
void
LCD_Source_Clock_Divider_Config (uint32_t ClkDivider)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_SOURCE_DIVIDE(ClkDivider));

    /*---------------- 配置LCD_CTL寄存器FR位 ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_FR, ClkDivider);
}

/**
  * 描述  配置LCD分压选择。
  * 输入  VoltageSelect: LCD分压选择，取值范围为:
  *                        LCD_VOLTAGE_INTERNAL: 由内部分压电阻电路提供分压
  *                        LCD_VOLTAGE_EXTERNAL: 由外部提供分压
  * 返回  无。
  */
void
LCD_Voltage_Divider_Config (uint32_t VoltageSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_VOLTAGE_SELECT(VoltageSelect));

    /*---------------- 设置LCD_CTL寄存器VLCDS位 ----------------*/
    if (VoltageSelect != LCD_VOLTAGE_INTERNAL)
    {
        /* 由外部提供分压 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_VLCDS_POS);
    }
    else
    {
        /* 由内部分压电阻电路提供分压 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_VLCDS_POS);
    }
}

/**
  * 描述  配置LCD偏置电压源选择。
  * 输入  VoltageSelect: LCD偏置电压源选择，取值范围为:
  *                        LCD_VOLTAGE_INTERNAL: 内部电源给LCD供电
  *                        LCD_VOLTAGE_EXTERNAL: 外部电源给LCD供电
  * 返回  无。
  */
void
LCD_Bias_Voltage_Source_Config (uint32_t VoltageSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_VOLTAGE_SELECT(VoltageSelect));

    /*---------------- 设置LCD_CTL寄存器VLCDMD位 ----------------*/
    if (VoltageSelect != LCD_VOLTAGE_INTERNAL)
    {
        /* 外部电源给LCD供电 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_VLCDMD_POS);
    }
    else
    {
        /* 内部电源给LCD供电 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_VLCDMD_POS);
    }
}

/**
  * 描述  配置LCD PUMP功能使能。
  * 输入  NewState: LCD PUMP功能使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Pump_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置LCD_CTL寄存器PUMPEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能PUMP */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_PUMPEN_POS);
    }
    else
    {
        /* 不使用PUMP功能 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_PUMPEN_POS);
    }
}

/**
  * 描述  配置时钟源选择。
  * 输入  SourceClock: LCD时钟源选择，取值范围为:
  *                      LCD_SOURCE_HALF_SCLK: SCLK / 2
  *                      LCD_SOURCE_HALF_HFCLK: HFCLK / 2
  *                      LCD_SOURCE_HALF_LFCLK: LFCLK / 2
  *                      LCD_SOURCE_HALF_INTLF: INTLF / 2
  * 返回  无。
  */
void
LCD_Source_Clock_Config (uint32_t SourceClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_SOURCE(SourceClock));

    /*---------------- 配置LCD_CTL寄存器CS位 ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_CS, SourceClock);
}

/**
  * 描述  配置LCD波形类型选择。
  * 输入  WaveType: LCD波形类型选择，取值范围为:
  *                   LCD_WAVE_TYPE_A: A 型波形（在每一公共端类型内改变相位）
  *                   LCD_WAVE_TYPE_B: B 型波形（在每一帧边界改变相位）
  * 返回  无。
  */
void
LCD_Wave_Type_Config (uint32_t WaveType)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_WAVE_TYPE(WaveType));

    /*---------------- 设置LCD_CTL寄存器WFT位 ----------------*/
    if (WaveType != LCD_WAVE_TYPE_A)
    {
        /* B 型波形 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_WFT_POS);
    }
    else
    {
        /* A 型波形 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_WFT_POS);
    }
}

/**
  * 描述  配置LCD偏置模式选择，需要先配置LCD_Common_Port_Config函数。
  * 输入  BiasMode: LCD偏置模式选择，取值范围为:
  *                   LCD_BIAS_MODE_STATIC: 静态偏置模式
  *                   LCD_BIAS_MODE_1_DIV_3: 1/3 偏置模式
  *                   LCD_BIAS_MODE_1_DIV_2: 1/2 偏置模式
  * 返回  无。
  */
void
LCD_Bias_Mode_Config (uint32_t BiasMode)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_BIAS_MODE(BiasMode));

    /* 获取LCD_CTL寄存器LMUX信息 */
    tmpreg = LCD_CTL;
    tmpreg &= LCD_CTL_LMUX;
    switch (tmpreg)
    {
        case LCD_COMMON_PORT_STATIC:
            CHECK_RESTRICTION(LCD_BIAS_MODE_STATIC==BiasMode);
            break;
        case LCD_COMMON_PORT_DIV_4:
        case LCD_COMMON_PORT_DIV_8:
            CHECK_RESTRICTION(LCD_BIAS_MODE_1_DIV_3==BiasMode);
            break;
        default:
            break;
    }

    /*---------------- 设置LCD_CTL寄存器BIASMD位 ----------------*/
    if (BiasMode != LCD_BIAS_MODE_1_DIV_3)
    {
        /* 1/3 偏置模式 或 静态偏置模式 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_BIASMD_POS);
    }
    else
    {
        /* 1/2 偏置模式 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_BIASMD_POS);
    }
}

/**
  * 描述  配置LCD预分频比。
  * 输入  Prescaler: LCD预分频比，取值范围为:
  *                    LCD_PRESCALER_1: 1:1
  *                    LCD_PRESCALER_2: 1:2
  *                    LCD_PRESCALER_3: 1:3
  *                    LCD_PRESCALER_4: 1:4
  *                    LCD_PRESCALER_5: 1:5
  *                    LCD_PRESCALER_6: 1:6
  *                    LCD_PRESCALER_7: 1:7
  *                    LCD_PRESCALER_8: 1:8
  *                    LCD_PRESCALER_9: 1:9
  *                    LCD_PRESCALER_10: 1:10
  *                    LCD_PRESCALER_11: 1:11
  *                    LCD_PRESCALER_12: 1:12
  *                    LCD_PRESCALER_13: 1:13
  *                    LCD_PRESCALER_14: 1:14
  *                    LCD_PRESCALER_15: 1:15
  *                    LCD_PRESCALER_16: 1:16
  * 返回  无。
  */
void
LCD_Prescaler_Config (uint32_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_PRESCALER(Prescaler));

    /*---------------- 配置LCD_CTL寄存器LP位 ----------------*/
    LCD_CTL = SFR_Config (LCD_CTL, ~LCD_CTL_LP, Prescaler);
}

/**
  * 描述  配置LCD闪烁模式点亮时间，点亮时间=(32 * FlickerOn)/帧频率。
  * 输入  FlickerOn: LCD闪烁模式点亮时间，取值为8位有效数值。
  * 返回  无。
  */
void
LCD_Flicker_On_Time_Config (uint8_t FlickerOn)
{
	uint32_t tmpreg = 0;
    /*---------------- 配置LCD_PTL寄存器FON位 ----------------*/
	tmpreg = FlickerOn << LCD_PTL_FON0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_FON, tmpreg);
}

/**
  * 描述  配置LCD闪烁模式点亮时间，点亮时间=(32 * FlickerOff)/帧频率。
  * 输入  FlickerOff: LCD闪烁模式点亮时间，取值为8位有效数值。
  * 返回  无。
  */
void
LCD_Flicker_Off_Time_Config (uint8_t FlickerOff)
{
	uint32_t tmpreg = 0;
    /*---------------- 配置LCD_PTL寄存器FOFF位 ----------------*/
	tmpreg = FlickerOff << LCD_PTL_FOFF0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_FOFF, tmpreg);
}

/**
  * 描述  配置LCD闪烁模式使能。
  * 输入  NewState: LCD闪烁模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Flicker_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置LCD_PTL寄存器FLKEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闪烁功能 */
        SFR_SET_BIT_ASM(LCD_PTL, LCD_PTL_FLKEN_POS);
    }
    else
    {
        /* 禁止闪烁功能 */
        SFR_CLR_BIT_ASM(LCD_PTL, LCD_PTL_FLKEN_POS);
    }
}

/**
  * 描述  配置LCD节能模式使能。
  * 输入  NewState: LCD节能模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Save_Power_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置LCD_PTL寄存器SPEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能节能模式 */
        SFR_SET_BIT_ASM(LCD_PTL, LCD_PTL_SPEN_POS);
    }
    else
    {
        /* 禁止节能模式 */
        SFR_CLR_BIT_ASM(LCD_PTL, LCD_PTL_SPEN_POS);
    }
}

/**
  * 描述  配置LCD时间单位大小。
  * 输入  TimeSlice: LCD时间单位大小，取值范围为：
  *                    LCD_TIME_SLICE_EQU_2_FRAME: 时间单位t等于2帧
  *                    LCD_TIME_SLICE_EQU_4_FRAME: 时间单位t等于4帧
  *                    LCD_TIME_SLICE_EQU_8_FRAME: 时间单位t等于8帧
  *                    LCD_TIME_SLICE_EQU_16_FRAME: 时间单位t等于16帧
  *                    LCD_TIME_SLICE_EQU_32_FRAME: 时间单位t等于32帧
  *                    LCD_TIME_SLICE_EQU_64_FRAME: 时间单位t等于64帧
  *                    LCD_TIME_SLICE_EQU_128_FRAME: 时间单位t等于128帧
  *                    LCD_TIME_SLICE_EQU_256_FRAME: 时间单位t等于256帧
  * 返回  无。
  */
void
LCD_Time_Slice_Config (uint32_t TimeSlice)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_TIME_SLICE(TimeSlice));

    /*---------------- 配置LCD_PTL寄存器TS位 ----------------*/
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_TS, TimeSlice);
}

/**
  * 描述  配置工作时间和关闭时间比例。
  * 输入  WorkTime: 工作时间和关闭时间比例，取值范围为：
  *                   LCD_WORK_VS_STOP_8_8: LCD工作时间和关闭时间的比例为8:8
  *                   LCD_WORK_VS_STOP_7_9: LCD工作时间和关闭时间的比例为7:9
  *                   LCD_WORK_VS_STOP_6_10: LCD工作时间和关闭时间的比例为6:10
  *                   LCD_WORK_VS_STOP_5_10: LCD工作时间和关闭时间的比例为5:11
  *                   LCD_WORK_VS_STOP_4_12: LCD工作时间和关闭时间的比例为4:12
  *                   LCD_WORK_VS_STOP_3_13: LCD工作时间和关闭时间的比例为3:13
  *                   LCD_WORK_VS_STOP_2_14: LCD工作时间和关闭时间的比例为2:14
  *                   LCD_WORK_VS_STOP_1_15: LCD工作时间和关闭时间的比例为1:15
  * 返回  无。
  */
void
LCD_Work_Time_Config (uint32_t WorkTime)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_WORK_VS_STOP(WorkTime));

    /*---------------- 配置LCD_PTL寄存器IF位 ----------------*/
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_IF, WorkTime);
}

/**
  * 描述  配置A功耗模式控制。
  * 输入  PowerCtl: 功耗模式控制，取值范围为：
  *                   LCD_HIGH_RESISTANCE_NO_BUF: 使用高电阻且不采用BUF
  *                   LCD_LOW_RESISTANCE_NO_BUF: 使用低电阻且不采用BUF
  *                   LCD_HIGH_RESISTANCE_WITH_BUF: 使用高电阻且采用BUF
  *                   LCD_LOW_RESISTANCE_WITH_BUF: 使用低电阻且采用BUF
  * 返回  无。
  */
void
LCD_A_Power_Control_Config (uint32_t PowerCtl)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_POWER_CONTROL(PowerCtl));

    /*---------------- 配置LCD_PTL寄存器APC位 ----------------*/
    tmpreg = PowerCtl << LCD_PTL_APC0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_APC, tmpreg);
}

/**
  * 描述  配置B功耗模式控制。
  * 输入  PowerCtl: 功耗模式控制，取值范围为：
  *                   LCD_HIGH_RESISTANCE_NO_BUF: 使用高电阻且不采用BUF
  *                   LCD_LOW_RESISTANCE_NO_BUF: 使用低电阻且不采用BUF
  *                   LCD_HIGH_RESISTANCE_WITH_BUF: 使用高电阻且采用BUF
  *                   LCD_LOW_RESISTANCE_WITH_BUF: 使用低电阻且采用BUF
  * 返回  无。
  */
void
LCD_B_Power_Control_Config (uint32_t PowerCtl)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_POWER_CONTROL(PowerCtl));

    /*---------------- 配置LCD_PTL寄存器BPC位 ----------------*/
    tmpreg = PowerCtl << LCD_PTL_BPC0_POS;
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_BPC, tmpreg);
}

/**
  * 描述  配置时间间隔长度控制。
  * 输入  PowerMode: 时间间隔长度控制，取值范围为：
  *                    LCD_POWER_B_MODE: 功耗模式总是为B功耗模式
  *                    LCD_POWER_A_VS_B_IS_1_15: 间隔A与间隔B的时间长度比值为1:15
  *                    LCD_POWER_A_VS_B_IS_2_14: 间隔A与间隔B的时间长度比值为2:14
  *                    LCD_POWER_A_VS_B_IS_3_13: 间隔A与间隔B的时间长度比值为3:13
  *                    LCD_POWER_A_VS_B_IS_4_12: 间隔A与间隔B的时间长度比值为4:12
  *                    LCD_POWER_A_VS_B_IS_5_11: 间隔A与间隔B的时间长度比值为5:11
  *                    LCD_POWER_A_VS_B_IS_6_10: 间隔A与间隔B的时间长度比值为6:10
  *                    LCD_POWER_A_VS_B_IS_7_9: 间隔A与间隔B的时间长度比值为7:9
  * 返回  无。
  */
void
LCD_Power_Mode_Config (uint32_t PowerMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_POWER_MODE(PowerMode));

    /*---------------- 配置LCD_PTL寄存器ATIC位 ----------------*/
    LCD_PTL = SFR_Config (LCD_PTL, ~LCD_PTL_ATIC, PowerMode);
}

/**
  * 描述  配置LCD段使能。
  * 输入  SegPin: 段引脚编号掩码，取值为LCD_SEG_PIN_0~LCD_SEG_PIN_47中的一个或组合。
  *       SegFunction: 段引脚功能配置，取值为：
  *                      LCD_SEG_PIN_IO: 使能引脚的I/O功能
  *                      LCD_SEG_PIN_SEG: 使能引脚的段功能
  * 返回  无。
  */
void
LCD_Seg_Pin_Function_Config (uint64_t SegPin, uint32_t SegFunction)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(SegPin));
    CHECK_RESTRICTION(CHECK_LCD_SEG_FUNCTION(SegFunction));

    tmpreg = (uint32_t)SegPin;
    if (tmpreg != 0)
    {
        /*---------------- 配置LCD_SEG0寄存器SEGx位 ----------------*/
        if (SegFunction != LCD_SEG_PIN_IO)
        {
            /* 使能引脚的段功能 */
            LCD_SEG0 |= tmpreg;
        }
        else
        {
            /* 使能引脚的I/O功能 */
            LCD_SEG0 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }

    tmpreg = (uint32_t)(SegPin >> 32);
    if (tmpreg != 0)
    {
        /*---------------- 配置LCD_SEG1寄存器SEGx位 ----------------*/
        if (SegFunction != LCD_SEG_PIN_IO)
        {
            /* 使能引脚的段功能 */
            LCD_SEG1 |= tmpreg;
        }
        else
        {
            /* 使能引脚的I/O功能 */
            LCD_SEG1 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }
}

/**
  * 描述  配置LCD数据。
  * 输入  ComPin: COM引脚编号，取值为LCD_PIN_SOURCE_COM0~LCD_PIN_SOURCE_COM7中的一个。
  *       SegPin: 取值为LCD_SEG_PIN_0~LCD_SEG_PIN_47中的一个或组合。
  *       Pixel: 像素点亮状态，取值为：
  *                      LCD_PIXEL_TRANSPARENCY: 不点亮像素（透明）
  *                      LCD_PIXEL_LIGHT: 点亮像素（不透明）
  * 返回  无。
  */
void
LCD_Data_Pixel_Config (uint32_t ComPin, uint64_t SegPin, uint32_t Pixel)
{
    uint32_t tmpmask = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_PIN_SOURCE_COM(ComPin));
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(SegPin));
    CHECK_RESTRICTION(CHECK_LCD_PIXEL(Pixel));

    /* 配置SEG0~SEG31引脚 */
    tmpmask = (uint32_t)SegPin;
    if (tmpmask != 0)
    {
        /*---------------- 配置LCD_DATAx寄存器COMxSEGx位 ----------------*/
        if (Pixel != LCD_PIXEL_TRANSPARENCY)
        {
            /* 点亮像素（不透明） */
            LCD_SFR->DATA[ComPin] |= tmpmask;
        }
        else
        {
            /* 不点亮像素（透明） */
            LCD_SFR->DATA[ComPin] &= ~tmpmask;
        }
    }
    else
    {
        ;
    }

    /* 配置SEG32~SEG39引脚 */
    tmpmask = (uint32_t)(SegPin >> 32);
    tmpmask &= 0xFF;
    if (tmpmask != 0)
    {
        /*---------------- 配置LCD_DATAx寄存器COMxSEGx位 ----------------*/
        tmpreg = (ComPin & 0x3) * 8;
        if (Pixel != LCD_PIXEL_TRANSPARENCY)
        {
            /* 点亮像素（不透明） */
            LCD_SFR->DATA[(ComPin >> 2) + 8] |= tmpmask << tmpreg;
        }
        else
        {
            /* 不点亮像素（透明） */
            LCD_SFR->DATA[(ComPin >> 2) + 8] &= ~(tmpmask << tmpreg);
        }
    }
    else
    {
        ;
    }

    /* 配置SEG40~SEG47引脚 */
    tmpmask = (uint32_t)(SegPin >> (32 + 8));
    if (tmpmask != 0)
    {
        /*---------------- 配置LCD_DATAx寄存器COMxSEGx位 ----------------*/
        tmpreg = (ComPin & 0x3) * 8;
        if (Pixel != LCD_PIXEL_TRANSPARENCY)
        {
            /* 点亮像素（不透明） */
            LCD_SFR->DATA[(ComPin >> 2) + 10] |= tmpmask << tmpreg;
        }
        else
        {
            /* 不点亮像素（透明） */
            LCD_SFR->DATA[(ComPin >> 2) + 10] &= ~(tmpmask << tmpreg);
        }
    }
    else
    {
        ;
    }
}
/**
  * 描述 : LCD写入寄存器DATAx。
  * 输入 : LCD_Datax_Sel：0-7(COM0-7)
  * 	   LCD_Datax_Data：要更改的数据
  * 	   LCD_Datax_Loc：要写入的位置
  * 返回 ：参数异常返回0，正确无返回
  * 效果 ：用户将显示LCD的数据，写入指定的数据寄存器。提供入口。
  */
uint16_t LCD_SET_Datareg (uint8_t LCD_Datax_Sel,uint64_t LCD_Datax_Data,uint64_t LCD_Datax_Loc)
{
	uint32_t masktmp;//掩码
	/*----------------检查是否为Data0-7 ----------------*/
	CHECK_RESTRICTION(CHECK_LCD_COM_SEL(LCD_Datax_Sel));
	/*----------------检查用户输入的64位数，高16位是否为0----------------*/
	if(((LCD_Datax_Data>>48) != 0) | ((LCD_Datax_Loc>>48) != 0))
	{
		return 0;
	}
	/*----------------写入LCD_DATA寄存器DATA位 ----------------
	 * com0-7的低32位写入0-7
	 * com0-3的第32-39位写入com8
	 * com4-7的第32-39位写入com9
	 * com0-3的第40-47位写入com10
	 * com4-7的第40-47位写入com11
	 * 详情参考LCD数据寄存器
	 * */
	if(LCD_Datax_Sel==0)
	{
		LCD_DATA0=(LCD_DATA0 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//0的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~masktmp) | ((LCD_Datax_Data >>32) & masktmp );//8的0-7
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10=(LCD_DATA10 & ~masktmp) | ((LCD_Datax_Data >>40) & masktmp );//10的0-7
		}
	}
	if(LCD_Datax_Sel==1)
	{
		LCD_DATA1=(LCD_DATA1 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//1的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~(masktmp<<8)) | ((LCD_Datax_Data>>32 & masktmp )<<8);//8的8-15
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10 = (LCD_DATA10 & ~(masktmp<<8)) | ((LCD_Datax_Data>>40 & masktmp )<<8);//10的8-15
		}
	}
	if(LCD_Datax_Sel==2)
	{
		LCD_DATA2=(LCD_DATA2 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//2的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~(masktmp<<16)) | ((LCD_Datax_Data>>32 & masktmp )<<16);//8的16-23
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10 = (LCD_DATA10 & ~(masktmp<<16)) | ((LCD_Datax_Data>>40 & masktmp )<<16);//10的16-23
		}
	}
	if(LCD_Datax_Sel==3)
	{
		LCD_DATA3=(LCD_DATA3 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//3的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA8 = (LCD_DATA8 & ~(masktmp<<24)) | ((LCD_Datax_Data>>32 & masktmp )<<24);//8的16-23//8的24-31
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA10 = (LCD_DATA10 & ~(masktmp<<24)) | ((LCD_Datax_Data>>40 & masktmp )<<24);//10的24-31
		}
	}
	if(LCD_Datax_Sel==4)
	{
		LCD_DATA4=(LCD_DATA4 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//4的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~masktmp) | ((LCD_Datax_Data >>32) & masktmp );//9的0-7
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11=(LCD_DATA11 & ~masktmp) | ((LCD_Datax_Data >>40) & masktmp );//11的0-7
		}
	}
	if(LCD_Datax_Sel==5)
	{
		LCD_DATA5=(LCD_DATA5 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//5的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~(masktmp<<8)) | ((LCD_Datax_Data>>32 & masktmp )<<8);//9的8-15
		}
	if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11 = (LCD_DATA11 & ~(masktmp<<8)) | ((LCD_Datax_Data>>40 & masktmp )<<8);//11的8-15
		}
	}
	if(LCD_Datax_Sel==6)
	{
		LCD_DATA6=(LCD_DATA6 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//6的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~(masktmp<<16)) | ((LCD_Datax_Data>>32 & masktmp )<<16);//9的16-23
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11 = (LCD_DATA11 & ~(masktmp<<16)) | ((LCD_Datax_Data>>40 & masktmp )<<16);//11的16-23
		}
	}
	if(LCD_Datax_Sel==7)
	{
		LCD_DATA7=(LCD_DATA7 & ~LCD_Datax_Loc) | (LCD_Datax_Data & LCD_Datax_Loc);//7的0-31
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<32)//如果第16-23位需要更改
		{
			masktmp= LCD_Datax_Loc<<24>>56;
			LCD_DATA9 = (LCD_DATA9 & ~(masktmp<<24)) | ((LCD_Datax_Data>>32 & masktmp )<<24);//9的16-23//8的24-31
		}
		if(LCD_Datax_Loc & (uint64_t)(0xFF)<<40)//如果第24-31位需要更改
		{
			masktmp = LCD_Datax_Loc<<16>>56;
			LCD_DATA11 = (LCD_DATA11 & ~(masktmp<<24)) | ((LCD_Datax_Data>>40 & masktmp )<<24);//11的24-31
		}
	}
}
/**
  * 描述  配置LCD SEG通道选择。
  * 输入  SegSel: SEG通道编号，取值为LCD_SEG_PIN_0~LCD_SEG_PIN_47中的一个或组合。
  *       NewState: SEG通道选择状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Seg_Channel_Select_Config (uint64_t SegSel, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_SEG_PIN(SegSel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    tmpreg = (uint32_t)SegSel;
    if (tmpreg != 0)
    {
        /*---------------- 配置LCD_SEL0寄存器SEGSELx位 ----------------*/
        if (NewState != FALSE)
        {
            /* 选择对应通道 */
            LCD_SEL0 |= tmpreg;
        }
        else
        {
            /* 不选择对应通道 */
            LCD_SEL0 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }

    tmpreg = (uint32_t)(SegSel >> 32);
    if (tmpreg != 0)
    {
        /*---------------- 配置LCD_SEL1寄存器SEGSELx位 ----------------*/
        if (NewState != FALSE)
        {
            /* 选择对应通道 */
            LCD_SEL1 |= tmpreg;
        }
        else
        {
            /* 不选择对应通道 */
            LCD_SEL1 &= ~tmpreg;
        }
    }
    else
    {
        ;
    }
}

/**
  * 描述  配置LCD COM通道选择。
  * 输入  ComPin: COM通道编号，取值为LCD_COM_PIN_0~LCD_COM_PIN_7中的一个或组合。
  *       NewState: COM通道选择状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Com_Channel_Select_Config (uint32_t ComPin,FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_COM_PIN(ComPin));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_SEL1寄存器COMxSELy位 ----------------*/
    tmpreg = (ComPin<<LCD_SEL_COM0_POS) ;
    if (NewState != FALSE)
    {
        /* 选择对应通道 */
        LCD_SEL1 |= tmpreg;
    }
    else
    {
        /* 不选择对应通道 */
        LCD_SEL1 &= ~tmpreg;
    }
}

/**
  * 描述  配置LCD通道选择。
  * 输入  IOConfig: IO口选择，取值为:
  *                   LCD_IO_NONE: 不配置；
  *                   LCD_SEG0_CONFIGURE_PB4
  *                   LCD_SEG0_CONFIGURE_PD13
  *                   LCD_SEG1_CONFIGURE_PB5
  *                   LCD_SEG1_CONFIGURE_PF5
  *                   LCD_SEG2_CONFIGURE_PB6
  *                   LCD_SEG2_CONFIGURE_PC13
  *                   LCD_SEG3_CONFIGURE_PB7
  *                   LCD_SEG3_CONFIGURE_PH15
  *                   LCD_SEG10_CONFIGURE_PC0
  *                   LCD_SEG10_CONFIGURE_PH12
  *                   LCD_SEG11_CONFIGURE_PC1
  *                   LCD_SEG11_CONFIGURE_PH13
  *                   LCD_SEG12_CONFIGURE_PC2
  *                   LCD_SEG12_CONFIGURE_PH11
  *
  *                   LCD_COM0_CONFIGURE_PB0
  *                   LCD_COM0_CONFIGURE_PB14
  *                   LCD_COM1_CONFIGURE_PB1
  *                   LCD_COM1_CONFIGURE_PB15
  *                   LCD_COM2_CONFIGURE_PB2
  *                   LCD_COM2_CONFIGURE_PF0
  *                   LCD_COM4_CONFIGURE_PD13
  *                   LCD_COM4_CONFIGURE_PB4
  *                   LCD_COM5_CONFIGURE_PF5
  *                   LCD_COM5_CONFIGURE_PB5
  *                   LCD_COM6_CONFIGURE_PC13
  *                   LCD_COM6_CONFIGURE_PB6
  *                   LCD_COM7_CONFIGURE_PH15
  *                   LCD_COM7_CONFIGURE_PB7
  *
  *
  * 返回  无。
  */
void
LCD_Channel_IO_Config (uint32_t IOConfig)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_IO(IOConfig));

    /* 不配置IO口，则无需操作。 */
    if (LCD_IO_NONE == IOConfig)
    {
        return;
    }

    /*---------------- 设置LCD_SELx寄存器 ----------------*/
    tmpreg = IOConfig >> 4;
    if (tmpreg < LCD_PIN_SOURCE_SEG32)
    {
        /* 配置SEG0~SEG31通道 */
        tmpmask = IOConfig & 0xF;
        tmpmask <<= tmpreg;
        /* SEGSEL0~SEGSEL31的配置 */
        if (0 != tmpmask)
        {
            LCD_SEL0 |= tmpmask;
        }
        else
        {
            LCD_SEL0 &= ~tmpmask;
        }
    }
    else if (tmpreg < LCD_PIN_SOURCE_SEG47)
    {
        /* 配置SEG32~SEG47通道 */
        tmpreg -= LCD_PIN_SOURCE_SEG32;
        tmpmask = IOConfig & 0xF;
        tmpmask <<= tmpreg;
        /* SEGSEL32~SEGSEL47的配置 */
        if (0 != tmpmask)
        {
            LCD_SEL1 |= tmpmask;
        }
        else
        {
            LCD_SEL1 &= ~tmpmask;
        }
    }
    else
    {
        /* 配置COM0SELy~COM7SELy通道 */
        tmpreg -= LCD_PIN_SOURCE_SEG32;
        tmpmask = IOConfig & 0xF;
        tmpmask <<= tmpreg;
        /* COM0~COM7的配置 */
        if (0 != tmpmask)
        {
            LCD_SEL1 |= tmpmask;
        }
        else
        {
            LCD_SEL1 &= ~tmpmask;
        }
    }
}


/**
  * 描述  获取LCD当前工作状态。
  * 输入  无。
  * 返回  1: LCD驱动模块工作，0:LCD驱动器模块不工作。
  */
FlagStatus
LCD_Get_Work_Status (void)
{
    /*---------------- 读取LCD_CTL寄存器LCDA位 ----------------*/
    if (LCD_CTL & LCD_CTL_LCDA)
    {
        /* LCD驱动模块工作 */
        return SET;
    }
    else
    {
        /* LCD驱动器模块不工作 */
        return RESET;
    }
}

/**
  * 描述  获取LCD闪烁模式状态。
  * 输入  无。
  * 返回  1:LCD状态为点亮，0:LCD状态为不亮。
  */
FlagStatus
LCD_Get_Flicker_Status (void)
{
    /*---------------- 读取LCD_PTL寄存器FLKSTU位 ----------------*/
    if (LCD_PTL & LCD_PTL_FLKSTU)
    {
        /* LCD状态为点亮 */
        return SET;
    }
    else
    {
        /* LCD状态为不亮 */
        return RESET;
    }
}
/**
  *   ##### 液晶显示(LCD)功能配置函数定义结束 #####
  */

/**
  * 描述  清除对应DATA寄存器
  * 输入  ComPin: COM引脚编号，取值为LCD_PIN_SOURCE_COM0~LCD_PIN_SOURCE_COM7中的一个。
  * 返回  无。
  */
void 
LCD_Clear_DATA_Registers(uint32_t ComPin)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_COM_PIN(ComPin));
    /*--------------- 清除0~31位 ----------------*/
    LCD_SFR->DATA[ComPin] = 0;
    /*--------------- 清除32~39位 ----------------*/
    if(ComPin < LCD_PIN_SOURCE_COM4)
    {
        LCD_SFR->DATA[8] &= ~((uint32_t)0xff<<(8*ComPin));
    }
    else 
    {
        LCD_SFR->DATA[9] &= ~((uint32_t)0xff<<(8*ComPin));
    }
    /*--------------- 清除40~47位 ----------------*/
    if(ComPin < LCD_PIN_SOURCE_COM4)
    {
        LCD_SFR->DATA[10] &= ~((uint32_t)0xff<<(8*ComPin));
    }
    else 
    {
        LCD_SFR->DATA[11] &= ~((uint32_t)0xff<<(8*ComPin));
    }
}

/**
  * 描述  清除所有DATA寄存器
  * 输入  无。
  * 返回  无。
  */
void 
LCD_Clear_ALLDATA_Registers(void)
{
    uint8_t i;
    for(i=0;i<12;i=0)
    {
        LCD_SFR->DATA[i] = 0;
    }
}

/**
  *   ##### 液晶显示(LCD)中断管理函数 #####
  */
/**
  * 描述  配置更新事件请求。
  * 输入  NewState: 更新事件请求状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Updata_All_Registers_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器UDR位 ----------------*/
    if (NewState != FALSE)
    {
        /* 更新所有LCD_DATA寄存器 */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDR_POS);
    }
    else
    {
        /* 无动作 */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDR_POS);
    }
}

/**
  * 描述  配置帧死区插入使能。
  * 输入  NewState: 帧死区插入使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Insert_Dead_Time_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器LCDDEAD位 ----------------*/
    if (NewState != FALSE)
    {
        /* 在下一帧开始插入一帧死区 */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_LCDDEAD_POS);
    }
    else
    {
        /* 不插入死区 */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_LCDDEAD_POS);
    }
}
/**
  * 描述 LCD与IO 接口使能。
  * 输入  NewState: LCD 与 IO 的连接使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_IO_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_CTL寄存器IOEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 允许 LCD 与 IO 的连接 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_IOEN_POS);
    }
    else
    {
        /* 禁止 LCD 与 IO 的连接 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_IOEN_POS);
    }
}
/**
  * 描述 VLCDPEN： 接口使能。
  * 输入  NewState: VLCDPEN： 接口使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_VLCDIO_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_CTL寄存器VLCDPEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 允许 VLCDIO 接口使能 */
        SFR_SET_BIT_ASM(LCD_CTL, LCD_CTL_VLCDPEN_POS);
    }
    else
    {
        /* 禁止 VLCDIO 接口使能 */
        SFR_CLR_BIT_ASM(LCD_CTL, LCD_CTL_VLCDPEN_POS);
    }
}
/**
  * 描述  配置LCD中断使能。
  * 输入  InterruptType: LCD中断事件，取值为:
  *                        LCD_INT_FRAME: 帧中断事件
  *                        LCD_INT_FLICKER_OFF: 点亮中断事件
  *                        LCD_INT_FLICKER_ON: 熄灭中断事件
  *                        LCD_INT_UPDATA: 更新中断事件
  *       NewState: 更新事件中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器对应位 ----------------*/
    tmpreg = InterruptType & 0xF;
    tmpreg = 0x1 << tmpreg;
    if (NewState != FALSE)
    {
        /* 使能中断 */
        LCD_INTCTL |= tmpreg;
    }
    else
    {
        /* 禁止中断 */
        LCD_INTCTL &= ~tmpreg;
    }
}

/**
  * 描述  清除LCD中断标志。
  * 输入  InterruptType: LCD中断事件，取值为:
  *                        LCD_INT_FRAME: 帧中断事件
  *                        LCD_INT_FLICKER_OFF: 点亮中断事件
  *                        LCD_INT_FLICKER_ON: 熄灭中断事件
  *                        LCD_INT_UPDATA: 更新中断事件
  * 返回  无。
  */
void
LCD_Clear_INT_Flag (uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_INT_EVENT(InterruptType));

    /*---------------- 清除LCD_INTCTL寄存器对应位 ----------------*/
    tmpreg = InterruptType >> 4;
    tmpreg = 0x1 << tmpreg;
    while(LCD_INTCTL & tmpreg)
    {
    	LCD_INTCTL &= ~tmpreg;
    }
}

/**
  * 描述  获取LCD中断标志。
  * 输入  InterruptType: LCD中断事件，取值为:
  *                        LCD_INT_FRAME: 帧中断事件
  *                        LCD_INT_FLICKER_OFF: 点亮中断事件
  *                        LCD_INT_FLICKER_ON: 熄灭中断事件
  *                        LCD_INT_UPDATA: 更新中断事件
  * 返回  1:发生相应中断标志，0:未发生相应中断标志。
  */
FlagStatus
LCD_Get_INT_Flag (uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_LCD_INT_EVENT(InterruptType));

    /*---------------- 读取LCD_INTCTL寄存器对应位 ----------------*/
    tmpreg = InterruptType >> 4;
    tmpreg = 0x1 << tmpreg;
    if (LCD_INTCTL & tmpreg)
    {
        /* 发生相应中断标志 */
        return SET;
    }
    else
    {
        /* 未发生相应中断标志 */
        return RESET;
    }
}

/**
  * 描述  配置更新事件中断使能。
  * 输入  NewState: 更新事件中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Updata_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器UDDIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能更新事件中断 */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDDIE_POS);
    }
    else
    {
        /* 禁止更新事件中断 */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDDIE_POS);
    }
}

/**
  * 描述  配置点亮中断使能。
  * 输入  NewState: 点亮中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Flicker_On_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器FCKONIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能点亮中断 */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKONIE_POS);
    }
    else
    {
        /* 禁止点亮中断 */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKONIE_POS);
    }
}

/**
  * 描述  配置熄灭中断使能。
  * 输入  NewState: 熄灭中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Flicker_Off_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器FCKOFFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能熄灭中断 */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKOFFIE_POS);
    }
    else
    {
        /* 禁止熄灭中断 */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKOFFIE_POS);
    }
}

/**
  * 描述  配置帧中断使能。
  * 输入  NewState: 帧中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
LCD_Frame_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置LCD_INTCTL寄存器FRAMIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能帧中断 */
        SFR_SET_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FRAMIE_POS);
    }
    else
    {
        /* 禁止帧中断 */
        SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FRAMIE_POS);
    }
}

/**
  * 描述  获取更新事件中断标志。
  * 输入  无。
  * 返回  1:发生更新事件，0:未发生更新事件。
  */
FlagStatus
LCD_Get_Updata_INT_Flag (void)
{
    /*---------------- 读取LCD_INTCTL寄存器UDDIF位 ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_UDDIF)
    {
        /* 发生更新事件 */
        return SET;
    }
    else
    {
        /* 未发生更新事件 */
        return RESET;
    }
}

/**
  * 描述  获取点亮中断标志。
  * 输入  无。
  * 返回  1:检测到点亮中断，0:未检测点亮帧中断。
  */
FlagStatus
LCD_Get_Flicker_On_INT_Flag (void)
{
    /*---------------- 读取LCD_INTCTL寄存器FCKONIF位 ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_FCKONIF)
    {
        /* 检测到点亮中断 */
        return SET;
    }
    else
    {
        /* 未检测点亮帧中断 */
        return RESET;
    }
}

/**
  * 描述  获取熄灭中断标志。
  * 输入  无。
  * 返回  1:检测到熄灭中断，0:未检测熄灭帧中断。
  */
FlagStatus
LCD_Get_Flicker_Off_INT_Flag (void)
{
    /*---------------- 读取LCD_INTCTL寄存器FCKOFFIF位 ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_FCKOFFIF)
    {
        /* 检测到熄灭中断 */
        return SET;
    }
    else
    {
        /* 未检测熄灭帧中断 */
        return RESET;
    }
}

/**
  * 描述  获取帧中断标志。
  * 输入  无。
  * 返回  1:检测到帧中断，0:未检测到帧中断。
  */
FlagStatus
LCD_Get_Frame_INT_Flag (void)
{
    /*---------------- 读取LCD_INTCTL寄存器FRAMIF位 ----------------*/
    if (LCD_INTCTL & LCD_INTCTL_FRAMIF)
    {
        /* 检测到帧中断 */
        return SET;
    }
    else
    {
        /* 未检测到帧中断 */
        return RESET;
    }
}

/**
  * 描述  清除LCD更新事件中断标志。
  * 输入  无。
  * 返回  无。
  */
void
LCD_Clear_Updata_INT_Flag (void)
{
    /*-------------------- 设置LCD_INTCTL寄存器UDDIF位 --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_UDDIF)>>LCD_INTCTL_UDDIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_UDDIF_POS);
	}
}

/**
  * 描述  清除LCD点亮中断标志。
  * 输入  无。
  * 返回  无。
  */
void
LCD_Clear_Flicker_On_INT_Flag (void)
{
    /*-------------------- 设置LCD_INTCTL寄存器FCKONIF位 --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_FCKONIF)>>LCD_INTCTL_FCKONIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKONIF_POS);
	}
}

/**
  * 描述  清除LCD熄灭中断标志。
  * 输入  无。
  * 返回  无。
  */
void
LCD_Clear_Flicker_Off_INT_Flag (void)
{
    /*-------------------- 设置LCD_INTCTL寄存器FCKOFFIF位 --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_FCKOFFIF)>>LCD_INTCTL_FCKOFFIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FCKOFFIF_POS);
	}
}

/**
  * 描述  清除LCD帧中断标志。
  * 输入  无。
  * 返回  无。
  */
void
LCD_Clear_Frame_INT_Flag (void)
{
    /*-------------------- 设置LCD_INTCTL寄存器FRAMIF位 --------------------*/
	while((LCD_INTCTL & LCD_INTCTL_FRAMIF)>>LCD_INTCTL_FRAMIF_POS)
	{
		SFR_CLR_BIT_ASM(LCD_INTCTL, LCD_INTCTL_FRAMIF_POS);
	}
}

#endif  //KF32A_Periph_lcd
/**
  *   ##### 液晶显示(LCD)中断管理函数定义结束 #####
  */
