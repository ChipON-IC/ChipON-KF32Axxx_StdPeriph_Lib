/**
  ******************************************************************************
  * 文件名  kf32a_basic_rtc.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了实时时钟(RTC)相关的功能函数，包含：
  *          + 实时时钟(RTC)功能初始化函数
  *          + 实时时钟(RTC)控制功能函数
  *          + 实时时钟(RTC)闹钟配置函数
  *          + 实时时钟(RTC)日期时间配置函数
  *          + 实时时钟(RTC)定时器配置函数
  *          + 实时时钟(RTC)中断管理函数
  *          + 实时时钟(RTC)私有函数
  *
  *********************************************************************
  */

#include "kf32a_basic_osc.h"
#include "kf32a_basic_bkp.h"
#include "kf32a_basic_rtc.h"

/* 实时时钟(RTC)私有宏定义 --------------------------------------------------*/
/* RTC_TMR寄存器初始化配置掩码 */
#define RTC_TMR_INIT_MASK               (RTC_TMR_WEK \
                                       | RTC_TMR_AMPM \
                                       | RTC_TMR_HOUR \
                                       | RTC_TMR_MIN \
                                       | RTC_TMR_SEC)
/* RTC_DTR寄存器初始化配置掩码 */
#define RTC_DTR_INIT_MASK               (RTC_DTR_DAY \
                                       | RTC_DTR_MTH \
                                       | RTC_DTR_YEAR)
/* RTC_ALRA/B寄存器初始化配置掩码 */
#define RTC_ALR_INIT_MASK               (RTC_ALRA_ALRSA \
                                       | RTC_ALRA_ASENA \
                                       | RTC_ALRA_ALRMA \
                                       | RTC_ALRA_AMENA \
                                       | RTC_ALRA_ALRHA \
                                       | RTC_ALRA_AMPMA \
                                       | RTC_ALRA_AHENA \
                                       | RTC_ALRA_ALRWA \
                                       | RTC_ALRA_AWENA)


/* 实时时钟(RTC)私有函数 ----------------------------------------------------*/
static uint8_t RTC_Byte_To_Bcd (uint8_t Value);
static uint8_t RTC_Bcd_To_Byte (uint8_t Value);


/**
  *   ##### 实时时钟(RTC)功能初始化函数 #####
  */
/**
  * 描述  复位RTC模块。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Reset (void)
{
    /* 操作RTC模块前，配置相关寄存器 */
    SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);
    SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS);
    SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);

    /* 复位RTC模块 */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
    /* 退出复位RTC模块 */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
}

/**
  * 描述  日期时间初始化配置，该函数结束时会退出配置模式。
  * 输入  TimeFormat: 日期时间数值格式，选择是否为BCD码，取值范围为：
  *                     RTC_TIME_FORMAT_BCD: BCD编码方式，
  *                     RTC_TIME_FORMAT_BIN: 不使用BCD编码。
  *       rtcInitStruct: 初始化信息结构体指针。
  * 返回  无。
  */
void
RTC_Configuration (uint32_t TimeFormat, RTC_InitTypeDef * rtcInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));
    CHECK_RESTRICTION(CHECK_RTC_SOURCE(rtcInitStruct->m_ClockSource));
    CHECK_RESTRICTION(CHECK_RTC_HOURFORMAT(rtcInitStruct->m_HourFormat));
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12(rtcInitStruct->m_TimeStruct.m_Hours));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcInitStruct->m_TimeStruct.m_AMPM));
        }
        else
        {
            rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_AM;
            CHECK_RESTRICTION(CHECK_RTC_HOUR24(rtcInitStruct->m_TimeStruct.m_Hours));
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES(rtcInitStruct->m_TimeStruct.m_Minutes));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS(rtcInitStruct->m_TimeStruct.m_Seconds));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcInitStruct->m_DateStruct.m_WeekDay));
        CHECK_RESTRICTION(CHECK_RTC_DATE(rtcInitStruct->m_DateStruct.m_Day));
        CHECK_RESTRICTION(CHECK_RTC_MONTH_DEC(rtcInitStruct->m_DateStruct.m_Month));
        CHECK_RESTRICTION(CHECK_RTC_YEAR(rtcInitStruct->m_DateStruct.m_Year));
    }
    else
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Hours)));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcInitStruct->m_TimeStruct.m_AMPM));
        }
        else
        {
            rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_AM;
            CHECK_RESTRICTION(CHECK_RTC_HOUR24( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Hours)));
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Minutes)));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_TimeStruct.m_Seconds)));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcInitStruct->m_DateStruct.m_WeekDay));
        CHECK_RESTRICTION(CHECK_RTC_DATE( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_DateStruct.m_Day)));
        CHECK_RESTRICTION(CHECK_RTC_MONTH_BCD(rtcInitStruct->m_DateStruct.m_Month));
        CHECK_RESTRICTION(CHECK_RTC_YEAR( \
                        RTC_Bcd_To_Byte(rtcInitStruct->m_DateStruct.m_Year)));
    }

    /* 进入配置模式 */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);

    /*-------------------- 设置RTC_TMR寄存器 --------------------*/
    /* 根据结构体成员m_TimeStruct.m_WeekDay，设置WEK位域 */
    /* 根据结构体成员m_TimeStruct.m_Hours，设置HOUR位域 */
    /* 根据结构体成员m_TimeStruct.m_Minutes，设置MIN位域 */
    /* 根据结构体成员m_TimeStruct.m_Seconds，设置SEC位域 */
    /* 根据结构体成员m_TimeStruct.m_AMPM，设置AMPM位域 */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        tmpreg = ((uint32_t)rtcInitStruct->m_DateStruct.m_WeekDay \
                    << RTC_TMR_WEK0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_TimeStruct.m_Hours) \
                    << RTC_TMR_HOUR0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_TimeStruct.m_Minutes) \
                    << RTC_TMR_MIN0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_TimeStruct.m_Seconds) \
                    << RTC_TMR_SEC0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_AMPM << RTC_TMR_AMPM_POS);
    }
    else
    {
        tmpreg = ((uint32_t)rtcInitStruct->m_DateStruct.m_WeekDay << RTC_TMR_WEK0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_Hours << RTC_TMR_HOUR0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_Minutes << RTC_TMR_MIN0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_Seconds << RTC_TMR_SEC0_POS) \
               | ((uint32_t)rtcInitStruct->m_TimeStruct.m_AMPM << RTC_TMR_AMPM_POS);
    }
    RTC_TMR = SFR_Config (RTC_TMR,
                          ~RTC_TMR_INIT_MASK,
                          tmpreg);

    /*-------------------- 设置RTC_DTR寄存器 --------------------*/
    /* 根据结构体成员m_DateStruct.m_Day，设置DAY位域 */
    /* 根据结构体成员m_DateStruct.m_Month，设置MTH位域 */
    /* 根据结构体成员m_DateStruct.m_Year，设置YEAR位域 */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        tmpreg = ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_DateStruct.m_Month) \
                    << RTC_DTR_MTH0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_DateStruct.m_Day) \
                    << RTC_DTR_DAY0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcInitStruct->m_DateStruct.m_Year) \
                    << RTC_DTR_YEAR0_POS);
    }
    else
    {
        tmpreg = ((uint32_t)rtcInitStruct->m_DateStruct.m_Month << RTC_DTR_MTH0_POS) \
               | ((uint32_t)rtcInitStruct->m_DateStruct.m_Day << RTC_DTR_DAY0_POS) \
               | ((uint32_t)rtcInitStruct->m_DateStruct.m_Year << RTC_DTR_YEAR0_POS);
    }
    RTC_DTR = SFR_Config (RTC_DTR,
                          ~RTC_DTR_INIT_MASK,
                          tmpreg);

    /*-------------------- 设置RTC_CR寄存器 --------------------*/
    /* 根据结构体成员m_HourFormat，设置HT位域 */
    tmpreg = rtcInitStruct->m_HourFormat;
    RTC_CR = SFR_Config (RTC_CR,
                          ~RTC_CR_HT,
                          tmpreg);

    /*-------------------- 设置BKP_CTL寄存器 --------------------*/
    /* 根据结构体成员m_ClockSource，设置RTCCLKS位域 */
    BKP_CTL = SFR_Config (BKP_CTL,
                          ~BKP_CTL_RTCCLKS,
                          rtcInitStruct->m_ClockSource);

    /* 退出配置模式 */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
}

/**
  * 描述  初始化时间信息结构体。
  * 输入  rtcTimeInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
RTC_Time_Struct_Init (RTC_TimeTypeDef* rtcTimeInitStruct)
{
    /* 时间: AM 00h:00m:00s */
    /* 设置时钟 */
    rtcTimeInitStruct->m_Hours = 0;
    /* 设置分钟 */
    rtcTimeInitStruct->m_Minutes = 0;
    /* 设置秒钟 */
    rtcTimeInitStruct->m_Seconds = 0;
    /* 设置AMPM */
    rtcTimeInitStruct->m_AMPM = RTC_TIME_AM;
}

/**
  * 描述  初始化日期信息结构体。
  * 输入  rtcDateInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
RTC_Date_Struct_Init (RTC_DateTypeDef* rtcDateInitStruct)
{
    /* 日期: xx00/1/1 周一*/
    /* 设置周 */
    rtcDateInitStruct->m_WeekDay = RTC_WEEKDAY_MONDAY_BCD;
    /* 设置年 */
    rtcDateInitStruct->m_Year = 0;
    /* 设置月 */
    rtcDateInitStruct->m_Month = RTC_MONTH_JANUARY_BCD;
    /* 设置日 */
    rtcDateInitStruct->m_Day = 1;
}

/**
  * 描述  初始化配置信息结构体。
  * 输入  rtcInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
RTC_Struct_Init (RTC_InitTypeDef * rtcInitStruct)
{
    /* 设置RTC时钟源 */
    rtcInitStruct->m_ClockSource = BKP_RTC_NO_CLK;
    /* 设置RTC时间格式 */
    rtcInitStruct->m_HourFormat = RTC_HOUR_FORMAT_24;
    /* 设置时钟 */
    rtcInitStruct->m_TimeStruct.m_Hours = 0;
    /* 设置分钟 */
    rtcInitStruct->m_TimeStruct.m_Minutes = 0;
    /* 设置秒钟 */
    rtcInitStruct->m_TimeStruct.m_Seconds = 0;
    /* 设置AMPM */
    rtcInitStruct->m_TimeStruct.m_AMPM = RTC_TIME_AM;
    /* 设置周 */
    rtcInitStruct->m_DateStruct.m_WeekDay = RTC_WEEKDAY_MONDAY_BCD;
    /* 设置年 */
    rtcInitStruct->m_DateStruct.m_Year = 0;
    /* 设置月 */
    rtcInitStruct->m_DateStruct.m_Month = RTC_MONTH_JANUARY_BCD;
    /* 设置日 */
    rtcInitStruct->m_DateStruct.m_Day = 1;
}

/**
  * 描述  获取时间信息。
  * 输入  TimeFormat: 日期时间数值格式，选择是否为BCD码，取值范围为：
  *                     RTC_TIME_FORMAT_BCD: BCD编码方式，
  *                     RTC_TIME_FORMAT_BIN: 不使用BCD编码。
  *       rtcTimeStruct: 时间信息存储指针。
  * 返回  无。
  */
void
RTC_Get_Time_Configuration (uint32_t TimeFormat,
                    RTC_TimeTypeDef* rtcTimeStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));

    /* 获取RTC_TMR寄存器 */
    tmpreg = RTC_TMR;

    /* 获取时间信息 */
    rtcTimeStruct->m_Hours = (uint8_t)((tmpreg & RTC_TMR_HOUR) \
                            >> RTC_TMR_HOUR0_POS);
    rtcTimeStruct->m_Minutes = (uint8_t)((tmpreg & RTC_TMR_MIN) \
                              >> RTC_TMR_MIN0_POS);
    rtcTimeStruct->m_Seconds = (uint8_t)((tmpreg & RTC_TMR_SEC) \
                              >> RTC_TMR_SEC0_POS);
    rtcTimeStruct->m_AMPM = (uint8_t)((tmpreg & RTC_TMR_AMPM) \
                           >> RTC_TMR_AMPM_POS);

    /* BCD编码转为数值 */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        rtcTimeStruct->m_Hours = RTC_Bcd_To_Byte(rtcTimeStruct->m_Hours);
        rtcTimeStruct->m_Minutes = RTC_Bcd_To_Byte(rtcTimeStruct->m_Minutes);
        rtcTimeStruct->m_Seconds = RTC_Bcd_To_Byte(rtcTimeStruct->m_Seconds);
    }
}

/**
  * 描述  获取日期信息。
  * 输入  TimeFormat: 日期时间数值格式，选择是否为BCD码，取值范围为：
  *                     RTC_TIME_FORMAT_BCD: BCD编码方式，
  *                     RTC_TIME_FORMAT_BIN: 不使用BCD编码。
  *       rtcDateStruct: 日期信息存储指针。
  * 返回  无。
  */
void
RTC_Get_Date_Configuration (uint32_t TimeFormat,
                    RTC_DateTypeDef* rtcDateStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));

    /* 获取RTC_TMR寄存器 */
    tmpreg = RTC_TMR;

    /* 获取周信息 */
    rtcDateStruct->m_WeekDay = (uint8_t)((tmpreg & RTC_TMR_WEK) \
                              >> RTC_TMR_WEK0_POS);

    /* 获取RTC_DTR寄存器 */
    tmpreg = RTC_DTR;

    /* 获取时间信息 */
    rtcDateStruct->m_Year = (uint8_t)((tmpreg & RTC_DTR_YEAR) \
                           >> RTC_DTR_YEAR0_POS);
    rtcDateStruct->m_Month = (uint8_t)((tmpreg & RTC_DTR_MTH) \
                           >> RTC_DTR_MTH0_POS);
    rtcDateStruct->m_Day = (uint8_t)((tmpreg & RTC_DTR_DAY) \
                           >> RTC_DTR_DAY0_POS);

    /* BCD编码转为数值 */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        rtcDateStruct->m_Year = RTC_Bcd_To_Byte(rtcDateStruct->m_Year);
        rtcDateStruct->m_Month = RTC_Bcd_To_Byte(rtcDateStruct->m_Month);
        rtcDateStruct->m_Day = RTC_Bcd_To_Byte(rtcDateStruct->m_Day);
    }
}

/**
  * 描述  闹钟初始化配置，配置此函数前需配置HT位，
  *       通过RTC_Hour_Format_Config函数实现。
  * 输入  AlarmSelect: 闹钟选择，取值范围为：
  *                      RTC_ALARM_A_ADDR_OFFSET: 闹钟A，
  *                      RTC_ALARM_B_ADDR_OFFSET: 闹钟B。
  *       TimeFormat: 日期时间数值格式，选择是否为BCD码，取值范围为：
  *                     RTC_TIME_FORMAT_BCD: BCD编码方式，
  *                     RTC_TIME_FORMAT_BIN: 不使用BCD编码。
  *       rtcAlarmInitStruct: 闹钟信息结构体指针。
  * 返回  无。
  */
void
RTC_Alarm_Configuration (uint32_t AlarmSelect,
                    uint32_t TimeFormat,
                    RTC_AlarmTypeDef* rtcAlarmInitStruct)
{
    uint32_t tmpreg = 0;
    uint32_t tmpaddr = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_ALARM(AlarmSelect));
    CHECK_RESTRICTION(CHECK_RTC_TIME_FORMAT(TimeFormat));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_WeekDayEnable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_HoursEnable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_MinutesEnable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(rtcAlarmInitStruct->m_SecondsEnable));

    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12(rtcAlarmInitStruct->m_Hours));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcAlarmInitStruct->m_AMPM));
        }
        else
        {
            rtcAlarmInitStruct->m_AMPM = RTC_TIME_AM;
            CHECK_RESTRICTION(CHECK_RTC_HOUR24(rtcAlarmInitStruct->m_Hours));
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES(rtcAlarmInitStruct->m_Minutes));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS(rtcAlarmInitStruct->m_Seconds));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcAlarmInitStruct->m_WeekDay));
    }
    else
    {
        if ((RTC_CR & RTC_CR_HT) != RTC_HOUR_FORMAT_24)
        {
            CHECK_RESTRICTION(CHECK_RTC_HOUR12( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Hours)));
            CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(rtcAlarmInitStruct->m_AMPM));
        }
        else
        {
            rtcAlarmInitStruct->m_AMPM = RTC_TIME_AM;
            CHECK_RESTRICTION(CHECK_RTC_HOUR24( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Hours)));
        }
        CHECK_RESTRICTION(CHECK_RTC_MINUTES( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Minutes)));
        CHECK_RESTRICTION(CHECK_RTC_SECONDS( \
                        RTC_Bcd_To_Byte(rtcAlarmInitStruct->m_Seconds)));
        CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(rtcAlarmInitStruct->m_WeekDay));
    }

    /* 进入配置模式 */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);

    /* 根据AlarmSelect，选择对应的闹钟控制寄存器 */
    tmpaddr = (uint32_t)&RTC_ALRA;
    tmpaddr += AlarmSelect;

    /*-------------------- 设置RTC_ALRx寄存器 --------------------*/
    /* 根据结构体成员m_WeekDayEnable，设置AWENx位域 */
    /* 根据结构体成员m_HoursEnable，设置AHENx位域 */
    /* 根据结构体成员m_MinutesEnable，设置AMENx位域 */
    /* 根据结构体成员m_SecondsEnable，设置ASEN位域 */
    /* 根据结构体成员m_AMPM，设置AMPMx位域 */
    /* 根据结构体成员m_WeekDay，设置ALRWx位域 */
    /* 根据结构体成员m_Hours，设置ALRHx位域 */
    /* 根据结构体成员m_Minutes，设置ALRMx位域 */
    /* 根据结构体成员m_Seconds，设置ALRSx位域 */
    if (TimeFormat != RTC_TIME_FORMAT_BCD)
    {
        tmpreg = (rtcAlarmInitStruct->m_WeekDayEnable << RTC_ALRA_AWENA_POS) \
               | (rtcAlarmInitStruct->m_HoursEnable << RTC_ALRA_AHENA_POS) \
               | (rtcAlarmInitStruct->m_MinutesEnable << RTC_ALRA_AMENA_POS) \
               | (rtcAlarmInitStruct->m_SecondsEnable << RTC_ALRA_ASENA_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_AMPM << RTC_TMR_AMPM_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_WeekDay << RTC_ALRA_ALRWA0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcAlarmInitStruct->m_Hours) \
                    << RTC_ALRA_ALRHA0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcAlarmInitStruct->m_Minutes) \
                    << RTC_ALRA_ALRMA0_POS) \
               | ((uint32_t)RTC_Byte_To_Bcd(rtcAlarmInitStruct->m_Seconds) \
                    << RTC_ALRA_ALRSA0_POS);
    }
    else
    {
        tmpreg = (rtcAlarmInitStruct->m_WeekDayEnable << RTC_ALRA_AWENA_POS) \
               | (rtcAlarmInitStruct->m_HoursEnable << RTC_ALRA_AHENA_POS) \
               | (rtcAlarmInitStruct->m_MinutesEnable << RTC_ALRA_AMENA_POS) \
               | (rtcAlarmInitStruct->m_SecondsEnable << RTC_ALRA_ASENA_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_AMPM << RTC_TMR_AMPM_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_WeekDay << RTC_ALRA_ALRWA0_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_Hours << RTC_ALRA_ALRHA0_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_Minutes << RTC_ALRA_ALRMA0_POS) \
               | ((uint32_t)rtcAlarmInitStruct->m_Seconds << RTC_ALRA_ALRSA0_POS);
    }

    *(volatile uint32_t *)tmpaddr =
                    SFR_Config (*(volatile uint32_t *)tmpaddr,
                                ~RTC_ALR_INIT_MASK,
                                tmpreg);

    /* 退出配置模式 */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
}

/**
  * 描述  初始化闹钟信息结构体。
  * 输入  rtcAlarmInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
RTC_Alarm_Struct_Init (RTC_AlarmTypeDef* rtcAlarmInitStruct)
{
    /* 设置AM/PM */
    rtcAlarmInitStruct->m_AMPM = RTC_TIME_AM;
    /* 设置周 */
    rtcAlarmInitStruct->m_WeekDay = RTC_WEEKDAY_MONDAY_BCD;
    /* 使能周 */
    rtcAlarmInitStruct->m_WeekDayEnable = FALSE;
    /* 设置时 */
    rtcAlarmInitStruct->m_Hours = 0;
    /* 使能时 */
    rtcAlarmInitStruct->m_HoursEnable = FALSE;
    /* 设置分 */
    rtcAlarmInitStruct->m_Minutes = 0;
    /* 使能分 */
    rtcAlarmInitStruct->m_MinutesEnable = FALSE;
    /* 设置秒 */
    rtcAlarmInitStruct->m_Seconds = 0;
    /* 使能秒 */
    rtcAlarmInitStruct->m_SecondsEnable = FALSE;
}
/**
  *   ##### 实时时钟(RTC)功能初始化函数定义结束 #####
  */


/**
  *   ##### 实时时钟(RTC)控制功能函数 #####
  */
/**
  * 描述  配置RTC时钟校正值。
  * 输入  Calibration: RTC时钟偏差的值，取值为8位有符号数值。
  * 返回  无。
  */
void
RTC_Clock_Calibration_Config (int8_t Calibration)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置RTC_CR寄存器RTCCAL位 ----------------*/
    tmpreg = (uint32_t)Calibration << RTC_CR_RTCCAL0_POS;
    RTC_CR = SFR_Config (RTC_CR, ~RTC_CR_RTCCAL, tmpreg);
}

/**
  * 描述  设置RTC的周期时间节拍（TT）输出使能。
  * 输入  NewState: RTC的周期时间节拍（TT）输出使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Time_Tick_Output_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_CR寄存器TTOE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能TT输出 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_TTOE_POS);
    }
    else
    {
        /* 禁止TT输出 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_TTOE_POS);
    }
}

/**
  * 描述  设置时间戳触发沿。
  * 输入  TimeStamp: 时间戳触发沿，取值为：
  *                    RTC_TIME_STAMP_RISE: 上升沿触发
  *                    RTC_TIME_STAMP_FALL: 下降沿触发
  * 返回  无。
  */
void
RTC_Time_Stamp_Edge_Config (uint32_t TimeStamp)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_STAMP_EDGE(TimeStamp));

    /*---------------- 设置RTC_CR寄存器TSEDGE位 ----------------*/
    if (TimeStamp != RTC_TIME_STAMP_RISE)
    {
        /* 下降沿触发 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_TSEDGE_POS);
    }
    else
    {
        /* 上升沿触发 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_TSEDGE_POS);
    }
}

/**
  * 描述  设置时间戳触发沿使能。
  * 输入  NewState: 时间戳触发沿使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Time_Stamp_Edge_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_CR寄存器TSEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能时间戳功能 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_TSEN_POS);
    }
    else
    {
        /* 未使能时间戳功能 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_TSEN_POS);
    }
}

/**
  * 描述  设置RTC时间增加1小时使能。
  * 输入  NewState: RTC时间增加1小时使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Add_One_Hour_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_CR寄存器ADD1H位 ----------------*/
    if (NewState != FALSE)
    {
        /* RTC时间增加1小时 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_ADD1H_POS);
    }
    else
    {
        /* RTC正常计时 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_ADD1H_POS);
    }
}

/**
  * 描述  设置RTC时间减少1小时使能。
  * 输入  NewState: RTC时间减少1小时使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Sub_One_Hour_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_CR寄存器SUB1H位 ----------------*/
    if (NewState != FALSE)
    {
        /* RTC时间减少1小时 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_SUB1H_POS);
    }
    else
    {
        /* RTC正常计时 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_SUB1H_POS);
    }
}

/**
  * 描述  配置时间节拍。
  * 输入  TimeTick: RTC时间节拍，取值为:
  *                   RTC_TIME_TICK_DIV_1: 时间节拍为1秒
  *                   RTC_TIME_TICK_DIV_2: 时间节拍为1/2秒
  *                   RTC_TIME_TICK_DIV_4: 时间节拍为1/4秒
  *                   RTC_TIME_TICK_DIV_8: 时间节拍为1/8秒
  *                   RTC_TIME_TICK_DIV_16: 时间节拍为1/16秒
  *                   RTC_TIME_TICK_DIV_32: 时间节拍为1/32秒
  *                   RTC_TIME_TICK_DIV_64: 时间节拍为1/64秒
  *                   RTC_TIME_TICK_DIV_128: 时间节拍为1/128秒
  * 返回  无。
  */
void
RTC_Time_Tick_Config (uint32_t Calibration)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_TICK(Calibration));

    /*---------------- 配置RTC_CR寄存器RTCTT位 ----------------*/
    RTC_CR = SFR_Config (RTC_CR, ~RTC_CR_RTCTT, Calibration);
}

/**
  * 描述  启动实时时钟。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Start_Config (void)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置RTC_CR寄存器RTCSRT位 ----------------*/
    tmpreg = 0x55 << RTC_CR_RTCSRT0_POS;
    RTC_CR = SFR_Config (RTC_CR, ~RTC_CR_RTCSRT, tmpreg);
}

/**
  * 描述  初始化实时时钟模块。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Reset_Config (void)
{
    /*---------------- 配置RTC_CR寄存器RESET位 ----------------*/
    /* 复位RTC模块 */
    SFR_SET_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
    /* 退出复位RTC模块 */
    SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_RESET_POS);
}

/**
  * 描述  获取闰年指示标志。
  * 输入  无。
  * 返回  1:当前年份为闰年；0:当前年份为平年。
  */
FlagStatus
RTC_Get_Leap_Year_Flag (void)
{
    /*---------------- 读取RTC_CR寄存器LIF位 ----------------*/
    if (RTC_CR & RTC_CR_LIF)
    {
        /* 当前年份为闰年 */
        return SET;
    }
    else
    {
        /* 当前年份为平年 */
        return RESET;
    }
}

/**
  * 描述  设置小时显示的类型。
  * 输入  HourFormat: RTC小时显示的类型，取值为:
  *                     RTC_HOUR_FORMAT_24: 24小时制，
  *                     RTC_HOUR_FORMAT_12: 12小时制。
  * 返回  无。
  */
void
RTC_Hour_Format_Config (uint32_t HourFormat)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_HOURFORMAT(HourFormat));

    /*---------------- 设置RTC_CR寄存器HT位 ----------------*/
    if (HourFormat != RTC_HOUR_FORMAT_24)
    {
        /* 12小时制 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_HT_POS);
    }
    else
    {
        /* 24小时制 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_HT_POS);
    }
}

/**
  * 描述  设置配置模式使能。
  * 输入  ConfigMode: 配置模式状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Config_Mode_Enable (FunctionalState ConfigMode)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ConfigMode));

    /*---------------- 设置RTC_CR寄存器CNF位 ----------------*/
    if (ConfigMode != FALSE)
    {
        /* 进入配置模式 */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
    }
    else
    {
        /* 退出配置模式 */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_CNF_POS);
    }
}

/**
  * 描述  获取RTC操作关闭指示标志。
  * 输入  无。
  * 返回  1:上一次对RTC寄存器的写操作已经完成；
  *       0:上一次对RTC寄存器的写操作仍在进行。
  */
FlagStatus
RTC_Get_Operation_Off_Flag (void)
{
    /*---------------- 读取RTC_CR寄存器RTOFF位 ----------------*/
    if (RTC_CR & RTC_CR_RTOFF)
    {
        /* 上一次对RTC寄存器的写操作已经完成 */
        return SET;
    }
    else
    {
        /* 上一次对RTC寄存器的写操作仍在进行 */
        return RESET;
    }
}

/**
  * 描述  获取RTC活动状态 。
  * 输入  无。
  * 返回  1:RTC处在正常有效状态；
  *       0:RTC处在复位状态。
  */
FlagStatus
RTC_Get_Action_State_Flag (void)
{
    /*---------------- 读取RTC_CR寄存器RTCLD位 ----------------*/
    if (RTC_CR & RTC_CR_RTCLD)
    {
        /* RTC处在正常有效状态 */
        return SET;
    }
    else
    {
        /* RTC处在复位状态 */
        return RESET;
    }
}

/**
  * 描述  设置RTC使能。
  * 输入  NewState: RTC使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_CR寄存器RTCEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能RTC */
        SFR_SET_BIT_ASM(RTC_CR, RTC_CR_RTCEN_POS);
    }
    else
    {
        /* 禁止RTC */
        SFR_CLR_BIT_ASM(RTC_CR, RTC_CR_RTCEN_POS);
    }
}
/**
  *   ##### 实时时钟(RTC)控制功能函数定义结束 #####
  */


/**
  *   ##### 实时时钟(RTC)闹钟配置函数 #####
  */
/**
  * 描述  设置闹钟A使能。
  * 输入  NewState: 闹钟A使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_A_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRA寄存器ALRENA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟A功能 */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_ALRENA_POS);
    }
    else
    {
        /* 禁止闹钟A功能 */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_ALRENA_POS);
    }
}

/**
  * 描述  设置闹钟A周使能。
  * 输入  NewState: 闹钟A周使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_A_Weekday_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRA寄存器AWENA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟A周位 */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AWENA_POS);
    }
    else
    {
        /* 禁止闹钟A周位 */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AWENA_POS);
    }
}

/**
  * 描述  配置闹钟中断星期时间。
  * 输入  Weekday: 闹钟中断星期时间，取值为:
  *                  RTC_WEEKDAY_MONDAY: 星期一
  *                  RTC_WEEKDAY_TUESDAY: 星期二
  *                  RTC_WEEKDAY_WEDNESDAY: 星期三
  *                  RTC_WEEKDAY_THURSDAY: 星期四
  *                  RTC_WEEKDAY_FRIDAY: 星期五
  *                  RTC_WEEKDAY_SATURDAY: 星期六
  *                  RTC_WEEKDAY_SUNDAY: 星期天
  * 返回  无。
  */
void
RTC_Alarm_A_Weekday_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- 配置RTC_ALRA寄存器ALRWA位 ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_ALRA_ALRWA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRWA, tmpreg);
}

/**
  * 描述  设置闹钟A时钟使能。
  * 输入  NewState: 闹钟A时钟使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_A_Hours_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRA寄存器AHENA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟A时钟位 */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AHENA_POS);
    }
    else
    {
        /* 禁止闹钟A时钟位 */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AHENA_POS);
    }
}

/**
  * 描述  设置闹钟A上午下午选择。
  * 输入  NewSelect: 闹钟A上午下午选择，取值为:
  *                    RTC_TIME_AM: 选择上午时间，
  *                    RTC_TIME_PM: 选择下午时间。
  * 返回  无。
  */
void
RTC_Alarm_A_AMPM_Config (uint32_t NewSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- 设置RTC_ALRA寄存器AMPMA位 ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* 选择下午时间 */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AMPMA_POS);
    }
    else
    {
        /* 选择上午时间 */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AMPMA_POS);
    }
}

/**
  * 描述  配置闹钟中断小时时间，用户控制匹配12小时制或24小时制。
  * 输入  Hour: 闹钟中断小时时间，取值匹配12小时制或24小时制。
  * 返回  无。
  */
void
RTC_Alarm_A_Hours_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- 配置RTC_ALRA寄存器ALRHA位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_ALRA_ALRHA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRHA, tmpreg);
}

/**
  * 描述  设置闹钟A分钟使能。
  * 输入  NewState: 闹钟A分钟使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_A_Minutes_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRA寄存器AMENA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟A分钟位 */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_AMENA_POS);
    }
    else
    {
        /* 禁止闹钟A分钟位 */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_AMENA_POS);
    }
}

/**
  * 描述  配置闹钟中断分钟时间。
  * 输入  Minutes: 闹钟中断分钟时间，取值为0-59。
  * 返回  无。
  */
void
RTC_Alarm_A_Minutes_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- 配置RTC_ALRA寄存器ALRMA位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_ALRA_ALRMA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRMA, tmpreg);
}

/**
  * 描述  设置闹钟A秒使能。
  * 输入  NewState: 闹钟A秒使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_A_Seconds_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRA寄存器ASENA位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟A秒钟位 */
        SFR_SET_BIT_ASM(RTC_ALRA, RTC_ALRA_ASENA_POS);
    }
    else
    {
        /* 禁止闹钟A秒钟位 */
        SFR_CLR_BIT_ASM(RTC_ALRA, RTC_ALRA_ASENA_POS);
    }
}

/**
  * 描述  配置闹钟中断秒时间。
  * 输入  Seconds: 闹钟中断秒时间，取值为0-59。
  * 返回  无。
  */
void
RTC_Alarm_A_Seconds_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- 配置RTC_ALRA寄存器ALRSA位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_ALRA_ALRSA0_POS;
    RTC_ALRA = SFR_Config (RTC_ALRA, ~RTC_ALRA_ALRSA, tmpreg);
}

/**
  * 描述  设置闹钟B使能。
  * 输入  NewState: 闹钟B使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_B_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRB寄存器ALRENB位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟B功能 */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_ALRENB_POS);
    }
    else
    {
        /* 禁止闹钟B功能 */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_ALRENB_POS);
    }
}

/**
  * 描述  设置闹钟B周使能。
  * 输入  NewState: 闹钟B周使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_B_Weekday_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRB寄存器AWENB位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟B周位 */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AWENB_POS);
    }
    else
    {
        /* 禁止闹钟B周位 */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AWENB_POS);
    }
}

/**
  * 描述  配置闹钟中断星期时间。
  * 输入  Weekday: 闹钟中断星期时间，取值为:
  *                  RTC_WEEKDAY_MONDAY: 星期一
  *                  RTC_WEEKDAY_TUESDAY: 星期二
  *                  RTC_WEEKDAY_WEDNESDAY: 星期三
  *                  RTC_WEEKDAY_THURSDAY: 星期四
  *                  RTC_WEEKDAY_FRIDAY: 星期五
  *                  RTC_WEEKDAY_SATURDAY: 星期六
  *                  RTC_WEEKDAY_SUNDAY: 星期天
  * 返回  无。
  */
void
RTC_Alarm_B_Weekday_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- 配置RTC_ALRB寄存器ALRWB位 ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_ALRB_ALRWB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRWB, tmpreg);
}

/**
  * 描述  设置闹钟B时钟使能。
  * 输入  NewState: 闹钟B时钟使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_B_Hours_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRB寄存器AHENB位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟B时钟位 */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AHENB_POS);
    }
    else
    {
        /* 禁止闹钟B时钟位 */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AHENB_POS);
    }
}

/**
  * 描述  设置闹钟B上午下午选择。
  * 输入  NewSelect: 闹钟B上午下午选择，取值为:
  *                    RTC_TIME_AM: 选择上午时间，
  *                    RTC_TIME_PM: 选择下午时间。
  * 返回  无。
  */
void
RTC_Alarm_B_AMPM_Config (uint32_t NewSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- 设置RTC_ALRB寄存器AMPMB位 ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* 选择下午时间 */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AMPMB_POS);
    }
    else
    {
        /* 选择上午时间 */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AMPMB_POS);
    }
}

/**
  * 描述  配置闹钟中断小时时间，用户控制匹配12小时制或24小时制。
  * 输入  Hour: 闹钟中断小时时间，取值匹配12小时制或24小时制。
  * 返回  无。
  */
void
RTC_Alarm_B_Hours_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- 配置RTC_ALRB寄存器ALRHB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_ALRB_ALRHB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRHB, tmpreg);
}

/**
  * 描述  设置闹钟B分钟使能。
  * 输入  NewState: 闹钟B分钟使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_B_Minutes_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRB寄存器AMENB位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟B分钟位 */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_AMENB_POS);
    }
    else
    {
        /* 禁止闹钟B分钟位 */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_AMENB_POS);
    }
}

/**
  * 描述  配置闹钟中断分钟时间。
  * 输入  Minutes: 闹钟中断分钟时间，取值为0-59。
  * 返回  无。
  */
void
RTC_Alarm_B_Minutes_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- 配置RTC_ALRB寄存器ALRMB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_ALRB_ALRMB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRMB, tmpreg);
}

/**
  * 描述  设置闹钟B秒使能。
  * 输入  NewState: 闹钟B秒使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_B_Seconds_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_ALRB寄存器ASENB位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟B秒钟位 */
        SFR_SET_BIT_ASM(RTC_ALRB, RTC_ALRB_ASENB_POS);
    }
    else
    {
        /* 禁止闹钟B秒钟位 */
        SFR_CLR_BIT_ASM(RTC_ALRB, RTC_ALRB_ASENB_POS);
    }
}

/**
  * 描述  配置闹钟中断秒时间。
  * 输入  Seconds: 闹钟中断秒时间，取值为0-59。
  * 返回  无。
  */
void
RTC_Alarm_B_Seconds_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- 配置RTC_ALRB寄存器ALRSB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_ALRB_ALRSB0_POS;
    RTC_ALRB = SFR_Config (RTC_ALRB, ~RTC_ALRB_ALRSB, tmpreg);
}
/**
  *   ##### 实时时钟(RTC)闹钟配置函数定义结束 #####
  */


/**
  *   ##### 实时时钟(RTC)日期时间配置函数 #####
  */
/**
  * 描述  配置RTC时钟星期。
  * 输入  Weekday: RTC时钟星期，取值为:
  *                  RTC_WEEKDAY_MONDAY: 星期一
  *                  RTC_WEEKDAY_TUESDAY: 星期二
  *                  RTC_WEEKDAY_WEDNESDAY: 星期三
  *                  RTC_WEEKDAY_THURSDAY: 星期四
  *                  RTC_WEEKDAY_FRIDAY: 星期五
  *                  RTC_WEEKDAY_SATURDAY: 星期六
  *                  RTC_WEEKDAY_SUNDAY: 星期天
  * 返回  无。
  */
void
RTC_Weekday_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- 配置RTC_TMR寄存器WEK位 ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_TMR_WEK0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_WEK, tmpreg);
}

/**
  * 描述  设置RTC时钟上午下午选择。
  * 输入  NewSelect: RTC时钟上午下午选择，取值为:
  *                    RTC_TIME_AM: 选择上午时间，
  *                    RTC_TIME_PM: 选择下午时间。
  * 返回  无。
  */
void
RTC_AMPM_Config (uint32_t NewSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- 设置RTC_TMR寄存器AMPM位 ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* 选择下午时间 */
        SFR_SET_BIT_ASM(RTC_TMR, RTC_TMR_AMPM_POS);
    }
    else
    {
        /* 选择上午时间 */
        SFR_CLR_BIT_ASM(RTC_TMR, RTC_TMR_AMPM_POS);
    }
}

/**
  * 描述  配置RTC时钟时钟时间。
  * 输入  Hour: RTC时钟时钟时间，取值匹配12小时制或24小时制。
  * 返回  无。
  */
void
RTC_Hours_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- 配置RTC_TMR寄存器HOUR位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_TMR_HOUR0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_HOUR, tmpreg);
}

/**
  * 描述  配置RTC时钟分钟时间。
  * 输入  Minutes: RTC时钟分钟时间，取值为0-59。
  * 返回  无。
  */
void
RTC_Minutes_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- 配置RTC_TMR寄存器MIN位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_TMR_MIN0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_MIN, tmpreg);
}

/**
  * 描述  配置RTC时钟秒钟时间。
  * 输入  Seconds: RTC时钟秒钟时间，取值为0-59。
  * 返回  无。
  */
void
RTC_Seconds_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- 配置RTC_TMR寄存器SEC位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_TMR_SEC0_POS;
    RTC_TMR = SFR_Config (RTC_TMR, ~RTC_TMR_SEC, tmpreg);
}

/**
  * 描述  配置RTC时钟年份。
  * 输入  Year: RTC时钟年份，取值为0-99。
  * 返回  无。
  */
void
RTC_Year_Config (uint32_t Year)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_YEAR(Year));

    /*---------------- 配置RTC_DTR寄存器YEAR位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Year);
    tmpreg <<= RTC_DTR_YEAR0_POS;
    RTC_DTR = SFR_Config (RTC_DTR, ~RTC_DTR_YEAR, tmpreg);
}

/**
  * 描述  配置RTC时钟月份。
  * 输入  Month: RTC时钟月份，取值为:
  *                RTC_MONTH_JANUARY: 1月
  *                RTC_MONTH_FEBRUARY: 2月
  *                RTC_MONTH_MARCH: 3月
  *                RTC_MONTH_APRIL: 4月
  *                RTC_MONTH_MAY: 5月
  *                RTC_MONTH_JUNE: 6月
  *                RTC_MONTH_JULY: 7月
  *                RTC_MONTH_AUGUST: 8月
  *                RTC_MONTH_SEPTEMBER: 9月
  *                RTC_MONTH_OCTOBER: 10月
  *                RTC_MONTH_NOVEMBER: 11月
  *                RTC_MONTH_DECEMBER: 12月
  * 返回  无。
  */
void
RTC_Month_Config (uint32_t Month)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_MONTH_BCD(Month));

    /*---------------- 配置RTC_DTR寄存器MTH位 ----------------*/
    tmpreg = Month << RTC_DTR_MTH0_POS;
    RTC_DTR = SFR_Config (RTC_DTR, ~RTC_DTR_MTH, tmpreg);
}

/**
  * 描述  配置RTC时钟日期。
  * 输入  Day: RTC时钟日期，取值为1-31。
  * 返回  无。
  */
void
RTC_Day_Config (uint32_t Day)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_DATE(Day));

    /*---------------- 配置RTC_DTR寄存器DAY位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Day);
    tmpreg <<= RTC_DTR_DAY0_POS;
    RTC_DTR = SFR_Config (RTC_DTR, ~RTC_DTR_DAY, tmpreg);
}

/**
  * 描述  配置RTC时钟星期备份。
  * 输入  Weekday: RTC时钟星期备份，取值为:
  *                  RTC_WEEKDAY_MONDAY: 星期一
  *                  RTC_WEEKDAY_TUESDAY: 星期二
  *                  RTC_WEEKDAY_WEDNESDAY: 星期三
  *                  RTC_WEEKDAY_THURSDAY: 星期四
  *                  RTC_WEEKDAY_FRIDAY: 星期五
  *                  RTC_WEEKDAY_SATURDAY: 星期六
  *                  RTC_WEEKDAY_SUNDAY: 星期天
  * 返回  无。
  */
void
RTC_Weekday_Backup_Config (uint8_t Weekday)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_WEEKDAY(Weekday));

    /*---------------- 配置RTC_TMBR寄存器WEKB位 ----------------*/
    tmpreg = (uint32_t)Weekday << RTC_TMBR_WEKB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_WEKB, tmpreg);
}

/**
  * 描述  设置RTC时钟上午下午选择备份。
  * 输入  NewSelect: RTC时钟上午下午选择备份，取值为:
  *                    RTC_TIME_AM: 选择上午时间，
  *                    RTC_TIME_PM: 选择下午时间。
  * 返回  无。
  */
void
RTC_AMPM_Backup_Config (uint32_t NewSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIME_AMPM(NewSelect));

    /*---------------- 设置RTC_TMBR寄存器AMPMB位 ----------------*/
    if (NewSelect != RTC_TIME_AM)
    {
        /* 选择下午时间 */
        SFR_SET_BIT_ASM(RTC_TMBR, RTC_TMBR_AMPMB_POS);
    }
    else
    {
        /* 选择上午时间 */
        SFR_CLR_BIT_ASM(RTC_TMBR, RTC_TMBR_AMPMB_POS);
    }
}

/**
  * 描述  配置RTC时钟时钟时间备份。
  * 输入  Hour: RTC时钟时钟时间备份，取值匹配12小时制或24小时制。
  * 返回  无。
  */
void
RTC_Hours_Backup_Config (uint32_t Hour)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_HOUR24(Hour));

    /*---------------- 配置RTC_TMBR寄存器HOURB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Hour);
    tmpreg <<= RTC_TMBR_HOURB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_HOURB, tmpreg);
}

/**
  * 描述  配置RTC时钟分钟时间备份。
  * 输入  Minutes: RTC时钟分钟时间备份，取值为0-59。
  * 返回  无。
  */
void
RTC_Minutes_Backup_Config (uint32_t Minutes)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_MINUTES(Minutes));

    /*---------------- 配置RTC_TMBR寄存器MINB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Minutes);
    tmpreg <<= RTC_TMBR_MINB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_MINB, tmpreg);
}

/**
  * 描述  配置RTC时钟秒钟时间备份。
  * 输入  Seconds: RTC时钟秒钟时间备份，取值为0-59。
  * 返回  无。
  */
void
RTC_Seconds_Backup_Config (uint32_t Seconds)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_SECONDS(Seconds));

    /*---------------- 配置RTC_TMBR寄存器SECB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Seconds);
    tmpreg <<= RTC_TMBR_SECB0_POS;
    RTC_TMBR = SFR_Config (RTC_TMBR, ~RTC_TMBR_SECB, tmpreg);
}

/**
  * 描述  配置RTC时钟年份备份。
  * 输入  Year: RTC时钟年份备份，取值为0-99。
  * 返回  无。
  */
void
RTC_Year_Backup_Config (uint32_t Year)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_YEAR(Year));

    /*---------------- 配置RTC_DTBR寄存器YEARB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Year);
    tmpreg <<= RTC_DTBR_YEARB0_POS;
    RTC_DTBR = SFR_Config (RTC_DTBR, ~RTC_DTBR_YEARB, tmpreg);
}

/**
  * 描述  配置RTC时钟月份备份。
  * 输入  Month: RTC时钟月份备份，取值为:
  *                RTC_MONTH_JANUARY: 1月
  *                RTC_MONTH_FEBRUARY: 2月
  *                RTC_MONTH_MARCH: 3月
  *                RTC_MONTH_APRIL: 4月
  *                RTC_MONTH_MAY: 5月
  *                RTC_MONTH_JUNE: 6月
  *                RTC_MONTH_JULY: 7月
  *                RTC_MONTH_AUGUST: 8月
  *                RTC_MONTH_SEPTEMBER: 9月
  *                RTC_MONTH_OCTOBER: 10月
  *                RTC_MONTH_NOVEMBER: 11月
  *                RTC_MONTH_DECEMBER: 12月
  * 返回  无。
  */
void
RTC_Month_Backup_Config (uint32_t Month)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_MONTH_BCD(Month));

    /*---------------- 配置RTC_DTBR寄存器MTHB位 ----------------*/
    tmpreg = Month << RTC_DTBR_MTHB0_POS;
    RTC_DTBR = SFR_Config (RTC_DTBR, ~RTC_DTBR_MTHB, tmpreg);
}

/**
  * 描述  配置RTC时钟日期备份。
  * 输入  Day: RTC时钟日期备份，取值为1-31。
  * 返回  无。
  */
void
RTC_Day_Backup_Config (uint32_t Day)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_DATE(Day));

    /*---------------- 配置RTC_DTBR寄存器DAYB位 ----------------*/
    tmpreg = RTC_Byte_To_Bcd(Day);
    tmpreg <<= RTC_DTBR_DAYB0_POS;
    RTC_DTBR = SFR_Config (RTC_DTBR, ~RTC_DTBR_DAYB, tmpreg);
}
/**
  *   ##### 实时时钟(RTC)日期时间配置函数定义结束 #####
  */


/**
  *   ##### 实时时钟(RTC)定时器配置函数 #####
  */
/**
  * 描述  配置RTC定时器1计数值。
  * 输入  Counter: RTC定时器1计数值，取值为16位数值。
  * 返回  无。
  */
void
RTC_Timer1_Config (uint16_t Counter)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置RTC_TMER寄存器TMR1位 ----------------*/
    tmpreg = (uint32_t)Counter << RTC_TMER_TMR1_0_POS;
    RTC_TMER = SFR_Config (RTC_TMER, ~RTC_TMER_TMR1, tmpreg);
}

/**
  * 描述  配置RTC定时器0计数值。
  * 输入  Counter: RTC定时器0计数值，取值为16位数值。
  * 返回  无。
  */
void
RTC_Timer0_Config (uint16_t Counter)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置RTC_TMER寄存器TMR0位 ----------------*/
    tmpreg = (uint32_t)Counter << RTC_TMER_TMR0_0_POS;
    RTC_TMER = SFR_Config (RTC_TMER, ~RTC_TMER_TMR0, tmpreg);
}

/**
  * 描述  设置RTC定时器1使能。
  * 输入  TimerEnable: RTC定时器1使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Timer1_Enable (FunctionalState TimerEnable)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(TimerEnable));

    /*---------------- 设置RTC_TCR寄存器TMR1EN位 ----------------*/
    if (TimerEnable != FALSE)
    {
        /* 使能定时器1 */
        SFR_SET_BIT_ASM(RTC_TCR, RTC_TCR_TMR1EN_POS);
    }
    else
    {
        /* 禁止定时器1 */
        SFR_CLR_BIT_ASM(RTC_TCR, RTC_TCR_TMR1EN_POS);
    }
}

/**
  * 描述  设置RTC定时器0使能。
  * 输入  TimerEnable: RTC定时器0使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Timer0_Enable (FunctionalState TimerEnable)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(TimerEnable));

    /*---------------- 设置RTC_TCR寄存器TMR0EN位 ----------------*/
    if (TimerEnable != FALSE)
    {
        /* 使能定时器0 */
        SFR_SET_BIT_ASM(RTC_TCR, RTC_TCR_TMR0EN_POS);
    }
    else
    {
        /* 禁止定时器0 */
        SFR_CLR_BIT_ASM(RTC_TCR, RTC_TCR_TMR0EN_POS);
    }
}

/**
  * 描述  配置定时器1时钟源选择。
  * 输入  ClockSource: 定时器1时钟源选择，取值为:
  *                      RTC_TIMER_CLOCK_RTC_DIV_128: RTC时钟源/128 约1/256s
  *                      RTC_TIMER_CLOCK_RTC_DIV_512: RTC时钟源/512 约1/64s
  *                      RTC_TIMER_CLOCK_RTC_DIV_1024: RTC时钟源/1024 约1/32s
  *                      RTC_TIMER_CLOCK_RTC_DIV_2048: RTC时钟源/2048 约1/16s
  *                      RTC_TIMER_CLOCK_RTC_DIV_8192: RTC时钟源/8192 约1/4s
  *                      RTC_TIMER_CLOCK_ONE_SECOND: 1s
  *                      RTC_TIMER_CLOCK_ONE_MINUTE: 1min
  *                      RTC_TIMER_CLOCK_ONE_HOUR: 1hour
  *                      RTC_TIMER_CLOCK_CONNECTION: 计数器TMR1和TMR0级联
  * 返回  无。
  */
void
RTC_Timer1_Source_Config (uint16_t ClockSource)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIMER1_CLOCK(ClockSource));

    /*---------------- 配置RTC_TCR寄存器TMR1C位 ----------------*/
    tmpreg = (uint32_t)ClockSource << RTC_TCR_TMR1C0_POS;
    RTC_TCR = SFR_Config (RTC_TCR, ~RTC_TCR_TMR1C, tmpreg);
}

/**
  * 描述  配置定时器0时钟源选择。
  * 输入  ClockSource: 定时器0时钟源选择，取值为:
  *                      RTC_TIMER_CLOCK_RTC_DIV_128: RTC时钟源/128 约1/256s
  *                      RTC_TIMER_CLOCK_RTC_DIV_512: RTC时钟源/512 约1/64s
  *                      RTC_TIMER_CLOCK_RTC_DIV_1024: RTC时钟源/1024 约1/32s
  *                      RTC_TIMER_CLOCK_RTC_DIV_2048: RTC时钟源/2048 约1/16s
  *                      RTC_TIMER_CLOCK_RTC_DIV_8192: RTC时钟源/8192 约1/4s
  *                      RTC_TIMER_CLOCK_ONE_SECOND: 1s
  *                      RTC_TIMER_CLOCK_ONE_MINUTE: 1min
  *                      RTC_TIMER_CLOCK_ONE_HOUR: 1hour
  * 返回  无。
  */
void
RTC_Timer0_Source_Config (uint16_t ClockSource)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_RTC_TIMER0_CLOCK(ClockSource));

    /*---------------- 配置RTC_TCR寄存器TMR0C位 ----------------*/
    tmpreg = (uint32_t)ClockSource << RTC_TCR_TMR0C0_POS;
    RTC_TCR = SFR_Config (RTC_TCR, ~RTC_TCR_TMR0C, tmpreg);
}

/**
  *   ##### 实时时钟(RTC)定时器配置函数定义结束 #####
  */


/**
  *   ##### 实时时钟(RTC)中断管理函数 #####
  */
/**
  * 描述  设置RTC时间戳中断使能。
  * 输入  NewState: RTC时间戳中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Time_Stamp_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器TSIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能时间戳中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_TSIE_POS);
    }
    else
    {
        /* 禁止时间戳中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_TSIE_POS);
    }
}

/**
  * 描述  设置RTC时间戳溢出中断使能。
  * 输入  NewState: RTC时间戳溢出中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Time_Stamp_Overflow_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器TSOVFIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能时间戳溢出中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_TSOVFIE_POS);
    }
    else
    {
        /* 禁止时间戳溢出中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_TSOVFIE_POS);
    }
}

/**
  * 描述  设置RTC定时器1中断使能。
  * 输入  NewState: RTC定时器1中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Timer1_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器RTCT1IE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能定时器1中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_RTCT1IE_POS);
    }
    else
    {
        /* 禁止定时器1中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_RTCT1IE_POS);
    }
}

/**
  * 描述  设置RTC定时器0中断使能。
  * 输入  NewState: RTC定时器0中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Timer0_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器RTCT0IE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能定时器0中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_RTCT0IE_POS);
    }
    else
    {
        /* 禁止定时器0中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_RTCT0IE_POS);
    }
}

/**
  * 描述  设置RTC时间节拍中断使能。
  * 输入  NewState: RTC时间节拍中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Time_Tick_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器TTIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能时间节拍中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_TTIE_POS);
    }
    else
    {
        /* 禁止时间节拍中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_TTIE_POS);
    }
}

/**
  * 描述  设置RTC闹钟B中断使能。
  * 输入  NewState: RTC闹钟B中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_B_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器ALRBIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟B中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_ALRBIE_POS);
    }
    else
    {
        /* 禁止闹钟B中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_ALRBIE_POS);
    }
}

/**
  * 描述  设置RTC闹钟A中断使能。
  * 输入  NewState: RTC闹钟A中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Alarm_A_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器ALRAIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能闹钟A中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_ALRAIE_POS);
    }
    else
    {
        /* 禁止闹钟A中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_ALRAIE_POS);
    }
}

/**
  * 描述  设置RTC日进程中断使能。
  * 输入  NewState: RTC日进程中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Days_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器DAYIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能日进程中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_DAYIE_POS);
    }
    else
    {
        /* 禁止日进程中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_DAYIE_POS);
    }
}

/**
  * 描述  设置RTC小时进程中断使能。
  * 输入  NewState: RTC小时进程中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Hours_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器HOURIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能小时进程中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_HOURIE_POS);
    }
    else
    {
        /* 禁止小时进程中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_HOURIE_POS);
    }
}

/**
  * 描述  设置RTC分进程中断使能。
  * 输入  NewState: RTC分进程中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Minutes_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器MINIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能分进程中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_MINIE_POS);
    }
    else
    {
        /* 禁止分进程中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_MINIE_POS);
    }
}

/**
  * 描述  设置RTC秒进程中断使能。
  * 输入  NewState: RTC秒进程中断使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
RTC_Seconds_INT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置RTC_IER寄存器SECIE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能秒进程中断 */
        SFR_SET_BIT_ASM(RTC_IER, RTC_IER_SECIE_POS);
    }
    else
    {
        /* 禁止秒进程中断 */
        SFR_CLR_BIT_ASM(RTC_IER, RTC_IER_SECIE_POS);
    }
}

/**
  * 描述  获取时间戳中断标志。
  * 输入  无。
  * 返回  1:产生了时间戳中断；0:未产生时间戳中断。
  */
FlagStatus
RTC_Get_Time_Stamp_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器TSIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_TSIF)
    {
        /* 产生了时间戳中断 */
        return SET;
    }
    else
    {
        /* 未产生时间戳中断 */
        return RESET;
    }
}

/**
  * 描述  获取时间戳溢出中断标志。
  * 输入  无。
  * 返回  1:产生了时间戳溢出中断；0:未产生时间戳溢出中断。
  */
FlagStatus
RTC_Get_Time_Stamp_Overflow_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器TSOVFIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_TSOVFIF)
    {
        /* 产生了时间戳溢出中断 */
        return SET;
    }
    else
    {
        /* 未产生时间戳溢出中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC定时器1中断标志。
  * 输入  无。
  * 返回  1:产生了RTC定时器1中断；0:未产生RTC定时器1中断。
  */
FlagStatus
RTC_Get_Timer1_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器RTCT1IF位 ----------------*/
    if (RTC_IFR & RTC_IFR_RTCT1IF)
    {
        /* 产生了RTC定时器1中断 */
        return SET;
    }
    else
    {
        /* 未产生RTC定时器1中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC定时器0中断标志。
  * 输入  无。
  * 返回  1:产生了RTC定时器0中断；0:未产生RTC定时器0中断。
  */
FlagStatus
RTC_Get_Timer0_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器RTCT0IF位 ----------------*/
    if (RTC_IFR & RTC_IFR_RTCT0IF)
    {
        /* 产生了RTC定时器0中断 */
        return SET;
    }
    else
    {
        /* 未产生RTC定时器0中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC时间节拍中断标志。
  * 输入  无。
  * 返回  1:产生了时间节拍中断；0:未产生时间节拍中断。
  */
FlagStatus
RTC_Get_Time_Tick_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器TTIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_TTIF)
    {
        /* 产生了时间节拍中断 */
        return SET;
    }
    else
    {
        /* 未产生时间节拍中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC闹钟B中断标志。
  * 输入  无。
  * 返回  1:产生了闹钟B中断；0:未产生闹钟B中断。
  */
FlagStatus
RTC_Get_Alarm_B_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器ALRBIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_ALRBIF)
    {
        /* 产生了闹钟B中断 */
        return SET;
    }
    else
    {
        /* 未产生闹钟B中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC闹钟A中断标志。
  * 输入  无。
  * 返回  1:产生了闹钟A中断；0:未产生闹钟B中断。
  */
FlagStatus
RTC_Get_Alarm_A_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器ALRAIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_ALRAIF)
    {
        /* 产生了闹钟A中断 */
        return SET;
    }
    else
    {
        /* 未产生闹钟A中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC日进程中断标志。
  * 输入  无。
  * 返回  1:产生了日进程中断；0:未产生日进程中断。
  */
FlagStatus
RTC_Get_Days_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器DAYIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_DAYIF)
    {
        /* 产生了日进程中断 */
        return SET;
    }
    else
    {
        /* 未产生日进程中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC小时进程中断标志。
  * 输入  无。
  * 返回  1:产生了小时进程中断；0:未产生小时进程中断。
  */
FlagStatus
RTC_Get_Hours_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器HOURIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_HOURIF)
    {
        /* 产生了小时进程中断 */
        return SET;
    }
    else
    {
        /* 未产生小时进程中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC分进程中断标志。
  * 输入  无。
  * 返回  1:产生了分进程中断；0:未产生分进程中断。
  */
FlagStatus
RTC_Get_Minutes_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器MINIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_MINIF)
    {
        /* 产生了分进程中断 */
        return SET;
    }
    else
    {
        /* 未产生分进程中断 */
        return RESET;
    }
}

/**
  * 描述  获取RTC秒进程中断标志。
  * 输入  无。
  * 返回  1:产生了秒进程中断；0:未产生秒进程中断。
  */
FlagStatus
RTC_Get_Seconds_INT_Flag (void)
{
    /*---------------- 读取RTC_IFR寄存器SECIF位 ----------------*/
    if (RTC_IFR & RTC_IFR_SECIF)
    {
        /* 产生了秒进程中断 */
        return SET;
    }
    else
    {
        /* 未产生秒进程中断 */
        return RESET;
    }
}

/**
  * 描述  清零RTC时间戳中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Time_Stamp_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器TSIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_TSIC_POS);
    while(RTC_IFR & RTC_IFR_TSIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_TSIC_POS);
}

/**
  * 描述  清零RTC时间戳溢出中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Time_Stamp_Overflow_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器TSOVFIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_TSOVFIC_POS);
    while(RTC_IFR & RTC_IFR_TSOVFIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_TSOVFIC_POS);
}

/**
  * 描述  清零RTC定时器1中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Timer1_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器RTCT1IC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_RTCT1IC_POS);
    while(RTC_IFR & RTC_IFR_RTCT1IF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_RTCT1IC_POS);
}

/**
  * 描述  清零RTC定时器0中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Timer0_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器RTCT0IC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_RTCT0IC_POS);
    while(RTC_IFR & RTC_IFR_RTCT0IF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_RTCT0IC_POS);
}

/**
  * 描述  清零时间节拍中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Time_Tick_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器TTIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_TTIC_POS);
    while(RTC_IFR & RTC_IFR_TTIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_TTIC_POS);
}

/**
  * 描述  清零闹钟B中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Alarm_B_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器ALRBIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_ALRBIC_POS);
    while(RTC_IFR & RTC_IFR_ALRBIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_ALRBIC_POS);
}

/**
  * 描述  清零闹钟A中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Alarm_A_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器ALRAIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_ALRAIC_POS);
    while(RTC_IFR & RTC_IFR_ALRAIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_ALRAIC_POS);
}

/**
  * 描述  清零RTC日进程中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Days_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器DAYIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_DAYIC_POS);
    while(RTC_IFR & RTC_IFR_DAYIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_DAYIC_POS);
}

/**
  * 描述  清零RTC小时进程中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Hours_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器HOURIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_HOURIC_POS);
    while(RTC_IFR & RTC_IFR_HOURIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_HOURIC_POS);
}

/**
  * 描述  清零RTC分进程中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Minutes_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器MINIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_MINIC_POS);
    while(RTC_IFR & RTC_IFR_MINIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_MINIC_POS);
}

/**
  * 描述  清零RTC秒进程中断标志。
  * 输入  无。
  * 返回  无。
  */
void
RTC_Clear_Seconds_INT_Flag (void)
{
    /*---------------- 设置RTC_IFR寄存器SECIC位 ----------------*/
    SFR_SET_BIT_ASM(RTC_IFR, RTC_IFR_SECIC_POS);
    while(RTC_IFR & RTC_IFR_SECIF);
    SFR_CLR_BIT_ASM(RTC_IFR, RTC_IFR_SECIC_POS);
}
/**
  *   ##### 实时时钟(RTC)中断管理函数定义结束 #####
  */


/**
  *   ##### 实时时钟(RTC)私有函数 #####
  */
/**
  * 描述  十进制数值转换为BCD码。
  * 输入  Value: 待转换的十进制两位数，8位有效数值。
  * 返回  2字节BCD码。
  */
static uint8_t
RTC_Byte_To_Bcd (uint8_t Value)
{
    uint8_t bcdhigh = 0;
    uint8_t bcdlow = 0;

    bcdhigh = Value / ((uint8_t)10);
    bcdlow = Value - bcdhigh * ((uint8_t)10);

    return (uint8_t)((bcdhigh << (uint8_t)4) | bcdlow);
}

/**
  * 描述  BCD码转换为十进制两位数。
  * 输入  Value: 待转换的BCD码，8位有效数值。
  * 返回  十进制两位数。
  */
static uint8_t
RTC_Bcd_To_Byte (uint8_t Value)
{
    uint8_t tmp = 0;

    tmp = (Value >> (uint8_t)4) * ((uint8_t)10);

    return (tmp + (Value & (uint8_t)0xF));
}
/**
  *   ##### 实时时钟(RTC)私有函数定义结束 #####
  */

