/**
  ******************************************************************************
  * 文件名  kf32a_basic_bkp.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描  述  该文件提供了备份域(BKP)的库函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_BKP_H_
#define _KF32A_BASIC_BKP_H_

#include "KF32A_BASIC.h"


/**
  * 侵入检测引脚选择
  */
#define BKP_PIN_RTC_TAMP1               ((uint32_t)0)
#define BKP_PIN_RTC_TAMP2               ((uint32_t)1)
#define BKP_PIN_RTC_TAMP3               ((uint32_t)2)
#define CHECK_BKP_PIN_RTC_TAMP(SEL)     (((SEL) == BKP_PIN_RTC_TAMP1) \
                                      || ((SEL) == BKP_PIN_RTC_TAMP2) \
                                      || ((SEL) == BKP_PIN_RTC_TAMP3))

/**
  * 有效电平
  */
#define BKP_HIGH_LEVEL_EFFECTIVE        ((uint32_t)0)
#define BKP_LOW_LEVEL_EFFECTIVE         ((uint32_t)1)
#define CHECK_BKP_EFFECTIVE_LEVEL(SEL)  (((SEL) == BKP_HIGH_LEVEL_EFFECTIVE) \
                                      || ((SEL) == BKP_LOW_LEVEL_EFFECTIVE))

/**
  * RTC时钟源
  */
#define BKP_RTC_NO_CLK                  ((uint32_t)0<<BKP_CTL_RTCCLKS0_POS)
#define BKP_RTC_EXTLF                   ((uint32_t)1<<BKP_CTL_RTCCLKS0_POS)
#define BKP_RTC_INTLF                   ((uint32_t)2<<BKP_CTL_RTCCLKS0_POS)
#define BKP_RTC_EXTHF_DIV_128           ((uint32_t)3<<BKP_CTL_RTCCLKS0_POS)
#define CHECK_BKP_RTC_CLK(SEL)          (((SEL) == BKP_RTC_NO_CLK) \
                                      || ((SEL) == BKP_RTC_EXTLF) \
                                      || ((SEL) == BKP_RTC_INTLF) \
                                      || ((SEL) == BKP_RTC_EXTHF_DIV_128))

/**
  * 时钟源选择
  */
#define BKP_EXTHF                       ((uint32_t)0)
#define BKP_EXTLF                       ((uint32_t)1)
#define CHECK_BKP_CLOCK_SOURCE(SEL)     (((SEL) == BKP_EXTLF) \
                                      || ((SEL) == BKP_EXTHF))

/**
  * 备份数据寄存器编号
  */
#define CHECK_BKP_DATA_NUMBER(NUM)      ((uint32_t)(NUM) < 32)


/* 备份域(BKP)复位及读写允许函数定义****************************************/
void BKP_Reset (void);
void BKP_Write_And_Read_Enable (FunctionalState NewState);
/* 备份域(BKP)功能配置函数定义**********************************************/
void BKP_Reset_Enable (FunctionalState NewState);
void BKP_Pin_Effective_Level_Config (uint32_t PinSel, uint32_t Effective);
void BKP_Pin_Enable (uint32_t PinSel, FunctionalState NewState);
void BKP_RTC_Clock_Config (uint32_t Source);
void BKP_External_Clock_Bypass_Enable (uint32_t Source,
                    FunctionalState NewState);
void BKP_Data_Config (uint32_t OrderNumber, uint32_t WriteData);
uint32_t BKP_Get_Data (uint32_t OrderNumber);
/* 备份域(BKP)中断管理函数定义**********************************************/
void BKP_Pin_TAMP_INT_Enable (uint32_t PinSel, FunctionalState NewState);
FlagStatus BKP_Get_Pin_TAMP_INT_Flag (uint32_t PinSel);
void BKP_Clear_Pin_TAMP_INT_Flag (uint32_t PinSel);

#endif /* _KF32A_BASIC_BKP_H */
