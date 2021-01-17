/**
  *********************************************************************
  * 文件名  kf32a_basic_tim.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了定时器外设的库函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_TIM_H_
#define _KF32A_BASIC_TIM_H_

#include "KF32A_BASIC.h"

/*** 定时器通用定义  ***/
/* 定时器编号定义 */
#define TIM0    (0)
#define TIM1    (1)
#define TIM2    (2)
#define TIM3    (3)
#define TIM4    (4)
#define TIM5    (5)
#define TIM6    (6)
#define TIM9    (9)
#define TIM10   (10)
#define TIM14   (14)
#define TIM15   (15)
#define TIM18   (18)
#define TIM19   (19)
#define TIM20   (20)
#define TIM21   (21)
#define TIM22   (22)
#define TIM23   (23)
#define CHECK_TIMX_ALL_NUM(NUM)         (((NUM) == TIM0) \
                                      || ((NUM) == TIM1) \
                                      || ((NUM) == TIM2) \
                                      || ((NUM) == TIM3) \
                                      || ((NUM) == TIM4) \
                                      || ((NUM) == TIM5) \
                                      || ((NUM) == TIM6) \
                                      || ((NUM) == TIM9) \
                                      || ((NUM) == TIM10) \
                                      || ((NUM) == TIM14) \
                                      || ((NUM) == TIM15) \
                                      || ((NUM) == TIM18) \
                                      || ((NUM) == TIM19) \
                                      || ((NUM) == TIM20) \
                                      || ((NUM) == TIM21) \
                                      || ((NUM) == TIM22) \
                                      || ((NUM) == TIM23))
#define CHECK_BTIM_NUM(NUM)             (((NUM) == TIM14) \
                                      || ((NUM) == TIM15))
#define CHECK_GPTIM_NUM(NUM)            (((NUM) == TIM0) \
                                      || ((NUM) == TIM1) \
                                      || ((NUM) == TIM2) \
                                      || ((NUM) == TIM3) \
                                      || ((NUM) == TIM4) \
                                      || ((NUM) == TIM18) \
                                      || ((NUM) == TIM19) \
                                      || ((NUM) == TIM20) \
                                      || ((NUM) == TIM21) \
                                      || ((NUM) == TIM22) \
                                      || ((NUM) == TIM23))
#define CHECK_ATIM_NUM(NUM)             (((NUM) == TIM5) \
                                      || ((NUM) == TIM6) \
                                      || ((NUM) == TIM9) \
                                      || ((NUM) == TIM10))

/* 定时器指针定义 */
#define T0_TYPE                         (GPTIM_SFRmap *)
#define T1_TYPE                         (GPTIM_SFRmap *)
#define T2_TYPE                         (GPTIM_SFRmap *)
#define T3_TYPE                         (GPTIM_SFRmap *)
#define T4_TYPE                         (GPTIM_SFRmap *)
#define T5_TYPE                         (ATIM_SFRmap *)
#define T6_TYPE                         (ATIM_SFRmap *)
#define T9_TYPE                         (ATIM_SFRmap *)
#define T10_TYPE                        (ATIM_SFRmap *)
#define T14_TYPE                        (BTIM_SFRmap *)
#define T15_TYPE                        (BTIM_SFRmap *)
#define T18_TYPE                        (GPTIM_SFRmap *)
#define T19_TYPE                        (GPTIM_SFRmap *)
#define T20_TYPE                        (GPTIM_SFRmap *)
#define T21_TYPE                        (GPTIM_SFRmap *)
#define T22_TYPE                        (GPTIM_SFRmap *)
#define T23_TYPE                        (GPTIM_SFRmap *)
//#define CHECK_TIM_ALL_PERIPH(ADDR)      (((ADDR) == T0_ADDR) \
//                                      || ((ADDR) == T1_ADDR) \
//                                      || ((ADDR) == T2_ADDR) \
//                                      || ((ADDR) == T3_ADDR) \
//                                      || ((ADDR) == T4_ADDR) \
//                                      || ((ADDR) == T5_ADDR) \
//                                      || ((ADDR) == T6_ADDR) \
//                                      || ((ADDR) == T9_ADDR) \
//                                      || ((ADDR) == T10_ADDR) \
//                                      || ((ADDR) == T14_ADDR) \
//                                      || ((ADDR) == T15_ADDR) \
//                                      || ((ADDR) == T18_ADDR) \
//                                      || ((ADDR) == T19_ADDR) \
//                                      || ((ADDR) == T20_ADDR) \
//                                      || ((ADDR) == T21_ADDR) \
//                                      || ((ADDR) == T22_ADDR) \
//                                      || ((ADDR) == T23_ADDR))
#define CHECK_TIM_ALL_PERIPH(ADDR)      (((ADDR) == ((uint32_t)0x40000880)) \
                                      || ((ADDR) == ((uint32_t)0x40000100)) \
                                      || ((ADDR) == ((uint32_t)0x40000180)) \
                                      || ((ADDR) == ((uint32_t)0x40000200)) \
                                      || ((ADDR) == ((uint32_t)0x40000280)) \
                                      || ((ADDR) == ((uint32_t)0x40000300)) \
                                      || ((ADDR) == ((uint32_t)0x40000400)) \
                                      || ((ADDR) == ((uint32_t)0x40001800)) \
                                      || ((ADDR) == ((uint32_t)0x40001880)) \
                                      || ((ADDR) == ((uint32_t)0x40001A80)) \
                                      || ((ADDR) == ((uint32_t)0x40001B00)) \
                                      || ((ADDR) == ((uint32_t)0x40001B80)) \
                                      || ((ADDR) == ((uint32_t)0x40001C00)) \
                                      || ((ADDR) == ((uint32_t)0x40001C80)) \
                                      || ((ADDR) == ((uint32_t)0x40001D00)))
//#define CHECK_BTIM_PERIPH(PERIPH)       (((PERIPH) == T14_SFR) \
//                                      || ((PERIPH) == T15_SFR))
#define CHECK_BTIM_PERIPH(PERIPH)       (((PERIPH) == ((BTIM_SFRmap *)(uint32_t)0x40001800)) \
                                      || ((PERIPH) == ((BTIM_SFRmap *)(uint32_t)0x40001880)))
//#define CHECK_GPTIM_PERIPH(PERIPH)      (((PERIPH) == T0_SFR) \
//                                      || ((PERIPH) == T1_SFR) \
//                                      || ((PERIPH) == T2_SFR) \
//                                      || ((PERIPH) == T3_SFR) \
//                                      || ((PERIPH) == T4_SFR) \
//                                      || ((PERIPH) == T18_SFR) \
//                                      || ((PERIPH) == T19_SFR) \
//                                      || ((PERIPH) == T20_SFR) \
//                                      || ((PERIPH) == T21_SFR) \
//                                      || ((PERIPH) == T22_SFR) \
//                                      || ((PERIPH) == T23_SFR))
#define CHECK_GPTIM_PERIPH(PERIPH)      (((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000880)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000100)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000180)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000200)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40000280)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001A80)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001B00)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001B80)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001C00)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001C80)) \
                                      || ((PERIPH) == ((GPTIM_SFRmap *)(uint32_t)0x40001D00)))
//#define CHECK_ATIM_PERIPH(PERIPH)       (((PERIPH) == T5_SFR) \
//                                      || ((PERIPH) == T6_SFR) \
//                                      || ((PERIPH) == T9_SFR) \
//                                      || ((PERIPH) == T10_SFR))
#define CHECK_ATIM_PERIPH(PERIPH)       (((PERIPH) == ((ATIM_SFRmap *)(uint32_t)0x40000300)) \
                                      || ((PERIPH) == ((ATIM_SFRmap *)(uint32_t)0x40000400)))

/**
  * 引脚状态
  */
#define PIN_INACTIVE                    ((uint32_t)0x0)
#define PIN_ACTIVE                      ((uint32_t)0x1)
#define PIN_TRISTATE                    ((uint32_t)0x2)
#define CHECK_PIN_STATE(STATE)          (((STATE) == PIN_INACTIVE) \
                                      || ((STATE) == PIN_ACTIVE) \
                                      || ((STATE) == PIN_TRISTATE))
/*** 定时器通用定义结束  ***/


/*** 基本定时器(BTIM)相关定义  */
/**
  * 描述  基本定时器(BTIM)配置信息结构体
  */
typedef struct
{
    uint16_t m_Counter;         /* 定时器计数值，取值16位数据。 */
    uint16_t m_Period;          /* 定时器周期值，取值16位数据。 */
    uint16_t m_Prescaler;       /* 定时器预分频值，取值16位数据。 */
    uint16_t m_CounterMode;     /* 定时器计数模式，
                                   取值为宏“BTIM定时器计数模式”中的一个 */
    uint16_t m_Clock;           /* 定时器工作时钟，
                                   取值为宏“BTIM定时器工作时钟”中的一个 */
    uint16_t m_WorkMode;        /* 定时/计数模式选择
                                   取值为宏“BTIM定时/计数模式选择”中的一个 */
    uint16_t m_MasterMode;      /* 主模式选择，
                                   取值为宏“BTIM主模式选择”中的一个 */
    uint16_t m_SlaveMode;       /* 从模式选择，
                                   取值为宏“BTIM从模式选择”中的一个 */
    uint16_t m_EXPulseSync;     /* Tx计数模式外部触发脉冲输入同步控制，
                                   取值为宏“BTIM计数模式外部触发脉冲输入同步控制”中的一个 */
    uint16_t m_MasterSlaveSync; /* 主从模式同步控制，
                                   取值为TRUE或FALSE */
} BTIM_InitTypeDef; /* T14 / T15 */


/**
  * BTIM定时器计数模式
  */
#define BTIM_COUNT_DOWN_UF              ((uint32_t)0 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_OF                ((uint32_t)4 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_DOWN_OF           ((uint32_t)1 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_DOWN_UF           ((uint32_t)2 << BTIM_CTL1_TXCMS0_POS)
#define BTIM_COUNT_UP_DOWN_OUF          ((uint32_t)3 << BTIM_CTL1_TXCMS0_POS)
#define CHECK_BTIM_COUNTER_MODE(MODE)   ((((MODE) & 0x3F) == 0) && \
                                         (((MODE) >> 9) == 0))

/**
  * BTIM定时器工作时钟
  */
#define BTIM_SCLK                       ((uint32_t)0 << BTIM_CTL1_TXCLK0_POS)
#define BTIM_HFCLK                      ((uint32_t)1 << BTIM_CTL1_TXCLK0_POS)
#define BTIM_LFCLK                      ((uint32_t)2 << BTIM_CTL1_TXCLK0_POS)
#define CHECK_BTIM_CLOCK_CONFIG(CLK)    ((((CLK) & 0x7) == 0) && \
                                         (((CLK) >> 5) == 0))

/**
  * BTIM定时/计数模式选择
  */
#define BTIM_TIMER_MODE                 ((uint32_t)0 << BTIM_CTL1_TXCS_POS)
#define BTIM_COUNTER_MODE               ((uint32_t)1 << BTIM_CTL1_TXCS_POS)
#define CHECK_BTIM_WORK_MODE(MODE)      (((MODE) == BTIM_TIMER_MODE) \
                                      || ((MODE) == BTIM_COUNTER_MODE))

/**
  * BTIM主模式选择
  */
#define BTIM_MASTER_UR_SIGNAL           ((uint32_t)0 << BTIM_CTL2_TXMMS0_POS)
#define BTIM_MASTER_EN_SIGNAL           ((uint32_t)1 << BTIM_CTL2_TXMMS0_POS)
#define BTIM_MASTER_IF_SIGNAL           ((uint32_t)2 << BTIM_CTL2_TXMMS0_POS)
#define CHECK_BTIM_MASTER_MODE(MODE)    (((MODE) == BTIM_MASTER_UR_SIGNAL) \
                                      || ((MODE) == BTIM_MASTER_EN_SIGNAL) \
                                      || ((MODE) == BTIM_MASTER_IF_SIGNAL))

/**
  * BTIM从模式选择
  */
#define BTIM_SLAVE_FORBIDDEN_MODE       ((uint32_t)0 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_TRIGGER_MODE         ((uint32_t)4 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_GATED_MODE           ((uint32_t)5 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_RESET_MODE           ((uint32_t)6 << BTIM_CTL2_TXSMS0_POS)
#define BTIM_SLAVE_COUNTER_MODE         ((uint32_t)7 << BTIM_CTL2_TXSMS0_POS)
#define CHECK_BTIM_SLAVE_MODE(MODE)     (((MODE) == BTIM_SLAVE_FORBIDDEN_MODE) \
                                      || ((MODE) == BTIM_SLAVE_TRIGGER_MODE) \
                                      || ((MODE) == BTIM_SLAVE_GATED_MODE) \
                                      || ((MODE) == BTIM_SLAVE_RESET_MODE) \
                                      || ((MODE) == BTIM_SLAVE_COUNTER_MODE))

/**
  * BTIM计数模式外部触发脉冲输入同步控制
  */
#define BTIM_EX_SYNC_MODE               ((uint32_t)0 << BTIM_CTL1_TXSY_POS)
#define BTIM_NO_SYNC_MODE               ((uint32_t)1 << BTIM_CTL1_TXSY_POS)
#define CHECK_BTIM_SYNC_MODE(MODE)      (((MODE) == BTIM_EX_SYNC_MODE) \
                                      || ((MODE) == BTIM_NO_SYNC_MODE))

/**
  * BTIM触发选择
  */
#define BTIM_TRIGGER_T1                 ((uint32_t)0 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T2                 ((uint32_t)1 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T3                 ((uint32_t)2 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T4                 ((uint32_t)3 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T5                 ((uint32_t)4 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T9                 ((uint32_t)5 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T14                ((uint32_t)6 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T15                ((uint32_t)7 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T18                ((uint32_t)8 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T19                ((uint32_t)9 << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T20                ((uint32_t)0xA << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_T21                ((uint32_t)0xB << BTIM_CTL2_TXTS0_POS)
#define BTIM_TRIGGER_TXCK               ((uint32_t)0xC << BTIM_CTL2_TXTS0_POS)
#define CHECK_BTIM_TRIGGER_MODE(MODE)   (((MODE) == BTIM_TRIGGER_T1) \
                                      || ((MODE) == BTIM_TRIGGER_T2) \
                                      || ((MODE) == BTIM_TRIGGER_T3) \
                                      || ((MODE) == BTIM_TRIGGER_T4) \
                                      || ((MODE) == BTIM_TRIGGER_T5) \
                                      || ((MODE) == BTIM_TRIGGER_T9) \
                                      || ((MODE) == BTIM_TRIGGER_T14) \
                                      || ((MODE) == BTIM_TRIGGER_T15) \
                                      || ((MODE) == BTIM_TRIGGER_T18) \
                                      || ((MODE) == BTIM_TRIGGER_T19) \
                                      || ((MODE) == BTIM_TRIGGER_T20) \
                                      || ((MODE) == BTIM_TRIGGER_T21) \
                                      || ((MODE) == BTIM_TRIGGER_TXCK))

/**
  * BTIM计数方向标志
  */
typedef enum
{
    DIR_DOWN = 0,
    DIR_UP = !DIR_DOWN
} DIRStatus;
#define CHECK_TIMER_DIR(state)          (((state) == DIR_DOWN) \
                                      || ((state) == DIR_UP))


/* 定时器外设复位函数定义 ************************************************/
void TIM_Reset (void* TIMx);

/* 基本定时器(BTIM)初始化及配置函数定义 **********************************/
void BTIM_Configuration (BTIM_SFRmap* BTIMx,
                    BTIM_InitTypeDef* btimInitStruct);
void BTIM_Struct_Init (BTIM_InitTypeDef* btimInitStruct);
void BTIM_Cmd (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Set_Counter (BTIM_SFRmap* BTIMx, uint16_t Counter);
void BTIM_Set_Period (BTIM_SFRmap* BTIMx, uint16_t Period);
void BTIM_Set_Prescaler (BTIM_SFRmap* BTIMx, uint16_t Prescaler);
void BTIM_Counter_Mode_Config (BTIM_SFRmap* BTIMx, uint32_t CounterMode);
void BTIM_Clock_Config (BTIM_SFRmap* BTIMx, uint32_t NewClock);
void BTIM_External_Pulse_Sync_Config (BTIM_SFRmap* BTIMx, uint32_t PulseSync);
void BTIM_Work_Mode_Config (BTIM_SFRmap* BTIMx, uint32_t NewState);
void BTIM_Generate_Trigger_Config (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Single_Pulse_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Single_Pulse_Shut_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_Immediately_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Master_Slave_Snyc_Config (BTIM_SFRmap* BTIMx, FunctionalState NewState);
void BTIM_Trigger_Select_Config (BTIM_SFRmap* BTIMx,
                    uint32_t TriggerSelect);
void BTIM_Slave_Mode_Config (BTIM_SFRmap* BTIMx,
                    uint32_t SlaveMode);
void BTIM_Master_Mode_Config (BTIM_SFRmap* BTIMx,
                    uint32_t MasterMode);
void BTIM_Updata_Rising_Edge_Config (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState);
DIRStatus BTIM_Get_Direction (BTIM_SFRmap* BTIMx);
uint16_t BTIM_Get_Counter (BTIM_SFRmap* BTIMx);
uint16_t BTIM_Get_Period (BTIM_SFRmap* BTIMx);
uint16_t BTIM_Get_Prescaler (BTIM_SFRmap* BTIMx);

/* 基本定时器(BTIM)中断及标志管理函数 ************************************/
void BTIM_Trigger_DMA_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_DMA_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Overflow_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Trigger_INT_Enable (BTIM_SFRmap* BTIMx,
                    FunctionalState NewState);
void BTIM_Updata_INT_Enable (BTIM_SFRmap* BTIMx, FunctionalState NewState);
INTStatus BTIM_Get_Trigger_DMA_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Updata_DMA_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Overflow_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Trigger_INT_Status (BTIM_SFRmap* BTIMx);
INTStatus BTIM_Get_Updata_INT_Status (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Trigger_DMA_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Updata_DMA_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Overflow_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Trigger_INT_Flag (BTIM_SFRmap* BTIMx);
FlagStatus BTIM_Get_Updata_INT_Flag (BTIM_SFRmap* BTIMx);
void BTIM_Clear_Overflow_INT_Flag (BTIM_SFRmap* BTIMx);
void BTIM_Clear_Trigger_INT_Flag (BTIM_SFRmap* BTIMx);
void BTIM_Clear_Updata_INT_Flag (BTIM_SFRmap* BTIMx);

/*** 基本定时器(BTIM)相关定义结束  */


/*** 通用定时器(GPTIM)、CCP相关定义  */
/**
  * 描述  通用定时器(GPTIM)配置信息结构体
  */
typedef struct
{
    uint32_t m_Counter;         /* 定时器计数值，取值32位数据。 */
    uint32_t m_Period;          /* 定时器周期值，取值32位数据。 */
    uint32_t m_Prescaler;       /* 定时器预分频值，取值32位数据。 */
    uint16_t m_CounterMode;     /* 定时器计数模式，
                                   取值为宏“GPTIM定时器计数模式”中的一个 */
    uint16_t m_Clock;           /* 定时器工作时钟，
                                   取值为宏“GPTIM定时器工作时钟”中的一个 */
    uint16_t m_WorkMode;        /* 定时/计数模式选择
                                   取值为宏“GPTIM定时/计数模式选择”中的一个 */
    uint16_t m_MasterMode;      /* 主模式选择，
                                   取值为宏“GPTIM主模式选择”中的一个 */
    uint16_t m_SlaveMode;       /* 从模式选择，
                                   取值为宏“GPTIM从模式选择”中的一个 */
    uint16_t m_EXPulseSync;     /* Tx计数模式外部触发脉冲输入同步控制，
                                   取值为宏“GPTIM计数模式外部触发脉冲输入同步控制”中的一个 */
    uint16_t m_MasterSlaveSync; /* 主从模式同步控制，
                                   取值为TRUE或FALSE */
} GPTIM_InitTypeDef; /* T0/T1/T2/T3/T4/T18/T19/T20/T21 */

/**
  * 描述  CCP比较模式配置信息结构体
  */
typedef struct
{
    uint16_t m_Channel;         /* 通道编号，
                                   取值范围满足“CCP通道”的宏。 */
    uint16_t m_CompareMode;     /* 捕捉/比较器模式选择，
                                   取值范围满足宏CHECK_CCP_CMP_MODE的约定条件。 */
    uint32_t m_CompareValue;    /* 与TX比较的值，
                                   取值范围为32位数据。 */
} CCP_CompareInitTypeDef;

/**
  * 描述  CCP捕捉模式配置信息结构体
  */
typedef struct
{
    uint16_t m_Channel;         /* 通道编号，
                                   取值范围满足“CCP通道”的宏。 */
    uint16_t m_CaptureMode;     /* 捕捉/比较器模式选择，
                                   取值范围满足宏CHECK_CCP_CAP_MODE的约定条件。 */
    FunctionalState m_PWMInput;        /* PWM测量模式，
                                   取值为TRUE或FALSE。 */
    FunctionalState m_XORMode;         /* 输入异或模式，
                                   取值为TRUE或FALSE。 */
    FunctionalState m_ChannelCompare4; /* 比较器的输出触发捕捉，只有通道4可以配置，
                                   取值为TRUE或FALSE。 */
} CCP_CaptureInitTypeDef;

/**
  * 描述  CCP PWM模式配置信息结构体
  */
typedef struct
{
    uint16_t m_Channel;         /* 通道编号，
                                   取值范围满足“CCP通道”的宏。 */
    uint16_t m_PwmMode;         /* 捕捉/比较器模式选择，
                                   取值范围满足宏CHECK_CCP_PWM_MODE的约定条件。 */
    uint32_t m_DutyRatio;       /* 占空比，
                                   取值范围为32位数据。 */
    uint32_t m_OutputCtl;       /* 通道输出控制，
                                   取值为“CCP通道输出控制”中的一个。 */
    uint32_t m_SinglePWM;       /* 单脉冲输出模式，
                                   取值为TRUE或FALSE。 */
    uint32_t m_CloseTimer;      /* 单脉冲输出模式选择，
                                   取值为TRUE或FALSE。 */
} CCP_PWMInitTypeDef;

/**
  * GPTIM定时器计数模式
  */
#define GPTIM_COUNT_DOWN_UF             ((uint32_t)0x0 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_OF               ((uint32_t)0x4 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_DOWN_OF          ((uint32_t)0x1 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_DOWN_UF          ((uint32_t)0x2 << GPTIM_CTL1_TXCMS0_POS)
#define GPTIM_COUNT_UP_DOWN_OUF         ((uint32_t)0x3 << GPTIM_CTL1_TXCMS0_POS)
#define CHECK_GPTIM_COUNTER_MODE(MODE)  (((MODE) == GPTIM_COUNT_DOWN_UF) \
                                      || ((MODE) == GPTIM_COUNT_UP_OF) \
                                      || ((MODE) == GPTIM_COUNT_UP_DOWN_OF) \
                                      || ((MODE) == GPTIM_COUNT_UP_DOWN_UF) \
                                      || ((MODE) == GPTIM_COUNT_UP_DOWN_OUF))

/**
  * GPTIM定时器工作时钟
  */
#define GPTIM_SCLK                      ((uint32_t)0x0 << GPTIM_CTL1_TXCLK0_POS)
#define GPTIM_HFCLK                     ((uint32_t)0x1 << GPTIM_CTL1_TXCLK0_POS)
#define GPTIM_LFCLK                     ((uint32_t)0x2 << GPTIM_CTL1_TXCLK0_POS)
#define CHECK_GPTIM_CLOCK_CONFIG(CLK)   (((CLK) == GPTIM_SCLK) \
                                      || ((CLK) == GPTIM_HFCLK) \
                                      || ((CLK) == GPTIM_LFCLK))

/**
  * GPTIM定时/计数模式选择
  */
#define GPTIM_TIMER_MODE                ((uint32_t)0 << GPTIM_CTL1_TXCS_POS)
#define GPTIM_COUNTER_MODE              ((uint32_t)1 << GPTIM_CTL1_TXCS_POS)
#define CHECK_GPTIM_WORK_MODE(MODE)     (((MODE) == GPTIM_TIMER_MODE) \
                                      || ((MODE) == GPTIM_COUNTER_MODE))

/**
  * GPTIM主模式选择
  */
#define GPTIM_MASTER_UR_SIGNAL          ((uint32_t)0 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_TXEN_SIGNAL        ((uint32_t)1 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_TXIF_SIGNAL        ((uint32_t)2 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH1IF_SIGNAL   ((uint32_t)3 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH1_SIGNAL     ((uint32_t)4 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH2_SIGNAL     ((uint32_t)5 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH3_SIGNAL     ((uint32_t)6 << GPTIM_CTL2_TXMMS0_POS)
#define GPTIM_MASTER_CCPXCH4_SIGNAL     ((uint32_t)7 << GPTIM_CTL2_TXMMS0_POS)
#define CHECK_GPTIM_MASTER_MODE(MODE)   (((MODE) == GPTIM_MASTER_UR_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_TXEN_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_TXIF_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH1IF_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH1_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH2_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH3_SIGNAL) \
                                      || ((MODE) == GPTIM_MASTER_CCPXCH4_SIGNAL))

/**
  * GPTIM从模式选择
  */
#define GPTIM_SLAVE_FORBIDDEN_MODE      ((uint32_t)0 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_TRIGGER_MODE        ((uint32_t)4 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_GATED_MODE          ((uint32_t)5 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_RESET_MODE          ((uint32_t)6 << GPTIM_CTL2_TXSMS0_POS)
#define GPTIM_SLAVE_COUNTER_MODE        ((uint32_t)7 << GPTIM_CTL2_TXSMS0_POS)
#define CHECK_GPTIM_SLAVE_MODE(MODE)    (((MODE) == GPTIM_SLAVE_FORBIDDEN_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_TRIGGER_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_GATED_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_RESET_MODE) \
                                      || ((MODE) == GPTIM_SLAVE_COUNTER_MODE))

/**
  * GPTIM计数模式外部触发脉冲输入同步控制
  */
#define GPTIM_EX_SYNC_MODE              ((uint32_t)0x0 << GPTIM_CTL1_TXSY_POS)
#define GPTIM_NO_SYNC_MODE              ((uint32_t)0x1 << GPTIM_CTL1_TXSY_POS)
#define CHECK_GPTIM_SYNC_MODE(MODE)     (((MODE) == GPTIM_EX_SYNC_MODE) \
                                      || ((MODE) == GPTIM_NO_SYNC_MODE))

/**
  * GPTIM触发选择
  */
#define GPTIM_TRIGGER_T1                ((uint32_t)0x0 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T2                ((uint32_t)0x1 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T3                ((uint32_t)0x2 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T4                ((uint32_t)0x3 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T5                ((uint32_t)0x4 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T9                ((uint32_t)0x5 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T14               ((uint32_t)0x6 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T15               ((uint32_t)0x7 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T18               ((uint32_t)0x8 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T19               ((uint32_t)0x9 << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T20               ((uint32_t)0xA << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_T21               ((uint32_t)0xB << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_TXCK              ((uint32_t)0xC << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_CCPXCH1           ((uint32_t)0xD << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_CCPXCH2           ((uint32_t)0xE << GPTIM_CTL2_TXTS0_POS)
#define GPTIM_TRIGGER_CCPXCH3           ((uint32_t)0xF << GPTIM_CTL2_TXTS0_POS)
#define CHECK_GPTIM_TRIGGER_MODE(MODE)  (((uint32_t)(MODE) & (~GPTIM_CTL2_TXTS)) == 0)

/**
  * CCP通道
  */
#define CCP_CHANNEL_1                   ((uint16_t)0)
#define CCP_CHANNEL_2                   ((uint16_t)1)
#define CCP_CHANNEL_3                   ((uint16_t)2)
#define CCP_CHANNEL_4                   ((uint16_t)3)
#define CHECK_CCP_CHANNEL(CHANNEL)      (((CHANNEL) >> 2) == 0)

/**
  * CCP通道模式选择
  */
#define CCP_MODE_RST                    ((uint16_t)0x0)
#define CCP_CMP_TOGGLE_LEVEL            ((uint16_t)0x2)
#define CCP_CAP_FALLING_EDGE            ((uint16_t)0x4)
#define CCP_CAP_RISING_EDGE             ((uint16_t)0x5)
#define CCP_CAP_4TH_RISING_EDGE         ((uint16_t)0x6)
#define CCP_CAP_16TH_RISING_EDGE        ((uint16_t)0x7)
#define CCP_CMP_ACTIVE_LEVEL            ((uint16_t)0x8)
#define CCP_CMP_INACTIVE_LEVEL          ((uint16_t)0x9)
#define CCP_CMP_SOFT_INT                ((uint16_t)0xA)
#define CCP_CMP_SPECIAL_EVENT           ((uint16_t)0xB)
#define CCP_PWM_MODE                    ((uint16_t)0xC)
#define CHECK_CCP_MODE(MODE)            (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == CCP_CAP_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == CCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_16TH_RISING_EDGE) \
                                      || ((MODE) == CCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_SOFT_INT) \
                                      || ((MODE) == CCP_CMP_SPECIAL_EVENT) \
                                      || ((MODE) == CCP_PWM_MODE))
#define CHECK_CCP_CAP_MODE(MODE)        (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_CAP_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == CCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == CCP_CAP_16TH_RISING_EDGE))
#define CHECK_CCP_CMP_MODE(MODE)        (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == CCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == CCP_CMP_SOFT_INT) \
                                      || ((MODE) == CCP_CMP_SPECIAL_EVENT))
#define CHECK_CCP_PWM_MODE(MODE)        (((MODE) == CCP_MODE_RST) \
                                      || ((MODE) == CCP_PWM_MODE))

/**
  * CCP比较器检测选择
  */
#define CCP_COMPARE_SELECT_1            ((uint32_t)0x0 << CCP_CTL2_PXASS0_POS)
#define CCP_COMPARE_SELECT_2            ((uint32_t)0x1 << CCP_CTL2_PXASS0_POS)
#define CCP_COMPARE_SELECT_3            ((uint32_t)0x2 << CCP_CTL2_PXASS0_POS)
#define CHECK_CCP_COMPARE_SELECT(ASS)   (((ASS) == CCP_COMPARE_SELECT_1) \
                                      || ((ASS) == CCP_COMPARE_SELECT_2) \
                                      || ((ASS) == CCP_COMPARE_SELECT_3))

/**
  * CCP通道输出控制
  */
#define CCP_CHANNEL_OUTPUT_PWM_ACTIVE   ((uint32_t)0x0)
#define CCP_CHANNEL_OUTPUT_PWM_INACTIVE ((uint32_t)0x1)
#define CCP_CHANNEL_OUTPUT_INACTIVE     ((uint32_t)0x2)
#define CCP_CHANNEL_OUTPUT_ACTIVE       ((uint32_t)0x3)
#define CHECK_CCP_CHANNEL_OUTPUT(CTL)   (((CTL) >> 2) == 0)

/**
  * 通用定时器、CCP函数定义
  */
/* 通用定时器(GPTIM)初始化及配置函数定义 *********************************/
void GPTIM_Configuration(GPTIM_SFRmap* GPTIMx,
                    GPTIM_InitTypeDef* gptimInitStruct);
void GPTIM_Struct_Init (GPTIM_InitTypeDef* gptimInitStruct);
void GPTIM_Cmd (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Set_Counter (GPTIM_SFRmap* GPTIMx, uint32_t Counter);
void GPTIM_Set_Period (GPTIM_SFRmap* GPTIMx, uint32_t Period);
void GPTIM_Set_Prescaler (GPTIM_SFRmap* GPTIMx, uint32_t Prescaler);
void GPTIM_Counter_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t CounterMode);
void GPTIM_Clock_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewClock);
void GPTIM_External_Pulse_Sync_Config (GPTIM_SFRmap* GPTIMx,
                    uint32_t PulseSync);
void GPTIM_Work_Mode_Config (GPTIM_SFRmap* GPTIMx, uint32_t NewState);
void GPTIM_Updata_Immediately_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
void GPTIM_Master_Slave_Snyc_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
void GPTIM_Trigger_Select_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState TriggerSelect);
void GPTIM_Slave_Mode_Config (GPTIM_SFRmap* GPTIMx, FunctionalState SlaveMode);
void GPTIM_Master_Mode_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState MasterMode);
void GPTIM_Updata_Rising_Edge_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
void GPTIM_Updata_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Trigger_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Updata_DMA_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Updata_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Trigger_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Generate_Trigger_Config (GPTIM_SFRmap* GPTIMx,
                    FunctionalState NewState);
DIRStatus GPTIM_Get_Direction (GPTIM_SFRmap* GPTIMx);
uint32_t GPTIM_Get_Counter (GPTIM_SFRmap* GPTIMx);
uint32_t GPTIM_Get_Period (GPTIM_SFRmap* GPTIMx);
uint32_t GPTIM_Get_Prescaler (GPTIM_SFRmap* GPTIMx);
/* 通用定时器(GPTIM)中断及标志管理函数定义 *******************************/
void GPTIM_Overflow_INT_Enable (GPTIM_SFRmap* GPTIMx, FunctionalState NewState);
void GPTIM_Clear_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx);
void GPTIM_Clear_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx);
void GPTIM_Clear_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Overflow_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Updata_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Trigger_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Updata_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx);
FlagStatus GPTIM_Get_Trigger_DMA_INT_Flag (GPTIM_SFRmap* GPTIMx);
/* 通用捕捉/比较/PWM外设(CCP)捕捉功能函数定义 ****************************/
void CCP_Compare_Configuration(CCP_SFRmap* CCPx,
                    CCP_CompareInitTypeDef* ccpInitStruct);
void CCP_Compare_Struct_Init (CCP_CompareInitTypeDef* ccpInitStruct);
void CCP_Capture_Configuration (CCP_SFRmap* CCPx,
                    CCP_CaptureInitTypeDef* ccpInitStruct);
void CCP_Capture_Struct_Init (CCP_CaptureInitTypeDef* ccpInitStruct);
void CCP_PWM_Configuration (CCP_SFRmap* CCPx,
                    CCP_PWMInitTypeDef* ccpInitStruct);
void CCP_PWM_Struct_Init (CCP_PWMInitTypeDef* ccpInitStruct);
void CCP_Capture_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t EdgeConfig);
void CCP_Compare_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t EdgeConfig);
void CCP_PWM_Mode_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t EdgeConfig);
uint32_t CCP_Get_Capture_Result (CCP_SFRmap* CCPx, uint32_t Channel);
void CCP_Set_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t Value);
#define CCP_Set_PWM_Duty_Ratio(CCPX, CHANNEL, VALUE)  \
        CCP_Set_Compare_Result(CCPX, CHANNEL, VALUE)
uint32_t CCP_Get_Compare_Result (CCP_SFRmap* CCPx, uint32_t Channel);
#define CCP_Get_PWM_Duty_Ratio(CCPX, CHANNEL)  \
        CCP_Get_Compare_Result(CCPX, CHANNEL)
void CCP_Compare_PWM_Signal_Clear (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_Compare_Check_Config (CCP_SFRmap* CCPx, uint32_t CompareSelect);
void CCP_Channel_Output_Control (CCP_SFRmap* CCPx, uint32_t Channel,
                    uint32_t ChannelOutputCtl);
void CCP_Single_Pulse_Shut_Enable (CCP_SFRmap* CCPx, FunctionalState NewState);
void CCP_Single_Pulse_Enable (CCP_SFRmap* CCPx, FunctionalState NewState);
void CCP_PWM_Input_Measurement_Config (CCP_SFRmap* CCPx,
                    FunctionalState NewState);
void CCP_Input_XOR_Config (CCP_SFRmap* CCPx, FunctionalState NewState);
void CCP_Channel_DMA_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_Generate_Trigger_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_USART_Receive_Config (CCP_SFRmap* CCPx, FunctionalState NewState);
/* 通用捕捉/比较/PWM外设(CCP)中断及标志管理函数定义 **********************/
FlagStatus CCP_Get_Channel_Trigger_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel);
FlagStatus CCP_Get_Trigger_DMA_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel);
void CCP_Channel_INT_Config (CCP_SFRmap* CCPx, uint32_t Channel,
                    FunctionalState NewState);
void CCP_Clear_Channel_INT_Flag (CCP_SFRmap* CCPx, uint32_t Channel);
/*** 通用定时器(GPTIM)、CCP相关定义结束  */


/*** 高级定时器(ATIM)、ECCP相关定义  */
/**
  * 描述  高级定时器(ATIM)配置信息结构体
  */
typedef struct
{
    uint16_t m_Counter;         /* 定时器计数值，取值16位数据。 */
    uint16_t m_Period;          /* 定时器周期值，取值16位数据。 */
    uint16_t m_Prescaler;       /* 定时器预分频值，取值16位数据。 */
    uint16_t m_Postscaler;      /* 定时器后分频器的值，
                                   取值为宏“ATIM定时器后分频器的值”中的一个。 */
    uint32_t m_CounterMode;     /* 定时器计数模式，
                                   取值为宏“ATIM定时器计数模式”中的一个 */
    uint16_t m_Clock;           /* 定时器工作时钟，
                                   取值为宏“ATIM定时器工作时钟”中的一个 */
    uint16_t m_WorkMode;        /* 定时/计数模式选择
                                   取值为宏“ATIM定时/计数模式选择”中的一个 */
    uint16_t m_EXPulseSync;     /* Tx计数模式外部触发脉冲输入同步控制，
                                   取值为宏“ATIM计数模式外部触发脉冲输入同步控制”中的一个 */
} ATIM_InitTypeDef; /* T5/T6、T9/T10 */

/**
  * 描述  ECCP 捕捉模式配置信息结构体
  */
typedef struct
{
    uint32_t m_Channel;         /* 通道编号，
                                   取值范围满足“ECCP通道”的宏。 */
    uint32_t m_Mode;            /* 捕捉模式选择，
                                   取值范围满足宏CHECK_ECCP_CAP_MODE的约定条件。 */
    FunctionalState m_PWMInput;        /* PWM测量模式，
                                   取值为TRUE或FALSE。 */
    FunctionalState m_XORMode;         /* 输入异或模式，
                                   取值为TRUE或FALSE。 */
} ECCP_CaptureInitTypeDef;

/**
  * 描述  ECCP PWM模式配置信息结构体
  */
typedef struct
{
    uint32_t m_Channel;         /* 通道编号，
                                   取值范围满足“ECCP通道”的宏。 */
    uint32_t m_Mode;            /* 捕捉/比较器模式选择，
                                   取值范围满足宏CHECK_ECCP_PWM_MODE的约定条件。 */
    uint16_t m_DutyRatio;       /* 占空比，
                                   取值范围为0~65535。 */
    uint8_t m_DeadTime;         /* 通道死区控制，
                                   取值为0~255。 */
    uint8_t m_OutputMode;       /* 通道输出模式，
                                   取值为“ECCP通道输出模式”中的一个。 */
    uint16_t m_HOutputCtl;      /* 通道H输出控制，
                                   取值为“ECCP通道输出控制”中的一个。 */
    uint16_t m_LOutputCtl;      /* 通道L输出控制，
                                   取值为“ECCP通道输出控制”中的一个。 */
    FunctionalState m_PhaseMove;       /* 相位移动使能，
                                   取值为TRUE或FALSE。 */
    FunctionalState m_SinglePWM;       /* 单脉冲输出模式，
                                   取值为TRUE或FALSE。 */
    FunctionalState m_CloseTimer;      /* 单脉冲输出模式选择，
                                   取值为TRUE或FALSE。 */
} ECCP_PWMInitTypeDef;

/**
  * ATIM定时器后分频器的值
  */
#define ATIM_POSTSCALER_DIV_1           ((uint16_t)0x0)
#define ATIM_POSTSCALER_DIV_2           ((uint16_t)0x1)
#define ATIM_POSTSCALER_DIV_3           ((uint16_t)0x2)
#define ATIM_POSTSCALER_DIV_4           ((uint16_t)0x3)
#define ATIM_POSTSCALER_DIV_5           ((uint16_t)0x4)
#define ATIM_POSTSCALER_DIV_6           ((uint16_t)0x5)
#define ATIM_POSTSCALER_DIV_7           ((uint16_t)0x6)
#define ATIM_POSTSCALER_DIV_8           ((uint16_t)0x7)
#define ATIM_POSTSCALER_DIV_9           ((uint16_t)0x8)
#define ATIM_POSTSCALER_DIV_10          ((uint16_t)0x9)
#define ATIM_POSTSCALER_DIV_11          ((uint16_t)0xA)
#define ATIM_POSTSCALER_DIV_12          ((uint16_t)0xB)
#define ATIM_POSTSCALER_DIV_13          ((uint16_t)0xC)
#define ATIM_POSTSCALER_DIV_14          ((uint16_t)0xD)
#define ATIM_POSTSCALER_DIV_15          ((uint16_t)0xE)
#define ATIM_POSTSCALER_DIV_16          ((uint16_t)0xF)
#define CHECK_ATIM_POSTSCALER(SCALER)   (((SCALER) >= 0) && ((SCALER) <= 0xF))

/**
  * ATIM定时器计数模式
  */
#define ATIM_COUNT_DOWN_UF              ((uint32_t)0x0 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_OF                ((uint32_t)0x4 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_DOWN_OF           ((uint32_t)0x1 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_DOWN_UF           ((uint32_t)0x2 << ATIM_TXCTL_TXCMS0_POS)
#define ATIM_COUNT_UP_DOWN_OUF          ((uint32_t)0x3 << ATIM_TXCTL_TXCMS0_POS)
#define CHECK_ATIM_COUNTER_MODE(MODE)   (((MODE) == ATIM_COUNT_DOWN_UF) \
                                      || ((MODE) == ATIM_COUNT_UP_OF) \
                                      || ((MODE) == ATIM_COUNT_UP_DOWN_OF) \
                                      || ((MODE) == ATIM_COUNT_UP_DOWN_UF) \
                                      || ((MODE) == ATIM_COUNT_UP_DOWN_OUF))

/**
  * ATIM定时器工作时钟
  */
#define ATIM_SCLK                       ((uint32_t)0x0 << ATIM_TXCTL_TXCLK0_POS)
#define ATIM_HFCLK                      ((uint32_t)0x1 << ATIM_TXCTL_TXCLK0_POS)
#define ATIM_LFCLK                      ((uint32_t)0x2 << ATIM_TXCTL_TXCLK0_POS)
#define CHECK_ATIM_CLOCK_CONFIG(CLK)    (((CLK) == ATIM_SCLK) \
                                      || ((CLK) == ATIM_HFCLK) \
                                      || ((CLK) == ATIM_LFCLK))

/**
  * ATIM定时/计数模式选择
  */
#define ATIM_TIMER_MODE                 ((uint32_t)0 << ATIM_TXCTL_TXCS_POS)
#define ATIM_COUNTER_MODE               ((uint32_t)1 << ATIM_TXCTL_TXCS_POS)
#define CHECK_ATIM_WORK_MODE(MODE)      (((MODE) == ATIM_TIMER_MODE) \
                                      || ((MODE) == ATIM_COUNTER_MODE))

/**
  * ATIM计数模式外部触发脉冲输入同步控制
  */
#define ATIM_EX_SYNC_MODE               ((uint32_t)0x0 << ATIM_TXCTL_TXSY_POS)
#define ATIM_NO_SYNC_MODE               ((uint32_t)0x1 << ATIM_TXCTL_TXSY_POS)
#define CHECK_ATIM_SYNC_MODE(MODE)      (((MODE) == ATIM_EX_SYNC_MODE) \
                                      || ((MODE) == ATIM_NO_SYNC_MODE))

/**
  * ECCP通道
  */
#define ECCP_CHANNEL_1                  ((uint32_t)0x0)
#define ECCP_CHANNEL_2                  ((uint32_t)0x1)
#define ECCP_CHANNEL_3                  ((uint32_t)0x2)
#define ECCP_CHANNEL_4                  ((uint32_t)0x3)
#define CHECK_ECCP_CHANNEL(CHANNEL)     (((CHANNEL) >> 2) == 0)

/**
  * ECCP通道模式选择
  */
#define ECCP_MODE_RST                   ((uint32_t)0x0)
#define ECCP_CMP_TOGGLE_LEVEL           ((uint32_t)0x2)
#define ECCP_CAP_FALLING_EDGE           ((uint32_t)0x4)
#define ECCP_CAP_RISING_EDGE            ((uint32_t)0x5)
#define ECCP_CAP_4TH_RISING_EDGE        ((uint32_t)0x6)
#define ECCP_CAP_16TH_RISING_EDGE       ((uint32_t)0x7)
#define ECCP_CMP_ACTIVE_LEVEL           ((uint32_t)0x8)
#define ECCP_CMP_INACTIVE_LEVEL         ((uint32_t)0x9)
#define ECCP_CMP_SOFT_INT               ((uint32_t)0xA)
#define ECCP_CMP_SPECIAL_EVENT          ((uint32_t)0xB)
#define ECCP_PWM_FREE                   ((uint32_t)0xC)
#define ECCP_PWM_COORDINATION           ((uint32_t)0xD)
#define ECCP_PWM_SINGLE                 ((uint32_t)0xE)
#define CHECK_ECCP_MODE(MODE)           (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == ECCP_CAP_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == ECCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_16TH_RISING_EDGE) \
                                      || ((MODE) == ECCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_SOFT_INT) \
                                      || ((MODE) == ECCP_CMP_SPECIAL_EVENT) \
                                      || ((MODE) == ECCP_PWM_MODE))
#define CHECK_ECCP_CAP_MODE(MODE)       (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_CAP_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_FALLING_EDGE) \
                                      || ((MODE) == ECCP_CAP_4TH_RISING_EDGE) \
                                      || ((MODE) == ECCP_CAP_16TH_RISING_EDGE))
#define CHECK_ECCP_CMP_MODE(MODE)       (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_CMP_TOGGLE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_ACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_INACTIVE_LEVEL) \
                                      || ((MODE) == ECCP_CMP_SOFT_INT) \
                                      || ((MODE) == ECCP_CMP_SPECIAL_EVENT))
#define CHECK_ECCP_PWM_MODE(MODE)       (((MODE) == ECCP_MODE_RST) \
                                      || ((MODE) == ECCP_PWM_FREE) \
                                      || ((MODE) == ECCP_PWM_COORDINATION) \
                                      || ((MODE) == ECCP_PWM_SINGLE))

/**
  * ECCP通道输出模式
  */
#define ECCP_OUTPUT_INDEPENDENT         ((uint8_t)0x0)
#define ECCP_OUTPUT_COMPLEMENTARY       ((uint8_t)0x1)
#define CHECK_ECCP_OUTPUT_MODE(MODE)    (((MODE) == ECCP_OUTPUT_INDEPENDENT) \
                                     || ((MODE) == ECCP_OUTPUT_COMPLEMENTARY))

/**
  * ECCP通道输出控制
  */
#define ECCP_CHANNEL_OUTPUT_PWM_ACTIVE   ((uint16_t)0x0)
#define ECCP_CHANNEL_OUTPUT_PWM_INACTIVE ((uint16_t)0x1)
#define ECCP_CHANNEL_OUTPUT_INACTIVE     ((uint16_t)0x2)
#define ECCP_CHANNEL_OUTPUT_ACTIVE       ((uint16_t)0x3)
#define CHECK_ECCP_CHANNEL_OUTPUT(CTL)   (((CTL) >> 2) == 0)

/**
  * ATIM从模式选择
  */
#define ATIM_SLAVE_FORBIDDEN_MODE       ((uint32_t)0x0)
#define ATIM_SLAVE_TRIGGER_MODE         ((uint32_t)0x4)
#define ATIM_SLAVE_GATED_MODE           ((uint32_t)0x5)
#define ATIM_SLAVE_RESET_MODE           ((uint32_t)0x6)
#define ATIM_SLAVE_COUNTER_MODE         ((uint32_t)0x7)
#define CHECK_ATIM_SLAVE_MODE(MODE)     (((MODE) == ATIM_SLAVE_FORBIDDEN_MODE) \
                                     || ((MODE) == ATIM_SLAVE_TRIGGER_MODE) \
                                     || ((MODE) == ATIM_SLAVE_GATED_MODE) \
                                     || ((MODE) == ATIM_SLAVE_RESET_MODE) \
                                     || ((MODE) == ATIM_SLAVE_COUNTER_MODE))

/**
  * ATIM主模式选择
  */
#define ATIM_MASTER_TXEN_SIGNAL         ((uint32_t)1 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_TXIF_SIGNAL         ((uint32_t)2 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH1IF_SIGNAL    ((uint32_t)3 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH1_SIGNAL      ((uint32_t)4 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH2_SIGNAL      ((uint32_t)5 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH3_SIGNAL      ((uint32_t)6 << ECCP_CTL2_TXMMS0_POS)
#define ATIM_MASTER_CCPXCH4_SIGNAL      ((uint32_t)7 << ECCP_CTL2_TXMMS0_POS)
#define CHECK_ATIM_MASTER_MODE(MODE)    (((MODE) == ATIM_MASTER_TXEN_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_TXIF_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH1IF_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH1_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH2_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH3_SIGNAL) \
                                     || ((MODE) == ATIM_MASTER_CCPXCH4_SIGNAL))

/**
  * ATIM触发选择
  */
#define ATIM_TRIGGER_T1                 ((uint32_t)0x0 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_T20                ((uint32_t)0x1 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_T5                 ((uint32_t)0x2 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_T9                 ((uint32_t)0x3 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_ECCPXCH1           ((uint32_t)0x4 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_ECCPXCH2           ((uint32_t)0x5 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_ECCPXCH3           ((uint32_t)0x6 << ECCP_CTL2_TXTS0_POS)
#define ATIM_TRIGGER_TXCK               ((uint32_t)0x7 << ECCP_CTL2_TXTS0_POS)
#define CHECK_ATIM_TRIGGER_MODE(MODE)   (((uint32_t)(MODE) & (~ECCP_CTL2_TXTS)) == 0)

/**
  * ECCP通道端口
  */
#define ECCP_PORT_LOW                   ((uint32_t)0x0)
#define ECCP_PORT_HIGH                  ((uint32_t)0x2)
#define CHECK_ECCP_PORT(PORT)           (((PORT) == ECCP_PORT_LOW) \
                                     || ((PORT) == ECCP_PORT_HIGH))

/**
  * ECCP自动关闭事件状态
  */
#define ECCP_CHANNEL_WORKING_STATE      ((uint32_t)0x0)
#define ECCP_CHANNEL_SHUTDOWN_STATE     ((uint32_t)0x1)
#define CHECK_ECCP_CHANNEL_STATE(STATE) (((STATE) == ECCP_CHANNEL_SHUTDOWN_STATE) \
                                      || ((STATE) == ECCP_CHANNEL_WORKING_STATE))

/**
  * 通道自动关闭源
  */
#define ECCP_CHANNEL_SHUTDOWN_FORBID    ((uint32_t)0x0)
#define ECCP_CHANNEL_CMP2CMP3_ACTIVE    ((uint32_t)0x1)
#define ECCP_CHANNEL_BKIN_INACTIVE      ((uint32_t)0x2)
#define CHECK_ECCP_SHUTDOWN_SIGNAL(STATE) \
                                        (((STATE) == ECCP_CHANNEL_SHUTDOWN_FORBID) \
                                        || ((STATE) == ECCP_CHANNEL_CMP2CMP3_ACTIVE)\
                                      || ((STATE) == ECCP_CHANNEL_BKIN_INACTIVE))

/**
  * 零点检测时钟
  */
#define ECCP_ZERO_CLOCK_DIV_1           ((uint32_t)0x0 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_2           ((uint32_t)0x1 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_4           ((uint32_t)0x2 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_8           ((uint32_t)0x3 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_16          ((uint32_t)0x4 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_32          ((uint32_t)0x5 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_64          ((uint32_t)0x6 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_128         ((uint32_t)0x7 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_256         ((uint32_t)0x8 << ECCP_ZPDCTL0_ZCLK0_POS)
#define ECCP_ZERO_CLOCK_DIV_512         ((uint32_t)0x9 << ECCP_ZPDCTL0_ZCLK0_POS)
#define CHECK_ECCP_ZERO_CLOCK(CLK)      (((CLK) == ECCP_ZERO_CLOCK_DIV_1) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_2) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_4) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_8) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_16) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_32) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_64) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_128) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_256) \
                                      || ((CLK) == ECCP_ZERO_CLOCK_DIV_512))

/**
  * 通道零点检测电压
  */
#define ECCP_ZERO_VOLTAGE_DECIMAL_15    ((uint32_t)0x0)
#define ECCP_ZERO_VOLTAGE_DECIMAL_25    ((uint32_t)0x1)
#define ECCP_ZERO_VOLTAGE_DECIMAL_35    ((uint32_t)0x2)
#define ECCP_ZERO_VOLTAGE_DECIMAL_45    ((uint32_t)0x3)
#define ECCP_ZERO_VOLTAGE_DECIMAL_55    ((uint32_t)0x4)
#define CHECK_ECCP_ZERO_VOLTAGE(VOL)    (((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_15) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_25) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_35) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_45) \
                                     || ((VOL) == ECCP_ZERO_VOLTAGE_DECIMAL_55))
/**
  * 高级定时器、ECCP函数定义
  */
/* 高级定时器(ATIM)初始化及配置函数定义***********************************/
void ATIM_X_Configuration(ATIM_SFRmap* ATIMx, ATIM_InitTypeDef* atimInitStruct);
void ATIM_Z_Configuration(ATIM_SFRmap* ATIMx, ATIM_InitTypeDef* atimInitStruct);
void ATIM_Struct_Init (ATIM_InitTypeDef* atimInitStruct);
void ATIM_X_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Updata_Configuration (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter,
                    uint32_t UpdataOutput, uint32_t UpdataImmediately);
void ATIM_Z_Updata_Configuration (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter,
                    uint32_t UpdataOutput, uint32_t UpdataImmediately);
void ATIM_X_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter);
void ATIM_Z_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter);
void ATIM_X_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period);
void ATIM_Z_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period);
void ATIM_X_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler);
void ATIM_Z_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler);
void ATIM_X_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode);
void ATIM_Z_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode);
void ATIM_X_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock);
void ATIM_Z_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock);
void ATIM_X_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler);
void ATIM_Z_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler);
void ATIM_X_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync);
void ATIM_Z_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync);
void ATIM_X_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState);
void ATIM_Z_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState);
DIRStatus ATIM_X_Get_Direction (ATIM_SFRmap* ATIMx);
DIRStatus ATIM_Z_Get_Direction (ATIM_SFRmap* ATIMx);
void ATIM_X_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx,
                    uint16_t CompareAD0, uint16_t CompareAD1);
void ATIM_Z_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx, uint16_t CompareAD);
void ATIM_X_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
void ATIM_Z_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
void ATIM_X_Updata_Output_Ctl (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_Output_Ctl (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter);
void ATIM_Z_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter);
void ATIM_X_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode);
void ATIM_Z_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode);
void ATIM_Master_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t MasterMode);
void ATIM_Master_Slave_Snyc_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
void ATIM_Trigger_Select_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState TriggerSelect);
void ATIM_Timer_Unite_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
uint16_t ATIM_X_Get_Counter (ATIM_SFRmap* ATIMx);
uint16_t ATIM_Z_Get_Counter (ATIM_SFRmap* ATIMx);
uint16_t ATIM_X_Get_Period (ATIM_SFRmap* ATIMx);
uint16_t ATIM_Z_Get_Period (ATIM_SFRmap* ATIMx);
uint16_t ATIM_X_Get_Prescaler (ATIM_SFRmap* ATIMx);
uint16_t ATIM_Z_Get_Prescaler (ATIM_SFRmap* ATIMx);
/* 高级定时器(ATIM)中断及标志管理函数定义*********************************/
void ATIM_X_Updata_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Trigger_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_Z_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
void ATIM_X_Trigger_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState);
FlagStatus ATIM_X_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_Z_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_X_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_Z_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_X_Get_Trigger_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Generate_Trigger_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState);
FlagStatus ATIM_X_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_Z_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ATIM_X_Get_Trigger_DMA_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_Z_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_Z_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx);
void ATIM_X_Clear_Trigger_INT_Flag (ATIM_SFRmap* ATIMx);
/* 增强型捕捉/比较/PWM外设(ECCP)捕捉功能函数定义**************************/
void ECCP_Compare_Configuration(ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t CompareMode, uint16_t CompareValue);
void ECCP_Capture_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_CaptureInitTypeDef* eccpInitStruct);
void ECCP_Capture_Struct_Init (ECCP_CaptureInitTypeDef* eccpInitStruct);
void ECCP_PWM_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_PWMInitTypeDef* ECCP_InitStruct);
void ECCP_PWM_Struct_Init (ECCP_PWMInitTypeDef* ECCP_InitStruct);
void ECCP_Capture_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t EdgeConfig);
void ECCP_Compare_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t CmpConfig);
void ECCP_PWM_Mode_Config (ECCP_SFRmap* ECCPx, uint32_t PWMConfig);
uint16_t ECCP_Get_Capture_Result (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_Set_Compare_Result (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint16_t Value);
void ECCP_Generate_Trigger_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, FunctionalState NewState);
void ECCP_PWM_Input_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Input_XOR_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Single_Pulse_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Single_Pulse_Shut_Enable (ECCP_SFRmap* ECCPx,
                    FunctionalState NewState);
void ECCP_PWM_Restart_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Dead_Time_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint8_t DeadTime);
void ECCP_Channel_Output_Control (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelOutputCtl);
void ECCP_Channel_Output_Mode (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t ChannelOutputMode);
void ECCP_Channel_Work_State_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t WorkingState);
FlagStatus ECCP_Get_Channel_Work_State (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_Channel_Shutdown_Signal (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t ShutDownSignal);
void ECCP_Channel_Pin_Ctl (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelPinCtl);
void ECCP_Zero_Clock_Config (ECCP_SFRmap* ECCPx, uint32_t ZeroClock);
void ECCP_Channel_Pin_Tristate_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t Port, uint32_t PinTristateCtl);
/* 增强型捕捉/比较/PWM外设(ECCP)中断及标志管理函数定义********************/
void ECCP_Channel_INT_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, FunctionalState NewState);
void ECCP_X_Turn_off_DMA_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Channel_DMA_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, FunctionalState NewState);
FlagStatus ECCP_Get_Channel_Trigger_INT_Flag (ECCP_SFRmap* ECCPx,
                    uint32_t Channel);
FlagStatus ECCP_X_Get_Turn_off_DMA_Flag (ATIM_SFRmap* ATIMx);
FlagStatus ECCP_Get_Trigger_DMA_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_Clear_Channel_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel);
void ECCP_PWM_Move_Phase_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState);
void ECCP_Channel_Zero_Detect_Sequential_Ctl (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t NewState);
FlagStatus ECCP_Get_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel);
void ECCP_Clear_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel);
void ECCP_Channel_Zero_Detect_Enable (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t NewState);
void ECCP_Channel_Zero_Voltage_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t ZeroDetectVoltage);

/*** 高级定时器(ATIM)、ECCP相关定义结束  */


/*** 增强型脉宽调制模块(EPWM)及相关定时器定义  */
/**
  * 描述  增强型脉宽调制模块定时器(ETIM)定时器配置信息结构体
  */
typedef struct
{
    uint32_t m_Counter;         /* 定时器计数值，取值32位数据。 */
    uint32_t m_Period;          /* 定时器周期值，取值32位数据。 */
    uint32_t m_Prescaler;       /* 定时器预分频值，取值32位数据。 */
    uint32_t m_Phase;           /* 定时器相位值，取值32位数据。 */
    uint32_t m_CounterMode;     /* 定时器计数模式，
                                   取值为宏“ETIM定时器计数模式”中的一个 */
    uint32_t m_Sync;            /* Tx计数模式外部触发脉冲输入同步控制，
                                   取值为宏“ETIM计数模式外部触发脉冲输入同步控制”中的一个 */
    uint32_t m_Clock;           /* 定时器工作时钟，
                                   取值为宏“ETIM定时器工作时钟”中的一个 */
    uint32_t m_WorkMode;        /* 定时/计数模式选择
                                   取值为宏“ETIM定时/计数模式选择”中的一个 */
    FunctionalState m_PhaseEn;  /* 相位寄存器载入使能,
                                   取值为TRUE 或 FALSE。 */
    uint32_t m_PhaseDirection;  /* 相位方向控制，
                                   取值为宏“ETIM相位方向控制”中的一个 */
    FunctionalState m_GateEn;  /* 门控使能,
                                   取值为TRUE 或 FALSE。 */
} ETIM_InitTypeDef; /* T11、T12、T13 */

/**
  * ETIM相位方向控制
  */
#define ETIM_PHASE_DOWN                 ((uint32_t)0x0 << EPWM_TXCTL_PHSDIR_POS)
#define ETIM_PHASE_UP                   ((uint32_t)0x1 << EPWM_TXCTL_PHSDIR_POS)
#define CHECK_ETIM_PHASE_DIR(DIR)       (((DIR) == ETIM_PHASE_UP) \
                                      || ((DIR) == ETIM_PHASE_DOWN))

/**
  * ETIM定时器计数模式
  */
#define ETIM_COUNT_DOWN_UF              ((uint32_t)0x0 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_OF                ((uint32_t)0x4 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_DOWN_OF           ((uint32_t)0x1 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_DOWN_UF           ((uint32_t)0x2 << EPWM_TXCTL_TXCMS0_POS)
#define ETIM_COUNT_UP_DOWN_OUF          ((uint32_t)0x3 << EPWM_TXCTL_TXCMS0_POS)
#define CHECK_ETIM_COUNTER_MODE(MODE)   (((MODE) == ETIM_COUNT_DOWN_UF) \
                                      || ((MODE) == ETIM_COUNT_UP_OF) \
                                      || ((MODE) == ETIM_COUNT_UP_DOWN_OF) \
                                      || ((MODE) == ETIM_COUNT_UP_DOWN_UF) \
                                      || ((MODE) == ETIM_COUNT_UP_DOWN_OUF))

/**
  * ETIM定时器工作时钟
  */
#define ETIM_SCLK                       ((uint32_t)0x0 << EPWM_TXCTL_TXCLK0_POS)
#define ETIM_HFCLK                      ((uint32_t)0x1 << EPWM_TXCTL_TXCLK0_POS)
#define ETIM_LFCLK                      ((uint32_t)0x2 << EPWM_TXCTL_TXCLK0_POS)
#define CHECK_ETIM_CLOCK_CONFIG(CLK)    (((CLK) == ETIM_SCLK) \
                                      || ((CLK) == ETIM_HFCLK) \
                                      || ((CLK) == ETIM_LFCLK))

/**
  * ETIM定时/计数模式选择
  */
#define ETIM_TIMER_MODE                 ((uint32_t)0 << EPWM_TXCTL_TXCS_POS)
#define ETIM_COUNTER_MODE               ((uint32_t)1 << EPWM_TXCTL_TXCS_POS)
#define CHECK_ETIM_WORK_MODE(MODE)      (((MODE) == ETIM_TIMER_MODE) \
                                      || ((MODE) == ETIM_COUNTER_MODE))

/**
  * ETIM计数模式外部触发脉冲输入同步控制
  */
#define ETIM_EX_SYNC_MODE               ((uint32_t)0x0 << EPWM_TXCTL_TXSY_POS)
#define ETIM_NO_SYNC_MODE               ((uint32_t)0x1 << EPWM_TXCTL_TXSY_POS)
#define CHECK_ETIM_SYNC_MODE(MODE)      (((MODE) == ETIM_EX_SYNC_MODE) \
                                      || ((MODE) == ETIM_NO_SYNC_MODE))

#endif /* _KF32A_BASIC_TIM_H */
