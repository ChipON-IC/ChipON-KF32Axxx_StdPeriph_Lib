/**
  ******************************************************************************
  * 文件名  kf32a_basic_sysctl.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了系统控制相关功能函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_SYSCTL_H_
#define _KF32A_BASIC_SYSCTL_H_

#include "KF32A_BASIC.h"

/**
  * 系统控制(SYSCTL)指针定义
  */
#define CHECK_SYSCTL_PERIPH(PERIPH)     ((PERIPH) == SYSCTL_SFR)

/**
  * 当前有效堆栈指针选择
  */
#define SYSCTL_SP_IS_MSP                ((uint32_t)0<<SYS_MCTL_SPSEL_POS)
#define SYSCTL_SP_IS_PSP                ((uint32_t)1<<SYS_MCTL_SPSEL_POS)
#define CHECK_SYSCTL_SP(SP)             (((SP) == SYSCTL_SP_IS_MSP) \
                                      || ((SP) == SYSCTL_SP_IS_PSP))

/**
  * 累加器编号
  */
#define ACC_NUMBER_0                    ((uint32_t)0x1)
#define ACC_NUMBER_1                    ((uint32_t)0x2)
#define ACC_NUMBER_2                    ((uint32_t)0x4)
#define ACC_NUMBER_3                    ((uint32_t)0x8)
#define ACC_NUMBER_4                    ((uint32_t)0x10)
#define ACC_NUMBER_5                    ((uint32_t)0x20)
#define ACC_NUMBER_6                    ((uint32_t)0x40)
#define ACC_NUMBER_7                    ((uint32_t)0x80)
#define CHECK_ACC_NUMBER(NUM)           ((uint32_t)(NUM) <= 0xFF)
#define CHECK_ONE_ACC_NUMBER(NUM)       (((NUM) == ACC_NUMBER_0) \
                                      || ((NUM) == ACC_NUMBER_1) \
                                      || ((NUM) == ACC_NUMBER_2) \
                                      || ((NUM) == ACC_NUMBER_3) \
                                      || ((NUM) == ACC_NUMBER_4) \
                                      || ((NUM) == ACC_NUMBER_5) \
                                      || ((NUM) == ACC_NUMBER_6) \
                                      || ((NUM) == ACC_NUMBER_7))

/**
  * 舍入模式选择
  */
#define SYSCTL_NON_BIAS_ROUND           ((uint32_t)0 << SYS_DSPCS_ROUND_POS)
#define SYSCTL_BIAS_ROUND               ((uint32_t)1 << SYS_DSPCS_ROUND_POS)
#define CHECK_SYSCTL_ROUND_MODE(MODE)   (((MODE) == SYSCTL_NON_BIAS_ROUND) \
                                      || ((MODE) == SYSCTL_BIAS_ROUND))

/**
  * RAM空间长度
  */
#define CHECK_RAM_SPACE(LENGTH)         ((uint32_t)(LENGTH) <= SYS_RAMSPA_RAMSPA)

/**
  * FLASH开始512字节空间映射控制
  */
#define SYSCTL_FLASH_REMAP_FOR_ROM      ((uint32_t)0 << SYS_MEMCTL_MEMM0_POS)
#define SYSCTL_FLASH_REMAP_FOR_RAM      ((uint32_t)1 << SYS_MEMCTL_MEMM0_POS)
#define SYSCTL_FLASH_REMAP_FOR_FLASH    ((uint32_t)2 << SYS_MEMCTL_MEMM0_POS)
#define SYSCTL_FLASH_REMAP_STOP_CPU     ((uint32_t)3 << SYS_MEMCTL_MEMM0_POS)
#define CHECK_SYSCTL_FLASH_REMAP(CTL)   (((CTL) == SYSCTL_FLASH_REMAP_FOR_ROM) \
                                      || ((CTL) == SYSCTL_FLASH_REMAP_FOR_RAM) \
                                      || ((CTL) == SYSCTL_FLASH_REMAP_FOR_FLASH) \
                                      || ((CTL) == SYSCTL_FLASH_REMAP_STOP_CPU))

/**
  * DO循环深度
  */
#define DOLOOP_NEST_0                   ((uint32_t)0 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_1                   ((uint32_t)1 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_2                   ((uint32_t)2 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_3                   ((uint32_t)3 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_4                   ((uint32_t)4 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_5                   ((uint32_t)5 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_6                   ((uint32_t)6 << SYS_DOCTL_DONEST0_POS)
#define DOLOOP_NEST_7                   ((uint32_t)7 << SYS_DOCTL_DONEST0_POS)
#define CHECK_DOLOOP_NEST(NEST)         (((NEST) == DOLOOP_NEST_0) \
                                      || ((NEST) == DOLOOP_NEST_1) \
                                      || ((NEST) == DOLOOP_NEST_2) \
                                      || ((NEST) == DOLOOP_NEST_3) \
                                      || ((NEST) == DOLOOP_NEST_4) \
                                      || ((NEST) == DOLOOP_NEST_5) \
                                      || ((NEST) == DOLOOP_NEST_6) \
                                      || ((NEST) == DOLOOP_NEST_7))


/* 系统控制(SYSCTL)功能配置函数定义*****************************************/
FlagStatus SYSCTL_Get_V_Flag (void);
FlagStatus SYSCTL_Get_C_Flag (void);
FlagStatus SYSCTL_Get_Z_Flag (void);
FlagStatus SYSCTL_Get_N_Flag (void);
void SYSCTL_Set_V_Flag (FunctionalState NewState);
void SYSCTL_Set_C_Flag (FunctionalState NewState);
void SYSCTL_Set_Z_Flag (FunctionalState NewState);
void SYSCTL_Set_N_Flag (FunctionalState NewState);
void SYSCTL_Sleep_On_Exit_Enable (FunctionalState NewState);
void SYSCTL_Deep_Sleep_Enable (FunctionalState NewState);
void SYSCTL_Interrupt_Awake_Enable (FunctionalState NewState);
FlagStatus SYSCTL_Stack_Align_State (void);
void SYSCTL_Super_User_Config (FunctionalState NewState);
FlagStatus SYSCTL_Stack_Pointer_State (void);
void SYSCTL_Stack_Pointer_Config (uint32_t PresentSP);
void SYSCTL_Exception_Reset_Enable (FunctionalState NewState);
void SYSCTL_System_Reset_Enable (FunctionalState NewState);

#endif /* _KF32A_BASIC_SYSCTL_H */
