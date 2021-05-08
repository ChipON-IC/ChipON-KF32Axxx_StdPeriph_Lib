/**
  ******************************************************************************
  * 文件名  kf32a_basic_usb.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了USB模块(USB)相关的功能函数，包含：
  *          + USB模块(USB)初始化函数
  *          + USB模块(USB)功能配置函数
  *          + USB模块(USB)缓冲区函数
  *          + USB模块(USB)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_usb.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"
#include "string.h"


#ifdef KF32A_Periph_usb
/* 定时器私有定义 ----------------------------------------------------*/
/* USB_UCON寄存器初始化掩码 */
#define USB_UCON_INIT_MASK              (USB_UCON_SPEEDEN \
                                       | USB_UCON_UPUEN \
                                       | USB_UCON_DB)

/* USB_UEP寄存器初始化掩码 */
#define USB_UEP_INIT_MASK               (USB_UEP_EPHSHK \
                                       | USB_UEP_EPCONDIS \
                                       | USB_UEP_EPOUTEN \
                                       | USB_UEP_EPINEN)

/* USB_UEP寄存器初始化掩码 */
#define USB_BDSTAT_INIT_MASK            (USB_BDSTAT_BADDR \
                                       | USB_BDSTAT_BC \
                                       | USB_BDSTAT_UOWN \
                                       | USB_BDSTAT_DTS \
                                       | USB_BDSTAT_DTSEN \
                                       | USB_BDSTAT_BSTALL)

/**
  *   ##### USB模块(USB)初始化函数 #####
  */
/**
  * 描述  复位USB外设，使能外设时钟。
  * 输入  无。
  * 返回  无。
  */
void
USB_Reset(void)
{
    /* 复位USB外设 */
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USBRST, TRUE);
    RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_USBRST, FALSE);

    /* 使能USB外设时钟 */
    PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_USBCLKEN, TRUE);
}

/**
  * 描述  USB模块(USB)初始化。
  * 输入  usbInitStruct: USB配置信息结构体。
  * 返回  无。
  */
void
USB_Configuration(USB_InitTypeDef* usbInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t i = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_SPEED_SELECT(usbInitStruct->m_Speed));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Pullup));
    CHECK_RESTRICTION(CHECK_USB_DOUBLE_BUFFER(usbInitStruct->m_DoubleBufferCfg));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_DoubleBufferRst));
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_MASK(usbInitStruct->m_EndPointMask));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Handshake));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Bidirectional));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Output));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_Input));

    /*---------------- 配置USB_UCON寄存器 ----------------*/
    /* 根据结构体成员m_Speed，设置SPEEDEN位域 */
    /* 根据结构体成员m_Pullup，设置UPUEN位域 */
    /* 根据结构体成员m_DoubleBufferCfg，设置DB位域 */
    tmpreg = usbInitStruct->m_Speed \
           | ((uint32_t)usbInitStruct->m_Pullup << USB_UCON_UPUEN_POS) \
           | usbInitStruct->m_DoubleBufferCfg
           | ((uint32_t)usbInitStruct->m_DoubleBufferRst << USB_UCON_DBRST_POS);
    USB_UCON = SFR_Config (USB_UCON, ~USB_UCON_INIT_MASK, tmpreg);

    if (usbInitStruct->m_DoubleBufferRst != FALSE)
    {
        /* 双缓冲区复位清零 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_DBRST_POS);
    }
    else
    {
        ;
    }

    /*---------------- 配置USB_UEPn寄存器 ----------------*/
    /* 根据结构体成员m_EndPointMask，选择USB_UEPn寄存器 */
    /* 根据结构体成员m_Handshake，设置EPHSHK位域 */
    /* 根据结构体成员m_Bidirectional，设置EPCONDIS位域 */
    /* 根据结构体成员m_Output，设置EPOUTEN位域 */
    /* 根据结构体成员m_Input，设置EPINEN位域 */
    tmpreg = ((uint32_t)usbInitStruct->m_Handshake << USB_UEP_EPHSHK_POS) \
           | ((uint32_t)usbInitStruct->m_Bidirectional << USB_UEP_EPCONDIS_POS) \
           | ((uint32_t)usbInitStruct->m_Output << USB_UEP_EPOUTEN_POS) \
           | ((uint32_t)usbInitStruct->m_Input << USB_UEP_EPINEN_POS);
    tmpreg1 = usbInitStruct->m_EndPointMask;
    for (i = 0; i <= USB_ENDPOINT_7; i++)
    {
        if (tmpreg1 & 0x1)
        {
            USB_SFR->UEP[i] = SFR_Config (USB_SFR->UEP[i],
                                    ~USB_UEP_INIT_MASK,
                                    tmpreg);
        }
        else
        {
            ;
        }
        tmpreg1 >>= 1;
    }
}

/**
  * 描述  USB模块(USB)缓冲区初始化。
  * 输入  usbInitStruct: USB缓冲区信息结构体。
  * 返回  无。
  */
void
USB_Buffer_Configuration(USB_BufferTypeDef* usbInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(usbInitStruct->m_EndPoint));
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_DIRECT(usbInitStruct->m_Direction));
    CHECK_RESTRICTION(CHECK_USB_ODD_EVEN(usbInitStruct->m_BDPointer));
    CHECK_RESTRICTION(CHECK_USB_DATA_LENGTH(usbInitStruct->m_ByteCount));
    CHECK_RESTRICTION(CHECK_USB_OWN_BD(usbInitStruct->m_Own));
    CHECK_RESTRICTION(CHECK_USB_DATA_TOGGLE_SYNC(usbInitStruct->m_DataToggleSync));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_DataToggleSyncEn));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(usbInitStruct->m_BufferStall));

    /* 获取缓冲区配置地址及内存 */
    tmpaddr = USB_Get_Endpoint_Double_Buffer_Addr (usbInitStruct->m_EndPoint, \
                                usbInitStruct->m_Direction,
                                usbInitStruct->m_BDPointer);
    tmpreg = *(volatile uint32_t *) tmpaddr;

    /*---------------- 配置USB_BDnSTAT寄存器 ----------------*/
    /* 根据结构体成员m_BufferAddr，设置BADDR位域 */
    /* 根据结构体成员m_ByteCount，设置BC位域 */
    /* 根据结构体成员m_Own，设置UOWN位域 */
    /* 根据结构体成员m_DataToggleSync，设置DTS位域 */
    /* 根据结构体成员m_DataToggleSyncEn，设置DTSEN位域 */
    /* 根据结构体成员m_BufferStall，设置BSTALL位域 */

    /* 获取BC位的高两位 */
    tmpreg &= USB_BDSTAT_BCH;
    tmpreg >>= USB_BDSTAT_BC8_POS;
    /* 获取BC位的低两位 */
    tmpreg1 &= USB_BDSTAT_BCL;
    tmpreg1 >>= USB_BDSTAT_BC0_POS;
    /* 组合BC的BIT位置 */
    tmpreg = (tmpreg << (USB_BDSTAT_BC7_POS + 1)) | (tmpreg1);

    /* USB_BDnSTAT寄存器配置信息 */
    tmpreg = ((uint32_t)usbInitStruct->m_BufferAddr << USB_BDSTAT_BADDR0_POS) \
           | tmpreg \
           | (usbInitStruct->m_Own << USB_BDSTAT_UOWN_POS) \
           | usbInitStruct->m_DataToggleSync \
           | ((uint32_t)usbInitStruct->m_DataToggleSyncEn << USB_BDSTAT_DTSEN_POS) \
           | ((uint32_t)usbInitStruct->m_BufferStall << USB_BDSTAT_BSTALL_POS);

    *(volatile uint32_t *) tmpaddr =
                    SFR_Config (*(volatile uint32_t *) tmpaddr,
                            ~USB_BDSTAT_INIT_MASK,
                            tmpreg);
}

/**
  * 描述  初始化USB配置信息结构体。
  * 输入  usbInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
USB_Struct_Init (USB_InitTypeDef* usbInitStruct)
{
    /* 初始化 USB速度 */
    usbInitStruct->m_Speed = USB_LOW_SPEED;

    /* 初始化 USB片上上拉使能 */
    usbInitStruct->m_Pullup = FALSE;

    /* 初始化 USB双缓冲区配置 */
    usbInitStruct->m_DoubleBufferCfg = USB_DOUBLE_BUFFER_FORBID;

    /* 初始化 USB端点编号掩码 */
    usbInitStruct->m_EndPointMask = USB_ENDPOINT_MASK_NONE;

    /* 初始化 USB端点握手使能 */
    usbInitStruct->m_Handshake = FALSE;

    /* 初始化 双向端点控制使能 */
    usbInitStruct->m_Bidirectional = FALSE;

    /* 初始化 USB端点输出使能 */
    usbInitStruct->m_Output = FALSE;

    /* 初始化 USB端点输入使能 */
    usbInitStruct->m_Input = FALSE;
}

/**
  * 描述  初始化USB缓冲区信息结构体。
  * 输入  usbInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
USB_Buffer_Struct_Init (USB_BufferTypeDef* usbInitStruct)
{
    /* 初始化 USB端点编号 */
    usbInitStruct->m_EndPoint = USB_ENDPOINT_0;

    /* 初始化 USB端点方向 */
    usbInitStruct->m_Direction = USB_ENDPOINT_OUTPUT;

    /* 初始化 USB端点奇偶BD存储区 */
    usbInitStruct->m_BDPointer = USB_EVEN_BUFFER_DESCRIPOR;

    /* 初始化 RAM缓存地址 */
    usbInitStruct->m_BufferAddr = 0;

    /* 初始化 RAM字节计数 */
    usbInitStruct->m_ByteCount = 0;

    /* 初始化 USB 拥有位 */
    usbInitStruct->m_Own = USB_KERNEL_OWN_BD;

    /* 初始化 USB数据翻转同步 */
    usbInitStruct->m_DataToggleSync = USB_DATA_0_TOGGLE_SYNC;

    /* 初始化 USB数据翻转同步使能 */
    usbInitStruct->m_DataToggleSyncEn = FALSE;

    /* 初始化 USB缓冲区停止使能 */
    usbInitStruct->m_BufferStall = FALSE;
}
/**
  *   ##### USB模块(USB)初始化函数定义结束 #####
  */


/**
  *   ##### USB模块(USB)功能配置函数 #####
  */
/**
  * 描述  配置USB双缓冲区复位使能。
  * 输入  NewState: USB双缓冲区复位使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Double_Buffer_Pointer_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UCON寄存器DBRST位 ----------------*/
    if (NewState != FALSE)
    {
        /* 将所有双缓冲区指针复位为偶数缓冲区描述符BD存储区 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_DBRST_POS);
    }
    else
    {
        /* 不复位双缓冲区指针 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_DBRST_POS);
    }
}

/**
  * 描述  获取有效单端0电平标志。
  * 输入  无。
  * 返回  1:USB总线上的单端0电平有效，0:未检测到单端0电平。
  */
FlagStatus
USB_Get_Single_Ended_0_Flag (void)
{
    /*---------------- 读取USB_UCON寄存器SE0位 ----------------*/
    if (USB_UCON_SE0 & USB_UCON)
    {
        /* USB总线上的单端0电平有效 */
        return SET;
    }
    else
    {
        /* 未检测到单端0电平 */
        return RESET;
    }
}

/**
  * 描述  清零有效单端0电平标志。
  * 输入  无。
  * 返回  无。
  */
void
USB_Clear_Single_Ended_0_Flag (void)
{
    /*---------------- 清零USB_UCON寄存器SE0位 ----------------*/
	while(USB_UCON_SE0 & USB_UCON)
	{
		SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_SE0_POS);
	}
}

/**
  * 描述  获取数据包传送标志。
  * 输入  无。
  * 返回  1: 已禁止SIE令牌和数据包处理，收到SETUP令牌；
  *       0: 允许SIE令牌和数据包处理。
  */
FlagStatus
USB_Get_Package_Transmit_Flag (void)
{
    /*---------------- 读取USB_UCON寄存器PKTDIS位 ----------------*/
    if (USB_UCON_PKTDIS & USB_UCON)
    {
        /* 已禁止SIE令牌和数据包处理，收到SETUP令牌 */
        return SET;
    }
    else
    {
        /* 允许SIE令牌和数据包处理 */
        return RESET;
    }
}

/**
  * 描述  清零数据包传送标志。
  * 输入  无。
  * 返回  无。
  */
void
USB_Clear_Package_Transmit_Flag (void)
{
    /*---------------- 清零USB_UCON寄存器PKTDIS位 ----------------*/
	while(USB_UCON_PKTDIS & USB_UCON)
	{
		SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_PKTDIS_POS);
	}
}

/**
  * 描述  配置USB暂停(挂起)使能。
  * 输入  NewState: USB暂停(挂起)使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Suspend_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UCON寄存器SUSPND位 ----------------*/
    if (NewState != FALSE)
    {
        /* USB模块和支持电路处于休眠模式，SIE时钟不工作 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_SUSPND_POS);
    }
    else
    {
        /* USB模块和支持电路处于正常工作状态，SIE时钟以配置的速率运行 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_SUSPND_POS);
    }
}

/**
  * 描述  配置USB恢复信号传输使能。
  * 输入  NewState: USB恢复信号传输使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Resume_Signal_Transmit_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UCON寄存器RESUME位 ----------------*/
    if (NewState != FALSE)
    {
        /* 激活恢复信号传输 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_RESUME_POS);
    }
    else
    {
        /* 禁止恢复信号传输 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_RESUME_POS);
    }
}

/**
  * 描述  配置USB模块使能。
  * 输入  NewState: USB模块使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Cmd (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UCON寄存器USBEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能USB模块和支持电路（连接设备） */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_USBEN_POS);
    }
    else
    {
        /* 禁止USB模块和支持电路（断开设备） */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_USBEN_POS);
    }
}

/**
  * 描述  配置USB眼图测试使能。
  * 输入  NewState: USB眼图测试使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Eye_Test_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UCON寄存器EYETEST位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能自动眼图测试 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_EYETEST_POS);
    }
    else
    {
        /* 禁止自动眼图测试 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_EYETEST_POS);
    }
}

/**
  * 描述  配置USB速度选择。
  * 输入  SpeedSelect: USB速度选择，取值为:
  *                      USB_LOW_SPEED: 低速设备
  *                      USB_FULL_SPEED: 全速设备
  * 返回  无。
  */
void
USB_Speed_Config (uint32_t SpeedSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_SPEED_SELECT(SpeedSelect));

    /*---------------- 配置USB_UCON寄存器SPEEDEN位 ----------------*/
    if (SpeedSelect != USB_LOW_SPEED)
    {
        /* 全速设备 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_SPEEDEN_POS);
    }
    else
    {
        /* 低速设备 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_SPEEDEN_POS);
    }
}

/**
  * 描述  配置USB片上上拉使能。
  * 输入  NewState: USB片上上拉使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_On_Chip_Pull_Up_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UCON寄存器UPUEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能片上上拉 */
        SFR_SET_BIT_ASM(USB_UCON, USB_UCON_UPUEN_POS);
    }
    else
    {
        /* 禁止片上上拉 */
        SFR_CLR_BIT_ASM(USB_UCON, USB_UCON_UPUEN_POS);
    }
}

/**
  * 描述  配置USB双缓冲区使能。
  * 输入  DoubleBuffer: USB双缓冲区使能状态，取值为:
  *                       USB_DOUBLE_BUFFER_FORBID: 禁止偶数/奇数双缓冲区
  *                       USB_OUT_ENDPOINT_0: 使能OUT端点0的偶数/奇数双缓冲区
  *                       USB_ALL_ENDPOINTS: 使能所有端点的偶数/奇数双缓冲区
  *                       USB_ENDPOINT_1_TO_7: 使能端点1至7的偶数/奇数双缓冲区
  * 返回  无。
  */
void
USB_Double_Buffer_Config (uint32_t DoubleBuffer)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_DOUBLE_BUFFER(DoubleBuffer));

    /*---------------- 配置USB_UCON寄存器DB位 ----------------*/
    USB_UCON = SFR_Config (USB_UCON, ~USB_UCON_DB, DoubleBuffer);
}

/**
  * 描述  获取USB状态信息。
  * 输入  usbStateStruct: USB状态信息结构体，记录SIE中的事物状态。
  * 返回  无。
  */
void
USB_Get_Arrangement_State (USB_StateTypeDef* usbStateStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(usbStateStruct != (USB_StateTypeDef*)0);

    /*---------------- 读取USB_USTAT寄存器 ----------------*/
    tmpreg = USB_USTAT;

    /*---------------- 获取USB_USTAT寄存器PEP位 ----------------*/
    /* 记录m_EndPoint和m_EndPointDirection */
    tmpreg1 = tmpreg & USB_USTAT_PEP;
    tmpreg1 >>= USB_USTAT_PEP0_POS;
    usbStateStruct->m_EndPoint = tmpreg1;

    /*---------------- 获取USB_USTAT寄存器PDIR位 ----------------*/
    tmpreg1 = tmpreg & USB_USTAT_PDIR;
    tmpreg1 >>= USB_USTAT_PDIR_POS;
    usbStateStruct->m_BDDirection = tmpreg1;

    /*---------------- 获取USB_USTAT寄存器DBI位 ----------------*/
    tmpreg1 = tmpreg & USB_USTAT_DBI;
    tmpreg1 >>= USB_USTAT_DBI_POS;
    usbStateStruct->m_BDPointer = tmpreg1;
}

/**
  * 描述  配置USB地址。
  * 输入  Address: USB地址，取值为0~0x7F。
  * 返回  无。
  */
void
USB_Address_Config (uint32_t Address)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ADDRESS(Address));

    /*---------------- 配置USB_UADDR寄存器UADDR位 ----------------*/
    tmpreg = Address << USB_UADDR_UADDR0_POS;
    USB_UADDR = SFR_Config (USB_UADDR, ~USB_UADDR_UADDR, tmpreg);
}

/**
  * 描述  获取USB地址。
  * 输入  无。
  * 返回  USB地址，取值为0~0x7F。
  */
uint32_t
USB_Get_Address (void)
{
    uint32_t tmpreg = 0;

    /*------------- 读取USB_UADDR寄存器UADDR位 -------------*/
    tmpreg = USB_UADDR;
    tmpreg &= USB_UADDR_UADDR;
    tmpreg >>= USB_UADDR_UADDR0_POS;

    return tmpreg;
}

/**
  * 描述  获取USB帧编号。
  * 输入  无。
  * 返回  USB帧编号，取值为0~0x7FF。
  */
uint32_t
USB_Get_Frame_Num (void)
{
    uint32_t tmpreg = 0;

    /*------------- 读取USB_UFRM寄存器UFRM位 -------------*/
    tmpreg = USB_UFRM;
    tmpreg &= USB_UFRM_UFRM;
    tmpreg >>= USB_UFRM_UFRM0_POS;

    return tmpreg;
}

/**
  * 描述  配置USB端点握手使能。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  *       NewState: USB端点握手使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Endpoint_Handshake_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UEP寄存器EPHSHK位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能端点握手 */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPHSHK_POS);
    }
    else
    {
        /* 禁止端点握手 */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPHSHK_POS);
    }
}

/**
  * 描述  配置USB双向端点控制使能。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  *       NewState: USB双向端点控制使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Bidirectional_Endpoint_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UEP寄存器EPCONDIS位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能端点控制（SETUP）传送；同时允许IN 和OUT 传送 */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPCONDIS_POS);
    }
    else
    {
        /* 禁止端点控制传送；仅允许IN 和OUT 传送 */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPCONDIS_POS);
    }
}

/**
  * 描述  配置USB端点输出使能。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  *       NewState: USB端点输出使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Endpoint_Output_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UEP寄存器EPOUTEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能端点输出 */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPOUTEN_POS);
    }
    else
    {
        /* 禁止端点输出 */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPOUTEN_POS);
    }
}

/**
  * 描述  配置USB端点输入使能。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  *       NewState: USB端点输入使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Endpoint_Input_Enable (uint32_t EndPoint, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UEP寄存器EPINEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能端点输入 */
        SFR_SET_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPINEN_POS);
    }
    else
    {
        /* 禁止端点输入 */
        SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPINEN_POS);
    }
}

/**
  * 描述  获取USB端点STALL指示标志。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  * 返回  USB端点STALL指示标志，1：端点上发出STALL握手包，2：端点上未发出
  *       STALL握手包。
  */
FlagStatus
USB_Get_Endpoint_STALL_Flag (uint32_t EndPoint)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));

    /*---------------- 读取USB_UEP寄存器EPSTL位 ----------------*/
    if (USB_SFR->UEP[EndPoint] & USB_UEP_EPSTL)
    {
        /* 端点上发出STALL握手包 */
        return SET;
    }
    else
    {
        /* 端点上未发出STALL握手包 */
        return RESET;
    }
}

/**
  * 描述  清零USB端点STALL指示标志。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  * 返回  无。
  */
void
USB_Clear_Endpoint_STALL_Flag (uint32_t EndPoint)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));

    /*---------------- 清零USB_UEP寄存器EPSTL位 ----------------*/
    while(USB_SFR->UEP[EndPoint] & USB_UEP_EPSTL)
    {
    	SFR_CLR_BIT_ASM(USB_SFR->UEP[EndPoint], USB_UEP_EPSTL_POS);
    }
}

/**
  * 描述  配置USB Debug功能使能。
  * 输入  NewState: USB Debug功能使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Debug_Signal_Output_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UDB寄存器DEBUGEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能Debug信号输出 */
        SFR_SET_BIT_ASM(USB_UDB, USB_UDB_DEBUGEN_POS);
    }
    else
    {
        /* 禁止Debug信号输出 */
        SFR_CLR_BIT_ASM(USB_UDB, USB_UDB_DEBUGEN_POS);
    }
}

/**
  * 描述  配置USB Debug输出选择。
  * 输入  DebugCtl: USB Debug输出选择，取值为:
  *                   USB_DBG_DATA_OUTPUT_AND_CLOCK: 数字输出正端、数字输出负端、数字内部时钟
  *                   USB_DBG_RECEIVE_AND_CLOCK: 数字模块处理后接收数据、模拟模块接收到的数据、数字模块同步时钟
  *                   USB_DBG_DATA_INPUT_AND_CLOCK: 数字输入正端、数字输入负端、发送使能
  *                   USB_DBG_RAM_DATA: RAM数据
  *                   USB_DBG_USB_STATE_MACHINE: USB状态机
  *                   USB_DBG_RAM_STATE_MACHINE: USB RAM状态机
  *                   USB_DBG_RAM_ADDRESS: RAM地址
  *                   USB_DBG_OPERATIONAL_CONTROL: CRC5运行控制、CRC16运行控制、USB接收控制
  * 返回  无。
  */
void
USB_Debug_Output_Config (uint32_t DebugCtl)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_DBG_CONFIG(DebugCtl));

    /*---------------- 配置USB_UDB寄存器DEBUG位 ----------------*/
    USB_UDB = SFR_Config (USB_UDB, ~USB_UDB_DEBUG, DebugCtl);
}
/**
  *   ##### USB模块(USB)功能配置函数定义结束 #####
  */


/**
  *   ##### USB模块(USB)缓冲区函数 #####
  */
/**
  * 描述  获取USB端点缓冲区描述符RAM地址，根据USB_UCON寄存器的DB位选择相应的参数
  *       计算对应的缓冲区。若无奇偶描述符，则参数BufferDescripor无效。
  * 输入  EndPoint: USB端点编号，取值为USB_ENDPOINT_0~USB_ENDPOINT_8。
  *       Direction: USB端点方向，取值为：
  *                    USB_ENDPOINT_OUTPUT: 端点输出
  *                    USB_ENDPOINT_INPUT: 端点输入
  *       BufferDescripor: USB奇偶BD存储区，取值为：
  *                          USB_ODD_BUFFER_DESCRIPOR: 奇数描述符
  *                          USB_EVEN_BUFFER_DESCRIPOR: 偶数描述符
  * 返回  USB端点缓冲区描述符32位RAM地址，不存在则返回0。
  */
uint32_t
USB_Get_Endpoint_Double_Buffer_Addr (uint32_t EndPoint,
                    uint32_t Direction, uint32_t BufferDescripor)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_NUM(EndPoint));
    CHECK_RESTRICTION(CHECK_USB_ENDPOINT_DIRECT(Direction));
    CHECK_RESTRICTION(CHECK_USB_ODD_EVEN(BufferDescripor));

    /*------------- 读取USB_UCON寄存器DB位 -------------*/
    tmpreg = USB_UCON;
    tmpreg &= USB_UCON_DB;

    /* 端点缓冲区的工作模式匹配 */
    switch (tmpreg)
    {
        /* 禁止偶数/奇数双缓冲区 */
        case USB_DOUBLE_BUFFER_FORBID:
        {
            /* 端点0到7的双缓冲区描述符 */
            tmpaddr = USB_BUFFER_START_ADDR;
            tmpaddr += EndPoint * 8;
            tmpaddr += Direction * 4;
        }
            break;
        /* 使能OUT端点0的偶数/奇数双缓冲区 */
        case USB_OUT_ENDPOINT_0:
        {
            if (EndPoint != USB_ENDPOINT_0)
            {
                /* 端点1到7的双缓冲区描述符 */
                tmpaddr = USB_BUFFER_START_ADDR + (4 * 3);
                tmpaddr += (EndPoint - 1) * 8;
                tmpaddr += Direction * 4;
            }
            else
            {
                /* 端点0的双缓冲区描述符 */
                if (Direction != USB_ENDPOINT_INPUT)
                {
                    /* 端点0的输出双缓冲区描述符 */
                    tmpaddr = USB_BUFFER_START_ADDR;
                    tmpaddr += (uint32_t)(!BufferDescripor) * 4;
                }
                else
                {
                    /* 端点0的输入双缓冲区描述符 */
                    tmpaddr = USB_BUFFER_START_ADDR + (4 * 2);
                }
            }
        }
            break;
        /* 使能所有端点的偶数/奇数双缓冲区 */
        case USB_ALL_ENDPOINTS:
        {
            tmpaddr = USB_BUFFER_START_ADDR;
            tmpaddr += EndPoint * 4 * 4;
            tmpaddr += Direction * 4 * 2;
            tmpaddr += (uint32_t)(!BufferDescripor) * 4;
        }
            break;
        /* 使能端点1至7的偶数/奇数双缓冲区 */
        case USB_ENDPOINT_1_TO_7:
        {
            if (EndPoint != USB_ENDPOINT_0)
            {
                /* 端点1到7的双缓冲区描述符 */
                tmpaddr = USB_BUFFER_START_ADDR + 4 * 2;
                tmpaddr += (EndPoint - 1) * 4 * 4;
                tmpaddr += Direction * 4 * 2;
                tmpaddr += (uint32_t)(!BufferDescripor) * 4;
            }
            else
            {
                /* 端点0的双缓冲区描述符 */
                tmpaddr = USB_BUFFER_START_ADDR;
                tmpaddr += Direction * 4;
            }
        }
            break;
        default:
            CHECK_RESTRICTION(0);
            break;
    }

    return tmpaddr;
}

/**
  * 描述  配置USB RAM缓存区地址，硬件10位有效，且字对齐。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  *       BufferAddr: USB RAM缓存区地址，取值为0~0xFFFF。
  * 返回  无。
  */
void
USB_Buffer_Address_Config (uint32_t SfrAddr, uint16_t BufferAddr)
{
    uint32_t tmpaddr = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- 配置USB_BDnSTAT寄存器BADDR位 ----------------*/
    tmpaddr = SfrAddr;
    tmpreg = ((uint32_t)BufferAddr) << USB_BDSTAT_BADDR0_POS;
    *(volatile uint32_t *) tmpaddr =
                    SFR_Config (*(volatile uint32_t *) tmpaddr,
                            ~USB_BDSTAT_BADDR,
                            tmpreg);
}

/**
  * 描述  获取USB RAM缓存区地址，硬件10位有效，且字对齐。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  * 返回  USB RAM缓存区地址，字对齐，10位有效数值。
  */
uint32_t
USB_Get_Buffer_Address (uint32_t SfrAddr)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- 获取USB_BDnSTAT寄存器BADDR位 ----------------*/
    tmpreg = *(volatile uint32_t *) SfrAddr;
    tmpreg &= USB_BDSTAT_BADDR;
    tmpreg >>= USB_BDSTAT_BADDR0_POS;

    return tmpreg;
}

/**
  * 描述  配置USB RAM缓存区数据字节长度。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  *       ByteLength: USB RAM缓存区数据字节长度，取值为0~0x3FF。
  * 返回  无。
  */
void
USB_Buffer_Data_Length_Config (uint32_t SfrAddr, uint32_t ByteLength)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_USB_DATA_LENGTH(ByteLength));

    /*---------------- 配置USB_BDnSTAT寄存器BC位 ----------------*/
    tmpaddr = SfrAddr;
    tmpreg = ((ByteLength >> 8) & USB_BDSTAT_BCH) << USB_BDSTAT_BC8_POS;
    tmpreg |= (ByteLength & 0xFF) << USB_BDSTAT_BC0_POS;
    *(volatile uint32_t *) tmpaddr =
                    SFR_Config (*(volatile uint32_t *) tmpaddr,
                            ~USB_BDSTAT_BC,
                            tmpreg);
}

/**
  * 描述  获取USB缓存区字节数。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  * 返回  USB缓存区字节数，10位有效数值。
  */
uint32_t
USB_Get_Buffer_Data_Length (uint32_t SfrAddr)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- 获取USB_BDnSTAT寄存器BADDR位 ----------------*/
    tmpaddr = SfrAddr;
    tmpreg =  *(volatile uint32_t *) tmpaddr;
    tmpreg1 = *(volatile uint32_t *) tmpaddr;
    /* 获取BC位的高两位 */
    tmpreg &= USB_BDSTAT_BCH;
    tmpreg >>= USB_BDSTAT_BC8_POS;
    /* 获取BC位的低两位 */
    tmpreg1 &= USB_BDSTAT_BCL;
    tmpreg1 >>= USB_BDSTAT_BC0_POS;

    tmpreg = (tmpreg << (USB_BDSTAT_BC7_POS + 1)) | (tmpreg1);

    return tmpreg;
}

/**
  * 描述  配置USB 拥有位。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  *       OwnSelect: USB 拥有位选择，取值为:
  *                    USB_KERNEL_OWN_BD: 单片机内核拥有BD 及其相应的缓冲区
  *                    USB_SIE_OWN_BD: SIE 拥有BD 及其相应的缓冲区
  * 返回  无。
  */
void
USB_Own_Config (uint32_t SfrAddr, uint32_t OwnSelect)
{
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_USB_OWN_BD(OwnSelect));

    /*---------------- 配置USB_BDnSTAT寄存器UOWN位 ----------------*/
    tmpaddr = SfrAddr;
    if (OwnSelect != USB_KERNEL_OWN_BD)
    {
        /* SIE 拥有BD 及其相应的缓冲区 */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_UOWN_POS);
    }
    else
    {
        /* 单片机内核拥有BD 及其相应的缓冲区 */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_UOWN_POS);
    }
}

/**
  * 描述  获取USB 拥有位。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  * 返回  1: SIE 拥有BD 及其相应的缓冲区，
  *       0: 单片机内核拥有BD 及其相应的缓冲区。
  */
FlagStatus
USB_Get_Own_Status (uint32_t SfrAddr)
{
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- 读取USB_BDnSTAT寄存器UOWN位 ----------------*/
    tmpaddr = SfrAddr;
    if (USB_BDSTAT_UOWN & (*(volatile uint32_t *) tmpaddr))
    {
        /* SIE 拥有BD 及其相应的缓冲区 */
        return SET;
    }
    else
    {
        /* 单片机内核拥有BD 及其相应的缓冲区 */
        return RESET;
    }
}

/**
  * 描述  配置USB数据翻转同步位，只有单片机内核拥有BD及其相应的缓冲区，
  *       且使能数据翻转同步时，该位有效。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  *       DataPackage: USB 数据翻转同步位选择，取值为:
  *                      USB_DATA_0_TOGGLE_SYNC: 数据0 数据包
  *                      USB_DATA_1_TOGGLE_SYNC: 数据1 数据包
  * 返回  无。
  */
void
USB_Data_Toggle_Sync_Config (uint32_t SfrAddr, uint32_t DataPackage)
{
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_USB_DATA_TOGGLE_SYNC(DataPackage));

    /*---------------- 配置USB_BDnSTAT寄存器DTS位 ----------------*/
    tmpaddr = SfrAddr;
    if (DataPackage != USB_DATA_0_TOGGLE_SYNC)
    {
        /* 数据1 数据包 */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTS_POS);
    }
    else
    {
        /* 数据0 数据包 */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTS_POS);
    }
}

/**
  * 描述  获取USB数据翻转同步位，只有单片机内核拥有BD及其相应的缓冲区，
  *       且使能数据翻转同步时，该位有效。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  * 返回  1: 数据1 数据包，
  *       0: 数据0 数据包。
  */
FlagStatus
USB_Get_Data_Toggle_Sync_Status (uint32_t SfrAddr)
{
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- 读取USB_BDnSTAT寄存器DTS位 ----------------*/
    tmpaddr = SfrAddr;
    if (USB_BDSTAT_DTS & (*(volatile uint32_t *) tmpaddr))
    {
        /* 数据1 数据包 */
        return SET;
    }
    else
    {
        /* 数据0 数据包 */
        return RESET;
    }
}

/**
  * 描述  配置USB数据翻转同步使能，只有单片机内核拥有BD及其相应的缓冲区时，
  *       该功能有效。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  *       NewState: USB数据翻转同步使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Data_Toggle_Sync_Enable (uint32_t SfrAddr, FunctionalState NewState)
{
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_BDnSTAT寄存器DTSEN位 ----------------*/
    tmpaddr = SfrAddr;
    if (NewState != FALSE)
    {
        /* 使能数据翻转同步 */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTSEN_POS);
    }
    else
    {
        /* 不执行数据翻转同步 */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_DTSEN_POS);
    }
}

/**
  * 描述  配置USB缓冲区停止使能，只有单片机内核拥有BD及其相应的缓冲区时，
  *       该功能有效。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  *       NewState: USB缓冲区停止使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Buffer_Stall_Enable (uint32_t SfrAddr, FunctionalState NewState)
{
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));


    /*---------------- 配置USB_BDnSTAT寄存器BSTALL位 ----------------*/
    tmpaddr = SfrAddr;
    if (NewState != FALSE)
    {
        /* 使能缓冲区停止 */
        SFR_SET_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_BSTALL_POS);
    }
    else
    {
        /* 禁止缓冲区停止 */
        SFR_CLR_BIT_ASM(*(volatile uint32_t *) tmpaddr, USB_BDSTAT_BSTALL_POS);
    }
}

/**
  * 描述  获取USB数据包标识符，只有SIE拥有BD及其相应的缓冲区时，该功能有效。
  * 输入  SfrAddr: USB 缓存区描述符状态寄存器地址，取值为指定的32位RAM地址，
  *                可由函数USB_Get_Endpoint_Double_Buffer_Addr获取。
  * 返回  上一次传送接收到的令牌PID值。
  */
uint32_t
USB_Get_Last_Receive_PID (uint32_t SfrAddr)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(SfrAddr));

    /*---------------- 获取USB_BDnSTAT寄存器PID位 ----------------*/
    tmpaddr = SfrAddr;
    tmpreg = *(volatile uint32_t *) tmpaddr;
    tmpreg &= USB_BDSTAT_PID;
    tmpreg >>= USB_BDSTAT_PID0_POS;

    return tmpreg;
}

/**
  * 描述  写入一次USB缓冲区数据，写入长度不进行溢出验证。WriteAddr为USB缓冲区地
  *       址，WriteAddr和WriteDataAddr的内存在长度Length中不能重叠。
  * 输入  WriteAddr: 待写入的RAM地址，取值为指定的32位RAM地址。
  *       WriteDataAddr: 待写入的数据地址，取值为指定的32位RAM地址。
  *       Length: 待写入的数据长度，取值10位有效数值。
  * 返回  无。
  */
void
USB_Write_Buffer_Data (uint32_t WriteAddr,
                    uint32_t WriteDataAddr, uint32_t Length)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_BUFFER_ADDR(WriteAddr));
    CHECK_RESTRICTION(CHECK_USB_DATA_LENGTH(Length));

    memcpy ((void *)WriteAddr, (void *)WriteDataAddr, Length);
}
/**
  *   ##### USB模块(USB)缓冲区函数定义结束 #####
  */


/**
  *   ##### USB模块(USB)中断管理函数 #####
  */
/**
  * 描述  获取USB中断标志。
  * 输入  InterruptType: USB中断类型，取值范围为下面提供的一个:
  *                        USB_INT_ERROR: USB 错误条件中断
  *                        USB_INT_RESET: USB 复位中断
  *                        USB_INT_TRANSACTION_COMPLETED: 事务完成中断
  *                        USB_INT_BUS_ACTIVE: 总线活动检测中断
  *                        USB_INT_IDLE: 空闲检测中断
  *                        USB_INT_STALL_HANDSHAKE: STALL 握手中断
  *                        USB_INT_SOF_TAKEN: 帧起始令牌中断
  *                        USB_INT_DATE_FIELD_NOT_BYTE: 数据字段大小错误中断
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 主机错误中断
  *                        USB_INT_CRC16_FAILURE: CRC16 失败中断
  *                        USB_INT_PID_CHECK_FAILURE: PID 检查失败中断
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: 总线周转超时错误中断
  *                        USB_INT_BIT_STUFF_ERROR: 位填充错误中断
  * 返回  1:发生中断，0:未发生中断。
  */
FlagStatus
USB_Get_INT_Flag (uint32_t InterruptType)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_INT_ONE_EVENT(InterruptType));

    /*---------------- 读取USB_UIR寄存器中断标志位 ----------------*/
    if (USB_UIR & InterruptType)
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
  * 描述  清零USB中断标志。
  * 输入  InterruptType: USB中断类型，取值范围为下面提供的一个或多个组合:
  *                        USB_INT_ERROR: USB 错误条件中断
  *                        USB_INT_RESET: USB 复位中断
  *                        USB_INT_TRANSACTION_COMPLETED: 事务完成中断
  *                        USB_INT_BUS_ACTIVE: 总线活动检测中断
  *                        USB_INT_IDLE: 空闲检测中断
  *                        USB_INT_STALL_HANDSHAKE: STALL 握手中断
  *                        USB_INT_SOF_TAKEN: 帧起始令牌中断
  *                        USB_INT_DATE_FIELD_NOT_BYTE: 数据字段大小错误中断
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 主机错误中断
  *                        USB_INT_CRC16_FAILURE: CRC16 失败中断
  *                        USB_INT_PID_CHECK_FAILURE: PID 检查失败中断
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: 总线周转超时错误中断
  *                        USB_INT_BIT_STUFF_ERROR: 位填充错误中断
  * 返回  无。
  */
void
USB_Clear_INT_Flag (uint32_t InterruptType)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_INT_EVENT(InterruptType));

    /*---------------- 清零USB_UIR寄存器中断标志位 ----------------*/
    while(USB_UIR & InterruptType)
    {
    	USB_UIR &= ~InterruptType;
    }
}

/**
  * 描述  配置USB中断使能。
  * 输入  InterruptType: USB中断类型，取值范围为下面提供的一个或多个组合:
  *                        USB_INT_ERROR: USB 错误条件中断
  *                        USB_INT_RESET: USB 复位中断
  *                        USB_INT_TRANSACTION_COMPLETED: 事务完成中断
  *                        USB_INT_BUS_ACTIVE: 总线活动检测中断
  *                        USB_INT_IDLE: 空闲检测中断
  *                        USB_INT_STALL_HANDSHAKE: STALL 握手中断
  *                        USB_INT_SOF_TAKEN: 帧起始令牌中断
  *                        USB_INT_DATE_FIELD_NOT_BYTE: 数据字段大小错误中断
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 主机错误中断
  *                        USB_INT_CRC16_FAILURE: CRC16 失败中断
  *                        USB_INT_PID_CHECK_FAILURE: PID 检查失败中断
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: 总线周转超时错误中断
  *                        USB_INT_BIT_STUFF_ERROR: 位填充错误中断
  *       NewState: USB中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
USB_Set_INT_Enable (uint32_t InterruptType, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_INT_EVENT(InterruptType));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置USB_UIE寄存器中断使能位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能中断 */
        USB_UIE |= InterruptType;
    }
    else
    {
        /* 禁止中断 */
        USB_UIE &= ~InterruptType;
    }
}

/**
  * 描述  获取USB中断使能状态。
  * 输入  InterruptType: USB中断类型，取值范围为下面提供的一个:
  *                        USB_INT_ERROR: USB 错误条件中断
  *                        USB_INT_RESET: USB 复位中断
  *                        USB_INT_TRANSACTION_COMPLETED: 事务完成中断
  *                        USB_INT_BUS_ACTIVE: 总线活动检测中断
  *                        USB_INT_IDLE: 空闲检测中断
  *                        USB_INT_STALL_HANDSHAKE: STALL 握手中断
  *                        USB_INT_SOF_TAKEN: 帧起始令牌中断
  *                        USB_INT_DATE_FIELD_NOT_BYTE: 数据字段大小错误中断
  *                        USB_INT_CRC5_HOST_ERROR: CRC5 主机错误中断
  *                        USB_INT_CRC16_FAILURE: CRC16 失败中断
  *                        USB_INT_PID_CHECK_FAILURE: PID 检查失败中断
  *                        USB_INT_BUS_TURNAROUND_TIMEOUT: 总线周转超时错误中断
  *                        USB_INT_BIT_STUFF_ERROR: 位填充错误中断
  * 返回  1:中断已使能，0:中断未使能。
  */
FlagStatus
USB_Get_INT_Enable (uint32_t InterruptType)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_USB_INT_ONE_EVENT(InterruptType));

    /*---------------- 读取USB_UIE寄存器中断标志位 ----------------*/
    if (USB_UIE & InterruptType)
    {
        /* 中断已使能 */
        return SET;
    }
    else
    {
        /* 中断未使能 */
        return RESET;
    }
}
/**
  *   ##### USB模块(USB)中断管理函数定义结束 #####
  */
#endif  //KF32A_Periph_usb

