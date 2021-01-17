/**
  ******************************************************************************
  * 文件名  kf32a_basic_sysctl.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了系统控制相关的功能函数，包含：
  *          + 系统控制(SYSCTL)功能配置函数
  *          + DO指令功能配置函数
  *
  *********************************************************************
  */

#include "kf32a_basic_sysctl.h"

/* 系统控制私有定义 ----------------------------------------------------*/
/* SYS_ARCTL寄存器写操作ARKEY编码 */
#define SYS_ARCTL_WRITE_CTL             ((uint32_t)0x05FA \
                                      << SYS_ARCTL_ARKEY_POS)


/**
  *   ##### 系统控制(SYSCTL)功能配置函数 #####
  */
/**
  * 描述  获取状态字V位溢出标志。
  * 输入  无。
  * 返回  1:溢出；0:非溢出。
  */
FlagStatus
SYSCTL_Get_V_Flag (void)
{
    /*---------------- 读取SYS_PSW寄存器V位 ----------------*/
    if (SYS_PSW & SYS_PSW_V)
    {
        /* 溢出 */
        return SET;
    }
    else
    {
        /* 非溢出 */
        return RESET;
    }
}

/**
  * 描述  获取状态字C位进位或借位标志。
  * 输入  无。
  * 返回  1:加法有进位或减法无借位；0:加法无进位或减法有借位。
  */
FlagStatus
SYSCTL_Get_C_Flag (void)
{
    /*---------------- 读取SYS_PSW寄存器C位 ----------------*/
    if (SYS_PSW & SYS_PSW_C)
    {
        /* 加法有进位或减法无借位 */
        return SET;
    }
    else
    {
        /* 加法无进位或减法有借位 */
        return RESET;
    }
}

/**
  * 描述  获取状态字Z位零值标志。
  * 输入  无。
  * 返回  1:零值；0:非零值。
  */
FlagStatus
SYSCTL_Get_Z_Flag (void)
{
    /*---------------- 读取SYS_PSW寄存器Z位 ----------------*/
    if (SYS_PSW & SYS_PSW_Z)
    {
        /* 零值 */
        return SET;
    }
    else
    {
        /* 非零值 */
        return RESET;
    }
}

/**
  * 描述  获取状态字N位负值标志。
  * 输入  无。
  * 返回  1:负值；0:非负值。
  */
FlagStatus
SYSCTL_Get_N_Flag (void)
{
    /*---------------- 读取SYS_PSW寄存器N位 ----------------*/
    if (SYS_PSW & SYS_PSW_N)
    {
        /* 负值 */
        return SET;
    }
    else
    {
        /* 非负值 */
        return RESET;
    }
}

/**
  * 描述  设置状态字V位溢出标志。
  * 输入  NewState: 状态字V位溢出标志，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Set_V_Flag (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_PSW寄存器V位 ----------------*/
    if (NewState != FALSE)
    {
        /* 设置溢出标志 */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_V_POS);
    }
    else
    {
        /* 清零溢出标志 */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_V_POS);
    }
}

/**
  * 描述  设置状态字C位溢出标志。
  * 输入  NewState: 状态字C位进位或借位标志，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Set_C_Flag (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_PSW寄存器C位 ----------------*/
    if (NewState != FALSE)
    {
        /* 设置进位或借位标志 */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_C_POS);
    }
    else
    {
        /* 清零进位或借位标志 */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_C_POS);
    }
}

/**
  * 描述  设置状态字Z位溢出标志。
  * 输入  NewState: 状态字Z位进位或借位标志，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Set_Z_Flag (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_PSW寄存器Z位 ----------------*/
    if (NewState != FALSE)
    {
        /* 设置进位或借位标志 */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_Z_POS);
    }
    else
    {
        /* 清零进位或借位标志 */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_Z_POS);
    }
}
/**
  * 描述  设置状态字N位负值标志。
  * 输入  NewState: 状态字N位负值或非负值标志，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Set_N_Flag (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_PSW寄存器Z位 ----------------*/
    if (NewState != FALSE)
    {
        /* 设置进位或借位标志 */
        SFR_SET_BIT_ASM(SYS_PSW, SYS_PSW_N_POS);
    }
    else
    {
        /* 清零进位或借位标志 */
        SFR_CLR_BIT_ASM(SYS_PSW, SYS_PSW_N_POS);
    }
}

/**
  * 描述  设置中断处理返回进入休眠模式使能。
  * 输入  NewState: 中断处理返回进入休眠模式使能状态，取值为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Sleep_On_Exit_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_MCTL寄存器SLEEPONEXIT位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能中断返回时进入休眠模式 */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPONEXIT_POS);
    }
    else
    {
        /* 禁止中断返回时进入休眠模式 */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPONEXIT_POS);
    }
}

/**
  * 描述  设置深度休眠模式使能。
  * 输入  NewState: 深度休眠模式使能状态，取值为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Deep_Sleep_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_MCTL寄存器SLEEPDEEP位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能深度休眠模式 */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPDEEP_POS);
    }
    else
    {
        /* 禁止深度休眠模式 */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SLEEPDEEP_POS);
    }
}

/**
  * 描述  设置唤醒模式选择。
  * 输入  NewState: 任意中断唤醒CPU使能状态，取值为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Interrupt_Awake_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_MCTL寄存器SLPONMOD位 ----------------*/
    if (NewState != FALSE)
    {
        /* 任意挂起的中断都能唤醒CPU */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SLPONMOD_POS);
    }
    else
    {
        /* 只有当挂起的中断优先级比当前优先级高时才能唤醒CPU */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SLPONMOD_POS);
    }
}

/**
  * 描述  获取当前中断自动堆栈的对齐方式。
  * 输入  无。
  * 返回  1:双字对齐使能；0:双字对齐未使能。
  */
FlagStatus
SYSCTL_Stack_Align_State (void)
{
    /*---------------- 读取SYS_MCTL寄存器STACKALIGN位 ----------------*/
    if (SYS_MCTL & SYS_MCTL_STACKALIGN)
    {
        /* 双字对齐使能 */
        return SET;
    }
    else
    {
        /* 双字对齐未使能 */
        return RESET;
    }
}

/**
  * 描述  设置超级用户控制。
  * 输入  NewState: 选择超级用户标志，取值为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Super_User_Config (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置SYS_MCTL寄存器SUSTA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 超级用户模式 */
        SFR_SET_BIT_ASM(SYS_MCTL, SYS_MCTL_SUSTA_POS);
    }
    else
    {
        /* 用户模式 */
        SFR_CLR_BIT_ASM(SYS_MCTL, SYS_MCTL_SUSTA_POS);
    }
}

/**
  * 描述  获取当前有效堆栈指针状态标志。
  * 输入  无。
  * 返回  1:PSP是当前的堆栈指针；0:MSP是当前的堆栈指针。
  */
FlagStatus
SYSCTL_Stack_Pointer_State (void)
{
    /*---------------- 读取SYS_MCTL寄存器SPSEL位 ----------------*/
    if (SYS_MCTL & SYS_MCTL_SPSEL)
    {
        /* PSP是当前的堆栈指针 */
        return SET;
    }
    else
    {
        /* MSP是当前的堆栈指针 */
        return RESET;
    }
}

/**
  * 描述  设置当前有效堆栈指针。
  * 输入  PresentSP: 当前有效堆栈指针，取值为：
  *                    SYSCTL_SP_IS_MSP: MSP是当前的堆栈指针
  *                    SYSCTL_SP_IS_PSP: PSP是当前的堆栈指针
  * 返回  无。
  */
void
SYSCTL_Stack_Pointer_Config (uint32_t PresentSP)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SYSCTL_SP(PresentSP));

    /*------------------ 设置SYS_MCTL寄存器SPSEL位 -----------------*/
    SYS_MCTL = SFR_Config (SYS_MCTL, ~SYS_MCTL_SPSEL, PresentSP);
}

/**
  * 描述  设置异常活动信息复位请求。
  * 输入  NewState: 异常活动信息复位请求，取值为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_Exception_Reset_Enable (FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ 设置SYS_ARCTL寄存器VECTRST位 -----------------*/
    tmpreg = SYS_ARCTL_WRITE_CTL | \
            ((uint32_t)NewState << SYS_ARCTL_VECTRST_POS);
    SYS_ARCTL = SFR_Config (SYS_ARCTL,
                        ~(SYS_ARCTL_VECTRST | SYS_ARCTL_ARKEY),
                        tmpreg);
}

/**
  * 描述  设置系统复位请求。
  * 输入  NewState: 系统复位请求，取值为：TRUE 或 FALSE。
  * 返回  无。
  */
void
SYSCTL_System_Reset_Enable (FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*------------------ 设置SYS_ARCTL寄存器SYSTEMRST位 -----------------*/
    tmpreg = SYS_ARCTL_WRITE_CTL | \
            ((uint32_t)NewState << SYS_ARCTL_SYSTEMRST_POS);
    SYS_ARCTL = SFR_Config (SYS_ARCTL,
                        ~(SYS_ARCTL_SYSTEMRST | SYS_ARCTL_ARKEY),
                        tmpreg);
}

/**
  * 描述  中断向量表重映射地址配置。
  * 输入  VectorOffset: 中断向量表重映射地址，取值32位数据，低两位硬件忽略。
  * 返回  无。
  */
void
SYSCTL_Vector_Offset_Config (uint32_t VectorOffset)
{
	uint32_t tmpreg = 0;
    /*---------------- 设置SYS_VECTOFF寄存器TBLOFF位 ----------------*/
	tmpreg = VectorOffset << SYS_VECTOFF_TBLOFF0_POS;
    SYS_VECTOFF = SFR_Config (SYS_VECTOFF,
                      ~(SYS_VECTOFF_TBLOFF),
                      tmpreg);
}


/**
  * 描述  配置RAM空间长度，指示RAM空间结束地址。
  * 输入  RamSpace: RAM空间长度，取值为26位有效数值。
  * 返回  无。
  */
void
SYSCTL_Ram_Space_Config (uint32_t RamSpace)
{
	uint32_t tmpreg = 0;
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RAM_SPACE(RamSpace));

    /*---------------- 配置SYS_RAMSPA寄存器RAMSPA位 ----------------*/
    tmpreg = RamSpace << SYS_RAMSPA_RAMSPA0_POS;
    SYS_RAMSPA = SFR_Config (SYS_RAMSPA,
                      ~SYS_RAMSPA_RAMSPA,
                      tmpreg);
}

/**
  * 描述  配置FLASH开始512字节空间映射控制。
  * 输入  MemCtl: 映射控制，取值范围为:
  *                 SYSCTL_FLASH_REMAP_FOR_ROM: 为ROM的映射
  *                 SYSCTL_FLASH_REMAP_FOR_RAM: 为RAM的映射
  *                 SYSCTL_FLASH_REMAP_FOR_FLASH: 为FLASH的映射
  *                 SYSCTL_FLASH_REMAP_STOP_CPU: 停止CPU运行
  * 返回  无。
  */
void
SYSCTL_Flash_Start_Remap_Config (uint32_t MemCtl)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SYSCTL_FLASH_REMAP(MemCtl));

    /*---------------- 配置SYS_MEMCTL寄存器MEMM位 ----------------*/
    SYS_MEMCTL = SFR_Config (SYS_MEMCTL, ~SYS_MEMCTL_MEMM, MemCtl);
}
/**
  *   ##### 系统控制(SYSCTL)功能配置函数定义结束 #####
  */


