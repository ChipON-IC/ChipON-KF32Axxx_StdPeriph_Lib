/**
  ******************************************************************************
  * 文件名  kf32a_basic_dac.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描述    该文件用于数模转换器(DAC)外设的库函数声明及相关宏定义。
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_DAC_H_
#define _KF32A_BASIC_DAC_H_

#include "KF32A_BASIC.h"


/**
  * DAC外设判断
  */
//#define CHECK_DAC_ALL_PERIPH(PERIPH)    (((PERIPH) == DAC0_SFR) \
//                                      || ((PERIPH) == DAC1_SFR))
#define CHECK_DAC_ALL_PERIPH(PERIPH)    (((PERIPH) == ((DAC_SFRmap *)(uint32_t)0x40000700)) \
                                      || ((PERIPH) == ((DAC_SFRmap *)(uint32_t)0x40000780)))
/**
  * 描述  DAC配置信息结构体
  */
typedef struct
{
    FunctionalState m_TriggerEnable;    /* DAC通道触发使能，
                                           取值为TRUE或FALSE */
    uint32_t m_TriggerEvent;            /* DAC通道触发事件
                                           取值为宏“DAC触发事件”中的一个 */
    FunctionalState m_TriggerDMAEnable; /* DAC触发DMA使能，
                                           取值为TRUE或FALSE */
    uint32_t m_Wave;                    /* DAC波形发生器使能
                                           取值为宏“DAC波形发生器使能选择”中的一个 */
    uint32_t m_Mas;                     /* DAC屏蔽/幅值选择器
                                           取值为宏“DAC屏蔽/幅值选择器”中的一个 */
    uint32_t m_Clock;                   /* DAC工作时钟
                                           取值为宏“DAC工作时钟”中的一个 */
    uint32_t m_ReferenceVoltage;        /* DAC参考电压选择
                                           取值为宏“DAC参考电压选择”中的一个 */
    FunctionalState m_OutputBuffer;     /* DAC输出缓冲使能，
                                           取值为TRUE或FALSE */
    uint32_t m_OutputPin;               /* DAC输出引脚，
                                           取值为宏“DAC输出引脚选择”中的一个 */
    uint32_t m_Output;                  /* DAC输出数据
                                           取值为12位数据(0~0xFFF) */
}DAC_InitTypeDef;

/**
  * 描述  DAC校验配置信息结构体
  */
typedef struct
{
    uint32_t m_PPositrim;               /* P管正失调方向校准位。
                                           取值为宏“DAC校准配置”中的一个 */
    uint32_t m_PNegatrim;               /* P管负失调方向校准位。
                                           取值为宏“DAC校准配置”中的一个 */
    uint32_t m_NPositrim;               /* N管正失调方向校准位。
                                           取值为宏“DAC校准配置”中的一个 */
    uint32_t m_NNegatrim;               /* N管负失调方向校准位。
                                           取值为宏“DAC校准配置”中的一个 */
    uint32_t m_Buftrim;                 /* 失调校准使能。
                                           取值为枚举类型TRUE或FALSE */
}DAC_CALTypeDef;

/**
  * DAC输出引脚选择
  */
#define DAC_OUTPUT_PIN_0                ((uint32_t)0 << DAC_CTL1_SEL_POS)
#define DAC_OUTPUT_PIN_1                ((uint32_t)1 << DAC_CTL1_SEL_POS)
#define CHECK_DAC_OUTPUT_PIN(SEL)       (((SEL) == DAC_OUTPUT_PIN_0) \
                                      || ((SEL) == DAC_OUTPUT_PIN_1))

/**
  * DAC触发事件
  */
#define DAC_TRIGGER_T1_TRGO             ((uint32_t)0x0 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T3_TRGO             ((uint32_t)0x1 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T5_TRGO             ((uint32_t)0x2 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T9_TRGO             ((uint32_t)0x3 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T14_TRGO            ((uint32_t)0x4 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T15_TRGO            ((uint32_t)0x5 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_EINT9_TRGO          ((uint32_t)0x6 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_SOFTWARE_TRGO       ((uint32_t)0x7 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T0_TRGO             ((uint32_t)0x8 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T2_TRGO             ((uint32_t)0x9 << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T4_TRGO             ((uint32_t)0xA << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T18_TRGO            ((uint32_t)0xB << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T19_TRGO            ((uint32_t)0xC << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T20_TRGO            ((uint32_t)0xD << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_T21_TRGO            ((uint32_t)0xE << DAC_CTL_TREVT0_POS)
#define DAC_TRIGGER_NONE                ((uint32_t)0xF << DAC_CTL_TREVT0_POS)
#define CHECK_DAC_TRIGGER_EVENT(EVENT)  ((((EVENT) >> 8) == 0) \
                                        && (((EVENT) & 0xF) == 0))

/**
  * DAC波形发生器使能选择
  */
#define DAC_WAVE_NONE                   ((uint32_t)0<<DAC_CTL_WAVE0_POS)
#define DAC_WAVE_NOISE                  ((uint32_t)1<<DAC_CTL_WAVE0_POS)
#define DAC_WAVE_TRIANGLE               ((uint32_t)2<<DAC_CTL_WAVE0_POS)
#define CHECK_DAC_WAVE(WAVE)            (((WAVE) == DAC_WAVE_NONE) \
        							    || ((WAVE) == DAC_WAVE_NOISE) \
        							    || ((WAVE) == DAC_WAVE_TRIANGLE))

/**
  * DAC屏蔽/幅值选择器
  */
#define DAC_LFSR_UNMASK_BITS0_0         ((uint32_t)0x0<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS1_0         ((uint32_t)0x1<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS2_0         ((uint32_t)0x2<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS3_0         ((uint32_t)0x3<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS4_0         ((uint32_t)0x4<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS5_0         ((uint32_t)0x5<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS6_0         ((uint32_t)0x6<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS7_0         ((uint32_t)0x7<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS8_0         ((uint32_t)0x8<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS9_0         ((uint32_t)0x9<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS10_0        ((uint32_t)0xA<<DAC_CTL_MAS0_POS)
#define DAC_LFSR_UNMASK_BITS11_0        ((uint32_t)0xB<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_1        ((uint32_t)0x0<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_3        ((uint32_t)0x1<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_7        ((uint32_t)0x2<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_15       ((uint32_t)0x3<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_31       ((uint32_t)0x4<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_63       ((uint32_t)0x5<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_127      ((uint32_t)0x6<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_255      ((uint32_t)0x7<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_511      ((uint32_t)0x8<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_1023     ((uint32_t)0x9<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_2047     ((uint32_t)0xA<<DAC_CTL_MAS0_POS)
#define DAC_TRIANGLE_AMPLITUDE_4095     ((uint32_t)0xB<<DAC_CTL_MAS0_POS)
#define CHECK_DAC_MAS(MAS)              ((((MAS) >> (DAC_CTL_MAS0_POS + 4)) == 0) \
	                                    && (((MAS) << (32 - DAC_CTL_MAS0_POS)) == 0))

/**
  * DAC工作时钟
  */
#define DAC_CLK_SCLK                    ((uint32_t)0<<DAC_CTL1_CLK0_POS)
#define DAC_CLK_HFCLK                   ((uint32_t)1<<DAC_CTL1_CLK0_POS)
#define DAC_CLK_LFCLK                   ((uint32_t)2<<DAC_CTL1_CLK0_POS)
#define CHECK_DAC_CLK(CLK)              (((CLK) == DAC_CLK_SCLK) \
                                        || ((CLK) == DAC_CLK_HFCLK) \
                                        || ((CLK) == DAC_CLK_LFCLK))

/**
  * DAC参考电压选择
  */
#define DAC_RFS_AVDD                    ((uint32_t)0<<DAC_CTL1_RFS0_POS)
#define DAC_RFS_REF                     ((uint32_t)1<<DAC_CTL1_RFS0_POS)
#define DAC_RFS_1P2V                    ((uint32_t)2<<DAC_CTL1_RFS0_POS)
#define DAC_RFS_2V                      ((uint32_t)3<<DAC_CTL1_RFS0_POS)
#define CHECK_DAC_RFS(RFS)              (((RFS) == DAC_RFS_AVDD) \
                                        || ((RFS) == DAC_RFS_REF) \
                                        || ((RFS) == DAC_RFS_1P2V) \
                                        || ((RFS) == DAC_RFS_2V))
/**
  * DAC输出数据
  */
#define CHECK_DAC_OUTPUT(DATA)          (((DATA) >> 12) == 0)

/**
  * DAC校准配置
  */
#define DAC_CAL_0                       ((uint32_t)0x00)
#define DAC_CAL_1                       ((uint32_t)0x01)
#define DAC_CAL_2                       ((uint32_t)0x02)
#define DAC_CAL_4                       ((uint32_t)0x04)
#define DAC_CAL_8                       ((uint32_t)0x08)
#define CHECK_DAC_CAL(CAL)              (((CAL) == DAC_CAL_0) \
                                        || ((CAL) == DAC_CAL_1) \
                                        || ((CAL) == DAC_CAL_2) \
                                        || ((CAL) == DAC_CAL_4) \
                                        || ((CAL) == DAC_CAL_8))


/** 初始化及配置函数 ************************************************/
void DAC_Reset (DAC_SFRmap* DACx);
void DAC_Configuration (DAC_SFRmap* DACx, DAC_InitTypeDef* dacInitStruct);
void DAC_Struct_Init (DAC_InitTypeDef* dacInitStruct);
void DAC_Cmd (DAC_SFRmap* DACx, FunctionalState NewState);
void DAC_Software_Trigger_Cmd (DAC_SFRmap* DACx, FunctionalState NewState);
void DAC_DMA_Cmd (DAC_SFRmap* DACx, FunctionalState NewState);
void DAC_Output_Buffer_Config (DAC_SFRmap* DACx, FunctionalState NewState);
void DAC_Output_Port_Config (DAC_SFRmap* DACx, FunctionalState NewState);
/** 数据读写函数 ****************************************************/
void DAC_Write_Output_Data (DAC_SFRmap* DACx, uint32_t Value);
uint32_t DAC_Read_Output_Data (DAC_SFRmap* DACx);
/** DAC校验寄存器配置函数 *******************************************/
void DAC_Calibration_Config (DAC_SFRmap* DACx, DAC_CALTypeDef* dacInitStruct);
void DAC_Calibration_Struct_Init (DAC_CALTypeDef* dacCalStruct);

#endif /* _KF32A_BASIC_DAC_H */
