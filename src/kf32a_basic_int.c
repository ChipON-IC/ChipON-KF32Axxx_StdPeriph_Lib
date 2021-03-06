/***
  ******************************************************************************
  * 文件名  kf32a_basic_int.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了中断相关的功能函数，包含：
  *          + 中断(INT)功能配置函数定义
  *          + 外部中断(INT)功能初始化函数定义
  *          + 外部中断(INT)功能配置函数定义
  *
  *********************************************************************
  */

#include "kf32a_basic_int.h"

/* 中断定时器私有定义 ----------------------------------------------------*/
/* INT_IP寄存器配置掩码 */
#define INT_IP_INIT_MASK                ((uint32_t)0xF)

/* INT_EINTSSx寄存器配置掩码 */
#define INT_EINTSS_INIT_MASK            ((uint32_t)0x7)


/**
  *   ##### 中断(INT)功能配置函数定义 #####
  */
/**
  * 描述  获取当前正在处理的中断向量编号。
  * 输入  无。
  * 返回  当前中断向量编号，7位有效数据。
  */
uint8_t
INT_Get_Interrupt_Action (void)
{
    uint32_t tmpreg = 0;

    /* 读取INT_CTL0寄存器INTACT位域 */
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_INTACT;
    tmpreg >>= INT_CTL0_INTACT0_POS;

    return (uint8_t)tmpreg;
}

/**
  * 描述  获取当前待响应的最高优先级中断向量编号。
  * 输入  无。
  * 返回  待响应的最高优先级中断向量编号，7位有效数据。
  */
uint8_t
INT_Get_Priority_Pending_Action (void)
{
    uint32_t tmpreg = 0;

    /* 读取INT_CTL0寄存器INTPEND位域 */
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_INTPEND;
    tmpreg >>= INT_CTL0_INTPEND0_POS;

    return (uint8_t)tmpreg;
}

/**
  * 描述  设置优先级响应基级。
  * 输入  PriBase: 优先级响应基级，取值4位数据。
  * 返回  无。
  */
void
INT_Priority_Base (uint8_t PriBase)
{
    uint32_t tmpreg = 0;

    CHECK_RESTRICTION(CHECK_INT_PRIORITY_BASE(PriBase));

    /*------------------ 设置INT_CTL0寄存器 -----------------*/
    /* 根据参数PriBase，设置PRIBASE位域 */
    tmpreg = (uint32_t)PriBase << INT_CTL0_PRIBASE0_POS;
    INT_CTL0 = SFR_Config (INT_CTL0,
                          ~INT_CTL0_PRIBASE,
                          tmpreg);
}

/**
  * 描述  获取优先级响应基级。
  * 输入  无。
  * 返回  优先级响应基级，4位有效数据。
  */
uint8_t
INT_Get_Priority_Base (void)
{
    uint32_t tmpreg = 0;

    /* 读取INT_CTL0寄存器PRIBASE位域 */
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_PRIBASE;
    tmpreg >>= INT_CTL0_PRIBASE0_POS;

    return (uint8_t)tmpreg;
}

/**
  * 描述  设置中断自动堆栈对齐选择。
  * 输入  StackAlign: 中断自动堆栈对齐选择，
  *                   取值为：
  *                     INT_STACK_DOUBLE_ALIGN: 中断自动堆栈使用双字对齐
  *                     INT_STACK_SINGLE_ALIGN: 中断自动堆栈使用单字对齐
  * 返回  无。
  */
void
INT_Stack_Align_Config (uint32_t StackAlign)
{
    CHECK_RESTRICTION(CHECK_INT_STACK_ALIGN(StackAlign));

    /*---------------- 设置INT_CTL0寄存器DSALIGN位 ----------------*/
    if (StackAlign != INT_STACK_DOUBLE_ALIGN)
    {
        /* 中断自动堆栈使用单字对齐 */
        SFR_SET_BIT_ASM(INT_CTL0, INT_CTL0_DSALIGN_POS);
    }
    else
    {
        /* 中断自动堆栈使用双字对齐 */
        SFR_CLR_BIT_ASM(INT_CTL0, INT_CTL0_DSALIGN_POS);
    }
}

/**
  * 描述  设置硬件错误中断屏蔽位。
  * 输入  NewState: 中断自动堆栈对齐选择，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
INT_Fault_Masking_Config (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置INT_CTL0寄存器FAULTMASK位 ----------------*/
    if (NewState != FALSE)
    {
        /* FAULT异常及优先级可配置中断被屏蔽 */
        SFR_SET_BIT_ASM(INT_CTL0, INT_CTL0_FAULTMASK_POS);
    }
    else
    {
        /* FAULT异常未屏蔽 */
        SFR_CLR_BIT_ASM(INT_CTL0, INT_CTL0_FAULTMASK_POS);
    }
}

/**
  * 描述  判断是否一个悬起的中断将在下一步时进入活动状态。
  * 输入  无。
  * 返回  1:进入活动状态；0:不进入活动状态。
  */
FlagStatus
INT_Get_Pre_Empty (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 设置INT_CTL0寄存器INTPREEMPT位 ----------------*/
    tmpreg = INT_CTL0;
    if (tmpreg & INT_CTL0_INTPREEMPT)
    {
        /* 悬起的中断将在下一步时进入活动状态 */
        return SET;
    }
    else
    {
        /* 中断不进入活动状态 */
        return RESET;
    }
}

/**
  * 描述  判断当前是否有除NMI之外的中断挂起。
  * 输入  无。
  * 返回  1:有除NMI之外的中断挂起；0:没有除NMI之外的中断挂起。
  */
FlagStatus
INT_Get_Pending_Flag (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 设置INT_CTL0寄存器INTPENDING位 ----------------*/
    tmpreg = INT_CTL0;
    if (tmpreg & INT_CTL0_INTPENDING)
    {
        /* 有除NMI之外的中断挂起 */
        return SET;
    }
    else
    {
        /* 没有除NMI之外的中断挂起 */
        return RESET;
    }
}

/**
  * 描述  设置优先级分组。
  * 输入  PriorityGroup: 中断自动堆栈对齐选择，
  *                      取值范围为：
  *                        INT_PRIORITY_GROUP_3VS1: 抢占优先级为3位，子优先级为1位
  *                        INT_PRIORITY_GROUP_2VS2: 抢占优先级为2位，子优先级为2位
  *                        INT_PRIORITY_GROUP_1VS3: 抢占优先级为1位，子优先级为3位
  *                        INT_PRIORITY_GROUP_0VS4: 抢占优先级为0位，子优先级为4位
  * 返回  无。
  */
void
INT_Priority_Group_Config (uint32_t PriorityGroup)
{
    CHECK_RESTRICTION(CHECK_INT_PRIORITY_GROUP(PriorityGroup));

    /*------------------ 设置INT_CTL0寄存器 -----------------*/
    /* 根据参数PriorityGroup，设置PRIGROUP位域 */
    INT_CTL0 = SFR_Config (INT_CTL0,
                          ~INT_CTL0_PRIGROUP,
                          PriorityGroup);
}

/**
  * 描述  获取优先级分组。
  * 输入  无。
  * 返回  优先级分组位域配置，32位有效数据。
  */
uint32_t
INT_Get_Priority_Group (void)
{
    uint32_t tmpreg = 0;

    /*------------------ 读取INT_CTL0寄存器 -----------------*/
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_PRIGROUP;

    return tmpreg;
}

/**
  * 描述  全局可屏蔽中断使能位，该中断使能控制不包含复位/NMI/硬件错误中断。
  * 输入  NewState: 全局可屏蔽中断使能，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
INT_All_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置INT_CTL0寄存器AIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 全局可屏蔽中断使能 */
        SFR_SET_BIT_ASM(INT_CTL0, INT_CTL0_AIE_POS);
    }
    else
    {
        /* 全局可屏蔽中断禁止 */
        SFR_CLR_BIT_ASM(INT_CTL0, INT_CTL0_AIE_POS);
    }
}

/**
  * 描述  外设或内核中断使能控制，对于用户未定义的保留区中断向量，
  *       可能出现未知结果。
  * 输入  Peripheral: 外设或内核中断向量编号，取值范围为：
  *                   枚举类型InterruptIndex中的外设中断向量编号。
  *       NewState: 外设或内核中断使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
INT_Interrupt_Enable (InterruptIndex Peripheral, FunctionalState NewState)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (Peripheral <= INT_SysTick)
    {
        /*---------------- 设置INT_EIE0寄存器 ----------------*/
        tmpreg = Peripheral - INT_StackFault;
        tmpreg1 = INT_EIE0_STACKIE << tmpreg;
        INT_EIE0 = SFR_Config (INT_EIE0,
                          ~tmpreg1,
                          NewState << (tmpreg+5));
    }
    else if (Peripheral <= INT_SPI1)
    {
        /*---------------- 设置INT_EIE1寄存器 ----------------*/
        tmpreg = Peripheral - INT_WWDT;
        tmpreg1 = INT_EIE1_WWDTIE << tmpreg;
        INT_EIE1 = SFR_Config (INT_EIE1,
                          ~tmpreg1,
                          NewState << tmpreg);
    }
    else if (Peripheral <= INT_USART7)
    {
        /*---------------- 设置INT_EIE2寄存器 ----------------*/
        tmpreg = Peripheral - INT_DMA1;
        tmpreg1 = INT_EIE2_DMA1IE << tmpreg;
        INT_EIE2 = SFR_Config (INT_EIE2,
                          ~tmpreg1,
                          NewState << tmpreg);
    }
    else
    {
        /*---------------- 设置INT_EIE4寄存器 ----------------*/
        /*---------------- 保留，可作为软件中断 ----------------*/
    }
}

/**
  * 描述  SYSTICK中断标志软件置位。
  * 输入  无。
  * 返回  无。
  */
void
INT_Set_Systick_Flag (void)
{
    SFR_SET_BIT_ASM(INT_EIF0, INT_EIF0_SYSTICKIF_POS);
}

/**
  * 描述  PendSV中断标志软件置位。
  * 输入  无。
  * 返回  无。
  */
void
INT_Set_PendSV_Flag (void)
{
    SFR_SET_BIT_ASM(INT_EIF0, INT_EIF0_PENDSVIF_POS);
}

/**
  * 描述  获取外设或内核中断标志，对于用户未定义的保留区中断向量，
  *       可能出现未知结果。
  * 输入  Peripheral: 外设或内核中断向量编号，取值范围为：
  *                   枚举类型InterruptIndex中的外设中断向量编号。
  * 返回  无。
  */
FlagStatus
INT_Get_Interrupt_Flag (InterruptIndex Peripheral)
{
    uint32_t tmpreg = 0;
    uint32_t tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));

    if (Peripheral <= INT_SysTick)
    {
        /*---------------- 设置INT_EIF0寄存器 ----------------*/
        tmpreg = INT_EIF0_NMIIF << (Peripheral - INT_NMI);
        tmpreg1 = INT_EIF0;
        if (INT_EIF0 & tmpreg)
        {
            /* 中断挂起 */
            return SET;
        }
        else
        {
            /* 无中断挂起 */
            return RESET;
        }
    }
    else if (Peripheral <= INT_SPI1)
    {
        /*---------------- 设置INT_EIF1寄存器 ----------------*/
        tmpreg = INT_EIF1_WWDTIF << (Peripheral - INT_WWDT);
        tmpreg1 = INT_EIF1;
        if (INT_EIF1 & tmpreg)
        {
            /* 中断挂起 */
            return SET;
        }
        else
        {
            /* 无中断挂起 */
            return RESET;
        }
    }
    else if (Peripheral <= INT_USART7)
    {
        /*---------------- 设置INT_EIF2寄存器 ----------------*/
        tmpreg = INT_EIF2_DMA1IF << (Peripheral - INT_DMA1);
        tmpreg1 = INT_EIF2;

    }
    else
    {
        /*---------------- 设置INT_EIF4寄存器 ----------------*/
        /*---------------- 保留，可作为软件中断 ----------------*/
    }

    /*---------------- 检验对应中断标志位状态 ----------------*/
    if ((*(volatile uint32_t*)tmpreg1) & tmpreg)
    {
        /* 中断挂起 */
        return SET;
    }
    else
    {
        /* 无中断挂起 */
        return RESET;
    }
}
/**
  * 描述 清外设或内核中断标志
  * 输入  Peripheral: 外设或内核中断向量编号，取值范围为：
  *                   枚举类型InterruptIndex中的外设中断向量编号。
  * 返回  无。
  */
void
INT_Clear_Interrupt_Flag (InterruptIndex Peripheral)
{
    uint32_t tmpreg = 0;
    volatile uint32_t *tmpreg1 = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));

    if (Peripheral <= INT_SysTick)
    {
        /*---------------- 设置INT_EIF0寄存器 ----------------*/
        tmpreg = INT_EIF0_NMIIF << (Peripheral - INT_NMI);
        tmpreg1 = &INT_EIF0;
    }
    else if (Peripheral <= INT_SPI1)
    {
        /*---------------- 设置INT_EIF1寄存器 ----------------*/
        tmpreg = INT_EIF1_WWDTIF << (Peripheral - INT_WWDT);
        tmpreg1 = &INT_EIF1;
    }
    else if (Peripheral <= INT_USART7)
    {
        /*---------------- 设置INT_EIF2寄存器 ----------------*/
        tmpreg = INT_EIF2_DMA1IF << (Peripheral - INT_DMA1);
        tmpreg1 = &INT_EIF2;
    }
    else
    {
        /*---------------- 设置INT_EIF4寄存器 ----------------*/
        /*---------------- 保留，可作为软件中断 ----------------*/
    }

    /*---------------- 清对应中断标志位状态 ----------------*/
    //(*(volatile uint32_t*)tmpreg1) &= ~tmpreg;
    *tmpreg1 &= ~tmpreg;

}
/**
  * 描述  外设或内核中断优先级配置，对于用户未定义的保留区中断向量，
  *       可能出现未知结果。
  * 输入  Peripheral: 外设或内核中断向量编号，取值范围为：
  *                   枚举类型InterruptIndex中的外设中断向量编号。
  *       Preemption: 抢占优先级，同时满足PRIGROUP设置。
  *       SubPriority: 子优先级， 同时满足PRIGROUP设置。
  *       (GROUP) == INT_PRIORITY_GROUP_3VS1时：Preemption取值范围为：0~7，SubPriority取值范围为：0~1
  *       (GROUP) == INT_PRIORITY_GROUP_2VS2时：Preemption取值范围为：0~3，SubPriority取值范围为：0~3
  *       (GROUP) == INT_PRIORITY_GROUP_1VS3时：Preemption取值范围为：0~1，SubPriority取值范围为：0~7
  *       (GROUP) == INT_PRIORITY_GROUP_0VS4时：Preemption取值范围为：0~0，SubPriority取值范围为：0~15
  *
  * 返回  无。
  */
void
INT_Interrupt_Priority_Config (InterruptIndex Peripheral,
                    uint32_t Preemption, uint32_t SubPriority)
{
    uint32_t tmpreg = 0;
    uint32_t regoffset = 0;
    uint32_t bitoffset = 0;
    uint32_t priorityconfig = 0;

    /*--------- 获取INT_CTL0寄存器的PRIGROUP位 ---------*/
    tmpreg = INT_CTL0;
    tmpreg &= INT_CTL0_PRIGROUP;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_PERIPHERAL_INTERRUPT_INDEX(Peripheral));
    CHECK_RESTRICTION(CHECK_PRIORITY_CONFIG(tmpreg, Preemption, SubPriority));

    /*--------- 获取中断优先级控制位的值 ---------*/
    switch (tmpreg)
    {
    case INT_PRIORITY_GROUP_3VS1:
        priorityconfig = (Preemption << 1) | (SubPriority & 0x1);
        break;
    case INT_PRIORITY_GROUP_2VS2:
        priorityconfig = (Preemption << 2) | (SubPriority & 0x3);
        break;
    case INT_PRIORITY_GROUP_1VS3:
        priorityconfig = (Preemption << 3) | (SubPriority & 0x7);
        break;
    case INT_PRIORITY_GROUP_0VS4:
        priorityconfig = SubPriority & 0xF;
        break;
    default:
        CHECK_RESTRICTION(0);
        break;
    }

    /*--------- 配置中断优先级控制位 ---------*/
    if (Peripheral <= INT_USART7)
    {
        /*---------------- 设置INT_IPx(x=0~18)寄存器 ----------------*/
        /* 获取中断优先级寄存器地址 */
        regoffset = (Peripheral - INT_Reserved4) >> 2;
        tmpreg = (uint32_t)&INT_IP0;
        tmpreg = tmpreg + (regoffset << 2);
    }
    else
    {
    }

    /*---------------- 设置INT_IPx(x=0~30)寄存器 ----------------*/
    bitoffset = ((Peripheral & 0x3) * 8) + 4;
    *(volatile uint32_t*)tmpreg
          = SFR_Config (*(volatile uint32_t*)tmpreg,
                      ~(INT_IP_INIT_MASK << bitoffset),
                      priorityconfig << bitoffset);
}

/**
  * 描述  外设或内核中断优先级配置，对于用户未定义的保留区中断向量，
  *      可能出现未知结果。
  * 输入   Peripheral: 外设或内核中断向量编号，取值范围为：
  *                  枚举类型InterruptIndex中的外设中断向量编号。
  *      Priority: 优先级，同时满足PRIGROUP设置。
  *      Priority取值范围为：0~15
  *
  * 返回   无。
  */
void
INT_Set_Interrupt_Priority(InterruptIndex Peripheral, uint32_t Priority)
{
    uint32_t tmpreg = 0;
    uint32_t regoffset = 0;
    uint32_t bitoffset = 0;

    regoffset = (Peripheral - INT_Reserved4) >> 2;
    tmpreg = (uint32_t)&INT_IP0;
    tmpreg = tmpreg + (regoffset << 2);
    bitoffset = ((Peripheral & 0x3) << 0X03) + 4;
    *(volatile uint32_t*)tmpreg
               = SFR_Config (*(volatile uint32_t*)tmpreg,
               ~((uint32_t)0x0f << bitoffset),
               Priority << bitoffset);
}


/**
  * 描述  中断延时配置。
  * 输入  IntDelay: 中断延时控制，取值8位数据。
  * 返回  无。
  */
void
INT_Stack_Delay_Enable (uint8_t IntDelay)
{
    /*---------------- 设置INT_CTL1寄存器INTDELY位 ----------------*/
    INT_CTL1 = SFR_Config (INT_CTL1,
                      ~(INT_CTL1_INTDELY),
                      (uint32_t)IntDelay << INT_CTL1_INTDELY0_POS);
}
/**
  *   ##### 中断(INT)功能配置函数定义结束 #####
  */


/**
  *   ##### 外部中断(INT)功能初始化函数定义 #####
  */
/**
  * 描述  外部中断(EINT)配置，并使能中断。
  * 输入  eintInitStruct: 外部中断配置信息结构体指针。
  * 返回  无。
  */
void
INT_External_Configuration (EINT_InitTypeDef* eintInitStruct)
{
    uint32_t eintoffset = 0;
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_NUM(eintInitStruct->m_Line));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eintInitStruct->m_Mask));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eintInitStruct->m_Rise));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(eintInitStruct->m_Fall));
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_SOURCE(eintInitStruct->m_Source));

    eintoffset = eintInitStruct->m_Line;
    /* 使能上升沿中断 */
    /*------------------ 设置INT_EINTRISE寄存器 -----------------*/
    /* 根据结构体成员m_Line和m_Rise，设置EINTRI位域 */
    INT_EINTRISE = SFR_Config (INT_EINTRISE,
                          ~(INT_EINTRISE_EINTRI0 << eintoffset),
                          eintInitStruct->m_Rise << eintoffset);

    /* 使能下降沿中断 */
    /*------------------ 设置INT_EINTFALL寄存器 -----------------*/
    /* 根据结构体成员m_Line和m_Fall，设置EINTFA位域 */
    INT_EINTFALL = SFR_Config (INT_EINTFALL,
                          ~(INT_EINTFALL_EINTFA0 << eintoffset),
                          eintInitStruct->m_Fall << eintoffset);

    /* 选择中断源 */
    /*------------------ 设置INT_EINTSSx寄存器 -----------------*/
    /* 根据结构体成员m_Line和m_Source，设置EINTRI位域 */
    if (eintoffset < 8)
    {
        tmpreg = eintInitStruct->m_Line * 4;
        INT_EINTSS0 = SFR_Config (INT_EINTSS0,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          eintInitStruct->m_Source << tmpreg);
    }
    else if (eintoffset < 16)
    {
        tmpreg = (eintInitStruct->m_Line - 8) * 4;
        INT_EINTSS1 = SFR_Config (INT_EINTSS1,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          eintInitStruct->m_Source << tmpreg);
    }
    else
    {
    }

    /* 使能外部中断 */
    /*------------------ 设置INT_EINTMASK寄存器 -----------------*/
    /* 根据结构体成员m_Line和m_Mask，设置EINTM位域 */
    INT_EINTMASK = SFR_Config (INT_EINTMASK,
                          ~(INT_EINTMASK_EINTM0 << eintoffset),
                          eintInitStruct->m_Mask << eintoffset);
}

/**
  * 描述  初始化外部中断(EINT)配置信息结构体。
  * 输入  eintInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
INT_External_Struct_Init (EINT_InitTypeDef* eintInitStruct)
{
    /* 设置外部中断编号 */
    eintInitStruct->m_Line = INT_EXTERNAL_INTERRUPT_0;
    /* 设置外部中断使能控制 */
    eintInitStruct->m_Mask = FALSE;
    /* 设置外部中断上升沿中断使能 */
    eintInitStruct->m_Rise = FALSE;
    /* 设置外部中断下降沿中断使能 */
    eintInitStruct->m_Fall = FALSE;
    /* 设置外部中断的中断源选择 */
    eintInitStruct->m_Source = INT_EXTERNAL_SOURCE_PA;
}
/**
  *   ##### 外部中断(INT)功能初始化函数定义结束 #####
  */


/**
  *   ##### 外部中断(INT)功能配置函数定义 #####
  */
/**
  * 描述  外部中断(EINT)使能配置。
  * 输入  EintMask: 外部中断编号掩码，
  *                 取值为宏INT_EINTMASK_EINTM0至INT_EINTMASK_EINTM31的位或组合。
  *       NewState: 外部中断使能请求，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
INT_External_Mask_Enable (uint32_t EintMask, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置INT_EINTMASK寄存器EINTMx位 ----------------*/
    if (NewState != FALSE)
    {
        INT_EINTMASK |= EintMask;
    }
    else
    {
        INT_EINTMASK &= ~EintMask;
    }
}

/**
  * 描述  外部中断(EINT)上升沿中断使能配置。
  * 输入  EintMask: 外部中断编号掩码，
  *                 取值为宏INT_EINTMASK_EINTM0至INT_EINTMASK_EINTM31的位或组合。
  *       NewState: 外部中断上升沿中断使能，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
INT_External_Rise_Enable (uint32_t EintMask, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置INT_EINTRISE寄存器EINTRIx位 ----------------*/
    if (NewState != FALSE)
    {
        INT_EINTRISE |= EintMask;
    }
    else
    {
        INT_EINTRISE &= ~EintMask;
    }
}

/**
  * 描述  外部中断(EINT)下降沿中断使能配置。
  * 输入  EintMask: 外部中断编号掩码，
  *                 取值为宏INT_EINTMASK_EINTM0至INT_EINTMASK_EINTM31的位或组合。
  *       NewState: 外部中断下降沿中断使能，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
INT_External_Fall_Enable (uint32_t EintMask, FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置INT_EINTFALL寄存器EINTFAx位 ----------------*/
    if (NewState != FALSE)
    {
        INT_EINTFALL |= EintMask;
    }
    else
    {
        INT_EINTFALL &= ~EintMask;
    }
}

/**
  * 描述  获取外部中断(EINT)中断标志位。
  * 输入  EintNum: 外部中断编号，
  *                取值为宏INT_EXTERNAL_INTERRUPT_0至
  *                INT_EXTERNAL_INTERRUPT_31中的一个，
  *                即0~31。
  * 返回  外部中断(EINT)中断标志，0：没有发生外部中断，1：发生外部中断。
  */
FlagStatus
INT_Get_External_Flag (uint32_t EintNum)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_NUM(EintNum));

    /*---------------- 读取INT_EINTF寄存器EINTIFx位 ----------------*/
    tmpreg = INT_EINTF_EINTIF0 << EintNum;
    if (INT_EINTF & tmpreg)
    {
        /* 发生了外部中断 */
        return SET;
    }
    else
    {
        /* 没有发生了外部中断 */
        return RESET;
    }
}

/**
  * 描述  清除外部中断(EINT)中断标志位。
  * 输入  EintNum: 外部中断编号，
  *                取值为宏INT_EXTERNAL_INTERRUPT_0至
  *                INT_EXTERNAL_INTERRUPT_31中的一个，
  *                即0~31。
  * 返回  无。
  */
void
INT_External_Clear_Flag (uint32_t EintNum)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_NUM(EintNum));

    /*---------------- 清零INT_EINTF寄存器EINTIFx位 ----------------*/
    tmpreg = INT_EINTF_EINTIF0 << EintNum;
    while((INT_EINTF & tmpreg)>>(EintNum+INT_EINTF_EINTIF0_POS))
    {
    	INT_EINTF &= ~tmpreg;
    }
}

/**
  * 描述  外部中断(EINT)中断源配置。
  * 输入  EintNum: 外部中断编号，
  *                取值为宏INT_EXTERNAL_INTERRUPT_0至
  *                INT_EXTERNAL_INTERRUPT_31中的一个，
  *                即0~15。
  *       PeripheralSource: 外设中断线的中断输入源，
  *                         取值范围为：
  *                           INT_EXTERNAL_SOURCE_PA
  *                           INT_EXTERNAL_SOURCE_PB
  *                           INT_EXTERNAL_SOURCE_PC
  *                           INT_EXTERNAL_SOURCE_PD
  *                           INT_EXTERNAL_SOURCE_PE
  *                           INT_EXTERNAL_SOURCE_PF
  *                           INT_EXTERNAL_SOURCE_PG
  *                           INT_EXTERNAL_SOURCE_PH
  *                           INT_EXTERNAL_SOURCE_PVD
  *                           INT_EXTERNAL_SOURCE_RTC
  *                           INT_EXTERNAL_SOURCE_TAMPER
  *                           INT_EXTERNAL_SOURCE_ALARMCLK
  *                           INT_EXTERNAL_SOURCE_AES
  *                           INT_EXTERNAL_SOURCE_EINT21TO31
  * 返回  无。
  */
void
INT_External_Source_Enable (uint32_t EintNum, uint32_t PeripheralSource)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_PERIPHERAL(EintNum));
    CHECK_RESTRICTION(CHECK_INT_EXTERNAL_SOURCE(PeripheralSource));

    /*---------------- 设置INT_EINTSS0寄存器EINTSOUx位 ----------------*/
    tmpreg = (EintNum % 8) * 4;
    if (EintNum < 8)
    {
        INT_EINTSS0 = SFR_Config (INT_EINTSS0,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          PeripheralSource << tmpreg);
    }
    else if (EintNum < 16)
    {
        INT_EINTSS1 = SFR_Config (INT_EINTSS1,
                          ~(INT_EINTSS_INIT_MASK << tmpreg),
                          PeripheralSource << tmpreg);
    }
    else
    {
    }
}
/**
  *   ##### 外部中断(INT)功能配置函数定义结束 #####
  */
