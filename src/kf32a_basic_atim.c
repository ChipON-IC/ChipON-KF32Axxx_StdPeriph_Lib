/**
  ******************************************************************************
  * 文件名  kf32a_basic_atim.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了高级定时器外设功能函数，包含：
  *          + 高级定时器(ATIM)初始化及配置函数定义
  *          + 高级定时器(ATIM)中断及标志管理函数定义
  *          + 增强型捕捉/比较/PWM外设(ECCP)捕捉功能函数定义
  *          + 增强型捕捉/比较/PWM外设(ECCP)中断及标志管理函数定义
  *
  *********************************************************************
  */

#include "kf32a_basic_tim.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* 定时器私有定义 ----------------------------------------------------*/
/* ATIMx_TXCTL寄存器配置掩码 */
#define ATIM_TXCTL_INIT_MASK            (ATIM_TXCTL_TXCS \
                                       | ATIM_TXCTL_TXSY \
                                       | ATIM_TXCTL_TXCKBS \
                                       | ATIM_TXCTL_TXCLK \
                                       | ATIM_TXCTL_TXCMS)

/* ATIMx_TZCTL寄存器配置掩码 */
#define ATIM_TZCTL_INIT_MASK            (ATIM_TZCTL_TZCS \
                                       | ATIM_TZCTL_TZSY \
                                       | ATIM_TZCTL_TZCKBS \
                                       | ATIM_TZCTL_TZCLK \
                                       | ATIM_TZCTL_TZCMS)

/* ATIMx_PXUDCTL以Tx为时基的寄存器更新配置掩码 */
#define ATIM_X_PXUDCTL_INIT_MASK  (ECCP_PXUDCTL_PXUDEVT0 \
                                  | ECCP_PXUDCTL_PXUDEVT1)

/* ATIMx_PXUDCTL以Tz为时基的寄存器更新配置掩码 */
#define ATIM_Z_PXUDCTL_INIT_MASK  (ECCP_PXUDCTL_PXUDEVT2 \
                                  | ECCP_PXUDCTL_PXUDEVT3)

/* ECCPX_CTL2寄存器初始化掩码 */
#define ECCP_CTL2_INIT_MASK             (ECCP_CTL2_PXPWMI \
                                       | ECCP_CTL2_PXXORM)

/* ECCPXRY寄存器偏移地址 */
#define ECCP_RY_OFFSET                   ((uint32_t)0x84)

/* ECCPXCY寄存器偏移地址 */
#define ECCP_CY_OFFSET                   ((uint32_t)0xD0)


/**
  *   ##### 高级定时器(ATIM)初始化及配置函数定义 #####
  */
/**
  * 描述  高级定时器(ATIM)配置，配置Tx。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       atimInitStruct: 高级定时器配置信息结构体指针。
  * 返回  无
  */
void ATIM_X_Configuration(ATIM_SFRmap* ATIMx,
                    ATIM_InitTypeDef* atimInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(atimInitStruct->m_Postscaler));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(atimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(atimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(atimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(atimInitStruct->m_EXPulseSync));

    /*---------------- 设置TxCNT寄存器 ----------------*/
    ATIMx->TXCNT = (uint32_t)atimInitStruct->m_Counter;

    /*--------------- 设置PPx周期寄存器 ---------------*/
    ATIMx->TXPPX = (uint32_t)atimInitStruct->m_Period;

    /*-------------- 设置Tx预分频寄存器 ---------------*/
    ATIMx->TXPRSC = (uint32_t)atimInitStruct->m_Prescaler;

    /*------------------ 设置TXCTL寄存器 -----------------*/
    /* 根据结构体成员m_Postscaler，设置TXCKBS位域 */
    /* 根据结构体成员m_CounterMode，设置TXCMS位域 */
    /* 根据结构体成员m_Clock，设置TXCLK位域 */
    /* 根据结构体成员m_WorkMode，设置TXCS位域 */
    /* 根据结构体成员m_EXPulseSync，设置TXSY位域 */
    tmpreg = ((uint32_t)atimInitStruct->m_Postscaler << ATIM_TXCTL_TXCKBS0_POS) \
             | (atimInitStruct->m_CounterMode) \
             | ((uint32_t)atimInitStruct->m_Clock) \
             | ((uint32_t)atimInitStruct->m_WorkMode) \
             | ((uint32_t)atimInitStruct->m_EXPulseSync);
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  高级定时器(ATIM)配置，配置Tz。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       atimInitStruct: 高级定时器配置信息结构体指针。
  * 返回  无
  */
void ATIM_Z_Configuration(ATIM_SFRmap* ATIMx,
                    ATIM_InitTypeDef* atimInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(atimInitStruct->m_Postscaler));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(atimInitStruct->m_CounterMode));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(atimInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(atimInitStruct->m_WorkMode));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(atimInitStruct->m_EXPulseSync));

    /*---------------- 设置TxCNT寄存器 ----------------*/
    ATIMx->TZCNT = (uint32_t)atimInitStruct->m_Counter;

    /*--------------- 设置PPx周期寄存器 ---------------*/
    ATIMx->TZPPZ = (uint32_t)atimInitStruct->m_Period;

    /*-------------- 设置Tx预分频寄存器 ---------------*/
    ATIMx->TZPRSC = (uint32_t)atimInitStruct->m_Prescaler;

    /*------------------ 设置TXCTL寄存器 -----------------*/
    /* 根据结构体成员m_Postscaler，设置TXCKBS位域 */
    /* 根据结构体成员m_CounterMode，设置TXCMS位域 */
    /* 根据结构体成员m_Clock，设置TXCLK位域 */
    /* 根据结构体成员m_WorkMode，设置TXCS位域 */
    /* 根据结构体成员m_EXPulseSync，设置TXSY位域 */
    tmpreg = ((uint32_t)atimInitStruct->m_Postscaler << ATIM_TZCTL_TZCKBS0_POS) \
             | (atimInitStruct->m_CounterMode) \
             | ((uint32_t)atimInitStruct->m_Clock) \
             | ((uint32_t)atimInitStruct->m_WorkMode) \
             | ((uint32_t)atimInitStruct->m_EXPulseSync);
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化高级定时器配置信息结构体。
  * 输入  atimInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
ATIM_Struct_Init (ATIM_InitTypeDef* atimInitStruct)
{
    /* 设置Tx计数值 */
    atimInitStruct->m_Counter = 0x00000000;
    /* 设置Tx周期值 */
    atimInitStruct->m_Period = 0x0000FFFF;
    /* 设置Tx预分频值 */
    atimInitStruct->m_Prescaler = 0x00000000;
    /* 设置Tx后分频值 */
    atimInitStruct->m_Postscaler = 0x0;
    /* 设置Tx计数模式 */
    atimInitStruct->m_CounterMode = ATIM_COUNT_UP_OF;
    /* 设置Tx工作时钟 */
    atimInitStruct->m_Clock = ATIM_SCLK;
    /* 设置定时/计数模式选择 */
    atimInitStruct->m_WorkMode = ATIM_TIMER_MODE;
    /* 设置Tx计数模式外部触发脉冲输入同步控制 */
    atimInitStruct->m_EXPulseSync = ATIM_EX_SYNC_MODE;
}

/**
  * 描述  定时器Tx启动控制使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: 定时器使能控制，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL寄存器TXEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能Tx定时器 */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXEN_POS);
    }
    else
    {
        /* 禁止Tx定时器 */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXEN_POS);
    }
}

/**
  * 描述  定时器Tz启动控制使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: 定时器使能控制，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TZCTL寄存器TZEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能Tz定时器 */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZEN_POS);
    }
    else
    {
        /* 禁止Tz定时器 */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZEN_POS);
    }
}

/**
  * 描述  高级定时器(ATIM)更新配置，配置Tx。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       UpdataCounter: 高级定时器更新计数器的值，取值8位数据。
  *       UpdataOutput: 更新输出控制寄存器使能控制，
  *                       取值为TRUE或FALSE。
  *       UpdataImmediately: 立即更新定时器使能控制，
  *                            取值为TRUE或FALSE。
  * 返回  无
  */
void
ATIM_X_Updata_Configuration (ATIM_SFRmap* ATIMx,
                    uint8_t UpdataCounter,
                    uint32_t UpdataOutput,
                    uint32_t UpdataImmediately)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataOutput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataImmediately));

    /*-------------- 设置TXUDTIM更新计数寄存器 ---------------*/
    ATIMx->TXUDTIM = UpdataCounter;

    /*------------------ 设置PXUDCTL寄存器 -----------------*/
    /* 根据结构体成员UpdataOutput，设置PXUDEVT0位域 */
    /* 根据结构体成员UpdataImmediately，设置PXUDEVT1位域 */
    tmpreg = (UpdataOutput << ECCP_PXUDCTL_PXUDEVT0_POS) \
             | (UpdataImmediately << ECCP_PXUDCTL_PXUDEVT1_POS);
    ATIMx->PXUDCTL = SFR_Config (ATIMx->PXUDCTL,
                          ~ATIM_X_PXUDCTL_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  高级定时器(ATIM)更新配置，配置Tz。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       UpdataCounter: 高级定时器更新计数器的值，取值8位数据。
  *       UpdataOutput: 更新输出控制寄存器使能控制，
  *                       取值为TRUE或FALSE。
  *       UpdataImmediately: 立即更新定时器使能控制，
  *                            取值为TRUE或FALSE。
  * 返回  无
  */
void
ATIM_Z_Updata_Configuration (ATIM_SFRmap* ATIMx,
                    uint8_t UpdataCounter,
                    uint32_t UpdataOutput,
                    uint32_t UpdataImmediately)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataOutput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(UpdataImmediately));

    /*-------------- 设置TXUDTIM更新计数寄存器 ---------------*/
    ATIMx->TZUDTIM = UpdataCounter;

    /*------------------ 设置PXUDCTL寄存器 -----------------*/
    /* 根据结构体成员UpdataOutput，设置PXUDEVT2位域 */
    /* 根据结构体成员UpdataImmediately，设置PXUDEVT3位域 */
    tmpreg = (UpdataOutput << ECCP_PXUDCTL_PXUDEVT2_POS) \
             | (UpdataImmediately << ECCP_PXUDCTL_PXUDEVT3_POS);
    ATIMx->PXUDCTL = SFR_Config (ATIMx->PXUDCTL,
                          ~ATIM_Z_PXUDCTL_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  定时器Tx更新使能控制。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: 定时器Tx更新使能控制，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEN0位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能Tx定时器更新 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
    else
    {
        /* 禁止Tx定时器更新 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
}

/**
  * 描述  定时器Tz更新使能控制。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: 定时器Tz更新使能控制，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Updata_Cmd (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEN1位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能Tz定时器 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
    else
    {
        /* 禁止Tz定时器 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
}

/**
  * 描述  配置定时器Tx计数值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       Counter: 计数值，取值16位数据。
  * 返回  无
  */
void
ATIM_X_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TXCNT寄存器 ----------------*/
    ATIMx->TXCNT = Counter;
}

/**
  * 描述  配置定时器Tz计数值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       Counter: 计数值，取值16位数据。
  * 返回  无
  */
void
ATIM_Z_Set_Counter (ATIM_SFRmap* ATIMx, uint16_t Counter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TZCNT寄存器 ----------------*/
    ATIMx->TZCNT = Counter;
}

/**
  * 描述  配置定时器Tx周期值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       Period: 周期值，取值16位数据。
  * 返回  无
  */
void
ATIM_X_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TXPPX寄存器 ----------------*/
    ATIMx->TXPPX = Period;
}

/**
  * 描述  配置定时器Tz周期值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       Period: 周期值，取值16位数据。
  * 返回  无
  */
void
ATIM_Z_Set_Period (ATIM_SFRmap* ATIMx, uint16_t Period)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TZPPZ寄存器 ----------------*/
    ATIMx->TZPPZ = Period;
}

/**
  * 描述  配置定时器Tx预分频值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       Prescaler: 预分频，取值16位数据。
  * 返回  无
  */
void
ATIM_X_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TXPRSC寄存器 ----------------*/
    ATIMx->TXPRSC = Prescaler;
}

/**
  * 描述  配置定时器Tz预分频值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       Prescaler: 预分频，取值16位数据。
  * 返回  无
  */
void
ATIM_Z_Set_Prescaler (ATIM_SFRmap* ATIMx, uint16_t Prescaler)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TZPRSC寄存器 ----------------*/
    ATIMx->TZPRSC = Prescaler;
}

/**
  * 描述  配置定时器Tx计数模式。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       CounterMode: 新的计数模式，
  *                    取值范围为：
  *                      ATIM_COUNT_DOWN_UF: 向下计数,下溢产生中断标志
  *                      ATIM_COUNT_UP_OF: 向上计数,上溢产生中断标志
  *                      ATIM_COUNT_UP_DOWN_OF: 向上-向下计数,上溢产生中断标志
  *                      ATIM_COUNT_UP_DOWN_UF: 向上-向下计数,下溢产生中断标志
  *                      ATIM_COUNT_UP_DOWN_OUF: 向上-向下计数,上溢和下溢产生中断标志
  * 返回  无
  */
void
ATIM_X_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(CounterMode));

    /*-------------------- 设置TXCTL寄存器TXCMS位 --------------------*/
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_TXCMS,
                          CounterMode);
}

/**
  * 描述  配置定时器Tz计数模式。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       CounterMode: 新的计数模式，
  *                    取值范围为：
  *                      ATIM_COUNT_DOWN_UF: 向下计数,下溢产生中断标志
  *                      ATIM_COUNT_UP_OF: 向上计数,上溢产生中断标志
  *                      ATIM_COUNT_UP_DOWN_OF: 向上-向下计数,上溢产生中断标志
  *                      ATIM_COUNT_UP_DOWN_UF: 向上-向下计数,下溢产生中断标志
  *                      ATIM_COUNT_UP_DOWN_OUF: 向上-向下计数,上溢和下溢产生中断标志
  * 返回  无
  */
void
ATIM_Z_Counter_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t CounterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_COUNTER_MODE(CounterMode));

    /*-------------------- 设置TZCTL寄存器TZCMS位 --------------------*/
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_TZCMS,
                          CounterMode);
}

/**
  * 描述  配置定时器Tx工作时钟。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewClock: 定时器Tx工作时钟，
  *                 取值范围为：
  *                   ATIM_SCLK: 选用SCLK时钟
  *                   ATIM_HFCLK: 选用HFCLK时钟
  *                   ATIM_LFCLK: 选用LFCLK时钟
  * 返回  无
  */
void
ATIM_X_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(NewClock));

    /*-------------------- 设置TXCTL寄存器TXCLK位 --------------------*/
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_TXCLK,
                          NewClock);
}

/**
  * 描述  配置定时器Tz工作时钟。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewClock: 定时器Tz工作时钟，
  *                 取值范围为：
  *                   ATIM_SCLK: 选用SCLK时钟
  *                   ATIM_HFCLK: 选用HFCLK时钟
  *                   ATIM_LFCLK: 选用LFCLK时钟
  * 返回  无
  */
void
ATIM_Z_Clock_Config (ATIM_SFRmap* ATIMx, uint32_t NewClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_CLOCK_CONFIG(NewClock));

    /*-------------------- 设置TZCTL寄存器TZCLK位 --------------------*/
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_TZCLK,
                          NewClock);
}

/**
  * 描述  配置定时器Tx的后分频器。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewPostscaler: 定时器Tx的后分频器，取值为4位数据，范围如下：
  *                   ATIM_POSTSCALER_DIV_1: 分频比为1:1
  *                   ATIM_POSTSCALER_DIV_2: 分频比为1:2
  *                   ATIM_POSTSCALER_DIV_3: 分频比为1:3
  *                   ATIM_POSTSCALER_DIV_4: 分频比为1:4
  *                   ATIM_POSTSCALER_DIV_5: 分频比为1:5
  *                   ATIM_POSTSCALER_DIV_6: 分频比为1:6
  *                   ATIM_POSTSCALER_DIV_7: 分频比为1:7
  *                   ATIM_POSTSCALER_DIV_8: 分频比为1:8
  *                   ATIM_POSTSCALER_DIV_9: 分频比为1:9
  *                   ATIM_POSTSCALER_DIV_10: 分频比为1:10
  *                   ATIM_POSTSCALER_DIV_11: 分频比为1:11
  *                   ATIM_POSTSCALER_DIV_12: 分频比为1:12
  *                   ATIM_POSTSCALER_DIV_13: 分频比为1:13
  *                   ATIM_POSTSCALER_DIV_14: 分频比为1:14
  *                   ATIM_POSTSCALER_DIV_15: 分频比为1:15
  *                   ATIM_POSTSCALER_DIV_16: 分频比为1:16
  * 返回  无
  */
void
ATIM_X_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(NewPostscaler));

    /*-------------------- 设置TXCTL寄存器TXCKBS位 --------------------*/
    tmpreg = NewPostscaler << ATIM_TXCTL_TXCKBS0_POS;
    ATIMx->TXCTL = SFR_Config (ATIMx->TXCTL,
                          ~ATIM_TXCTL_TXCKBS,
                          tmpreg);
}

/**
  * 描述  配置定时器Tz的后分频器。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewPostscaler: 定时器Tz的后分频器，取值为4位数据，范围如下：
  *                   ATIM_POSTSCALER_DIV_1: 分频比为1:1
  *                   ATIM_POSTSCALER_DIV_2: 分频比为1:2
  *                   ATIM_POSTSCALER_DIV_3: 分频比为1:3
  *                   ATIM_POSTSCALER_DIV_4: 分频比为1:4
  *                   ATIM_POSTSCALER_DIV_5: 分频比为1:5
  *                   ATIM_POSTSCALER_DIV_6: 分频比为1:6
  *                   ATIM_POSTSCALER_DIV_7: 分频比为1:7
  *                   ATIM_POSTSCALER_DIV_8: 分频比为1:8
  *                   ATIM_POSTSCALER_DIV_9: 分频比为1:9
  *                   ATIM_POSTSCALER_DIV_10: 分频比为1:10
  *                   ATIM_POSTSCALER_DIV_11: 分频比为1:11
  *                   ATIM_POSTSCALER_DIV_12: 分频比为1:12
  *                   ATIM_POSTSCALER_DIV_13: 分频比为1:13
  *                   ATIM_POSTSCALER_DIV_14: 分频比为1:14
  *                   ATIM_POSTSCALER_DIV_15: 分频比为1:15
  *                   ATIM_POSTSCALER_DIV_16: 分频比为1:16
  * 返回  无
  */
void
ATIM_Z_Postscaler_Config (ATIM_SFRmap* ATIMx, uint32_t NewPostscaler)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_POSTSCALER(NewPostscaler));

    /*-------------------- 设置TZCTL寄存器TZCKBS位 --------------------*/
    tmpreg = NewPostscaler << ATIM_TZCTL_TZCKBS0_POS;
    ATIMx->TZCTL = SFR_Config (ATIMx->TZCTL,
                          ~ATIM_TZCTL_TZCKBS,
                          tmpreg);
}

/**
  * 描述  配置定时器Tx计数模式外部触发脉冲输入同步控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewClock: 定时器Tx计数模式外部触发脉冲输入同步控制位使能状态，
  *                 取值范围为：
  *                   ATIM_EX_SYNC_MODE: 与外部触发脉冲输入同步
  *                   ATIM_NO_SYNC_MODE: 不与外部触发脉冲输入同步
  * 返回  无
  */
void
ATIM_X_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(PulseSync));

    /*-------------------- 设置TXCTL寄存器TXSY位 --------------------*/
    if (PulseSync != ATIM_EX_SYNC_MODE)
    {
        /* 不与外部触发脉冲输入同步 */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXSY_POS);
    }
    else
    {
        /* 与外部触发脉冲输入同步 */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXSY_POS);
    }
}

/**
  * 描述  配置定时器Tz计数模式外部触发脉冲输入同步控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewClock: 定时器Tz计数模式外部触发脉冲输入同步控制位使能状态，
  *                 取值范围为：
  *                   ATIM_EX_SYNC_MODE: 与外部触发脉冲输入同步
  *                   ATIM_NO_SYNC_MODE: 不与外部触发脉冲输入同步
  * 返回  无
  */
void
ATIM_Z_External_Pulse_Sync_Config (ATIM_SFRmap* ATIMx, uint32_t PulseSync)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SYNC_MODE(PulseSync));

    /*-------------------- 设置TZCTL寄存器TZSY位 --------------------*/
    if (PulseSync != ATIM_EX_SYNC_MODE)
    {
        /* 不与外部触发脉冲输入同步 */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZSY_POS);
    }
    else
    {
        /* 与外部触发脉冲输入同步 */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZSY_POS);
    }
}

/**
  * 描述  配置Tx定时/计数模式选择。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: Tx定时/计数模式选择，
  *                 取值范围为：
  *                   ATIM_TIMER_MODE: 定时模式
  *                   ATIM_COUNTER_MODE: 计数模式
  * 返回  无
  */
void
ATIM_X_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(NewState));

    /*-------------------- 设置TXCTL寄存器TXCS位 --------------------*/
    if (NewState != ATIM_TIMER_MODE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXCS_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXCS_POS);
    }
}

/**
  * 描述  配置Tz定时/计数模式选择。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: Tz定时/计数模式选择，
  *                 取值范围为：
  *                   ATIM_TIMER_MODE: 定时模式
  *                   ATIM_COUNTER_MODE: 计数模式
  * 返回  无
  */
void
ATIM_Z_Work_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_WORK_MODE(NewState));

    /*-------------------- 设置TZCTL寄存器TZCS位 --------------------*/
    if (NewState != ATIM_TIMER_MODE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZCS_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZCS_POS);
    }
}

/**
  * 描述  读Tx计数方向。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  * 返回  Tx计数方向，0：向下，1：向上。
  */
DIRStatus
ATIM_X_Get_Direction (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取TXCTL寄存器TXDIR位 --------------------*/
    if (ATIMx->TXCTL & ATIM_TXCTL_TXDIR)
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
  * 描述  读Tz计数方向。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  * 返回  Tz计数方向，0：向下，1：向上。
  */
DIRStatus
ATIM_Z_Get_Direction (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取TZCTL寄存器TZDIR位 --------------------*/
    if (ATIMx->TZCTL & ATIM_TZCTL_TZDIR)
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
  * 描述  配置Tx上溢中断触发AD使能位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: Tx上溢中断触发AD使能位，
  *                 取值为：TRUE或FALSE。
  * 返回  无
  */
void
ATIM_X_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL寄存器TXOFS1位 --------------------*/
    if (NewState != FALSE)
    {
    	/* 上溢中断使能 */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS1_POS);
    }
    else
    {
        /* 上溢中断禁止 */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS1_POS);
    }
}

/**
  * 描述  配置Tz上溢中断触发AD使能位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: Tz上溢中断触发AD使能位，
  *                 取值为：TRUE或FALSE。
  * 返回  无
  */
void
ATIM_Z_Overflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TZCTL寄存器TZOFS1位 --------------------*/
    if (NewState != FALSE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS1_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS1_POS);
    }
}

/**
  * 描述  配置Tx下溢中断触发AD使能位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: Tx下溢中断触发AD使能位，
  *                 取值为：TRUE或FALSE。
  * 返回  无
  */
void
ATIM_X_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL寄存器TXOFS0位 --------------------*/
    if (NewState != FALSE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS0_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXOFS0_POS);
    }
}

/**
  * 描述  配置Tz下溢中断触发AD使能位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: Tz下溢中断触发AD使能位，
  *                 取值为：TRUE或FALSE。
  * 返回  无
  */
void
ATIM_Z_Underflow_AD_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TZCTL寄存器TZOFS0位 --------------------*/
    if (NewState != FALSE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS0_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZOFS0_POS);
    }
}

/**
  * 描述  配置Tx溢出中断自动触发AD控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: Tx溢出中断自动触发AD控制位，
  *                 取值为：TRUE或FALSE。
  * 返回  无
  */
void
ATIM_X_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TXCTL寄存器TXADAUTO位 --------------------*/
    if (NewState != FALSE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXADAUTO_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TXCTL, ATIM_TXCTL_TXADAUTO_POS);
    }
}

/**
  * 描述  配置Tz溢出中断自动触发AD控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: Tz溢出中断自动触发AD控制位，
  *                 取值为：TRUE或FALSE。
  * 返回  无
  */
void
ATIM_Z_TriggerAD_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置TZCTL寄存器TZADAUTO位 --------------------*/
    if (NewState != FALSE)
    {
        /* 计数模式 */
        SFR_SET_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZADAUTO_POS);
    }
    else
    {
        /* 定时模式 */
        SFR_CLR_BIT_ASM(ATIMx->TZCTL, ATIM_TZCTL_TZADAUTO_POS);
    }
}

/**
  * 描述  配置定时器Tx触发AD寄存器的值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       CompareAD0: Tx触发AD寄存器0的值，取值16位数据。
  *       CompareAD1: Tx触发AD寄存器1的值，取值16位数据。
  * 返回  无
  */
void
ATIM_X_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx,
                    uint16_t CompareAD0, uint16_t CompareAD1)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TXCCR0/TXCCR1寄存器 ----------------*/
    ATIMx->TXCCR0 = CompareAD0;
    ATIMx->TXCCR1 = CompareAD1;
}

/**
  * 描述  配置定时器Tz触发AD寄存器的值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       CompareAD: Tz触发AD寄存器0的值，取值16位数据。
  * 返回  无
  */
void
ATIM_Z_Set_TriggerAD_Signal (ATIM_SFRmap* ATIMx,
                    uint16_t CompareAD)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TZCCR0寄存器 ----------------*/
    ATIMx->TZCCR0 = CompareAD;
}

/**
  * 描述  配置Tx立即更新控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: 立即更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEVT1位 --------------------*/
    if (NewState != FALSE)
    {
        /* 产生更新事件 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT1_POS);
    }
    else
    {
    	  /* 当定时器 UDTIMX 为 0 时更新以 Tx 为时基的占空比、周期、预分频、更新计数器寄存器 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT1_POS);
    }
}

/**
  * 描述  配置Tz立即更新控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: 立即更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Updata_Immediately_Config (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEVT3位 --------------------*/
    if (NewState != FALSE)
    {
        /* 产生更新事件 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT3_POS);
    }
    else
    {
        /* 当定时器UDTIMz为0时更新 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT3_POS);
    }
}

/**
  * 描述  配置Tx输出控制寄存器更新控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: 立即更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Updata_Output_Ctl (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEVT0位 --------------------*/
    if (NewState != FALSE)
    {
        /* 立即更新以Tx为时基的输出控制寄存器 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT0_POS);
    }
    else
    {
        /* 当定时器UDTIMx为0时更新 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT0_POS);
    }
}

/**
  * 描述  配置Tz输出控制寄存器更新控制位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: 立即更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  *
  */
void
ATIM_Z_Updata_Output_Ctl (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEVT2位 --------------------*/
    if (NewState != FALSE)
    {
        /* 立即更新以Tz为时基的输出控制寄存器 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT2_POS);
    }
    else
    {
        /* 当定时器UDTIMz为0时更新 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEVT2_POS);
    }
}

/**
  * 描述  配置Tx更新使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: 更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEN0位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能更新 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
    else
    {
        /* 禁止更新 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN0_POS);
    }
}

/**
  * 描述  配置Tz更新使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       NewState: 更新使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Updata_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_PXUDCTL寄存器PXUDEN1位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能更新 */
        SFR_SET_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
    else
    {
        /* 禁止更新 */
        SFR_CLR_BIT_ASM(ATIMx->PXUDCTL, ECCP_PXUDCTL_PXUDEN1_POS);
    }
}

/**
  * 描述  配置定时器Tx更新计数器。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       UpdataCounter: 更新计数值，取值8位数据。
  * 返回  无
  */
void
ATIM_X_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TXUDTIM寄存器 ----------------*/
    ATIMx->TXUDTIM = UpdataCounter;
}

/**
  * 描述  配置定时器Tz更新计数器。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       UpdataCounter: 更新计数值，取值8位数据。
  * 返回  无
  */
void
ATIM_Z_Set_Updata_Counter (ATIM_SFRmap* ATIMx, uint8_t UpdataCounter)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*---------------- 更新TZUDTIM寄存器 ----------------*/
    ATIMx->TZUDTIM = UpdataCounter;
}

/**
  * 描述  配置Tx从模式选择位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       SlaveSelect: 从模式选择，
  *                    取值范围为：
  *                      ATIM_SLAVE_FORBIDDEN_MODE: 从模式禁止
  *                      ATIM_SLAVE_TRIGGER_MODE: 触发模式
  *                      ATIM_SLAVE_GATED_MODE: 门控模式
  *                      ATIM_SLAVE_RESET_MODE: 复位模式
  *                      ATIM_SLAVE_COUNTER_MODE: 计数模式2
  * 返回  无
  */
void
ATIM_X_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SLAVE_MODE(SlaveMode));

    /*-------------------- 设置ECCP_CTL2寄存器TXSMS位 --------------------*/
    tmpreg = SlaveMode << ECCP_CTL2_TXSMS0_POS;
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TXSMS,
                          tmpreg);
}

/**
  * 描述  配置Tz从模式选择位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  *       SlaveSelect: 从模式选择，
  *                    取值范围为：
  *                      ATIM_SLAVE_FORBIDDEN_MODE: 从模式禁止
  *                      ATIM_SLAVE_TRIGGER_MODE: 触发模式
  *                      ATIM_SLAVE_GATED_MODE: 门控模式
  *                      ATIM_SLAVE_RESET_MODE: 复位模式
  *                      ATIM_SLAVE_COUNTER_MODE: 计数模式2
  * 返回  无
  */
void
ATIM_Z_Slave_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t SlaveMode)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_SLAVE_MODE(SlaveMode));

    /*-------------------- 设置ECCP_CTL2寄存器TZSMS位 --------------------*/
    tmpreg = SlaveMode << ECCP_CTL2_TZSMS0_POS;
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TZSMS,
                          tmpreg);
}

/**
  * 描述  配置Tx主模式选择位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       MasterMode: 主模式选择，取值范围为：
  *                     ATIM_MASTER_TXEN_SIGNAL: 使能信号TXEN作为触发输出TRGO
  *                     ATIM_MASTER_TXIF_SIGNAL: TXIF信号作为触发输出TRGO（定时器溢出时的中断脉冲信号）
  *                     ATIM_MASTER_CCPXCH1IF_SIGNAL: ECCPx的CC1IF脉冲作为触发输出TRGO
  *                     ATIM_MASTER_CCPXCH1_SIGNAL: ECCPxCH1作为触发输出TRGO
  *                     ATIM_MASTER_CCPXCH2_SIGNAL: ECCPxCH2 作为触发输出TRGO
  *                     ATIM_MASTER_CCPXCH3_SIGNAL: ECCPxCH3 作为触发输出TRGO
  *                     ATIM_MASTER_CCPXCH4_SIGNAL: ECCPxCH4 作为触发输出TRGO
  * 返回  无
  */
void
ATIM_Master_Mode_Config (ATIM_SFRmap* ATIMx, uint32_t MasterMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_MASTER_MODE(MasterMode));

    /*-------------------- 设置ECCP_CTL2寄存器TXMMS位 --------------------*/
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TXMMS,
                          MasterMode);
}

/**
  * 描述  配置主从模式同步位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: 主从模式同步位状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Master_Slave_Snyc_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_CTL2寄存器TXMSSYNC位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能主从模式同步 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXCTL2, ECCP_CTL2_TXMSSYNC_POS);
    }
    else
    {
        /* 未使能 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXCTL2, ECCP_CTL2_TXMSSYNC_POS);
    }
}

/**
  * 描述  配置触发选择位。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       TriggerSelect: 触发选择，
  *                      取值范围为：
  *                        ATIM_TRIGGER_T1
  *                        ATIM_TRIGGER_T20
  *                        ATIM_TRIGGER_T5
  *                        ATIM_TRIGGER_T9
  *                        ATIM_TRIGGER_ECCPXCH1
  *                        ATIM_TRIGGER_ECCPXCH2
  *                        ATIM_TRIGGER_ECCPXCH3
  *                        ATIM_TRIGGER_TXCK
  * 返回  无
  */
void
ATIM_Trigger_Select_Config (ATIM_SFRmap* ATIMx,
                    uint32_t TriggerSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_ATIM_TRIGGER_MODE(TriggerSelect));

    /*-------------------- 设置ECCP_CTL2寄存器TXTS位 --------------------*/
    ATIMx->ECCPXCTL2 = SFR_Config (ATIMx->ECCPXCTL2,
                          ~ECCP_CTL2_TXTS,
                          TriggerSelect);
}

/**
  * 描述  配置Tx、Tz联立使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  *       NewState: Tx、Tz联立使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Timer_Unite_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_CTL3寄存器TUNITE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 联立Tx、Tz */
        SFR_SET_BIT_ASM(ATIMx->ECCPXCTL3, ECCP_CTL3_TUNITE_POS);
    }
    else
    {
        /* 不联立 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXCTL3, ECCP_CTL3_TUNITE_POS);
    }
}

/**
  * 描述  读Tx定时器计数值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  * 返回  Tx计数值，16位
  */
uint16_t
ATIM_X_Get_Counter (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* 读定时器计数值TxCNT */
    tmpreg = ATIMx->TXCNT;

    return tmpreg;
}

/**
  * 描述  读Tz定时器计数值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  * 返回  Tz计数值，16位
  */
uint16_t
ATIM_Z_Get_Counter (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* 读定时器计数值TzCNT */
    tmpreg = ATIMx->TZCNT;

    return tmpreg;
}

/**
  * 描述  读Tx定时器周期值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  * 返回  Tx周期值，16位
  */
uint16_t
ATIM_X_Get_Period (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* 读定时器周期值TxPPX */
    tmpreg = ATIMx->TXPPX;

    return tmpreg;
}

/**
  * 描述  读Tz定时器周期值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  * 返回  Tz周期值，16位
  */
uint16_t
ATIM_Z_Get_Period (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* 读定时器周期值TZPPZ */
    tmpreg = ATIMx->TZPPZ;

    return tmpreg;
}

/**
  * 描述  读Tx定时器预分频值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  * 返回  Tx预分频值，16位
  */
uint16_t
ATIM_X_Get_Prescaler (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* 读定时器预分频值TxPPX */
    tmpreg = ATIMx->TXPRSC;

    return tmpreg;
}

/**
  * 描述  读Tz定时器预分频值。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T6_SFR/T10_SFR。
  * 返回  Tz预分频值，16位
  */
uint16_t
ATIM_Z_Get_Prescaler (ATIM_SFRmap* ATIMx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /* 读定时器预分频值TxPPX */
    tmpreg = ATIMx->TZPRSC;

    return tmpreg;
}

/**
  *   ##### 高级定时器(ATIM)初始化及配置函数定义结束 #####
  */


/**
  *   ##### 高级定时器(ATIM)中断及标志管理函数定义 #####
  */
/**
  * 描述  配置Tx更新事件中断使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  *       NewState: Tx更新事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Updata_INT_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_IE寄存器TXUIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx更新中断 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXUIE_POS);
    }
    else
    {
        /* 禁止Tx更新中断 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXUIE_POS);
    }
}

/**
  * 描述  配置Tz更新事件中断使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  *       NewState: Tz更新事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Updata_INT_Enable (ATIM_SFRmap* ATIMx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_IE寄存器TZUIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tz更新中断 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZUIE_POS);
    }
    else
    {
        /* 禁止Tz更新中断 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZUIE_POS);
    }
}

/**
  * 描述  配置Tx计数溢出中断使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  *       NewState: Tx计数溢出中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_IE寄存器TXIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx计数溢出引起中断 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXIE_POS);
    }
    else
    {
        /* 禁止Tx计数溢出引起中断 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXIE_POS);
    }
}

/**
  * 描述  配置Tz计数溢出中断使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  *       NewState: Tz计数溢出中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Overflow_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_IE寄存器TZIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tz计数溢出引起中断 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZIE_POS);
    }
    else
    {
        /* 禁止Tz计数溢出引起中断 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TZIE_POS);
    }
}

/**
  * 描述  配置Tx触发事件中断使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  *       NewState: Tx触发事件中断，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Trigger_INT_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_IE寄存器TXTIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许Tx触发事件引起中断 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXTIE_POS);
    }
    else
    {
        /* 禁止Tx触发事件引起中断 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXIE, ECCP_IE_TXTIE_POS);
    }
}

/**
  * 描述  配置Tx更新事件的DMA请求使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: Tx更新事件的DMA请求，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_DE寄存器TXUDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许更新事件的DMA请求 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXUDE_POS);
    }
    else
    {
        /* 禁止更新事件的DMA请求 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXUDE_POS);
    }
}

/**
  * 描述  配置Tz更新事件的DMA请求使能。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值ECCP6/ECCP10。
  *       NewState: Tz更新事件的DMA请求，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_Z_Updata_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_DE寄存器TZUDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许更新事件的DMA请求 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TZUDE_POS);
    }
    else
    {
        /* 禁止更新事件的DMA请求 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TZUDE_POS);
    }
}

/**
  * 描述  允许Tx触发事件的DMA请求配置
  * 输入  ECCPx: 指向CCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: Tx触发事件的DMA请求使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Trigger_DMA_Enable (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_DE寄存器TXTDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许触发事件的DMA请求 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXTDE_POS);
    }
    else
    {
        /* 禁止触发事件的DMA请求 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXDE, ECCP_DE_TXTDE_POS);
    }
}

/**
  * 描述  读取Tx更新事件中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_X_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取ECCPXEGIF寄存器TXUIF位 --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TXUIF)
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
  * 描述  读取Tz更新事件中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_Z_Get_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取ECCPXEGIF寄存器TZUIF位 --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TZUIF)
    {
        /* 发生Tz更新事件中断 */
        return SET;
    }
    else
    {
        /* 未发生Tz更新事件中断 */
        return RESET;
    }
}

/**
  * 描述  读取Tx计数溢出中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_X_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取ECCPXEGIF寄存器TXIF位 --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TXIF)
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
  * 描述  读取Tz计数溢出中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_Z_Get_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取ECCPXEGIF寄存器TZIF位 --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TZIF)
    {
        /* 发生Tz计数溢出中断 */
        return SET;
    }
    else
    {
        /* 未发生Tz计数溢出中断 */
        return RESET;
    }
}

/**
  * 描述  读取Tx触发事件中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，取值T5_SFR/T9_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_X_Get_Trigger_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 读取ECCPXEGIF寄存器TXTIF位 --------------------*/
    if (ATIMx->ECCPXEGIF & ECCP_EGIF_TXTIF)
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
  * 描述  产生触发事件配置位。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  *       NewState: 定时器使能控制状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ATIM_X_Generate_Trigger_Config (ATIM_SFRmap* ATIMx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_EGIF寄存器TXTRG位 --------------------*/
    if (NewState != FALSE)
    {
        /* 产生触发响应 */
        SFR_SET_BIT_ASM(ATIMx->ECCPXEGIF, ECCP_EGIF_TXTRG_POS);
    }
    else
    {
        /* 无动作 */
        SFR_CLR_BIT_ASM(ATIMx->ECCPXEGIF, ECCP_EGIF_TXTRG_POS);
    }
}

/**
  * 描述  读取Tx更新事件DMA中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_X_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_DF寄存器TXUDF位 --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_TXUDF)
    {
        /* 发生Tx更新DMA中断 */
        return SET;
    }
    else
    {
        /* 未发生Tx更新DMA中断 */
        return RESET;
    }
}

/**
  * 描述  读取Tz更新事件DMA中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_Z_Get_Updata_DMA_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_DF寄存器TZUDF位 --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_TZUDF)
    {
        /* 发生Tz更新DMA中断 */
        return SET;
    }
    else
    {
        /* 未发生Tz更新DMA中断 */
        return RESET;
    }
}

/**
  * 描述  读取Tx触发事件触发DMA中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ATIM_X_Get_Trigger_DMA_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_DF寄存器TRIGDF位 --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_TRIGDF)
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
  * 描述  清除Tx更新事件中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  无
  * 特殊说明：清除该标志位需在使能对应定时器的情况下进行，否则会导致清除失败。
  */
void
ATIM_X_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_SRIC寄存器TXUIC位 --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXUIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TXUIF)>>ECCP_EGIF_TXUIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXUIC_POS);
}

/**
  * 描述  清除Tz更新事件中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  * 返回  无
  * 特殊说明：清除该标志位需在使能对应定时器的情况下进行，否则会导致清除失败。
  */
void
ATIM_Z_Clear_Updata_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_SRIC寄存器TZUIC位 --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZUIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TZUIF)>>ECCP_EGIF_TZUIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZUIC_POS);
}

/**
  * 描述  清除Tx溢出中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  无
  */
void
ATIM_X_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_SRIC寄存器TXIC位 --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TXIF)>>ECCP_EGIF_TXIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXIC_POS);
}

/**
  * 描述  清除Tz溢出中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T6_SFR/T10_SFR。
  * 返回  无
  */
void
ATIM_Z_Clear_Overflow_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_SRIC寄存器TZIC位 --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TZIF)>>ECCP_EGIF_TZIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TZIC_POS);
}

/**
  * 描述  清除Tx触发事件中断标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  无
  */
void
ATIM_X_Clear_Trigger_INT_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_SRIC寄存器TXTIC位 --------------------*/
    SFR_SET_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXTIC_POS);
    while((ATIMx->ECCPXEGIF & ECCP_EGIF_TXTIF)>>ECCP_EGIF_TXTIF_POS);
    SFR_CLR_BIT_ASM(ATIMx->ECCPXSRIC, ECCP_SRIC_TXTIC_POS);
}

/**
  *   ##### 高级定时器(ATIM)中断及标志管理函数定义结束 #####
  */


/**
  *   ##### 增强型捕捉/比较/PWM外设(ECCP)捕捉功能函数定义 #####
  */
/**
  * 描述  增强型比较器(ECCP)配置。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 比较通道，取值为:
  *                ECCP_CHANNEL_1: 通道1
  *                ECCP_CHANNEL_2: 通道2
  *                ECCP_CHANNEL_3: 通道3
  *                ECCP_CHANNEL_4: 通道4
  *       CompareMode: 比较器模式，取值为:
  *                      ECCP_MODE_RST: 关闭比较功能
  *                      ECCP_CMP_TOGGLE_LEVEL: 比较匹配时输出翻转
  *                      ECCP_CMP_ACTIVE_LEVEL: 比较匹配时输出高电平
  *                      ECCP_CMP_INACTIVE_LEVEL: 比较匹配时输出低电平
  *                      ECCP_CMP_SOFT_INT: 比较匹配时产生软中断
  *                      ECCP_CMP_SPECIAL_EVENT: 比较匹配时除法特殊事件
  *       CompareValue: 比较寄存器的值，取值为16位数据。
  * 返回  无
  */
void
ECCP_Compare_Configuration(ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t CompareMode, uint16_t CompareValue)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CMP_MODE(CompareMode));

    /*------------------ 设置ECCPX_CTL1寄存器 -----------------*/
    /* 根据Channel和CompareMode，设置ECCPxCHyM位域 */
    tmpreg = 4 * Channel;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << tmpreg),
                          CompareMode << tmpreg);

    /*------------------ 设置ECCPXRy寄存器 -----------------*/
    /* 获取ECCPXRY寄存器地址 */
    tmpreg1 = (uint32_t)ECCPx;
    tmpreg1 = tmpreg1 + ECCP_RY_OFFSET + tmpreg;
    /* 根据CompareValue，设置ECCPXRy位域 */
    *(volatile uint32_t*)tmpreg1 = CompareValue;
}

/**
  * 描述  增强型捕捉模块(ECCP)配置。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       eccpInitStruct: 增强型捕捉模块(ECCP)配置信息结构体指针。
  * 返回  无
  */
void
ECCP_Capture_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_CaptureInitTypeDef* eccpInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(eccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CAP_MODE(eccpInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_PWMInput));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_XORMode));

    /*------------------ 设置ECCPX_CTL1寄存器 -----------------*/
    /* 根据m_Channel和m_Mode，设置ECCPXCHyM位域 */
    tmpreg = 4 * eccpInitStruct->m_Channel;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << tmpreg),
                          eccpInitStruct->m_Mode << tmpreg);

    /*------------------ 设置ECCPX_CTL2寄存器 -----------------*/
    /* 根据PWMInput，设置PXPWMI位域 */
    /* 根据m_XORMode，设置PXXORM位域 */
    tmpreg = (eccpInitStruct->m_PWMInput << ECCP_CTL2_PXPWMI_POS) \
           | (eccpInitStruct->m_PWMInput << ECCP_CTL2_PXXORM_POS);
    ECCPx->ECCPXCTL2 = SFR_Config (ECCPx->ECCPXCTL2,
                          ~ECCP_CTL2_INIT_MASK,
                          tmpreg);
}

/**
  * 描述  初始化增强型捕捉模块(ECCP)配置信息结构体。
  * 输入  eccpInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
ECCP_Capture_Struct_Init (ECCP_CaptureInitTypeDef* eccpInitStruct)
{
    /* 设置PWM通道 */
    eccpInitStruct->m_Channel = ECCP_CHANNEL_1;
    /* 设置通道的模式选择位 */
    eccpInitStruct->m_Mode = ECCP_MODE_RST;
    /* 设置PWM测量模式 */
    eccpInitStruct->m_PWMInput = FALSE;
    /* 设置输入异或模式 */
    eccpInitStruct->m_XORMode = FALSE;
}

/**
  * 描述  增强型PWM模块(ECCP)配置。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       eccpInitStruct: 增强型PWM模块(ECCP)配置信息结构体指针。
  * 返回  无
  */
void
ECCP_PWM_Configuration (ECCP_SFRmap* ECCPx,
                    ECCP_PWMInitTypeDef* eccpInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(eccpInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PWM_MODE(eccpInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_ECCP_OUTPUT_MODE(eccpInitStruct->m_OutputMode));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_OUTPUT(eccpInitStruct->m_HOutputCtl));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_OUTPUT(eccpInitStruct->m_LOutputCtl));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_PhaseMove));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_SinglePWM));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eccpInitStruct->m_CloseTimer));

    /*------------------ 设置ECCPX_CTL1寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_Mode，设置ECCPXCHyM位域 */
    tmpreg = 4 * eccpInitStruct->m_Channel;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << tmpreg),
                          eccpInitStruct->m_Mode << tmpreg);

    /*------------------ 设置ECCPX_Ry寄存器 -----------------*/
    /* 根据结构体成员m_Channel，获取CCPXRY寄存器地址 */
    tmpreg1 = (uint32_t)ECCPx;
    tmpreg1 = tmpreg1 + ECCP_RY_OFFSET + tmpreg;
    /* 根据结构体成员m_DutyRatio，设置ECCPXRy位域 */
    *(volatile uint32_t*)tmpreg1
        = (uint32_t)eccpInitStruct->m_DutyRatio;

    /*------------------ 设置PXUDCTL寄存器 -----------------*/
    /* 根据结构体成员m_PhaseMove，设置PXMPEN位域 */
    ECCPx->PXUDCTL = SFR_Config (ECCPx->PXUDCTL,
                        ~ECCP_PXUDCTL_PXMPEN,
                        eccpInitStruct->m_PhaseMove);

    /*------------------ 设置PXDTCTL寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_DeadTime，设置PZDC或PZDC位域 */
    if (ECCP_CHANNEL_4 == eccpInitStruct->m_Channel)
    {
        tmpreg = ((uint32_t)eccpInitStruct->m_DeadTime \
                    << ECCP_PXDTCTL_PZDC0_POS);
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                            ~ECCP_PXDTCTL_PZDC,
                            tmpreg);
    }
    else
    {
        tmpreg = ((uint32_t)eccpInitStruct->m_DeadTime \
                    << ECCP_PXDTCTL_PXDC0_POS);
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                            ~ECCP_PXDTCTL_PXDC,
                            tmpreg);
    }

    /*------------------ 设置PXATRCTL寄存器 -----------------*/
    /* 根据结构体成员m_Channel和m_OutputMode，设置PxCHyMOD位域 */
    tmpreg = eccpInitStruct->m_Channel;
    ECCPx->PXATRCTL = SFR_Config (ECCPx->PXATRCTL,
                          ~(ECCP_PXATRCTL_PXCH1MOD << tmpreg),
                          (uint32_t)eccpInitStruct->m_OutputMode << tmpreg);

    /*------------------ 设置PWMXOC寄存器 -----------------*/
    /* 根据结构体成员m_Channel、m_HOutputCtl和m_LOutputCtl，
       设置PxOCyH和PxOCyL位域。 */
    tmpreg = 4 * eccpInitStruct->m_Channel;
    tmpreg1 = ((uint32_t)eccpInitStruct->m_HOutputCtl << ECCP_PWMXOC_PXOC1H0_POS) \
            | ((uint32_t)eccpInitStruct->m_LOutputCtl << ECCP_PWMXOC_PXOC1L0_POS);
    ECCPx->PWMXOC = SFR_Config (ECCPx->PWMXOC,
                          ~((ECCP_PWMXOC_PXOC1L | ECCP_PWMXOC_PXOC1H) \
                            << tmpreg),
                          tmpreg1 << tmpreg);

    /*------------------ 设置ECCPX_CTL2寄存器 -----------------*/
    /* 根据结构体成员m_SinglePWM，设置PxSPM位域 */
    tmpreg = (eccpInitStruct->m_SinglePWM << ECCP_CTL2_PXSPM_POS);
    ECCPx->ECCPXCTL2 = SFR_Config (ECCPx->ECCPXCTL2,
                          ~(ECCP_CTL2_PXSPM),
                          tmpreg);

    /*------------------ 设置ECCPX_CTL3寄存器 -----------------*/
    /* 根据结构体成员m_CloseTimer，设置PxSPMST位域 */
    tmpreg = (eccpInitStruct->m_CloseTimer << ECCP_CTL3_PXSPMST_POS);
    ECCPx->ECCPXCTL3 = SFR_Config (ECCPx->ECCPXCTL3,
                          ~ECCP_CTL3_PXSPMST,
                          tmpreg);
}

/**
  * 描述  初始化增强型PWM模块(ECCP)配置信息结构体。
  * 输入  eccpInitStruct: 指向待初始化的结构体指针。
  * 返回  无
  */
void
ECCP_PWM_Struct_Init (ECCP_PWMInitTypeDef* eccpInitStruct)
{
    /* 设置PWM通道 */
    eccpInitStruct->m_Channel = ECCP_CHANNEL_1;
    /* 设置通道的模式选择位 */
    eccpInitStruct->m_Mode = ECCP_MODE_RST;
    /* 设置PWM占空比 */
    eccpInitStruct->m_DutyRatio = 0x0000;
    /* 设置死区控制 */
    eccpInitStruct->m_DeadTime = 0x00;
    /* 设置通道输出模式 */
    eccpInitStruct->m_OutputMode = ECCP_OUTPUT_INDEPENDENT;
    /* 设置通道H输出控制 */
    eccpInitStruct->m_HOutputCtl = ECCP_CHANNEL_OUTPUT_PWM_ACTIVE;
    /* 设置通道L输出控制 */
    eccpInitStruct->m_LOutputCtl = ECCP_CHANNEL_OUTPUT_PWM_ACTIVE;
    /* 设置相位移动使能 */
    eccpInitStruct->m_PhaseMove = FALSE;
    /* 设置单脉冲输出模式 */
    eccpInitStruct->m_SinglePWM = FALSE;
    /* 设置单脉冲输出模式选择 */
    eccpInitStruct->m_CloseTimer = FALSE;
}

/**
  * 描述  配置ECCP捕捉功能。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       EdgeConfig: 匹配边沿，取值范围为:
  *                     ECCP_MODE_RST: 关闭捕捉功能
  *                     ECCP_CAP_RISING_EDGE: 每个下降沿发生捕捉
  *                     ECCP_CAP_FALLING_EDGE: 每个上升沿发生捕捉
  *                     ECCP_CAP_4TH_RISING_EDGE: 每4个上升沿发生捕捉
  *                     ECCP_CAP_16TH_RISING_EDGE: 每16个上升沿发生捕捉
  * 返回  无
  */
void
ECCP_Capture_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t EdgeConfig)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CAP_MODE(EdgeConfig));

    /*-------------------- 设置ECCPX_CTL1寄存器 --------------------*/
    tmpreg = EdgeConfig << (4 * Channel);
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * 描述  配置ECCP比较功能。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 比较通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       CmpConfig: 比较匹配模式，取值范围为:
  *                    ECCP_MODE_RST: 关闭比较功能
  *                    ECCP_CMP_TOGGLE_LEVEL: 匹配时输出电平翻转
  *                    ECCP_CMP_ACTIVE_LEVEL: 比较匹配时输出高电平
  *                    ECCP_CMP_INACTIVE_LEVEL: 比较匹配时输出低电平
  *                    ECCP_CMP_SOFT_INT: 比较匹配时产生软件中断
  *                    ECCP_CMP_SPECIAL_EVENT: 触发特殊事件
  * 返回  无
  */
void
ECCP_Compare_Mode_Config (ECCP_SFRmap* ECCPx,
                    uint32_t Channel, uint32_t CmpConfig)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CMP_MODE(CmpConfig));

    /*-------------------- 设置ECCPX_CTL1寄存器 --------------------*/
    tmpreg = CmpConfig << (4 * Channel);
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~(ECCP_CTL1_CH1M << (4 * Channel)),
                          tmpreg);
}

/**
  * 描述  配置ECCP PWM功能,模式选择，自由，协同，单时机模式。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       PWMConfig: PWM匹配模式，取值范围为:
  *                    ECCP_MODE_RST: 关闭比较功能
  *                    ECCP_PWM_FREE: PWM自由模式
  *                    ECCP_PWM_COORDINATION: PWM协同模式
  *                    ECCP_PWM_SINGLE: PWM单时基模式
  * 返回  无
  */
void
ECCP_PWM_Mode_Config (ECCP_SFRmap* ECCPx, uint32_t PWMConfig)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_PWM_MODE(PWMConfig));

    /*-------------------- 设置ECCPX_CTL1寄存器 --------------------*/
    tmpreg = PWMConfig << ECCP_CTL1_CH1M0_POS;
    ECCPx->ECCPXCTL1 = SFR_Config (ECCPx->ECCPXCTL1,
                          ~ECCP_CTL1_CH1M,
                          tmpreg);
}

/**
  * 描述  读取ECCP捕捉寄存器。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  捕捉寄存器的16位值。
  */
uint16_t
ECCP_Get_Capture_Result (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /* 获取寄存器访问地址 */
    tmpreg = (uint32_t)ECCPx;
    tmpreg = tmpreg + ECCP_CY_OFFSET + (4 * Channel);

    /* 通过16位类型指针获取寄存器低16位的值 */
    return (*(volatile const uint16_t*)  tmpreg);
}

/**
  * 描述  读取ECCP比较/EPWM占空比寄存器。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *               取值T5_SFR/T9_SFR
  *               或ECCP5_SFR/ECCP9_SFR
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  比较/PWM占空比寄存器的值。
  */
uint16_t
ECCP_Get_Compare_Result (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
      CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
      CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));


    /* 获取寄存器访问地址 */
    tmpreg = (uint32_t)ECCPx;
    tmpreg = tmpreg + ECCP_RY_OFFSET + (4 * Channel);

    /* 通过16位类型指针获取寄存器低16位的值 */
    return (*(volatile const uint16_t*)  tmpreg);
}



/**
  * 描述  配置ECCP比较/PWM占空比寄存器。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  无。
  */
void
ECCP_Set_Compare_Result (ECCP_SFRmap* ECCPx, uint32_t Channel, uint16_t Value)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /* 获取CCPXRY寄存器地址 */
    tmpreg = (uint32_t)ECCPx;
    tmpreg = tmpreg + ECCP_RY_OFFSET + (4 * Channel);

    /*-------------------- 设置ECCPX_RY寄存器 --------------------*/
    *(volatile uint32_t*)tmpreg = (uint16_t)Value;
}

/**
  * 描述  产生捕捉/比较事件配置位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       NewState: 产生捕捉/比较事件配置状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_Generate_Trigger_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ 设置ECCPX_EGIF寄存器CCyG位 ------------------*/
    if (NewState != FALSE)
    {
        /* 在通道Channel上产生一个捕捉/比较事件 */
        ECCPx->ECCPXEGIF |= ECCP_EGIF_CC1G << Channel;
    }
    else
    {
        /* 无动作 */
        ECCPx->ECCPXEGIF &= ~(ECCP_EGIF_CC1G << Channel);
    }
}

/**
  * 描述  PWM输入测量模式使能
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: PWM输入测量模式使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_PWM_Input_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_CTL2寄存器PXPWMI位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能PWM输入测量模式 */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXPWMI_POS);
    }
    else
    {
        /* 未使能PWM输入测量模式 */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXPWMI_POS);
    }
}

/**
  * 描述  输入异或使能位配置
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: 输入异或使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_Input_XOR_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCP_CTL2寄存器PXXORM位 --------------------*/
    if (NewState != FALSE)
    {
        /* 使能PWM输入测量模式 */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXXORM_POS);
    }
    else
    {
        /* 未使能PWM输入测量模式 */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXXORM_POS);
    }
}

/**
  * 描述  配置单脉冲输出模式。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: 单脉冲输出模式使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_Single_Pulse_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_CTL2寄存器PXSPM位 --------------------*/
    if (NewState != FALSE)
    {
        /* 单脉冲输出模式使能 */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXSPM_POS);
    }
    else
    {
        /* 单脉冲输出模式未使能 */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL2, ECCP_CTL2_PXSPM_POS);
    }
}

/**
  * 描述  配置单脉冲输出模式选择。
  * 输入  ECCPx: 指向定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: 单脉冲输出模式选择，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_Single_Pulse_Shut_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_CTL3寄存器PXSPMST位 --------------------*/
    if (NewState != FALSE)
    {
        /* 单脉冲输出后关闭定时器使能位 */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXSPMST_POS);
    }
    else
    {
        /* 单脉冲输出后不关闭定时器使能位 */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXSPMST_POS);
    }
}

/**
  * 描述  配置PWM通道1/2/3/4重启使能位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: PWM通道1/2/3/4重启使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_PWM_Restart_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_CTL3寄存器PXRSEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 自动关闭时，一旦关闭事件消失，PxASE位将自动清零，PWM自动重启 */
        SFR_SET_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXRSEN_POS);
    }
    else
    {
        /* 自动关闭时，PxASE由软件清零，以重启PWM */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXCTL3, ECCP_CTL3_PXRSEN_POS);
    }
}

/**
  * 描述  配置PWM通道死区延时时间。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       DeadTime：死区延时时间，取值为8位数据。
  * 返回  无
  */
void
ECCP_Dead_Time_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint8_t DeadTime)
{
    uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*-------------------- 设置ECCPX_PxDTCTL寄存器 --------------------*/
    if (ECCP_CHANNEL_4 == Channel)
    {
        /* 设置通道4的死区延时的时间 */
        tmpreg = (uint32_t)DeadTime << ECCP_PXDTCTL_PZDC0_POS;
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                             ~ECCP_PXDTCTL_PZDC,
                             tmpreg);
    }
    else
    {
        /* 设置通道1/2/3的死区延时的时间 */
        tmpreg = (uint32_t)DeadTime << ECCP_PXDTCTL_PXDC0_POS;
        ECCPx->PXDTCTL = SFR_Config (ECCPx->PXDTCTL,
                             ~ECCP_PXDTCTL_PXDC,
                             tmpreg);
    }
}

/**
  * 描述  配置ECCP通道输出控制。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       Port: 端口选择，取值范围为:
  *               ECCP_PORT_LOW: CHxL端口
  *               ECCP_PORT_HIGH: CHxH端口
  *       ChannelOutputCtl: 通道端口输出控制，取值范围为:
  *                           ECCP_CHANNEL_OUTPUT_PWM_ACTIVE: PWM输出，高有效
  *                           ECCP_CHANNEL_OUTPUT_PWM_INACTIVE: PWM输出，低有效
  *                           ECCP_CHANNEL_OUTPUT_INACTIVE: 强制低电平输出
  *                           ECCP_CHANNEL_OUTPUT_ACTIVE: 强制高电平输出
  * 返回  无。
  */
void
ECCP_Channel_Output_Control (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelOutputCtl)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PORT(Port));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_OUTPUT(ChannelOutputCtl));

    /*--------------- 设置ECCPX_PWMXOC寄存器PXOCyH/PXOCyL位 ---------------*/
    tmpreg = ChannelOutputCtl << (Port + Channel * 4);
    ECCPx->PWMXOC = SFR_Config (ECCPx->PWMXOC,
                          ~(ECCP_PWMXOC_PXOC1L << (Port + Channel * 4)),
                          tmpreg);
}

/**
  * 描述  配置ECCP通道输出模式。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       ChannelOutputMode: PWM通道输出模式，取值范围为:
  *                            ECCP_OUTPUT_INDEPENDENT: 独立输出模式
  *                            ECCP_OUTPUT_COMPLEMENTARY: 互补输出模式
  * 返回  无。
  */
void
ECCP_Channel_Output_Mode (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t ChannelOutputMode)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_OUTPUT_MODE(ChannelOutputMode));

    /*----------------- 设置ECCPX_PXATRCTL寄存器PXCHyMOD位 -----------------*/
    tmpreg = ChannelOutputMode << (ECCP_PXATRCTL_PXCH1MOD_POS + Channel);
    ECCPx->PXATRCTL = SFR_Config (ECCPx->PXATRCTL,
                          ~(ECCP_PXATRCTL_PXCH1MOD << Channel),
                          tmpreg);
}

/**
  * 描述  配置通道自动关闭事件状态位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       WorkingState: 通道自动关闭事件状态，取值范围为:
  *                       ECCP_CHANNEL_WORKING_STATE: 通道正常工作
  *                       ECCP_CHANNEL_SHUTDOWN_STATE: 通道关闭
  * 返回  无。
  */
void
ECCP_Channel_Work_State_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t WorkingState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL_STATE(WorkingState));

    /*-------------------- 设置ECCPX_PXASCTL0寄存器PXASEy位 --------------------*/
    tmpreg = WorkingState << (ECCP_PXASCTL0_PXASE1_POS + Channel);
    ECCPx->PXASCTL0 = SFR_Config (ECCPx->PXASCTL0,
                          ~(ECCP_PXASCTL0_PXASE1 << Channel),
                          tmpreg);
}

/**
  * 描述  读取通道自动关闭事件状态位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  通道自动关闭事件状态，发生关闭事件返回1，正常工作返回0。
  */
FlagStatus
ECCP_Get_Channel_Work_State (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*--------------- 读取ECCPX_PXASCTL0寄存器PXASEy位 ---------------*/
    tmpreg = ECCP_PXASCTL0_PXASE1 << Channel;
    if (ECCPx->PXASCTL0 & tmpreg)
    {
        /* 发生了关闭事件,通道为关闭状态 */
        return SET;
    }
    else
    {
        /* 通道正常工作 */
        return RESET;
    }
}

/**
  * 描述  配置通道自动关闭源选择位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       ShutDownSignal: 通道自动关闭源，取值范围为:
  *                         ECCP_CHANNEL_SHUTDOWN_FORBID: 禁止自动关断
  *                         ECCP_CHANNEL_CMP2CMP3_ACTIVE: 比较器2/3输出高电平
  *                         ECCP_CHANNEL_BKIN_INACTIVE: ECCP_BKIN引脚上的低电平
  *       注意：比较器3输出作为 ECCP5/9 通道 1/2/3 自动关断的触发信号(高电平关断)
  *       		比较器 2/3 的输出作为 ECCP5/9 通道4 自动关断的触发信号(高电平关断)
  * 返回  无。
  */
void
ECCP_CHANNEL4_Shutdown_SEL (ECCP_SFRmap* ECCPx,
                    uint32_t ShutDownSignal)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_CHANNEL4_SHUTDOWN_SIGNAL(ShutDownSignal));


    if (ShutDownSignal != CMP3_OUTPUT_SHUNT)
    {
        /* 自动关闭时，一旦关闭事件消失，PxASE位将自动清零，PWM自动重启 */
        SFR_SET_BIT_ASM(ECCPx->PXASCTL, ECCP_PXASCTL_PXASS_POS);
    }
    else
    {
        /* 自动关闭时，PxASE由软件清零，以重启PWM */
        SFR_CLR_BIT_ASM(ECCPx->PXASCTL, ECCP_PXASCTL_PXASS_POS);
    }
}
/**
  * 描述  配置通道自动关闭源选择位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       ShutDownSignal: 通道自动关闭源，取值范围为:
  *                         ECCP_CHANNEL_SHUTDOWN_FORBID: 禁止自动关断
  *                         ECCP_CHANNEL_CMP2CMP3_ACTIVE: 比较器2/3输出高电平
  *                         ECCP_CHANNEL_BKIN_INACTIVE: ECCP_BKIN引脚上的低电平
  *       注意：比较器3输出作为 ECCP5/9 通道 1/2/3 自动关断的触发信号(高电平关断)
  *       		比较器 2/3 的输出作为 ECCP5/9 通道4 自动关断的触发信号(高电平关断)
  * 返回  无。
  */
void
ECCP_Channel_Shutdown_Signal (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t ShutDownSignal)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_SHUTDOWN_SIGNAL(ShutDownSignal));

    /*-------------------- 设置ECCPX_PXASCTL0寄存器PXASSy位 --------------------*/
    tmpreg = ShutDownSignal << (Channel * 2);
    ECCPx->PXASCTL0 = SFR_Config (ECCPx->PXASCTL0,
                          ~(ECCP_PXASCTL0_PXASS1 << (Channel * 2)),
                          tmpreg);
}

/**
  * 描述  配置引脚ECCPxCHyH/ECCPxCHyL关闭状态控制位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       Port: 端口选择，取值范围为:
  *               ECCP_PORT_LOW: CHxL端口
  *               ECCP_PORT_HIGH: CHxH端口
  *       ChannelPinCtl: 引脚状态，取值范围为:
  *                        PIN_INACTIVE: 驱动引脚为低电平
  *                        PIN_ACTIVE: 驱动引脚为高电平
  *                        PIN_TRISTATE: 驱动引脚为三态
  * 返回  无。
  */
void
ECCP_Channel_Pin_Ctl (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t ChannelPinCtl)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PORT(Port));
    CHECK_RESTRICTION(CHECK_PIN_STATE(ChannelPinCtl));

    /*--------------- 设置ECCPX_PXASCTL1寄存器PxSSyH/PxSSyL位 ---------------*/
    tmpreg = ChannelPinCtl << (Port + Channel * 4);
    ECCPx->PXASCTL1 = SFR_Config (ECCPx->PXASCTL1,
                          ~(ECCP_PXASCTL1_PXSS1L << (Port + Channel * 2)),
                          tmpreg);
}

/**
  * 描述  配置零点检测时钟。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       ZeroClock: 零点检测时钟，取值范围为:
  *                    ECCP_ZERO_CLOCK_DIV_1: 预分频时钟
  *                    ECCP_ZERO_CLOCK_DIV_2: 预分频时钟 / 2
  *                    ECCP_ZERO_CLOCK_DIV_4: 预分频时钟 / 4
  *                    ECCP_ZERO_CLOCK_DIV_8: 预分频时钟 / 8
  *                    ECCP_ZERO_CLOCK_DIV_16: 预分频时钟 / 16
  *                    ECCP_ZERO_CLOCK_DIV_32: 预分频时钟 / 32
  *                    ECCP_ZERO_CLOCK_DIV_64: 预分频时钟 / 64
  *                    ECCP_ZERO_CLOCK_DIV_128: 预分频时钟 / 128
  *                    ECCP_ZERO_CLOCK_DIV_256: 预分频时钟 / 256
  *                    ECCP_ZERO_CLOCK_DIV_512: 预分频时钟 / 512
  * 返回  无。
  */
void
ECCP_Zero_Clock_Config (ECCP_SFRmap* ECCPx,
                    uint32_t ZeroClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_ZERO_CLOCK(ZeroClock));

    /*-------------------- 设置ECCPX_ZPDCTL0寄存器ZCLK位 --------------------*/
    ECCPx->ZPDCTL0 = SFR_Config (ECCPx->ZPDCTL0,
                          ~(ECCP_ZPDCTL0_ZCLK),
                          ZeroClock);
}

/**
  * 描述  配置引脚ECCPxCHyH/ECCPxCHyL三态控制位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       Port: 端口选择，取值范围为:
  *               ECCP_PORT_LOW: CHxL端口
  *               ECCP_PORT_HIGH: CHxH端口
  *       PinTristateCtl: 引脚状态，取值范围为:
  *                        PIN_INACTIVE: 驱动引脚为低电平
  *                        PIN_ACTIVE: 驱动引脚为高电平
  *                        PIN_TRISTATE: 驱动引脚为三态
  * 返回  无。
  */
void
ECCP_Channel_Pin_Tristate_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t Port, uint32_t PinTristateCtl)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_PORT(Port));
    CHECK_RESTRICTION(CHECK_PIN_STATE(PinTristateCtl));

    /*------------- 设置ECCPX_ZPDPORT寄存器ZPORT位 -------------*/
    tmpreg = (Port / 2) + (Channel * 2);
    tmpreg1 = PinTristateCtl << tmpreg;
    ECCPx->ZPDPORT = SFR_Config (ECCPx->ZPDPORT,
                          ~(ECCP_ZPDPORT_ZPORT << tmpreg),
                          tmpreg1);
}

/**
  *   ##### 增强型捕捉/比较/PWM外设(ECCP)捕捉功能函数定义结束 #####
  */

/**
  *   ##### 增强型捕捉/比较/PWM外设(ECCP)中断及标志管理函数定义 #####
  */
/**
  * 描述  通道捕获/比较的中断使能配置。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       NewState: 通道捕获/比较的中断使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
ECCP_Channel_INT_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_IE寄存器CCxIE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许CCP通道Channel捕捉/比较中断 */
        ECCPx->ECCPXIE |= ECCP_IE_CC1IE << Channel;
    }
    else
    {
        /* 禁止CCP通道Channel捕捉/比较中断 */
        ECCPx->ECCPXIE &= ~(ECCP_IE_CC1IE << Channel);
    }
}

/**
  * 描述  关断事件的DMA请求使能位
  * 输入  ECCPx: 指向CCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: 关断事件的DMA请求使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
ECCP_X_Turn_off_DMA_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_DE寄存器CLSDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许关断事件的DMA请求 */
        SFR_SET_BIT_ASM(ECCPx->ECCPXDE, ECCP_DE_CLSDE_POS);
    }
    else
    {
        /* 禁止关断事件的DMA请求 */
        SFR_CLR_BIT_ASM(ECCPx->ECCPXDE, ECCP_DE_CLSDE_POS);
    }
}

/**
  * 描述  通道捕获/比较的DMA请求允许使能位配置。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       NewState: 通道捕获/比较的DMA请求允许使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
ECCP_Channel_DMA_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_DE寄存器CHxDE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许捕获/比较通道x的DMA请求 */
        ECCPx->ECCPXDE |= ECCP_DE_CH1DE << Channel;
    }
    else
    {
        /* 禁止捕获/比较通道x的DMA请求 */
        ECCPx->ECCPXDE &= ~(ECCP_DE_CH1DE << Channel);
    }
}

/**
  * 描述  读取ECCP通道捕捉/比较中断标志。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ECCP_Get_Channel_Trigger_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));

    /*-------------------- 读取ECCPXEGIF寄存器CCxIF位 --------------------*/
    tmpreg = ECCP_EGIF_CC1IF << Channel;
    if (ECCPx->ECCPXEGIF & tmpreg)
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
  * 描述  读取关断事件触发DMA标志。
  * 输入  ATIMx: 指向定时器内存结构的指针，
  *              取值T5_SFR/T9_SFR。
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ECCP_X_Get_Turn_off_DMA_Flag (ATIM_SFRmap* ATIMx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ATIMx));

    /*-------------------- 设置ECCPX_DF寄存器CLSDF位 --------------------*/
    if (ATIMx->ECCPXDF & ECCP_DF_CLSDF)
    {
        /* 发生关断事件触发DMA标志 */
        return SET;
    }
    else
    {
        /* 未发生关断事件触发DMA标志 */
        return RESET;
    }
}

/**
  * 描述  读取ECCP通道捕捉/比较DMA中断标志。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
ECCP_Get_Trigger_DMA_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));

    /*-------------------- 读取ECCPXEGIF寄存器CHxDF位 --------------------*/
    tmpreg = ECCP_DF_CH1DF << Channel;
    if (ECCPx->ECCPXDF & tmpreg)
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
  * 描述  清除ECCPx通道捕捉/比较中断标志。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  无。
  */
void
ECCP_Clear_Channel_INT_Flag (ECCP_SFRmap* ECCPx, uint32_t Channel)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*-------------------- 设置ECCPX_SRIC寄存器CCxIC位 --------------------*/
    tmpreg = ECCP_SRIC_CC1IC << Channel;
    tmpreg1 = ECCP_EGIF_CC1IF << Channel;
    ECCPx->ECCPXSRIC |= tmpreg;

    while((ECCPx->ECCPXEGIF & tmpreg1)>>(ECCP_EGIF_CC1IF_POS+Channel));

    ECCPx->ECCPXSRIC &= ~tmpreg;
}

/**
  * 描述  PWM相位移动使能位配置。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       NewState: PWM相位移动使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
ECCP_PWM_Move_Phase_Enable (ECCP_SFRmap* ECCPx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_PxUDCTL寄存器PxMPEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 开启PWM自动相移功能 */
        SFR_SET_BIT_ASM(ECCPx->PXUDCTL, ECCP_PXUDCTL_PXMPEN_POS);
    }
    else
    {
        /* 关闭PWM自动相移功能 */
        SFR_CLR_BIT_ASM(ECCPx->PXUDCTL, ECCP_PXUDCTL_PXMPEN_POS);
    }
}

/**
  * 描述  配置通道的零点检测时序控制。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       NewState: 通道捕获/比较的中断使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
ECCP_Channel_Zero_Detect_Sequential_Ctl (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_ZPDCTL0寄存器ZPDyTWIN位 --------------------*/
    tmpreg = NewState << (Channel + ECCP_ZPDCTL0_ZPD1TWIN_POS);
    ECCPx->ZPDCTL0 = SFR_Config (ECCPx->ZPDCTL0,
                          ~(ECCP_ZPDCTL0_ZPD1TWIN << Channel),
                          tmpreg);
}

/**
  * 描述  读取通道零点检测的感应电压检测状态。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  通道零点检测的感应电压检测状态，
  *       未检测到感应电压（检测到0点）返回1，
  *       检测到感应电压（未检测到0点）返回0。
  */
FlagStatus
ECCP_Get_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*--------------- 读取ECCPX_ZPDCTL0寄存器ZPDyFLAG位 ---------------*/
    tmpreg = ECCP_ZPDCTL0_ZPD1FLAG << Channel;
    if (ECCPx->ZPDCTL0 & tmpreg)
    {
        /* 未检测到感应电压（检测到0点） */
        return SET;
    }
    else
    {
        /* 检测到感应电压（未检测到0点） */
        return RESET;
    }
}

/**
  * 描述  清零通道零点检测的感应电压检测状态。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  * 返回  无。
  */
void
ECCP_Clear_Channel_Zero_Detection_State (ECCP_SFRmap* ECCPx,
                    uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));

    /*--------------- 清零ECCPX_ZPDCTL0寄存器ZPDyFLAG位 ---------------*/
    tmpreg = ECCP_ZPDCTL0_ZPD1FLAG << Channel;
    ECCPx->ZPDCTL0 &= ~tmpreg;
}

/**
  * 描述  配置通道的零点检测比较器使能位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       NewState: 通道捕获/比较的中断使能状态，
  *                 取值范围为：TRUE 或 FALSE
  * 返回  无。
  */
void
ECCP_Channel_Zero_Detect_Enable (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置ECCPX_ZPDCTL1寄存器ZPDyEN位 --------------------*/
    tmpreg = NewState << (Channel + ECCP_ZPDCTL1_ZPD1EN_POS);
    ECCPx->ZPDCTL1 = SFR_Config (ECCPx->ZPDCTL1,
                          ~(ECCP_ZPDCTL1_ZPD1EN << Channel),
                          tmpreg);
}

/**
  * 描述  配置通道零点检测电压选择位。
  * 输入  ECCPx: 指向ECCP或高级定时器内存结构的指针，
  *              取值ECCP5_SFR/ECCP9_SFR。
  *       Channel: 捕捉通道选择，取值范围为:
  *                  ECCP_CHANNEL_1: 通道1
  *                  ECCP_CHANNEL_2: 通道2
  *                  ECCP_CHANNEL_3: 通道3
  *                  ECCP_CHANNEL_4: 通道4
  *       ZeroDetectVoltage: 通道零点检测电压，取值范围为：
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_15: 0.15V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_25: 0.25V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_35: 0.35V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_45: 0.45V
  *                            ECCP_ZERO_VOLTAGE_DECIMAL_55: 0.55V
  * 返回  无。
  */
void
ECCP_Channel_Zero_Voltage_Config (ECCP_SFRmap* ECCPx, uint32_t Channel,
                    uint32_t ZeroDetectVoltage)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_ATIM_PERIPH(ECCPx));
    CHECK_RESTRICTION(CHECK_ECCP_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_ECCP_ZERO_VOLTAGE(ZeroDetectVoltage));

    /*-------------------- 设置ECCPX_ZPDCTL1寄存器ZPDyEN位 --------------------*/
    tmpreg = ZeroDetectVoltage \
             << (Channel * 3 + ECCP_ZPDCTL1_ZPD1S0_POS);
    ECCPx->ZPDCTL1 = SFR_Config (ECCPx->ZPDCTL1,
                          ~(ECCP_ZPDCTL1_ZPD1S << (Channel * 3)),
                          tmpreg);
}

/**
  *   ##### 增强型捕捉/比较/PWM外设(ECCP)中断及标志管理函数定义结束 #####
  */

