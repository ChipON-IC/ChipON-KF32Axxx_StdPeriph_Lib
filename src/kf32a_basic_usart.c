/**
  ******************************************************************************
  * 文件名  kf32a_basic_usart.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描  述  该文件提供了USART模块(USART)相关的功能函数，包含：
  *          + USART模块(USART)初始化函数
  *          + USART模块(USART)功能配置函数
  *          + USART模块(USART)7816初始化及配置函数
  *          + USART模块(USART)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_usart.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

/* USART私有定义 ----------------------------------------------------*/
/* USARTx_CTLR寄存器配置掩码 */
#define USART_CTLR_INIT_MASK            (USART_CTLR_SYNC \
                                       | USART_CTLR_CSRS \
                                       | USART_CTLR_TXEN \
                                       | USART_CTLR_RXEN \
                                       | USART_CTLR_DT9EN \
                                       | USART_CTLR_STPW \
                                       | USART_CTLR_PARM \
                                       | USART_CTLR_RTSEN \
                                       | USART_CTLR_CTSEN)

/* USARTx_U7816R_寄存器配置掩码 */
#define USART_U7816R_INIT_MASK          (USART_U7816R_ERSW \
                                       | USART_U7816R_PSEL \
                                       | USART_U7816R_TXRE \
                                       | USART_U7816R_RXRE \
                                       | USART_U7816R_CLKOUT \
                                       | USART_U7816R_CLKDIV \
                                       | USART_U7816R_EGT)

/* USARTx_BRGR_寄存器配置掩码 */
#define USART_BRGR_INIT_MASK            (USART_BRGR_BRGM \
                                       | USART_BRGR_BRGF1 \
                                       | USART_BRGR_BRGF2)


/**
  *   ##### USART模块(USART)初始化函数 #####
  */
/**
  * 描述  USART外设复位
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Reset (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
#ifdef  KF32A_Periph_usart0
    if (USARTx == USART0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USART0CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart1
    if (USARTx == USART1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USART1CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart2
    if (USARTx == USART2_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART2RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART2RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USART2CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart3
    if (USARTx == USART3_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART3RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART3RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USART3CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart4
    if (USARTx == USART4_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART4RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USART4RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USART4CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart5
   if (USARTx == USART5_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_USART5RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_USART5RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_USART5CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart6
   if (USARTx == USART6_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_USART6RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_USART6RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_USART6CLKEN, TRUE);
    }
#endif
#ifdef  KF32A_Periph_usart7
    if (USARTx == USART7_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_USART7RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_USART7RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_USART7CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  配置USART外设初始化。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       USARTInitStruct: USART配置信息结构体指针。
  * 返回  无
  */
void
USART_Configuration (USART_SFRmap* USARTx, USART_InitTypeDef* usartInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_MODE(usartInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_USART_CLOCKSOURCE(usartInitStruct->m_HalfDuplexClkSource));
    CHECK_RESTRICTION(CHECK_USART_DIRECTION(usartInitStruct->m_TransferDir));
    CHECK_RESTRICTION(CHECK_USART_WORD_LENGTH(usartInitStruct->m_WordLength));
    CHECK_RESTRICTION(CHECK_USART_STOPBITS(usartInitStruct->m_StopBits));//
    CHECK_RESTRICTION(CHECK_USART_Bit9_MODE(usartInitStruct->m_Bit9SEL));
    CHECK_RESTRICTION(CHECK_USART_PARITY(usartInitStruct->m_Parity));
    CHECK_RESTRICTION(CHECK_USART_ABRDEN(usartInitStruct->m_BRAutoDetect));
    CHECK_RESTRICTION(CHECK_USART_HARDWARE_FLOW_CONTROL(usartInitStruct->m_HardwareFlowControl));
    CHECK_RESTRICTION(CHECK_USART_RECEIVE_DIR(usartInitStruct->m_ReceiveOrder));
    CHECK_RESTRICTION(CHECK_USART_TRANSMIT_DIR(usartInitStruct->m_TransmitOrder));
    CHECK_RESTRICTION(CHECK_USART_CLK(usartInitStruct->m_BaudRateBRCKS));
    CHECK_RESTRICTION(CHECK_USART_BAUDRATE_DECIMAL(usartInitStruct->m_BaudRateNumerator));
    CHECK_RESTRICTION(CHECK_USART_BAUDRATE_DECIMAL(usartInitStruct->m_BaudRateDenominator));

    /*------------------- USART_CTLR寄存器配置  -----------------*/
    /* 根据结构体成员m_Mode，设置SYNC位域 */
    /* 根据结构体成员m_HalfDuplexClkSource，设置CSRS位域 */
    /* 根据结构体成员m_TransferDir，设置TXEN/RXEN位域 */
    /* 根据结构体成员m_WordLength，设置DT9EN位域 */
    /* 根据结构体成员m_StopBits，设置STPW位域 */
    /* 根据结构体成员m_Bit9SEL，设置TX9SEL位域 */
    /* 根据结构体成员m_Parity，设置PARM位域 */
    /* 根据结构体成员m_HardwareFlowControl，设置RTSEN/CTSEN位域 */
    tmpreg = ((usartInitStruct->m_Mode)     \
            | (usartInitStruct->m_HalfDuplexClkSource)  \
            | (usartInitStruct->m_TransferDir)  \
            | (usartInitStruct->m_WordLength)  \
            | (usartInitStruct->m_Bit9SEL)   \
            | (usartInitStruct->m_StopBits)   \
            | (usartInitStruct->m_ReceiveOrder)   \
            | (usartInitStruct->m_TransmitOrder)   \
            | (usartInitStruct->m_Parity)     \
            | (usartInitStruct->m_BRAutoDetect)     \
            | (usartInitStruct->m_HardwareFlowControl)     \
            | (usartInitStruct->m_BaudRateBRCKS));

    USARTx->CTLR = SFR_Config (USARTx->CTLR, ~USART_CTLR_INIT_MASK, tmpreg);

    /*------------------- USART_BRGR寄存器配置  -----------------*/
    /* 根据结构体成员m_BaudRateInteger，设置BRGM位域 */
    /* 根据结构体成员m_BaudRateNumerator，设置BRGF1位域 */
    /* 根据结构体成员m_BaudRateDenominator，设置BRGF2位域 */
    tmpreg = ((uint32_t)usartInitStruct->m_BaudRateInteger << USART_BRGR_BRGM0_POS)  \
            | ((uint32_t)usartInitStruct->m_BaudRateNumerator << USART_BRGR_BRGF1_0_POS)   \
            | ((uint32_t)usartInitStruct->m_BaudRateDenominator << USART_BRGR_BRGF2_0_POS);
    USARTx->BRGR = SFR_Config (USARTx->BRGR, ~USART_BRGR_INIT_MASK, tmpreg);
}

/**
  * 描述  7816外设配置。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       USARTInitStruct: USART_U7816配置信息
  * 返回  无
  */
void
USART_U7816R_Configuration (USART_SFRmap* USARTx, U7816R_InitTypeDef* usartInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_U7816R_ERRORSIGNAL(usartInitStruct->m_ErrorSignal));
    CHECK_RESTRICTION(CHECK_U7816R_PASSAGEWAY_SELECT(usartInitStruct->m_PassagewaySelect));
    CHECK_RESTRICTION(CHECK_U7816R_TRANSMIT_REPEAT(usartInitStruct->m_TransmitRepeat));
    CHECK_RESTRICTION(CHECK_U7816R_RECEIVE_REPEAT(usartInitStruct->m_ReceiveRepeat));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usartInitStruct->m_Clkout));

    /*---------------------------- USARTx_U7816R寄存器配置  -----------------*/
    /* 根据结构体成员m_ErrorSignal，设置ERSW位域 */
    /* 根据结构体成员m_PassagewaySelect，设置PSEL位域 */
    /* 根据结构体成员m_TransmitRepeat，设置TXRE位域 */
    /* 根据结构体成员m_ReceiveRepeat，设置RXRE位域 */
    /* 根据结构体成员m_Clkout，设置CLKOUT位域 */
    /* 根据结构体成员m_ClkDiv，设置CLKDIV位域 */
    /* 根据结构体成员m_Egt，设置EGT位域 */

    tmpreg = ((usartInitStruct->m_ErrorSignal)      \
            | (usartInitStruct->m_PassagewaySelect) \
            | (usartInitStruct->m_TransmitRepeat)   \
            | (usartInitStruct->m_ReceiveRepeat)    \
            | ((uint32_t)usartInitStruct->m_Clkout << USART_U7816R_CLKOUT_POS) \
            | ((uint32_t)usartInitStruct->m_Egt << USART_U7816R_EGT0_POS)      \
            | ((uint32_t)usartInitStruct->m_ClkDiv << USART_U7816R_CLKDIV0_POS));
    USARTx->U7816R = SFR_Config (USARTx->U7816R, ~USART_U7816R_INIT_MASK, tmpreg);
}

/**
  * 描述  初始化USART配置信息结构体。
  * 输入  usartInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
USART_Struct_Init(USART_InitTypeDef* usartInitStruct)
{
    /* 初始化 USART模式 */
    usartInitStruct->m_Mode = USART_MODE_FULLDUPLEXASY;

    /* 初始化 USART时钟源选择 */
    usartInitStruct->m_HalfDuplexClkSource = USART_SLAVE_CLOCKSOURCE_EXTER;

    /* 初始化 USART传输方向 */
    usartInitStruct->m_TransferDir = USART_DIRECTION_TRANSMIT;

    /* 初始化 USART字长 */
    usartInitStruct->m_WordLength = USART_WORDLENGTH_8B;

    /* 初始化 USART停止位 */
    usartInitStruct->m_StopBits = USART_STOPBITS_1;

    /* 初始化 USART发送数据第9位数据 */
    usartInitStruct->m_Bit9SEL = USART_BIT9_PAR;

    /* 初始化 USART奇偶校验位 */
    usartInitStruct->m_Parity = USART_PARITY_ODD;

    /* 初始化 USART接收次序 */
    usartInitStruct->m_ReceiveOrder = USART_RECEIVE_LSB;

    /* 初始化USART发送次序 */
    usartInitStruct->m_TransmitOrder = USART_TRANSMIT_LSB;

    /* 初始化 USART自动波特率检测使能位 */
    usartInitStruct->m_BRAutoDetect = USART_ABRDEN_OFF;

    /* 初始化 USART硬件流控制 */
    usartInitStruct->m_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;

    /* 初始化 USART波特率发生器时钟选择 */
    usartInitStruct->m_BaudRateBRCKS = USART_CLK_SCLK;

    /* 初始化 USART波特率整数部分 */
    usartInitStruct->m_BaudRateInteger = 1;

    /* 初始化 USART波特率小数分子部分 */
    usartInitStruct->m_BaudRateNumerator = 0;

    /* 初始化 USART波特率小数分母部分 */
    usartInitStruct->m_BaudRateDenominator = 1;
}

/**
  * 描述  初始化USART_7816配置信息结构体。
  * 输入  usartInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
USART_U7816R_Struct_Init(U7816R_InitTypeDef* usartInitStruct)
{
    /* 初始化 USART错误宽度 */
    usartInitStruct->m_ErrorSignal = USART_U7816R_ERRORSIGNAL_2ETU;

    /* 初始化 USART数据通道 */
    usartInitStruct->m_PassagewaySelect = USART_U7816R_PASSAGEWAY_TX0;

    /* 初始化 USART奇偶校验出错时重发送的最大次数 */
    usartInitStruct->m_TransmitRepeat = USART_U7816R_TRANSMIT_REPEAT_0S;

    /* 初始化 USART奇偶校验出错时重接收的最大次数 */
    usartInitStruct->m_ReceiveRepeat = USART_U7816R_RECEIVE_REPEAT_0S;

    /* 初始化 USB端点输入使能 */
    usartInitStruct->m_Clkout = FALSE;

    /* 初始化 7816工作时钟和引脚输出时钟控制 */
    usartInitStruct->m_ClkDiv = 0;

    /* 初始化 发送时插入的EGT */
    usartInitStruct->m_Egt = 0;
}
/**
  *   ##### USART模块(USART)初始化函数定义结束 #####
  */


/**
  *   ##### USART模块(USART)功能配置函数 #####
  */
/**
  * 描述  配置USART使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART使能使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Cmd (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USART_CTLR寄存器USARTEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART */
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_USARTEN_POS);
    }
    else
    {
        /* 禁止USART */
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_USARTEN_POS);
    }
}

/**
  * 描述  配置USART波特率发生器时钟选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       CLK: 时钟选择，取值范围为：
  *              USART_CLK_SCLK  :主时钟
  *              USART_CLK_HFCLK :高频外设时钟
  *              USART_CLK_LFCLK :低频外设时钟
  * 返回  无。
  */
void
USART_BaudRate_Clock_Config(USART_SFRmap* USARTx, uint32_t CLK)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_CLK(CLK));

    /*---------------- 设置USART_CTLR寄存器BRCKS位 ----------------*/
    USARTx->CTLR = SFR_Config (USARTx->CTLR, ~USART_CTLR_BRCKS, CLK);
}

/**
  * 描述  设置USART半双工同步时钟极性选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART半双工同步时钟极性，取值为：
  *                   USART_CPOL_HIGH:数据在时钟下降沿同步，
  *                   USART_CPOL_LOW :数据在时钟上升沿同步。
  * 返回  无。
  */
void
USART_HalfDuplex_ClockPolarity_Config(USART_SFRmap* USARTx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_CPOL(NewState));

    /*---------------- 设置USART_CTLR寄存器SCKPS位 ----------------*/
    if (NewState != USART_CPOL_LOW)
    {
        /* 数据在时钟下降沿同步 */
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_SCKPS_POS);
    }
    else
    {
        /* 数据在时钟上升沿同步*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_SCKPS_POS);
    }
}

/**
  * 描述  设置USART发送次序选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART发送次序，取值为：
  *                   USART_TRANSMIT_MSB:先发送MSB，
  *                   USART_TRANSMIT_LSB:先发送LSB。
  * 返回  无。
  */
void
USART_Transmit_Order_Config (USART_SFRmap* USARTx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_TRANSMIT_DIR(NewState));

    /*---------------- 设置USART_CTLR寄存器TCONV位 ----------------*/
    if (NewState != USART_TRANSMIT_LSB)
    {
        /* 先发送MSB */
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_TCONV_POS);
    }
    else
    {
        /* 先发送LSB */
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_TCONV_POS);
    }
}

/**
  * 描述  设置USART接收次序选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART接收次序，取值为：
  *                   USART_RECEIVE_MSB:先接收MSB，
  *                   USART_RECEIVE_LSB:先接收LSB。
  * 返回  无。
  */
void
USART_Receive_Order_Config (USART_SFRmap* USARTx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_RECEIVE_DIR(NewState));

    /*---------------- 设置USART_CTLR寄存器RCONV位 ----------------*/
    if (NewState != USART_RECEIVE_LSB)
    {
        /* 先接收MSB */
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_RCONV_POS);
    }
    else
    {
        /* 先接收LSB */
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_RCONV_POS);
    }
}

/**
  * 描述  设置红外调制高电平输出极性。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 红外调制高电平输出极性，取值为
  *                   TRUE: 发送和接收反向电平，
  *                   FALSE: 发送和接收正向电平。
  * 返回  无。
  */
void
USART_Infrare_Detector_Voltage_Config(USART_SFRmap* USARTx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器DINV位 ----------------*/
    if (NewState != FALSE)
    {
        /* 发送和接收反向电平*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_DINV_POS);
    }
    else
    {
        /* 发送和接收正向电平*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_DINV_POS);
    }
}

/**
  * 描述  设置USART唤醒使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART唤醒使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_WeakUP_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器WUEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART唤醒 */
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_WUEN_POS);
    }
    else
    {
        /* 禁止USART唤醒 */
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_WUEN_POS);
    }
}

/**
  * 描述  设置USART时钟源选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       ClockSource: USART时钟源选择，取值为TRUE 或 FALSE。
  *                      USART_MASTER_CLOCKSOURCE_INTER: 主模式(由BRG内部产生时钟)，
  *                      USART_SLAVE_CLOCKSOURCE_EXTER: 从模式(时钟源来自外部)。
  * 返回  无。
  */
void
USART_Clock_Source_Config(USART_SFRmap* USARTx, uint32_t ClockSource)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_CLOCKSOURCE(ClockSource));

    /*---------------- 设置USART_CTLR寄存器CSRS位 ----------------*/
    if (ClockSource != USART_SLAVE_CLOCKSOURCE_EXTER)
    {
        /* 主模式(由BRG内部产生时钟*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_CSRS_POS);
    }
    else
    {
        /* 从模式(时钟源来自外部*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_CSRS_POS);
    }
}

/**
  * 描述  设置USART地址检测使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART地址检测使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Address_Detection_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器ADREN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能地址检测，当地址位为1时数据装入BUF*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_ADREN_POS);
    }
    else
    {
        /* 禁止地址检测，接收所有字节并且第9位可作为奇偶校验位*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_ADREN_POS);
    }
}

/**
  * 描述  设置USART自动波特率检测使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART自动波特率检测使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Auto_BaudRate_Detection_Enable(USART_SFRmap* USARTx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器ABRDEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能自动波特率模式(完成自动波特率后清0)*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_ABRDEN_POS);
    }
    else
    {
        /* 禁止自动波特率模式*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_ABRDEN_POS);
    }
}
/**
  * 描述  读取USART自动波特率检测使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART自动波特率检测使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
FlagStatus USART_Get_Auto_BaudRate_Detection_Flag(USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_CTLR寄存器ABRDEN位 ----------------*/
    if (USARTx->CTLR & USART_CTLR_ABRDEN)
    {
        return SET;
    }
    else
    {
    	/* 自动波特率完成自动清零 */
    	return RESET;
    }
}
/**
  * 描述  设置USART发送间隔字符使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART发送间隔字符使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Send_Blank_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器SENDB位 ----------------*/
    if (NewState != FALSE)
    {
        /* 发送间隔字符使能(完成后由软件清0)*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_SENDB_POS);
    }
    else
    {
        /* 未使能发送间隔字符*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_SENDB_POS);
    }
}

/**
  * 描述  设置USART串行通信模式选择位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewMode: USART模式选择，取值为：
  *                  USART_MODE_HALFDUPLEXSYN:半双工同步模式，
  *                  USART_MODE_FULLDUPLEXASY:全双工异步模式。
  * 返回  无。
  */
void
USART_SYN_Choice_Config(USART_SFRmap* USARTx, uint32_t NewMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_MODE(NewMode));

    /*---------------- 设置USART_CTLR寄存器SYNC位 ----------------*/
    if (NewMode != USART_MODE_FULLDUPLEXASY)
    {
        /* 半双工同步模式*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_SYNC_POS);
    }
    else
    {
        /*全双工异步模式*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_SYNC_POS);
    }
}

/**
  * 描述  设置USART发送使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART发送使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Transmit_Data_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器TXEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能发送*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_TXEN_POS);
    }
    else
    {
        /* 禁止发送*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_TXEN_POS);
    }
}

/**
  * 描述  设置USART接收使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART接收使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Receive_Data_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器RXEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能接收*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_RXEN_POS);
    }
    else
    {
        /* 禁止接收*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_RXEN_POS);
    }
}

/**
  * 描述  设置USART停止位长度选择位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewLength: 取值为USART_STOPBITS_2 或 USART_STOPBITS_1。
  *                    USART_STOPBITS_2 :停止位长度为2bits，
  *                    USART_STOPBITS_1 :停止位长度为1bit。
  * 返回  无。
  */
void
USART_STOP_Word_Config(USART_SFRmap* USARTx, uint32_t NewLength)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_STOPBITS(NewLength));

    /*---------------- 设置USART_CTLR寄存器STPW位 ----------------*/
    if (NewLength != USART_STOPBITS_1)
    {
        /* 停止位长度为2bits*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_STPW_POS);
    }
    else
    {
        /* 停止位长度为1bit*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_STPW_POS);
    }
}

/**
  * 描述  设置USART发送数据第9位数据选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 取值为USART_BIT9_RS485 或 USART_BIT9_PAR。
  *                   USART_BIT9_RS485 :RS-485模式，
  *                   USART_BIT9_PAR   :选择奇偶校验。
  * 返回  无。
  */
void
USART_Transmit_9Word_Select_Config (USART_SFRmap* USARTx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_Bit9_MODE(NewState));

    /*---------------- 设置USART_CTLR寄存器TX9SEL位 ----------------*/
    if (NewState != USART_BIT9_PAR)
    {
        /* RS-485模式*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_TX9SEL_POS);
    }
    else
    {
        /* 选择奇偶校验*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_TX9SEL_POS);
    }
}

/**
  * 描述  设置USART奇偶校验选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 取值为USART_PARITY_EVEN 或USART_PARITY_ODD。
  *                   USART_PARITY_EVEN :偶校验，
  *                   USART_PARITY_ODD  :奇校验。
  * 返回  无。
  */
void
USART_Parity_Select_Config(USART_SFRmap* USARTx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_PARITY(NewState));

    /*---------------- 设置USART_CTLR寄存器PARM位 ----------------*/
    if (NewState != USART_PARITY_ODD)
    {
        /* 偶校验*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_PARM_POS);
    }
    else
    {
        /* 奇校验*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_PARM_POS);
    }
}

/**
  * 描述  设置USART第9位数据使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART第9位数据使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_9Data_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器DT9EN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能第9位数据发送和接收*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_DT9EN_POS);
    }
    else
    {
        /* 未使能第9位数据发送和接收*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_DT9EN_POS);
    }
}

/**
  * 描述  设置USART CTS使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART CTS使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_CTS_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器CTSEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CTS功能*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_CTSEN_POS);
    }
    else
    {
        /* 禁能CTS功能*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_CTSEN_POS);
    }
}

/**
  * 描述  设置USART RTS使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART RTS使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_RTS_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器RTSEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能RTS功能*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_RTSEN_POS);
    }
    else
    {
        /* 禁能RTS功能*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_RTSEN_POS);
    }
}

/**
  * 描述  设置USART 红外调制使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 红外调制使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Infrare_Detector_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器IRDAEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能IrDA功能*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_IRDAEN_POS);
    }
    else
    {
        /* 禁能IrDA功能*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_IRDAEN_POS);
    }
}
/**
  * 描述  设置USART RESHD位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 单线模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_RESHD_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器RESHD位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能单线模式*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_RESHD_POS);
    }
    else
    {
        /* 禁止单线模式*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_RESHD_POS);
    }
}
/**
  * 描述  设置USART 单线模式使能位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 单线模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Singlet_Line_Mode_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器SLMEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能单线模式*/
        SFR_SET_BIT_ASM(USARTx->CTLR, USART_CTLR_SLMEN_POS);
    }
    else
    {
        /* 禁止单线模式*/
        SFR_CLR_BIT_ASM(USARTx->CTLR, USART_CTLR_SLMEN_POS);
    }
}

/**
  * 描述  配置USART USARTDIV的整数部分。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       DIV: 取值范围为0~65535。
  * 返回  无。
  */
void
USART_BaudRate_Integer_Config(USART_SFRmap* USARTx, uint16_t DIV)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_BRGR寄存器BRGM位 ----------------*/
    tmpreg = DIV << USART_BRGR_BRGM0_POS;
    USARTx->BRGR = SFR_Config (USARTx->BRGR, ~USART_BRGR_BRGM, tmpreg);
}

/**
  * 描述  配置USART 小数波特率分子。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       DIV: 取值范围为0~15。
  * 返回  无。
  */
void
USART_BaudRate_Decimal1_Config(USART_SFRmap* USARTx, uint32_t DIV)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_BAUDRATE_DECIMAL(DIV));

    /*---------------- 设置USART_BRGR寄存器BRGF1位 ----------------*/
    tmpreg = DIV << USART_BRGR_BRGF1_0_POS ;
    USARTx->BRGR = SFR_Config (USARTx->BRGR, ~USART_BRGR_BRGF1, tmpreg);
}

/**
  * 描述  配置USART 小数波特率分母。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       DIV: 取值范围为0~15。
  * 返回  无。
  */
void
USART_BaudRate_Decimal2_Config(USART_SFRmap* USARTx, uint32_t DIV)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_USART_BAUDRATE_DECIMAL(DIV));

    /*---------------- 设置USART_BRGR寄存器BRGF2位 ----------------*/
    tmpreg = DIV << USART_BRGR_BRGF2_0_POS ;
    USARTx->BRGR = SFR_Config (USARTx->BRGR, ~USART_BRGR_BRGF2, tmpreg);
}

/**
  * 描述  USART发送数据。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       Data: 写入数据寄存器的值，取值为0~255。
  */
void
USART_SendData(USART_SFRmap* USARTx, uint8_t Data)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_TBUFR寄存器 ----------------*/
    USARTx->TBUFR = Data;
}

/**
  * 描述  USART发送字节,并等待发送器和缓冲器空。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       Data: 写入数据寄存器的值，取值为0~255。
  */
void
USART_TransmitData(USART_SFRmap* USARTx, uint8_t Data)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    /*---------------- 	等待发送器为空	----------------*/
     while(!(USARTx->STR & USART_STR_TXEIF));
    /*---------------- 设置USART_TBUFR寄存器 ----------------*/
    USARTx->TBUFR = Data;
}

/**
  * 描述  USART接收数据。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  32位类型数据的8位数据。
  */
uint32_t
USART_ReceiveData(USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_RBUFR寄存器 ----------------*/
    return USARTx->RBUFR;
}

/**
  * 描述  配置 USARTx地址匹配功能设置位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       address: USARTx地址匹配功能，取值范围为0~255。
  * 返回  无。
  */
void
USART_Address_Match_Config(USART_SFRmap* USARTx, uint8_t DIV)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_ADM寄存器ADM位 ----------------*/
    tmpreg = DIV << USART_ADM_ADM0_POS;
    USARTx->ADM = SFR_Config (USARTx->ADM, ~USART_ADM_ADM, tmpreg);
}
/**
  *   ##### USART模块(USART)功能配置函数结束 #####
  */


/**
  *   ##### USART模块(USART)7816初始化及配置函数 #####
  */
/**
  * 描述  设置USART 7816模式使能控制。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 7816模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_7816_Cmd(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_U7816R寄存器7816EN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能7816模式*/
        SFR_SET_BIT_ASM(USARTx->U7816R, USART_U7816R_7816EN_POS);
    }
    else
    {
        /* 禁能7816模式*/
        SFR_CLR_BIT_ASM(USARTx->U7816R, USART_U7816R_7816EN_POS);
    }
}

/**
  * 描述  设置USART 7816时钟输出使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 7816时钟输出使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_7816_CLKOUT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_U7816R寄存器CLKOUT位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能时钟输出*/
        SFR_SET_BIT_ASM(USARTx->U7816R, USART_U7816R_CLKOUT_POS);
    }
    else
    {
        /* 禁止时钟输出*/
        SFR_CLR_BIT_ASM(USARTx->U7816R, USART_U7816R_CLKOUT_POS);
    }
}

/**
  * 描述  配置USART 7816 error signal宽度选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       signal: 宽度选择，取值范围为：
  *                 USART_U7816R_ERRORSIGNAL_2ETU   : 2etu
  *                 USART_U7816R_ERRORSIGNAL_1P5ETU : 1.5etu
  *                 USART_U7816R_ERRORSIGNAL_1ETU   : 1etu
  * 返回  无。
  */
void
USART_7816_Error_Signal_Config (USART_SFRmap* USARTx, uint32_t ERRORSIGNAL)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_U7816R_ERRORSIGNAL(ERRORSIGNAL));

    /*---------------- 设置USART_U7816R寄存器ERSW位 ----------------*/
    USARTx->U7816R = SFR_Config (USARTx->U7816R, ~USART_U7816R_ERSW, ERRORSIGNAL);
}

/**
  * 描述  设置USART 数据通道选择。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 取值为USART_U7816R_PASSAGEWAY_TX1  或 USART_U7816R_PASSAGEWAY_TX0。
  *                   USART_U7816R_PASSAGEWAY_TX1 :选择通道1(TX1)，
  *                   USART_U7816R_PASSAGEWAY_TX0 :选择通道0(TX0)
  * 返回  无。
  */
void
USART_Passageway_Select_Config(USART_SFRmap* USARTx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_U7816R_PASSAGEWAY_SELECT(NewState));

    /*---------------- 设置USART_U7816R寄存器PSEL位 ----------------*/
    if (NewState != USART_U7816R_PASSAGEWAY_TX0)
    {
        /* 选择通道1(TX1)*/
        SFR_SET_BIT_ASM(USARTx->U7816R, USART_U7816R_PSEL_POS);
    }
    else
        {
        /* 选择通道0(TX0)*/
        SFR_CLR_BIT_ASM(USARTx->U7816R, USART_U7816R_PSEL_POS);
    }
}

/**
  * 描述  设置USART 控制接收到发送之间是否插入BGT。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 取值为TRUE 或 FALSE。
  *                   TRUE:插入BGT，宽度为22etu，
  *                   FALSE:不插入BGT
  * 返回  无。
  */
void
USART_BGT_Config(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_U7816R寄存器BGT位 ----------------*/
    if (NewState != FALSE)
    {
        /* 插入BGT，宽度为22etu*/
        SFR_SET_BIT_ASM(USARTx->U7816R, USART_U7816R_BGT_POS);
    }
    else
    {
        /* 不插入BGT*/
        SFR_CLR_BIT_ASM(USARTx->U7816R, USART_U7816R_BGT_POS);
    }
}
/**
  * 描述  设置USART 重发使能控制。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 取值为TRUE 或 FALSE。
  *                   TRUE:收到校验错误信号后重发，达到最大次数后进入中断，
  *                   FALSE:收到校验错误信号后直接进入中断
  * 返回  无。
  */
void
USART_Transmit_Repeat_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_U7816R寄存器TREPEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 收到校验错误信号后重发，达到最大次数后进入中断*/
        SFR_SET_BIT_ASM(USARTx->U7816R, USART_U7816R_TREPEN_POS);
    }
    else
    {
        /* 收到校验错误信号后直接进入中断*/
        SFR_CLR_BIT_ASM(USARTx->U7816R, USART_U7816R_TREPEN_POS);
    }
}

/**
  * 描述  设置USART 重收使能控制。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: 取值为TRUE 或 FALSE。
  *               TRUE:奇偶校验错误后重收数据，达到最大次数后进入中断，
  *               FALSE:奇偶校验错误后直接进入中断
  * 返回  无。
  */
void
USART_Receive_Repeat_Enable(USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_U7816R寄存器RREPEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 奇偶校验错误后重收数据，达到最大次数后进入中断*/
        SFR_SET_BIT_ASM(USARTx->U7816R, USART_U7816R_RREPEN_POS);
    }
    else
    {
        /* 奇偶校验错误后直接进入中断*/
        SFR_CLR_BIT_ASM(USARTx->U7816R, USART_U7816R_RREPEN_POS);
    }
}

/**
  * 描述  配置USART 奇偶校验出错时重发送的最大次数。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       SELECT: 最大次数选择，取值范围为：
  *                 USART_U7816R_TRANSMIT_REPEAT_0S: 最大发送收次数为0
  *                 USART_U7816R_TRANSMIT_REPEAT_1S: 最大发送收次数为1
  *                 USART_U7816R_TRANSMIT_REPEAT_2S: 最大发送收次数为2
  *                 USART_U7816R_TRANSMIT_REPEAT_3S: 最大发送收次数为3
  * 返回  无。
  */
void
USART_Transmit_Repeat_Times_Config(USART_SFRmap* USARTx, uint32_t SELECT)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_U7816R_TRANSMIT_REPEAT(SELECT));

    /*---------------- 设置USART_U7816R寄存器TXRE位 ----------------*/
    USARTx->U7816R = SFR_Config (USARTx->U7816R, ~USART_U7816R_TXRE, SELECT);
}

/**
  * 描述  配置USART 奇偶校验出错时重接收的最大次数。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       SELECT: 最大次数选择，取值范围为：
  *                 USART_U7816R_RECEIVE_REPEAT_0S: 最大发送收次数为0
  *                 USART_U7816R_RECEIVE_REPEAT_1S: 最大发送收次数为1
  *                 USART_U7816R_RECEIVE_REPEAT_2S: 最大发送收次数为2
  *                 USART_U7816R_RECEIVE_REPEAT_3S: 最大发送收次数为3
  * 返回  无。
  */
void
USART_Receive_Repeat_Times_Config(USART_SFRmap* USARTx, uint32_t SELECT)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_U7816R_RECEIVE_REPEAT(SELECT));

    /*---------------- 设置USART_U7816R寄存器RXRE位 ----------------*/
    USARTx->U7816R = SFR_Config (USARTx->U7816R, ~USART_U7816R_RXRE, SELECT);
}

/**
  * 描述  配置USART 7816工作时钟和引脚输出时钟控制。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       CLKDIV: 取值范围为0~255。
  * 返回  无。
  */
void
USART_7816_CLKDIV_Config (USART_SFRmap* USARTx, uint8_t DIV)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_U7816R寄存器位CLKDIV ----------------*/
    tmpreg = DIV << USART_U7816R_CLKDIV0_POS;
    USARTx->U7816R = SFR_Config (USARTx->U7816R, ~USART_U7816R_CLKDIV, tmpreg);
}

/**
  * 描述  配置USART 7816发送时插入的EGT(extra guard time)（单位etu）。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       EGT: 取值范围为0~255。
  * 返回  无。
  */
void
USART_7816_EGT_Config(USART_SFRmap* USARTx, uint8_t EGT)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_U7816R寄存器位EGT ----------------*/
    tmpreg = EGT << USART_U7816R_EGT0_POS;
    USARTx->U7816R = SFR_Config (USARTx->U7816R, ~USART_U7816R_EGT, tmpreg);
}
/**
  *   ##### USART模块(USART)7816初始化及配置函数定义结束 #####
  */


/**
  *   ##### USART模块(USART)中断管理函数 #####
  */
/**
  * 描述  设置USART接收溢出中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART接收溢出中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Receive_Overflow_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器OVFEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART接收溢出中断 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_OVFEIE_POS);
    }
    else
    {
        /* 禁止USART接收溢出中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_OVFEIE_POS);
    }
}

/**
  * 描述  设置USART奇偶校验错误中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART奇偶校验错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Parity_ERROR_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器PAREIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART奇偶校验错误中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_PAREIE_POS);
    }
    else
    {
        /* 禁止USART奇偶校验错误中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_PAREIE_POS);
    }
}

/**
  * 描述  设置USART帧错误中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART帧错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Frame_ERROE_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器FREIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART帧错误中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_FREIE_POS);
    }
    else
    {
        /* 禁止USART帧错误中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_FREIE_POS);
    }
}

/**
  * 描述  设置USART间隔符位中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART间隔符位中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Blank_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器BRIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART间隔符位中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_BRIE_POS);
    }
    else
    {
        /* 禁止USART间隔符位中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_BRIE_POS);
    }
}

/**
  * 描述  设置USART自动波特率超时中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART自动波特率超时中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Auto_BaudRate_TimeOver_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器BRIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART自动波特率超时中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_ABTOIE_POS);
    }
    else
    {
        /* 禁止USART自动波特率超时中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_ABTOIE_POS);
    }
}

/**
  * 描述  设置USART自动唤醒中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART自动唤醒中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_WeakUP_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器WUIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART自动唤醒中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_WUIE_POS);
    }
    else
    {
        /* 禁止USART自动唤醒中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_WUIE_POS);
    }
}

/**
  * 描述  设置USART7816发送错误中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART7816发送错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Transmit_ERROR_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器TEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART7816发送错误中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_TEIE_POS);
    }
    else
    {
        /* 禁止USART7816发送错误中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_TEIE_POS);
    }
}

/**
  * 描述  设置USART7816接收错误中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART7816接收错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Receive_ERROR_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器REIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART7816接收错误中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_REIE_POS);
    }
    else
    {
        /* 禁止USART7816接收错误中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_REIE_POS);
    }
}

/**
  * 描述  设置USART CTS中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART CTS中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_CTS_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器CTSIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART CTS中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_CTSIE_POS);
    }
    else
    {
        /* 禁止USART CTS中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_CTSIE_POS);
    }
}

/**
  * 描述  设置USART RDR中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART RDR中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_RDR_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器RDRIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART RDR中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_RDRIE_POS);
    }
    else
    {
        /* 禁止USART RDR中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_RDRIE_POS);
    }
}

/**
  * 描述  设置USART TFE中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART TFE中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_TFE_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器TFEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART TFE中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_TFEIE_POS);
    }
    else
    {
        /* 禁止USART TFE中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_TFEIE_POS);
    }
}

/**
  * 描述  设置USART TXE中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART TXE中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_TXE_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器TXEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART TXE中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_TXEIE_POS);
    }
    else
    {
        /* 禁止USART TXE中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_TXEIE_POS);
    }
}

/**
  * 描述  设置USART 接收数据DMA中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 接收数据DMA中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Receive_DMA_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器URCDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART 接收数据DMA中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_URCDE_POS);
    }
    else
    {
        /* 禁止USART 接收数据DMA中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_URCDE_POS);
    }
}

/**
  * 描述  设置USART 发送数据DMA中断使能。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  *       NewState: USART 发送数据DMA中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USART_Transmit_DMA_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_IER寄存器UTXDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USART 发送数据DMA中断位 */
        SFR_SET_BIT_ASM(USARTx->IER, USART_IER_UTXDE_POS);
    }
    else
    {
        /* 禁止USART 发送数据DMA中断 */
        SFR_CLR_BIT_ASM(USARTx->IER, USART_IER_UTXDE_POS);
    }
}

/**
  * 描述  获取USART接收溢出中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART接收溢出；
  *       0: USART没有接收溢出。
  */
FlagStatus
USART_Get_Receive_Overflow_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器OVFEIF位 ----------------*/
    if (USARTx->STR & USART_STR_OVFEIF)
    {
        /* USART接收溢出 */
        return SET;
    }
    else
    {
        /* USART没有接收溢出 */
        return RESET;
    }
}

/**
  * 描述  获取USART奇偶校验错误中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART奇偶校验错误；
  *       0: USART没有接收溢出。
  */
FlagStatus
USART_Get_Parity_ERROR_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器PAREIF位 ----------------*/
    if (USARTx->STR & USART_STR_PAREIF)
    {
        /* USART奇偶校验错误 */
        return SET;
    }
    else
    {
        /* USART奇偶校验未发生错误 */
        return RESET;
    }
}

/**
  * 描述  获取USART帧错误中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART接收数据时发生帧错误；
  *       0: USART接收数据时未发生帧错误。
  */
FlagStatus
USART_Get_Frame_ERROR_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器FREIF位 ----------------*/
    if (USARTx->STR & USART_STR_FREIF)
    {
        /* USART接收数据时发生了帧错误 */
        return SET;
    }
    else
    {
        /* USART接收数据时未发生帧错误 */
        return RESET;
    }
}

/**
  * 描述  获取USART间隔符中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART接收到间隔字符；
  *       0: USART未接受到间隔字符。
  */
FlagStatus
USART_Get_Blank_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器BRIF位 ----------------*/
    if (USARTx->STR & USART_STR_BRIF)
    {
        /* USART接收到间隔字符 */
        return SET;
    }
    else
    {
        /* USART未接受到间隔字符 */
        return RESET;
    }
}

/**
  * 描述  获取USART自动波特率超时中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART自动波特率超时；
  *       0: USART自动波特率未超时。
  */
FlagStatus
USART_Get_Auto_Baudrate_TimeOver_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器ABTOIF位 ----------------*/
    if (USARTx->STR & USART_STR_ABTOIF)
    {
        /* USART自动波特率超时 */
        return SET;
    }
    else
    {
        /* USART自动波特率未超时 */
        return RESET;
    }
}

/**
  * 描述  获取USART自动唤醒中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART发生了自动唤醒事件；
  *       0: USART未发生自动唤醒事件。
  */
FlagStatus
USART_Get_WeakUP_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器WUIF位 ----------------*/
    if (USARTx->STR & USART_STR_WUIF)
    {
        /* USART发生了自动唤醒事件*/
        return SET;
    }
    else
    {
        /* USART未发生自动唤醒事件 */
        return RESET;
    }
}

/**
  * 描述  获取USART 7816发送错误中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART产生了7816发送错误中断；
  *       0: USART未产生7816发送错误中断。
  */
FlagStatus
USART_Get_7816Transmit_ERROR_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器TEIF位 ----------------*/
    if (USARTx->STR & USART_STR_TEIF)
    {
        /* USART 产生了7816发送错误中断*/
        return SET;
    }
    else
    {
        /* USART 未产生7816发送错误中断 */
        return RESET;
    }
}

/**
  * 描述  获取USART 7816接收错误中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART产生了7816接收错误中断；
  *       0: USART未产生7816接收错误中断。
  */
FlagStatus
USART_Get_7816Receive_ERROR_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器REIF 位 ----------------*/
    if (USARTx->STR & USART_STR_REIF)
    {
        /* USART 产生了7816接收错误中断*/
        return SET;
    }
    else
    {
        /* USART 未产生7816接收错误中断 */
        return RESET;
    }
}

/**
  * 描述  获取USART CTS中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART接收到有效CTS信号；
  *       0: USART未接收到有效CTS信号。
  */
FlagStatus
USART_Get_CTS_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器CTSIF位 ----------------*/
    if (USARTx->STR & USART_STR_CTSIF)
    {
        /* USART 接收到有效CTS信号*/
        return SET;
    }
    else
    {
        /* USART 未接收到有效CTS信号 */
        return RESET;
    }
}

/**
  * 描述  获取USART 数据就绪中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART接收BUF中有数据可读；
  *       0: USART接收BUF中无数据可读。
  */
FlagStatus
USART_Get_Receive_BUFR_Ready_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器RDRIF位 ----------------*/
    if (USARTx->STR & USART_STR_RDRIF)
    {
        /* USART 接收BUF中有数据可读*/
        return SET;
    }
    else
    {
        /* USART 接收BUF中无数据可读 */
        return RESET;
    }
}

/**
  * 描述  获取USART 唤醒使能位状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回      1: USART正在等待下降沿；
  *       0: USART接收器正常工作。
  */
FlagStatus
USART_Get_WUEN_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_CTLR寄存器WUEN位 ----------------*/
    if (USARTx->CTLR & USART_CTLR_WUEN)
    {
        /* 等待下降沿*/
        return SET;
    }
    else
    {
    	/*正常工作*/
        return RESET;
    }
}
/**
  * 描述  获取USART 发送BUF为空中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART发送BUF为空；
  *       0: USART发送BUF不为空。
  */
FlagStatus
USART_Get_Transmit_BUFR_Empty_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器TFEIF位 ----------------*/
    if (USARTx->STR & USART_STR_TFEIF)
    {
        /* USART 发送BUF为空*/
        return SET;
    }
    else
    {
        /* USART 发送BUF不为空 */
        return RESET;
    }
}

/**
  * 描述  获取USART 发射器为空中断标志状态 。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  1: USART发射器为空；
  *       0: USART发射器不为空。
  */
FlagStatus
USART_Get_Transmitter_Empty_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 读取USART_STR寄存器TXEIF位 ----------------*/
    if (USARTx->STR & USART_STR_TXEIF)
    {
        /* USART 发射器为空*/
        return SET;
    }
    else
    {
        /* USART 发射器不为空 */
        return RESET;
    }
}

/**
  * 描述  清零USART接收溢出中断标志。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Receive_Overflow_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器OVFEIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_OVFEIC_POS);
    while(USARTx->STR & USART_STR_OVFEIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_OVFEIC_POS);
}

/**
  * 描述  清零USART奇偶校验错误位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Parity_ERROR_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器PAREIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_PAREIC_POS);
    while(USARTx->STR & USART_STR_PAREIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_PAREIC_POS);
}

/**
  * 描述  清零USART帧错误位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Frame_ERROR_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器FREIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_FREIC_POS);
    while(USARTx->STR & USART_STR_FREIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_FREIC_POS);
}

/**
  * 描述  清零USART间隔符位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Blank_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器BRIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_BRIC_POS);
    while(USARTx->STR & USART_STR_BRIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_BRIC_POS);
}

/**
  * 描述  清零USART自动波特率超时位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Auto_BaudRate_TimeOver_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器ABTOIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_ABTOIC_POS);
    while(USARTx->STR & USART_STR_ABTOIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_ABTOIC_POS);
}

/**
  * 描述  清零USART自动唤醒中断位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_WeakUP_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器WUIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_WUIC_POS);
    while(USARTx->STR & USART_STR_WUIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_WUIC_POS);
}

/**
  * 描述  清零USART发送错误中断位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Transmit_ERROR_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器TEIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_TEIC_POS);
    while(USARTx->STR & USART_STR_TEIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_TEIC_POS);
}

/**
  * 描述  清零USART接收错误中断位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Receive_ERROR_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器REIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_REIC_POS);
    while(USARTx->STR & USART_STR_REIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_REIC_POS);
}

/**
  * 描述  清零USART CTS中断位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_CTS_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器REIC位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_CTSIC_POS);
    while(USARTx->STR & USART_STR_CTSIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_CTSIC_POS);
}

/**
  * 描述  清零USART 接收BUF中断位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Receive_BUFR_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器RCFR位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_RCFR_POS);
    while(USARTx->STR & USART_STR_RDRIF);
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_RCFR_POS);
}

/**
  * 描述  清零USART 发送BUF中断位。
  * 输入  USARTx: 指向USART内存结构的指针，取值为USART0_SFR~USART8_SFR。
  * 返回  无。
  */
void
USART_Clear_Transmit_BUFR_INT_Flag (USART_SFRmap* USARTx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USART_ALL_PERIPH(USARTx));

    /*---------------- 设置USART_STR寄存器TXFR位 ----------------*/
    SFR_SET_BIT_ASM(USARTx->STR, USART_STR_TXFR_POS);
    while(!((USARTx->STR & USART_STR_TFEIF)>>USART_STR_TFEIF_POS));
    SFR_CLR_BIT_ASM(USARTx->STR, USART_STR_TXFR_POS);
}
/**
  *   ##### USART模块(USART)中断管理函数定义结束 #####
  */

