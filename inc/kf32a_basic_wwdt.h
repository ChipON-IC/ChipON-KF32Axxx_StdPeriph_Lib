/**
  ******************************************************************************
  * 文件名  kf32a_basic_wwdt.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了独立看门狗(WWDT)相关功能函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_WWDT_H_
#define _KF32A_BASIC_WWDT_H_

#include "KF32A_BASIC.h"


/**
  * 预分频值
  */
#define WWDT_PRESCALER_1                ((uint32_t)0x0<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_2                ((uint32_t)0x1<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_4                ((uint32_t)0x2<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_8                ((uint32_t)0x3<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_16               ((uint32_t)0x4<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_32               ((uint32_t)0x5<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_64               ((uint32_t)0x6<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_128              ((uint32_t)0x7<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_256              ((uint32_t)0x8<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_512              ((uint32_t)0x9<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_1024             ((uint32_t)0xA<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_2048             ((uint32_t)0xB<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_4096             ((uint32_t)0xC<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_8192             ((uint32_t)0xD<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_16384            ((uint32_t)0xE<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_32768            ((uint32_t)0xF<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_65536            ((uint32_t)0x10<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_131072           ((uint32_t)0x11<<WWDT_CTL_WPS0_POS)
#define WWDT_PRESCALER_262144           ((uint32_t)0x12<<WWDT_CTL_WPS0_POS)
#define CHECK_WWDT_PRESCALER(PS)        (((PS) == WWDT_PRESCALER_1) \
                                      || ((PS) == WWDT_PRESCALER_2) \
                                      || ((PS) == WWDT_PRESCALER_4) \
                                      || ((PS) == WWDT_PRESCALER_8) \
                                      || ((PS) == WWDT_PRESCALER_16) \
                                      || ((PS) == WWDT_PRESCALER_32) \
                                      || ((PS) == WWDT_PRESCALER_64) \
                                      || ((PS) == WWDT_PRESCALER_128) \
                                      || ((PS) == WWDT_PRESCALER_256) \
                                      || ((PS) == WWDT_PRESCALER_512) \
                                      || ((PS) == WWDT_PRESCALER_1024) \
                                      || ((PS) == WWDT_PRESCALER_2048) \
                                      || ((PS) == WWDT_PRESCALER_4096) \
                                      || ((PS) == WWDT_PRESCALER_8192) \
                                      || ((PS) == WWDT_PRESCALER_16384) \
                                      || ((PS) == WWDT_PRESCALER_32768) \
                                      || ((PS) == WWDT_PRESCALER_65536) \
                                      || ((PS) == WWDT_PRESCALER_131072) \
                                      || ((PS) == WWDT_PRESCALER_262144))

/**
  * 7位计数值
  */
#define CHECK_WWDT_THRESHOLD(VALUE)     ((uint32_t)(VALUE) <= 0x3F)


/* 窗口看门狗(WWDT)控制功能函数定义*****************************************/
void WWDT_Reset (void);
void WWDT_Threshold_Config (uint32_t Threshold);
void WWDT_Prescaler_Config (uint32_t Prescaler);
void WWDT_Enable (uint32_t NewState);
void WWDT_Counter_Config (uint32_t Counter);
uint32_t WWDT_Get_Counter (void);
/* 窗口看门狗(WWDT)中断管理函数定义*****************************************/
void WWDT_INT_Enable (uint32_t NewState);
FlagStatus WWDT_Get_INT_Flag (void);
void WWDT_Clear_INT_Flag (void);

#endif /* _KF32A_BASIC_WWDT_H */
