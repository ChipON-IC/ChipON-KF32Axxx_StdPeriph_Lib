/**
  ******************************************************************************
  * 文件名  kf32a_basic_btim.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了定时器外设功能函数，包含：
  *          + 定时器外设复位函数定义
  *          + 基本定时器(BTIM)初始化及配置函数定义
  *          + 基本定时器(BTIM)中断及标志管理函数定义
  *
  *********************************************************************
  */

#include "kf32a_basic_tim.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* 定时器私有定义 ----------------------------------------------------*/
/* BTIMx_CTL1寄存器配置掩码 */
#define BTIM_CTL1_INIT_MASK  (BTIM_CTL1_TXCLK | \
                              BTIM_CTL1_TXCMS | \
                              BTIM_CTL1_TXSY | \
                              BTIM_CTL1_TXCS)

/* BTIMx_CTL2寄存器配置掩码 */
#define BTIM_CTL2_INIT_MASK  (BTIM_CTL2_TXMMS | \
                              BTIM_CTL2_TXSMS | \
                              BTIM_CTL2_TXMSSYNC)


/**
  *   ##### 定时器外设复位函数定义 #####
  */
/**
  * 描述  定时器外设复位，使能外设时钟。
  * 输入  TIMx: 定时器内存结构指针，取值为 T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/
  *           T5_SFR/T6_SFR/T7_SFR/T8_SFR/T9_SFR/T10_SFR/
  *           T14_SFR/T15_SFR/T18_SFR/T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR。
  * 返回  无
  */
void TIM_Reset (void* TIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_TIM_ALL_PERIPH((uint32_t)TIMx));

#ifdef KF32A_Periph_gptime0
    if ((uint32_t)TIMx == T0_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_gptime1
   if ((uint32_t)TIMx == T1_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T1CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_gptime2
    if ((uint32_t)TIMx == T2_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T2RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T2RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T2CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_gptime3
    if ((uint32_t)TIMx == T3_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T3RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T3RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T3CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_gptime4
    if ((uint32_t)TIMx == T4_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T4RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T4RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T4CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_atime5
    if ((uint32_t)TIMx == T5_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T5T6RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T5T6RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T5T6CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_atime6
    if ((uint32_t)TIMx == T6_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T5T6RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T5T6RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T5T6CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_atime9
    if ((uint32_t)TIMx == T9_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T9T10RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T9T10RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T9T10CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_atime10
    if ((uint32_t)TIMx == T10_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T9T10RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_T9T10RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_T9T10CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_btime14
    if ((uint32_t)TIMx == T14_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T14RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T14RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T14CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_btime15
    if ((uint32_t)TIMx == T15_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T15RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T15RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T15CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_gptime18
    if ((uint32_t)TIMx == T18_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T18RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T18RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T18CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_gptime19
    if ((uint32_t)TIMx == T19_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T19RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T19RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T19CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_gptime20
    if ((uint32_t)TIMx == T20_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T20RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T20RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T20CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_gptime21
    if ((uint32_t)TIMx == T21_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T21RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T21RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T21CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_gptime22_time23
#ifdef  KF32A_Periph_gptime22
    if ((uint32_t)TIMx == T22_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T22RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T22RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T22CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_gptime23
    if ((uint32_t)TIMx == T23_ADDR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T23RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_T23RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_T23CLKEN, TRUE);
    }
#endif
#endif//KF32A_Periph_gptime22_time23

#ifdef  KF32A_Periph_qei
#ifdef  KF32A_Periph_qei0
    if ((uint32_t)TIMx == T7_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_QEI0CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_qei1
    if ((uint32_t)TIMx == T8_ADDR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_QEI1CLKEN, TRUE);
    }
#endif
#endif//KF32A_Periph_qei
}
/**
  *   ##### 定时器外设复位函数定义结束 #####
  */

/**
  *   ##### 基本定时器(BTIM)初始化及配置函数定义 #####
  */
/**
  * 描述  基本定时器(BTIM)配置。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       btimInitStruct: 基本定时器配置信息结构体指针。
  * 返回  无
  */
void
BTIM_Configuration (BTIM_SFRmap* BTIMx,
                    BTIM_InitTypeDef* btimInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_COUNTER_MODE(btimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_BTIM_CLOCK_CONFIG(btimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_BTIM_WORK_MODE(btimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_BTIM_MASTER_MODE(btimInitStruct->m_MasterMode));
    CHECK_RESTRICTION(CHECK_BTIM_SLAVE_MODE(btimInitStruct->m_SlaveMode));
    CHECK_RESTRICTION(CHECK_BTIM_SYNC_MODE(btimInitStruct->m_EXPulseSync));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(btimInitStruct->m_MasterSlaveSync));

    /*-------------------- 设置TxCNT寄存器 --------------------*/
    BTIMx->CNT = (uint32_t)btimInitStruct->m_Counter;

    /*-------------------- 设置PPx周期寄存器 --------------------*/
    BTIMx->PPX = (uint32_t)btimInitStruct->m_Period;

    /*-------------------- 设置Tx预分频寄存器 --------------------*/
    BTIMx->PRSC = (uint32_t)btimInitStruct->m_Prescaler;

    /*-------------------- 设置TXCTL1寄存器 --------------------*/
    /* 根据结构体成员m_CounterMode，设置TXCMS位域 */
    /* 根据结构体成员m_Clock，设置TXCLK位域 */
    /* 根据结构体成员m_WorkMode，设置TXCS位域 */
    /* 根据结构体成员m_EXPulseSync，设置TXSY位域 */
    tmpreg = ((uint32_t)btimInitStruct->m_CounterMode) | \
             ((uint32_t)btimInitStruct->m_Clock) | \
             ((uint32_t)btimInitStruct->m_WorkMode) | \
             ((uint32_t)btimInitStruct->m_EXPulseSync);
    BTIMx->CTL1 = SFR_Config (BTIMx->CTL1,
                          ~BTIM_CTL1_INIT_MASK,
                          tmpreg);

    /*-------------------- 设置TXCTL2寄存器 --------------------*/
    /* 根据结构体成员m_MasterMode，设置TXMMS位域 */
    /* 根据结构体成员m_SlaveMode，设置TXSMS位域 */
    /* 根据结构体成员m_MasterSlaveSync，设置TXMSSYNC位域 */
    tmpreg = ((uint32_t)btimInitStruct->m_MasterMode) | \
             ((uint32_t)btimInitStruct->m_SlaveMode) | \
             ((uint32_t)btimInitStruct->m_MasterSlaveSync << BTIM_CTL2_TXMSSYNC_POS);
    BTIMx->CTL2 = SFR_Config (BTIMx->CTL2,
                          ~BTIM_CTL2_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化基本定时器配置信息结构体。
  * 输入  btimInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
BTIM_Struct_Init (BTIM_InitTypeDef* btimInitStruct)
{
    /* 设置Tx计数值 */
    btimInitStruct->m_Counter = 0x0000;
    /* 设置Tx周期值 */
    btimInitStruct->m_Period = 0xFFFF;
    /* 设置Tx预分频值 */
    btimInitStruct->m_Prescaler = 0x0000;
    /* 设置Tx计数模式 */
    btimInitStruct->m_CounterMode = BTIM_COUNT_UP_OF;
    /* 设置Tx工作时钟 */
    btimInitStruct->m_Clock = BTIM_SCLK;
    /* 设置定时/计数模式选择 */
    btimInitStruct->m_WorkMode = BTIM_TIMER_MODE;
    /* 设置主模式选择 */
    btimInitStruct->m_MasterMode = BTIM_MASTER_UR_SIGNAL;
    /* 设置从模式选择 */
    btimInitStruct->m_SlaveMode = BTIM_SLAVE_FORBIDDEN_MODE;
    /* 设置Tx计数模式外部触发脉冲输入同步控制 */
    btimInitStruct->m_EXPulseSync = BTIM_EX_SYNC_MODE;
    /* 设置主从模式同步控制 */
    btimInitStruct->m_MasterSlaveSync = FALSE;
}

/**
  * 描述  定时器启动控制使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 定时器使能控制，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Cmd (BTIM_SFRmap* BTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL1寄存器TXEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(BTIMx->CTL1, BTIM_CTL1_TXEN_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(BTIMx->CTL1, BTIM_CTL1_TXEN_POS);
    }
}

/**
  * 描述  更新定时器计数值。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       Counter: 新的计数值，取值16位数据。
  * 返回  无
  */
void
BTIM_Set_Counter (BTIM_SFRmap* BTIMx, uint16_t Counter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /* 更新定时器计数值 */
    BTIMx->CNT = Counter;
}

/**
  * 描述  更新定时器周期值。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       Period: 新的周期值，取值16位数据。
  * 返回  无
  */
void
BTIM_Set_Period (BTIM_SFRmap* BTIMx, uint16_t Period)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /* 更新定时器计数值 */
    BTIMx->PPX = Period;
}

/**
  * 描述  更新定时器预分频值。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       Prescaler: 新的预分频，取值16位数据。
  * 返回  无
  */
void
BTIM_Set_Prescaler (BTIM_SFRmap* BTIMx, uint16_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /* 更新定时器计数值 */
    BTIMx->PRSC = Prescaler;
}

/**
  * 描述  更新定时器计数模式。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       CounterMode: 新的计数模式，
  *                    取值范围为：
  *                      BTIM_COUNT_DOWN_UF: 向下计数,下溢产生中断标志
  *                      BTIM_COUNT_UP_OF: 向上计数,上溢产生中断标志
  *                      BTIM_COUNT_UP_DOWN_OF: 向上-向下计数,上溢产生中断标志
  *                      BTIM_COUNT_UP_DOWN_UF: 向上-向下计数,下溢产生中断标志
  *                      BTIM_COUNT_UP_DOWN_OUF: 向上-向下计数,上溢和下溢产生中断标志
  * 返回  无
  */
void
BTIM_Counter_Mode_Config (BTIM_SFRmap* BTIMx, uint32_t CounterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_COUNTER_MODE(CounterMode));

    /*-------------------- 设置TXCTL1寄存器TXCMS位 --------------------*/
    BTIMx->CTL1 = SFR_Config (BTIMx->CTL1,
                          ~BTIM_CTL1_TXCMS,
                          CounterMode);
}

/**
  * 描述  更新定时器工作时钟。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewClock: 新的工作时钟，
  *                 取值范围为：
  *                   BTIM_SCLK: 选用SCLK时钟
  *                   BTIM_HFCLK: 选用HFCLK时钟
  *                   BTIM_LFCLK: 选用LFCLK时钟
  * 返回  无
  */
void
BTIM_Clock_Config (BTIM_SFRmap* BTIMx, uint32_t NewClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_CLOCK_CONFIG(NewClock));

    /*-------------------- 设置TXCTL1寄存器TXCLK位 --------------------*/
    BTIMx->CTL1 = SFR_Config (BTIMx->CTL1,
                          ~BTIM_CTL1_TXCLK,
                          NewClock);
}

/**
  * 描述  更新定时器计数模式外部触发脉冲输入同步控制位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewClock: 新的定时器计数模式外部触发脉冲输入同步控制位使能状态，
  *                 取值范围为：
  *                   BTIM_EX_SYNC_MODE: 与外部触发脉冲输入同步
  *                   BTIM_NO_SYNC_MODE: 不与外部触发脉冲输入同步
  * 返回  无
  */
void
BTIM_External_Pulse_Sync_Config (BTIM_SFRmap* BTIMx, uint32_t PulseSync)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_SYNC_MODE(PulseSync));

    /*-------------------- 设置TXCTL1寄存器TXSY位 --------------------*/
    if (PulseSync != BTIM_EX_SYNC_MODE)
    {
        /* 不与外部触发脉冲输入同步 */
        SFR_SET_BIT_ASM(BTIMx->CTL1, BTIM_CTL1_TXSY_POS);
    }
    else
    {
        /* 与外部触发脉冲输入同步 */
        SFR_CLR_BIT_ASM(BTIMx->CTL1, BTIM_CTL1_TXSY_POS);
    }
}

/**
  * 描述  更新定时/计数模式选择。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 新的定时/计数模式，
  *                 取值范围为：
  *                   BTIM_TIMER_MODE: 定时模式
  *                   BTIM_COUNTER_MODE: 计数模式
  * 返回  无
  */
void
BTIM_Work_Mode_Config (BTIM_SFRmap* BTIMx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_WORK_MODE(NewState));

    /*-------------------- 设置TXCTL1寄存器TXCS位 --------------------*/
    if (NewState != BTIM_TIMER_MODE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(BTIMx->CTL1, BTIM_CTL1_TXCS_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(BTIMx->CTL1, BTIM_CTL1_TXCS_POS);
    }
}

/**
  * 描述  更新产生触发事件配置位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 定时器使能控制状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Generate_Trigger_Config (BTIM_SFRmap* BTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXTRG位 --------------------*/
    if (NewState != FALSE)
    {
        /* 产生触发响应 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXTRG_POS);
    }
    else
    {
        /* 无动作 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXTRG_POS);
    }
}

/**
  * 描述  配置单脉冲输出模式。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 单脉冲输出模式使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Single_Pulse_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器PXSPM位 --------------------*/
    if (NewState != FALSE)
    {
        /* 单脉冲输出模式使能 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_PXSPM_POS);
    }
    else
    {
        /* 单脉冲输出模式未使能 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_PXSPM_POS);
    }
}

/**
  * 描述  配置单脉冲输出模式。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 单脉冲输出模式选择，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Single_Pulse_Shut_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器PXSPMST位 --------------------*/
    if (NewState != FALSE)
    {
        /* 单脉冲输出后关闭定时器使能位 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_PXSPMST_POS);
    }
    else
    {
        /* 单脉冲输出后不关闭定时器使能位 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_PXSPMST_POS);
    }
}

/**
  * 描述  配置立即更新控制位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 立即更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Updata_Immediately_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXUR位 --------------------*/
    if (NewState != FALSE)
    {
        /* 立即更新使能 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXUR_POS);
    }
    else
    {
        /* 立即更新未使能 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXUR_POS);
    }
}

/**
  * 描述  配置主从模式同步位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 主从模式同步位状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Master_Slave_Snyc_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXMSSYNC位 --------------------*/
    if (NewState != FALSE)
    {
        /* 立即更新使能 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXMSSYNC_POS);
    }
    else
    {
        /* 立即更新未使能 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXMSSYNC_POS);
    }
}

/**
  * 描述  配置触发选择位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       TriggerSelect: 触发选择，
  *                      取值范围为：
  *                         BTIM_TRIGGER_T1
  *                         BTIM_TRIGGER_T2
  *                         BTIM_TRIGGER_T3
  *                         BTIM_TRIGGER_T4
  *                         BTIM_TRIGGER_T5
  *                         BTIM_TRIGGER_T9
  *                         BTIM_TRIGGER_T14
  *                         BTIM_TRIGGER_T15
  *                         BTIM_TRIGGER_T18
  *                         BTIM_TRIGGER_T19
  *                         BTIM_TRIGGER_T20
  *                         BTIM_TRIGGER_T21
  *                         BTIM_TRIGGER_TXCK
  * 返回  无
  */
void
BTIM_Trigger_Select_Config (BTIM_SFRmap* BTIMx,
                    uint32_t TriggerSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_TRIGGER_MODE(TriggerSelect));

    /*-------------------- 设置TXCTL2寄存器TXTS位 --------------------*/
    BTIMx->CTL2 = SFR_Config (BTIMx->CTL2,
                          ~BTIM_CTL2_TXTS,
                          TriggerSelect);
}

/**
  * 描述  配置从模式选择位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       SlaveSelect: 从模式选择，
  *                    取值范围为：
  *                      BTIM_SLAVE_FORBIDDEN_MODE: 从模式禁止
  *                      BTIM_SLAVE_TRIGGER_MODE: 触发模式
  *                      BTIM_SLAVE_GATED_MODE: 门控模式
  *                      BTIM_SLAVE_RESET_MODE: 复位模式
  *                      BTIM_SLAVE_COUNTER_MODE: 计数模式2
  * 返回  无
  */
void
BTIM_Slave_Mode_Config (BTIM_SFRmap* BTIMx,
                    uint32_t SlaveMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_SLAVE_MODE(SlaveMode));

    /*-------------------- 设置TXCTL2寄存器TXSMS位 --------------------*/
    BTIMx->CTL2 = SFR_Config (BTIMx->CTL2,
                          ~BTIM_CTL2_TXSMS,
                          SlaveMode);
}

/**
  * 描述  配置主模式选择位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       MasterMode: 主模式选择，
  *                   取值范围为：
  *                     BTIM_MASTER_UR_SIGNAL: UR位作为触发
  *                     BTIM_MASTER_EN_SIGNAL: TXEN作为触发
  *                     BTIM_MASTER_IF_SIGNAL: TXIF作为触发
  * 返回  无
  */
void
BTIM_Master_Mode_Config (BTIM_SFRmap* BTIMx,
                    uint32_t MasterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_BTIM_MASTER_MODE(MasterMode));

    /*-------------------- 设置TXCTL2寄存器TXMMS位 --------------------*/
    BTIMx->CTL2 = SFR_Config (BTIMx->CTL2,
                          ~BTIM_CTL2_TXMMS,
                          MasterMode);
}

/**
  * 描述  配置上升沿更新事件控制位。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 更新事件控制状态，
  *                 取值范围为：
  *                   TRUE: 上升沿立即更新
  *                   FALSE: 每周期更新
  * 返回  无
  */
void
BTIM_Updata_Rising_Edge_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXMSSYNC位 --------------------*/
    if (NewState != FALSE)
    {
        /* 上升沿立即更新 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXUDEVT_POS);
    }
    else
    {
        /* 每周期更新 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXUDEVT_POS);
    }
}

/**
  * 描述  配置更新使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 更新使能状态，
  *                 取值范围为：
  *                   TRUE: 允许更新
  *                   FALSE: 禁止更新
  * 返回  无
  */
void
BTIM_Updata_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXUDEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 上升沿立即更新 */
        SFR_SET_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXUDEN_POS);
    }
    else
    {
        /* 每周期更新 */
        SFR_CLR_BIT_ASM(BTIMx->CTL2, BTIM_CTL2_TXUDEN_POS);
    }
}

/**
  * 描述  读TX计数方向。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  TX计数方向，0：向下，1：向上。
  */
DIRStatus
BTIM_Get_Direction (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 读取TXCTL1寄存器TXDIR位 --------------------*/
    if (BTIMx->CTL1 & BTIM_CTL1_TXDIR)
    {
        /* 当前计数方向为向上计数 */
        return DIR_UP;
    }
    else
    {
        /* 当前计数方向为向下计数 */
        return DIR_DOWN;
    }
}

/**
  * 描述  读定时器计数值。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  Tx计数值，16位
  */
uint16_t
BTIM_Get_Counter (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /* 读定时器计数值 */
    return BTIMx->CNT;
}

/**
  * 描述  读定时器周期值。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  Tx周期值，16位
  */
uint16_t
BTIM_Get_Period (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /* 读定时器计数值 */
    return BTIMx->PPX;
}

/**
  * 描述  读定时器预分频值。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  Tx预分频值，16位
  */
uint16_t
BTIM_Get_Prescaler (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /* 读定时器计数值 */
    return BTIMx->PRSC;
}

/**
  *   ##### 基本定时器(BTIM)初始化及配置函数定义结束 #####
  */


/**
  *   ##### 基本定时器(BTIM)中断及标志管理函数定义 #####
  */
/**
  * 描述  配置触发事件的DMA请求使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 触发事件的DMA请求，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Trigger_DMA_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXDIER寄存器TXTDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许触发事件的DMA请求 */
        SFR_SET_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXTDE_POS);
    }
    else
    {
        /* 禁止触发事件的DMA请求 */
        SFR_CLR_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXTDE_POS);
    }
}

/**
  * 描述  配置更新事件的DMA请求使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: 更新事件的DMA请求，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Updata_DMA_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXDIER寄存器TXUDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许更新事件的DMA请求 */
        SFR_SET_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXUDE_POS);
    }
    else
    {
        /* 禁止更新事件的DMA请求 */
        SFR_CLR_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXUDE_POS);
    }
}

/**
  * 描述  配置Tx计数溢出中断使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: Tx计数溢出中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Overflow_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXDIER寄存器TXIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx计数溢出引起中断 */
        SFR_SET_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXIE_POS);
    }
    else
    {
        /* 禁止Tx计数溢出引起中断 */
        SFR_CLR_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXIE_POS);
    }
}

/**
  * 描述  配置Tx触发事件中断使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: Tx触发事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Trigger_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXDIER寄存器TXTIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx触发事件引起中断 */
        SFR_SET_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXTIE_POS);
    }
    else
    {
        /* 禁止Tx触发事件引起中断 */
        SFR_CLR_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXTIE_POS);
    }
}

/**
  * 描述  配置Tx更新事件中断使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  *       NewState: Tx更新事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
BTIM_Updata_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXDIER寄存器TXUIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx更新事件引起中断 */
        SFR_SET_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXUIE_POS);
    }
    else
    {
        /* 禁止Tx更新事件引起中断 */
        SFR_CLR_BIT_ASM(BTIMx->DIER, BTIM_DIER_TXUIE_POS);
    }
}

/**
  * 描述  配置触发事件的DMA请求使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断使能状态，0：未使能，1：使能
  */
INTStatus
BTIM_Get_Trigger_DMA_INT_Status (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXDIER寄存器TXTDE位 --------------------*/
    if (BTIMx->DIER & BTIM_DIER_TXTDE)
    {
        /* 当前计数方向为向上计数 */
        return SET;
    }
    else
    {
        /* 当前计数方向为向下计数 */
        return RESET;
    }
}

/**
  * 描述  配置更新事件的DMA请求使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断使能状态，0：未使能，1：使能
  */
INTStatus
BTIM_Get_Updata_DMA_INT_Status (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXDIER寄存器TXUDE位 --------------------*/
    if (BTIMx->DIER & BTIM_DIER_TXUDE)
    {
        /* 当前计数方向为向上计数 */
        return SET;
    }
    else
    {
        /* 当前计数方向为向下计数 */
        return RESET;
    }
}

/**
  * 描述  配置Tx计数溢出中断使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断使能状态，0：未使能，1：使能
  */
INTStatus
BTIM_Get_Overflow_INT_Status (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXDIER寄存器TXIE位 --------------------*/
    if (BTIMx->DIER & BTIM_DIER_TXIE)
    {
        /* 当前计数方向为向上计数 */
        return SET;
    }
    else
    {
        /* 当前计数方向为向下计数 */
        return RESET;
    }
}

/**
  * 描述  配置Tx触发事件中断使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断使能状态，0：未使能，1：使能
  */
INTStatus
BTIM_Get_Trigger_INT_Status (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXDIER寄存器TXTIE位 --------------------*/
    if (BTIMx->DIER & BTIM_DIER_TXTIE)
    {
        /* 当前计数方向为向上计数 */
        return SET;
    }
    else
    {
        /* 当前计数方向为向下计数 */
        return RESET;
    }
}

/**
  * 描述  配置Tx更新事件中断使能。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断使能状态，0：未使能，1：使能
  */
INTStatus
BTIM_Get_Updata_INT_Status (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXDIER寄存器TXUIE位 --------------------*/
    if (BTIMx->DIER & BTIM_DIER_TXUIE)
    {
        /* 当前计数方向为向上计数 */
        return SET;
    }
    else
    {
        /* 当前计数方向为向下计数 */
        return RESET;
    }
}

/**
  * 描述  获取触发事件触发DMA中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
BTIM_Get_Trigger_DMA_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSR寄存器TXTDF位 --------------------*/
    if (BTIMx->SR & BTIM_SR_TXTDF)
    {
        /* 发生触发事件触发DMA中断 */
        return SET;
    }
    else
    {
        /* 未发生触发事件触发DMA中断 */
        return RESET;
    }
}

/**
  * 描述  获取更新事件触发DMA中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
BTIM_Get_Updata_DMA_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSR寄存器TXUDF位 --------------------*/
    if (BTIMx->SR & BTIM_SR_TXUDF)
    {
        /* 发生更新事件触发DMA中断 */
        return SET;
    }
    else
    {
        /* 未发生更新事件触发DMA中断 */
        return RESET;
    }
}

/**
  * 描述  获取Tx计数溢出中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
BTIM_Get_Overflow_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSR寄存器TXIF位 --------------------*/
    if (BTIMx->SR & BTIM_SR_TXIF)
    {
        /* 发生Tx计数溢出中断 */
        return SET;
    }
    else
    {
        /* 未发生Tx计数溢出中断 */
        return RESET;
    }
}

/**
  * 描述  获取Tx触发事件中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
BTIM_Get_Trigger_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSR寄存器TXTIF位 --------------------*/
    if (BTIMx->SR & BTIM_SR_TXTIF)
    {
        /* 发生Tx触发事件中断 */
        return SET;
    }
    else
    {
        /* 未发生Tx触发事件中断 */
        return RESET;
    }
}

/**
  * 描述  获取Tx更新事件中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
BTIM_Get_Updata_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSR寄存器TXUIF位 --------------------*/
    if (BTIMx->SR & BTIM_SR_TXUIF)
    {
        /* 发生Tx更新事件中断 */
        return SET;
    }
    else
    {
        /* 未发生Tx更新事件中断 */
        return RESET;
    }
}

/**
  * 描述  清除Tx计数溢出中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  无
  */
void
BTIM_Clear_Overflow_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSRIC寄存器TXIC位 --------------------*/
    SFR_SET_BIT_ASM(BTIMx->SRIC, BTIM_SRIC_TXIC_POS);
    while((BTIMx->SR & BTIM_SR_TXIF)>>BTIM_SR_TXIF_POS);
    SFR_CLR_BIT_ASM(BTIMx->SRIC, BTIM_SRIC_TXIC_POS);
}

/**
  * 描述  清除Tx触发事件中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  无
  */
void
BTIM_Clear_Trigger_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSRIC寄存器TXTIC位 --------------------*/
    SFR_SET_BIT_ASM(BTIMx->SRIC, BTIM_SRIC_TXTIC_POS);
    while((BTIMx->SR & BTIM_SR_TXTIF)>>BTIM_SR_TXTIF_POS);
    SFR_CLR_BIT_ASM(BTIMx->SRIC, BTIM_SRIC_TXTIC_POS);
}

/**
  * 描述  清除Tx更新事件中断标志。
  * 输入  BTIMx: 指向定时器内存结构的指针，取值T14_SFR、T15_SFR。
  * 返回  无
  */
void
BTIM_Clear_Updata_INT_Flag (BTIM_SFRmap* BTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_BTIM_PERIPH(BTIMx));

    /*-------------------- 设置TXSRIC寄存器TXUIC位 --------------------*/
    SFR_SET_BIT_ASM(BTIMx->SRIC, BTIM_SRIC_TXUIC_POS);
    while((BTIMx->SR & BTIM_SR_TXUIF)>>BTIM_SR_TXUIF_POS);
    SFR_CLR_BIT_ASM(BTIMx->SRIC, BTIM_SRIC_TXUIC_POS);
}
/**
  *   ##### 基本定时器(BTIM)中断及标志管理函数定义结束 #####
  */

