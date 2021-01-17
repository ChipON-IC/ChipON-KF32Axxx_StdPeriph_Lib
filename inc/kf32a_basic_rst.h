/**
  ******************************************************************************
  * 文件名  kf32a_basic_rst.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了复位模块(RST)相关功能函数声明及相关宏定义。
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_RST_H_
#define _KF32A_BASIC_RST_H_

#include "KF32A_BASIC.h"


/**
  * 复位控制寄存器0对于的外设编号
  */
#define CHECK_RST_CTL0_RESET_PERIPH(PERIPH) ((((PERIPH) >> 9) == 0x00) \
                                          && ((PERIPH) != 0x00))

/**
  * 复位控制寄存器1对于的外设编号
  */
#define CHECK_RST_CTL1_RESET_PERIPH(PERIPH) ((((PERIPH) & 0x40000281) == 0x00) \
                                          && ((PERIPH) != 0x00))

/**
  * 复位控制寄存器2对于的外设编号
  */
#define CHECK_RST_CTL2_RESET_PERIPH(PERIPH) ((((PERIPH) & 0x1810AFD3) == 0x00) \
                                          && ((PERIPH) != 0x00))

/**
  * 复位控制寄存器3对于的外设编号
  */
#define CHECK_RST_CTL3_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xFFE01F79) == 0x00) \
                                          && ((PERIPH) != 0x00))


/* 复位模块(RST)功能配置函数定义********************************************/
void RST_CTL0_Peripheral_Reset_Enable(uint32_t RST_CTL0Periph,
                    FunctionalState NewState);
void RST_CTL1_Peripheral_Reset_Enable(uint32_t RST_CTL1Periph,
                    FunctionalState NewState);
void RST_CTL2_Peripheral_Reset_Enable(uint32_t RST_CTL2Periph,
                    FunctionalState NewState);
void RST_CTL3_Peripheral_Reset_Enable(uint32_t RST_CTL3Periph,
                    FunctionalState NewState);

#endif /* _KF32A_BASIC_RST_H */
