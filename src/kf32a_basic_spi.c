/**
  ******************************************************************************
  * 文件名  kf32a_basic_btim.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了串行外设接口(SPI)外设功能函数，包含：
  *          + 串行外设接口(SPI)初始化函数
  *          + 串行外设接口(SPI)配置函数
  *          + 串行外设接口(SPI)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_spi.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* SPI定时器私有定义 ----------------------------------------------------*/
/* SPIx_CTLR寄存器SPI初始化掩码 */
#define SPI_CTLR_SPI_INIT_MASK          (SPI_CTLR_SPIMS \
                                       | SPI_CTLR_SPICKS \
                                       | SPI_CTLR_SPIDS \
                                       | SPI_CTLR_SPICKP \
                                       | SPI_CTLR_SPICKE \
                                       | SPI_CTLR_SPIBS )

/* SPIx_CTLR寄存器I2S初始化掩码 */
#define SPI_CTLR_I2S_INIT_MASK          (SPI_CTLR_I2SMC \
                                       | SPI_CTLR_I2SSS \
                                       | SPI_CTLR_PCMS \
                                       | SPI_CTLR_I2SCKS \
                                       | SPI_CTLR_PCMCKP )


/**
  *   ##### 串行外设接口(SPI)初始化函数 #####
  */
/**
  * 描述  SPI外设复位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  无。
  */
void
SPI_Reset(SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
#ifdef KF32A_Periph_spi0
    if (SPIx == SPI0_SFR)
    {
       RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_SPI0RST, TRUE);
       RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_SPI0RST, FALSE);
       PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_SPI0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_spi1
    if (SPIx == SPI1_SFR)
    {
       RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_SPI1RST, TRUE);
       RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_SPI1RST, FALSE);
       PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_SPI1CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_spi2
    if (SPIx == SPI2_SFR)
    {
       RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_SPI2RST, TRUE);
       RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_SPI2RST, FALSE);
       PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_SPI2CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_spi3
    if (SPIx == SPI3_SFR)
    {
       RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_SPI3RST, TRUE);
       RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_SPI3RST, FALSE);
       PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_SPI3CLKEN, TRUE);
    }
#endif
}

/**
  * 描述  SPI模式初始化配置。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       SPIInitStruct: SPI协议信息结构体指针。
  * 返回  无。
  */
void
SPI_Configuration(SPI_SFRmap* SPIx, SPI_InitTypeDef* spiInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_MODE(spiInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_SPI_CLK(spiInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_SPI_FIRST_BIT(spiInitStruct->m_FirstBit));
    CHECK_RESTRICTION(CHECK_SPI_CKP(spiInitStruct->m_CKP));
    CHECK_RESTRICTION(CHECK_SPI_CKE(spiInitStruct->m_CKE));
    CHECK_RESTRICTION(CHECK_SPI_DATASIZE(spiInitStruct->m_DataSize));

    /*----------------- 配置SPI_CTLR寄存器  -----------------*/
    /* 根据结构体成员m_Mode，设置SPIMS位域 */
    /* 根据结构体成员m_Clock，设置SPICKS位域 */
    /* 根据结构体成员m_FirstBit，设置SPIDS位域 */
    /* 根据结构体成员m_CKP，设置SPICKP位域 */
    /* 根据结构体成员m_CKE，设置SPICKE位域 */
    /* 根据结构体成员m_DataSize，设置SPIBS位域 */
    tmpreg = ((spiInitStruct->m_Mode)      \
            | (spiInitStruct->m_Clock)     \
            | (spiInitStruct->m_FirstBit)  \
            | (spiInitStruct->m_CKP)       \
            | (spiInitStruct->m_CKE)       \
            | (spiInitStruct->m_DataSize));
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_SPI_INIT_MASK, tmpreg);

    /*----------------- 配置SPI_BRGR寄存器  -----------------*/
    /* 根据结构体成员m_BaudRate，设置SPIBRG位域 */
    tmpreg = (uint32_t)spiInitStruct->m_BaudRate << SPI_BRGR_SPIBRG0_POS;
    SPIx->BRGR = SFR_Config (SPIx->BRGR, ~SPI_BRGR_SPIBRG, tmpreg);
}

/**
  * 描述  I2S模式初始化配置。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       I2SInitStruct:I2S协议信息结构体指针。
  * 返回  无。
  */
void
I2S_Configuration(SPI_SFRmap* SPIx, I2S_InitTypeDef* i2sInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_I2S_MODE(i2sInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_I2S_STANDARD(i2sInitStruct->m_Standard));
    CHECK_RESTRICTION(CHECK_I2S_PCM(i2sInitStruct->m_PCM));
    CHECK_RESTRICTION(CHECK_I2S_CLK(i2sInitStruct->m_Clock));
    CHECK_RESTRICTION(CHECK_I2S_CKP(i2sInitStruct->m_CKP));

    /*----------------- SPIx_CTLR寄存器配置 -----------------*/
    /* 根据结构体成员m_Mode，设置I2SMC位域 */
    /* 根据结构体成员m_STANDARD，设置I2SSS位域 */
    /* 根据结构体成员m_PCM，设置PCMS位域 */
    /* 根据结构体成员m_CLK，设置CHLEN位域 */
    /* 根据结构体成员m_CPOL，设置PCMCKP位域 */
    tmpreg = ((i2sInitStruct->m_Mode)     \
            | (i2sInitStruct->m_Standard) \
            | (i2sInitStruct->m_PCM)      \
            | (i2sInitStruct->m_Clock)    \
            | (i2sInitStruct->m_CKP));
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_I2S_INIT_MASK , tmpreg);

    /*----------------- 配置SPI_BRGR寄存器  -----------------*/
    /* 根据结构体成员m_Prescaler，设置I2SDIV位域 */
    tmpreg = (uint32_t)i2sInitStruct->m_Prescaler << SPI_BRGR_I2SDIV0_POS;
    SPIx->BRGR = SFR_Config (SPIx->BRGR, ~SPI_BRGR_I2SDIV, tmpreg);
}

/**
  * 描述  初始化SPI协议信息结构体。
  * 输入  SPI_InitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
SPI_Struct_Init(SPI_InitTypeDef* SPI_InitStruct)
{
    /* 初始化 SPI模式选择 */
    SPI_InitStruct->m_Mode = SPI_MODE_SLAVE;
    /* 初始化SPI工作时钟选择 */
    SPI_InitStruct->m_Clock = SPI_CLK_HFCLK;
    /* 初始化 SPI数据方向选择位 */
    SPI_InitStruct->m_FirstBit = SPI_FIRSTBIT_MSB;
    /* 初始化 SPI时钟极性选择 */
    SPI_InitStruct->m_CKP = SPI_CKP_LOW;
    /* 初始化 SPI时钟边沿选择 */
    SPI_InitStruct->m_CKE = SPI_CKE_1EDGE;
    /* 初始化 SPI位模式选择 */
    SPI_InitStruct->m_DataSize = SPI_DATASIZE_8BITS;
    /* 初始化 SPI波特率没设置 */
    SPI_InitStruct->m_BaudRate = 0;
}

/**
  * 描述  初始化I2S协议信息结构体。
  * 输入  I2S_InitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
I2S_Struct_Init(I2S_InitTypeDef* I2S_InitStruct)
{
    /* 初始化 I2S模式选择 */
    I2S_InitStruct->m_Mode = I2S_MODE_SLAVE_TX;
    /* 初始化 I2S标准选择 */
    I2S_InitStruct->m_Standard = I2S_STANDARD_PHILLIPS;
    /* 初始化 PCM帧同步 */
    I2S_InitStruct->m_PCM = I2S_PCM_LONG;
    /* 初始化 I2S时钟分频选择 */
    I2S_InitStruct->m_Clock = I2S_CLK_BAUDRATE;
    /* 初始化 PCM模式时钟极性选择 */
    I2S_InitStruct->m_CKP = I2S_CKP_LOW;
    /* 初始化 I2S预分频 */
    I2S_InitStruct->m_Prescaler = 0;
}
/**
  *   ##### 串行外设接口(SPI)初始化函数定义结束 #####
  */


/**
  *   ##### 串行外设接口(SPI)配置函数 #####
  */
/**
  * 描述  控制SPI使能位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_Cmd (SPI_SFRmap* SPIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*----------------- 配置SPI_CTLR寄存器SPIEN位  -----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI */
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPIEN_POS);
    }
    else
    {
        /* 禁止SPI */
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPIEN_POS);
    }
}

/**
  * 描述  控制I2S选择位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: I2S选择位配置信息，取值为 TRUE 或 FALSE。
  *       TRUE：代表选择I2S工作模式
  *       FALSE:代表选择SPI工作模式
  * 返回  无。
  */
void
I2S_Mode_Select(SPI_SFRmap* SPIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*----------------- 配置SPI_CTLR寄存器I2SMOD位  -----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2S */
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SMOD_POS);
    }
    else
    {
        /* 禁止I2S */
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SMOD_POS);
    }
}

/**
  * 描述  获取SPI_I2S接收数据。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  32位接收数据。
  */
uint32_t
SPI_I2S_ReceiveData (SPI_SFRmap* SPIx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*----------------- 获取SPI_BUFR寄存器  -----------------*/
    tmpreg = SPIx->BUFR;

    return tmpreg;
}

/**
  * 描述  配置SPI_I2S发送32位数据。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  无。
  */
void
SPI_I2S_SendData32(SPI_SFRmap* SPIx, uint32_t Data)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*----------------- 配置SPI_BUFR寄存器  -----------------*/
    SPIx->BUFR = Data;
}
/**
  * 描述  配置SPI_I2S发送8位数据。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  无。
  */
void
SPI_I2S_SendData8(SPI_SFRmap* SPIx, uint8_t Data)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*----------------- 配置SPI_BUFR寄存器  -----------------*/
    SPIx->BUFR = Data;
}
/**
  * 描述  配置SPI波特率寄存器。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       BAUDRATE: 波特率选择，取值为0~65535。
  * 返回  无。
  */
void
SPI_BaudRate_Config (SPI_SFRmap* SPIx, uint16_t BAUDRATE)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 设置SPI_CTLR寄存器I2SSS位 ----------------*/
    tmpreg = (uint32_t)BAUDRATE<< SPI_BRGR_SPIBRG0_POS;
    SPIx->BRGR = SFR_Config (SPIx->BRGR, ~SPI_BRGR_SPIBRG, tmpreg);
}

/**
  * 描述  配置I2S预分频寄存器。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       DIV: I2S预分频选择，取值为0~255。
  * 返回  无。
  */
void
I2S_DIV_Config (SPI_SFRmap* SPIx,uint8_t DIV)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 设置SPI_CTLR寄存器位I2SSS ----------------*/
    tmpreg = (uint32_t)DIV << SPI_BRGR_I2SDIV0_POS;
    SPIx->BRGR = SFR_Config (SPIx->BRGR, ~SPI_BRGR_I2SDIV, tmpreg);
}

/**
  * 描述  配置SPI同步串行端口模式选择位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       MODE: 模式选择，取值范围为：
  *               SPI_MODE_MASTER_CLKDIV4: SPI主控模式，时钟 = 工作时钟/4
  *               SPI_MODE_MASTER_CLKDIV16: SPI主控模式，时钟 = 工作时钟/16
  *               SPI_MODE_MASTER_CLKDIV64: SPI主控模式，时钟 = 工作时钟/64
  *               SPI_MODE_MASTER_T2DIV2: SPI主控模式，时钟 = TIMER2/2
  *               SPI_MODE_SLAVE: SPI从动模式，时钟 = SCK引脚。使能SS引脚控制
  * 返回  无。
  */
void
SPI_MODE_Config (SPI_SFRmap* SPIx,uint32_t MODE)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_MODE(MODE));

    /*---------------- 设置SPI_CTLR寄存器SPIMS位 ----------------*/
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_SPIMS, MODE);
}

/**
  * 描述  配置SPI工作时钟选择寄存器。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       ClockSource: 模式选择，取值范围为：
  *                      SPI_CLK_SCLK  :选用SCLK为SPI工作时钟
  *                      SPI_CLK_HFCLK :选用HFCLK为SPI工作时钟
  *                      SPI_CLK_LFCLK :选用LFCLK为SPI工作时钟
  * 返回  无。
  */
void
SPI_CLK_Config (SPI_SFRmap* SPIx,uint32_t ClockSource)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_CLK(ClockSource));

    /*---------------- 设置SPI_CTLR寄存器SPIMS位 ----------------*/
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_SPICKS, ClockSource);
}

/**
  * 描述  设置SPI数据方向选择位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       DataDirection: SPI数据方向，取值为：
  *                        SPI_FIRSTBIT_MSB:（最高位）在前，
  *                        SPI_FIRSTBIT_LSB:（最低位）在前。
  * 返回  无。
  */
void
SPI_Data_Direction_Config(SPI_SFRmap* SPIx,uint32_t DataDirection)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_FIRST_BIT(DataDirection));

    /*---------------- 设置SPI_CTLR寄存器SPIDS位 ----------------*/
    if (DataDirection != SPI_FIRSTBIT_LSB)
    {
        /* MSB（最高位）在前 */
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPIDS_POS);
    }
    else
    {
        /* LSB（最低位）在前 */
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPIDS_POS);
    }
}

/**
  * 描述  设置SPI时钟极性选择位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       Polarity: SPI时钟极性，取值为：
  *                   SPI_CKP_LOW:空闲状态时，时钟为低电平；
  *                   SPI_CKP_HIGH:空闲状态时，时钟为高电平。
  * 返回  无。
  */
void
SPI_Clock_Polarity_Config (SPI_SFRmap* SPIx,uint32_t Polarity)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_CKP(Polarity));

    /*---------------- 设置SPI_CTLR寄存器SPICKP位 ----------------*/
    if (Polarity != SPI_CKP_LOW)
    {
        /* 空闲状态时，时钟为高电平 */
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPICKP_POS);
    }
    else
    {
        /* 空闲状态时，时钟为低电平 */
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPICKP_POS);
    }
}

/**
  * 描述  设置SPI时钟边沿选择位。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       ClockEdge: SPI时钟边沿，取值为：
  *                    SPI_CKE_2EDGE:在第2个时钟沿（包括上升，下降）发送数据，
  *                    SPI_CKE_1EDGE:在第1个时钟沿（包括上升，下降）发送数据。
  * 返回  无。
  */
void
SPI_Clock_Edge_Config (SPI_SFRmap* SPIx,uint32_t ClockEdge)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_CKE(ClockEdge));

    /*---------------- 设置SPI_CTLR寄存器SPICKE位 ----------------*/
    if (ClockEdge != SPI_CKE_1EDGE)
    {
        /* 在第2个时钟沿（包括上升，下降）发送数据 */
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPICKE_POS);
    }
    else
    {
        /* 在第1个时钟沿（包括上升，下降）发送数据 */
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_SPICKE_POS);
    }
}

/**
  * 描述  配置SPI位模式选择。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       DataSize: 模式选择，取值范围为：
  *                   SPI_DATASIZE_8BITS  :SPI使用8位模式（不支持I2S模式）
  *                   SPI_DATASIZE_16BITS :SPI使用16位模式
  *                   SPI_DATASIZE_32BITS :SPI使用32位模式
  * 返回  无。
  */
void
SPI_BIT_SELECT_Config(SPI_SFRmap* SPIx,uint32_t DataSize)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_SPI_DATASIZE(DataSize));

    /*---------------- 设置SPI_CTLR寄存器SPIBS位 ----------------*/
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_SPIBS, DataSize);
}

/**
  * 描述  配置I2S模式设置。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       Mode: 模式选择，取值范围为：
  *               I2S_MODE_SLAVE_TX   :从设备发送模式
  *               I2S_MODE_SLAVE_RX   :从设备接收模式
  *               I2S_MODE_MASTER_TX  :主设备发送模式
  *               I2S_MODE_MASTER_RX  :主设备接收模式
  * 返回  无。
  */
void
SPI_I2S_MODE_Config (SPI_SFRmap* SPIx,uint32_t Mode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_I2S_MODE(Mode));

    /*---------------- 设置SPI_CTLR寄存器I2SMC位 ----------------*/
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_I2SMC, Mode);
}

/**
  * 描述  配置I2S标准选择。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       Standard: 模式选择，取值范围为：
  *                   I2S_STANDARD_PHILLIPS :I2S飞利浦标准
  *                   I2S_STANDARD_MSB      :低字节对齐标准（右对齐）
  *                   I2S_STANDARD_LSB      :高字节对齐标准（左对齐）
  *                   I2S_STANDARD_PCM      :PCM标准
  * 返回  无。
  */
void
SPI_I2S_STANDARD_Config(SPI_SFRmap* SPIx,uint32_t Standard)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_I2S_STANDARD(Standard));

    /*---------------- 设置SPI_CTLR寄存器I2SSS位 ----------------*/
    SPIx->CTLR = SFR_Config (SPIx->CTLR, ~SPI_CTLR_I2SSS, Standard);
}

/**
  * 描述  设置I2S PCM帧同步。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: PCM帧同步配置，取值为：
  *                   I2S_PCM_LONG: 长帧同步，
  *                   I2S_PCM_SHORT:短帧同步。
  * 返回  无。
  */
void
SPI_PCM_Config (SPI_SFRmap* SPIx,uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_I2S_PCM(NewState));

    /*---------------- 设置SPI_CTLR寄存器PCMS位 ----------------*/
    if (NewState != I2S_PCM_SHORT)
    {
        /* 长帧同步*/
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_PCMS_POS);
    }
    else
    {
        /*短帧同步*/
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_PCMS_POS);
    }
}

/**
  * 描述  设置I2S 模式的时钟分频选择。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: 时钟分频配置，取值为：
  *                   I2S_CLK_BAUDRATE:正常波特率，
  *                   I2S_CLK_BAUDRATEDIV2:正常波特率/2。
  * 返回  无。
  */
void
SPI_CHLEN_Config (SPI_SFRmap* SPIx,uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_I2S_CLK(NewState));

    /*---------------- 设置SPI_CTLR寄存器CHLEN位 ----------------*/
    if (NewState == I2S_CLK_BAUDRATEDIV8)
    {
        /* 正常波特率/8*/
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SCKS0_POS);
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SCKS1_POS);
    }
    else if(NewState == I2S_CLK_BAUDRATEDIV4)
    {
        /*正常波特率/4*/
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SCKS0_POS);
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SCKS1_POS);
    }
    else
    {
    	/* 正常波特率*/
    	SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SCKS0_POS);
    	SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_I2SCKS1_POS);
    }
}

/**
  * 描述  设置PCM模式时钟极性选择。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: PCM模式时钟极性，取值为：
  *                   I2S_CKP_HIGH: PCM数据在时钟下降沿发送，
  *                   I2S_CKP_LOW:  PCM数据在时钟上升沿发送。
  * 返回  无。
  */
void
SPI_PCM_CLOCK_Polarity_Config(SPI_SFRmap* SPIx,uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_I2S_CKP(NewState));

    /*---------------- 设置SPI_CTLR寄存器PCMCKP位 ----------------*/
    if (NewState != I2S_CKP_LOW)
    {
        /* PCM数据在时钟下降沿发送*/
        SFR_SET_BIT_ASM(SPIx->CTLR, SPI_CTLR_PCMCKP_POS);
    }
    else
    {
        /*PCM数据在时钟上升沿发送*/
        SFR_CLR_BIT_ASM(SPIx->CTLR, SPI_CTLR_PCMCKP_POS);
    }
}

/**
  * 描述  设置主设备时钟输出使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: 主设备时钟输出使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_MAIN_CLOCK_OUT_Enable (SPI_SFRmap* SPIx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_BRGR寄存器MCKEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能MCK输出*/
        SFR_SET_BIT_ASM(SPIx->BRGR, SPI_BRGR_MCKEN_POS);
    }
    else
    {
        /*未使能MCK输出*/
        SFR_CLR_BIT_ASM(SPIx->BRGR, SPI_BRGR_MCKEN_POS);
    }
}
/**
  *   ##### 串行外设接口(SPI)配置函数定义结束 #####
  */


/**
  *   ##### 串行外设接口(SPI)中断管理函数 #####
  */
/**
  * 描述  设置SPI接收溢出中断使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI接收溢出中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_Receive_Overflow_INT_Enable (SPI_SFRmap* SPIx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器ROVFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI接收溢出中断 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_ROVFIE_POS);
    }
    else
    {
        /* 禁止SPI接收溢出中断 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_ROVFIE_POS);
    }
}

/**
  * 描述  设置SPI发送溢出中断使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI发送溢出中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_Transmit_Overflow_INT_Enable (SPI_SFRmap* SPIx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器TOVFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI发送溢出中断 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_TOVFIE_POS);
    }
    else
    {
        /* 禁止SPI发送溢出中断 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_TOVFIE_POS);
    }
}

/**
  * 描述  设置SPI RBUF不为空中断使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI RBUF不为空中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_RNEIE_INT_Enable (SPI_SFRmap* SPIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器RNEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI RBUF不为空中断 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_RNEIE_POS);
    }
    else
    {
        /* 禁止SPI RBUF不为空中断 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_RNEIE_POS);
    }
}

/**
  * 描述  设置SPI TBUF为空中断使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI TBUF为空中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_TNEIE_INT_Enable (SPI_SFRmap* SPIx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器TNEIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI TBUF为空中断 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_TNEIE_POS);
    }
    else
    {
        /* 禁止SPI TBUF为空中断 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_TNEIE_POS);
    }
}


/**
  * 描述  设置SPI接收DMA中断使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI接收DMA中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_Receive_DMA_INT_Enable (SPI_SFRmap* SPIx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器SRCDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI接收DMA中断 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_SRCDE_POS);
    }
    else
    {
        /* 禁止SPI接收DMA中断 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_SRCDE_POS);
    }
}

/**
  * 描述  设置SPI发送DMA中断使能。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI发送DMA中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_Transmit_DMA_INT_Enable (SPI_SFRmap* SPIx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器STXDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SPI发送DMA中断 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_STXDE_POS);
    }
    else
    {
        /* 禁止SPI发送DMA中断 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_STXDE_POS);
    }
}

/**
  * 描述  设置SPI发送声道选择。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  *       NewState: SPI发送声道选择，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
SPI_Transmit_CHSIDE_INT_Enable (SPI_SFRmap* SPIx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SPI_STR寄存器CHSIDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 正在发送的是右声道 */
        SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_CHSIDE_POS);
    }
    else
    {
        /* 正在发送的是左声道 */
        SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_CHSIDE_POS);
    }
}

/**
  * 描述  获取SPI忙状态 。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  1: SPI正在发送数据；
  *       0: SPI没有在发送数据。
  */
FlagStatus
SPI_Get_BUSY_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 读取SPI_STR寄存器SPIBUSY位 ----------------*/
    if ((SPIx->STR) & SPI_STR_SPIBUSY)
    {
        /* SPI正在发送数据 */
        return SET;
    }
    else
    {
        /* SPI没有在发送数据 */
        return RESET;
    }
}

/**
  * 描述  获取SPI接收溢出中断标志状态 。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  1: SPI接收溢出；
  *       0: SPI没有接收溢出。
  */
FlagStatus
SPI_Get_Receive_Overflow_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 读取SPI_STR寄存器ROVFIF位 ----------------*/
    if (SPIx->STR & SPI_STR_ROVFIF)
    {
        /* SPI接收溢出 */
        return SET;
    }
    else
    {
        /* SPI没有接收溢出 */
        return RESET;
    }
}


/**
  * 描述  获取SPI产生发送溢出标志状态 。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  1: SPI产生发送溢出；
  *       0: SPI没有产生发送溢出。
  */
FlagStatus
SPI_Get_Transmit_Overflow_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 读取SPI_STR寄存器TOVFIF位 ----------------*/
    if (SPIx->STR & SPI_STR_TOVFIF)
    {
        /* SPI产生发送溢出 */
        return SET;
    }
    else
    {
        /* SPI没有产生发送溢出 */
        return RESET;
    }
}

/**
  * 描述  获取SPI接收BUF未空状态 。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  1: SPI接收BUF不为空；
  *       0: SPI接收BUF为空。
  */
FlagStatus
SPI_Get_Receive_Buf_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 读取SPI_STR寄存器RNE位 ----------------*/
    if (SPIx->STR & SPI_STR_RNE)
    {
        /* SPI发送BUF不为空 */
        return SET;
    }
    else
    {
        /* SPI发送BUF为空 */
        return RESET;
    }
}

/**
  * 描述  获取SPI发送BUF未空状态 。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  1: SPI发送BUF不为空；
  *       0: SPI发送BUF为空。
  */
FlagStatus
SPI_Get_Transmit_Buf_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 读取SPI_STR寄存器TNE位 ----------------*/
    if (SPIx->STR & SPI_STR_TNE)
    {
        /* SPI发送BUF不为空 */
        return SET;
    }
    else
    {
        /* SPI发送BUF为空 */
        return RESET;
    }
}

/**
  * 描述  清零SPI接收溢出中断标志。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  无。
  */
void
SPI_Clear_Receive_Overflow_INT_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 设置SPI_STR寄存器ROVFIC位 ----------------*/
    SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_ROVFIC_POS);
    while(SPIx->STR & SPI_STR_ROVFIF);
    SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_ROVFIC_POS);
}

/**
  * 描述  清零SPI发送溢出中断标志。
  * 输入  SPIx: 指向SPI内存结构的指针，取值为SPI0_SFR~SPI3_SFR。
  * 返回  无。
  */
void
SPI_Clear_Transmit_Overflow_INT_Flag (SPI_SFRmap* SPIx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SPI_ALL_PERIPH(SPIx));

    /*---------------- 设置SPI_STR寄存器TOVFIC位 ----------------*/
    SFR_SET_BIT_ASM(SPIx->STR, SPI_STR_TOVFIC_POS);
    while(SPIx->STR & SPI_STR_TOVFIF);
    SFR_CLR_BIT_ASM(SPIx->STR, SPI_STR_TOVFIC_POS);
}
/**
  *   ##### 串行外设接口(SPI)中断管理函数定义结束 #####
  */
