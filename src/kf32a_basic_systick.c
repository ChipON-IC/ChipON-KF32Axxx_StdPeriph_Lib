/**
  ******************************************************************************
  * 文件名  kf32a_basic_systick.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.6
  * 日  期  2019-11-16
  * 描  述  该文件提供了节拍定时器外设功能函数，包含：
  *          + 节拍定时器(SYSTICK)外设初始化函数定义
  *          + 节拍定时器(SYSTICK)功能配置函数定义
  *
  *********************************************************************
  */

#include "kf32a_basic_systick.h"

/*
 * 说明   该文件未考虑STCALIB校验寄存器信息，STCALIB校验寄存器变动可能
 *        会导致节拍定时器的时钟异常。KungFu不建议用户修改该寄存器。
 */


/* 节拍定时器私有定义 ----------------------------------------------------*/
/* ST_CTL寄存器配置掩码 */
#define SYSTICK_CTL_INIT_MASK           (ST_CTL_TICKINTEN \
                                       | ST_CTL_STCLKS)

/**
  *   ##### 节拍定时器(SYSTICK)外设初始化函数定义 #####
  */
/**
  * 描述  节拍定时器(SYSTICK)配置。
  * 输入  Clock: 系统节拍定时器时钟源选择，
  *              取值范围为：
  *                SYSTICK_SYS_CLOCK_DIV_1: SCLK作为时钟
  *                SYSTICK_SYS_CLOCK_DIV_2:  SCLK/2作为时钟
  *       SysTickINT: SysTick中断使能控制，
  *                   取值范围为：TRUE 或 FALSE。
  *       Reload: 系统节拍定时器重加载值，节拍定时器的周期值为重加载值加1。
  *               取值为24位数据。
  * 返回  无
  */
void
SYSTICK_Configuration (uint32_t Clock, uint32_t SysTickINT,
                    uint32_t Reload)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SYSTICK_CLOCK(Clock));
    CHECK_RESTRICTION(CHECK_SYSTICK_RELOAD(Reload));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(SysTickINT));

    /*------------ 禁止节拍定时器，防止意外中断 -------------*/
    SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_STEN_POS);

    /*--------------- 设置ST_RELOAD寄存器 ---------------*/
    ST_RELOAD = Reload;

    /*-- 设置ST_CV寄存器，以清零当前值寄存器并清零COUNTZERO标志位，
         确保重载值能加载到当前值寄存器中 -------------------*/
    ST_CV = 0;

    /*------------------ 设置ST_CTL寄存器 -----------------*/
    /* 根据参数Clock，设置STCLKS位域 */
    /* 根据参数SysTickINT，设置TICKINTEN位域 */
    tmpreg = Clock | (SysTickINT << ST_CTL_TICKINTEN_POS);
    ST_CTL = SFR_Config (ST_CTL,
                          ~SYSTICK_CTL_INIT_MASK,
                          tmpreg);
}

/**
  *   ##### 节拍定时器(SYSTICK)外设初始化函数定义结束 #####
  */


/**
  *   ##### 节拍定时器(SYSTICK)功能配置函数定义 #####
  */
/**
  * 描述  节拍定时器(SYSTICK)启动控制使能。
  * 输入  NewState: 节拍定时器使能控制，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无
  */
void
SYSTICK_Cmd (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置ST_CTL寄存器STEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能系统节拍定时器 */
        SFR_SET_BIT_ASM(ST_CTL, ST_CTL_STEN_POS);
    }
    else
    {
        /* 禁止系统节拍定时器 */
        SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_STEN_POS);
    }
}

/**
  * 描述  节拍定时器(SYSTICK)时钟源配置。
  * 输入  SysClock: 系统节拍定时器时钟源选择，
  *                 取值范围为：
  *                   SYSTICK_SYS_CLOCK_DIV_1: SCLK作为时钟
  *                   SYSTICK_SYS_CLOCK_DIV_2:  SCLK/2作为时钟
  * 返回  无
  */
void
SYSTICK_Clock_Config (uint32_t SysClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SYSTICK_CLOCK(SysClock));

    /*---------------- 设置ST_CTL寄存器STCLKS位 ----------------*/
    if (SysClock != SYSTICK_SYS_CLOCK_DIV_2)
    {
        /*  选择SCLK作为时钟 */
        SFR_SET_BIT_ASM(ST_CTL, ST_CTL_STCLKS_POS);
    }
    else
    {
        /*  选择SCLK/2作为时钟 */
        SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_STCLKS_POS);
    }
}

/**
  * 描述  节拍定时器(SYSTICK)SysTick中断使能配置。
  * 输入  SysClock: 系统节拍定时器SysTick中断使能状态，
  *                 取值范围为：TRUE 或 FALSE。
  * 返回  无
  */
void
SYSTICK_Systick_INT_Enable (uint32_t SysClock)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(SysClock));

    /*---------------- 设置ST_CTL寄存器TICKINTEN位 ----------------*/
    if (SysClock != FALSE)
    {
        /*  计数到0发送SysTick中断请求 */
        SFR_SET_BIT_ASM(ST_CTL, ST_CTL_TICKINTEN_POS);
    }
    else
    {
        /*  计数到0不发送SysTick中断请求 */
        SFR_CLR_BIT_ASM(ST_CTL, ST_CTL_TICKINTEN_POS);
    }
}

/**
  * 描述  读取节拍定时器计数零标志。
  * 输入  无。
  * 返回  节拍定时器计数到零的标志，0：计数未到零，1：计数到零。
  */
FlagStatus
SYSTICK_Get_Count_Zero_Flag (void)
{
    /*-------------------- 设置ST_CTL寄存器COUNTZERO位 --------------------*/
    if (ST_CTL & ST_CTL_COUNTZERO)
    {
        /* 计数到零 */
        return SET;
    }
    else
    {
        /* 计数未到零 */
        return RESET;
    }
}

/**
  * 描述  节拍定时器(SYSTICK)重加载寄存器配置。
  * 输入  Reload: 系统节拍定时器重加载值，节拍定时器的周期值为重加载值加1。
  *               取值为24位数据。
  * 返回  无
  */
void
SYSTICK_Reload_Config (uint32_t Reload)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_SYSTICK_RELOAD(Reload));

    /*--------------- 设置ST_RELOAD寄存器 ---------------*/
    ST_RELOAD = Reload;
}

/**
  * 描述  节拍定时器(SYSTICK)计数器配置，该函数会清零COUNTZERO位，
  *       并将STRELOAD值能加载到计数器寄存器中。
  * 输入  无。
  * 返回  无。
  */
void
SYSTICK_Counter_Updata (void)
{
    /*-- 向ST_CV寄存器写任意值，以清零当前值寄存器，
         并清零COUNTZERO标志位，确保重载值能加载到当前值寄存器中 --*/
    ST_CV = 0;
}

/**
  * 描述  读取系统节拍定时器重加载寄存器。
  * 输入  无。
  * 返回  重加载寄存器的值，24位有效数据。
  */
uint32_t
SYSTICK_Get_Reload (void)
{
    uint32_t tmpreg = 0;

    /* 读取ST_RELOAD寄存器 */
    tmpreg = ST_RELOAD & ST_RELOAD_STRELOAD;

    return tmpreg;
}

/**
  * 描述  读取系统节拍定时器计数器。
  * 输入  无。
  * 返回  当前计数值，24位有效数据。
  */
uint32_t
SYSTICK_Get_Counter (void)
{
    uint32_t tmpreg = 0;

    /* 读取ST_CV寄存器 */
    tmpreg = ST_CV & ST_CV_STCV;

    return tmpreg;
}
/**
  *   ##### 节拍定时器(SYSTICK)功能配置函数定义结束 #####
  */
