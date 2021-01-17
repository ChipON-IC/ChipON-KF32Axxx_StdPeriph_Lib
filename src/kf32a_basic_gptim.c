/**
  ******************************************************************************
  * 文件名  kf32a_basic_gptim.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了通用定时器外设功能函数，包含：
  *          + 通用定时器(GPTIM)初始化及配置函数定义
  *          + 通用定时器(GPTIM)中断及标志管理函数定义
  *          + 通用捕捉/比较/PWM外设(CCP)捕捉功能函数定义
  *          + 通用捕捉/比较/PWM外设(CCP)中断及标志管理函数定义
  *
  *********************************************************************
  */

#include "kf32a_basic_tim.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* 定时器私有定义 ----------------------------------------------------*/
/* GPTIMx_CTL1寄存器配置掩码 */
#define GPTIM_CTL1_INIT_MASK            (GPTIM_CTL1_TXCLK \
                                       | GPTIM_CTL1_TXCMS \
                                       | GPTIM_CTL1_TXSY \
                                       | GPTIM_CTL1_TXCS)

/* GPTIMx_CTL2寄存器配置掩码 */
#define GPTIM_CTL2_INIT_MASK            (GPTIM_CTL2_TXMMS \
                                       | GPTIM_CTL2_TXSMS \
                                       | GPTIM_CTL2_TXMSSYNC)

/* CCPXCTL3寄存器捕捉模式配置掩码 */
#define CCP_CTL3_CAP_INIT_MASK          (CCP_CTL3_PXPWMI | CCP_CTL3_PXXORM)

/* CCPXCTL3寄存器PWM配置掩码 */
#define CCP_CTL3_PWM_INIT_MASK          (CCP_CTL3_PXSPM | CCP_CTL3_PXSPMST)

/* CCPXCY寄存器偏移地址 */
#define CCP_CY_OFFSET                   ((uint32_t)0x20)

/* CCPXRY寄存器偏移地址 */
#define CCP_RY_OFFSET                   ((uint32_t)0x44)

/**
  *   ##### 通用定时器(GPTIM)初始化及配置函数定义 #####
  */
/**
  * 描述  通用定时器(GPTIM)配置。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       gptimInitStruct: 通用定时器配置信息结构体指针。
  * 返回  无
  */
void GPTIM_Configuration(GPTIM_SFRmap* GPTIMx,
                    GPTIM_InitTypeDef* gptimInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_COUNTER_MODE(gptimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_GPTIM_CLOCK_CONFIG(gptimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_GPTIM_WORK_MODE(gptimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_GPTIM_MASTER_MODE(gptimInitStruct->m_MasterMode));
    CHECK_RESTRICTION(CHECK_GPTIM_SLAVE_MODE(gptimInitStruct->m_SlaveMode));
    CHECK_RESTRICTION(CHECK_GPTIM_SYNC_MODE(gptimInitStruct->m_EXPulseSync));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(gptimInitStruct->m_MasterSlaveSync));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- 设置TxCNT寄存器 ----------------*/
        GPTIMx->CNT = gptimInitStruct->m_Counter;

        /*--------------- 设置PPx周期寄存器 ---------------*/
        GPTIMx->PPX = gptimInitStruct->m_Period;

        /*-------------- 设置Tx预分频寄存器 ---------------*/
        GPTIMx->PRSC = gptimInitStruct->m_Prescaler;
    }
    else
    {
        /*---------------- 设置TxCNT寄存器 ----------------*/
        GPTIMx->CNT = (uint16_t)gptimInitStruct->m_Counter;

        /*--------------- 设置PPx周期寄存器 ---------------*/
        GPTIMx->PPX = (uint16_t)gptimInitStruct->m_Period;

        /*-------------- 设置Tx预分频寄存器 ---------------*/
        GPTIMx->PRSC = (uint16_t)gptimInitStruct->m_Prescaler;
    }

    /*------------------ 设置TXCTL1寄存器 -----------------*/
    /* 根据结构体成员m_CounterMode，设置TXCMS位域 */
    /* 根据结构体成员m_Clock，设置TXCLK位域 */
    /* 根据结构体成员m_WorkMode，设置TXCS位域 */
    /* 根据结构体成员m_EXPulseSync，设置TXSY位域 */
    tmpreg = ((uint32_t)gptimInitStruct->m_CounterMode) | \
             ((uint32_t)gptimInitStruct->m_Clock) | \
             ((uint32_t)gptimInitStruct->m_WorkMode) | \
             ((uint32_t)gptimInitStruct->m_EXPulseSync);
    GPTIMx->CTL1 = SFR_Config (GPTIMx->CTL1,
                          ~GPTIM_CTL1_INIT_MASK,
                          tmpreg);

    /*-------------------- 设置TXCTL2寄存器 --------------------*/
    /* 根据结构体成员m_MasterMode，设置TXMMS位域 */
    /* 根据结构体成员m_SlaveMode，设置TXSMS位域 */
    /* 根据结构体成员m_MasterSlaveSync，设置TXMSSYNC位域 */
    tmpreg = ((uint32_t)gptimInitStruct->m_MasterMode) | \
             ((uint32_t)gptimInitStruct->m_SlaveMode) | \
             ((uint32_t)gptimInitStruct->m_MasterSlaveSync << GPTIM_CTL2_TXMSSYNC_POS);
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化通用定时器配置信息结构体。
  * 输入  gptimInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
GPTIM_Struct_Init (GPTIM_InitTypeDef* gptimInitStruct)
{
    /* 设置Tx计数值 */
    gptimInitStruct->m_Counter = 0x00000000;
    /* 设置Tx周期值 */
    gptimInitStruct->m_Period = 0xFFFFFFFF;
    /* 设置Tx预分频值 */
    gptimInitStruct->m_Prescaler = 0x00000000;
    /* 设置Tx计数模式 */
    gptimInitStruct->m_CounterMode = GPTIM_COUNT_UP_OF;
    /* 设置Tx工作时钟 */
    gptimInitStruct->m_Clock = GPTIM_SCLK;
    /* 设置定时/计数模式选择 */
    gptimInitStruct->m_WorkMode = GPTIM_TIMER_MODE;
    /* 设置主模式选择 */
    gptimInitStruct->m_MasterMode = GPTIM_MASTER_UR_SIGNAL;
    /* 设置从模式选择 */
    gptimInitStruct->m_SlaveMode = GPTIM_SLAVE_FORBIDDEN_MODE;
    /* 设置Tx计数模式外部触发脉冲输入同步控制 */
    gptimInitStruct->m_EXPulseSync = GPTIM_EX_SYNC_MODE;
    /* 设置主从模式同步控制 */
    gptimInitStruct->m_MasterSlaveSync = FALSE;
}

/**
  * 描述  定时器启动控制使能。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewState: 定时器使能控制，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Cmd (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL1寄存器TXEN位 --------------------*/
    if (NewState != FALSE)
    {

        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXEN_POS);
    }
    else
    {

        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXEN_POS);
    }
}

/**
  * 描述  更新定时器计数值。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       Counter: 新的计数值，取值16位数据。
  * 返回  无
  */
void
GPTIM_Set_Counter (GPTIM_SFRmap* GPTIMx, uint32_t Counter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- 更新TxCNT寄存器 ----------------*/
        GPTIMx->CNT = Counter;
    }
    else
    {
        /*---------------- 更新TxCNT寄存器 ----------------*/
        GPTIMx->CNT = (uint16_t)Counter;
    }
}

/**
  * 描述  更新定时器周期值。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       Period: 新的周期值，取值16位数据。
  * 返回  无
  */
void
GPTIM_Set_Period (GPTIM_SFRmap* GPTIMx, uint32_t Period)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- 更新TxPPX寄存器 ----------------*/
        GPTIMx->PPX = Period;
    }
    else
    {
        /*---------------- 更新TxPPX寄存器 ----------------*/
        GPTIMx->PPX = (uint16_t)Period;
    }
}

/**
  * 描述  更新定时器预分频值。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       Prescaler: 新的预分频，取值32位或16位数据。
  * 返回  无
  */
void
GPTIM_Set_Prescaler (GPTIM_SFRmap* GPTIMx, uint32_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /*---------------- 更新TxPRSC寄存器 ----------------*/
        GPTIMx->PRSC = Prescaler;
    }
    else
    {
        /*---------------- 更新TxPRSC寄存器 ----------------*/
        GPTIMx->PRSC = (uint16_t)Prescaler;
    }
}

/**
  * 描述  更新定时器计数模式。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       CounterMode: 新的计数模式，
  *                    取值范围为：
  *                      GPTIM_COUNT_DOWN_UF: 向下计数,下溢产生中断标志
  *                      GPTIM_COUNT_UP_OF: 向上计数,上溢产生中断标志
  *                      GPTIM_COUNT_UP_DOWN_OF: 向上-向下计数,上溢产生中断标志
  *                      GPTIM_COUNT_UP_DOWN_UF: 向上-向下计数,下溢产生中断标志
  *                      GPTIM_COUNT_UP_DOWN_OUF: 向上-向下计数,上溢和下溢产生中断标志
  * 返回  无
  */
void
GPTIM_Counter_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t CounterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_COUNTER_MODE(CounterMode));

    /*-------------------- 设置TXCTL1寄存器TXCMS位 --------------------*/
    GPTIMx->CTL1 = SFR_Config (GPTIMx->CTL1,
                          ~GPTIM_CTL1_TXCMS,
                          CounterMode);
}

/**
  * 描述  更新定时器工作时钟。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewClock: 新的工作时钟，
  *                 取值范围为：
  *                   GPTIM_SCLK: 选用SCLK时钟
  *                   GPTIM_HFCLK: 选用HFCLK时钟
  *                   GPTIM_LFCLK: 选用LFCLK时钟
  * 返回  无
  */
void
GPTIM_Clock_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_CLOCK_CONFIG(NewClock));

    /*-------------------- 设置TXCTL1寄存器TXCLK位 --------------------*/
    GPTIMx->CTL1 = SFR_Config (GPTIMx->CTL1,
                          ~GPTIM_CTL1_TXCLK,
                          NewClock);
}

/**
  * 描述  更新定时器计数模式外部触发脉冲输入同步控制位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewClock: 新的定时器计数模式外部触发脉冲输入同步控制位使能状态，
  *                 取值范围为：
  *                   GPTIM_EX_SYNC_MODE: 与外部触发脉冲输入同步
  *                   GPTIM_NO_SYNC_MODE: 不与外部触发脉冲输入同步
  * 返回  无
  */
void
GPTIM_External_Pulse_Sync_Config (GPTIM_SFRmap* GPTIMx, uint32_t PulseSync)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_SYNC_MODE(PulseSync));

    /*-------------------- 设置TXCTL1寄存器TXSY位 --------------------*/
    if (PulseSync != GPTIM_EX_SYNC_MODE)
    {
        /* 不与外部触发脉冲输入同步 */
        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXSY_POS);
    }
    else
    {
        /* 与外部触发脉冲输入同步 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXSY_POS);
    }
}

/**
  * 描述  更新定时/计数模式选择。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewState: 新的定时/计数模式，
  *                 取值范围为：
  *                   GPTIM_TIMER_MODE: 定时模式
  *                   GPTIM_COUNTER_MODE: 计数模式
  * 返回  无
  */
void
GPTIM_Work_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_WORK_MODE(NewState));

    /*-------------------- 设置TXCTL1寄存器TXCS位 --------------------*/
    if (NewState != GPTIM_TIMER_MODE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXCS_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXCS_POS);
    }
}

/**
  * 描述  配置立即更新控制位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewState: 立即更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Updata_Immediately_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXUR位 --------------------*/
    if (NewState != FALSE)
    {
        /* 立即更新使能 */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUR_POS);
    }
    else
    {
        /* 立即更新未使能 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUR_POS);
    }
}

/**
  * 描述  配置主从模式同步位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewState: 主从模式同步位状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Master_Slave_Snyc_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXMSSYNC位 --------------------*/
    if (NewState != FALSE)
    {
        /* 立即更新使能 */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXMSSYNC_POS);
    }
    else
    {
        /* 立即更新未使能 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXMSSYNC_POS);
    }
}

/**
  * 描述  配置触发选择位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       TriggerSelect: 触发选择，
  *                      取值范围为：
  *                         GPTIM_TRIGGER_T1
  *                         GPTIM_TRIGGER_T2
  *                         GPTIM_TRIGGER_T3
  *                         GPTIM_TRIGGER_T4
  *                         GPTIM_TRIGGER_T5
  *                         GPTIM_TRIGGER_T9
  *                         GPTIM_TRIGGER_T14
  *                         GPTIM_TRIGGER_T15
  *                         GPTIM_TRIGGER_T18
  *                         GPTIM_TRIGGER_T19
  *                         GPTIM_TRIGGER_T20
  *                         GPTIM_TRIGGER_T21
  *                         GPTIM_TRIGGER_TXCK
  *                         GPTIM_TRIGGER_CCPXCH1
  *                         GPTIM_TRIGGER_CCPXCH2
  *                         GPTIM_TRIGGER_CCPXCH3
  * 返回  无
  */
void
GPTIM_Trigger_Select_Config (GPTIM_SFRmap* GPTIMx,
                    uint32_t TriggerSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_TRIGGER_MODE(TriggerSelect));

    /*-------------------- 设置TXCTL2寄存器TXTS位 --------------------*/
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_TXTS,
                          TriggerSelect);
}

/**
  * 描述  配置从模式选择位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       SlaveSelect: 从模式选择，
  *                    取值范围为：
  *                      GPTIM_SLAVE_FORBIDDEN_MODE: 从模式禁止
  *                      GPTIM_SLAVE_TRIGGER_MODE: 触发模式
  *                      GPTIM_SLAVE_GATED_MODE: 门控模式
  *                      GPTIM_SLAVE_RESET_MODE: 复位模式
  *                      GPTIM_SLAVE_COUNTER_MODE: 计数模式2
  * 返回  无
  */
void
GPTIM_Slave_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t SlaveMode)
{
   /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_SLAVE_MODE(SlaveMode));

    /*-------------------- 设置TXCTL2寄存器TXSMS位 --------------------*/
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_TXSMS,
                          SlaveMode);
}

/**
  * 描述  配置主模式选择位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       MasterMode: 主模式选择，
  *                   取值范围为：
  *                     GPTIM_MASTER_UR_SIGNAL: UR位作为触发
  *                     GPTIM_MASTER_EN_SIGNAL: TXEN作为触发
  *                     GPTIM_MASTER_TXIF_SIGNAL: TXIF作为触发
  *                     GPTIM_MASTER_CCPXCH1IF_SIGNAL: CCPxCH1IF脉冲作为触发
  *                     GPTIM_MASTER_CCPXCH1_SIGNAL: CCPxCH1 作为触发
  *                     GPTIM_MASTER_CCPXCH2_SIGNAL: CCPxCH2 作为触发
  *                     GPTIM_MASTER_CCPXCH3_SIGNAL: CCPxCH3 作为触发
  *                     GPTIM_MASTER_CCPXCH4_SIGNAL: CCPxCH4 作为触发
  * 返回  无
  */
void
GPTIM_Master_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t MasterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_GPTIM_MASTER_MODE(MasterMode));

    /*-------------------- 设置TXCTL2寄存器TXMMS位 --------------------*/
    GPTIMx->CTL2 = SFR_Config (GPTIMx->CTL2,
                          ~GPTIM_CTL2_TXMMS,
                          MasterMode);
}

/**
  * 描述  配置上升沿更新事件控制位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewState: 更新事件控制状态，
  *                 取值范围为：
  *                   TRUE: 上升沿立即更新
  *                   FALSE: 每周期更新
  * 返回  无
  */
void
GPTIM_Updata_Rising_Edge_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXUDEVT位 --------------------*/
    if (NewState != FALSE)
    {
        /* 上升沿立即更新 */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEVT_POS);
    }
    else
    {
        /* 每周期更新 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEVT_POS);
    }
}

/**
  * 描述  配置更新使能。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  *       NewState: 更新使能状态，
  *                 取值范围为：
  *                   TRUE: 允许更新
  *                   FALSE: 禁止更新
  * 返回  无
  */
void
GPTIM_Updata_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器TXUDEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 上升沿立即更新 */
        SFR_SET_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEN_POS);
    }
    else
    {
        /* 每周期更新 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL2, GPTIM_CTL2_TXUDEN_POS);
    }
}

/**
  * 描述  允许触发事件的DMA请求配置
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 触发事件的DMA请求使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Trigger_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器TXTDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许触发事件的DMA请求 */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTDE_POS);
    }
    else
    {
        /* 禁止触发事件的DMA请求 */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTDE_POS);
    }
}

/**
  * 描述  配置更新事件的DMA请求使能。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 更新事件的DMA请求，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Updata_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器TXUDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许更新事件的DMA请求 */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUDE_POS);
    }
    else
    {
        /* 禁止更新事件的DMA请求 */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUDE_POS);
    }
}

/**
  * 描述  配置Tx更新事件中断使能。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: Tx更新事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Updata_INT_Enable (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器TXUIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx更新事件引起中断 */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUIE_POS);
    }
    else
    {
        /* 禁止Tx更新事件引起中断 */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXUIE_POS);
    }
}

/**
  * 描述  配置Tx触发事件中断使能。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: Tx触发事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Trigger_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器TXTIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx触发事件引起中断 */
        SFR_SET_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTIE_POS);
    }
    else
    {
        /* 禁止Tx触发事件引起中断 */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXCTL3, CCP_CTL3_TXTIE_POS);
    }
}

/**
  * 描述  产生触发事件配置位。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 定时器使能控制状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Generate_Trigger_Config (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXEGIF寄存器TXTRG位 --------------------*/
    if (NewState != FALSE)
    {
        /* 产生触发响应 */
        SFR_SET_BIT_ASM(GPTIMx->CCPXEGIF, CCP_EGIF_TXTRG_POS);
    }
    else
    {
        /* 无动作 */
        SFR_CLR_BIT_ASM(GPTIMx->CCPXEGIF, CCP_EGIF_TXTRG_POS);
    }
}

/**
  * 描述  读TX计数方向。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  * 返回  TX计数方向，0：向下，1：向上。
  */
DIRStatus
GPTIM_Get_Direction (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 读取TXCTL1寄存器TXDIR位 --------------------*/
    if (GPTIMx->CTL1 & GPTIM_CTL1_TXDIR)
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
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  * 返回  Tx计数值，32位
  */
uint32_t
GPTIM_Get_Counter (GPTIM_SFRmap* GPTIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /* 读定时器计数值TxCNT */
        tmpreg = GPTIMx->CNT;
    }
    else
    {
        /* 读定时器计数值TxCNT */
        tmpreg = (uint16_t)GPTIMx->CNT;
    }
    return tmpreg;
}

/**
  * 描述  读定时器周期值。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  * 返回  Tx周期值，32位
  */
uint32_t
GPTIM_Get_Period (GPTIM_SFRmap* GPTIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /* 读定时器周期值TxPPX */
        tmpreg = GPTIMx->PPX;
    }
    else
    {
        /* 读定时器周期值TxPPX */
        tmpreg = (uint16_t)GPTIMx->PPX;
    }
    return tmpreg;
}

/**
  * 描述  读定时器预分频值。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  * 返回  Tx预分频值，32位
  */
uint32_t
GPTIM_Get_Prescaler (GPTIM_SFRmap* GPTIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    if ((GPTIMx == T20_SFR) || (GPTIMx == T21_SFR))
    {
        /* 读定时器预分频值TxPRSC */
        tmpreg = GPTIMx->PRSC;
    }
    else
    {
        /* 读定时器预分频值TxPRSC */
        tmpreg = (uint16_t)GPTIMx->PRSC;
    }
    return tmpreg;
}
/**
  *   ##### 通用定时器(GPTIM)初始化及配置函数定义结束 #####
  */


/**
  *   ##### 通用定时器(GPTIM)中断及标志管理函数定义 #####
  */
/**
  * 描述  读取Tx计数溢出中断使能。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: Tx计数溢出中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
GPTIM_Overflow_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 读取TXCTL1寄存器TXIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx计数溢出引起中断 */
        SFR_SET_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXIE_POS);
    }
    else
    {
        /* 禁止Tx计数溢出引起中断 */
        SFR_CLR_BIT_ASM(GPTIMx->CTL1, GPTIM_CTL1_TXIE_POS);
    }
}

/**
  * 描述  清除Tx溢出中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  无
  */
void
GPTIM_Clear_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 设置CCPXSRIC寄存器TXIC位 --------------------*/
    SFR_SET_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXIC_POS);
    while((GPTIMx->CTL1 & GPTIM_CTL1_TXIF)>>GPTIM_CTL1_TXIF_POS);
    SFR_CLR_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXIC_POS);
}

/**
  * 描述  清除Tx更新事件中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  无
  */
void
GPTIM_Clear_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 设置CCPXSRIC寄存器TXUIC位 --------------------*/
    SFR_SET_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXUIC_POS);
    while((GPTIMx->CCPXEGIF & CCP_EGIF_TXUIF)>>CCP_EGIF_TXUIF_POS);
    SFR_CLR_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXUIC_POS);
}

/**
  * 描述  清除Tx触发事件中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  无
  */
void
GPTIM_Clear_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 设置CCPXSRIC寄存器TXTIC位 --------------------*/
    SFR_SET_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXTIC_POS);
    while((GPTIMx->CCPXEGIF & CCP_EGIF_TXTIF)>>CCP_EGIF_TXTIF_POS);
    SFR_CLR_BIT_ASM(GPTIMx->CCPXSRIC, CCP_SRIC_TXTIC_POS);
}

/**
  * 描述  读取Tx计数溢出中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
GPTIM_Get_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 读取TXCTL1寄存器TXIF位 --------------------*/
    if (GPTIMx->CTL1 & GPTIM_CTL1_TXIF)
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
  * 描述  读取Tx更新事件中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
GPTIM_Get_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 读取CCPXEGIF寄存器TXUIF位 --------------------*/
    if (GPTIMx->CCPXEGIF & CCP_EGIF_TXUIF)
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
  * 描述  读取Tx触发事件中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
GPTIM_Get_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 读取CCPXEGIF寄存器TXTIF位 --------------------*/
    if (GPTIMx->CCPXEGIF & CCP_EGIF_TXTIF)
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
  * 描述  读取Tx更新事件触发DMA中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断，DMA 响应后该位由硬件自动清零
  */
FlagStatus
GPTIM_Get_Updata_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 设置CCPXDF寄存器TXUDF位 --------------------*/
    if (GPTIMx->CCPXDF & CCP_DF_TXUDF)
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
  * 描述  配置触发事件触发DMA中断标志。
  * 输入  GPTIMx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断，DMA 响应后该位由硬件自动清零
  */
FlagStatus
GPTIM_Get_Trigger_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(GPTIMx));

    /*-------------------- 设置CCPXDF寄存器TXTDF位 --------------------*/
    if (GPTIMx->CCPXDF & CCP_DF_TXTDF)
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
  *   ##### 通用定时器(GPTIM)中断及标志管理函数定义结束 #####
  */


/**
  *   ##### 通用捕捉/比较/PWM外设(CCP)捕捉功能函数定义 #####
  */
/**
  * 描述  通用比较器(CCP)配置。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       ccpInitStruct: 通用比较器(CCP)配置信息结构体指针。
  * 返回  无
  */
void
CCP_Compare_Configuration(CCP_SFRmap* CCPx,
                    CCP_CompareInitTypeDef* ccpInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(ccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_CCP_CMP_MODE(ccpInitStruct->m_CompareMode));

    /*------------------ 设置CCPXCTL1寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_CompareMode，设置CCPxCHyM位域 */
    tmpreg = 4 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << tmpreg),
                          (uint32_t)ccpInitStruct->m_CompareMode << tmpreg);

    /*------------------ 设置CCPXRy寄存器 -----------------*/
    /* 根据结构体成员m_CompareValue，设置CCPXRy位域 */
    /* 获取CCPXRY寄存器地址 */
    tmpreg1 = (uint32_t)CCPx;
    tmpreg1 = tmpreg1 + CCP_RY_OFFSET + tmpreg;
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        *(volatile uint32_t*)tmpreg1
            = ccpInitStruct->m_CompareValue;
    }
    else
    {
        *(volatile uint32_t*)tmpreg1
            = (uint16_t)ccpInitStruct->m_CompareValue;
    }
}

/**
  * 描述  初始化通用比较器(CCP)配置信息结构体。
  * 输入  ccpInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
CCP_Compare_Struct_Init (CCP_CompareInitTypeDef* ccpInitStruct)
{
    /* 设置比较器通道 */
    ccpInitStruct->m_Channel = CCP_CHANNEL_1;
    /* 设置通道的模式选择位 */
    ccpInitStruct->m_CompareMode = CCP_MODE_RST;
    /* 设置比较值 */
    ccpInitStruct->m_CompareValue = 0x00000000;
}

/**
  * 描述  通用捕捉模块(CCP)配置。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       ccpInitStruct: 通用捕捉模块(CCP)配置信息结构体指针。
  * 返回  无
  */
void
CCP_Capture_Configuration (CCP_SFRmap* CCPx,
                    CCP_CaptureInitTypeDef* ccpInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(ccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_CCP_CAP_MODE(ccpInitStruct->m_CaptureMode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_PWMInput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_XORMode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_ChannelCompare4));

    /*------------------ 设置CCPXCTL1寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_CaptureMode，设置CCPxCHyM位域 */
    tmpreg = 4 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << tmpreg),
                          (uint32_t)ccpInitStruct->m_CaptureMode << tmpreg);

    /*------------------ 设置CCPXCTL2寄存器 -----------------*/
    /* 根据结构体成员m_ChannelCompare4，设置PxOC4CE位域 */
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~CCP_CTL2_PXOC4CE,
                          ccpInitStruct->m_ChannelCompare4);

    /*------------------ 设置CCPXCTL3寄存器 -----------------*/
    /* 根据结构体成员m_PWMInput，设置PxPWMI位域 */
    /* 根据结构体成员m_XORMode，设置PxXORM位域 */
    tmpreg = (ccpInitStruct->m_PWMInput << CCP_CTL3_PXPWMI_POS) \
             | (ccpInitStruct->m_XORMode << CCP_CTL3_PXXORM_POS);
    CCPx->CCPXCTL3 = SFR_Config (CCPx->CCPXCTL3,
                          ~CCP_CTL3_CAP_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化通用捕捉模块(CCP)配置信息结构体。
  * 输入  ccpInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
CCP_Capture_Struct_Init (CCP_CaptureInitTypeDef* ccpInitStruct)
{
    /* 设置捕捉通道 */
    ccpInitStruct->m_Channel = CCP_CHANNEL_1;
    /* 设置通道的模式选择位 */
    ccpInitStruct->m_CaptureMode = CCP_MODE_RST;
    /* 设置PWM测量模式 */
    ccpInitStruct->m_PWMInput = FALSE;
    /* 设置输入异或模式 */
    ccpInitStruct->m_XORMode = FALSE;
    /* 设置比较器的输出触发捕捉 */
    ccpInitStruct->m_ChannelCompare4 = FALSE;
}

/**
  * 描述  通用PWM模块(CCP)配置。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       ccpInitStruct: 通用PWM模块(CCP)配置信息结构体指针。
  * 返回  无
  */
void
CCP_PWM_Configuration (CCP_SFRmap* CCPx,
                    CCP_PWMInitTypeDef* ccpInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(ccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_CCP_PWM_MODE(ccpInitStruct->m_PwmMode));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL_OUTPUT(ccpInitStruct->m_OutputCtl));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_SinglePWM));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ccpInitStruct->m_CloseTimer));

    /*------------------ 设置CCPXCTL1寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_PwmMode，设置CCPxCHyM位域 */
    tmpreg = 4 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << tmpreg),
                          (uint32_t)ccpInitStruct->m_PwmMode << tmpreg);

    /*------------------ 设置CCPXRy寄存器 -----------------*/
    /* 根据结构体成员m_Channel，获取CCPXRY寄存器地址 */
    tmpreg1 = (uint32_t)CCPx;
    tmpreg1 = tmpreg1 + CCP_RY_OFFSET + tmpreg;
    /* 根据结构体成员m_DutyRatio，设置CCPXRy位域 */
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        *(volatile uint32_t*)tmpreg1
            = ccpInitStruct->m_DutyRatio;
    }
    else
    {
        *(volatile uint32_t*)tmpreg1
            = (uint16_t)ccpInitStruct->m_DutyRatio;
    }

    /*------------------ 设置CCPXCTL2寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_OutputCtl，设置PxOCy位域 */
    tmpreg = 2 * ccpInitStruct->m_Channel;
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXOC1 << tmpreg),
                          ccpInitStruct->m_OutputCtl << tmpreg);

    /*------------------ 设置CCPXCTL3寄存器 -----------------*/
    /* 根据结构体成员m_SinglePWM，设置PxSPM位域 */
    /* 根据结构体成员m_CloseTimer，设置PxSPMST位域 */
    tmpreg = (ccpInitStruct->m_SinglePWM << CCP_CTL3_PXSPM_POS) \
             | (ccpInitStruct->m_CloseTimer << CCP_CTL3_PXSPMST_POS);
    CCPx->CCPXCTL3 = SFR_Config (CCPx->CCPXCTL3,
                          ~CCP_CTL3_PWM_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化通用PWM模块(CCP)配置信息结构体。
  * 输入  ccpInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
CCP_PWM_Struct_Init (CCP_PWMInitTypeDef* ccpInitStruct)
{
    /* 设置PWM通道 */
    ccpInitStruct->m_Channel = CCP_CHANNEL_1;
    /* 设置通道的模式选择位 */
    ccpInitStruct->m_PwmMode = CCP_MODE_RST;
    /* 设置PWM占空比 */
    ccpInitStruct->m_DutyRatio = 0x00000000;
    /* 设置输出控制 */
    ccpInitStruct->m_OutputCtl = CCP_CHANNEL_OUTPUT_PWM_ACTIVE;
    /* 设置单脉冲输出模式 */
    ccpInitStruct->m_SinglePWM = FALSE;
    /* 设置单脉冲输出模式选择 */
    ccpInitStruct->m_CloseTimer = FALSE;
}

/**
  * 描述  配置CCP捕捉功能通道模式。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       EdgeConfig: 匹配边沿，取值范围为:
  *                     CCP_MODE_RST: 模式关闭
  *                     CCP_CAP_FALLING_EDGE: 在每个下降沿发生捕捉
  *                     CCP_CAP_RISING_EDGE: 在每个上升沿发生捕捉
  *                     CCP_CAP_4TH_RISING_EDGE: 每4个上升沿发生捕捉
  *                     CCP_CAP_16TH_RISING_EDGE: 每16个上升沿发生捕捉
  * 返回  无
  */
void
CCP_Capture_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_CAP_MODE(EdgeConfig));

    /*-------------------- 设置CCPXCTL1寄存器 --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * 描述  配置CCP比较功能通道模式。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       EdgeConfig: 匹配边沿，取值范围为:
  *                     CCP_MODE_RST: 模式关闭
  *                     CCP_CMP_ACTIVE_LEVEL: 比较匹配时输出高电平
  *                     CCP_CMP_INACTIVE_LEVEL: 比较匹配时输出低电平
  *                     CCP_CMP_SOFT_INT: 比较匹配时产生软件中断
  *                     CCP_CMP_SPECIAL_EVENT: 触发特殊事件
  * 返回  无
  */
void
CCP_Compare_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_CMP_MODE(EdgeConfig));

    /*-------------------- 设置CCPXCTL1寄存器 --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * 描述  配置CCP PWM功能通道模式。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       EdgeConfig: 匹配边沿，取值范围为:
  *                     CCP_MODE_RST: 模式关闭
  *                     CCP_PWM_MODE: PWM模式
  * 返回  无
  */
void
CCP_PWM_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_PWM_MODE(EdgeConfig));

    /*-------------------- 设置CCPXCTL1寄存器 --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    CCPx->CCPXCTL1 = SFR_Config (CCPx->CCPXCTL1,
                          ~(CCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * 描述  读取CCP捕捉寄存器。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  * 返回  捕捉寄存器的值。
  */
uint32_t
CCP_Get_Capture_Result (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /* 获取寄存器访问地址 */
    tmpreg = (uint32_t)CCPx;
    tmpreg = tmpreg + CCP_CY_OFFSET + (4 * Channel);

    /* 获取寄存器的值 */
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        return (*(volatile const uint32_t*)  tmpreg);
    }
    else
    {
        return (uint16_t)(*(volatile const uint32_t*)  tmpreg);
    }
}

/**
  * 描述  配置CCP比较/PWM占空比寄存器。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  * 返回  无。
  */
void
CCP_Set_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel, uint32_t Value)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /* 获取CCPXRY寄存器地址 */
    tmpreg = (uint32_t)CCPx;
    tmpreg = tmpreg + CCP_RY_OFFSET + (4 * Channel);

    /*-------------------- 设置CCPXRY寄存器 --------------------*/
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        *(volatile uint32_t*)tmpreg = Value;
    }
    else
    {
        *(volatile uint32_t*)tmpreg = (uint16_t)Value;
    }
}

/**
  * 描述  读取CCP比较/PWM占空比寄存器。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  * 返回  比较/PWM占空比寄存器的值。
  */
uint32_t
CCP_Get_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /* 获取寄存器访问地址 */
    tmpreg = (uint32_t)CCPx;
    tmpreg = tmpreg + CCP_RY_OFFSET + (4 * Channel);

    /* 获取寄存器的值 */
    if ((CCPx == CCP20_SFR) || (CCPx == CCP21_SFR))
    {
        return (*(volatile uint32_t*)  tmpreg);
    }
    else
    {
        return (uint16_t)(*(volatile uint32_t*)  tmpreg);
    }
}

/**
  * 描述  配置CCP输出比较/PWM信号清零使能位。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       NewState: CCP输出比较/PWM信号清零使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
CCP_Compare_PWM_Signal_Clear (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL2寄存器PXOCyCE位 --------------------*/
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXOC1CE << (Channel)),
                          (uint32_t)NewState << (CCP_CTL2_PXOC1CE_POS + Channel));
}

/**
  * 描述  配置CCP比较器检测选择。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       CompareSelect: 比较器检测选择，取值范围为:
  *                        CCP_COMPARE_SELECT_1: 检测比较器1的输出
  *                        CCP_COMPARE_SELECT_2: 检测比较器2的输出
  *                        CCP_COMPARE_SELECT_3: 检测比较器3的输出
  * 返回  无。
  */
void
CCP_Compare_Check_Config (CCP_SFRmap* CCPx, uint32_t CompareSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_COMPARE_SELECT(CompareSelect));

    /*-------------------- 设置CCPXCTL2寄存器PXASS位 --------------------*/
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXASS),
                          CompareSelect);
}

/**
  * 描述  配置CCP通道输出控制。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       ChannelOutputCtl: 比较器检测选择，取值范围为:
  *                           CCP_CHANNEL_OUTPUT_PWM_ACTIVE: PWM输出，高有效
  *                           CCP_CHANNEL_OUTPUT_PWM_INACTIVE: PWM输出，低有效
  *                           CCP_CHANNEL_OUTPUT_INACTIVE: 强制低电平输出
  *                           CCP_CHANNEL_OUTPUT_ACTIVE: 强制高电平输出
  * 返回  无。
  */
void
CCP_Channel_Output_Control (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t ChannelOutputCtl)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL_OUTPUT(ChannelOutputCtl));

    /*-------------------- 设置CCPXCTL2寄存器PXOCy位 --------------------*/
    tmpreg = ChannelOutputCtl << (Channel * 2);
    CCPx->CCPXCTL2 = SFR_Config (CCPx->CCPXCTL2,
                          ~(CCP_CTL2_PXOC1 << (Channel * 2)),
                          tmpreg);
}

/**
  * 描述  配置单脉冲输出模式。
  * 输入  CCPx: 指向定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 单脉冲输出模式选择，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CCP_Single_Pulse_Shut_Enable (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器PXSPMST位 --------------------*/
    if (NewState != FALSE)
    {
        /* 单脉冲输出后关闭定时器使能位 */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPMST_POS);
    }
    else
    {
        /* 单脉冲输出后不关闭定时器使能位 */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPMST_POS);
    }
}

/**
  * 描述  配置单脉冲输出模式。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 单脉冲输出模式使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CCP_Single_Pulse_Enable (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器PXSPM位 --------------------*/
    if (NewState != FALSE)
    {
        /* 单脉冲输出模式使能 */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPM_POS);
    }
    else
    {
        /* 单脉冲输出模式未使能 */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXSPM_POS);
    }
}

/**
  * 描述  PWM输入测量模式使能
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: PWM输入测量模式使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CCP_PWM_Input_Measurement_Config (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器PXPWMI位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能PWM输入测量模式 */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXPWMI_POS);
    }
    else
    {
        /* 未使能PWM输入测量模式 */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXPWMI_POS);
    }
}

/**
  * 描述  输入异或使能位配置
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T19_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 输入异或使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CCP_Input_XOR_Config (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL2寄存器PXXORM位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能PWM输入测量模式 */
        SFR_SET_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXXORM_POS);
    }
    else
    {
        /* 未使能PWM输入测量模式 */
        SFR_CLR_BIT_ASM(CCPx->CCPXCTL3, CCP_CTL3_PXXORM_POS);
    }
}

/**
  * 描述  捕获/比较通道的DMA请求配置。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       NewState: 捕获/比较通道的DMA请求使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
CCP_Channel_DMA_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器CCPXCCYDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许捕获/比较通道的DMA请求 */
        CCPx->CCPXCTL3 |= CCP_CTL3_CC1DE << Channel;
    }
    else
    {
        /* 禁止捕获/比较通道的DMA请求 */
        CCPx->CCPXCTL3 &= ~(CCP_CTL3_CC1DE << Channel);
    }
}

/**
  * 描述  产生捕捉/比较事件配置位。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       NewState: 产生捕捉/比较事件配置状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CCP_Generate_Trigger_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ 设置CCPXEGIF寄存器CCPXCCyG位 ------------------*/
    if (NewState != FALSE)
    {
        /* 在通道Channel上产生一个捕捉/比较事件 */
        CCPx->CCPXEGIF |= CCP_EGIF_CC1G << Channel;
    }
    else
    {
        /* 无动作 */
        CCPx->CCPXEGIF &= ~(CCP_EGIF_CC1G << Channel);
    }
}

/**
  * 描述  产生串口接收信号测量事件配置位。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       NewState: 产生串口接收信号测量使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CCP_USART_Receive_Config (CCP_SFRmap* CCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXEGIF寄存器USARTRXEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能测量串口接收信号 */
        SFR_SET_BIT_ASM(CCPx->CCPXEGIF, CCP_EGIF_UARTRXEN_POS);
    }
    else
    {
        /* 禁止测量串口接收信号 */
        SFR_CLR_BIT_ASM(CCPx->CCPXEGIF, CCP_EGIF_UARTRXEN_POS);
    }
}
/**
  *   ##### 通用捕捉/比较/PWM外设(CCP)捕捉功能函数定义结束 #####
  */


/**
  *   ##### 通用捕捉/比较/PWM外设(CCP)中断及标志管理函数定义 #####
  */
/**
  * 描述  读取CCP通道捕捉/比较中断标志。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
CCP_Get_Channel_Trigger_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));

    /*-------------------- 读取CCPXEGIF寄存器TXTIF位 --------------------*/
    tmpreg = CCP_EGIF_CC1IF << Channel;
    if (CCPx->CCPXEGIF & tmpreg)
    {
        /* 发生中断 */
        return SET;
    }
    else
    {
        /* 未发生中断 */
        return RESET;
    }
}

/**
  * 描述  读取CCP通道触发DMA中断标志。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  * 返回  中断状态，0：未发生中断，1：发生中断，DMA 响应后该位由硬件自动清零
  */
FlagStatus
CCP_Get_Trigger_DMA_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));

    /*-------------------- 读取CCPXDF寄存器CCyDF位 --------------------*/
    tmpreg = CCP_DF_CC1DF << Channel;
    if (CCPx->CCPXDF & tmpreg)
    {
        /*  CCP通道触发DMA */
        return SET;
    }
    else
    {
        /*  CCP通道无触发DMA */
        return RESET;
    }
}

/**
  * 描述  通道捕获/比较的中断使能配置。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  *       NewState: 通道捕获/比较的中断使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
CCP_Channel_INT_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CCPXCTL3寄存器CCPXCCYIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许CCP通道Channel捕捉/比较中断 */
        CCPx->CCPXCTL3 |= CCP_CTL3_CC1IE << Channel;
    }
    else
    {
        /* 禁止CCP通道Channel捕捉/比较中断 */
        CCPx->CCPXCTL3 &= ~(CCP_CTL3_CC1IE << Channel);
    }
}

/**
  * 描述  清除CCP通道捕获/比较的中断标志。
  * 输入  CCPx: 指向CCP或通用定时器内存结构的指针，
  *               取值T0_SFR/T1_SFR/T2_SFR/T3_SFR/T4_SFR/T18_SFR/
  *               T19_SFR/T20_SFR/T21_SFR/T22_SFR/T23_SFR，
  *               或CCP0_SFR/CCP1_SFR/CCP2_SFR/CCP3_SFR/CCP4_SFR/
  *               CCP18_SFR/CCP19_SFR/CCP20_SFR/CCP21_SFR/CCP22_SFR/CCP23_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  CCP_CHANNEL_1: 通道1
  *                  CCP_CHANNEL_2: 通道2
  *                  CCP_CHANNEL_3: 通道3
  *                  CCP_CHANNEL_4: 通道4
  * 返回  无。
  */
void
CCP_Clear_Channel_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPTIM_PERIPH(CCPx));
    CHECK_RESTRICTION(CHECK_CCP_CHANNEL(Channel));

    /*-------------------- 设置CCPXSRIC寄存器CCPXCCYIC位 --------------------*/
    tmpreg = CCP_EGIF_CC1IF << Channel;
    CCPx->CCPXSRIC |= CCP_SRIC_CC1IC << Channel;
    while((CCPx->CCPXEGIF & tmpreg)>>(Channel+CCP_EGIF_CC1IF_POS));
    CCPx->CCPXSRIC &= ~(CCP_SRIC_CC1IC << Channel);
}
/**
  *   ##### 通用捕捉/比较/PWM外设(CCP)中断及标志管理函数定义结束 #####
  */
