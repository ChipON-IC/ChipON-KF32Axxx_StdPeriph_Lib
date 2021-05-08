/**
  ******************************************************************************
  * 文件名  kf32a_basic_dma.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了DMA模块(DMA)相关功能函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_DMA_H_
#define _KF32A_BASIC_DMA_H_

#include "KF32A_BASIC.h"


/**
  * 描述  DMA模块配置信息结构体
  */
typedef struct
{
    uint8_t m_Channel;                /* DMA通道选择，
                                         取值为宏“DMA通道”中的一个。*/
    uint8_t m_Direction;              /* DMA传输方向，
                                         取值为宏“传输方向”中的一个。*/
    uint8_t m_PeripheralDataSize;     /* 外设数据位宽，
                                         取值为宏“传输数据位宽”中的一个。*/
    uint8_t m_MemoryDataSize;         /* 存储器数据位宽，
                                         取值为宏“传输数据位宽”中的一个。*/
    uint16_t m_Priority;               /* DMA通道优先级，
                                         取值为宏“DMA通道优先级”中的一个。*/
    uint16_t m_Number;                /* 传输数据个数，
                                         取值范围为0~65535。*/
    FunctionalState m_PeripheralInc;  /* 外设地址增量模式使能，
                                         取值为TRUE或FALSE。*/
    FunctionalState m_MemoryInc;      /* 存储器地址增量模式使能，
                                         取值为TRUE或FALSE。*/
    FunctionalState m_LoopMode;       /* 循环模式使能，
                                         取值为TRUE或FALSE。*/
    uint32_t m_BlockMode;             /* 数据块传输模式，
                                         取值为宏“数据块传输模式”中的一个。*/
    uint32_t m_PeriphAddr;            /* 外设起始地址，
                                         取值为32位数值。*/
    uint32_t m_MemoryAddr;            /* 内存起始地址，
                                         取值为32位数值。*/
}DMA_InitTypeDef;

/**
  * DMA内存指针
  */
//#define CHECK_DMA_ALL_PERIPH(PERIPH)    (((PERIPH) == DMA0_SFR) \
//                                        || ((PERIPH) == DMA1_SFR))
#define CHECK_DMA_ALL_PERIPH(PERIPH)    (((PERIPH) == ((DMA_SFRmap *)(uint32_t)0x40001600)) \
                                        || ((PERIPH) == ((DMA_SFRmap *)(uint32_t)0x40001700)))

/**
  * DMA通道
  */
#define DMA_CHANNEL_1                   ((uint32_t)0)
#define DMA_CHANNEL_2                   ((uint32_t)1)
#define DMA_CHANNEL_3                   ((uint32_t)2)
#define DMA_CHANNEL_4                   ((uint32_t)3)
#define DMA_CHANNEL_5                   ((uint32_t)4)
#define DMA_CHANNEL_6                   ((uint32_t)5)
#define DMA_CHANNEL_7                   ((uint32_t)6)
#define CHECK_DMA_CHANNEL(CHANNEL)      (((CHANNEL) == DMA_CHANNEL_1) \
                                        || ((CHANNEL) == DMA_CHANNEL_2) \
                                        || ((CHANNEL) == DMA_CHANNEL_3) \
                                        || ((CHANNEL) == DMA_CHANNEL_4) \
                                        || ((CHANNEL) == DMA_CHANNEL_5) \
                                        || ((CHANNEL) == DMA_CHANNEL_6) \
                                        || ((CHANNEL) == DMA_CHANNEL_7))

/**
  * DMA通道优先级
  */
#define DMA_CHANNEL_LOWER               ((uint32_t)0<<DMA_CTLR_CPL0_POS)
#define DMA_CHANNEL_MEDIUM              ((uint32_t)1<<DMA_CTLR_CPL0_POS)
#define DMA_CHANNEL_HIGHER              ((uint32_t)2<<DMA_CTLR_CPL0_POS)
#define DMA_CHANNEL_TOP                 ((uint32_t)3<<DMA_CTLR_CPL0_POS)
#define CHECK_DMA_CHANNEL_PRIORITY(PRI) (((PRI) == DMA_CHANNEL_LOWER) \
                                        || ((PRI) == DMA_CHANNEL_MEDIUM) \
                                        || ((PRI) == DMA_CHANNEL_HIGHER) \
                                        || ((PRI) == DMA_CHANNEL_TOP))

/**
  * 传输数据位宽
  */
#define DMA_DATA_WIDTH_8_BITS           ((uint32_t)0)
#define DMA_DATA_WIDTH_16_BITS          ((uint32_t)1)
#define DMA_DATA_WIDTH_32_BITS          ((uint32_t)2)
#define CHECK_DMA_DATA_WIDTH(WIDTH)     (((WIDTH) == DMA_DATA_WIDTH_8_BITS) \
                                        || ((WIDTH) == DMA_DATA_WIDTH_16_BITS) \
                                        || ((WIDTH) == DMA_DATA_WIDTH_32_BITS))

/**
  * 传输方向
  */
#define DMA_PERIPHERAL_TO_MEMORY        ((uint32_t)0<<DMA_CTLR_DDIR_POS)
#define DMA_MEMORY_TO_PERIPHERAL        ((uint32_t)1<<DMA_CTLR_DDIR_POS)
#define CHECK_DMA_DIRECTION(DIR)        (((DIR) == DMA_PERIPHERAL_TO_MEMORY) \
                                        || ((DIR) == DMA_MEMORY_TO_PERIPHERAL))

/**
  * 数据块传输模式
  */
#define DMA_TRANSFER_BYTE               ((uint32_t)0<<DMA_CTLR_BLKM_POS)
#define DMA_TRANSFER_BLOCK              ((uint32_t)1<<DMA_CTLR_BLKM_POS)
#define CHECK_DMA_TRANSFER_MODE(MODE)   (((MODE) == DMA_TRANSFER_BYTE) \
                                      || ((MODE) == DMA_TRANSFER_BLOCK))

/**
  * DMA中断类型
  */
#define DMA_INT_FINISH_TRANSFER         ((uint32_t)1)
#define DMA_INT_HALF_TRANSFER           ((uint32_t)2)
#define DMA_INT_ERROR_TRANSFER          ((uint32_t)4)
#define CHECK_DMA_INT_ONE_TYPE(TYPE)    (((TYPE) == DMA_INT_FINISH_TRANSFER) \
                                        || ((TYPE) == DMA_INT_HALF_TRANSFER) \
                                        || ((TYPE) == DMA_INT_ERROR_TRANSFER))
#define CHECK_DMA_INT_TYPE(TYPE)        (((TYPE) & (~(DMA_INT_FINISH_TRANSFER \
                                                    | DMA_INT_HALF_TRANSFER \
                                                    | DMA_INT_ERROR_TRANSFER))) == 0)


/* DMA模块(DMA)初始化函数定义***********************************************/
void DMA_Reset (DMA_SFRmap* DMAx);
void DMA_Configuration (DMA_SFRmap* DMAx, DMA_InitTypeDef* dmaInitStruct);
void DMA_Struct_Init (DMA_InitTypeDef* dmaInitStruct);
/* DMA模块(DMA)功能配置函数定义*********************************************/
void DMA_Transfer_Number_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint16_t Number);
void DMA_Memory_To_Memory_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Channel_Priority_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Priority);
void DMA_Peripheral_Data_Width_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Width);
void DMA_Memory_Data_Width_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Width);
void DMA_Peripheral_addr_increase_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Memory_addr_increase_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Loop_Mode_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Transfer_Direction_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Direction);
void DMA_Transfer_Mode_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t BlockMode);
void DMA_Oneshot_Enable (DMA_SFRmap* DMAx,
                                        uint32_t Channel, FunctionalState NewState);
void DMA_Channel_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Peripheral_Start_Address_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Address);
void DMA_Memory_Start_Address_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Address);
uint32_t DMA_Get_Peripheral_Current_Address (DMA_SFRmap* DMAx,
                    uint32_t Channel);
uint32_t DMA_Get_Memory_Current_Address (DMA_SFRmap* DMAx, uint32_t Channel);
uint16_t DMA_Get_Transfer_Number_Remain (DMA_SFRmap* DMAx, uint32_t Channel);
/* DMA模块(DMA)中断管理函数定义*********************************************/
FlagStatus DMA_Get_INT_Flag (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t InterruptType);
void DMA_Clear_INT_Flag (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t InterruptType);
void DMA_Set_INT_Enable (DMA_SFRmap* DMAx, uint32_t Channel,
                    uint32_t InterruptType, FunctionalState NewState);
FlagStatus DMA_Get_Error_Transfer_INT_Flag (DMA_SFRmap* DMAx,
                    uint32_t Channel);
FlagStatus DMA_Get_Half_Transfer_INT_Flag (DMA_SFRmap* DMAx, uint32_t Channel);
FlagStatus DMA_Get_Finish_Transfer_INT_Flag (DMA_SFRmap* DMAx,
                    uint32_t Channel);
void DMA_Error_Transfer_INT_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Half_Transfer_INT_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);
void DMA_Finish_Transfer_INT_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState);

#endif /* _KF32A_BASIC_DMA_H */
