/**
  ******************************************************************************
  * 文件名  kf32a_basic_usb.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了USB模块(USB)相关功能函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_USB_H_
#define _KF32A_BASIC_USB_H_

#include "KF32A_BASIC.h"

#ifdef KF32A_Periph_usb
/**
  * 描述  USB配置信息结构体
  */
typedef struct
{
    uint32_t m_Speed;                 /* USB速度，
                                         取值宏“USB速度选择”中的一个。 */
    FunctionalState m_Pullup;         /* USB片上上拉使能，
                                         取值为TRUE或FALSE。 */
    uint32_t m_DoubleBufferCfg;       /* USB双缓冲区配置，
                                         取值宏“USB双缓冲区配置”中的一个。 */
    FunctionalState m_DoubleBufferRst;  /* USB双缓冲区复位使能，
                                         取值为TRUE或FALSE。 */
    uint32_t m_EndPointMask;          /* USB端点编号掩码，
                                         取值宏“USB端点编号掩码”中的一个或多个组合。 */
    FunctionalState m_Handshake;      /* USB端点握手使能，
                                         取值为TRUE或FALSE。 */
    FunctionalState m_Bidirectional;  /* 双向端点控制使能，
                                         取值为TRUE或FALSE。 */
    FunctionalState m_Output;         /* USB端点输出使能，
                                         取值为TRUE或FALSE。 */
    FunctionalState m_Input;          /* USB端点输入使能，
                                         取值为TRUE或FALSE。 */
}USB_InitTypeDef;

/**
  * 描述  USB缓冲区信息结构体
  */
typedef struct
{
    uint16_t m_EndPoint;                 /* USB端点编号，
                                            取值宏“USB端点编号”中的一个。 */
    uint16_t m_Direction;                /* USB端点方向，
                                            取值宏“USB端点方向”中的一个。 */
    uint16_t m_BDPointer;                /* USB端点奇偶BD存储区，
                                            取值宏“USB奇偶BD存储区”中的一个。 */
    uint16_t m_BufferAddr;               /* USB RAM缓存地址，
                                            取值为16位有效数值。 */
    uint32_t m_ByteCount;                /* USB RAM字节计数，
                                            取值为10位有效数值。 */
    uint32_t m_Own;                      /* USB 拥有位，
                                            取值宏“USB拥有位”中的一个。 */
    uint32_t m_DataToggleSync;           /* USB数据翻转同步，
                                            取值宏“USB数据翻转同步位”中的一个。 */
    FunctionalState m_DataToggleSyncEn;  /* USB数据翻转同步使能，
                                            取值为TRUE或FALSE。 */
    FunctionalState m_BufferStall;       /* USB缓冲区停止使能，
                                            取值为TRUE或FALSE。 */
}USB_BufferTypeDef;

/**
  * 描述  USB状态信息结构体
  */
typedef struct
{
    uint8_t m_EndPoint;             /* 上一个活动端点，
                                       取值宏“USB端点编号”中的一个。 */
    uint8_t m_BDDirection;          /* 上一个BD方向，
                                       取值宏“USB令牌”中的一个。 */
    uint8_t m_BDPointer;            /* 乒乓BD指针，
                                       取值宏“USB奇偶BD存储区”中的一个。 */
}USB_StateTypeDef;

/**
  * USB缓冲区起始地址
  */
#define USB_BUFFER_START_ADDR           ((uint32_t)0x1FFEFC00)
#define USB_BUFFER_END_ADDR             ((uint32_t)(0x1FFEFE00 - 1))
#define CHECK_USB_BUFFER_ADDR(ADDR)     (((uint32_t)(ADDR) - USB_BUFFER_START_ADDR) \
                                       <= (USB_BUFFER_END_ADDR - USB_BUFFER_START_ADDR))

/**
  * USB速度选择
  */
#define USB_LOW_SPEED                   ((uint32_t)0<<USB_UCON_SPEEDEN_POS)
#define USB_FULL_SPEED                  ((uint32_t)1<<USB_UCON_SPEEDEN_POS)
#define CHECK_USB_SPEED_SELECT(SEL)     (((SEL) == USB_LOW_SPEED) \
                                      || ((SEL) == USB_FULL_SPEED))

/**
  * USB双缓冲区配置
  */
#define USB_DOUBLE_BUFFER_FORBID        ((uint32_t)0<<USB_UCON_DB0_POS)
#define USB_OUT_ENDPOINT_0              ((uint32_t)1<<USB_UCON_DB0_POS)
#define USB_ALL_ENDPOINTS               ((uint32_t)2<<USB_UCON_DB0_POS)
#define USB_ENDPOINT_1_TO_7             ((uint32_t)3<<USB_UCON_DB0_POS)
#define CHECK_USB_DOUBLE_BUFFER(EN)     (((EN) == USB_DOUBLE_BUFFER_FORBID) \
                                      || ((EN) == USB_OUT_ENDPOINT_0) \
                                      || ((EN) == USB_ALL_ENDPOINTS) \
                                      || ((EN) == USB_ENDPOINT_1_TO_7))

/**
  * USB端点编号
  */
#define USB_ENDPOINT_0                  ((uint32_t)0)
#define USB_ENDPOINT_1                  ((uint32_t)1)
#define USB_ENDPOINT_2                  ((uint32_t)2)
#define USB_ENDPOINT_3                  ((uint32_t)3)
#define USB_ENDPOINT_4                  ((uint32_t)4)
#define USB_ENDPOINT_5                  ((uint32_t)5)
#define USB_ENDPOINT_6                  ((uint32_t)6)
#define USB_ENDPOINT_7                  ((uint32_t)7)
#define CHECK_USB_ENDPOINT_NUM(NUM)     (((NUM) == USB_ENDPOINT_0) \
                                      || ((NUM) == USB_ENDPOINT_1) \
                                      || ((NUM) == USB_ENDPOINT_2) \
                                      || ((NUM) == USB_ENDPOINT_3) \
                                      || ((NUM) == USB_ENDPOINT_4) \
                                      || ((NUM) == USB_ENDPOINT_5) \
                                      || ((NUM) == USB_ENDPOINT_6) \
                                      || ((NUM) == USB_ENDPOINT_7))

/**
  * USB端点编号掩码
  */
#define USB_ENDPOINT_MASK_NONE          ((uint32_t)0)
#define USB_ENDPOINT_MASK_0             ((uint32_t)1<<USB_ENDPOINT_0)
#define USB_ENDPOINT_MASK_1             ((uint32_t)1<<USB_ENDPOINT_1)
#define USB_ENDPOINT_MASK_2             ((uint32_t)1<<USB_ENDPOINT_2)
#define USB_ENDPOINT_MASK_3             ((uint32_t)1<<USB_ENDPOINT_3)
#define USB_ENDPOINT_MASK_4             ((uint32_t)1<<USB_ENDPOINT_4)
#define USB_ENDPOINT_MASK_5             ((uint32_t)1<<USB_ENDPOINT_5)
#define USB_ENDPOINT_MASK_6             ((uint32_t)1<<USB_ENDPOINT_6)
#define USB_ENDPOINT_MASK_7             ((uint32_t)1<<USB_ENDPOINT_7)
#define CHECK_USB_ENDPOINT_MASK(NUM)    (((NUM) == USB_ENDPOINT_MASK_NONE) \
                                      || ((NUM) == USB_ENDPOINT_MASK_0) \
                                      || ((NUM) == USB_ENDPOINT_MASK_1) \
                                      || ((NUM) == USB_ENDPOINT_MASK_2) \
                                      || ((NUM) == USB_ENDPOINT_MASK_3) \
                                      || ((NUM) == USB_ENDPOINT_MASK_4) \
                                      || ((NUM) == USB_ENDPOINT_MASK_5) \
                                      || ((NUM) == USB_ENDPOINT_MASK_6) \
                                      || ((NUM) == USB_ENDPOINT_MASK_7))

/**
  * USB端点方向
  */
#define USB_ENDPOINT_OUTPUT             ((uint32_t)0)
#define USB_ENDPOINT_INPUT              ((uint32_t)1)
#define CHECK_USB_ENDPOINT_DIRECT(DIR)  (((DIR) == USB_ENDPOINT_OUTPUT) \
                                      || ((DIR) == USB_ENDPOINT_INPUT))

/**
  * USB令牌
  */
#define USB_TOKEN_OUT_SETUP             ((uint32_t)0)
#define USB_TOKEN_IN                    ((uint32_t)1)
#define CHECK_USB_TOKEN(DIR)            (((DIR) == USB_TOKEN_OUT_SETUP) \
                                      || ((DIR) == USB_TOKEN_IN))

/**
  * USB奇偶BD存储区
  */
#define USB_ODD_BUFFER_DESCRIPOR        ((uint32_t)0)
#define USB_EVEN_BUFFER_DESCRIPOR       ((uint32_t)1)
#define CHECK_USB_ODD_EVEN(SEL)         (((SEL) == USB_ODD_BUFFER_DESCRIPOR) \
                                      || ((SEL) == USB_EVEN_BUFFER_DESCRIPOR))

/**
  * USB地址
  */
#define CHECK_USB_ADDRESS(CFG)          (((uint32_t)(CFG) >> 7) == 0)

/**
  * USB缓冲区数据字节长度
  */
#define CHECK_USB_DATA_LENGTH(CNT)      (((uint32_t)(CNT) >> 10) == 0)

/**
  * USB拥有位
  */
#define USB_KERNEL_OWN_BD               ((uint32_t)0<<USB_BDSTAT_UOWN_POS)
#define USB_SIE_OWN_BD                  ((uint32_t)1<<USB_BDSTAT_UOWN_POS)
#define CHECK_USB_OWN_BD(SEL)           (((SEL) == USB_KERNEL_OWN_BD) \
                                      || ((SEL) == USB_SIE_OWN_BD))

/**
  * USB数据翻转同步位
  */
#define USB_DATA_0_TOGGLE_SYNC          ((uint32_t)0<<USB_BDSTAT_DTS_POS)
#define USB_DATA_1_TOGGLE_SYNC          ((uint32_t)1<<USB_BDSTAT_DTS_POS)
#define CHECK_USB_DATA_TOGGLE_SYNC(SEL) (((SEL) == USB_DATA_0_TOGGLE_SYNC) \
                                      || ((SEL) == USB_DATA_1_TOGGLE_SYNC))

/**
  * USB中断事件
  */
#define USB_INT_ERROR                   (USB_UIR_ERRIF)
#define USB_INT_RESET                   (USB_UIR_RSTIF)
#define USB_INT_TRANSACTION_COMPLETED   (USB_UIR_TRNSIF)
#define USB_INT_BUS_ACTIVE              (USB_UIR_ACTVIF)
#define USB_INT_IDLE                    (USB_UIR_IDLEIF)
#define USB_INT_STALL_HANDSHAKE         (USB_UIR_STLIF)
#define USB_INT_SOF_TAKEN               (USB_UIR_SOFIF)
#define USB_INT_DATE_FIELD_NOT_BYTE     (USB_UIR_DFN8EIF)
#define USB_INT_CRC5_HOST_ERROR         (USB_UIR_CRC5EIF)
#define USB_INT_CRC16_FAILURE           (USB_UIR_CRC16EIF)
#define USB_INT_PID_CHECK_FAILURE       (USB_UIR_PIDEIF)
#define USB_INT_BUS_TURNAROUND_TIMEOUT  (USB_UIR_BTOEIF)
#define USB_INT_BIT_STUFF_ERROR         (USB_UIR_BTSEIF)
#define CHECK_USB_INT_ONE_EVENT(EVENT)  (((EVENT) == USB_INT_ERROR) \
                                      || ((EVENT) == USB_INT_RESET) \
                                      || ((EVENT) == USB_INT_TRANSACTION_COMPLETED) \
                                      || ((EVENT) == USB_INT_BUS_ACTIVE) \
                                      || ((EVENT) == USB_INT_IDLE) \
                                      || ((EVENT) == USB_INT_STALL_HANDSHAKE) \
                                      || ((EVENT) == USB_INT_SOF_TAKEN) \
                                      || ((EVENT) == USB_INT_DATE_FIELD_NOT_BYTE) \
                                      || ((EVENT) == USB_INT_CRC5_HOST_ERROR) \
                                      || ((EVENT) == USB_INT_CRC16_FAILURE) \
                                      || ((EVENT) == USB_INT_PID_CHECK_FAILURE) \
                                      || ((EVENT) == USB_INT_BUS_TURNAROUND_TIMEOUT) \
                                      || ((EVENT) == USB_INT_BIT_STUFF_ERROR))
#define CHECK_USB_INT_EVENT(EVENT)      (((EVENT) & (~(USB_INT_ERROR \
                                                    | USB_INT_RESET \
                                                    | USB_INT_TRANSACTION_COMPLETED \
                                                    | USB_INT_BUS_ACTIVE \
                                                    | USB_INT_IDLE \
                                                    | USB_INT_STALL_HANDSHAKE \
                                                    | USB_INT_SOF_TAKEN \
                                                    | USB_INT_DATE_FIELD_NOT_BYTE \
                                                    | USB_INT_CRC5_HOST_ERROR \
                                                    | USB_INT_CRC16_FAILURE \
                                                    | USB_INT_PID_CHECK_FAILURE \
                                                    | USB_INT_BUS_TURNAROUND_TIMEOUT \
                                                    | USB_INT_BIT_STUFF_ERROR))) == 0)

/**
  * USB Debug输出选择
  */
#define USB_DBG_DATA_OUTPUT_AND_CLOCK   ((uint32_t)0<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RECEIVE_AND_CLOCK       ((uint32_t)1<<USB_UDB_DEBUG0_POS)
#define USB_DBG_DATA_INPUT_AND_CLOCK    ((uint32_t)2<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RAM_DATA                ((uint32_t)3<<USB_UDB_DEBUG0_POS)
#define USB_DBG_USB_STATE_MACHINE       ((uint32_t)4<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RAM_STATE_MACHINE       ((uint32_t)5<<USB_UDB_DEBUG0_POS)
#define USB_DBG_RAM_ADDRESS             ((uint32_t)6<<USB_UDB_DEBUG0_POS)
#define USB_DBG_OPERATIONAL_CONTROL     ((uint32_t)7<<USB_UDB_DEBUG0_POS)
#define CHECK_USB_DBG_CONFIG(CFG)       (((CFG) == USB_DBG_DATA_OUTPUT_AND_CLOCK) \
                                      || ((CFG) == USB_DBG_RECEIVE_AND_CLOCK) \
                                      || ((CFG) == USB_DBG_DATA_INPUT_AND_CLOCK) \
                                      || ((CFG) == USB_DBG_RAM_DATA) \
                                      || ((CFG) == USB_DBG_USB_STATE_MACHINE) \
                                      || ((CFG) == USB_DBG_RAM_STATE_MACHINE) \
                                      || ((CFG) == USB_DBG_RAM_ADDRESS) \
                                      || ((CFG) == USB_DBG_OPERATIONAL_CONTROL))


/* USB模块(USB)初始化函数定义**************************************/
void USB_Reset(void);
void USB_Configuration(USB_InitTypeDef* usbInitStruct);
void USB_Buffer_Configuration(USB_BufferTypeDef* usbInitStruct);
void USB_Struct_Init (USB_InitTypeDef* usbInitStruct);
void USB_Buffer_Struct_Init (USB_BufferTypeDef* usbInitStruct);
/* USB模块(USB)功能配置函数定义************************************/
void USB_Double_Buffer_Pointer_Enable (FunctionalState NewState);
FlagStatus USB_Get_Single_Ended_0_Flag (void);
void USB_Clear_Single_Ended_0_Flag (void);
FlagStatus USB_Get_Package_Transmit_Flag (void);
void USB_Clear_Package_Transmit_Flag (void);
void USB_Suspend_Enable (FunctionalState NewState);
void USB_Resume_Signal_Transmit_Enable (FunctionalState NewState);
void USB_Cmd (FunctionalState NewState);
void USB_Eye_Test_Enable (FunctionalState NewState);
void USB_Speed_Config (uint32_t SpeedSelect);
void USB_On_Chip_Pull_Up_Enable (FunctionalState NewState);
void USB_Double_Buffer_Config (uint32_t DoubleBuffer);
void USB_Get_Arrangement_State (USB_StateTypeDef* usbStateStruct);
void USB_Address_Config (uint32_t Address);
uint32_t USB_Get_Address (void);
uint32_t USB_Get_Frame_Num (void);
void USB_Endpoint_Handshake_Enable (uint32_t EndPoint,
                    FunctionalState NewState);
void USB_Bidirectional_Endpoint_Enable (uint32_t EndPoint,
                    FunctionalState NewState);
void USB_Endpoint_Output_Enable (uint32_t EndPoint, FunctionalState NewState);
void USB_Endpoint_Input_Enable (uint32_t EndPoint, FunctionalState NewState);
FlagStatus USB_Get_Endpoint_STALL_Flag (uint32_t EndPoint);
void USB_Clear_Endpoint_STALL_Flag (uint32_t EndPoint);
void USB_Debug_Signal_Output_Enable (FunctionalState NewState);
void USB_Debug_Output_Config (uint32_t DebugCtl);
/* USB模块(USB)缓存区函数定义**************************************/
uint32_t USB_Get_Endpoint_Double_Buffer_Addr (uint32_t EndPoint,
                    uint32_t Direction, uint32_t BufferDescripor);
void USB_Buffer_Address_Config (uint32_t SfrAddr, uint16_t BufferAddr);
uint32_t USB_Get_Buffer_Address (uint32_t SfrAddr);
void USB_Buffer_Data_Length_Config (uint32_t SfrAddr, uint32_t ByteLength);
uint32_t USB_Get_Buffer_Data_Length (uint32_t SfrAddr);
void USB_Own_Config (uint32_t SfrAddr, uint32_t OwnSelect);
FlagStatus USB_Get_Own_Status (uint32_t SfrAddr);
void USB_Data_Toggle_Sync_Config (uint32_t SfrAddr, uint32_t DataPackage);
FlagStatus USB_Get_Data_Toggle_Sync_Status (uint32_t SfrAddr);
void USB_Data_Toggle_Sync_Enable (uint32_t SfrAddr, FunctionalState NewState);
void USB_Buffer_Stall_Enable (uint32_t SfrAddr, FunctionalState NewState);
uint32_t USB_Get_Last_Receive_PID (uint32_t SfrAddr);
void USB_Write_Buffer_Data (uint32_t WriteAddr,
                    uint32_t WriteDataAddr, uint32_t Length);
/* USB模块(USB)中断管理函数定义************************************/
FlagStatus USB_Get_INT_Flag (uint32_t InterruptType);
void USB_Clear_INT_Flag (uint32_t InterruptType);
void USB_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState);
FlagStatus USB_Get_INT_Enable (uint32_t InterruptType);

#endif //KF32A_Periph_usb
#endif /* _KF32A_BASIC_USB_H */
