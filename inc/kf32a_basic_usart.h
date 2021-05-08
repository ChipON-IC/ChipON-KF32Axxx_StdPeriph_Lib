/**
  ******************************************************************************
  * 文件名  kf32a_basic_usart.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描述    该文件用于USART外设的库函数声明及相关宏定义。
  *
  *********************************************************************
  */

#ifndef _KF32A_BASIC_USART_H_
#define _KF32A_BASIC_USART_H_

#include "KF32A_BASIC.h"

/* USART外设判断 ------------------------------------------------------------*/
//#define CHECK_USART_ALL_PERIPH(PERIPH)   (((PERIPH) == USART0_SFR) \
//                                       || ((PERIPH) == USART1_SFR) \
//                                       || ((PERIPH) == USART2_SFR) \
//                                       || ((PERIPH) == USART3_SFR) \
//                                       || ((PERIPH) == USART4_SFR) \
//                                       || ((PERIPH) == USART5_SFR) \
//                                       || ((PERIPH) == USART6_SFR) \
//                                       || ((PERIPH) == USART7_SFR))
#define CHECK_USART_ALL_PERIPH(PERIPH)   (((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40000980)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40000A00)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40000A80)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40000B00)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40000B80)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40001E80)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40001F00)) \
                                       || ((PERIPH) == ((USART_SFRmap *)(uint32_t)0x40001F80)))

/**   描述 USART配置信息结构体   */
typedef struct
{
    uint32_t m_Mode;                /* USART模式配置.
                                       取值为宏“USART模式选择”中的一个 */
    uint32_t m_HalfDuplexClkSource; /* 时钟源选择,取值为宏“USART时钟源选择”中的一个。*/
    uint32_t m_TransferDir;         /* USART传输方向,取值为宏“USART传输方向”中的一个。*/
    uint32_t m_WordLength;          /* USART字长配置取值为宏“USART字长”中的一个 */
    uint32_t m_StopBits;            /* USART停止位配置 取值为宏“USART停止位”中的一个 */
    uint32_t m_Bit9SEL;             /* USART发送数据第9位数据选择.取值为宏“USART发送数据第9位数据选择”中的一个 */
    uint32_t m_Parity;              /* USART奇偶校验位配置取值为宏“USART奇偶校验位”中的一个*/
    uint32_t m_ReceiveOrder;        /* USART接收次序设置为宏“接收次序选择”中的一个*/
    uint32_t m_TransmitOrder;       /* USART发送次序设置为宏“发送次序选择”中的一个*/
    uint32_t m_BRAutoDetect;        /* USART自动波特率模式设置为宏“自动波特率检测使能位”中的一个*/
    uint32_t m_HardwareFlowControl; /* USART硬件流控制配置取值为宏“USART硬件流”中的一个 */
    uint16_t m_BaudRateBRCKS;     /* USART波特率发生器时钟选择取值为宏“USART时钟配置”中的一个 */
    uint16_t m_BaudRateInteger;     /* USART波特率整数部分，取值为0~65535。 */
    uint16_t m_BaudRateNumerator;   /* USART波特率小数分子部分，取值为0~0xF。 */
    uint16_t m_BaudRateDenominator; /* USART波特率小数分母部分，取值为0~0xF。 */
} USART_InitTypeDef;

/**
  * 描述 USART_7816配置信息结构体
  */
typedef struct
{
    uint32_t m_ErrorSignal;         /* USART错误宽度，
                                       取值为宏“USART错误宽度选择配置”中的一个 */
    uint32_t m_PassagewaySelect;    /* USART数据通道.
                                       取值为宏“USART数据通道选择”中的一个 */
    uint32_t m_TransmitRepeat;      /* USART奇偶校验出错时重发送的最大次数，
                                       取值为宏“USART奇偶校验出错时重发送的最大次数”中的一个 */
    uint32_t m_ReceiveRepeat;       /* USART奇偶校验出错时重接收的最大次数，
                                       取值为宏“USART奇偶校验出错时重接收的最大次数”中的一个 */
    FunctionalState m_Clkout;       /* USB端点输入使能，
                                       取值为TRUE或FALSE。 */
    uint8_t m_ClkDiv;               /* 7816工作时钟和引脚输出时钟控制，
                                       取值为0~0xFF。 */
    uint8_t m_Egt;                  /* 发送时插入的EGT，
                                       取值为0~0xFF。 */
} U7816R_InitTypeDef;

/**
  * USART错误宽度选择配置
  */
#define USART_U7816R_ERRORSIGNAL_2ETU          ((uint32_t)0<<USART_U7816R_ERSW0_POS)
#define USART_U7816R_ERRORSIGNAL_1P5ETU        ((uint32_t)2<<USART_U7816R_ERSW0_POS)
#define USART_U7816R_ERRORSIGNAL_1ETU          ((uint32_t)3<<USART_U7816R_ERSW0_POS)
#define CHECK_U7816R_ERRORSIGNAL(ERRORSIGNAL)  (((ERRORSIGNAL) == USART_U7816R_ERRORSIGNAL_2ETU ) \
                                             || ((ERRORSIGNAL) == USART_U7816R_ERRORSIGNAL_1P5ETU) \
                                             || ((ERRORSIGNAL) == USART_U7816R_ERRORSIGNAL_1ETU))

/**
  * USART数据通道选择
  */
#define USART_U7816R_PASSAGEWAY_TX0            ((uint32_t)0<<USART_U7816R_PSEL_POS)
#define USART_U7816R_PASSAGEWAY_TX1            ((uint32_t)1<<USART_U7816R_PSEL_POS)
#define CHECK_U7816R_PASSAGEWAY_SELECT(SELECT) (((SELECT) == USART_U7816R_PASSAGEWAY_TX0) \
                                             || ((SELECT) == USART_U7816R_PASSAGEWAY_TX1))

/**
  * USART奇偶校验出错时重发送的最大次数
  */
#define USART_U7816R_TRANSMIT_REPEAT_0S        ((uint32_t)0<<USART_U7816R_TXRE0_POS)
#define USART_U7816R_TRANSMIT_REPEAT_1S        ((uint32_t)1<<USART_U7816R_TXRE0_POS)
#define USART_U7816R_TRANSMIT_REPEAT_2S        ((uint32_t)2<<USART_U7816R_TXRE0_POS)
#define USART_U7816R_TRANSMIT_REPEAT_3S        ((uint32_t)3<<USART_U7816R_TXRE0_POS)
#define CHECK_U7816R_TRANSMIT_REPEAT(SELECT)   (((SELECT) == USART_U7816R_TRANSMIT_REPEAT_0S) \
                                             || ((SELECT) == USART_U7816R_TRANSMIT_REPEAT_1S) \
                                             || ((SELECT) == USART_U7816R_TRANSMIT_REPEAT_2S) \
                                             || ((SELECT) == USART_U7816R_TRANSMIT_REPEAT_3S))
/**
  * USART奇偶校验出错时重接收的最大次数
  */
#define USART_U7816R_RECEIVE_REPEAT_0S         ((uint32_t)0<<USART_U7816R_RXRE0_POS)
#define USART_U7816R_RECEIVE_REPEAT_1S         ((uint32_t)1<<USART_U7816R_RXRE0_POS)
#define USART_U7816R_RECEIVE_REPEAT_2S         ((uint32_t)2<<USART_U7816R_RXRE0_POS)
#define USART_U7816R_RECEIVE_REPEAT_3S         ((uint32_t)3<<USART_U7816R_RXRE0_POS)
#define CHECK_U7816R_RECEIVE_REPEAT(SELECT)    (((SELECT) == USART_U7816R_RECEIVE_REPEAT_0S) \
                                             || ((SELECT) == USART_U7816R_RECEIVE_REPEAT_1S) \
                                             || ((SELECT) == USART_U7816R_RECEIVE_REPEAT_2S) \
                                             || ((SELECT) == USART_U7816R_RECEIVE_REPEAT_3S))

/**
  * USART传输方向
  */
#define USART_DIRECTION_NONE                    ((uint32_t)0<<USART_CTLR_TXEN_POS)
#define USART_DIRECTION_TRANSMIT                ((uint32_t)1<<USART_CTLR_TXEN_POS)
#define USART_DIRECTION_RECEIVE                 ((uint32_t)2<<USART_CTLR_TXEN_POS)
#define USART_DIRECTION_FULL_DUPLEX             ((uint32_t)3<<USART_CTLR_TXEN_POS)
#define CHECK_USART_DIRECTION(SELECT)           (((SELECT) == USART_DIRECTION_NONE) \
                                              || ((SELECT) == USART_DIRECTION_TRANSMIT) \
                                              || ((SELECT) == USART_DIRECTION_RECEIVE) \
                                              || ((SELECT) == USART_DIRECTION_FULL_DUPLEX))

/**
  * USART字长
  */
#define USART_WORDLENGTH_8B                     ((uint32_t)0<<USART_CTLR_DT9EN_POS)
#define USART_WORDLENGTH_9B                     ((uint32_t)1<<USART_CTLR_DT9EN_POS)
#define CHECK_USART_WORD_LENGTH(LENGTH)         (((LENGTH) == USART_WORDLENGTH_8B) \
                                              || ((LENGTH) == USART_WORDLENGTH_9B))

/**
  * USART停止位长度
  */
#define USART_STOPBITS_1                        ((uint32_t)0<<USART_CTLR_STPW_POS)
#define USART_STOPBITS_2                        ((uint32_t)1<<USART_CTLR_STPW_POS)
#define CHECK_USART_STOPBITS(STOPBITS)          (((STOPBITS) == USART_STOPBITS_1) \
                                              || ((STOPBITS) == USART_STOPBITS_2))

/**
  * USART发送数据第9位数据选择
  */
#define USART_BIT9_PAR                          ((uint32_t)0<<USART_CTLR_TX9SEL_POS)
#define USART_BIT9_RS485                        ((uint32_t)1<<USART_CTLR_TX9SEL_POS)
#define CHECK_USART_Bit9_MODE(MODE)             (((MODE) == USART_BIT9_PAR) \
                                              || ((MODE) == USART_BIT9_RS485))

/**
  * USART奇偶校验
  */
#define USART_PARITY_ODD                        ((uint32_t)0<<USART_CTLR_PARM_POS)
#define USART_PARITY_EVEN                       ((uint32_t)1<<USART_CTLR_PARM_POS)
#define CHECK_USART_PARITY(PARITY)              (((PARITY) == USART_PARITY_EVEN)  \
                                              || ((PARITY) == USART_PARITY_ODD))

/**
  * USART模式选择
  */
#define USART_MODE_FULLDUPLEXASY                ((uint32_t)0<<USART_CTLR_SYNC_POS)
//#define USART_MODE_HALFDUPLEXSYN                ((uint32_t)1<<USART_CTLR_SYNC_POS)
#define USART_MODE_HALFDUPLEXSYN                ((uint32_t)0x10001<<USART_CTLR_SYNC_POS)
#define CHECK_USART_MODE(MODE)                  (((MODE) == USART_MODE_FULLDUPLEXASY) \
                                              || ((MODE) == USART_MODE_HALFDUPLEXSYN))
/**
  * USART硬件流控制
  */
#define USART_HARDWAREFLOWCONTROL_NONE          ((uint32_t)0x0<<USART_CTLR_CTSEN_POS)
#define USART_HARDWAREFLOWCONTROL_CTS           ((uint32_t)0x1<<USART_CTLR_CTSEN_POS)
#define USART_HARDWAREFLOWCONTROL_RTS           ((uint32_t)0x2<<USART_CTLR_CTSEN_POS)
#define USART_HARDWAREFLOWCONTROL_BOTH          ((uint32_t)0x3<<USART_CTLR_CTSEN_POS)
#define CHECK_USART_HARDWARE_FLOW_CONTROL(CTL)  (((CTL) == USART_HARDWAREFLOWCONTROL_NONE) \
                                              || ((CTL) == USART_HARDWAREFLOWCONTROL_CTS) \
                                              || ((CTL) == USART_HARDWAREFLOWCONTROL_RTS) \
                                              || ((CTL) == USART_HARDWAREFLOWCONTROL_BOTH))


/**
  * 自动波特率检测使能位
  *   */
#define USART_ABRDEN_OFF                        ((uint32_t)0<<USART_CTLR_ABRDEN_POS)
#define USART_ABRDEN_ON                         ((uint32_t)1<<USART_CTLR_ABRDEN_POS)
#define CHECK_USART_ABRDEN(SELECT)              (((SELECT) == USART_ABRDEN_OFF) \
                                              || ((SELECT) == USART_ABRDEN_ON))

/**
  * USART时钟源选择
  */
#define USART_SLAVE_CLOCKSOURCE_EXTER           ((uint32_t)0<<USART_CTLR_CSRS_POS)
#define USART_MASTER_CLOCKSOURCE_INTER          ((uint32_t)1<<USART_CTLR_CSRS_POS)
#define CHECK_USART_CLOCKSOURCE(SOURCE)         (((SOURCE) == USART_SLAVE_CLOCKSOURCE_EXTER) \
                                              || ((SOURCE) == USART_MASTER_CLOCKSOURCE_INTER))

/**
  * USART时钟配置
  */
#define USART_CLK_SCLK                          ((uint32_t)0<<USART_CTLR_BRCKS0_POS)
#define USART_CLK_HFCLK                         ((uint32_t)1<<USART_CTLR_BRCKS0_POS)
#define USART_CLK_LFCLK                         ((uint32_t)2<<USART_CTLR_BRCKS0_POS)
#define USART_CLK_INTLF                         ((uint32_t)3<<USART_CTLR_BRCKS0_POS)
#define CHECK_USART_CLK(CLK)                    (((CLK) == USART_CLK_SCLK) \
                                              || ((CLK) == USART_CLK_HFCLK) \
                                              || ((CLK) == USART_CLK_LFCLK)\
									          || ((CLK) == USART_CLK_INTLF))
/**
  * USART时钟极性
  */
#define USART_CPOL_LOW                          ((uint32_t)0<<USART_CTLR_SCKPS_POS)
#define USART_CPOL_HIGH                         ((uint32_t)1<<USART_CTLR_SCKPS_POS)
#define CHECK_USART_CPOL(CPOL)                  (((CPOL) == USART_CPOL_LOW) \
                                              || ((CPOL) == USART_CPOL_HIGH))

/**
  * USART发送次序选择
  */
#define USART_TRANSMIT_LSB                      ((uint32_t)0<<USART_CTLR_TCONV_POS)
#define USART_TRANSMIT_MSB                      ((uint32_t)1<<USART_CTLR_TCONV_POS)
#define CHECK_USART_TRANSMIT_DIR(DIR)           (((DIR) == USART_TRANSMIT_LSB) \
                                              || ((DIR) == USART_TRANSMIT_MSB))

/**
  * USART接收次序选择
  */
#define USART_RECEIVE_LSB                       ((uint32_t)0<<USART_CTLR_RCONV_POS)
#define USART_RECEIVE_MSB                       ((uint32_t)1<<USART_CTLR_RCONV_POS)
#define CHECK_USART_RECEIVE_DIR(DIR)            (((DIR) == USART_RECEIVE_LSB) \
                                              || ((DIR) == USART_RECEIVE_MSB))

/**
  * USART小数波特率分子、分母检验
  */
#define CHECK_USART_BAUDRATE_DECIMAL(DIV)       (((uint32_t)(DIV) >> 4) == 0)

/**
  * USART中断定义
  */
#define USART_IT_OVFEIE                         ((uint32_t)1<<USART_IER_OVFEIE_POS)
#define USART_IT_PAREIE                         ((uint32_t)1<<USART_IER_PAREIE_POS)
#define USART_IT_FREIE                          ((uint32_t)1<<USART_IER_FREIE_POS)
#define USART_IT_BRIE                           ((uint32_t)1<<USART_IER_BRIE_POS)
#define USART_IT_ABTOIE                         ((uint32_t)1<<USART_IER_ABTOIE_POS)
#define USART_IT_WUIE                           ((uint32_t)1<<USART_IER_WUIE_POS)
#define USART_IT_TEIE                           ((uint32_t)1<<USART_IER_TEIE_POS)
#define USART_IT_REIE                           ((uint32_t)1<<USART_IER_REIE_POS)
#define USART_IT_CTSIE                          ((uint32_t)1<<USART_IER_CTSIE_POS)
#define USART_IT_RDRIE                          ((uint32_t)1<<USART_IER_RDRIE_POS)
#define USART_IT_TFEIE                          ((uint32_t)1<<USART_IER_TFEIE_POS)
#define USART_IT_TXEIE                          ((uint32_t)1<<USART_IER_TXEIE_POS)
#define USART_IT_URCDE                          ((uint32_t)1<<USART_IER_URCDE_POS)
#define USART_IT_IDLEIE                 	    ((uint32_t)1<<USART_IER_IDLEIE_POS)
#define USART_IT_UTXDE                          ((uint32_t)1<<USART_IER_UTXDE_POS)
#define USART_IT_UADMIE                 		((uint32_t)1<<USART_IER_UADMIE_POS)
#define CHECK_USART_CONFIG_IT(IT)               (((IT) == USART_IT_OVFEIE) \
                                              || ((IT) == USART_IT_PAREIE) \
                                              || ((IT) == USART_IT_FREIE) \
                                              || ((IT) == USART_IT_BRIE) \
                                              || ((IT) == USART_IT_ABTOIE) \
                                              || ((IT) == USART_IT_WUIE) \
                                              || ((IT) == USART_IT_TEIE) \
                                              || ((IT) == USART_IT_REIE) \
                                              || ((IT) == USART_IT_CTSIE) \
                                              || ((IT) == USART_IT_RDRIE) \
                                              || ((IT) == USART_IT_TFEIE) \
                                              || ((IT) == USART_IT_TXEIE) \
                                              || ((IT) == USART_IT_URCDE) \
                                              || ((IT) == USART_IT_IDLEIE) \
                                              || ((IT) == USART_IT_UTXDE) \
                                              || ((IT) == USART_IT_UADMIE))
#define CHECK_USART_GET_IT(IT)                  (((IT) == USART_IT_OVFEIE) \
                                              || ((IT) == USART_IT_PAREIE) \
                                              || ((IT) == USART_IT_FREIE) \
                                              || ((IT) == USART_IT_BRIE) \
                                              || ((IT) == USART_IT_ABTOIE) \
                                              || ((IT) == USART_IT_WUIE) \
                                              || ((IT) == USART_IT_TEIE) \
                                              || ((IT) == USART_IT_REIE) \
                                              || ((IT) == USART_IT_CTSIE) \
                                              || ((IT) == USART_IT_RDRIE) \
                                              || ((IT) == USART_IT_TFEIE) \
                                              || ((IT) == USART_IT_TXEIE) \
                                              || ((IT) == USART_IT_URCDE) \
                                              || ((IT) == USART_IT_IDLEIE) \
                                              || ((IT) == USART_IT_UTXDE) \
                                              || ((IT) == USART_IT_UADMIE))

/**
  * USART标志位定义
  */
#define USART_FLAG_OVFEIF                       ((uint32_t)1<<USART_STR_OVFEIF_POS)
#define USART_FLAG_PAREIF                       ((uint32_t)1<<USART_STR_PAREIF_POS)
#define USART_FLAG_FREIF                        ((uint32_t)1<<USART_STR_FREIF_POS)
#define USART_FLAG_BRIF                         ((uint32_t)1<<USART_STR_BRIF_POS)
#define USART_FLAG_ABTOIF                       ((uint32_t)1<<USART_STR_ABTOIF_POS)
#define USART_FLAG_WUIF                         ((uint32_t)1<<USART_STR_WUIF_POS)
#define USART_FLAG_TEIF                         ((uint32_t)1<<USART_STR_TEIF_POS)
#define USART_FLAG_REIF                         ((uint32_t)1<<USART_STR_REIF_POS)
#define USART_FLAG_CTSIF                        ((uint32_t)1<<USART_STR_CTSIF_POS)
#define USART_FLAG_RDRIF                        ((uint32_t)1<<USART_STR_RDRIF_POS)
#define USART_FLAG_TFEIF                        ((uint32_t)1<<USART_STR_TFEIF_POS)
#define USART_FLAG_TXEIF                        ((uint32_t)1<<USART_STR_TXEIF_POS)
#define USART_FLAG_UADMIF               	   	((uint32_t)1<<USART_STR_UADMIF_POS)
#define USART_FLAG_IDLFIF             		    ((uint32_t)1<<USART_STR_IDLFIF_POS)
#define CHECK_USART_GET_FLAG(FLAG)              (((FLAG) == USART_FLAG_OVFEIF) \
                                              || ((FLAG) == USART_FLAG_PAREIF) \
                                              || ((FLAG) == USART_FLAG_FREIF) \
                                              || ((FLAG) == USART_FLAG_BRIF) \
                                              || ((FLAG) == USART_FLAG_ABTOIF) \
                                              || ((FLAG) == USART_FLAG_WUIF) \
                                              || ((FLAG) == USART_FLAG_TEIF) \
                                              || ((FLAG) == USART_FLAG_REIF) \
                                              || ((FLAG) == USART_FLAG_CTSIF) \
                                              || ((FLAG) == USART_FLAG_RDRIF) \
                                              || ((FLAG) == USART_FLAG_TFEIF) \
                                              || ((FLAG) == USART_FLAG_TXEIF) \
                                              || ((FLAG) == USART_FLAG_UADMIF) \
                                              || ((FLAG) == USART_FLAG_IDLFIF))


/* USART模块(USART)初始化函数定义**************************************/
void USART_Reset (USART_SFRmap* USARTx);
void USART_Configuration (USART_SFRmap* USARTx,
                    USART_InitTypeDef* usartInitStruct);
void USART_U7816R_Configuration (USART_SFRmap* USARTx,
                    U7816R_InitTypeDef* usartInitStruct);
void USART_Struct_Init(USART_InitTypeDef* usartInitStruct);
void USART_U7816R_Struct_Init(U7816R_InitTypeDef* usartInitStruct);
/* USART模块(USART)功能配置函数定义************************************/
void USART_Cmd (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_BaudRate_Clock_Config(USART_SFRmap* USARTx, uint32_t CLK);
void USART_HalfDuplex_ClockPolarity_Config(USART_SFRmap* USARTx,
                    uint32_t NewState);
void USART_Transmit_Order_Config (USART_SFRmap* USARTx, uint32_t NewState);
void USART_Receive_Order_Config (USART_SFRmap* USARTx, uint32_t NewState);
void USART_Infrare_Detector_Voltage_Config (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_WeakUP_Enable(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Clock_Source_Config(USART_SFRmap* USARTx, uint32_t ClockSource);
void USART_Address_Detection_Enable(USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Auto_BaudRate_Detection_Enable(USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Send_Blank_Enable(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_SYN_Choice_Config(USART_SFRmap* USARTx, uint32_t NewMode);
void USART_Transmit_Data_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Receive_Data_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_STOP_Word_Config(USART_SFRmap* USARTx, uint32_t NewLength);
void USART_Transmit_9Word_Select_Config (USART_SFRmap* USARTx,
                    uint32_t NewState);
void USART_Parity_Select_Config(USART_SFRmap* USARTx, uint32_t NewState);
void USART_9Data_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_CTS_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_RTS_Enable(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Infrare_Detector_Enable(USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Singlet_Line_Mode_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_BaudRate_Integer_Config(USART_SFRmap* USARTx, uint16_t DIV);
void USART_BaudRate_Decimal1_Config(USART_SFRmap* USARTx, uint32_t DIV);
void USART_BaudRate_Decimal2_Config(USART_SFRmap* USARTx, uint32_t DIV);
void USART_SendData(USART_SFRmap* USARTx, uint8_t Data);
void USART_TransmitData(USART_SFRmap* USARTx, uint8_t Data);
uint32_t USART_ReceiveData(USART_SFRmap* USARTx);
void USART_Address_Match_Config(USART_SFRmap* USARTx, uint8_t DIV);
void USART_Send_Idle_Frame_Enable(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Receive_Idle_Frame_Config(USART_SFRmap* USARTx, FunctionalState NewState);

/* USART模块(USART)7816初始化及配置函数定义****************************/
void USART_7816_Cmd(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_7816_CLKOUT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_7816_Error_Signal_Config (USART_SFRmap* USARTx,
                    uint32_t ERRORSIGNAL);
void USART_Passageway_Select_Config(USART_SFRmap* USARTx, uint32_t NewState);
void USART_BGT_Config(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Transmit_Repeat_Enable(USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Receive_Repeat_Enable(USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Transmit_Repeat_Times_Config(USART_SFRmap* USARTx, uint32_t SELECT);
void USART_Receive_Repeat_Times_Config(USART_SFRmap* USARTx, uint32_t SELECT);
void USART_7816_CLKDIV_Config (USART_SFRmap* USARTx, uint8_t DIV);
void USART_7816_EGT_Config(USART_SFRmap* USARTx, uint8_t EGT);
void USART_7816_Resend_Mode_Select(USART_SFRmap* USARTx, FunctionalState NewState);

/* USART模块(USART)中断管理函数定义************************************/
void USART_Receive_Overflow_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Parity_ERROR_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Frame_ERROE_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Blank_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Auto_BaudRate_TimeOver_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_WeakUP_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Transmit_ERROR_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Receive_ERROR_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_CTS_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_RDR_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_TFE_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_TXE_INT_Enable (USART_SFRmap* USARTx, FunctionalState NewState);
void USART_Receive_DMA_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_Transmit_DMA_INT_Enable (USART_SFRmap* USARTx,
                    FunctionalState NewState);
void USART_IDLE_INT_Enable(USART_SFRmap* USARTx, FunctionalState NewState);
void USART_UADM_INT_Enable(USART_SFRmap* USARTx, FunctionalState NewState);
FlagStatus USART_Get_Receive_Overflow_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Parity_ERROR_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Frame_ERROR_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Blank_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Auto_Baudrate_TimeOver_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_WeakUP_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_7816Transmit_ERROR_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_7816Receive_ERROR_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_CTS_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Receive_BUFR_Ready_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Transmit_BUFR_Empty_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Transmitter_Empty_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Receive_Frame_Idel_Flag(USART_SFRmap* USARTx);
void USART_Clear_Receive_Overflow_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Parity_ERROR_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Frame_ERROR_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Blank_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Auto_BaudRate_TimeOver_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_WeakUP_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Transmit_ERROR_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Receive_ERROR_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_CTS_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_UADM_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_IDLE_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Receive_BUFR_INT_Flag (USART_SFRmap* USARTx);
void USART_Clear_Transmit_BUFR_INT_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_WUEN_Flag (USART_SFRmap* USARTx);
FlagStatus USART_Get_Auto_BaudRate_Detection_Flag(USART_SFRmap* USARTx);
void USART_RESHD_Enable (USART_SFRmap* USARTx, FunctionalState NewState);

#endif /* _KF32A_BASIC_USART_H */
