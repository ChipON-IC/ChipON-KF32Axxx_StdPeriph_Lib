/**
  ******************************************************************************
  * 文件名  kf32a_basic_qei.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了正交编码脉冲电路(QEI)相关的功能函数，包含：
  *          + 正交编码脉冲电路(QEI)初始化函数
  *          + 正交编码脉冲电路(QEI)功能配置函数
  *          + 正交编码脉冲电路(QEI)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_qei.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_qei
/* 定时器私有定义 ----------------------------------------------------*/
/* QEIx_CTL0寄存器定时器功能初始化掩码 */
#define QEI_CTL0_TIMER_INIT_MASK        (QEI_CTL0_TXDIR \
                                       | QEI_CTL0_TXCS)

/* QEIx_CTL0寄存器初始化掩码 */
#define QEI_CTL0_INIT_MASK              (QEI_CTL0_INDRES \
                                       | QEI_CTL0_TXDIROUT)

/* QEIx_CTL1寄存器初始化掩码 */
#define QEI_CTL1_INIT_MASK              (QEI_CTL1_QEICLK \
                                       | QEI_CTL1_QEIOUT \
                                       | QEI_CTL1_QEICK \
                                       | QEI_CTL1_SWPAB)

/**
  *   ##### 正交编码脉冲电路(QEI)初始化函数 #####
  */
/**
  * 描述  复位QEI外设，使能外设时钟。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  无。
  */
void
QEI_Reset (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

#ifdef KF32A_Periph_qei0
    if (QEIx == QEI0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_QEI0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_qei1
    if (QEIx == QEI1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_QEI1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_QEI1CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  QEI模块正交编码器初始化。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       qeiInitStruct: QEI模块正交编码器信息结构体。
  * 返回  无。
  */
void
QEI_Configuration(QEI_SFRmap* QEIx, QEI_InitTypeDef* qeiInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_IndexReset));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_DirectionEn));
    CHECK_RESTRICTION(CHECK_QEI_SOURCE(qeiInitStruct->m_WorkClock));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_DigitalFilterEn));
    CHECK_RESTRICTION(CHECK_QEI_DIGITAL_FILTER_CLK( \
                            qeiInitStruct->m_DigitalFilterPrescaler));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(qeiInitStruct->m_SwapABEn));

    /*---------------- 设置Tx_CNT寄存器 ----------------*/
    QEIx->CNT = (uint32_t)qeiInitStruct->m_Counter;

    /*---------------- 设置PPx周期寄存器 ---------------*/
    QEIx->PPX = (uint32_t)qeiInitStruct->m_Period;

    /*---------------- 设置Tx_PRSC预分频寄存器 ---------------*/
    QEIx->PRSC = (uint32_t)qeiInitStruct->m_Prescaler;

    /*---------------- 设置QEIx_CTL1寄存器 ----------------*/
    /* 根据结构体成员m_WorkClock，设置QEICLK位域 */
    /* 根据结构体成员m_DigitalFilterEn，设置QEIOUT位域 */
    /* 根据结构体成员m_DigitalFilterPrescaler，设置QEICK位域 */
    /* 根据结构体成员m_SwapABEn，设置SWPAB位域 */
    tmpreg = (qeiInitStruct->m_WorkClock) \
           | (qeiInitStruct->m_DigitalFilterEn << QEI_CTL1_QEIOUT_POS) \
           | (qeiInitStruct->m_DigitalFilterPrescaler) \
           | (qeiInitStruct->m_SwapABEn << QEI_CTL1_SWPAB_POS);
    QEIx->CTL1 = SFR_Config (QEIx->CTL1,
                          ~QEI_CTL1_INIT_MASK,
                          tmpreg);

    /*---------------- 设置QEIx_CTL0寄存器 ----------------*/
    /* 根据结构体成员m_IndexReset，设置INDRES位域 */
    /* 根据结构体成员m_DirectionEn，设置TXDIROUT位域 */
    tmpreg = (qeiInitStruct->m_IndexReset << QEI_CTL0_INDRES_POS) \
           | (qeiInitStruct->m_DirectionEn << QEI_CTL0_TXDIROUT_POS);
    QEIx->CTL0 = SFR_Config (QEIx->CTL0,
                          ~QEI_CTL0_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  QEI模块定时器初始化。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       qeiTimerInitStruct: QEI定时器(T7/T8)配置信息结构体指针。
  * 返回  无。
  */
void
QEI_Timer_Configuration(QEI_SFRmap* QEIx, QEI_TimerTypeDef* qeiTimerInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_COUNT_DIRECT(qeiTimerInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_QEI_TIMER_SOURCE(qeiTimerInitStruct->m_ClockSource));

    /*---------------- 设置Tx_CNT寄存器 ----------------*/
    QEIx->CNT = (uint32_t)qeiTimerInitStruct->m_Counter;

    /*---------------- 设置PPx周期寄存器 ---------------*/
    QEIx->PPX = (uint32_t)qeiTimerInitStruct->m_Period;

    /*---------------- 设置Tx预分频寄存器 --------------*/
    QEIx->PRSC = (uint32_t)qeiTimerInitStruct->m_Prescaler;

    /*---------------- 设置QEIx_CTL0寄存器 ----------------*/
    /* 根据结构体成员m_CounterMode，设置TXDIR位域 */
    /* 根据结构体成员m_ClockSource，设置TXCS位域 */
    tmpreg = (qeiTimerInitStruct->m_CounterMode) \
           | (qeiTimerInitStruct->m_ClockSource);
    QEIx->CTL0 = SFR_Config (QEIx->CTL0,
                          ~QEI_CTL0_TIMER_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化QEI模块正交编码器信息结构体。
  * 输入  qeiInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
QEI_Struct_Init (QEI_InitTypeDef* qeiInitStruct)
{
    /* 初始化 位置计数器复位使能 */
    qeiInitStruct->m_IndexReset = FALSE;
    /* 初始化 位置计数器方向状态输出使能 */
    qeiInitStruct->m_DirectionEn = FALSE;
    /* 初始化 QEI工作时钟 */
    qeiInitStruct->m_WorkClock = QEI_SOURCE_SCLK;
    /* 初始化 QEI数字滤波器输出使能 */
    qeiInitStruct->m_DigitalFilterEn = FALSE;
    /* 初始化 QEI数字滤波器时钟分频 */
    qeiInitStruct->m_DigitalFilterPrescaler = QEI_DIGITAL_FILTER_CLK_DIV_1;
    /* 初始化 A相和B相输入交换使能 */
    qeiInitStruct->m_SwapABEn = FALSE;
    /* 初始化 定时器计数值 */
    qeiInitStruct->m_Counter = 0;
    /* 初始化 定时器周期值 */
    qeiInitStruct->m_Period = 0;
    /* 初始化 定时器预分频 */
    qeiInitStruct->m_Prescaler = 0;
}

/**
  * 描述  初始化QEI定时器配置信息结构体。
  * 输入  qeiTimerInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
QEI_Timer_Struct_Init (QEI_TimerTypeDef* qeiTimerInitStruct)
{
    /* 初始化 定时器计数值 */
    qeiTimerInitStruct->m_Counter = 0;
    /* 初始化 定时器周期值 */
    qeiTimerInitStruct->m_Period = 0;
    /* 初始化 定时器预分频值 */
    qeiTimerInitStruct->m_Prescaler = 0;
    /* 初始化 计数器方向 */
    qeiTimerInitStruct->m_CounterMode = QEI_COUNT_DIRECT_REVERSE;
    /* 初始化 定时器时钟源选择 */
    qeiTimerInitStruct->m_ClockSource = QEI_TIMER_SOURCE_SCLK;
}
/**
  *   ##### 正交编码脉冲电路(QEI)初始化函数定义结束 #####
  */


/**
  *   ##### 正交编码脉冲电路(QEI)功能配置函数 #####
  */
/**
  * 描述  配置正交编码器接口模式。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       PortMode: 正交编码器接口模式，取值范围为:
  *                   QEI_ENCODER_OFF: 正交编码器接口/定时器关闭
  *                   QEI_GENERAL_PURPOSE_COUNTER: 使能计数方向/时钟可选的通用计数器模式
  *                   QEI_X2_INDEX_PULSE_RESET: 使能正交编码器接口（x2模式），索引脉冲复位位置计数器
  *                   QEI_X2_MATCH_RESET: 使能正交编码器接口（x2模式），匹配时复位位置计数器（PPX）
  *                   QEI_X4_INDEX_PULSE_RESET: 使能正交编码器接口（x4模式），索引脉冲复位位置计数器
  *                   QEI_X4_MATCH_RESET: 使能正交编码器接口（x4模式），匹配时复位位置计数器（PPX）
  * 返回  无。
  */
void
QEI_Encoder_Mode_Config (QEI_SFRmap* QEIx, uint32_t PortMode)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_PORT_MODE(PortMode));

    /*---------------- 配置QEI_CTL0寄存器QEIMOD位 ----------------*/
    tmpreg = PortMode << QEI_CTL0_QEIMOD0_POS;
    QEIx->CTL0 = SFR_Config (QEIx->CTL0, ~QEI_CTL0_QEIMOD, tmpreg);
}

/**
  * 描述  配置位置计数器复位使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState: 位置计数器复位使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Location_Counter_Reset_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置QEI_CTL0寄存器INDRES位 ----------------*/
    if (NewState != FALSE)
    {
        /* 索引脉冲复位位置计数器 */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_INDRES_POS);
    }
    else
    {
        /* 索引脉冲不复位位置计数器 */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_INDRES_POS);
    }
}

/**
  * 描述  配置QEI定时器时钟源。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       TimerClock: QEI定时器时钟源，取值范围为:
  *                     QEI_TIMER_SOURCE_SCLK: 内部时钟（SCLK）
  *                     QEI_TIMER_SOURCE_EXTERNAL_PIN_QEA: 来自QEAy引脚的外部时钟（下降沿触发计数）
  * 返回  无。
  */
void
QEI_Timer_Source_Clock_Config (QEI_SFRmap* QEIx, uint32_t TimerClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_TIMER_SOURCE(TimerClock));

    /*---------------- 设置QEI_CTL0寄存器TXCS位 ----------------*/
    if (TimerClock != QEI_TIMER_SOURCE_SCLK)
    {
        /* 来自QEAy引脚的外部时钟 */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXCS_POS);
    }
    else
    {
        /* 内部时钟（SCLK） */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXCS_POS);
    }
}

/**
  * 描述  获取QEI索引引脚状态。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  1:索引引脚为高电平，0:索引引脚为低电平。
  */
FlagStatus
QEI_Get_Index_Pin_State (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取QEI_CTL0寄存器INDEX位 ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_INDEX)
    {
        /* 索引引脚为高电平 */
        return SET;
    }
    else
    {
        /* 索引引脚为低电平 */
        return RESET;
    }
}

/**
  * 描述  配置位置计数器方向状态输出使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState: 位置计数器方向状态输出使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Location_Counter_Output_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置QEI_CTL0寄存器TXDIROUT位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能位置计数器方向状态输出 */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIROUT_POS);
    }
    else
    {
        /* 禁止位置计数器方向状态输出 */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIROUT_POS);
    }
}

/**
  * 描述  配置QEI位置计数器方向状态。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       Direction: QEI位置计数器方向状态，取值范围为:
  *                    QEI_COUNT_DIRECT_REVERSE: 位置计数器方向为反向（-）
  *                    QEI_COUNT_DIRECT_FORWARD: 位置计数器方向为正向（+）
  * 返回  无。
  */
void
QEI_Location_Counter_Direction_Config (QEI_SFRmap* QEIx, uint32_t Direction)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_COUNT_DIRECT(Direction));

    /*---------------- 设置QEI_CTL0寄存器TXDIR位 ----------------*/
    if (Direction != QEI_COUNT_DIRECT_REVERSE)
    {
        /* 位置计数器方向为正向（+） */
        SFR_SET_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIR_POS);
    }
    else
    {
        /* 位置计数器方向为反向（-） */
        SFR_CLR_BIT_ASM(QEIx->CTL0, QEI_CTL0_TXDIR_POS);
    }
}

/**
  * 描述  获取QEI位置计数器方向状态。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  1:位置计数器方向为正向（+），0:位置计数器方向为反向（-）。
  */
FlagStatus
QEI_Get_Location_Counter_Direction (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取QEI_CTL0寄存器TXDIR位 ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_TXDIR)
    {
        /* 位置计数器方向为正向（+） */
        return SET;
    }
    else
    {
        /* 位置计数器方向为反向（-） */
        return RESET;
    }
}

/**
  * 描述  配置QEI工作时钟。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       PeripheralClock: QEI工作时钟，取值范围为:
  *                          QEI_SOURCE_SCLK: 工作时钟为SCLK
  *                          QEI_SOURCE_HFCLK: 工作时钟为HFCLK
  *                          QEI_SOURCE_LFCLK: 工作时钟为LFCLK
  * 返回  无。
  */
void
QEI_Source_Clock_Config (QEI_SFRmap* QEIx, uint32_t PeripheralClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_SOURCE(PeripheralClock));

    /*---------------- 配置QEI_CTL1寄存器QEICLK位 ----------------*/
    QEIx->CTL1 = SFR_Config (QEIx->CTL1,
                            ~QEI_CTL1_QEICLK,
                            PeripheralClock);
}

/**
  * 描述  配置数字滤波器输出使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState: 数字滤波器输出使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Digital_Filter_Output_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置QEI_CTL1寄存器QEIOUT位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能QEAy/QEBy/INDXy引脚上的数字滤波器输出 */
        SFR_SET_BIT_ASM(QEIx->CTL1, QEI_CTL1_QEIOUT_POS);
    }
    else
    {
        /* 禁止数字滤波器输出 */
        SFR_CLR_BIT_ASM(QEIx->CTL1, QEI_CTL1_QEIOUT_POS);
    }
}

/**
  * 描述  配置QEI数字滤波器时钟分频。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       DigitalFilterClock: QEI数字滤波器时钟分频，对QEAy/QEBy/INDXy滤波时钟
  *                           进行分频，取值范围为:
  *                             QEI_DIGITAL_FILTER_CLK_DIV_1: 1:1分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_2: 1:2分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_4: 1:4分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_16: 1:16分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_32: 1:32分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_64: 1:64分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_128: 1:128分频
  *                             QEI_DIGITAL_FILTER_CLK_DIV_256: 1:256分频
  * 返回  无。
  */
void
QEI_Digital_Filter_Clock_Config (QEI_SFRmap* QEIx, uint32_t DigitalFilterClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_DIGITAL_FILTER_CLK(DigitalFilterClock));

    /*---------------- 配置QEI_CTL1寄存器QEICK位 ----------------*/
    QEIx->CTL1 = SFR_Config (QEIx->CTL1,
                            ~QEI_CTL1_QEICK,
                            DigitalFilterClock);
}

/**
  * 描述  配置A相和B相输入交换使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState: A相和B相输入交换使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Swap_Phase_AB_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置QEI_CTL1寄存器SWPAB位 ----------------*/
    if (NewState != FALSE)
    {
        /* A相和B相输入交换 */
        SFR_SET_BIT_ASM(QEIx->CTL1, QEI_CTL1_SWPAB_POS);
    }
    else
    {
        /* A相和B相输入不交换 */
        SFR_CLR_BIT_ASM(QEIx->CTL1, QEI_CTL1_SWPAB_POS);
    }
}

/**
  * 描述  配置QEI 16位的Tx定时器。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       Counter: 16位的Tx定时器，取值为16位数值。
  * 返回  无。
  */
void
QEI_Timer_Counter_Config (QEI_SFRmap* QEIx, uint16_t Counter)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 配置Tx_CNT寄存器TXCNT位 ----------------*/
    tmpreg = Counter << QEI_TXCNT_TXCNT0_POS;
    QEIx->CNT = SFR_Config (QEIx->CNT,
                            ~QEI_TXCNT_TXCNT,
                            (uint32_t)tmpreg);
}

/**
  * 描述  获取QEI 16位的Tx定时器。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  16位的Tx定时器，取值为16位数值。
  */
uint16_t
QEI_Get_Timer_Counter (QEI_SFRmap* QEIx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取Tx_CNT寄存器TXCNT位 ----------------*/
    tmpreg = QEIx->CNT;

    return (uint16_t)tmpreg;
}

/**
  * 描述  配置QEI 16位的Tx周期寄存器。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       Period: 16位的Tx周期寄存器，取值为16位数值。
  * 返回  无。
  */
void
QEI_Timer_Period_Config (QEI_SFRmap* QEIx, uint16_t Period)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 配置Tx_PPX寄存器PPX位 ----------------*/
    tmpreg = Period << QEI_PPX_PPX0_POS;
    QEIx->PPX = SFR_Config (QEIx->PPX,
                            ~QEI_PPX_PPX,
                            (uint32_t)tmpreg);
}

/**
  * 描述  获取QEI 16位的Tx周期寄存器。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  16位的Tx周期寄存器，取值为16位数值。
  */
uint16_t
QEI_Get_Timer_Period (QEI_SFRmap* QEIx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取Tx_PPX寄存器PPX位 ----------------*/
    tmpreg = QEIx->PPX;

    return (uint16_t)tmpreg;
}

/**
  * 描述  配置QEI 16位的Tx预分频器。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       Prescaler: 16位的Tx预分频器，取值为16位数值。
  * 返回  无。
  */
void
QEI_Timer_Prescaler_Config (QEI_SFRmap* QEIx, uint16_t Prescaler)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 配置Tx_PRSC寄存器TXPRSC位 ----------------*/
    tmpreg = Prescaler << QEI_TXPRSC_TXPRSC0_POS;
    QEIx->PRSC = SFR_Config (QEIx->PRSC,
                            ~QEI_TXPRSC_TXPRSC,
                            (uint32_t)tmpreg);
}
/**
  *   ##### 正交编码脉冲电路(QEI)功能配置函数定义结束 #####
  */


/**
  *   ##### 正交编码脉冲电路(QEI)中断管理函数 #####
  */
/**
  * 描述  获取QEI中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       InterruptType: QEI中断事件，取值为下面提供的一个:
  *                        QEI_INT_COUNT_ERROR: 计数器Tx溢出中断事件
  *                        QEI_INT_PHASE_ERROR: QEIy相位错误中断事件
  *                        QEI_INT_TX_OVERFLOW: QEIy计数错误中断事件
  * 返回  1:发生相应中断标志，0:未发生相应中断标志。
  */
FlagStatus
QEI_Get_INT_Flag (QEI_SFRmap* QEIx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_INT_ONE_EVENT(InterruptType));

    /*---------------- 读取QEI_CTL0寄存器对应位 ----------------*/
    tmpreg = InterruptType << QEI_CTL0_CNTERRIF_POS;
    if (QEIx->CTL0 & tmpreg)
    {
        /* 发生相应中断标志 */
        return SET;
    }
    else
    {
        /* 未发生相应中断标志 */
        return RESET;
    }
}

/**
  * 描述  清除QEI中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       InterruptType: QEI中断事件，取值为下面的一个或多个组合:
  *                        QEI_INT_COUNT_ERROR: 计数器Tx溢出中断事件
  *                        QEI_INT_PHASE_ERROR: QEIy相位错误中断事件
  *                        QEI_INT_TX_OVERFLOW: QEIy计数错误中断事件
  * 返回  无。
  */
void
QEI_Clear_INT_Flag (QEI_SFRmap* QEIx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_INT_EVENT(InterruptType));

    /*---------------- 清除QEI_DIER寄存器对应位 ----------------*/
    tmpreg1 = InterruptType << QEI_CTL0_CNTERRIF_POS;
    tmpreg = InterruptType << QEI_DIER_CNTERRIC_POS;
    QEIx->DIER |= tmpreg;
    while(QEIx->CTL0 & tmpreg1);
    QEIx->DIER &= ~tmpreg;
}

/**
  * 描述  配置QEI中断使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       InterruptType: QEI中断事件，取值为下面的一个或多个组合:
  *                        QEI_INT_COUNT_ERROR: 计数器Tx溢出中断事件
  *                        QEI_INT_PHASE_ERROR: QEIy相位错误中断事件
  *                        QEI_INT_TX_OVERFLOW: QEIy计数错误中断事件
  *       NewState: 中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Set_INT_Enable (QEI_SFRmap* QEIx,
                    uint32_t InterruptType, FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_QEI_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置QEI_DIER寄存器对应位 ----------------*/
    tmpreg = InterruptType << QEI_DIER_CNTERRIE_POS;
    if (NewState != FALSE)
    {
        /* 使能中断 */
        QEIx->DIER |= tmpreg;
    }
    else
    {
        /* 禁止中断 */
        QEIx->DIER &= ~tmpreg;
    }
}

/**
  * 描述  获取计数器Tx溢出中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  1:Tx发生向上或者向下溢出，0:Tx未发生向上或者向下溢出。
  */
FlagStatus
QEI_Get_Counter_Overflow_INT_Flag (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取QEI_CTL0寄存器TXIF位 ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_TXIF)
    {
        /* Tx发生向上或者向下溢出 */
        return SET;
    }
    else
    {
        /* Tx未发生向上或者向下溢出 */
        return RESET;
    }
}

/**
  * 描述  获取QEI相位错误中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  1:QEI相位出现错误，0:QEI相位未出现错误。
  */
FlagStatus
QEI_Get_Phase_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取QEI_CTL0寄存器PHEIF位 ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_PHEIF)
    {
        /* QEI相位出现错误 */
        return SET;
    }
    else
    {
        /* QEI相位未出现错误 */
        return RESET;
    }
}

/**
  * 描述  获取QEI计数错误中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  1:QEI计数出现错误，0:QEI计数未出现错误。
  */
FlagStatus
QEI_Get_Count_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*---------------- 读取QEI_CTL0寄存器PHEIF位 ----------------*/
    if (QEIx->CTL0 & QEI_CTL0_CNTERRIF)
    {
        /* QEI计数出现错误 */
        return SET;
    }
    else
    {
        /* QEI计数未出现错误 */
        return RESET;
    }
}

/**
  * 描述  清除QEI Tx溢出中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  无。
  */
void
QEI_Clear_Timer_Overflow_INT_Flag (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*-------------------- 设置QEI_DIER寄存器TXIC位 --------------------*/
    SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_TXIC_POS);
    while(QEIx->CTL0 & QEI_CTL0_TXIF);
    SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_TXIC_POS);
}

/**
  * 描述  清除QEI相位错误中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  无。
  */
void
QEI_Clear_Phase_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*-------------------- 设置QEI_DIER寄存器PHEIC位 --------------------*/
    SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIC_POS);
    while(QEIx->CTL0 & QEI_CTL0_PHEIF);
    SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIC_POS);
}

/**
  * 描述  清除QEI计数错误中断标志。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  * 返回  无。
  */
void
QEI_Clear_Counter_Error_INT_Flag (QEI_SFRmap* QEIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));

    /*-------------------- 设置QEI_DIER寄存器CNTERRIC位 --------------------*/
    SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIC_POS);
    while(QEIx->CTL0 & QEI_CTL0_CNTERRIF);
    SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIC_POS);
}

/**
  * 描述  配置QEI Tx溢出中断使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState: Tx溢出中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Timer_Overflow_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置QEI_DIER寄存器TXIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能计数器Tx溢出中断 */
        SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_TXIE_POS);
    }
    else
    {
        /* 禁止计数器Tx溢出中断 */
        SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_TXIE_POS);
    }
}

/**
  * 描述  配置QEI相位错误中断使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState:相位错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Phase_Error_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置QEI_DIER寄存器PHEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能QEIy相位错误中断 */
        SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIE_POS);
    }
    else
    {
        /* 禁止QEIy相位错误中断 */
        SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_PHEIE_POS);
    }
}

/**
  * 描述  配置QEI计数错误中断使能。
  * 输入  QEIx: 指向QEI内存结构的指针，取值为QEI0_SFR和QEI1_SFR。
  *       NewState:计数错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
QEI_Counter_Error_INT_Enable (QEI_SFRmap* QEIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_QEI_ALL_PERIPH(QEIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置QEI_DIER寄存器CNTERRIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能QEIy计数错误中断 */
        SFR_SET_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIE_POS);
    }
    else
    {
        /* 禁止QEIy计数错误中断 */
        SFR_CLR_BIT_ASM(QEIx->DIER, QEI_DIER_CNTERRIE_POS);
    }
}
/**
  *   ##### 正交编码脉冲电路(QEI)中断管理函数定义结束 #####
  */
#endif //KF32A_Periph_qei
