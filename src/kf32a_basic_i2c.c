/**
  ******************************************************************************
  * 文件名  kf32a_basic_i2c.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描  述  该文件提供了内部集成电路接口(I2C)外设功能函数，包含：
  *          + 内部集成电路接口(I2C)初始化函数
  *          + 内部集成电路接口(I2C)功能配置函数
  *          + 内部集成电路接口(I2C)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_i2c.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"


/* I2C私有定义 ----------------------------------------------------*/
/* I2C_CTLR寄存器主机配置掩码 */
#define I2C_CTLR_INIT_MASK              (I2C_CTLR_SMBUS \
                                       | I2C_CTLR_I2CCKS \
                                       | I2C_CTLR_BADR10 \
                                       | I2C_CTLR_SMBT \
                                       | I2C_CTLR_ACKEN \
                                       | I2C_CTLR_ACKDT)


/**
  *   ##### 内部集成电路接口(I2C)初始化函数 #####
  */
/**
  * 描述  I2C外设复位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Reset (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
#ifdef KF32A_Periph_i2c0
    if (I2Cx == I2C0_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C0RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C0RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_I2C0CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_i2c1
    if (I2Cx == I2C1_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C1RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C1RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_I2C1CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_i2c2
    if (I2Cx == I2C2_SFR)
    {
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C2RST, TRUE);
        RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_I2C2RST, FALSE);
        PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_I2C2CLKEN, TRUE);
    }
#endif
#ifdef KF32A_Periph_i2c3
    if (I2Cx == I2C3_SFR)
    {
        RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_I2C3RST, TRUE);
        RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_I2C3RST, FALSE);
        PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_I2C3CLKEN, TRUE);
    }
#endif

}

/**
  * 描述  I2C外设配置。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       i2cInitStruct: I2C模块配置信息结构体指针。
  * 返回  无。
  */
void
I2C_Configuration (I2C_SFRmap* I2Cx, I2C_InitTypeDef* i2cInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_MODE(i2cInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_I2C_CLK(i2cInitStruct->m_ClockSource));
    CHECK_RESTRICTION(CHECK_I2C_BUFR_ADDRESS(i2cInitStruct->m_BADR10));
    CHECK_RESTRICTION(CHECK_SMBUS_MODE(i2cInitStruct->m_MasterSlave));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(i2cInitStruct->m_AckEn));
    CHECK_RESTRICTION(CHECK_I2C_ACKDATA(i2cInitStruct->m_AckData));

    /*---------------------------- I2Cx_CTLR寄存器配置  -----------------*/
    /* 根据结构体成员m_Mode，设置SMBUS位域 */
    /* 根据结构体成员m_ClockSource，设置I2CCKS位域 */
    /* 根据结构体成员m_BADR10，设置BADR10位域 */
    /* 根据结构体成员m_MasterSlave，设置SMBT位域 */
    /* 根据结构体成员m_AckEn，设置ACKEN位域 */
    /* 根据结构体成员m_AckData，设置ACKDT位域 */
	tmpreg = ((i2cInitStruct->m_Mode)    \
			| (i2cInitStruct->m_ClockSource)   \
			| (i2cInitStruct->m_BADR10)  \
			| (i2cInitStruct->m_MasterSlave) \
			| (i2cInitStruct->m_AckEn << I2C_CTLR_ACKEN_POS) \
			| (i2cInitStruct->m_AckData));
    I2Cx->CTLR = SFR_Config (I2Cx->CTLR, ~I2C_CTLR_INIT_MASK, tmpreg);

    /*---------------------------- I2Cx_BRGR寄存器配置  -----------------*/
    /* 根据结构体成员m_BaudRateL，设置I2CBRGL位域 */
    /* 根据结构体成员m_BaudRateH，设置I2CBRGH位域 */
    tmpreg = ((i2cInitStruct->m_BaudRateL << I2C_BRGR_I2CBRGL0_POS) \
            | (i2cInitStruct->m_BaudRateH << I2C_BRGR_I2CBRGH0_POS));
    I2Cx->BRGR = SFR_Config (I2Cx->BRGR,
                        ~(I2C_BRGR_I2CBRGL | I2C_BRGR_I2CBRGH),
                        tmpreg);
}

/**
  * 描述  初始化I2C配置信息结构体。
  * 输入  dacInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void I2C_Struct_Init (I2C_InitTypeDef* I2C_InitStruct)
{
    /* 初始化 I2C模式选择*/
    I2C_InitStruct->m_Mode = I2C_MODE_I2C;

    /* 初始化 I2C时钟选择 */
    I2C_InitStruct->m_ClockSource = I2C_CLK_SCLK;

    /* 初始化 I2C地址选择 */
    I2C_InitStruct->m_BADR10 = I2C_BUFRADDRESS_7BIT;

    /* 初始化 SMBus类型选择 */
    I2C_InitStruct->m_MasterSlave = I2C_MODE_SMBUSDEVICE;

    /* 初始化 SCL低电平占用的时钟周期数 */
    I2C_InitStruct->m_BaudRateL = 4;

    /* 初始化 SCL高电平占用的时钟周期数 */
    I2C_InitStruct->m_BaudRateH = 4;

    /* 初始化 I2C应答使能配置 */
    I2C_InitStruct->m_AckEn = FALSE;

    /* 初始化 I2C应答数据位 */
    I2C_InitStruct->m_AckData = I2C_ACKDATA_ACK;
}
/**
  *   ##### 内部集成电路接口(I2C)初始化函数定义结束 #####
  */


/**
  *   ##### 内部集成电路接口(I2C)功能配置函数 #####
  */
/**
  * 描述  控制I2C使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Cmd(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器I2CEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_I2CEN_POS);
    }
    else
    {
        /* 禁止I2C */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_I2CEN_POS);
    }
}

/**
  * 描述  控制I2C10位地址使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C使能位配置信息，
  *                 取值为 I2C_BUFRADDRESS_10BIT 或 I2C_BUFRADDRESS_7BIT。
  * 返回  无。
  */
void
I2C_Bufr_Address_Config(I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_BUFR_ADDRESS(NewState));

    /*---------------- 配置I2C_CTLR寄存器BADR10位 ----------------*/
    if (NewState != I2C_BUFRADDRESS_7BIT)
    {
        /* I2C模块使用10位地址 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_BADR10_POS);
    }
    else
    {
        /* I2C模块使用7位地址 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_BADR10_POS);
    }
}

/**
  * 描述  控制I2C_启动使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C启动使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Generate_START(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器SEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C启动 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SEN_POS);
    }
    else
    {
        /* 禁止I2C启动 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SEN_POS);
    }
}

/**
  * 描述  控制I2C停止条件使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C停止条件使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Generate_STOP(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器PEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C停止条件 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_PEN_POS);
    }
    else
    {
        /* 禁止I2C停止条件 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_PEN_POS);
    }
}

/**
  * 描述  控制I2C_Ack应答使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C应答使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Ack_Config (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器ACKEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C_Ack */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKEN_POS);
    }
    else
    {
        /* 禁止I2C_Ack */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKEN_POS);
    }
}

/**
  * 描述  控制I2C_Ack应答数据位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C应答数据位配置信息，取值为
  *                   I2C_ACKDATA_ACK:  应答
  *                   I2C_ACKDATA_NO_ACK: 不应答
  * 返回  无。
  */
void
I2C_Ack_DATA_Config (I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_ACKDATA(NewState));

    /*---------------- 配置I2C_CTLR寄存器ACKDT位 ----------------*/
    if (NewState != I2C_ACKDATA_ACK)
    {
        /* 不应答*/
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKDT_POS);
    }
    else
    {
        /* 应答 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ACKDT_POS);
    }
}

/**
  * 描述  控制I2C_Call使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C_Call使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Call_Cmd (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器GCEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C_Call */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_GCEM_POS);
    }
    else
    {
        /* 禁止I2C_Call */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_GCEM_POS);
    }
}

/**
  * 描述  配置I2C工作时钟选择寄存器。
  * 输入  ClkSource: 时钟选择，取值范围为：
  *                    I2C_CLK_SCLK: 选用SCLK为I2C工作时钟
  *                    I2C_CLK_HFCLK: 选用HFCLK为I2C工作时钟
  *                    I2C_CLK_LFCLK: 选用LFCLK为I2C工作时钟
  * 返回  无。
  */
void
I2C_Clock_Config (I2C_SFRmap* I2Cx,uint32_t ClkSource)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_CLK(ClkSource));

    /*---------------- 设置USART_CTLR寄存器I2CCKS位 ----------------*/
    I2Cx->CTLR = SFR_Config (I2Cx->CTLR, ~I2C_CTLR_I2CCKS, ClkSource);
}

/**
  * 描述  控制I2C地址寄存器匹配位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C_地址寄存器匹配位，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_MATCH_ADDRESS_Config (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置USART_CTLR寄存器MTHALL位 ----------------*/
    if (NewState != FALSE)
    {
        /* 可在任意接受的地址上产生中断，将使器件监控总线上的所有通信量 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_MTHALL_POS);
    }
    else
    {
        /* 只有在I2CADDR地址匹配时才会接收数据 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_MTHALL_POS);
    }
}

/**
  * 描述  控制I2C SCL输出使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C_SCL输出使能，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_SCL_Enable (I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器ENASCL位 ----------------*/
    if (NewState != FALSE)
    {
        /* 正常使用SCL线 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ENASCL_POS);
    }
    else
    {
        /* SCL被强制为高 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ENASCL_POS);
    }
}

/**
  * 描述  控制I2C 监控模式使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C_监控模式使能位，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_NMENA_Enable(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器NMENA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能监控模式 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_NMENA_POS);
    }
    else
    {
        /* 不使能监控模式 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_NMENA_POS);
    }
}

/**
  * 描述  控制SMBus模式使能信号。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: SMBus模式使能信号，取值为:
  *                   I2C_MODE_SMBUS: SMBus模式
  *                   I2C_MODE_I2C: I2C模式
  * 返回  无。
  */
void
I2C_SMBUS_Enable(I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_MODE(NewState));

    /*---------------- 配置I2C_CTLR寄存器SMBUS位 ----------------*/
    if (NewState != I2C_MODE_I2C)
    {
        /* 1 = SMBus模式 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBUS_POS);
    }
    else
    {
        /* 0 = I2C模式 */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBUS_POS);
    }
}

/**
  * 描述  控制SMBUS类型。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: SMBUS类型，取值为:
  *                   I2C_MODE_SMBUSHOST: SMBus主机
  *                   I2C_MODE_SMBUSDEVICE: SMBus设备
  * 返回  无。
  */
void
I2C_SMBT_Config(I2C_SFRmap* I2Cx, uint32_t NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_SMBUS_MODE(NewState));

    /*---------------- 配置I2C_CTLR寄存器SMBT位 ----------------*/
    if (NewState != I2C_MODE_SMBUSDEVICE)
    {
        /* SMBus主机 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBT_POS);
    }
    else
    {
        /* SMBus设备*/
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_SMBT_POS);
    }
}

/**
  * 描述  控制SMBus提醒。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: SMBus提醒，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_SMBus_ALERT_Config(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器ALERT位 ----------------*/
    if (NewState != FALSE)
    {
        /* 驱动SMBALT引脚使其变低 */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ALERT_POS);
    }
    else
    {
        /* 释放SMBALT引脚使其变高*/
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ALERT_POS);
    }
}

/**
  * 描述  I2C发送数据。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。。
  *       Data: 写入数据寄存器的值，取值为10位数据。
  * 返回  无。
  */
void
I2C_SendData (I2C_SFRmap* I2Cx, uint32_t Data)
{
     /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_BUFR(Data));

    /*---------------- 配置I2C_BUFR寄存器 ----------------*/
    I2Cx->BUFR = Data;
}

/**
  * 描述  I2C发送8位数据。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。。
  *       Data: 写入数据寄存器的值，取值为8位数据。
  * 返回  无。
  */
void
I2C_SendData8 (I2C_SFRmap* I2Cx, uint8_t Data)
{
     /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_BUFR(Data));

    /*---------------- 配置I2C_BUFR寄存器 ----------------*/
    I2Cx->BUFR = Data;
}

/**
  * 描述  I2C接收数据。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。。
  * 返回  32位类型数据的10位数据。
  */
uint32_t I2C_ReceiveData(I2C_SFRmap* I2Cx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 获取I2C_BUFR寄存器 ----------------*/
    tmpreg = I2Cx->BUFR;
    tmpreg &= I2C_BUFR_I2CBUF;
    tmpreg >>= I2C_BUFR_I2CBUF0_POS;

    return tmpreg;
}

/**
  * 描述  控制I2C_ARP使能位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C_ARP使能位配置信息，取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_ARP_Enable(I2C_SFRmap* I2Cx, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置I2C_CTLR寄存器ARPEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C_ARP */
        SFR_SET_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ARPEN_POS);
    }
    else
    {
        /* 禁止I2C_ARP */
        SFR_CLR_BIT_ASM(I2Cx->CTLR, I2C_CTLR_ARPEN_POS);
    }
}

/**
  * 描述  配置I2C地址位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       AddrSelect: 地址寄存器选择，取值为0x0~0x3。
  *       Data: 地址位选择，取值为0x0~0x3FF。
  * 返回  无。
  */
void
I2C_ADDR_Config(I2C_SFRmap* I2Cx, uint32_t AddrSelect, uint32_t Data)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_ADDR_REGISTER(AddrSelect));
    CHECK_RESTRICTION(CHECK_I2C_ADDR(Data));

    /*---------------- 设置I2C_ADDR寄存器I2CADD位 ----------------*/
    tmpreg = Data << I2C_ADDR_I2CADD0_POS;
    if (0 == AddrSelect)
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0);
    }
    else
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0) + 4 + (4 * AddrSelect);
    }
    *(volatile uint32_t *)tmpaddr =
                SFR_Config (*(volatile uint32_t *)tmpaddr,
                            ~I2C_ADDR_I2CADD,
                            tmpreg);
//    I2Cx->ADDR0 = SFR_Config (I2Cx->ADDR0, ~I2C_ADDR_I2CADD, tmpreg);
}

/**
  * 描述  配置I2C地址屏蔽位。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       AddrSelect: 地址寄存器选择，取值为0x0~0x3。
  *       DataMask: 地址位选择，取值为0x0~0x3FF。
  * 返回  无。
  */
void
I2C_MSK_Config(I2C_SFRmap* I2Cx, uint32_t AddrSelect, uint32_t DataMask)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_I2C_ADDR_REGISTER(AddrSelect));
    CHECK_RESTRICTION(CHECK_I2C_MSK(DataMask));

    /*---------------- 设置I2C_ADDR寄存器位I2CMSK ----------------*/
    tmpreg = DataMask << I2C_ADDR_I2CMSK0_POS;
    if (0 == AddrSelect)
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0);
    }
    else
    {
        tmpaddr = (uint32_t)&(I2Cx->ADDR0) + 4 + (4 * AddrSelect);
    }
    *(volatile uint32_t *)tmpaddr =
                SFR_Config (*(volatile uint32_t *)tmpaddr,
                            ~I2C_ADDR_I2CMSK,
                            tmpreg);
}

/**
  * 描述  配置I2C SCL高电平占用的时钟周期数。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       Period: SCL高电平占用的时钟周期数，取值为0x0~0xFFFF。
  * 返回  无。
  */
void
I2C_BRGH_Config (I2C_SFRmap* I2Cx,uint16_t Period)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 设置I2C_ADDR寄存器位I2CMSK ----------------*/
    tmpreg = (uint32_t)Period << I2C_BRGR_I2CBRGH0_POS;
    I2Cx->BRGR = SFR_Config (I2Cx->BRGR, ~I2C_BRGR_I2CBRGH, tmpreg);
}

/**
  * 描述  配置I2C SCL低电平占用的时钟周期数。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       Period: SCL低电平占用的时钟周期数，取值为0x0~0xFFFF。
  * 返回  无。
  */
void
I2C_BRGL_Config (I2C_SFRmap* I2Cx,uint16_t Period)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 设置I2C_ADDR寄存器位I2CMSK ----------------*/
    tmpreg = (uint32_t)Period << I2C_BRGR_I2CBRGL0_POS;
    I2Cx->BRGR = SFR_Config (I2Cx->BRGR, ~I2C_BRGR_I2CBRGL, tmpreg);
}
/**
  *   ##### 内部集成电路接口(I2C)功能配置函数定义结束 #####
  */


/**
  *   ##### 内部集成电路接口(I2C)中断管理函数 #####
  */
/**
  * 描述  设置I2C起始信号中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C起始信号中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Start_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器SIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C起始信号中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SIE_POS);
    }
    else
    {
        /* 禁止I2C起始信号中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SIE_POS);
    }
}

/**
  * 描述  设置I2C停止信号中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C停止信号中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Stop_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器PIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C停止信号中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_PIE_POS);
    }
    else
    {
        /* 禁止I2C停止信号中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_PIE_POS);
    }
}

/**
  * 描述  设置I2C应答错误中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C应答错误中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Ack_Fail_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器AFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C应答错误中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_AFIE_POS);
    }
    else
    {
        /* 禁止I2C应答错误中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_AFIE_POS);
    }
}

/**
  * 描述  设置I2C失去仲裁中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C失去仲裁中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Arbitration_Lost_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器ARBLIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C失去仲裁中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_ARBLIE_POS);
    }
    else
    {
        /* 禁止I2C失去仲裁中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_ARBLIE_POS);
    }
}

/**
  * 描述  设置SMBus提醒中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: SMBus提醒中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_SMBus_Alert_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器SMBAIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SMBus提醒中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SMBAIE_POS);
    }
    else
    {
        /* 禁止SMBus提醒中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SMBAIE_POS);
    }
}

/**
  * 描述  设置SMBus主机头系列中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: SMBus主机头系列中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_SMBus_HostHead_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器SMBHIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SMBus主机头系列中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SMBHIE_POS);
    }
    else
    {
        /* 禁止SMBus主机头系列中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SMBHIE_POS);
    }
}

/**
  * 描述  设置SMBus设备默认地址中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: SMBus设备默认地址中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_SMBus_Device_Defaultaddress_INT_Enable (I2C_SFRmap* I2Cx,
                    FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器SMBDIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能SMBus设备默认地址中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_SMBDIE_POS);
    }
    else
    {
        /* 禁止SMBus设备默认地址中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_SMBDIE_POS);
    }
}

/**
  * 描述  设置I2C中断信号使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C中断信号状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_ISIE_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器ISIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C中断信号 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_ISIE_POS);
    }
    else
    {
        /* 禁止I2C中断信号 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_ISIE_POS);
    }
}

/**
  * 描述  设置I2C接收DMA中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C接收DMA中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Receive_DMA_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器IRCDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C接收DMA中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_IRCDE_POS);
    }
    else
    {
        /* 禁止I2C接收DMA中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_IRCDE_POS);
    }
}

/**
  * 描述  设置I2C发送DMA中断使能。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  *       NewState: I2C发送DMA中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
I2C_Transmit_DMA_INT_Enable (I2C_SFRmap* I2Cx,FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置I2C_IER寄存器ITXDE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能I2C发送DMA中断 */
        SFR_SET_BIT_ASM(I2Cx->IER, I2C_IER_ITXDE_POS);
    }
    else
    {
        /* 禁止I2C发送DMA中断 */
        SFR_CLR_BIT_ASM(I2Cx->IER, I2C_IER_ITXDE_POS);
    }
}

/**
  * 描述  获取I2C起始信号标志位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 总线上出现了起始位；
  *       0: 总线上未出现了起始位。
  */
FlagStatus
I2C_Get_Start_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器SIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_SIF)
    {
        /* 总线上出现了起始位 */
        return SET;
    }
    else
    {
        /* 总线上未出现了起始位 */
        return RESET;
    }
}

/**
  * 描述  清零I2C起始信号标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_Start_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器SIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_SIF)>>I2C_SR_SIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SIF_POS);
    }
}

/**
  * 描述  获取I2C停止信号标志位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 总线上出现了停止位；
  *       0: 总线上未出现了停止位。
  */
FlagStatus
I2C_Get_Stop_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器PIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_PIF)
    {
        /* 总线上出现了停止位 */
        return SET;
    }
    else
    {
        /* 总线上未出现了停止位 */
        return RESET;
    }
}

/**
  * 描述  清零I2C停止信号标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_Stop_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器PIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_PIF)>>I2C_SR_PIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_PIF_POS);
    }

}

/**
  * 描述  获取I2C地址匹配状态位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 从机收到匹配地址且应答；（总线上出现停止位将硬件清零该状态位）
  *       0: 从机未收到对应地址。
  */
FlagStatus
I2C_Get_Address_Match_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器ADDR位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_ADDR)
    {
        /* 从机收到匹配地址且应答 */
        return SET;
    }
    else
    {
        /* 从机未收到对应地址 */
        return RESET;
    }
}

/**
  * 描述  获取I2C高位地址状态位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 上次接收或发送的字节是高位地址；
  *       0: 上次接收或发送的字节不是高位地址。
  */
FlagStatus
I2C_Get_HighAddress_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器ADD10位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_ADD10)
    {
        /* 上次接收或发送的字节是高位地址 */
        return SET;
    }
    else
    {
        /* 上次接收或发送的字节不是高位地址*/
        return RESET;
    }
}

/**
  * 描述  获取I2C数据内容状态位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 表示上次接收或发送的字节是数据（总线上出现起始位将清零该状态位）
  *       0: 表示上次接收或发送的字节是地址。
  */
FlagStatus
I2C_Get_Data_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器DATA位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_DATA)
    {
        /* 表示上次接收或发送的字节是数据 */
        return SET;
    }
    else
    {
        /* 表示上次接收或发送的字节是地址*/
        return RESET;
    }
}

/**
  * 描述  获取I2C应答错误标志位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 发生了应答错误；
  *       0: 未发生应答错误。
  */
FlagStatus
I2C_Get_Ack_Fail_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器AFIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_AFIF)
    {
        /* 表示上次接收或发送的字节是数据 */
        return SET;
    }
    else
    {
        /* 表示上次接收或发送的字节是地址*/
        return RESET;
    }
}

/**
  * 描述  清零I2C应答错误标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_Ack_Fail_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器AFIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_AFIF)>>I2C_SR_AFIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_AFIF_POS);
    }
}

/**
  * 描述  获取I2C失去仲裁标志位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 发送数据过程中失去仲裁；
  *       0: 发送数据过程中未失去仲裁。
  */
FlagStatus
I2C_Get_Arbitration_Lost_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器ARBLIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_ARBLIF)
    {
        /* 发送数据过程中失去仲裁 */
        return SET;
    }
    else
    {
        /* 发送数据过程中未失去仲裁*/
        return RESET;
    }
}

/**
  * 描述  清零I2C失去仲裁标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_Arbitration_Lost_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器ARBLIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_ARBLIF)>>I2C_SR_ARBLIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_ARBLIF_POS);
    }
}

/**
  * 描述  获取I2C读/ 写信息状态位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 读信息状态位；
  *       0: 写信息状态位。
  */
FlagStatus
I2C_Get_Write_Read_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器RNW位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_RNW)
    {
        /* 读信息状态位*/
        return SET;
    }
    else
    {
        /* 写信息状态位*/
        return RESET;
    }
}

/**
  * 描述  获取SMBus提醒中断标志状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 读信息状态位；
  *       0: 写信息状态位。
  */
FlagStatus
I2C_Get_SMBus_Alert_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器SMBAIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_SMBAIF)
    {
        /* 产生SMBus提醒事件*/
        return SET;
    }
    else
    {
        /* 无SMBus提醒*/
        return RESET;
    }
}

/**
  * 描述  清零SMBus提醒中断标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_SMBus_Alert_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器SMBAIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_SMBAIF)>>I2C_SR_SMBAIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SMBAIF_POS);
    }
}

/**
  * 描述  获取SMBus主机头系列中断标志状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 读信息状态位；
  *       0: 写信息状态位。
  */
FlagStatus
I2C_Get_SMBus_Host_Header_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器SMBHIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_SMBHIF)
    {
        /* 产生SMBus提醒事件*/
        return SET;
    }
    else
    {
        /* 无SMBus提醒*/
        return RESET;
    }
}

/**
  * 描述  清零SMBus主机头系列中断标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_SMBus_Host_Header_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器SMBHIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_SMBHIF)>>I2C_SR_SMBHIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SMBHIF_POS);
    }
}

/**
  * 描述  获取SMBus设备默认地址(从模式) 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 读信息状态位；
  *       0: 写信息状态位。
  */
FlagStatus
I2C_Get_SMBus_Device_Default_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器SMBDIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_SMBDIF)
    {
        /* 当ARPEN=1时，收到SMBus设备的默认地址*/
        return SET;
    }
    else
    {
        /* 未收到SMBus设备的默认地址*/
        return RESET;
    }
}

/**
  * 描述  清零SMBus设备默认地址(从模式)标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_SMBus_Device_Default_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器SMBDIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_SMBDIF)>>I2C_SR_SMBDIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_SMBDIF_POS);
    }
}

/**
  * 描述  获取I2C中断信号标志位状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 产生了I2C中断信号标志位；
  *       0: 未产生I2C中断信号标志位。
  */
FlagStatus
I2C_Get_INTERRUPT_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器ISIF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_ISIF)
    {
        /* 产生了I2C中断信号标志位*/
        return SET;
    }
    else
    {
        /* 未产生I2C中断信号标志位*/
        return RESET;
    }
}

/**
  * 描述  清零I2C中断信号标志。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  无。
  */
void
I2C_Clear_INTERRUPT_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 清零I2C_SR寄存器ISIF位 ----------------*/
    while(((I2Cx->SR) & I2C_SR_ISIF)>>I2C_SR_ISIF_POS)
    {
    	SFR_CLR_BIT_ASM(I2Cx->SR, I2C_SR_ISIF_POS);

    }
}

/**
  * 描述  获取I2C接收BUFF为满状态 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 接收BUFF为满；(读 I2Cx_BUFR 清零该位)
  *       0: 接收BUFF为空。
  */
FlagStatus
I2C_Get_Receive_Buff_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器RCBF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_RCBF)
    {
        /* 接收BUFF为满*/
        return SET;
    }
    else
    {
        /* 接收BUFF为空*/
        return RESET;
    }
}

/**
  * 描述  获取I2C发送BUFF状态位 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 等待写I2Cx_BUFR；(写 I2Cx_BUFR 清零该位)
  *       0: 不需要写I2Cx_BUFR。
  */
FlagStatus
I2C_Get_Transmit_Buff_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器TXBE位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_TXBE)
    {
        /* 等待写I2Cx_BUFR*/
        return SET;
    }
    else
    {
        /* 不需要写I2Cx_BUFR*/
        return RESET;
    }
}

/**
  * 描述  获取I2C接收DMA中断标志位 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 产生了I2C接收DMA中断；
  *       0: 未产生I2C接收DMA中断。
  */
FlagStatus
I2C_Get_Receive_DMA_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器IRCDF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_IRCDF)
    {
        /* 产生了I2C接收DMA中断*/
        return SET;
    }
    else
    {
        /* 未产生I2C接收DMA中断*/
        return RESET;
    }
}

/**
  * 描述  获取I2C发送DMA中断标志位 。
  * 输入  I2Cx: 指向I2C内存结构的指针，取值为I2C0_SFR~I2C3_SFR。
  * 返回  1: 产生了I2C发送DMA中断；
  *       0: 未产生I2C发送DMA中断。
  */
FlagStatus
I2C_Get_Transmit_DMA_Flag (I2C_SFRmap* I2Cx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_I2C_ALL_PERIPH(I2Cx));

    /*---------------- 读取I2C_SR寄存器ITXDF位 ----------------*/
    if ((I2Cx->SR) & I2C_SR_ITXDF)
    {
        /* 产生了I2C发送DMA中断*/
        return SET;
    }
    else
    {
        /* 未产生I2C发送DMA中断*/
        return RESET;
    }
}
/**
  *   ##### 内部集成电路接口(I2C)中断管理函数定义结束 #####
  */
