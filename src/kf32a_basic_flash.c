/**
  ******************************************************************************
  * 文件名  kf32a_basic_flash.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了FLASH程序存储器(FLASH)相关的功能函数，包含：
  *          + FLASH程序存储器(FLASH)功能配置函数
  *          + FLASH程序存储器(FLASH)编程功能函数
  *
  *注意：使用片擦功能后，FLASH区清空，原有程序丢失。无法回切FLASH
  *注意：使用页擦功能后，需确认是否擦除了后期调用函数的FLASH，若已擦除，则无法回切FLASH
  *********************************************************************
  */

#include "kf32a_basic_flash.h"


/* 定时器私有定义 ----------------------------------------------------*/
/* FLASH_ISPCON0控制寄存器写操作高16位值 */
#define FLASH_ISPCON0_HIGH              ((uint32_t)0x50AF0000)
#define FLASH_ISPCON0_HIGH_MSK          ((uint32_t)0xFFFF0000)

/* FLASH_ISPCON1控制寄存器写操作高16位值 */
#define FLASH_ISPCON1_HIGH              ((uint32_t)0xA05F0000)
#define FLASH_ISPCON1_HIGH_MSK          ((uint32_t)0xFFFF0000)

/* FLASH_ISPTRG控制寄存器写操作高16位值 */
#define FLASH_ISPTRG_HIGH               ((uint32_t)0x50AF0000)
#define FLASH_ISPTRG_HIGH_MSK           ((uint32_t)0xFFFF0000)

/* FLASH_NVMUNLOCK寄存器解锁序列 */
#define FLASH_NVMUNLOCK_KEY1            ((uint32_t)0x87654321)
#define FLASH_NVMUNLOCK_KEY2            ((uint32_t)0x05040302)

/* FLASH_PROUNLOCK寄存器解锁序列 */
#define FLASH_PROUNLOCK_KEY1            ((uint32_t)0x789ABCDE)
#define FLASH_PROUNLOCK_KEY2            ((uint32_t)0x16151413)

/* FLASH_CFGUNLOCK寄存器解锁序列 */
#define FLASH_CFGUNLOCK_KEY1            ((uint32_t)0x879A43DE)
#define FLASH_CFGUNLOCK_KEY2            ((uint32_t)0x27262524)


/**
  *   ##### FLASH程序存储器(FLASH)功能配置函数 #####
  */

void __attribute__((section(".indata")))
CHECK_RESTRICTION_RAM(int expr)
{
	while(!expr)
		;
}

/* 写特殊功能寄存器RAM */
static inline uint32_t   __attribute__((section(".indata")))
SFR_Config_RAM (uint32_t SfrMem, uint32_t SfrMask, uint32_t WriteVal)
{
    return ((SfrMem & SfrMask) | (WriteVal));
}

/**
  * 描述  读FLASH编程组件解锁状态。
  * 输入  无。
  * 返回  锁定状态，1：解锁状态，0：锁定状态。
  */
LockStatus __attribute__((section(".indata")))
FLASH_Get_NonVolatile_Memory_Unlock_Status_RAM (void)
{
    if (FLASH_ISPCON0 & FLASH_ISPCON0_NVMLOCK)
    {
        /* 解锁状态 */
        return UNLOCK;
    }
    else
    {
        /* 锁定状态 */
        return LOCK;
    }
}

/**
  * 描述  清零FLASH编程组件解锁状态位。
  * 输入  无。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Clear_NonVolatile_Memory_Unlock_Status_RAM (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPCON0寄存器NVMLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | FLASH_ISPCON0_NVMLOCK;
    FLASH_ISPCON0 = SFR_Config_RAM (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_NVMLOCK),
                        tmpreg);
}
/**
  * 描述  清零FLASH编程组件解锁状态位。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Clear_NonVolatile_Memory_Unlock_Status (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPCON0寄存器NVMLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | FLASH_ISPCON0_NVMLOCK;
    FLASH_ISPCON0 = SFR_Config (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_NVMLOCK),
                        tmpreg);
}
/**
  * 描述  置位FLASH编程组件解锁状态位，解锁ISP寄存器。
  * 输入  无。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_Unlock_ISP_RAM   (void)
{
    /* 写入KEY1 和 KEY2 */
    FLASH_NVMUNLOCK = FLASH_NVMUNLOCK_KEY1;
    FLASH_NVMUNLOCK = FLASH_NVMUNLOCK_KEY2;
}

/**
  * 描述  置位FLASH编程组件解锁状态位，解锁ISP寄存器。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Unlock_ISP   (void)
{
    /* 写入KEY1 和 KEY2 */
    FLASH_NVMUNLOCK = FLASH_NVMUNLOCK_KEY1;
    FLASH_NVMUNLOCK = FLASH_NVMUNLOCK_KEY2;
}
/**
  * 描述  读FLASH解锁状态。
  * 输入  无。
  * 返回  锁定状态，1：解锁状态，0：锁定状态。
  */
LockStatus __attribute__((section(".indata")))
FLASH_Get_Flash_Unlock_Status_RAM (void)
{
    if (FLASH_ISPCON0 & FLASH_ISPCON0_FLASHLOCK)
    {
        /* 解锁状态 */
        return UNLOCK;
    }
    else
    {
        /* 锁定状态 */
        return LOCK;
    }
}

/**
  * 描述  清零FLASH解锁状态位。
  * 输入  无。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Clear_Flash_Unlock_Status_RAM (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPCON0寄存器FLASHLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | FLASH_ISPCON0_FLASHLOCK;
    FLASH_ISPCON0 = SFR_Config_RAM (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_FLASHLOCK),
                        tmpreg);
}
/**
  * 描述  清零FLASH解锁状态位。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Clear_Flash_Unlock_Status (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPCON0寄存器FLASHLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | FLASH_ISPCON0_FLASHLOCK;
    FLASH_ISPCON0 = SFR_Config (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_FLASHLOCK),
                        tmpreg);
}
/**
  * 描述  置位FLASH解锁状态位，解锁FLASH程序区。
  * 输入  无。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_Unlock_Code_RAM (void)
{
    /* 写入KEY1 和 KEY2 */
    FLASH_PROUNLOCK = FLASH_PROUNLOCK_KEY1;
    FLASH_PROUNLOCK = FLASH_PROUNLOCK_KEY2;
}

/**
  * 描述  置位FLASH解锁状态位，解锁FLASH程序区。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Unlock_Code (void)
{
    /* 写入KEY1 和 KEY2 */
    FLASH_PROUNLOCK = FLASH_PROUNLOCK_KEY1;
    FLASH_PROUNLOCK = FLASH_PROUNLOCK_KEY2;
}
/**
  * 描述  读FLASH配置区解锁状态。
  * 输入  无。
  * 返回  锁定状态，1：解锁状态，0：锁定状态。
  */
LockStatus __attribute__((section(".indata")))
FLASH_Get_Config_Unlock_Status_RAM (void)
{
    if (FLASH_ISPCON0 & FLASH_ISPCON0_CFGLOCK)
    {
        /* 解锁状态 */
        return UNLOCK;
    }
    else
    {
        /* 锁定状态 */
        return LOCK;
    }
}

/**
  * 描述  清零FLASH配置区解锁状态位。
  * 输入  无。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Clear_Config_Unlock_Status_RAM (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPCON0寄存器CFGLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | FLASH_ISPCON0_CFGLOCK;
    FLASH_ISPCON0 = SFR_Config_RAM (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_CFGLOCK),
                        tmpreg);
}
/**
  * 描述  清零FLASH配置区解锁状态位。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Clear_Config_Unlock_Status (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPCON0寄存器CFGLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | FLASH_ISPCON0_CFGLOCK;
    FLASH_ISPCON0 = SFR_Config (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_CFGLOCK),
                        tmpreg);
}
/**
  * 描述  置位FLASH配置区解锁状态位，用户配置区。
  * 输入  无。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_Unlock_User_Config_RAM (void)
{
    /* 写入KEY1 和 KEY2 */
    FLASH_CFGUNLOCK = FLASH_CFGUNLOCK_KEY1;
    FLASH_CFGUNLOCK = FLASH_CFGUNLOCK_KEY2;
}

/**
  * 描述  置位FLASH配置区解锁状态位，用户配置区。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Unlock_User_Config(void)
{
    /* 写入KEY1 和 KEY2 */
    FLASH_CFGUNLOCK = FLASH_CFGUNLOCK_KEY1;
    FLASH_CFGUNLOCK = FLASH_CFGUNLOCK_KEY2;
}
/**
  * 描述  配置FLASH写缓存使能。
  * 输入  NewState: FLASH写缓存使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_Data_Write_Enable_RAM (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置FLASH_ISPCON0寄存器DWEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* FLASH写缓存可以被写入数据 */
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_DWEN_POS));
    }
    else
    {
        /* FLASH写缓存禁止被写入数据 */
        __asm volatile("CLR [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_DWEN_POS));
    }
}

/**
  * 描述  配置FLASH信息区选择。
  * 输入  ZoneSelect: FLASH信息区选择，取值为：
  *                     FLASH_NVM_CODE_ZONE: FLASH程序区
  *                     FLASH_NVM_INFORMATION_ZONE: FLASH_NVM_CODE_ZONE
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Zone_Config_RAM (uint32_t ZoneSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_NVM_ZONE(ZoneSelect));

    /*---------------- 配置FLASH_ISPCON0寄存器IFEN位 ----------------*/
    if (ZoneSelect != FLASH_NVM_CODE_ZONE)
    {
        /* NVM操作存储器选择FLASH信息区 */
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_IFEN_POS));
    }
    else
    {
        /* NVM操作存储器选择FLASH程序区 */
        __asm volatile("CLR [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_IFEN_POS));
    }
}
/**
  * 描述  配置FLASH信息区选择。
  * 输入  ZoneSelect: FLASH信息区选择，取值为：
  *                     FLASH_NVM_CODE_ZONE: FLASH程序区
  *                     FLASH_NVM_INFORMATION_ZONE: FLASH_NVM_CODE_ZONE
  * 返回  无。
  */
void
FLASH_Zone_Config(uint32_t ZoneSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FLASH_NVM_ZONE(ZoneSelect));

    /*---------------- 配置FLASH_ISPCON0寄存器IFEN位 ----------------*/
    if (ZoneSelect != FLASH_NVM_CODE_ZONE)
    {
        /* NVM操作存储器选择FLASH信息区 */
        SFR_SET_BIT_ASM(FLASH_ISPCON0, FLASH_ISPCON0_IFEN_POS);
    }
    else
    {
        /* NVM操作存储器选择FLASH程序区 */
        SFR_CLR_BIT_ASM(FLASH_ISPCON0, FLASH_ISPCON0_IFEN_POS);
    }
}
/**
  * 描述  配置FLASH模式。
  * 输入  ModeSelect: FLASH信息区选择，取值为：
  *                     FLASH_MODE_NORMAL: 均为正常模式
  *                     FLASH_MODE_STANDBY1: STANDBY1 进入Stanby模式
  *                     FLASH_MODE_STANDBY2: STANDBY2 进入正常模式
  *                     FLASH_MODE_STANDBY1_STANDBY2: 均进入Stanby模式
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Standby_Mode_Config_RAM (uint32_t ModeSelect)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_MODE(ModeSelect));

    /*---------------- 配置FLASH_ISPCON0寄存器CFGLOCK位 ----------------*/
    tmpreg = FLASH_ISPCON0_HIGH | ModeSelect;
    FLASH_ISPCON0 = SFR_Config_RAM (FLASH_ISPCON0,
                        ~(FLASH_ISPCON0_HIGH_MSK | FLASH_ISPCON0_STANDBY1),
                        tmpreg);
}



/**
  * 描述  配置FLASH读模式。
  * 输入  ReadMode: FLASH读模式选择，取值为：
  *                   FLASH_READ_MODE_NORMAL: 正常模式
  *                   FLASH_READ_MODE_RECALL: RECALL模式
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Read_Mode_Config_RAM (uint32_t ReadMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_READ_MODE(ReadMode));

    /*---------------- 配置FLASH_ISPCON1寄存器RECALLEN位 ----------------*/
    if (ReadMode != FLASH_IP1)
    {
        /* FLASH读被设置为正常模式 */
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_ISPCON1)),"i"(FLASH_ISPCON1_RECALLEN_POS));
    }
    else
    {
        /* FLASH读被设置为RECALL模式 */
        __asm volatile("CLR [%0], #%1"::"r"(&(FLASH_ISPCON1)),"i"(FLASH_ISPCON1_RECALLEN_POS));
    }
}

/**
  * 描述  配置FLASH校准信息更新使能。
  * 输入  NewState: FLASH校准信息更新使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Calibration_Updata_Enable_RAM (FunctionalState NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置FLASH_ISPCON1寄存器CONFEN位 ----------------*/
    tmpreg = FLASH_ISPCON1_HIGH \
           | (NewState << FLASH_ISPCON1_CONFEN_POS);
    FLASH_ISPCON1 = SFR_Config_RAM (FLASH_ISPCON1,
                        ~(FLASH_ISPCON1_HIGH_MSK | FLASH_ISPCON1_CONFEN),
                        tmpreg);
}

/**
  * 描述  配置FLASH芯片信息区擦写解锁状态。
  * 输入  NewState: FLASH芯片信息区擦写解锁状态，取值为：
  *                   LOCK: 上锁状态
  *                   UNLOCK: 解锁状态
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Information_Zone_Wipe_Unlock_Config_RAM (LockStatus NewState)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_LOCK_STATUS(NewState));

    /*---------------- 配置FLASH_ISPCON1寄存器CONFEN位 ----------------*/
    tmpreg = FLASH_ISPCON1_HIGH \
           | (NewState << FLASH_ISPCON1_CHIPONEN_POS);
    FLASH_ISPCON1 = SFR_Config_RAM (FLASH_ISPCON1,
                        ~(FLASH_ISPCON1_HIGH_MSK | FLASH_ISPCON1_CHIPONEN),
                        tmpreg);
}

/**
  * 描述  配置FLASH半页编程模式下需要写入的字数。
  * 输入  WriteSize: 需要写入的字数，取值6位数值。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Half_Page_Write_Size_Config_RAM (uint32_t WriteSize)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_WRITE_SIZE(WriteSize));

    /*---------------- 配置FLASH_ISPCMD寄存器WSIZE位 ----------------*/
    tmpreg = WriteSize << FLASH_ISPCMD_WSIZE0_POS;
    FLASH_ISPCMD = SFR_Config_RAM (FLASH_ISPCMD,
                        ~FLASH_ISPCMD_WSIZE,
                        tmpreg);
}

/**
  * 描述  配置FLASH命令选择。
  * 输入  CmdSelect: FLASH命令选择，取值为：
  *                    FLASH_PROGRAM_CMD_WORD: 单字编程
  *                    FLASH_PROGRAM_CMD_ALL_CODE: 片擦
  *                    FLASH_PROGRAM_CMD_PAGE: 页擦
  *                    FLASH_PROGRAM_CMD_HALF_PAGE: 半页编程
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Program_Cmd_Config_RAM (uint32_t CmdSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_PROGRAM_CMD(CmdSelect));

    /*---------------- 配置FLASH_ISPCMD寄存器CMD位 ----------------*/
    FLASH_ISPCMD = SFR_Config_RAM (FLASH_ISPCMD,
                        ~FLASH_ISPCMD_CMD,
                        CmdSelect);
}

/**
  * 描述  执行当前FLASH_ISPCMD寄存器设定的命令，即FLASH_Program_Cmd_Config函数
  *       配置的命令。
  * 输入  无。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Executive_Cmd_RAM(void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPTRG寄存器ISPGO位 ----------------*/
    tmpreg = FLASH_ISPTRG_HIGH | FLASH_ISPTRG_ISPGO;
    FLASH_ISPTRG = SFR_Config_RAM (FLASH_ISPTRG,
                        ~(FLASH_ISPTRG_HIGH_MSK | FLASH_ISPTRG_ISPGO),
                        tmpreg);
}
/**
  * 描述  执行当前FLASH_ISPCMD寄存器设定的命令，即FLASH_Program_Cmd_Config函数
  *       配置的命令。
  * 输入  无。
  * 返回  无。
  */
void
FLASH_Executive_Cmd(void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_ISPTRG寄存器ISPGO位 ----------------*/
    tmpreg = FLASH_ISPTRG_HIGH | FLASH_ISPTRG_ISPGO;
    FLASH_ISPTRG = SFR_Config_RAM (FLASH_ISPTRG,
                        ~(FLASH_ISPTRG_HIGH_MSK | FLASH_ISPTRG_ISPGO),
                        tmpreg);
}
/**
  * 描述  配置FLASH非易失性存储器ECC使能。
  * 输入  NewState: FLASH非易失性存储器ECC使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_NonVolatile_Memory_ECC_Enable_RAM (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置FLASH_CFG寄存器ECCREADEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能ECC校验 */
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_CFG)),"i"(FLASH_CFG_ECCREADEN_POS));
    }
    else
    {
        /* 不使能ECC校验 */
        __asm volatile("CLR [%0], #%1"::"r"(&(FLASH_CFG)),"i"(FLASH_CFG_ECCREADEN_POS));
    }
}

/**
  * 描述  配置FLASH线性预取使能。
  * 输入  NewState: FLASH线性预取使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Linear_Prefetch_Enable_RAM (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置FLASH_CFG寄存器PREFETCHEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能线性预取 */
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_CFG)),"i"(FLASH_CFG_PREFETCHEN_POS));
    }
    else
    {
        /* 不使能线性预取 */
    	__asm volatile("CLR [%0], #%1"::"r"(&(FLASH_CFG)),"i"(FLASH_CFG_PREFETCHEN_POS));
    }
}

/**
  * 描述  配置FLASH访问周期。
  * 输入  PeriodNum: FLASH访问系统周期个数，取值为1~16。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Period_Number_Config_RAM (uint32_t PeriodNum)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_PERIOD(PeriodNum));

    /*---------------- 配置FLASH_CFG寄存器TCFG位 ----------------*/
    tmpreg = (PeriodNum - 1) << FLASH_CFG_TCFG0_POS;
    FLASH_CFG = SFR_Config_RAM (FLASH_CFG,
                        ~FLASH_CFG_TCFG,
                        tmpreg);
}

/**
  * 描述  配置FLASH编程和行擦期间的地址。
  * 输入  ProgramAddr: FLASH编程地址，取值为0x0~0xFFFFF，硬件忽略低2位。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Program_Addr_Config_RAM (uint32_t ProgramAddr)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_ADDR_20BITS(ProgramAddr));

    /*---------------- 配置FLASH_ISPADDR寄存器SADDR位 ----------------*/
    tmpreg = ProgramAddr << FLASH_ISPADDR_SADDR0_POS;
    FLASH_ISPADDR = SFR_Config_RAM (FLASH_ISPADDR,
                        ~FLASH_ISPADDR_SADDR,
                        tmpreg);
}

/**
  * 描述  读FLASH编程状态。   RAM
  * 输入  无。
  * 返回  编程状态，1：FLASH正处于编程状态，0：FLASH不在编程状态。
  */
FlagStatus  __attribute__((section(".indata")))
FLASH_Get_Program_Status_RAM (void)
{
    if (FLASH_STATE & FLASH_STATE_TBUSY)
    {
        /* 编程状态 */
        return SET;
    }
    else
    {
        /* 不在编程状态 */
        return RESET;
    }
}
/**
  * 描述  读FLASH编程状态。   RAM
  * 输入  无。
  * 返回  编程状态，1：FLASH正处于编程状态，0：FLASH不在编程状态。
  */
FlagStatus
FLASH_Get_Program_Status (void)
{
    if (FLASH_STATE & FLASH_STATE_TBUSY)
    {
        /* 编程状态 */
        return SET;
    }
    else
    {
        /* 不在编程状态 */
        return RESET;
    }
}



/**
  * 描述  读FLASH擦写状态。
  * 输入  无。
  * 返回  编程状态，1：ISP正在执行擦除或者写命令，0：ISP处于空闲状态。
  */
FlagStatus   __attribute__((section(".indata")))
FLASH_Get_Wipe_Status_RAM (void)
{
    if (FLASH_STATE & FLASH_STATE_TBIT)
    {
        /* 正在执行擦除或者写命令 */
        return SET;
    }
    else
    {
        /* 处于空闲状态 */
        return RESET;
    }
}

/**
  * 描述  读FLASH擦写状态。
  * 输入  无。
  * 返回  编程状态，1：ISP正在执行擦除或者写命令，0：ISP处于空闲状态。
  */
FlagStatus
FLASH_Get_Wipe_Status (void)
{
    if (FLASH_STATE & FLASH_STATE_TBIT)
    {
        /* 正在执行擦除或者写命令 */
        return SET;
    }
    else
    {
        /* 处于空闲状态 */
        return RESET;
    }
}
/**
  * 描述  读FLASH计算完成状态。
  * 输入  无。
  * 返回  编程状态，1：计算完成，0：未计算或者计算中。
  */
FlagStatus __attribute__((section(".indata")))
FLASH_Get_Compute_Complete_Status_RAM (void)
{
    if (FLASH_STATE & FLASH_STATE_SIGDONE)
    {
        /* 计算完成 */
        return SET;
    }
    else
    {
        /* 未计算或者计算中 */
        return RESET;
    }
}

/**
  * 描述  清零FLASH计算完成状态。
  * 输入  无。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Clear_Compute_Complete_Status_RAM (void)
{
    /* 写 1 清零 */
    while((FLASH_STATE & FLASH_STATE_SIGDONE)>>FLASH_STATE_SIGDONE_POS)
    {
    	__asm volatile("SET [%0], #%1"::"r"(&(FLASH_STATE)),"i"(FLASH_STATE_SIGDONE_POS));
    }
}

#if _CLOSE_SERVICE_

/**
  * 描述  读FLASH CFG编程错误标志。
  * 输入  无。
  * 返回  编程错误标志，1：CFG配置区编程错误，0：正常。
  */
FlagStatus  __attribute__((section(".indata")))
FLASH_Get_CFG_Error_Flag_RAM (void)
{
    if (FLASH_STATE & FLASH_STATE_CFGERROR)
    {
        /* CFG配置区编程错误 */
        return SET;
    }
    else
    {
        /* 正常 */
        return RESET;
    }
}

/**
  * 描述  清零FLASH CFG编程错误标志。
  * 输入  无。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_Clear_CFG_Error_Flag_RAM (void)
{
    /* 写 1 清零 */

    while((FLASH_STATE & FLASH_STATE_CFGERROR)>>FLASH_STATE_CFGERROR_POS)
    {
    	__asm volatile("SET [%0], #%1"::"r"(&(FLASH_STATE)),"i"(FLASH_STATE_CFGERROR_POS));
    }
}
#endif /* _CLOSE_SERVICE_ */

/**
  * 描述  配置FLASH CheckSum首尾地址。
  * 输入  StartAddr: FLASH CheckSum的首地址，地址区间为0x0~0xFFFFF，硬件忽略低4位。
  *       StopAddr: FLASH CheckSum的尾地址，地址区间为0x0~0xFFFFF，硬件忽略低4位。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_CheckSum_Addr_Config_RAM(uint32_t StartAddr, uint32_t StopAddr)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_ADDR_20BITS(StartAddr));
    CHECK_RESTRICTION_RAM(CHECK_FLASH_ADDR_20BITS(StopAddr));

    FLASH_CSSTART = StartAddr;
    FLASH_CSSTOP = StopAddr;
}

/**
  * 描述  配置FLASH启动SIG计算使能。
  * 输入  NewState: FLASH启动SIG计算使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void  __attribute__((section(".indata")))
FLASH_Start_SIG_Compute_Enable_RAM (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 配置FLASH_CSSTOP寄存器SIGGO位 ----------------*/
    if (NewState != FALSE)
    {
        /* 启动SIG计算 */
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_CSSTOP)),"i"(FLASH_CSSTOP_SIGGO_POS));
    }
    else
    {
        /* 不启动SIG计算 */
        __asm volatile("CLR [%0], #%1"::"r"(&(FLASH_CSSTOP)),"i"(FLASH_CSSTOP_SIGGO_POS));
    }
}

/**
  * 描述  获取FLASH CheckSum的结果。
  * 输入  CheckSumStruct: FLASH CheckSum信息结构体指针，
  *                       指针必须指向可写的地址空间。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Get_CheckSum_Result_RAM (FLASH_CheckSumResult* CheckSumStruct)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置FLASH_CSRESx寄存器CKSPRES位 ----------------*/
    tmpreg = FLASH_CSRES0;
    CheckSumStruct->m_ResultWord[0] = tmpreg;

    tmpreg = FLASH_CSRES1;
    CheckSumStruct->m_ResultWord[1] = tmpreg;

    tmpreg = FLASH_CSRES2;
    CheckSumStruct->m_ResultWord[2] = tmpreg;

    tmpreg = FLASH_CSRES3;
    CheckSumStruct->m_ResultWord[3] = tmpreg;
}
/**
  *   ##### FLASH程序存储器(FLASH)功能配置函数定义结束 #####
  */


/**
  *   ##### FLASH程序存储器(FLASH)编程功能函数 #####
  */
/**
  * 描述  FLASH擦除操作配置，用户需要确认FLASH擦写空闲状态。
  * 输入  WipeMode: FLASH 擦除模式，取值为：
  *                   FLASH_WIPE_CODE_PAGE: 程序区页擦
  *                   FLASH_WIPE_CFG_PAGE: 用户配置区页擦
  *                   FLASH_WIPE_CODE_ALL: 程序区片擦
  *       WipeAddr: 擦除地址，取值为0x0~0xFFFFF，硬件忽略低10位。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Wipe_Configuration_RAM (uint32_t WipeMode,uint32_t WipeAddr)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION_RAM(CHECK_FLASH_WIPE_MODE(WipeMode));
    CHECK_RESTRICTION_RAM(CHECK_FLASH_ADDR_20BITS(WipeAddr));



    /* 解锁ISP功能寄存器 */
    FLASH_Unlock_ISP_RAM();

	/* 解锁FLASH程序区 */
	FLASH_Unlock_Code_RAM();

	/* 解锁FLASH用户配置区 */
	FLASH_Unlock_User_Config_RAM();


    /* 设置待擦除的PAGE */
    if (WipeMode != FLASH_WIPE_CODE_ALL)
    {
        /* 页擦 */
        FLASH_ISPADDR = WipeAddr;


        if (FLASH_WIPE_CFG_PAGE == WipeMode)
        {

            /* 使能信息区映射 */
            /*---------------- 配置FLASH_ISPCON0寄存器IFEN位 ----------------*/
            __asm volatile("SET [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_IFEN_POS));
        }
        else
        {
            ;
        }
    }
    else
    {
        /* 片擦 */
    }
    /* 设置擦除模式 */
    if (WipeMode != FLASH_WIPE_CODE_ALL)
    {
        /* 页擦 */
        tmpreg = FLASH_PROGRAM_CMD_PAGE;
    }
    else
    {
        /* 片擦 */
        tmpreg = FLASH_PROGRAM_CMD_ALL_CODE;
    }
    /*---------------- 配置FLASH_ISPCMD寄存器CMD位 ----------------*/
    FLASH_ISPCMD = SFR_Config_RAM (FLASH_ISPCMD,
                        ~FLASH_ISPCMD_CMD,
                        tmpreg);



    /* 触发命令 */
    FLASH_Executive_Cmd_RAM();

    while(FLASH_Get_Wipe_Status_RAM());//等待写入完成

   	FLASH_Zone_Config_RAM (FLASH_NVM_CODE_ZONE);//回到程序区

    //上锁
	FLASH_Clear_NonVolatile_Memory_Unlock_Status_RAM ();//清零FLASH编程组件解锁状态位。
	FLASH_Clear_Flash_Unlock_Status_RAM ();//清零FLASH解锁状态位。
	FLASH_Clear_Config_Unlock_Status_RAM ();//清零FLASH配置区解锁状态位。


}

/**
  * 描述  FLASH擦除操作配置，用户需要确认FLASH擦写空闲状态。
  * 输入  WipeMode: FLASH 擦除模式，取值为：
  *                   FLASH_WIPE_CODE_PAGE: 程序区页擦
  *                   FLASH_WIPE_CFG_PAGE: 用户配置区页擦
  *                   FLASH_WIPE_CODE_ALL: 程序区片擦
  *       WipeAddr: 擦除地址，取值为0x0~0xFFFFF，硬件忽略低10位。
  * 返回  无。
  */
void
FLASH_Wipe_Configuration (uint32_t WipeMode,uint32_t WipeAddr)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FLASH_WIPE_MODE(WipeMode));
    CHECK_RESTRICTION(CHECK_FLASH_ADDR_20BITS(WipeAddr));



    /* 解锁ISP功能寄存器 */
    FLASH_Unlock_ISP();

	/* 解锁FLASH程序区 */
	FLASH_Unlock_Code();

	/* 解锁FLASH用户配置区 */
	FLASH_Unlock_User_Config();


    /* 设置待擦除的PAGE */
    if (WipeMode != FLASH_WIPE_CODE_ALL)
    {
        /* 页擦 */
        FLASH_ISPADDR = WipeAddr;


        if (FLASH_WIPE_CFG_PAGE == WipeMode)
        {

            /* 使能信息区映射 */
            /*---------------- 配置FLASH_ISPCON0寄存器IFEN位 ----------------*/
            SFR_SET_BIT_ASM(FLASH_ISPCON0, FLASH_ISPCON0_IFEN_POS);
        }
        else
        {
            ;
        }
    }
    else
    {
        /* 片擦 */
    }
    /* 设置擦除模式 */
    if (WipeMode != FLASH_WIPE_CODE_ALL)
    {
        /* 页擦 */
        tmpreg = FLASH_PROGRAM_CMD_PAGE;
    }
    else
    {
        /* 片擦 */
        tmpreg = FLASH_PROGRAM_CMD_ALL_CODE;
    }
    /*---------------- 配置FLASH_ISPCMD寄存器CMD位 ----------------*/
    FLASH_ISPCMD = SFR_Config (FLASH_ISPCMD,
                        ~FLASH_ISPCMD_CMD,
                        tmpreg);



    /* 触发命令 */
    FLASH_Executive_Cmd();

    while(FLASH_Get_Wipe_Status());//等待写入完成

   	FLASH_Zone_Config_RAM (FLASH_NVM_CODE_ZONE);//回到程序区

    //上锁
	FLASH_Clear_NonVolatile_Memory_Unlock_Status ();//清零FLASH编程组件解锁状态位。
	FLASH_Clear_Flash_Unlock_Status ();//清零FLASH解锁状态位。
	FLASH_Clear_Config_Unlock_Status ();//清零FLASH配置区解锁状态位。


}


/**
  * 描述  FLASH编程操作配置，用户需要确认FLASH擦写空闲状态。   RAM
  * 输入  flashProgramStruct: FLASH编程信息结构体指针。
  * 返回  无。
  */
void __attribute__((section(".indata")))
FLASH_Program_Configuration_RAM (FLASH_ProgramTypeDef * flashProgramStruct)
{
	uint32_t tmpreg = 0;
	uint32_t tmpaddr = 0;
	uint32_t * tmpptr = NULL;

    /* 参数校验 */

	CHECK_RESTRICTION_RAM(CHECK_FLASH_PROGRAM_MODE(flashProgramStruct->m_Mode));
	CHECK_RESTRICTION_RAM(CHECK_FLASH_PROGRAM_ZONE(flashProgramStruct->m_Zone));
	CHECK_RESTRICTION_RAM(CHECK_FLASH_ADDR_20BITS(flashProgramStruct->m_Addr));
	CHECK_RESTRICTION_RAM(CHECK_FLASH_WRITE_SIZE(flashProgramStruct->m_WriteSize));

    /* 解锁寄存器 */




    FLASH_Unlock_ISP_RAM();
    FLASH_Unlock_User_Config_RAM();
    FLASH_Unlock_Code_RAM();




    /*---------------- 配置FLASH_ISPCON0寄存器DWEN位 ----------------*/
    __asm volatile("SET [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_DWEN_POS));

    if (flashProgramStruct->m_Zone != FLASH_PROGRAM_CODE)
    {
        /*---------------- 配置FLASH_ISPCON0寄存器IFEN位 ----------------*/
        __asm volatile("SET [%0], #%1"::"r"(&(FLASH_ISPCON0)),"i"(FLASH_ISPCON0_IFEN_POS));

    }
    else
    {
    }

    /* 使用临时变量进行自增，避免修改结构体内容 */
	  tmpaddr = flashProgramStruct->m_Addr;
	  tmpptr =  flashProgramStruct->m_Data;

    /* 配置编程模式 */
    if (flashProgramStruct->m_Mode != FLASH_PROGRAM_WORD)
    {
        /* 半页编程 */
        /*----------- 配置FLASH_ISPCMD寄存器CMD/WSIZE位 -----------*/
        tmpreg = flashProgramStruct->m_Mode \
               | (flashProgramStruct->m_WriteSize << FLASH_ISPCMD_WSIZE0_POS);
        FLASH_ISPCMD = SFR_Config_RAM(FLASH_ISPCMD,
                            ~(FLASH_ISPCMD_CMD | FLASH_ISPCMD_WSIZE),
                            tmpreg);
        FLASH_Executive_Cmd_RAM();
		unsigned char j;
		for(j=0;j<=0x3f;j++)//0x3f最大值
		{
			__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));
			tmpaddr=tmpaddr+4;
			tmpptr++;
			__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));
			tmpaddr=tmpaddr+4;
			tmpptr++;
			while(FLASH_Get_Program_Status_RAM());
		}
    }
    else
    {
        /* 单字编程 */
        /*------------- 配置FLASH_ISPCMD寄存器CMD位 --------------*/
        FLASH_ISPCMD = SFR_Config_RAM (FLASH_ISPCMD,
                            ~FLASH_ISPCMD_CMD,
                            flashProgramStruct->m_Mode);
        /* 单字编程，先加载数据，后触发命令 */
        /* 加载64位数据 */

    	__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));
    	tmpaddr=tmpaddr+4;
    	tmpptr++;
    	__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));

        /* 触发命令 */

        FLASH_Executive_Cmd_RAM();

    }

    while(FLASH_Get_Wipe_Status_RAM());//等待写入完成

    /*---------------- 清零FLASH_ISPCON0寄存器DWEN位 ----------------*/


   	FLASH_Zone_Config_RAM (FLASH_NVM_CODE_ZONE);//回到程序区

   	FLASH_Data_Write_Enable_RAM (FALSE);
    //上锁
	FLASH_Clear_NonVolatile_Memory_Unlock_Status_RAM ();//清零FLASH编程组件解锁状态位。
	FLASH_Clear_Flash_Unlock_Status_RAM ();//清零FLASH解锁状态位。
	FLASH_Clear_Config_Unlock_Status_RAM ();//清零FLASH配置区解锁状态位。

}
/**
  * 描述  FLASH编程操作配置，用户需要确认FLASH擦写空闲状态。   RAM
  * 输入  flashProgramStruct: FLASH编程信息结构体指针。
  * 返回  无。
  */
void
FLASH_Program_Configuration (FLASH_ProgramTypeDef * flashProgramStruct)
{
	uint32_t tmpreg = 0;
	uint32_t tmpaddr = 0;
	uint32_t * tmpptr = NULL;

    /* 参数校验 */

	CHECK_RESTRICTION(CHECK_FLASH_PROGRAM_MODE(flashProgramStruct->m_Mode));
	CHECK_RESTRICTION(CHECK_FLASH_PROGRAM_ZONE(flashProgramStruct->m_Zone));
	CHECK_RESTRICTION(CHECK_FLASH_ADDR_20BITS(flashProgramStruct->m_Addr));
	CHECK_RESTRICTION(CHECK_FLASH_WRITE_SIZE(flashProgramStruct->m_WriteSize));

    /* 解锁寄存器 */




    FLASH_Unlock_ISP();
    FLASH_Unlock_User_Config();
    FLASH_Unlock_Code();




    /*---------------- 配置FLASH_ISPCON0寄存器DWEN位 ----------------*/
    SFR_SET_BIT_ASM(FLASH_ISPCON0, FLASH_ISPCON0_DWEN_POS);

    if (flashProgramStruct->m_Zone != FLASH_PROGRAM_CODE)
    {
        /*---------------- 配置FLASH_ISPCON0寄存器IFEN位 ----------------*/
        SFR_SET_BIT_ASM(FLASH_ISPCON0, FLASH_ISPCON0_IFEN_POS);

    }
    else
    {
    }

    /* 使用临时变量进行自增，避免修改结构体内容 */
	  tmpaddr = flashProgramStruct->m_Addr;
	  tmpptr =  flashProgramStruct->m_Data;

    /* 配置编程模式 */
    if (flashProgramStruct->m_Mode != FLASH_PROGRAM_WORD)
    {
        /* 半页编程 */
        /*----------- 配置FLASH_ISPCMD寄存器CMD/WSIZE位 -----------*/
        tmpreg = flashProgramStruct->m_Mode \
               | (flashProgramStruct->m_WriteSize << FLASH_ISPCMD_WSIZE0_POS);
        FLASH_ISPCMD = SFR_Config(FLASH_ISPCMD,
                            ~(FLASH_ISPCMD_CMD | FLASH_ISPCMD_WSIZE),
                            tmpreg);
        FLASH_Executive_Cmd();
		unsigned char j;
		for(j=0;j<=0x3f;j++)//0x3f最大值
		{
			__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));
			tmpaddr=tmpaddr+4;
			tmpptr++;
			__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));
			tmpaddr=tmpaddr+4;
			tmpptr++;
			while(FLASH_Get_Program_Status());
		}
    }
    else
    {
        /* 单字编程 */
        /*------------- 配置FLASH_ISPCMD寄存器CMD位 --------------*/
        FLASH_ISPCMD = SFR_Config (FLASH_ISPCMD,
                            ~FLASH_ISPCMD_CMD,
                            flashProgramStruct->m_Mode);
        /* 单字编程，先加载数据，后触发命令 */
        /* 加载64位数据 */

    	__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));
    	tmpaddr=tmpaddr+4;
    	tmpptr++;
    	__asm volatile("ST.w [%0], %1"::"r"(tmpaddr),"r"(*tmpptr));

        /* 触发命令 */

        FLASH_Executive_Cmd();

    }

    while(FLASH_Get_Wipe_Status());//等待写入完成

    /*---------------- 清零FLASH_ISPCON0寄存器DWEN位 ----------------*/


   	FLASH_Zone_Config (FLASH_NVM_CODE_ZONE);//回到程序区

   	FLASH_Data_Write_Enable_RAM (FALSE);
    //上锁
	FLASH_Clear_NonVolatile_Memory_Unlock_Status ();//清零FLASH编程组件解锁状态位。
	FLASH_Clear_Flash_Unlock_Status ();//清零FLASH解锁状态位。
	FLASH_Clear_Config_Unlock_Status ();//清零FLASH配置区解锁状态位。

}
/**
  *   ##### FLASH程序存储器(FLASH)编程功能函数定义结束 #####
  */

/**
  * 描述  读取flash的程序区或者信息区
  * 输入:address(读取地址）,ZoneSelect（读取区域选择）
  * 返回  输入指向地址的32位数据
  */
uint32_t read_data;
uint32_t __attribute__((section(".indata")))
Read_Flash_or_CFR_RAM (uint32_t address,uint32_t ZoneSelect)
{
	uint32_t *read_adr;
	CHECK_RESTRICTION_RAM(CHECK_FLASH_PROGRAM_ZONE(ZoneSelect));
	//解锁
	FLASH_Unlock_ISP_RAM();
	FLASH_Unlock_User_Config_RAM();
	FLASH_Unlock_Code_RAM();
	if(ZoneSelect!=FLASH_PROGRAM_CFG)
	{
		read_adr = (uint32_t *)address;
		read_data = *read_adr;
	}
	else
	{
		FLASH_Zone_Config_RAM (FLASH_NVM_INFORMATION_ZONE);//切到信息区
		read_adr = (uint32_t *)address;
		read_data = *read_adr;
		FLASH_Zone_Config_RAM (FLASH_NVM_CODE_ZONE);//回到程序区
	}
	 //上锁
	FLASH_Clear_NonVolatile_Memory_Unlock_Status ();//清零FLASH编程组件解锁状态位。
	FLASH_Clear_Flash_Unlock_Status ();//清零FLASH解锁状态位。
	FLASH_Clear_Config_Unlock_Status ();//清零FLASH配置区解锁状态位。
	return read_data;

}
uint32_t Read_Flash_or_CFR (uint32_t address,uint32_t ZoneSelect)
{
	uint32_t *read_adr;
	CHECK_RESTRICTION(CHECK_FLASH_PROGRAM_ZONE(ZoneSelect));
	//解锁
	FLASH_Unlock_ISP_RAM();
	FLASH_Unlock_User_Config_RAM();
	FLASH_Unlock_Code_RAM();
	if(ZoneSelect!=FLASH_PROGRAM_CFG)
	{
		read_adr = (uint32_t *)address;
		read_data = *read_adr;
	}
	else
	{
		FLASH_Zone_Config_RAM (FLASH_NVM_INFORMATION_ZONE);//切到信息区
		read_adr = (uint32_t *)address;
		read_data = *read_adr;
		FLASH_Zone_Config_RAM (FLASH_NVM_CODE_ZONE);//回到程序区
	}
	 //上锁
	FLASH_Clear_NonVolatile_Memory_Unlock_Status ();//清零FLASH编程组件解锁状态位。
	FLASH_Clear_Flash_Unlock_Status ();//清零FLASH解锁状态位。
	FLASH_Clear_Config_Unlock_Status ();//清零FLASH配置区解锁状态位。
	return read_data;

}
