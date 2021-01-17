/**
  ******************************************************************************
  * 文件名  kf32a_basic_dma.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描  述  该文件提供了DMA模块(DMA)相关的功能函数，包含：
  *          + DMA模块(DMA)初始化函数
  *          + DMA模块(DMA)功能配置函数
  *          + DMA模块(DMA)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_dma.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* 定时器私有定义 ----------------------------------------------------*/
/* DMA_CTLRy寄存器初始化掩码 */
#define DMA_CTLR_INIT_MASK              (DMA_CTLR_NUMODT \
                                       | DMA_CTLR_CPL \
                                       | DMA_CTLR_PDS \
                                       | DMA_CTLR_MDS \
                                       | DMA_CTLR_PINC \
                                       | DMA_CTLR_MINC \
                                       | DMA_CTLR_LPM \
                                       | DMA_CTLR_DDIR \
                                       | DMA_CTLR_BLKM)

/**
  *   ##### DMA模块(DMA)初始化函数 #####
  */
/**
  * 描述  复位DMA外设，使能外设时钟。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  * 返回  无。
  */
void
DMA_Reset (DMA_SFRmap* DMAx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
#ifdef KF32A_Periph_dma0
    if (DMAx == DMA0_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_DMA0RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_DMA0RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_DMA0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_dma1
    if (DMAx == DMA1_SFR)
    {
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_DMA1RST, TRUE);
        RST_CTL2_Peripheral_Reset_Enable(RST_CTL2_DMA1RST, FALSE);
        PCLK_CTL2_Peripheral_Clock_Enable(PCLK_CTL2_DMA1CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  DMA模块(DMA)初始化配置。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       dmaInitStruct: DMA模块配置信息结构体指针。
  * 返回  无。
  */
void
DMA_Configuration (DMA_SFRmap* DMAx,
                    DMA_InitTypeDef* dmaInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpchannel = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(dmaInitStruct->m_Channel));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL_PRIORITY(dmaInitStruct->m_Priority));
    CHECK_RESTRICTION(CHECK_DMA_DIRECTION(dmaInitStruct->m_Direction));
    CHECK_RESTRICTION(CHECK_DMA_DATA_WIDTH(dmaInitStruct->m_PeripheralDataSize));
    CHECK_RESTRICTION(CHECK_DMA_DATA_WIDTH(dmaInitStruct->m_MemoryDataSize));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dmaInitStruct->m_PeripheralInc));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dmaInitStruct->m_MemoryInc));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(dmaInitStruct->m_LoopMode));
    CHECK_RESTRICTION(CHECK_DMA_TRANSFER_MODE(dmaInitStruct->m_BlockMode));

    tmpchannel = dmaInitStruct->m_Channel;

    /*---------------- 设置DMAx_CTLRy寄存器 ----------------*/
    /* 根据结构体成员m_Number，设置NUMODT位域 */
    /* 根据结构体成员m_Priority，设置CPL位域 */
    /* 根据结构体成员m_PeripheralDataSize，设置PDS位域 */
    /* 根据结构体成员m_MemoryDataSize，设置MDS位域 */
    /* 根据结构体成员m_PeripheralInc，设置PINC位域 */
    /* 根据结构体成员m_MemoryInc，设置MINC位域 */
    /* 根据结构体成员m_LoopMode，设置LPM位域 */
    /* 根据结构体成员m_Direction，设置DDIR位域 */
    /* 根据结构体成员m_BlockMode，设置BLKM位域 */
    tmpreg = ((uint32_t)dmaInitStruct->m_Number << DMA_CTLR_NUMODT0_POS) \
           | ((uint32_t)dmaInitStruct->m_Priority) \
           | ((uint32_t)dmaInitStruct->m_PeripheralDataSize << DMA_CTLR_PDS0_POS) \
           | ((uint32_t)dmaInitStruct->m_MemoryDataSize << DMA_CTLR_MDS0_POS) \
           | (dmaInitStruct->m_PeripheralInc << DMA_CTLR_PINC_POS) \
           | (dmaInitStruct->m_MemoryInc << DMA_CTLR_MINC_POS) \
           | (dmaInitStruct->m_LoopMode << DMA_CTLR_LPM_POS) \
           | ((uint32_t)dmaInitStruct->m_Direction) \
           | (dmaInitStruct->m_BlockMode);
    DMAx->CTLR[tmpchannel] = SFR_Config (DMAx->CTLR[tmpchannel],
                          ~DMA_CTLR_INIT_MASK,
                          tmpreg);

    /*--------- 设置DMAx_PADDRy外设地址寄存器 ---------*/
    DMAx->PADDR[tmpchannel] = dmaInitStruct->m_PeriphAddr;

    /*--------- 设置DMAx_MADDRy外设地址寄存器 ---------*/
    DMAx->MADDR[tmpchannel] = dmaInitStruct->m_MemoryAddr;
}

/**
  * 描述  初始化DMA配置信息结构体。
  * 输入  dmaInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
DMA_Struct_Init (DMA_InitTypeDef* dmaInitStruct)
{
    /* 初始化 DMA通道选择 */
    dmaInitStruct->m_Channel = DMA_CHANNEL_1;
    /* 初始化 DMA通道优先级 */
    dmaInitStruct->m_Priority = DMA_CHANNEL_LOWER;
    /* 初始化 DMA传输方向 */
    dmaInitStruct->m_Direction = DMA_PERIPHERAL_TO_MEMORY;
    /* 初始化 外设数据位宽 */
    dmaInitStruct->m_PeripheralDataSize = DMA_DATA_WIDTH_8_BITS;
    /* 初始化 存储器数据位宽 */
    dmaInitStruct->m_MemoryDataSize = DMA_DATA_WIDTH_8_BITS;
    /* 初始化 传输数据个数 */
    dmaInitStruct->m_Number = 0;
    /* 初始化 外设地址增量模式使能 */
    dmaInitStruct->m_PeripheralInc = FALSE;
    /* 初始化 存储器地址增量模式使能 */
    dmaInitStruct->m_MemoryInc = FALSE;
    /* 初始化 循环模式使能 */
    dmaInitStruct->m_LoopMode = FALSE;
    /* 初始化 数据块传输模式 */
    dmaInitStruct->m_BlockMode = DMA_TRANSFER_BYTE;
    /* 初始化 外设起始地址 */
    dmaInitStruct->m_PeriphAddr = 0;
    /* 初始化 内存起始地址 */
    dmaInitStruct->m_MemoryAddr = 0;
}
/**
  *   ##### DMA模块(DMA)初始化函数定义结束 #####
  */


/**
  *   ##### DMA模块(DMA)功能配置函数 #####
  */
/**
  * 描述  配置DMA传输的数据量。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Number: 需要传输的数据个数，取值范围为0~65535。
  * 返回  无。
  */
void
DMA_Transfer_Number_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint16_t Number)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    /*---------------- 配置DMA_CTLR寄存器NUMODT位 ----------------*/
    tmpreg = (uint32_t)Number << DMA_CTLR_NUMODT0_POS;
    DMAx->CTLR[Channel] = SFR_Config (DMAx->CTLR[Channel],
                                ~DMA_CTLR_NUMODT,
                                tmpreg);
}

/**
  * 描述  配置存储器到存储器传输使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: 存储器到存储器传输使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Memory_To_Memory_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置DMA_CTLR寄存器M2M位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能存储器到存储器传输 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_M2M_POS);
    }
    else
    {
        /* 未使能存储器到存储器传输 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_M2M_POS);
    }
}

/**
  * 描述  配置DMA通道优先级。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Priority: 通道优先级，取值为：
  *                   DMA_CHANNEL_LOWER: 低优先级
  *                   DMA_CHANNEL_MEDIUM: 中优先级
  *                   DMA_CHANNEL_HIGHER: 高优先级
  *                   DMA_CHANNEL_TOP: 最高优先级
  * 返回  无。
  */
void
DMA_Channel_Priority_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Priority)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL_PRIORITY(Priority));

    /*---------------- 配置DMA_CTLR寄存器CPL位 ----------------*/
    tmpreg = (uint32_t)Priority << DMA_CTLR_CPL0_POS;
    DMAx->CTLR[Channel] = SFR_Config (DMAx->CTLR[Channel],
                                ~DMA_CTLR_CPL,
                                tmpreg);
}

/**
  * 描述  配置DMA外设数据位宽。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Width: 外设数据位宽，取值为：
  *                DMA_DATA_WIDTH_8_BITS: 数据为8位宽
  *                DMA_DATA_WIDTH_16_BITS: 数据为16位宽
  *                DMA_DATA_WIDTH_32_BITS: 数据为32位宽
  * 返回  无。
  */
void
DMA_Peripheral_Data_Width_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Width)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_DATA_WIDTH(Width));

    /*---------------- 配置DMA_CTLR寄存器PDS位 ----------------*/
    tmpreg = Width << DMA_CTLR_PDS0_POS;
    DMAx->CTLR[Channel] = SFR_Config (DMAx->CTLR[Channel],
                                ~DMA_CTLR_PDS,
                                tmpreg);
}

/**
  * 描述  配置DMA存储器数据位宽。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Width: 存储器数据位宽，取值为：
  *                DMA_DATA_WIDTH_8_BITS: 数据为8位宽
  *                DMA_DATA_WIDTH_16_BITS: 数据为16位宽
  *                DMA_DATA_WIDTH_32_BITS: 数据为32位宽
  * 返回  无。
  */
void
DMA_Memory_Data_Width_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Width)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_DATA_WIDTH(Width));

    /*---------------- 配置DMA_CTLR寄存器MDS位 ----------------*/
    tmpreg = Width << DMA_CTLR_MDS0_POS;
    DMAx->CTLR[Channel] = SFR_Config (DMAx->CTLR[Channel],
                                ~DMA_CTLR_MDS,
                                tmpreg);
}

/**
  * 描述  配置外设地址增量模式使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: 外设地址增量模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Peripheral_addr_increase_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置DMA_CTLR寄存器PINC位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能外设地址增量模式 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_PINC_POS);
    }
    else
    {
        /* 未使能外设地址增量模式 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_PINC_POS);
    }
}

/**
  * 描述  配置存储器地址增量模式使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: 存储器地址增量模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Memory_addr_increase_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置DMA_CTLR寄存器MINC位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能存储器地址增量模式 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_MINC_POS);
    }
    else
    {
        /* 未使能存储器地址增量模式 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_MINC_POS);
    }
}

/**
  * 描述  配置循环模式使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: 循环模式使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Loop_Mode_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置DMA_CTLR寄存器LPM位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能循环模式 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_LPM_POS);
    }
    else
    {
        /* 未使能循环模式 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_LPM_POS);
    }
}

/**
  * 描述  配置数据方向控制。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Direction: 数据方向控制状态，取值为：
  *                    DMA_PERIPHERAL_TO_MEMORY: 数据从外设地址读取，存入存储器地址
  *                    DMA_MEMORY_TO_PERIPHERAL: 数据从存储器地址读取，存入外设地址
  * 返回  无。
  */
void
DMA_Transfer_Direction_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Direction)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_DIRECTION(Direction));

    /*---------------- 设置DMA_CTLR寄存器DDIR位 ----------------*/
    if (Direction != DMA_PERIPHERAL_TO_MEMORY)
    {
        /* 数据从存储器地址读取，存入外设地址 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_DDIR_POS);
    }
    else
    {
        /* 数据从外设地址读取，存入存储器地址 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_DDIR_POS);
    }
}

/**
  * 描述  配置数据块传输模式。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       BlockMode: 数据块传输模式，取值为：
  *                    DMA_TRANSFER_BYTE: 数据按照字节传输
  *                    DMA_TRANSFER_BLOCK: 数据按照块传输
  * 返回  无。
  */
void
DMA_Transfer_Mode_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t BlockMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_TRANSFER_MODE(BlockMode));

    /*---------------- 设置DMA_CTLR寄存器BLKM位 ----------------*/
    if (BlockMode != DMA_TRANSFER_BYTE)
    {
        /* 数据按照块传输 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_BLKM_POS);
    }
    else
    {
        /* 数据按照字节传输 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_BLKM_POS);
    }
}

/**
  * 描述  配置DMA通道单次触发模式使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: DMA通道Channel使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Oneshot_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置DMA_CTLR寄存器DMAEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能DMA单次触发 */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_ONESHOT_POS);
    }
    else
    {
        /* 未使能DMA单次触发 */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_ONESHOT_POS);
    }
}
/**
  * 描述  配置DMA通道Channel使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: DMA通道Channel使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Channel_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置DMA_CTLR寄存器DMAEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能DMA通道Channel */
        SFR_SET_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_DMAEN_POS);
    }
    else
    {
        /* 未使能DMA通道Channel */
        SFR_CLR_BIT_ASM(DMAx->CTLR[Channel], DMA_CTLR_DMAEN_POS);
    }
}

/**
  * 描述  配置DMA通道的起始外设地址。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Address: DMA通道的起始外设地址，取值为32位有效数值。
  * 返回  无。
  */
void
DMA_Peripheral_Start_Address_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Address)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    /*---------------- 设置DMA_PADDR寄存器PADDR位 ----------------*/
    DMAx->PADDR[Channel] = Address;
}

/**
  * 描述  配置DMA通道的起始存储器地址。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       Address: DMA通道的起始存储器地址，取值为32位有效数值。
  * 返回  无。
  */
void
DMA_Memory_Start_Address_Config (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t Address)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    /*---------------- 设置DMA_MADDR寄存器MADDR位 ----------------*/
    DMAx->MADDR[Channel] = Address;
}

/**
  * 描述  获取DMA通道的当前外设地址。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  * 返回  DMA通道的当前外设地址，取值为32位有效数值。
  */
uint32_t
DMA_Get_Peripheral_Current_Address (DMA_SFRmap* DMAx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    /*---------------- 获取DMA_CPAR寄存器CPAR位 ----------------*/
    tmpreg = DMAx->CPAR[Channel];
    return tmpreg;
}

/**
  * 描述  获取DMA通道的当前存储器地址。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  * 返回  DMA通道的当前存储器地址，取值为32位有效数值。
  */
uint32_t
DMA_Get_Memory_Current_Address (DMA_SFRmap* DMAx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    /*---------------- 获取DMA_CMAR寄存器CMAR位 ----------------*/
    tmpreg = DMAx->CMAR[Channel];
    return tmpreg;
}

/**
  * 描述  获取DMA通道的当前剩余数据量。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  * 返回  DMA通道的当前剩余数据量，取值为16位有效数值。
  */
uint16_t
DMA_Get_Transfer_Number_Remain (DMA_SFRmap* DMAx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    /*---------------- 获取DMA_NCT寄存器NCT位 ----------------*/
    tmpreg = DMAx->NCT[Channel] & DMA_NCT_NCT;
    tmpreg >>= DMA_NCT_NCT16_POS;

    return (uint16_t)tmpreg;
}
/**
  *   ##### DMA模块(DMA)功能配置函数定义结束 #####
  */


/**
  *   ##### DMA模块(DMA)中断管理函数 #####
  */
/**
  * 描述  获取DMA通道传输中断标志。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       InterruptType: DMA通道中断类型，取值范围为:
  *                        DMA_INT_FINISH_TRANSFER: 完成传输中断
  *                        DMA_INT_HALF_TRANSFER: 半传输中断
  *                        DMA_INT_ERROR_TRANSFER: 错误传输中断
  * 返回  1:传输发生错误，0:传输无错误。
  */
FlagStatus
DMA_Get_INT_Flag (DMA_SFRmap* DMAx, uint32_t Channel, uint32_t InterruptType)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_INT_ONE_TYPE(InterruptType));

    /* 配置中断标志掩码 */
    tmpreg = InterruptType << (Channel * 3);
    /*---------------- 读取DMA_LIFR寄存器中断标志位 ----------------*/
    if (DMAx->LIFR & tmpreg)
    {
        /* 传输发生错误 */
        return SET;
    }
    else
    {
        /* 传输无错误 */
        return RESET;
    }
}

/**
  * 描述  清零DMA通道传输中断标志。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       InterruptType: DMA通道中断类型，取值范围为下面提供的一个或多个组合:
  *                        DMA_INT_FINISH_TRANSFER: 完成传输中断
  *                        DMA_INT_HALF_TRANSFER: 半传输中断
  *                        DMA_INT_ERROR_TRANSFER: 错误传输中断
  * 返回  无。
  */
void
DMA_Clear_INT_Flag (DMA_SFRmap* DMAx,
                    uint32_t Channel, uint32_t InterruptType)
{
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_INT_TYPE(InterruptType));

    /*---------------- 清零DMA_LIFR寄存器中断标志位 ----------------*/
    tmpmask = InterruptType << (Channel * 3);
    DMAx->LIFR &= ~tmpmask;
}

/**
  * 描述  配置DMA通道传输中断使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       InterruptType: DMA通道中断类型，取值范围为下面提供的一个或多个组合:
  *                        DMA_INT_FINISH_TRANSFER: 完成传输中断
  *                        DMA_INT_HALF_TRANSFER: 半传输中断
  *                        DMA_INT_ERROR_TRANSFER: 错误传输中断
  *       NewState: DMA通道错误传输中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Set_INT_Enable (DMA_SFRmap* DMAx, uint32_t Channel,
                    uint32_t InterruptType, FunctionalState NewState)
{
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_DMA_INT_TYPE(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置DMA_LIER寄存器中断使能位 ----------------*/
    tmpmask = InterruptType << (Channel * 3);
    if (NewState != FALSE)
    {
        DMAx->LIER |= tmpmask;
    }
    else
    {
        DMAx->LIER &= ~tmpmask;
    }
}

/**
  * 描述  获取DMA通道错误传输中断标志。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  * 返回  1:传输发生错误，0:传输无错误。
  */
FlagStatus
DMA_Get_Error_Transfer_INT_Flag (DMA_SFRmap* DMAx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    tmpreg = DMA_LIFR_ETIF1 << (Channel * 3);
    /*---------------- 读取DMA_LIFR寄存器ETIFy位 ----------------*/
    if (DMAx->LIFR & tmpreg)
    {
        /* 传输发生错误 */
        return SET;
    }
    else
    {
        /* 传输无错误 */
        return RESET;
    }
}

/**
  * 描述  获取DMA通道半传输中断标志。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  * 返回  1:传输已完成一半，0:传输未完成一半。
  */
FlagStatus
DMA_Get_Half_Transfer_INT_Flag (DMA_SFRmap* DMAx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    tmpreg = DMA_LIFR_HTIF1 << (Channel * 3);
    /*---------------- 读取DMA_LIFR寄存器HTIFy位 ----------------*/
    if (DMAx->LIFR & tmpreg)
    {
        /* 传输已完成一半 */
        return SET;
    }
    else
    {
        /* 传输未完成一半 */
        return RESET;
    }
}

/**
  * 描述  获取DMA通道完成传输中断标志。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  * 返回  1:传输已完成，0:传输未完成。
  */
FlagStatus
DMA_Get_Finish_Transfer_INT_Flag (DMA_SFRmap* DMAx, uint32_t Channel)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));

    tmpreg = DMA_LIFR_FTIF1 << (Channel * 3);
    /*---------------- 读取DMA_LIFR寄存器FTIFy位 ----------------*/
    if (DMAx->LIFR & tmpreg)
    {
        /* 传输已完成 */
        return SET;
    }
    else
    {
        /* 传输未完成 */
        return RESET;
    }
}

/**
  * 描述  配置DMA通道错误传输中断使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: DMA通道错误传输中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Error_Transfer_INT_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置DMA_LIER寄存器ETIEy位 ----------------*/
    tmpmask = DMA_LIER_ETIE1 << (Channel * 3);
    if (NewState != FALSE)
    {
        DMAx->LIER |= tmpmask;
    }
    else
    {
        DMAx->LIER &= ~tmpmask;
    }
}

/**
  * 描述  配置DMA通道半传输中断使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: DMA通道半传输中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Half_Transfer_INT_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置DMA_LIER寄存器HTIEy位 ----------------*/
    tmpmask = DMA_LIER_HTIE1 << (Channel * 3);
    if (NewState != FALSE)
    {
        DMAx->LIER |= tmpmask;
    }
    else
    {
        DMAx->LIER &= ~tmpmask;
    }
}

/**
  * 描述  配置DMA通道完成传输中断使能。
  * 输入  DMAx: 指向DMA内存结构的指针，取值为DMA0_SFR和DMA1_SFR。
  *       Channel: DMA通道选择，取值范围为:
  *                  DMA_CHANNEL_1: 通道1
  *                  DMA_CHANNEL_2: 通道2
  *                  DMA_CHANNEL_3: 通道3
  *                  DMA_CHANNEL_4: 通道4
  *                  DMA_CHANNEL_5: 通道5
  *                  DMA_CHANNEL_6: 通道6
  *                  DMA_CHANNEL_7: 通道7
  *       NewState: DMA通道完成传输中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
DMA_Finish_Transfer_INT_Enable (DMA_SFRmap* DMAx,
                    uint32_t Channel, FunctionalState NewState)
{
    uint32_t tmpmask = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_DMA_ALL_PERIPH(DMAx));
    CHECK_RESTRICTION(CHECK_DMA_CHANNEL(Channel));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置DMA_LIER寄存器FTIEy位 ----------------*/
    tmpmask = DMA_LIER_FTIE1 << (Channel * 3);
    if (NewState != FALSE)
    {
        DMAx->LIER |= tmpmask;
    }
    else
    {
        DMAx->LIER &= ~tmpmask;
    }
}
/**
  *   ##### DMA模块(DMA)中断管理函数定义结束 #####
  */
