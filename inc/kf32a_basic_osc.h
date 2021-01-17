/**
  ******************************************************************************
  * 文件名  _kf32a_basic_osc.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了振荡器(OSC)相关的库函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_OSC_H_
#define _KF32A_BASIC_OSC_H_

#include "KF32A_BASIC.h"


/*** 振荡器(OSC)相关定义  */
/**
  * 描述  振荡器(OSC) SCLK主时钟配置信息结构体
  */
typedef struct
{
    uint32_t m_InputSource;     /* 振荡源选择，根据不同的时钟，取值不同，
                                   取值满足“SCLK振荡源选择”、“HFCK振荡源选择”、“LFCK振荡源选择”、
                                   “CK48M振荡源选择”定义的一个。 */
    uint32_t m_CLKDivision;     /* 时钟分频系数，
                                   取值满足“SCLK分频系数”、“HFCK分频系数”、“LFCK分频系数”、
                                   “CK48M分频系数”定义的一个。 */
    uint32_t m_PLLInputSource;  /* PLL输入时钟源选择，当振荡源为PLL时有效，
                                   取值为宏“PLL输入时钟源选择”中的一个。 */

    uint32_t m_StartDelay;      /* PLL 或 外部启动延时时间配置，当振荡源为PLL或外部时有效，
                                   取值为宏“PLL启动延时时间配置”或“外部启动延时时间配置”中的一个。 */
} OSC_InitTypeDef;

/**
  * 系统时钟源
  */
#define OSC_SYSTEM_SCLK                 ((uint32_t)0)
#define OSC_SYSTEM_HFCK                 ((uint32_t)1)
#define OSC_SYSTEM_LFCK                 ((uint32_t)2)
#define OSC_SYSTEM_CK48M                ((uint32_t)3)
#define CHECK_OSC_SYSTEM_CLK(PLL)       (((PLL) == OSC_SYSTEM_SCLK) \
                                        || ((PLL) == OSC_SYSTEM_HFCK) \
                                        || ((PLL) == OSC_SYSTEM_LFCK) \
                                        || ((PLL) == OSC_SYSTEM_LFCK))

/**
  * PLL选择
  */
#define PLL_SELECT_0                    ((uint32_t)0)
#define PLL_SELECT_1                    ((uint32_t)1)
#define PLL_SELECT_2                    ((uint32_t)2)
#define EXT_HF_SELECT                   ((uint32_t)3)
#define EXT_LF_SELECT                   ((uint32_t)4)
#define CHECK_PLL_SELECT(PLL)           (((PLL) == PLL_SELECT_0) \
                                      || ((PLL) == PLL_SELECT_1) \
                                      || ((PLL) == PLL_SELECT_2))

/**
  * PLL启动延时时间配置
  */
#define PLL_START_DELAY_64              ((uint32_t)0)
#define PLL_START_DELAY_128             ((uint32_t)1)
#define PLL_START_DELAY_256             ((uint32_t)2)
#define PLL_START_DELAY_512             ((uint32_t)3)
#define PLL_START_DELAY_1024            ((uint32_t)4)
#define PLL_START_DELAY_2048            ((uint32_t)5)
#define PLL_START_DELAY_4096            ((uint32_t)6)
#define PLL_START_DELAY_8192            ((uint32_t)7)
#define PLL_START_DELAY_16384           ((uint32_t)8)
#define PLL_START_DELAY_32768           ((uint32_t)9)
#define CHECK_PLL_START_DELAY(PLL)      ((uint32_t)(PLL) <= 0xF)

/**
  * 外部启动延时时间配置
  */
#define EXT_START_DELAY_0               ((uint32_t)0)
#define EXT_START_DELAY_2               ((uint32_t)1)
#define EXT_START_DELAY_4               ((uint32_t)2)
#define EXT_START_DELAY_8               ((uint32_t)3)
#define EXT_START_DELAY_16              ((uint32_t)4)
#define EXT_START_DELAY_32              ((uint32_t)5)
#define EXT_START_DELAY_64              ((uint32_t)6)
#define EXT_START_DELAY_128             ((uint32_t)7)
#define EXT_START_DELAY_256             ((uint32_t)8)
#define EXT_START_DELAY_512             ((uint32_t)9)
#define CHECK_EXT_START_DELAY(EXT)      ((uint32_t)(EXT) <= 0xF)

/**
  * PLL输入时钟源选择
  */
#define PLL_INPUT_INTHF                 ((uint32_t)0x0)
#define PLL_INPUT_EXTHF                 ((uint32_t)0x1)
#define CHECK_PLL_INPUT(CFG)            (((CFG) == PLL_INPUT_INTHF) \
                                        || ((CFG) == PLL_INPUT_EXTHF))

/**
  * PLL分频系数
  */

#define CHECK_PLL_MULTIPLE_MValue(MUL)  ((uint32_t)(MUL) <= 0x3FFF)

#define PLL_MULTIPLE_NValue_15	15
#define PLL_MULTIPLE_NValue_14	14
#define PLL_MULTIPLE_NValue_13	13
#define PLL_MULTIPLE_NValue_12	12
#define PLL_MULTIPLE_NValue_11	11
#define PLL_MULTIPLE_NValue_10	10
#define PLL_MULTIPLE_NValue_9	9
#define PLL_MULTIPLE_NValue_8	8
#define PLL_MULTIPLE_NValue_7	7
#define PLL_MULTIPLE_NValue_6	6
#define PLL_MULTIPLE_NValue_5	5
#define PLL_MULTIPLE_NValue_4	4
#define PLL_MULTIPLE_NValue_3	3
#define PLL_MULTIPLE_NValue_2	2
#define PLL_MULTIPLE_NValue_1	1
#define CHECK_PLL_MULTIPLE_NValue(MUL)    ((uint32_t)(MUL) <= 0xF)

#define PLL_MULTIPLE_NOValue_8	8
#define PLL_MULTIPLE_NOValue_4	4
#define PLL_MULTIPLE_NOValue_2	2
#define PLL_MULTIPLE_NOValue_1	1
#define CHECK_PLL_MULTIPLE_NOValue(MUL)   ((uint32_t)(MUL) <= 0x8)
/**
  * SCLK振荡源选择
  */
#define SCLK_SOURCE_INTHF                 ((uint32_t)0x0)
#define SCLK_SOURCE_INTLF                 ((uint32_t)0x1)
#define SCLK_SOURCE_EXTHF                 ((uint32_t)0x2)
#define SCLK_SOURCE_EXTLF                 ((uint32_t)0x3)
#define SCLK_SOURCE_PLL                   ((uint32_t)0x4)
#define SCLK_SOURCE_LP4M                  ((uint32_t)0x6)
#define CHECK_SCLK_SOURCE(SCLK)           ((uint32_t)(SCLK) <= 0x7)

/**
  * SCLK分频系数
  */
#define SCLK_DIVISION_1                   ((uint32_t)0x0)
#define SCLK_DIVISION_2                   ((uint32_t)0x1)
#define SCLK_DIVISION_4                   ((uint32_t)0x2)
#define SCLK_DIVISION_8                   ((uint32_t)0x3)
#define SCLK_DIVISION_16                  ((uint32_t)0x4)
#define SCLK_DIVISION_32                  ((uint32_t)0x5)
#define SCLK_DIVISION_64                  ((uint32_t)0x6)
#define SCLK_DIVISION_128                 ((uint32_t)0x7)
#define CHECK_SCLK_DIVISION(SCLK)         ((uint32_t)(SCLK) <= SCLK_DIVISION_128)

/**
  * CLKOUT输出时钟选择
  */
#define CLKOUT_SCLK              		  ((uint32_t)0x0)
#define CLKOUT_EXTLF                      ((uint32_t)0x1)
#define CLKOUT_EXTHF                      ((uint32_t)0x2)
#define CLKOUT_INTLF                      ((uint32_t)0x3)
#define CLKOUT_INTHF                      ((uint32_t)0x4)
#define CLKOUT_PLL                        ((uint32_t)0x5)
#define CLKOUT_LP4M				          ((uint32_t)0x6)
#define CHECK_CLKOUT_SEL(CLKOUT)          ((uint32_t)(CLKOUT) <= 0x6)

/**
  * HFCK振荡源选择
  */
#define HFCK_SOURCE_INTHF                ((uint32_t)0x0)
#define HFCK_SOURCE_EXTHF                ((uint32_t)0x2)
#define HFCK_SOURCE_PLL                  ((uint32_t)0x4)
#define CHECK_HFCK_SOURCE(HFCK)          ((uint32_t)(HFCK) <= 0x7)

/**
  * HFCK分频系数
  */
#define HFCK_DIVISION_1                  ((uint32_t)0x0)
#define HFCK_DIVISION_2                  ((uint32_t)0x1)
#define HFCK_DIVISION_4                  ((uint32_t)0x2)
#define HFCK_DIVISION_8                  ((uint32_t)0x3)
#define HFCK_DIVISION_16                 ((uint32_t)0x4)
#define HFCK_DIVISION_32                 ((uint32_t)0x5)
#define HFCK_DIVISION_64                 ((uint32_t)0x6)
#define HFCK_DIVISION_128                ((uint32_t)0x7)
#define HFCK_DIVISION_256                ((uint32_t)0x8)
#define HFCK_DIVISION_512                ((uint32_t)0x9)
#define CHECK_HFCK_DIVISION(HFCK)        ((uint32_t)(HFCK) <= 0xF)

/**
  * LFCK振荡源选择
  */
#define LFCK_INPUT_INTLF                ((uint32_t)0x0)
#define LFCK_INPUT_EXTLF                ((uint32_t)0x1)
#define CHECK_LFCK_INPUT(LFCK)          (((LFCK) == LFCK_INPUT_INTLF) \
                                        || ((LFCK) == LFCK_INPUT_EXTLF))

/**
  * LFCK分频系数
  */
#define LFCK_DIVISION_1                 ((uint32_t)0x0)
#define LFCK_DIVISION_2                 ((uint32_t)0x1)
#define LFCK_DIVISION_4                 ((uint32_t)0x2)
#define LFCK_DIVISION_8                 ((uint32_t)0x3)
#define LFCK_DIVISION_16                ((uint32_t)0x4)
#define LFCK_DIVISION_32                ((uint32_t)0x5)
#define LFCK_DIVISION_64                ((uint32_t)0x6)
#define LFCK_DIVISION_128               ((uint32_t)0x7)
#define CHECK_LFCK_DIVISION(LFCK)       ((uint32_t)(LFCK) <= LFCK_DIVISION_128)

/**
  * CK48M振荡源选择
  */
#define CK48M_SOURCE_INTHF              ((uint32_t)0x0)
#define CK48M_SOURCE_EXTHF              ((uint32_t)0x2)
#define CK48M_SOURCE_PLL                ((uint32_t)0x4)
#define CHECK_CK48M_SOURCE(CLK)         ((uint32_t)(CLK) <= 0x7)

/**
  * CK48M分频系数
  */
#define CK48M_DIVISION_1                ((uint32_t)0x0)
#define CK48M_DIVISION_2                ((uint32_t)0x1)
#define CK48M_DIVISION_4                ((uint32_t)0x2)
#define CHECK_CK48M_DIVISION(CLK)       ((uint32_t)(CLK) <= 0x3)

/**
  * 零温漂电流档选择
  */
#define ZERO_DRIFT_SCALEDIV32           OSC_HFOSCCAL0_SCALEDIV32
#define ZERO_DRIFT_SCALEDIV16           OSC_HFOSCCAL0_SCALEDIV16
#define ZERO_DRIFT_SCALEDIV8            OSC_HFOSCCAL0_SCALEDIV8
#define ZERO_DRIFT_SCALEDIV4            OSC_HFOSCCAL0_SCALEDIV4
#define ZERO_DRIFT_SCALEDIV2            OSC_HFOSCCAL0_SCALEDIV2
#define ZERO_DRIFT_SCALE1X              OSC_HFOSCCAL0_SCALE1X
#define ZERO_DRIFT_SCALE2X              OSC_HFOSCCAL0_SCALE2X
#define ZERO_DRIFT_SCALE4X              OSC_HFOSCCAL0_SCALE4X
#define ZERO_DRIFT_SCALE8X              OSC_HFOSCCAL0_SCALE8X
#define ZERO_DRIFT_SCALE16X0            OSC_HFOSCCAL0_SCALE16X0
#define ZERO_DRIFT_SCALE16X1            OSC_HFOSCCAL0_SCALE16X1
#define ZERO_DRIFT_SCALE16X2            OSC_HFOSCCAL0_SCALE16X2
#define ZERO_DRIFT_SCALE16X3            OSC_HFOSCCAL0_SCALE16X3
#define ZERO_DRIFT_SCALE16X4            OSC_HFOSCCAL0_SCALE16X4
#define ZERO_DRIFT_SCALE32X             OSC_HFOSCCAL0_SCALE32X
#define CHECK_ZERO_DRIFT(SCALE)         (((SCALE) << 16) == 0)

/**
  * 内部高频振荡器的正温调节控制
  */
#define CHECK_POSITIVE_DRIFT(SCALE)     ((uint32_t)(SCALE) <= 0x1F)

/**
  * 内部高频振荡器的负温调节控制
  */
#define CHECK_NEGATIVE_DRIFT(SCALE)     ((uint32_t)(SCALE) <= 0x1F)

/**
  * 电流增益选择
  */
#define CHECK_CURRENT_GAIN(SEL)         ((uint32_t)(SEL) <= 3)

/**
  * 反馈电阻调节位
  */
#define FREQUENCY_MORE_THAN_10M         ((uint32_t)0)
#define FREQUENCY_LESS_THAN_10M         ((uint32_t)1)
#define CHECK_FEEDBACK_RESISTANCE(SW)   (((SW) == FREQUENCY_MORE_THAN_10M) \
                                      || ((SW) == FREQUENCY_LESS_THAN_10M))


/* 振荡器(OSC)外设初始化函数****************************************/
void OSC_SCLK_Configuration (OSC_InitTypeDef* oscInitStruct);
void OSC_HFCK_Configuration (OSC_InitTypeDef* oscInitStruct);
void OSC_LFCK_Configuration (OSC_InitTypeDef* oscInitStruct);
void OSC_Struct_Init (OSC_InitTypeDef* oscInitStruct);
/* 振荡器(OSC)功能配置函数******************************************/
void OSC_LFCK_Division_Config (uint32_t LFDivision);
void OSC_HFCK_Division_Config (uint32_t HFDivision);
void OSC_SCK_Division_Config (uint32_t SclkDivision);
void OSC_PLL_Input_Source_Config (uint32_t NewState);
void OSC_HFCK_Source_Config (uint32_t HFSource);
void OSC_HFCK_Enable (FunctionalState NewState);
void OSC_LFCK_Source_Config (uint32_t NewState);
void OSC_LFCK_Enable (FunctionalState NewState);
void OSC_SCK_Source_Config (uint32_t SclkSource);
void OSC_Backup_Write_Read_Enable (FunctionalState NewState);
void OSC_SCLK_Output_Enable (FunctionalState NewState);
void OSC_SCLK_Output_Select (uint32_t SclkSource);
void OSC_SCLK_Output_Division_Config (uint32_t OutputDivision);
void OSC_Clock_Failure_Check_Enable (FunctionalState NewState);
void OSC_PLL_Multiple_Value_Select (uint32_t PLLmultiple_M,uint32_t PLLmultiple_N,uint32_t PLLmultiple_NO);
void OSC_PLL_Start_Delay_Config (uint32_t PLLDelay);
void OSC_EXTHF_Start_Delay_Config (uint32_t ExternalDelay);
void OSC_EXTLF_Start_Delay_Config (uint32_t ExternalDelay);
void OSC_PLL_Software_Enable (FunctionalState NewState);
void OSC_EXTHF_Software_Enable (FunctionalState NewState);
void OSC_EXTLF_Software_Enable (FunctionalState NewState);
void OSC_INTHF_Software_Enable (FunctionalState NewState);
void OSC_INTLF_Software_Enable (FunctionalState NewState);
void OSC_Zero_Drift_Config (uint32_t Scale, FunctionalState NewState);
void OSC_Positive_Drift_Config (uint32_t PositiveDrift);
void OSC_Negative_Drift_Config (uint32_t PositiveDrift);
void OSC_Current_Gain_Config (uint32_t PositiveDrift);
void OSC_High_Speed_Enable (FunctionalState NewState);
void OSC_External_Input_Enable (FunctionalState NewState);
void OSC_Feedback_Resistance_Config (uint32_t NewState);
void OSC_PLL_Feedback_Test_Enable (FunctionalState NewState);
void OSC_PLL_Zero_Source_Enable (FunctionalState NewState);
void OSC_PLL_Vref2_Enable (FunctionalState NewState);
void OSC_PLL_Vref0p5_Enable (FunctionalState NewState);
void OSC_PLL_Vref1p2_Enable (FunctionalState NewState);
void OSC_PLL_Low_Power_20nA_Enable (FunctionalState NewState);
void OSC_PLL_Vref1p14_Enable (FunctionalState NewState);
void OSC_PLL_Low_Power_100nA_Enable (FunctionalState NewState);
void OSC_PLL_LDO_Enable (FunctionalState NewState);
/* 振荡器(OSC)中断控制函数******************************************/
void OSC_PLL_INT_Enable (FunctionalState NewState);
void OSC_EXTHF_INT_Enable (FunctionalState NewState);
void OSC_EXTLF_INT_Enable (FunctionalState NewState);
void OSC_INTHF_INT_Enable (FunctionalState NewState);
void OSC_INTLF_INT_Enable (FunctionalState NewState);
FlagStatus OSC_Get_Clock_Failure_INT_Flag (void);
FlagStatus OSC_Get_PLL_INT_Flag (void);
FlagStatus OSC_Get_EXTHF_INT_Flag (void);
FlagStatus OSC_Get_EXTLF_INT_Flag (void);
FlagStatus OSC_Get_INTHF_INT_Flag (void);
FlagStatus OSC_Get_INTLF_INT_Flag (void);
void OSC_PLL_RST(void);
#endif /* _KF32A_BASIC_OSC_H */
