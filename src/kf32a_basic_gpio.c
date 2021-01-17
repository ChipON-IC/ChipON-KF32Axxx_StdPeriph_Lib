/**
  ******************************************************************************
  * 文件名  kf32a_basic_gpio.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了通用输入输出端口(GPIO)外设功能函数，包含：
  *          + 通用输入输出端口(GPIO)初始化及配置函数
  *          + 通用输入输出端口(GPIO)读写函数
  *          + 通用输入输出端口(GPIO)重映射配置函数
  *
  *********************************************************************
  */

#include "kf32a_basic_gpio.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

/**
  *   ##### 通用输入输出端口(GPIO)初始化及配置函数 #####
  */
/**
  * 描述  复位GPIO外设，使能外设时钟。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  * 返回  无。
  */
void
GPIO_Reset (GPIO_SFRmap* GPIOx)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));

    if (GPIOx == GPIOA_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOARST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOARST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOACLKEN, TRUE);
    }
    else if (GPIOx == GPIOB_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOBRST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOBRST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOBCLKEN, TRUE);
    }
    else if (GPIOx == GPIOC_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOCRST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOCRST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOCCLKEN, TRUE);
    }
    else if (GPIOx == GPIOD_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIODRST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIODRST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIODCLKEN, TRUE);
    }
    else if (GPIOx == GPIOE_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOERST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOERST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOECLKEN, TRUE);
    }
    else if (GPIOx == GPIOF_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOFRST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOFRST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOFCLKEN, TRUE);
    }
    else if (GPIOx == GPIOG_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOGRST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOGRST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOGCLKEN, TRUE);
    }
    else if (GPIOx == GPIOH_SFR)
    {
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOHRST, TRUE);
        RST_CTL0_Peripheral_Reset_Enable(RST_CTL0_GPIOHRST, FALSE);
        PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIOHCLKEN, TRUE);
    }
    else
    {
        ;
    }
}

/**
  * 描述  通用输入输出端口(GPIO)初始化配置。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       gpioInitStruct: GPIO配置信息结构体指针。
  * 返回  无。
  */
void
GPIO_Configuration (GPIO_SFRmap* GPIOx, GPIO_InitTypeDef* gpioInitStruct)
{
    uint32_t pinpos = 0x00;
    uint32_t pos = 0x00;
    uint32_t currentpin = 0x00;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(gpioInitStruct->m_Pin));
    CHECK_RESTRICTION(CHECK_GPIO_MODE(gpioInitStruct->m_Mode));
    CHECK_RESTRICTION(CHECK_GPIO_SPEED(gpioInitStruct->m_Speed));
    CHECK_RESTRICTION(CHECK_GPIO_PU(gpioInitStruct->m_PullUp));
    CHECK_RESTRICTION(CHECK_GPIO_PD(gpioInitStruct->m_PullDown));

    /* 配置一个引脚信息 */
    for (pinpos = 0x00; pinpos < 0x10; pinpos++)
    {
        pos = ((uint32_t)0x01) << pinpos;
        /* 获取引脚位置信息 */
        currentpin = (gpioInitStruct->m_Pin) & pos;

        if (currentpin == pos)
        {
            /*----------- 设置GPIOx_PMOD寄存器 -----------*/
            /* 根据结构体成员m_Mode，设置PXPMDy位域 */
            tmpreg = (((uint32_t)gpioInitStruct->m_Mode) << (pinpos * 2));
            GPIOx->PMOD = SFR_Config (GPIOx->PMOD,
                                  ~(GPIO_PMOD_PXPMD0 << (pinpos * 2)),
                                  tmpreg);

            if ((gpioInitStruct->m_Mode == GPIO_MODE_OUT)
                || (gpioInitStruct->m_Mode == GPIO_MODE_RMP))
            {
                /* GPIO开漏控制参数校验 */
                CHECK_RESTRICTION(CHECK_GPIO_POD(gpioInitStruct->m_OpenDrain));

                /*----------- 设置GPIOx_PODR寄存器 -----------*/
                /* 根据结构体成员m_OpenDrain，设置PXPODR位域 */
                tmpreg = (((uint32_t)gpioInitStruct->m_OpenDrain) << (pinpos));
                GPIOx->PODR = SFR_Config (GPIOx->PODR,
                                      ~(GPIO_PODR_PXPODR0 << pinpos),
                                      tmpreg);
            }
            else
            {
                ;
            }

            /*----------- 设置GPIOx_OMOD寄存器 -----------*/
            /* 根据结构体成员m_Speed，设置PXOMD位域 */
            tmpreg = ((uint32_t)gpioInitStruct->m_Speed) << (pinpos * 2);
            GPIOx->OMOD = SFR_Config (GPIOx->OMOD,
                                  ~(GPIO_OMOD_PXOMD0 << (pinpos * 2)),
                                  tmpreg);

            /*----------- 设置GPIOx_PUR寄存器 -----------*/
            /* 根据结构体成员m_PullUp，设置PXPUR位域 */
            tmpreg = (((uint32_t)gpioInitStruct->m_PullUp) << (pinpos));
            GPIOx->PUR = SFR_Config (GPIOx->PUR,
                                  ~(GPIO_PUR_PXPUR0 << pinpos),
                                  tmpreg);

            /*----------- 设置GPIOx_PDR寄存器 -----------*/
            /* 根据结构体成员m_PullDown，设置PXPDR位域 */
            tmpreg = (((uint32_t)gpioInitStruct->m_PullDown) << (pinpos));
            GPIOx->PDR = SFR_Config (GPIOx->PDR,
                                  ~(GPIO_PDR_PXPDR0 << pinpos),
                                  tmpreg);
        }
        else
        {
            ;
        }
    }
}

/**
  * 描述  初始化GPIO配置信息结构体。
  * 输入  gpioInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
GPIO_Struct_Init (GPIO_InitTypeDef* gpioInitStruct)
{
    /* 初始化 GPIO端口掩码 */
    gpioInitStruct->m_Pin  = GPIO_PIN_MASK_All;

    /* 初始化 GPIO输出模式 */
    gpioInitStruct->m_Mode = GPIO_MODE_AN;

    /* 初始化 GPIO输出速度 */
    gpioInitStruct->m_Speed = GPIO_LOW_SPEED;

    /* 初始化 GPIO开漏控制 */
    gpioInitStruct->m_OpenDrain = GPIO_POD_PP;

    /* 初始化 GPIO上拉配置 */
    gpioInitStruct->m_PullUp = GPIO_NOPULL;

    /* 初始化 GPIO下拉配置 */
    gpioInitStruct->m_PullDown = GPIO_NOPULL;
}

/**
  * 描述  配置GPIO端口锁定。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       NewState: GPIO端口锁定状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
GPIO_Pin_Lock_Config (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, FunctionalState NewState)
{
    uint32_t tmpreg = 0x5C5A0000;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /* 保存原始锁定状态 */
    tmpreg |= GPIOx->LOCK;

    /* 配置锁定/解锁状态 */
    if (NewState != FALSE)
    {
        /* 锁定 */
        tmpreg |= (uint32_t)GpioPin;
    }
    else
    {
        /* 解锁 */
        tmpreg &= ~((uint32_t)GpioPin);
    }

    /*----------- 设置GPIOx_LOCK寄存器 -----------*/
    GPIOx->LOCK = tmpreg;
}

/**
  * 描述  配置GPIO端口上拉使能。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       NewState: GPIO端口上拉使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
GPIO_Pull_Up_Enable (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*----------- 设置GPIOx_PUR寄存器 -----------*/
    if (NewState != FALSE)
    {
        /* 使能上拉功能 */
        GPIOx->PUR |= (uint32_t)GpioPin;
    }
    else
    {
        /* 禁止上拉功能 */
        GPIOx->PUR &= ~((uint32_t)GpioPin);
    }
}

/**
  * 描述  配置GPIO端口上拉使能取反。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  * 返回  无。
  */
void
GPIO_Toggle_Pull_Up_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));

    /*----------- 设置GPIOx_PUR寄存器 -----------*/
    GPIOx->PUR ^= (uint32_t)GpioPin;
}

/**
  * 描述  配置GPIO端口下拉使能。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       NewState: GPIO端口下拉使能状态，取值范围为TRUE 或 FALSE。
  * 返回  无。
  */
void
GPIO_Pull_Down_Enable (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*----------- 设置GPIOx_PDR寄存器 -----------*/
    if (NewState != FALSE)
    {
        /* 使能下拉功能 */
        GPIOx->PDR |= (uint32_t)GpioPin;
    }
    else
    {
        /* 禁止下拉功能 */
        GPIOx->PDR &= ~((uint32_t)GpioPin);
    }
}

/**
  * 描述  配置GPIO端口下拉使能取反。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  * 返回  无。
  */
void
GPIO_Toggle_Pull_Down_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));

    /*----------- 设置GPIOx_PUR寄存器 -----------*/
    GPIOx->PDR ^= (uint32_t)GpioPin;
}

/**
  * 描述  配置GPIO端口开漏输出控制。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       NewState: GPIO开漏输出控制状态，取值为：
  *                   GPIO_POD_PP: 推挽输出
  *                   GPIO_POD_OD: 开漏输出
  * 返回  无。
  */
void
GPIO_Open_Drain_Enable (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, GPIOPOD_TypeDef NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_GPIO_POD(NewState));

    /*----------- 设置GPIOx_PODR寄存器 -----------*/
    if (NewState != GPIO_POD_PP)
    {
        /* 开漏输出 */
        GPIOx->PODR |= (uint32_t)GpioPin;
    }
    else
    {
        /* 推挽输出 */
        GPIOx->PODR &= ~((uint32_t)GpioPin);
    }
}

/**
  * 描述  配置GPIO端口开漏输出控制取反。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  * 返回  无。
  */
void
GPIO_Toggle_Open_Drain_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));

    /*----------- 设置GPIOx_PODR寄存器 -----------*/
    GPIOx->PODR ^= (uint32_t)GpioPin;
}

/**
  * 描述  配置GPIO输出模式控制。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       NewState: GPIO输出模式控制状态，取值为：
  *                   GPIO_MODE_IN: 通用IO口输入模式
  *                   GPIO_MODE_OUT: 通用IO口输出模式
  *                   GPIO_MODE_RMP: 重映射IO口功能模式
  *                   GPIO_MODE_AN: 模拟模式
  * 返回  无。
  */
void
GPIO_Write_Mode_Bits (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, GPIOMode_TypeDef NewState)
{
    uint32_t pinpos = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_GPIO_MODE(NewState));

    /*----------- 获取GPIOx_PMOD寄存器 -----------*/
    tmpreg = GPIOx->PMOD;
    while (GpioPin)
    {
        if (GpioPin & (uint16_t)0x1)
        {
            /* 记录配置信息 */
            tmpreg &= ~(GPIO_PMOD_PXPMD0 << pinpos);
            tmpreg |= (uint32_t)NewState << pinpos;
        }
        else
        {
            ;
        }
        GpioPin >>= 1;
        pinpos += 2;
    }

    /*----------- 配置GPIOx_PMOD寄存器 -----------*/
    GPIOx->PMOD = tmpreg;
}

/**
  * 描述  配置GPIO端口输出速度控制。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       NewState: GPIO端口输出速度控制状态，取值为：
  *                   GPIO_LOW_SPEED: 10MHZ
  *                   GPIO_HIGH_SPEED: 50MHZ
  * 返回  无。
  */
void
GPIO_Speed_Config (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, GPIOSpeed_TypeDef NewState)
{
    uint32_t pinpos = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_GPIO_SPEED(NewState));

    /*----------- 获取GPIOx_OMOD寄存器 -----------*/
    tmpreg = GPIOx->OMOD;
    while (GpioPin)
    {
        if (GpioPin & (uint16_t)0x1)
        {
            /* 记录配置信息 */
            tmpreg &= ~(GPIO_OMOD_PXOMD0 << pinpos);
            tmpreg |= (uint32_t)NewState << pinpos;
        }
        else
        {
            ;
        }
        GpioPin >>= 1;
        pinpos += 2;
    }

    /*----------- 配置GPIOx_PMOD寄存器 -----------*/
    GPIOx->OMOD = tmpreg;
}

/**
  * 描述  配置GPIO端口输出速度控制取反。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  * 返回  无。
  */
void
GPIO_Toggle_Speed_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    uint32_t pinpos = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));

    /*----------- 获取GPIOx_OMOD寄存器 -----------*/
    tmpreg = GPIOx->OMOD;
    while (GpioPin)
    {
        if (GpioPin & (uint16_t)0x1)
        {
            /* 取反，并记录配置信息 */
            tmpreg ^= GPIO_OMOD_PXOMD0 << pinpos;
        }
        else
        {
            ;
        }
        GpioPin >>= 1;
        pinpos += 2;
    }

    /*----------- 配置GPIOx_PMOD寄存器 -----------*/
    GPIOx->OMOD = tmpreg;
}
/**
  *   ##### 通用输入输出端口(GPIO)初始化及配置函数定义结束 #####
  */


/**
  *   ##### 通用输入输出端口(GPIO)读写函数 #####
  */
/**
  * 描述  获取GPIO端口指定引脚的输入数据。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 指定端口引脚，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的一个。
  * 返回  指定引脚的信息，1：逻辑高电平，0：逻辑低电平。
  */
BitAction
GPIO_Read_Input_Data_Bit (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));

    /*---------------- 获取GPIOx_PIR寄存器PXPIRy位 ----------------*/
    tmpreg = (uint32_t)GpioPin;
    if ((GPIOx->PIR & tmpreg) != Bit_RESET)
    {
        /* 逻辑高电平 */
        return Bit_SET;
    }
    else
    {
        /* 逻辑低电平 */
        return Bit_RESET;
    }
}

/**
  * 描述  获取GPIO端口输入数据。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  * 返回  输入数据，16位有效数据。
  */
uint16_t
GPIO_Read_Input_Data (GPIO_SFRmap* GPIOx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));

    /*---------------- 获取GPIOx_PIR寄存器 ----------------*/
    tmpreg = GPIOx->PIR;

    return (uint16_t)tmpreg;
}

/**
  * 描述  获取GPIO端口指定引脚的输出数据。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 指定端口引脚，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的一个。
  * 返回  指定引脚的信息，1：逻辑高电平，0：逻辑低电平。
  */
BitAction
GPIO_Read_Output_Data_Bit (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));

    /*---------------- 获取GPIOx_POR寄存器PXPORy位 ----------------*/
    tmpreg = (uint32_t)GpioPin;
    if ((GPIOx->POR & tmpreg) != Bit_RESET)
    {
        /* 逻辑高电平 */
        return Bit_SET;
    }
    else
    {
        /* 逻辑低电平 */
        return Bit_RESET;
    }
}

/**
  * 描述  获取GPIO端口输出数据。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  * 返回  输出数据，16位有效数据。
  */
uint16_t
GPIO_Read_Output_Data (GPIO_SFRmap* GPIOx)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));

    /*---------------- 获取GPIOx_POR寄存器 ----------------*/
    tmpreg = GPIOx->POR;

    return (uint16_t)tmpreg;
}

/**
  * 描述  配置GPIO端口指定引脚的输出数据。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  *       BitsValue: 引脚的配置值，取值为：
  *                    Bit_RESET: 逻辑低电平
  *                    Bit_SET: 逻辑高电平
  * 返回  无。
  */
void
GPIO_Set_Output_Data_Bits (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, BitAction BitsValue)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_MASK(GpioPin));
    CHECK_RESTRICTION(CHECK_GPIO_BIT_ACTION(BitsValue));

    /*----------- 设置GPIOx_POR寄存器 -----------*/
    if (BitsValue != Bit_RESET)
    {
        /* 逻辑高电平 */
        GPIOx->POR |= (uint32_t)GpioPin;
    }
    else
    {
        /* 逻辑低电平 */
        GPIOx->POR &= ~((uint32_t)GpioPin);
    }
}

/**
  * 描述  配置GPIO端口指定引脚的输出数据取反。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPin: 端口引脚掩码，取值为GPIO_PIN_MASK_0~GPIO_PIN_MASK_15中的
  *                一个或多个组合。
  * 返回  无。
  */
void
GPIO_Toggle_Output_Data_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));

    /*----------- 设置GPIOx_POR寄存器 -----------*/
    GPIOx->POR ^= (uint32_t)GpioPin;
}
/**
  *   ##### 通用输入输出端口(GPIO)读写函数定义结束 #####
  */


/**
  *   ##### 通用输入输出端口(GPIO)重映射配置函数 #####
  */
/**
  * 描述  配置GPIO端口引脚重映射。
  * 输入  GPIOx: 指向GPIO内存结构的指针，取值为GPIOA_SFR~GPIOH_SFR。
  *       GpioPinNum: 指定端口引脚，取值为GPIO_Pin_Num_0~GPIO_Pin_Num_15中的一个。
  *       PinRemap: 引脚重映射选择，取值为：
  *                   GPIO_RMP_AF0_SYSTEM: SYSTEM
  *                   GPIO_RMP_AF1_T0: T0
  *                   GPIO_RMP_AF1_T1: T1
  *                   GPIO_RMP_AF1_T2: T2
  *                   GPIO_RMP_AF1_T3: T3
  *                   GPIO_RMP_AF1_T4: T4
  *                   GPIO_RMP_AF2_T5: T5
  *                   GPIO_RMP_AF2_T6: T6
  *                   GPIO_RMP_AF2_T9: T9
  *                   GPIO_RMP_AF2_T10: T10
  *                   GPIO_RMP_AF3_T9: T9
  *                   GPIO_RMP_AF3_T20: T20
  *                   GPIO_RMP_AF3_T21: T21
  *                   GPIO_RMP_AF3_T23: T23
  *                   GPIO_RMP_AF3_QEI1: QEI1
  *                   GPIO_RMP_AF4_T: T9
  *                   GPIO_RMP_AF4_T14: T14
  *                   GPIO_RMP_AF4_T15: T15
  *                   GPIO_RMP_AF4_T18: T18
  *                   GPIO_RMP_AF4_T19: T19
  *                   GPIO_RMP_AF4_T22: T22
  *                   GPIO_RMP_AF4_QEI0: QEI0
  *                   GPIO_RMP_AF5_USART0: USART0
  *                   GPIO_RMP_AF5_USART1: USART1
  *                   GPIO_RMP_AF5_USART2: USART2
  *                   GPIO_RMP_AF6_USART3: USART3
  *                   GPIO_RMP_AF6_USART4: USART4
  *                   GPIO_RMP_AF6_USART5: USART5
  *                   GPIO_RMP_AF6_USART6: USART6
  *                   GPIO_RMP_AF6_USART7: USART7
  *                   GPIO_RMP_AF7_SPI0: SPI0
  *                   GPIO_RMP_AF7_SPI1: SPI1
  *                   GPIO_RMP_AF7_SPI2: SPI2
  *                   GPIO_RMP_AF7_SPI3: SPI3
  *                   GPIO_RMP_AF8_I2C0: I2C0
  *                   GPIO_RMP_AF8_I2C1: I2C1
  *                   GPIO_RMP_AF8_I2C2: I2C2
  *                   GPIO_RMP_AF8_I2C3: I2C3
  *                   GPIO_RMP_AF9_CAN0: CAN0
  *                   GPIO_RMP_AF9_CAN1: CAN1
  *                   GPIO_RMP_AF9_CAN2: CAN2
  *                   GPIO_RMP_AF9_CAN3: CAN3
  *                   GPIO_RMP_AF9_CAN4: CAN4
  *                   GPIO_RMP_AF9_CAN5: CAN5
  *                   GPIO_RMP_AF9_FLT: FLT
  *                   GPIO_RMP_AF10_CCP: CCP
  *                   GPIO_RMP_AF10_SPI: SPI
  *                   GPIO_RMP_AF10_I2C: I2C
  *                   GPIO_RMP_AF11_USART: USART
  *                   GPIO_RMP_AF11_CCP: CCP
  *                   GPIO_RMP_AF11_SPI: SPI
  *                   GPIO_RMP_AF11_I2C: I2C
  *                   GPIO_RMP_AF12_LED: LED
  *                   GPIO_RMP_AF12_CFGL: CFGL
  *                   GPIO_RMP_AF13_EXIC: EXIC
  *                   GPIO_RMP_AF14_LED: LED
  *                   GPIO_RMP_AF15_TESTPAD: TESTPAD
  *
  * 返回  无。
  */
void
GPIO_Pin_RMP_Config (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPinNum, uint8_t PinRemap)
{
    uint32_t tmpreg = 0x00;
    uint32_t tmpreg1 = 0x00;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_GPIO_ALL_PERIPH(GPIOx));
    CHECK_RESTRICTION(CHECK_GPIO_PIN_NUM(GpioPinNum));
    CHECK_RESTRICTION(CHECK_GPIO_RMP(PinRemap));

    /* 获取原始GPIO_RMPH/L数据 */
    tmpreg1 = GPIOx->RMP[GpioPinNum >> 0x03];

    /* 修改对应的数值 */
    tmpreg = ((uint32_t)PinRemap << (((uint32_t)GpioPinNum & 0x07) * 4));
    tmpreg1 &= ~((uint32_t)0xF << (((uint32_t)GpioPinNum & 0x07) * 4));
    tmpreg1 = tmpreg1 | tmpreg;

    /* 配置GPIO_RMPH/L数据 */
    GPIOx->RMP[GpioPinNum >> 0x03] = tmpreg1;
}
/**
  *   ##### 通用输入输出端口(GPIO)重映射配置函数定义结束 #####
  */
