/**
  ******************************************************************************
  * 文件名  kf32a_basic_exic.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描述    该文件用于EXIC模块外设的库函数声明及相关宏定义。
  *
  *********************************************************************
  */


#ifndef _KF32A_BASIC_EXIC_H_
#define _KF32A_BASIC_EXIC_H_

#include "KF32A_BASIC.h"


/**
  * 描述  EXIC接口模块配置信息结构体
  */
typedef struct
{
	FunctionalState m_Module_Enable; /* EXIC接口使能选择，
                                            取值为TRUE或FALSE。 */
	FunctionalState m_CS_Enable; /* EXIC片选使能选择，
                                           取值为TRUE或FALSE的一个。 */
	uint32_t m_Data_Control_Sel; /* EXIC数据/命令选择，
                                           取值为“EXIC数据/命令选择。 */
	FunctionalState m_Write_Enable; /* EXIC写数据使能，
                                            取值为TRUE或FALSE。 */
	FunctionalState m_Read_Enable; /* EXIC读数据使能，
                                           取值为TRUE或FALSE。 */
    FunctionalState m_DMA_Write_EN;  /* EXIC DMA写使能，
                                           取值为TRUE或FALSE。   */
    FunctionalState m_DMA_Read_EN; /* EXIC DMA读使能，
                                            取值为TRUE或FALSE。 */
    FunctionalState m_WriteFnish_INT_EN;/* EXIC高优先级通道外部触发转换模式使能，
                                           取值为TRUE或FALSE。 */
    FunctionalState m_ReadFnish_INT_EN; /* 读完成中断使能，
                                           取值为TRUE或FALSE。 */
    FunctionalState m_WriteData_EN;/* EXIC写数据使能，
                                           取值为TRUE或FALSE。 */
    FunctionalState m_ReadData_EN;  /* EXIC读数据使能，
                                           取值为TRUE或FALSE。 */
    uint32_t m_Data_Direction_Sel;/* EXIC数据方向选择，
                                            取值为宏“EXIC 数据方向选择”中的一个。 */
    uint32_t m_Mode_Sel;/* EXIC模式选择，
                                           取值为宏“EXIC 模式选择位”中的一个。 */
    uint32_t m_Source_Clock_Sel;/* EXIC工作时钟源选择，
                                            取值为宏“EXIC 工作时钟源选择”中的一个。 */
    uint32_t m_Pulse_Width_Sel; /* EXIC读写脉宽选择，
                                            取值为宏“EXIC 读写脉宽选择”中的一个。 */

}EXIC_InitTypeDef;

/**
 * EXIC 数据/命令选择
 */
#define EXIC_TRANSMIT_DATA           ( (uint32_t) 0x0 << EXIC_CTL0_DCSEL_POS )
#define EXIC_TRANSMIT_COMMAND        ( (uint32_t) 0x1 << EXIC_CTL0_DCSEL_POS )
#define CHECK_EXIC_TRANSMIT(SEL)       (((SEL) == EXIC_TRANSMIT_DATA) \
                                        || ((SEL) == EXIC_TRANSMIT_COMMAND) )
/**
 * EXIC 数据方向选择
 */
#define EXIC_DATA_INPUT              ( (uint32_t) 0x1 << EXIC_CTL0_DATATR_POS )
#define EXIC_DATA_OUTPUT             ( (uint32_t) 0x0 << EXIC_CTL0_DATATR_POS )
#define CHECK_DATA_DIRECTION(SEL)      (((SEL) == EXIC_DATA_INPUT) \
                                       || ((SEL) == EXIC_DATA_OUTPUT) )

/**
 * EXIC 模式选择
 */
#define EXIC_MODE_SEL_W1R1            ( (uint32_t) 0x1 << EXIC_CTL0_MODESEL_POS )
#define EXIC_MODE_SEL_WR              ( (uint32_t) 0x0 << EXIC_CTL0_MODESEL_POS )
#define CHECK_MODE_SEL(SEL)             (((SEL) == EXIC_MODE_SEL_W1R1) \
                                        || ((SEL) == EXIC_MODE_SEL_WR) )

/**
 * EXIC 工作时钟源选择
 */
#define EXIC_CLKSOURCE_SCLK           ( (uint32_t) 0x0 << EXIC_CTL1_EXICCS0_POS )
#define EXIC_CLKSOURCE_HFCLK          ( (uint32_t) 0x1 << EXIC_CTL1_EXICCS0_POS )
#define EXIC_CLKSOURCE_LFCLK          ( (uint32_t) 0x2 << EXIC_CTL1_EXICCS0_POS )
#define CHECK_EXIC_CLKSOURCE_SEL(SEL)   (((SEL) == EXIC_CLKSOURCE_SCLK) \
										|| ((SEL) == EXIC_CLKSOURCE_HFCLK) \
                                        || ((SEL) == EXIC_CLKSOURCE_LFCLK) )
/**
 * EXIC 读写脉宽选择
 */
#define EXIC_PULSEWIDTH_1Tclk           ( (uint32_t) 0x0 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_2Tclk           ( (uint32_t) 0x1 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_4Tclk           ( (uint32_t) 0x2 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_8Tclk           ( (uint32_t) 0x3 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_16Tclk          ( (uint32_t) 0x4 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_32Tclk          ( (uint32_t) 0x5 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_64Tclk          ( (uint32_t) 0x6 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_128Tclk         ( (uint32_t) 0x7 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_256Tclk         ( (uint32_t) 0x8 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_512Tclk         ( (uint32_t) 0x9 << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_1024Tclk        ( (uint32_t) 0xA << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_2048Tclk        ( (uint32_t) 0xB << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_4096Tclk        ( (uint32_t) 0xC << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_8192Tclk        ( (uint32_t) 0xD << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_16384Tclk       ( (uint32_t) 0xE << EXIC_CTL1_PULSEWIDTH0_POS )
#define EXIC_PULSEWIDTH_32768Tclk       ( (uint32_t) 0xF << EXIC_CTL1_PULSEWIDTH0_POS )
#define CHECK_PULSEWIDTH_SEL(SEL)       (((SEL) == EXIC_PULSEWIDTH_1Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_2Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_4Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_8Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_16Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_32Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_64Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_128Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_256Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_512Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_1024Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_2048Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_4096Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_8192Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_16384Tclk) \
										||((SEL) == EXIC_PULSEWIDTH_32768Tclk) )

#ifdef KF32A_Periph_exic
void EXIC_Struct_Init (EXIC_InitTypeDef* EXICInitStruct);
uint32_t EXIC_Read_Buff (void);
void EXIC_Write_Buff (uint32_t DATA);
void EXIC_Clear_Write_Finish_INT_Flag (void);
void EXIC_Clear_Read_Finish_INT_Flag (void);
FlagStatus EXIC_Get_Read_Finish_INT_Flag (void);
FlagStatus EXIC_Get_Write_Finish_INT_Flag (void);
void EXIC_Configuration (EXIC_InitTypeDef* EXICInitStruct);
void EXIC_Reset (void);

#endif //KF32A_Periph_exic
#endif /* KF32A_BASIC_EXIC_H_ */
