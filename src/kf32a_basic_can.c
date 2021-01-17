/**
  ******************************************************************************
  * 文件名  kf32a_basic_can.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描  述  该文件提供了控制器局域网总线(CAN)相关的功能函数，包含：
  *          + 控制器局域网总线(CAN)初始化函数
  *          + 控制器局域网总线(CAN)功能配置函数
  *          + 控制器局域网总线(CAN)发送接收函数
  *          + 控制器局域网总线(CAN)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_can.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_can
/* 定时器私有定义 ----------------------------------------------------*/
/* CANx_CTLR寄存器初始化掩码 */
#define CAN_CTLR_INIT_MASK              (CAN_CTLR_CANEN \
                                       | CAN_CTLR_CANCKS \
                                       | CAN_CTLR_LBACK \
                                       | CAN_CTLR_SILENT \
                                       | CAN_CTLR_RSMOD)

/* CANx_BRGR寄存器初始化掩码 */
#define CAN_BRGR_INIT_MASK              (CAN_BRGR_CANBRP \
                                       | CAN_BRGR_SJW \
                                       | CAN_BRGR_TSEG1 \
                                       | CAN_BRGR_TSEG2 \
                                       | CAN_BRGR_SAM)

/* CANx_INFR寄存器初始化掩码 */
#define CAN_INFR_INIT_MASK              (CAN_INFR_IDE \
                                       | CAN_INFR_RTR \
                                       | CAN_INFR_DLC)

/* CANx_TX0R寄存器SFF格式初始化掩码 */
#define CAN_TX0R_SFF_MASK               (CAN_TX0R_SFF_ID \
                                       | 0xFFFF)

/* CANx_TX0R寄存器EFF格式初始化掩码 */
#define CAN_TX0R_EFF_MASK               (CAN_TX0R_EFF_ID)

/* CANx_TX2R寄存器SFF格式初始化掩码 */
#define CAN_TX2R_SFF_MASK               (0xFFFF0000)

/* CAN接收地址 */
#define CAN0_RECEIVE_ADDR               ((uint32_t)0x1FFEF800)
#define CAN1_RECEIVE_ADDR               ((uint32_t)0x1FFEF900)
#define CAN2_RECEIVE_ADDR               ((uint32_t)0x1FFEFA00)
#define CAN3_RECEIVE_ADDR               ((uint32_t)0x1FFEFB00)
#define CAN4_RECEIVE_ADDR               ((uint32_t)0x1FFEFC00)
#define CAN5_RECEIVE_ADDR               ((uint32_t)0x1FFEFD00)

/**
  *   ##### 控制器局域网总线(CAN)初始化函数 #####
  */
/**
  * 描述  复位CAN外设，使能外设时钟。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  无。
  */
void
CAN_Reset(CAN_SFRmap* CANx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
#ifdef KF32A_Periph_can0
    if (CANx == CAN0_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN0RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN0RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_CAN0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_can1
    if(CANx == CAN1_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN1RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN1RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_CAN1CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_can2
    if(CANx == CAN2_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN2RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN2RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_CAN2CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_can3
    if(CANx == CAN3_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN3RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_CAN3RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_CAN3CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_can4
    if(CANx == CAN4_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL3_CAN4RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL3_CAN4RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL3_CAN4CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_can5
    if(CANx == CAN5_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL3_CAN5RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL3_CAN5RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL3_CAN5CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  控制器局域网总线(CAN)初始化，该函数会在配置中置一复位模式RSMOD，
  *       并在函数结束时清零RSMOD。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       canInitStruct: CAN配置信息结构体。
  * 返回  无。
  */
void
CAN_Configuration(CAN_SFRmap* CANx, CAN_InitTypeDef* canInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(canInitStruct->m_Enable));
    CHECK_RESTRICTION(CHECK_CAN_WORK_MODE(canInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_CAN_SOURCE(canInitStruct->m_WorkSource));
    CHECK_RESTRICTION(CHECK_CAN_BAUDRATE_PRESET(canInitStruct->m_BaudRate));
    CHECK_RESTRICTION(CHECK_CAN_SYNC_JMP_WIDTH(canInitStruct->m_SyncJumpWidth));
    CHECK_RESTRICTION(CHECK_CAN_TIME_SEGMENT1(canInitStruct->m_TimeSeg1));
    CHECK_RESTRICTION(CHECK_CAN_TIME_SEGMENT2(canInitStruct->m_TimeSeg2));
    CHECK_RESTRICTION(CHECK_CAN_BUS_SAMPLE(canInitStruct->m_BusSample));

    /*---------------- 配置CANx_CTLR寄存器 ----------------*/
    /* 根据结构体成员m_Enable，设置CANEN位域 */
    /* 根据结构体成员m_Mode，设置LBACK和SILENT位域 */
    /* 根据结构体成员m_WorkSource，设置CANCKS位域 */
    /* 配置RSMOD，进入复位模式 */
    tmpreg = ((uint32_t)canInitStruct->m_Enable << CAN_CTLR_CANEN_POS) \
           | canInitStruct->m_Mode \
           | canInitStruct->m_WorkSource \
           | CAN_CTLR_RSMOD;
    CANx->CTLR = SFR_Config (CANx->CTLR, ~CAN_CTLR_INIT_MASK, tmpreg);

    /*---------------- 配置CANx_BRGR寄存器 ----------------*/
    /* 根据结构体成员m_BaudRate，设置CANBRP位域 */
    /* 根据结构体成员m_SyncJumpWidth，设置SJW位域 */
    /* 根据结构体成员m_TimeSeg1，设置TSEG1位域 */
    /* 根据结构体成员m_TimeSeg2，设置TSEG2位域 */
    /* 根据结构体成员m_BusSample，设置SAM位域 */
    tmpreg = ((uint32_t)canInitStruct->m_BaudRate << CAN_BRGR_CANBRP0_POS) \
           | ((uint32_t)canInitStruct->m_SyncJumpWidth << CAN_BRGR_SJW0_POS) \
           | ((uint32_t)canInitStruct->m_TimeSeg1 << CAN_BRGR_TSEG1_0_POS) \
           | ((uint32_t)canInitStruct->m_TimeSeg2 << CAN_BRGR_TSEG2_0_POS) \
           | (canInitStruct->m_BusSample);
    CANx->BRGR = SFR_Config (CANx->BRGR, ~CAN_BRGR_INIT_MASK, tmpreg);

    /*---------------- 配置CANx_ACRR寄存器 ----------------*/
    CANx->ACRR = canInitStruct->m_Acceptance;

    /*---------------- 配置CANx_MSKR寄存器 ----------------*/
    CANx->MSKR = canInitStruct->m_AcceptanceMask;

    /* 退出复位模式 */
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_RSMOD_POS);
}

/**
  * 描述  初始化CAN配置信息结构体。
  * 输入  canInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
CAN_Struct_Init (CAN_InitTypeDef* canInitStruct)
{
    /* 初始化 CAN使能选择 */
    canInitStruct->m_Enable = FALSE;

    /* 初始化 CAN工作模式 */
    canInitStruct->m_Mode = CAN_MODE_NORMAL;

    /* 初始化 CAN工作时钟 */
    canInitStruct->m_WorkSource = CAN_SOURCE_SCLK_DIV_2;

    /* 初始化 CAN波特率预设值 */
    canInitStruct->m_BaudRate = 0;

    /* 初始化 CAN同步跳转宽度 */
    canInitStruct->m_SyncJumpWidth = 0;

    /* 初始化 CAN时间段1 */
    canInitStruct->m_TimeSeg1 = 0;

    /* 初始化 CAN时间段2 */
    canInitStruct->m_TimeSeg2 = 0;

    /* 初始化 CAN总线采样次数 */
    canInitStruct->m_BusSample = CAN_BUS_SAMPLE_1_TIME;

    /* 初始化 CAN验收代码 */
    canInitStruct->m_Acceptance = 0;

    /* 初始化 CAN验收掩码 */
    canInitStruct->m_AcceptanceMask = 0;
}
/**
  *   ##### 控制器局域网总线(CAN)初始化函数定义结束 #####
  */


/**
  *   ##### 控制器局域网总线(CAN)功能配置函数 #####
  */
/**
  * 描述  获取CAN RX信息计数器。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  RX信息计数器，有效值为5位。
  */
uint32_t
CAN_Get_Receive_Message_Counter (CAN_SFRmap* CANx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*---------------- 读取CANx_CTLR寄存器CANRMC位 ----------------*/
    tmpreg = CANx->CTLR;
    tmpreg &= CAN_CTLR_CANRMC;
    tmpreg >>= CAN_CTLR_CANRMC0_POS;

    return tmpreg;
}

/**
  * 描述  获取CAN传输事件状态类型。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Type: CAN传输事件状态类型，取值为:
  *               CAN_BUS_OFF_STATUS: 总线关闭状态
  *               CAN_ERROR_STATUS: 出错状态
  *               CAN_TX_STATUS: 发送状态
  *               CAN_RX_STATUS: 接收状态
  *               CAN_TX_COMPLETE_OFF_STATUS: 发送完毕状态
  *               CAN_TX_BUFFER_STATUS: 发送缓冲器状态
  *               CAN_DATA_OVERFLOW_STATUS: 数据满状态
  *               CAN_RX_BLANK_STATUS: 数据空状态
  * 返回  1: 总线关闭/出错/正在发送/正在接收/发送完毕/CPU可以访问发送缓冲器/
  *          RAM中未读数据满/RAM中有未读信息，
  *       0: 总线开启/无错/发送空闲/接收空闲/发送未完成/CPU不能访问发送缓冲器/
  *          RAM中未读数据未满/RAM中无未读信息。
  */
FlagStatus
CAN_Get_Transmit_Status (CAN_SFRmap* CANx, uint32_t Type)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_STATUS(Type));

    /*---------------- 读取CANx_CTLR寄存器对应位 ----------------*/
    tmpreg = CANx->CTLR;

    if (tmpreg & Type)
    {
        /* 状态位为1 */
        return SET;
    }
    else
    {
        /* 状态位为0 */
        return RESET;
    }
}

/**
  * 描述  配置CAN使能。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       NewState: CAN使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
CAN_Cmd (CAN_SFRmap* CANx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置CANx_CTLR寄存器CANEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* CAN使能 */
        SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_CANEN_POS);
    }
    else
    {
        /* CAN禁止 */
        SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_CANEN_POS);
    }
}

/**
  * 描述  配置CAN工作时钟选择。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       ClockSource: CAN工作时钟，取值为:
  *                      CAN_SOURCE_SCLK_DIV_2: 选择SCLK主时钟/2作为CAN工作时钟
  *                      CAN_SOURCE_HFCLK_DIV_2: 选择HFCLK时钟/2作为CAN工作时钟
  *                      CAN_SOURCE_LFCLK_DIV_2: 选择LFCLK时钟/2作为CAN工作时钟
  * 返回  无。
  */
void
CAN_Clock_Source_Config (CAN_SFRmap* CANx, uint32_t ClockSource)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_SOURCE(ClockSource));

    /*---------------- 配置CANx_CTLR寄存器CANCKS位 ----------------*/
    CANx->CTLR = SFR_Config (CANx->CTLR, ~CAN_CTLR_CANCKS, ClockSource);
}

/**
  * 描述  配置CAN睡眠模式使能。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       NewState: CAN睡眠模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
CAN_Sleep_Mode_Enable (CAN_SFRmap* CANx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置CANx_CTLR寄存器SLEEP位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能睡眠模式 */
        SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_SLEEP_POS);
    }
    else
    {
        /* 禁止睡眠模式 */
        SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_SLEEP_POS);
    }
}

/**
  * 描述  配置CAN复位模式使能。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       NewState: CAN复位模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
CAN_Reset_Mode_Enable (CAN_SFRmap* CANx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置CANx_CTLR寄存器RSMOD位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能复位模式 */
        SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_RSMOD_POS);
    }
    else
    {
        /* 禁止复位模式 */
        SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_RSMOD_POS);
    }
}

/**
  * 描述  配置CAN工作模式。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       ModeType: CAN四种模式，取值为:
  *                   CAN_MODE_NORMAL: 禁止安静和回环模式
  *                   CAN_MODE_SILENT: 使能安静模式
  *                   CAN_MODE_LOOPBACK: 使能回环模式
  *                   CAN_MODE_SILENT_LOOPBACK: 使能安静和回环模式
  * 返回  无。
  */
void
CAN_Work_Mode_Config (CAN_SFRmap* CANx, uint32_t ModeType)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_WORK_MODE(ModeType));

    /*------------ 配置CANx_CTLR寄存器SILENT和LBACK位 ------------*/
    CANx->CTLR = SFR_Config (CANx->CTLR,
                        ~(CAN_CTLR_SILENT | CAN_CTLR_LBACK),
                        ModeType);
}

/**
  * 描述  配置CAN总线采样次数。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Times: 总线采样次数，取值为:
  *                CAN_BUS_SAMPLE_1_TIME: 总线采样1次
  *                CAN_BUS_SAMPLE_3_TIMES: 总线采样3次
  * 返回  无。
  */
void
CAN_Bus_Sample_Times_Config (CAN_SFRmap* CANx, uint32_t Times)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_BUS_SAMPLE(Times));

    /*---------------- 配置CANx_BRGR寄存器SAM位 ----------------*/
    if (Times != CAN_BUS_SAMPLE_1_TIME)
    {
        /* 总线采样3次 */
        SFR_SET_BIT_ASM(CANx->BRGR, CAN_BRGR_SAM_POS);
    }
    else
    {
        /* 总线采样1次 */
        SFR_CLR_BIT_ASM(CANx->BRGR, CAN_BRGR_SAM_POS);
    }
}

/**
  * 描述  配置CAN两个时间段。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       TimeSeg1: CAN时间段1，取值为4位有效数值。
  *       TimeSeg2: CAN时间段2，取值为3位有效数值。
  * 返回  无。
  */
void
CAN_Time_Segment_Config (CAN_SFRmap* CANx,
                    uint32_t TimeSeg1, uint32_t TimeSeg2)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_TIME_SEGMENT1(TimeSeg1));
    CHECK_RESTRICTION(CHECK_CAN_TIME_SEGMENT2(TimeSeg2));

    /*------------- 配置CANx_BRGR寄存器TSEG1/TSEG2位 -------------*/
    tmpreg = (TimeSeg1 << CAN_BRGR_TSEG1_0_POS) \
           | (TimeSeg2 << CAN_BRGR_TSEG2_0_POS);
    CANx->BRGR = SFR_Config (CANx->BRGR,
                        ~(CAN_BRGR_TSEG1 | CAN_BRGR_TSEG2),
                        tmpreg);
}

/**
  * 描述  配置CAN同步跳转宽度。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       JumpWidth: 同步跳转宽度，取值为2位有效数值。
  * 返回  无。
  */
void
CAN_Sync_Jump_Width_Config (CAN_SFRmap* CANx, uint32_t JumpWidth)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_SYNC_JMP_WIDTH(JumpWidth));

    /*------------- 配置CANx_BRGR寄存器SJW位 -------------*/
    tmpreg = JumpWidth << CAN_BRGR_SJW0_POS;
    CANx->BRGR = SFR_Config (CANx->BRGR, ~CAN_BRGR_SJW, tmpreg);
}

/**
  * 描述  配置CAN同步跳转宽度。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       BaudRate: 波特率预设值，取值为6位有效数值。
  * 返回  无。
  */
void
CAN_Baud_Rate_Preset_Config (CAN_SFRmap* CANx, uint32_t BaudRate)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_BAUDRATE_PRESET(BaudRate));

    /*------------- 配置CANx_BRGR寄存器CANBRP位 -------------*/
    tmpreg = BaudRate << CAN_BRGR_CANBRP0_POS;
    CANx->BRGR = SFR_Config (CANx->BRGR, ~CAN_BRGR_CANBRP, tmpreg);
}

/**
  * 描述  获取CAN总线错误信息。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       canErrorStruct: CAN总线错误信息结构体，记录错误信息。
  * 返回  无。
  */
void
CAN_Get_Error_Code (CAN_SFRmap* CANx, CAN_ErrorTypeDef* canErrorStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(canErrorStruct != (CAN_ErrorTypeDef*)0);

    /*---------------- 读取CANx_RCR寄存器 ----------------*/
    tmpreg = CANx->RCR;

    /*---------------- 获取CANx_RCR寄存器CANERRC位 ----------------*/
    tmpreg1 = tmpreg & CAN_RCR_CANERRC;
    tmpreg1 >>= CAN_RCR_CANERRC0_POS;
    canErrorStruct->m_ErrorCode = tmpreg1;

    /*---------------- 获取CANx_RCR寄存器CANDIR位 ----------------*/
    tmpreg1 = tmpreg & CAN_RCR_CANDIR;
    tmpreg1 >>= CAN_RCR_CANDIR_POS;
    canErrorStruct->m_ErrorDirection = tmpreg1;

    /*---------------- 获取CANx_RCR寄存器CANSEG位 ----------------*/
    tmpreg1 = tmpreg & CAN_RCR_CANSEG;
    tmpreg1 >>= CAN_RCR_CANSEG0_POS;
    canErrorStruct->m_ErrorSegment = tmpreg1;

    /*---------------- 获取CANx_RCR寄存器CANALC位 ----------------*/
    tmpreg1 = tmpreg & CAN_RCR_CANALC;
    tmpreg1 >>= CAN_RCR_CANALC0_POS;
    canErrorStruct->m_ArbitrationLost = tmpreg1;
}

/**
  * 描述  获取CAN错误报警限制。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  错误报警限制，取值为8位数值。
  */
uint8_t
CAN_Get_Error_Warning_Limit (CAN_SFRmap* CANx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*---------------- 读取CANx_EROR寄存器CANEWL位 ----------------*/
    tmpreg = CANx->EROR;

    return (uint8_t)tmpreg;
}

/**
  * 描述  获取CAN发送/接收错误计数器。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Direction: CAN错误传输方向，取值为：
  *                    CAN_ERROR_AT_TX: 发送错误计数器
  *                    CAN_ERROR_AT_RX: 接收错误计数器
  * 返回  错误计数器，取值为8位数值。
  */
uint8_t
CAN_Get_Error_Counter (CAN_SFRmap* CANx, uint32_t Direction)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_ERROR_DIR(Direction));

    /*----------- 读取CANx_EROR寄存器 -----------*/
    tmpreg = CANx->EROR;

    if (Direction != CAN_ERROR_AT_TX)
    {
        /*----------- 读取CANRXE位 -----------*/
        tmpreg &= CAN_EROR_CANRXE;
        tmpreg >>= CAN_EROR_CANRXE0_POS;
    }
    else
    {
        /*----------- 读取CANTXE位 -----------*/
        tmpreg &= CAN_EROR_CANTXE;
        tmpreg >>= CAN_EROR_CANTXE0_POS;
    }

    return (uint8_t)tmpreg;
}

/**
  * 描述  配置CAN错误报警限制。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       ErrorLimit: 错误报警限制，取值为8位有效数值。
  * 返回  无。
  */
void
CAN_Error_Warning_Limit_Config (CAN_SFRmap* CANx, uint8_t ErrorLimit)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*------------- 配置CANx_EROR寄存器CANEWL位 -------------*/
    tmpreg = ErrorLimit << CAN_EROR_CANEWL0_POS;
    CANx->EROR = SFR_Config (CANx->EROR, ~CAN_EROR_CANEWL, tmpreg);
}

/**
  * 描述  配置CAN发送/接收错误计数器。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Direction: CAN错误传输方向，取值为：
  *                    CAN_ERROR_AT_TX: 发送错误计数器
  *                    CAN_ERROR_AT_RX: 接收错误计数器
  *       ErrorCounter: 错误计数，取值为8位有效数值。
  * 返回  无。
  */
void
CAN_Error_Counter_Config (CAN_SFRmap* CANx,
                    uint32_t Direction, uint8_t ErrorCounter)
{
    uint32_t tmpreg = 0;
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_ERROR_DIR(Direction));

    if (Direction != CAN_ERROR_AT_TX)
    {
        /*----------- 读取CANRXE位 -----------*/
        tmpreg = ErrorCounter << CAN_EROR_CANRXE0_POS;
        tmpmask = CAN_EROR_CANRXE;
    }
    else
    {
        /*----------- 读取CANTXE位 -----------*/
        tmpreg = ErrorCounter << CAN_EROR_CANTXE0_POS;
        tmpmask = CAN_EROR_CANTXE;
    }

    /*------------- 配置CANx_EROR寄存器 -------------*/
    CANx->EROR = SFR_Config (CANx->EROR, ~tmpmask, tmpreg);
}

/**
  * 描述  配置CAN验收代码。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Acceptance: 验收代码，取值为32位有效数值。
  * 返回  无。
  */
void
CAN_Acceptance_Config (CAN_SFRmap* CANx, uint32_t Acceptance)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*------------- 配置CANx_ACRR寄存器ACR位 -------------*/
    CANx->ACRR = Acceptance;
}

/**
  * 描述  获取CAN验收代码。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  验收代码，取值为32位有效数值。
  */
uint32_t
CAN_Get_Acceptance (CAN_SFRmap* CANx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*------------- 读取CANx_ACRR寄存器ACR位 -------------*/
    tmpreg = CANx->ACRR;

    return tmpreg;
}

/**
  * 描述  配置CAN验收屏蔽。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Acceptance: 验收屏蔽，取值为32位有效数值。
  * 返回  无。
  */
void
CAN_Acceptance_Mask_Config (CAN_SFRmap* CANx, uint32_t Acceptance)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*------------- 配置CANx_MSKR寄存器MSK位 -------------*/
    CANx->MSKR = Acceptance;
}

/**
  * 描述  获取CAN验收屏蔽。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  验收屏蔽，取值为32位有效数值。
  */
uint32_t
CAN_Get_Acceptance_Mask (CAN_SFRmap* CANx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*------------- 读取CANx_MSKR寄存器MSK位 -------------*/
    tmpreg = CANx->MSKR;

    return tmpreg;
}
/**
  *   ##### 控制器局域网总线(CAN)功能配置函数定义结束 #####
  */


/**
  *   ##### 控制器局域网总线(CAN)发送接收函数 #####
  */
/**
  * 描述  CAN模块(CAN)发送缓冲配置。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       canInitStruct: CAN模块配置信息结构体指针。
  * 返回  无。
  */
void
CAN_Transmit_Message_Configuration (CAN_SFRmap* CANx,
                    CAN_MessageTypeDef* canInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_FRAME_FORMAT(canInitStruct->m_FrameFormat));
    CHECK_RESTRICTION(CHECK_CAN_REMOTE_REQUEST(canInitStruct->m_RemoteTransmit));
    CHECK_RESTRICTION(CHECK_CAN_DATA_LENGTH(canInitStruct->m_DataLength));
    CHECK_RESTRICTION(CHECK_CAN_SFF_ID(canInitStruct->m_StandardID));
    CHECK_RESTRICTION(CHECK_CAN_EFF_ID(canInitStruct->m_ExtendedID));

    /*---------------- 设置CANx_INFR寄存器 ----------------*/
    /* 根据结构体成员m_FrameFormat，设置IDE位域 */
    /* 根据结构体成员m_RemoteTransmit，设置RTR位域 */
    /* 根据结构体成员m_DataLength，设置DLC位域 */
    tmpreg = (canInitStruct->m_FrameFormat << CAN_INFR_IDE_POS) \
           | (canInitStruct->m_RemoteTransmit << CAN_INFR_RTR_POS) \
           | (canInitStruct->m_DataLength << CAN_INFR_DLC0_POS);
    CANx->INFR = SFR_Config (CANx->INFR, ~CAN_INFR_INIT_MASK, tmpreg);

    /*----------- 设置CANx_TX0R/CANx_TX1R/CANx_TX2R寄存器 -----------*/
    /* 根据结构体成员m_FrameFormat，选择帧格式 */
    /* 根据结构体成员m_StandardID，设置IDx位域 */
    /* 根据结构体成员m_ExtendedID，设置IDx位域 */
    /* 根据结构体成员m_Data，设置CANTXDTx位域 */
    if (canInitStruct->m_FrameFormat != CAN_FRAME_FORMAT_EFF)
    {
        /*----------- 设置CANx_TX0R寄存器 -----------*/
        tmpreg = (canInitStruct->m_StandardID << CAN_TX0R_ID18_POS) \
               | ((uint32_t)canInitStruct->m_Data[0] << CAN_TX0R_CANTX0_8_POS) \
               | ((uint32_t)canInitStruct->m_Data[1] << CAN_TX0R_CANTX0_0_POS);
        CANx->TX0R = SFR_Config (CANx->TX0R, ~CAN_TX0R_SFF_MASK, tmpreg);

        /*----------- 设置CANx_TX1R寄存器 -----------*/
        tmpreg = ((uint32_t)canInitStruct->m_Data[2] << CAN_TX1R_CANTX1_24_POS) \
               | ((uint32_t)canInitStruct->m_Data[3] << CAN_TX1R_CANTX1_16_POS) \
               | ((uint32_t)canInitStruct->m_Data[4] << CAN_TX1R_CANTX1_8_POS) \
               | ((uint32_t)canInitStruct->m_Data[5] << CAN_TX1R_CANTX1_0_POS);
        CANx->TX1R = tmpreg;

        /*----------- 设置CANx_TX2R寄存器 -----------*/
        tmpreg = ((uint32_t)canInitStruct->m_Data[6] << CAN_TX2R_CANTX2_24_POS) \
               | ((uint32_t)canInitStruct->m_Data[7] << CAN_TX2R_CANTX2_16_POS);
        CANx->TX2R = SFR_Config (CANx->TX2R, ~CAN_TX2R_SFF_MASK, tmpreg);
    }
    else
    {
        /*----------- 设置CANx_TX0R寄存器 -----------*/
        tmpreg = canInitStruct->m_ExtendedID << CAN_TX0R_ID0_POS;
        CANx->TX0R = SFR_Config (CANx->TX0R, ~CAN_TX0R_EFF_MASK, tmpreg);

        /*----------- 设置CANx_TX1R寄存器 -----------*/
        tmpreg = ((uint32_t)canInitStruct->m_Data[0] << CAN_TX1R_CANTX1_24_POS) \
               | ((uint32_t)canInitStruct->m_Data[1] << CAN_TX1R_CANTX1_16_POS) \
               | ((uint32_t)canInitStruct->m_Data[2] << CAN_TX1R_CANTX1_8_POS) \
               | ((uint32_t)canInitStruct->m_Data[3] << CAN_TX1R_CANTX1_0_POS);
        CANx->TX1R = tmpreg;

        /*----------- 设置CANx_TX2R寄存器 -----------*/
        tmpreg = ((uint32_t)canInitStruct->m_Data[4] << CAN_TX2R_CANTX2_24_POS) \
               | ((uint32_t)canInitStruct->m_Data[5] << CAN_TX2R_CANTX2_16_POS) \
               | ((uint32_t)canInitStruct->m_Data[6] << CAN_TX2R_CANTX2_8_POS) \
               | ((uint32_t)canInitStruct->m_Data[7] << CAN_TX2R_CANTX2_0_POS);
        CANx->TX2R = tmpreg;
    }
}

/**
  * 描述  获取CAN模块(CAN)接收缓冲的一个帧信息。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       ReceiveOffset: RAM存储空间起始地址，取值为8位有效数，4字节对齐。
  *       canInitStruct: CAN模块配置信息结构体指针。
  * 返回  无。
  */
void
CAN_Receive_Message_Configuration (CAN_SFRmap* CANx,
                    uint32_t ReceiveOffset, CAN_MessageTypeDef* canInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpaddr = 0;
    uint8_t dataoffset[] = {1,0,7,6,5,4,11,10};
    uint32_t i = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_RECEIVE_OFFSET(ReceiveOffset));

    /* 获取RXDATA0，存储区域第一个数据 */
    tmpreg = (uint32_t)CANx;
    switch (tmpreg)
    {
#ifdef  KF32A_Periph_can0
		case CAN0_ADDR:
			tmpaddr = CAN0_RECEIVE_ADDR;
			break;
#endif
#ifdef  KF32A_Periph_can1
		case CAN1_ADDR:
			tmpaddr = CAN1_RECEIVE_ADDR;
			break;
#endif
#ifdef  KF32A_Periph_can2
		case CAN2_ADDR:
			tmpaddr = CAN2_RECEIVE_ADDR;
			break;
#endif
#ifdef  KF32A_Periph_can3
		case CAN3_ADDR:
			tmpaddr = CAN3_RECEIVE_ADDR;
			break;
#endif
#ifdef  KF32A_Periph_can4
		case CAN4_ADDR:
			tmpaddr = CAN4_RECEIVE_ADDR;
			break;
#endif
#ifdef  KF32A_Periph_can5
		case CAN5_ADDR:
			tmpaddr = CAN5_RECEIVE_ADDR;
			break;
#endif
		default:
			break;
    }
    tmpaddr += ReceiveOffset;
    tmpreg = *(volatile uint32_t *)tmpaddr;

    /* CAN帧格式 */
    tmpreg1 = (tmpreg & CAN_INFR_IDE) >> CAN_INFR_IDE_POS;
    canInitStruct->m_FrameFormat = tmpreg1;

    /* CAN远程发送请求 */
    tmpreg1 = (tmpreg & CAN_INFR_RTR) >> CAN_INFR_RTR_POS;
    canInitStruct->m_RemoteTransmit = tmpreg1;

    /* 判断有无数据字节的接收 */
    if (tmpreg1 != CAN_DATA_FRAME)//远程帧
    {
        return;
    }
    else//数据帧
    {
        ;
    }

    /* 数据长度 */
    tmpreg1 = (tmpreg & CAN_INFR_DLC) >> CAN_INFR_DLC0_POS;
    canInitStruct->m_DataLength = tmpreg1;

    if (canInitStruct->m_FrameFormat != CAN_FRAME_FORMAT_EFF)
    {
        /* 标准格式SFF */
        /* 获取RXDATA1，存储区域第二个数据 */
        tmpaddr += 4;
        tmpreg = *(volatile uint32_t *)tmpaddr;

        /* 标准帧ID */
        tmpreg1 = (tmpreg & CAN_TX0R_SFF_ID) >> CAN_TX0R_ID18_POS;
        canInitStruct->m_StandardID = tmpreg1;

        /* 获取数据区，数据长度为m_DataLength，最大为8字节 */
        for (i = 0; i < canInitStruct->m_DataLength; i++)
        {
            canInitStruct->m_Data[i] =
                    *(volatile uint8_t *)(tmpaddr + dataoffset[i]);
        }
    }
    else
    {
        /* 扩展帧格式EFF */
        /* 获取RXDATA1，存储区域第二个数据 */
        tmpaddr += 4;
        tmpreg = *(volatile uint32_t *)tmpaddr;

        /* 扩展帧ID */
        tmpreg1 = (tmpreg & CAN_TX0R_EFF_ID) >> CAN_TX0R_ID0_POS;
        canInitStruct->m_ExtendedID = tmpreg1;

        /* 获取数据区，数据长度为m_DataLength，最大为8字节 */
        tmpaddr += 4;
        if (canInitStruct->m_DataLength < 4)
        {
            /* 获取RXDATA2 */
            for (i = 0; i < canInitStruct->m_DataLength; i++)
            {
                canInitStruct->m_Data[i] =
                        *(volatile uint8_t *)(tmpaddr + 3 - i);
            }
        }
        else
        {
            /* 获取RXDATA2 */
            for (i = 0; i < 4; i++)
            {
                canInitStruct->m_Data[i] =
                        *(volatile uint8_t *)(tmpaddr + 3 - i);
            }

            /* 获取RXDATA3 */
            tmpaddr += 4;
            for (i = 4; i < canInitStruct->m_DataLength; i++)
            {
                canInitStruct->m_Data[i] =
                        *(volatile uint8_t *)(tmpaddr + 7 - i);
            }
        }
    }
}

/**
  * 描述  初始化CAN报文信息结构体。
  * 输入  canInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
CAN_Message_Struct_Init (CAN_MessageTypeDef* canInitStruct)
{
    uint32_t i = 0;

    /* 初始化 帧格式选择 */
    canInitStruct->m_FrameFormat = CAN_FRAME_FORMAT_SFF;

    /* 初始化 远程发送请求 */
    canInitStruct->m_RemoteTransmit = CAN_DATA_FRAME;

    /* 初始化 数据长度 */
    canInitStruct->m_DataLength = 0;

    /* 初始化 标准帧ID */
    canInitStruct->m_StandardID = 0;

    /* 初始化 扩展帧ID */
    canInitStruct->m_ExtendedID = 0;

    /* 初始化 数据区 */
    for (i = 0; i < 8; i++)
    {
        canInitStruct->m_Data[i] = 0;
    }
}

/**
  * 描述  清除CAN 清除缓冲器满标志。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  无。
  */
void
CAN_Clear_Buffer_Overflow_Flag (CAN_SFRmap* CANx)
{

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*-------------------- 设置CANx_CTLR寄存器RELF位 --------------------*/
    SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_RELRX_POS);
    while(!((CANx->CTLR & CAN_CTLR_RXBSTA)>>CAN_CTLR_RXBSTA_POS));
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_RELRX_POS);

    SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_RELF_POS);
    while((CANx->CTLR & CAN_CTLR_DOSTA)>>CAN_CTLR_DOSTA_POS);
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_RELF_POS);
}

/**
  * 描述  释放CAN 接收缓冲器。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *    	ReleaseCount: 释放缓冲器次数
  * 返回  无。
  */
void
CAN_Release_Receive_Buffer (CAN_SFRmap* CANx, uint32_t ReleaseCount)
{
	uint32_t i,tmprmc;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*-------------------- 设置CANx_CTLR寄存器RELRX位 --------------------*/
    for(i=0;i<ReleaseCount;i++)
    {
        tmprmc = (CANx->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS;
        if(tmprmc == 0)
        {
        	return ;
        }
        else
        {
    		SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_RELRX_POS);
    		while(tmprmc - ((CANx->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS) != 1);
    		SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_RELRX_POS);
    	}
    }
}

/**
  * 描述  单次发送，当发生错误或者仲裁丢失时不会进行重发（单次发送）
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  无。
  * 备注：调用之前需确保总线开启，并处于空闲状态
  */
void
CAN_Transmit_Single (CAN_SFRmap* CANx)
{
	volatile unsigned int	temp;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*-------------------- 设置CANx_CTLR寄存器TXR位 --------------------*/
//    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_ATX_POS);
//    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_TXR_POS);
    CANx->CTLR = CANx->CTLR | 0x300;
    while((CANx->CTLR & CAN_CTLR_TXSTA)>>CAN_CTLR_TXSTA_POS);
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_TXR_POS);
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_ATX_POS);
}

/**
 * 描述  发送，发生错误、仲裁丢失或发送结束时会进行重发（连续发送）。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  * 返回  无。
  * 备注：调用之前需确保总线开启，并处于空闲状态
  */
void
CAN_Transmit_Repeat (CAN_SFRmap* CANx)
{
	volatile unsigned int	temp;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));

    /*-------------------- 设置CANx_CTLR寄存器TXR位 --------------------*/
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_ATX_POS);
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_TXR_POS);
    SFR_SET_BIT_ASM(CANx->CTLR, CAN_CTLR_TXR_POS);
    while((CANx->CTLR & CAN_CTLR_TXSTA)>>CAN_CTLR_TXSTA_POS);
    SFR_CLR_BIT_ASM(CANx->CTLR, CAN_CTLR_TXR_POS);
}

/**
  * 描述  配置CAN发送缓冲器帧格式。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       FrameFormat: 帧格式，取值为:
  *                      CAN_FRAME_FORMAT_SFF: 标准帧格式SFF
  *                      CAN_FRAME_FORMAT_EFF: 扩展帧格式EFF
  * 返回  无。
  */
void
CAN_Frame_Format_Config (CAN_SFRmap* CANx, uint32_t FrameFormat)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_FRAME_FORMAT(FrameFormat));

    /*---------------- 配置CANx_INFR寄存器IDE位 ----------------*/
    if (FrameFormat != CAN_FRAME_FORMAT_SFF)
    {
        /* 扩展帧格式EFF */
        SFR_SET_BIT_ASM(CANx->INFR, CAN_INFR_IDE_POS);
    }
    else
    {
        /* 标准帧格式SFF */
        SFR_CLR_BIT_ASM(CANx->INFR, CAN_INFR_IDE_POS);
    }
}

/**
  * 描述  配置CAN发送缓冲器远程发送请求。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       RemoteRequest: 远程发送请求，取值为:
  *                        CAN_DATA_FRAME: 数据帧
  *                        CAN_REMOTE_FRAME: 远程帧
  * 返回  无。
  */
void
CAN_Remote_Request_Config (CAN_SFRmap* CANx, uint32_t RemoteRequest)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_REMOTE_REQUEST(RemoteRequest));

    /*---------------- 配置CANx_INFR寄存器RTR位 ----------------*/
    if (RemoteRequest != CAN_DATA_FRAME)
    {
        /* 远程帧 */
        SFR_SET_BIT_ASM(CANx->INFR, CAN_INFR_RTR_POS);
    }
    else
    {
        /* 数据帧 */
        SFR_CLR_BIT_ASM(CANx->INFR, CAN_INFR_RTR_POS);
    }
}

/**
  * 描述  配置CAN发送缓冲器数据长度，大于8的数据长度代码是不可用的。
  *       如果大于8，将以8个字节计。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Length: CAN数据长度，取值为4位有效数值。如果大于8，将以8个字节计。
  * 返回  无。
  */
void
CAN_Data_Length_Config (CAN_SFRmap* CANx, uint32_t Length)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_DATA_LENGTH(Length));

    /*------------- 配置CANx_INFR寄存器DLC位 -------------*/
    tmpreg = Length << CAN_INFR_DLC0_POS;
    CANx->EROR = SFR_Config (CANx->EROR, ~CAN_INFR_DLC, tmpreg);
}

/**
  * 描述  配置CAN发送缓冲器识别码。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       Format: CAN帧格式，取值为:
                    CAN_FRAME_FORMAT_SFF: 标准格式SFF
                    CAN_FRAME_FORMAT_EFF: 扩展格式EFF
  *       IDCode: CAN识别码，标准格式SFF为11位，扩展格式EFF位29位。
  * 返回  无。
  */
void
CAN_Identification_Code_Config (CAN_SFRmap* CANx,
                    uint32_t FrameFormat, uint32_t IDCode)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_FRAME_FORMAT(FrameFormat));
    if (FrameFormat != CAN_FRAME_FORMAT_SFF)
    {
        CHECK_RESTRICTION(CHECK_CAN_EFF_ID(IDCode));
    }
    else
    {
        CHECK_RESTRICTION(CHECK_CAN_SFF_ID(IDCode));
    }

    /*------------- 配置CANx_TX0R寄存器IDx位 -------------*/
    if (FrameFormat != CAN_FRAME_FORMAT_SFF)
    {
        /* 扩展格式EFF */
        tmpreg = IDCode << CAN_TX0R_ID0_POS;
        CANx->TX0R = SFR_Config (CANx->TX0R, ~CAN_TX0R_EFF_ID, tmpreg);
    }
    else
    {
        /* 标准格式SFF */
        tmpreg = IDCode << CAN_TX0R_ID18_POS;
        CANx->TX0R = SFR_Config (CANx->TX0R, ~CAN_TX0R_SFF_ID, tmpreg);
    }
}
/**
  *   ##### 控制器局域网总线(CAN)发送接收函数定义结束 #####
  */


/**
  *   ##### 控制器局域网总线(CAN)中断管理函数 #####
  */
/**
  * 描述  获取CAN中断标志。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       InterruptType: CAN中断类型，取值范围为下面提供的一个:
  *                        CAN_INT_RECEIVE: 接收中断
  *                        CAN_INT_TRANSMIT: 发送中断
  *                        CAN_INT_ERROR_ALARM: 错误报警中断
  *                        CAN_INT_DATA_OVERFLOW: 数据溢出中断
  *                        CAN_INT_WAKE_UP: 唤醒中断
  *                        CAN_INT_ERROR_NEGATIVE: 错误消极中断
  *                        CAN_INT_ARBITRATION_LOST: 仲裁丢失中断
  *                        CAN_INT_BUS_ERROR: 总线错误中断
  * 返回  1:发生中断，0:未发生中断。
  */
FlagStatus
CAN_Get_INT_Flag (CAN_SFRmap* CANx, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_INT_ONE_EVENT(InterruptType));

    /* 配置中断标志掩码 */
    tmpreg = InterruptType << CAN_IFR_CANRXIF_POS;
    /*---------------- 读取CAN_IFR寄存器中断标志位 ----------------*/
    if (CANx->IFR & tmpreg)
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
  * 描述  清零CAN中断标志。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       InterruptType: CAN中断类型，取值范围为下面提供的一个或多个组合:
  *                        CAN_INT_RECEIVE: 接收中断
  *                        CAN_INT_TRANSMIT: 发送中断
  *                        CAN_INT_ERROR_ALARM: 错误报警中断
  *                        CAN_INT_DATA_OVERFLOW: 数据溢出中断
  *                        CAN_INT_WAKE_UP: 唤醒中断
  *                        CAN_INT_ERROR_NEGATIVE: 错误消极中断
  *                        CAN_INT_ARBITRATION_LOST: 仲裁丢失中断
  *                        CAN_INT_BUS_ERROR: 总线错误中断
  * 返回  无。
  */
void
CAN_Clear_INT_Flag (CAN_SFRmap* CANx, uint32_t InterruptType)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_INT_EVENT(InterruptType));

    /*---------------- 清零CAN_IER寄存器中断标志位 ----------------*/
    tmpreg = InterruptType << CAN_IFR_CANRXIF_POS;
    CANx->IER |= InterruptType << CAN_IER_CANRXIC_POS;
    while((CANx->IFR & tmpreg)>0);
    CANx->IER &= ~(InterruptType << CAN_IER_CANRXIC_POS);
}

/**
  * 描述  配置CAN中断使能。
  * 输入  CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN5_SFR。
  *       InterruptType: CAN中断类型，取值范围为下面提供的一个或多个组合:
  *                        CAN_INT_RECEIVE: 接收中断
  *                        CAN_INT_TRANSMIT: 发送中断
  *                        CAN_INT_ERROR_ALARM: 错误报警中断
  *                        CAN_INT_DATA_OVERFLOW: 数据溢出中断
  *                        CAN_INT_WAKE_UP: 唤醒中断
  *                        CAN_INT_ERROR_NEGATIVE: 错误消极中断
  *                        CAN_INT_ARBITRATION_LOST: 仲裁丢失中断
  *                        CAN_INT_BUS_ERROR: 总线错误中断
  *       NewState: DMA通道错误传输中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
CAN_Set_INT_Enable (CAN_SFRmap* CANx,
                    uint32_t InterruptType, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CAN_ALL_PERIPH(CANx));
    CHECK_RESTRICTION(CHECK_CAN_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置CAN_IER寄存器中断使能位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能中断 */
        CANx->IER |= InterruptType << CAN_IER_CANRXIE_POS;
    }
    else
    {
        /* 禁止中断 */
        CANx->IER &= ~(InterruptType << CAN_IER_CANRXIE_POS);
    }
}
/**
  *   ##### 控制器局域网总线(CAN)中断管理函数定义结束 #####
  */

#endif //KF32A_Periph_can

