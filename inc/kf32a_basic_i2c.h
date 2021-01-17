/**
  ********************************************************************
  * 文件名  kf32a_basic_adc.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描述    该文件用于I2C外设的库函数声明及相关宏定义。
  *
  *********************************************************************
*/

#ifndef _KF32A_BASIC_I2C_H_
#define _KF32A_BASIC_I2C_H_

#include "KF32A_BASIC.h"

/* I2C外设判断 ------------------------------------------------------------*/
//#define CHECK_I2C_ALL_PERIPH(PERIPH)    (((PERIPH) == I2C0_SFR) \
//                                      || ((PERIPH) == I2C1_SFR) \
//                                      || ((PERIPH) == I2C2_SFR) \
//                                      || ((PERIPH) == I2C3_SFR))

#define CHECK_I2C_ALL_PERIPH(PERIPH)    (((PERIPH) == ((I2C_SFRmap *)(uint32_t)0x40000D00)) \
                                      || ((PERIPH) == ((I2C_SFRmap *)(uint32_t)0x40000D80)) \
                                      || ((PERIPH) == ((I2C_SFRmap *)(uint32_t)0x40000E00)) \
                                      || ((PERIPH) == ((I2C_SFRmap *)(uint32_t)0x40002380)))
/**
  * 描述 内部集成电路接口(I2C)配置信息结构体
  */
typedef struct
{
    uint32_t m_Mode;                  /* I2C模式选择配置，
                                         取值为宏“I2C模式”中的一个。 */
    uint32_t m_ClockSource;           /* I2C工作时钟，
                                         取值为宏“I2C工作时钟”中的一个。 */
    uint32_t m_BADR10;                /* I2C地址选择配置，
                                         取值为宏“I2C地址选择”中的一个。 */
    uint32_t m_MasterSlave;           /* SMBus类型选择，
                                         取值为宏“SMBus类型选择”中的一个。 */
    uint16_t m_BaudRateL;             /* SCL低电平占用的时钟周期数，
                                         取值为0~0xFFFF。 */
    uint16_t m_BaudRateH;             /* SCL高电平占用的时钟周期数，
                                         取值为0~0xFFFF。 */
    FunctionalState m_AckEn;          /* I2C应答使能，
                                         取值为TRUE或FALSE。 */
    uint32_t m_AckData;               /* 应答数据位，
                                         取值为宏“I2C应答数据位”中的一个。 */
}I2C_InitTypeDef;


/**
  * I2C应答数据位
  */
#define I2C_ACKDATA_ACK                 ((uint32_t)0<<I2C_CTLR_ACKDT_POS)
#define I2C_ACKDATA_NO_ACK              ((uint32_t)1<<I2C_CTLR_ACKDT_POS)
#define CHECK_I2C_ACKDATA(MODE)         (((MODE) == I2C_ACKDATA_ACK) \
                                      || ((MODE) == I2C_ACKDATA_NO_ACK))

/**
  * I2C模式
  */
#define I2C_MODE_I2C                    ((uint32_t)0<<I2C_CTLR_SMBUS_POS)
#define I2C_MODE_SMBUS                  ((uint32_t)1<<I2C_CTLR_SMBUS_POS)
#define CHECK_I2C_MODE(MODE)            (((MODE) == I2C_MODE_I2C) \
                                      || ((MODE) == I2C_MODE_SMBUS))

/**
  * SMBus类型选择
  */
#define I2C_MODE_SMBUSDEVICE            ((uint32_t)0<<I2C_CTLR_SMBT_POS)
#define I2C_MODE_SMBUSHOST              ((uint32_t)1<<I2C_CTLR_SMBT_POS)
#define CHECK_SMBUS_MODE(MODE)          (((MODE) == I2C_MODE_SMBUSDEVICE) \
                                      || ((MODE) == I2C_MODE_SMBUSHOST))
/**
  * I2C工作时钟
  */
#define I2C_CLK_SCLK                    ((uint32_t)0<<I2C_CTLR_I2CCKS0_POS)
#define I2C_CLK_HFCLK                   ((uint32_t)1<<I2C_CTLR_I2CCKS0_POS)
#define I2C_CLK_LFCLK                   ((uint32_t)2<<I2C_CTLR_I2CCKS0_POS)
#define CHECK_I2C_CLK(CLK)              (((CLK) == I2C_CLK_SCLK) \
                                      || ((CLK) == I2C_CLK_HFCLK) \
                                      || ((CLK) == I2C_CLK_LFCLK))

/**
  * I2C地址选择
  */
#define I2C_BUFRADDRESS_7BIT            ((uint32_t)0<<I2C_CTLR_BADR10_POS)
#define I2C_BUFRADDRESS_10BIT           ((uint32_t)1<<I2C_CTLR_BADR10_POS)
#define CHECK_I2C_BUFR_ADDRESS(ADDRESS) (((ADDRESS) == I2C_BUFRADDRESS_7BIT ) \
                                      || ((ADDRESS) == I2C_BUFRADDRESS_10BIT ))


/**
  * I2C中断定义
  */
#define I2C_IT_SIE                      ((uint32_t)1<<I2C_IER_SIE_POS)
#define I2C_IT_PIE                      ((uint32_t)1<<I2C_IER_PIE_POS)
#define I2C_IT_AFIE                     ((uint32_t)1<<I2C_IER_AFIE_POS)
#define I2C_IT_ARBLIE                   ((uint32_t)1<<I2C_IER_ARBLIE_POS)
#define I2C_IT_SMBAIE                   ((uint32_t)1<<I2C_IER_SMBAIE_POS)
#define I2C_IT_SMBHIE                   ((uint32_t)1<<I2C_IER_SMBHIE_POS)
#define I2C_IT_SMBDIE                   ((uint32_t)1<<I2C_IER_SMBDIE_POS)
#define I2C_IT_ISIE                     ((uint32_t)1<<I2C_IER_ISIE_POS)
#define I2C_IT_IRCDE                    ((uint32_t)1<<I2C_IER_IRCDE_POS)
#define I2C_IT_ITXDE                    ((uint32_t)1<<I2C_IER_ITXDE_POS)
#define CHECK_I2C_CONFIG_IT(IT)         (((IT) == I2C_IT_SIE) \
                                        || ((IT) == I2C_IT_PIE) \
                                        || ((IT) == I2C_IT_AFIE) \
                                        || ((IT) == I2C_IT_ARBLIE) \
                                        || ((IT) == I2C_IT_SMBAIE) \
                                        || ((IT) == I2C_IT_SMBHIE) \
                                        || ((IT) == I2C_IT_SMBDIE) \
                                        || ((IT) == I2C_IT_ISIE) \
                                        || ((IT) == I2C_IT_IRCDE) \
                                        || ((IT) == I2C_IT_ITXDE))
#define CHECK_I2C_GET_IT(IT)            (((IT) == I2C_IT_SIE) \
                                        || ((IT) == I2C_IT_PIE) \
                                        || ((IT) == I2C_IT_AFIE) \
                                        || ((IT) == I2C_IT_ARBLIE) \
                                        || ((IT) == I2C_IT_SMBAIE) \
                                        || ((IT) == I2C_IT_SMBHIE) \
                                        || ((IT) == I2C_IT_SMBDIE) \
                                        || ((IT) == I2C_IT_ISIE) \
                                        || ((IT) == I2C_IT_IRCDE) \
                                        || ((IT) == I2C_IT_ITXDE))

/**
  * I2C标志位定义
  */
#define I2C_FLAG_SIF                   ((uint32_t)1<<I2C_SR_SIF_POS)
#define I2C_FLAG_PIF                   ((uint32_t)1<<I2C_SR_PIF_POS)
#define I2C_FLAG_ADDR                  ((uint32_t)1<<I2C_SR_ADDR_POS)
#define I2C_FLAG_ADD10                 ((uint32_t)1<<I2C_SR_ADD10_POS)
#define I2C_FLAG_DATA                  ((uint32_t)1<<I2C_SR_DATA_POS)
#define I2C_FLAG_AFIF                  ((uint32_t)1<<I2C_SR_AFIF_POS)
#define I2C_FLAG_ARBLIF                ((uint32_t)1<<I2C_SR_ARBLIF_POS)
#define I2C_FLAG_RNW                   ((uint32_t)1<<I2C_SR_RNW_POS)
#define I2C_FLAG_SMBAIF                ((uint32_t)1<<I2C_SR_SMBAIF_POS)
#define I2C_FLAG_SMBHIF                ((uint32_t)1<<I2C_SR_SMBHIF_POS)
#define I2C_FLAG_SMBDIF                ((uint32_t)1<<I2C_SR_SMBDIF_POS)
#define I2C_FLAG_ISIF                  ((uint32_t)1<<I2C_SR_ISIF_POS)
#define I2C_FLAG_RCBF                  ((uint32_t)1<<I2C_SR_RCBF_POS)
#define I2C_FLAG_TXBE                  ((uint32_t)1<<I2C_SR_TXBE_POS)
#define I2C_FLAG_IRCDF                 ((uint32_t)1<<I2C_SR_IRCDF_POS)
#define I2C_FLAG_ITXDF                 ((uint32_t)1<<I2C_SR_ITXDF_POS)
#define CHECK_I2C_GET_FLAG(FLAG)       (((FLAG) == I2C_FLAG_SIF) \
                                       || ((FLAG) == I2C_FLAG_PIF) \
                                       || ((FLAG) == I2C_FLAG_ADDR) \
                                       || ((FLAG) == I2C_FLAG_ADD10) \
                                       || ((FLAG) == I2C_FLAG_DATA) \
                                       || ((FLAG) == I2C_FLAG_AFIF) \
                                       || ((FLAG) == I2C_FLAG_ARBLIF) \
                                       || ((FLAG) == I2C_FLAG_RNW) \
                                       || ((FLAG) == I2C_FLAG_SMBAIF) \
                                       || ((FLAG) == I2C_FLAG_SMBHIF) \
                                       || ((FLAG) == I2C_FLAG_SMBDIF) \
                                       || ((FLAG) == I2C_FLAG_ISIF) \
                                       || ((FLAG) == I2C_FLAG_RCBF) \
                                       || ((FLAG) == I2C_FLAG_TXBE) \
                                       || ((FLAG) == I2C_FLAG_IRCDF) \
                                       || ((FLAG) == I2C_FLAG_ITXDF))

/**
  * I2C接收数据
  */
#define CHECK_I2C_BUFR(DATA)           (((uint32_t)(DATA) >> 10) == 0)

/**
  * I2C地址寄存器编号
  */
#define CHECK_I2C_ADDR_REGISTER(NUM)   (((NUM) >= 0) && ((NUM) <= 3))

/**
  * I2C地址位
  */
#define CHECK_I2C_ADDR(DATA)           (((uint32_t)(DATA) >> 10) == 0)

/**
  * I2C地址位
  */
#define CHECK_I2C_MSK(DATA)            (((uint32_t)(DATA) >> 10) == 0)


/* 内部集成电路接口(I2C)初始化函数定义*************************************/
void I2C_Reset (I2C_SFRmap* I2Cx);
void I2C_Configuration (I2C_SFRmap* I2Cx,I2C_InitTypeDef* i2cInitStruct);
void I2C_Struct_Init (I2C_InitTypeDef* I2C_InitStruct);
/* 内部集成电路接口(I2C)功能配置函数定义***********************************/
void I2C_Cmd(I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Bufr_Address_Config(I2C_SFRmap* I2Cx,uint32_t NewState);
void I2C_Generate_START(I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Generate_STOP(I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Ack_Config (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Ack_DATA_Config (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Call_Cmd (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Clock_Config (I2C_SFRmap* I2Cx,uint32_t ClkSource);
void I2C_MATCH_ADDRESS_Config (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_SCL_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_NMENA_Enable(I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_SMBUS_Enable(I2C_SFRmap* I2Cx,uint32_t NewState);
void I2C_SMBT_Config(I2C_SFRmap* I2Cx,uint32_t NewState);
void I2C_SMBus_ALERT_Config(I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_SendData (I2C_SFRmap* I2Cx,uint32_t Data);
uint32_t I2C_ReceiveData(I2C_SFRmap* I2Cx);
void I2C_ARP_Enable(I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_ADDR_Config(I2C_SFRmap* I2Cx, uint32_t AddrSelect, uint32_t Data);
void I2C_MSK_Config(I2C_SFRmap* I2Cx, uint32_t AddrSelect, uint32_t DataMask);
void I2C_BRGH_Config (I2C_SFRmap* I2Cx,uint16_t Period);
void I2C_BRGL_Config (I2C_SFRmap* I2Cx,uint16_t Period);
/* 内部集成电路接口(I2C)中断管理函数定义***********************************/
void I2C_Start_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Stop_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Ack_Fail_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Arbitration_Lost_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_SMBus_Alert_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_SMBus_HostHead_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_SMBus_Device_Defaultaddress_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_ISIE_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Receive_DMA_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
void I2C_Transmit_DMA_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState);
FlagStatus I2C_Get_Start_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_Start_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Stop_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_Stop_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Address_Match_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_HighAddress_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Data_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Ack_Fail_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_Ack_Fail_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Arbitration_Lost_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_Arbitration_Lost_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Write_Read_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_SMBus_Alert_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_SMBus_Alert_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_SMBus_Host_Header_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_SMBus_Host_Header_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_SMBus_Device_Default_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_SMBus_Device_Default_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_INTERRUPT_Flag (I2C_SFRmap* I2Cx);
void I2C_Clear_INTERRUPT_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Receive_Buff_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Transmit_Buff_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Receive_DMA_Flag (I2C_SFRmap* I2Cx);
FlagStatus I2C_Get_Transmit_DMA_Flag (I2C_SFRmap* I2Cx);
void I2C_SendData8 (I2C_SFRmap* I2Cx, uint8_t Data);
#endif /* _KF32A_BASIC_I2C_H */
