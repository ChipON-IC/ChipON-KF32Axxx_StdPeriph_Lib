/**
  ******************************************************************************
  * 文件名  kf32a_basic_pclk.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了外设时钟使能模块(PCLK)相关的库函数声明及相关宏定义。
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_PCLK_H_
#define _KF32A_BASIC_PCLK_H_

#include "KF32A_BASIC.h"


/**
  * 外设时钟控制寄存器0对于的外设编号
  */
#define CHECK_PCLK_CTL0_PERIPH(PERIPH) ((((PERIPH) >> 8) == 0x00) && ((PERIPH) != 0x00))

/**
  * 外设时钟控制寄存器0对于的外设编号
  */
#define CHECK_PCLK_CTL1_PERIPH(PERIPH) ((((PERIPH) & 0x40000281) == 0x00) && ((PERIPH) != 0x00))

/**
  * 外设时钟控制寄存器0对于的外设编号
  */
#define CHECK_PCLK_CTL2_PERIPH(PERIPH) ((((PERIPH) & 0x1810AFD3) == 0x00) && ((PERIPH) != 0x00))

/**
  * 外设时钟控制寄存器0对于的外设编号
  */
#define CHECK_PCLK_CTL3_PERIPH(PERIPH) ((((PERIPH) & 0xFFE01F79) == 0x00) && ((PERIPH) != 0x00))


/* 外设时钟使能模块(PCLK)功能配置函数定义***********************************/
void PCLK_CTL0_Peripheral_Clock_Enable (uint32_t PCLK_CTL0_bit,
                    FunctionalState NewState);
void PCLK_CTL1_Peripheral_Clock_Enable (uint32_t PCLK_CTL1_bit,
                    FunctionalState NewState);
void PCLK_CTL2_Peripheral_Clock_Enable (uint32_t PCLK_CTL2_bit,
                    FunctionalState NewState);
void PCLK_CTL3_Peripheral_Clock_Enable (uint32_t PCLK_CTL3_bit,
                    FunctionalState NewState);

#endif /* _KF32A_BASIC_PCLK_H */
