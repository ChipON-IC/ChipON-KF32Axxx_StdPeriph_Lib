/**
  ******************************************************************************
  * 文件名  kf32a_basic_pm.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了电源管理(PM)相关功能函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_PM_H_
#define _KF32A_BASIC_PM_H_

#include "KF32A_BASIC.h"


/**
  * 备份区寄存器模块软件复位
  */
#define PERIPHERAL_RST_STATUS           ((uint32_t)0)
#define PERIPHERAL_OUTRST_STATUS        ((uint32_t)1)
#define CHECK_PERIPHERAL_RST(RST)       (((RST) == PERIPHERAL_RST_STATUS) \
                                      || ((RST) == PERIPHERAL_OUTRST_STATUS))

/**
  * SRAM的B区在standby模式下工作配置
  */
#define PM_SRAM_IN_STANDBY_POWER_DOWN   ((uint32_t)0)
#define PM_SRAM_IN_STANDBY_KEEP_DATA    ((uint32_t)1)
#define CHECK_PM_SRAM_IN_STANDBY(MODE)  (((MODE) == PM_SRAM_IN_STANDBY_POWER_DOWN) \
                                      || ((MODE) == PM_SRAM_IN_STANDBY_KEEP_DATA))

/**
  * 延时时间
  */
#define DELAY_TIME_1ms                  ((uint32_t)0)
#define DELAY_TIME_2ms                  ((uint32_t)0)
#define DELAY_TIME_32us                 ((uint32_t)1)
#define DELAY_TIME_500us                ((uint32_t)1)
#define CHECK_DELAY_TIME(TIME)          (((TIME) == DELAY_TIME_1ms) \
                                      || ((TIME) == DELAY_TIME_2ms) \
                                      || ((TIME) == DELAY_TIME_32us) \
                                      || ((TIME) == DELAY_TIME_500us))

/**
  * 低功耗外设IO口
  */
#define PM_GENERAL_PURPOSE_IO_PORT      ((uint32_t)0<<PM_CTL0_PHERIIOSEL_POS)
#define PM_LOW_POWER_IO_PORT            ((uint32_t)1<<PM_CTL0_PHERIIOSEL_POS)
#define CHECK_PM_LOW_POWER_IO(CFG)      (((CFG) == PM_GENERAL_PURPOSE_IO_PORT) \
                                      || ((CFG) == PM_LOW_POWER_IO_PORT))

/**
  * 低频晶振IO口
  */
#define PM_EXTLF_PIN1_IO_PORT      ((uint32_t)0<<PM_CAL1_LSESEL_POS)
#define PM_EXTLF_PIN2_IO_PORT      ((uint32_t)1<<PM_CAL1_LSESEL_POS)
#define CHECK_PM_EXTLF_PIN_IO(CFG)      (((CFG) == PM_EXTLF_PIN1_IO_PORT) \
                                      || ((CFG) == PM_EXTLF_PIN2_IO_PORT))
/**
  * 高频频晶振IO口
  */
#define PM_EXTHF_PIN1_IO_PORT      ((uint32_t)0<<PM_CAL1_HSESEL_POS)
#define PM_EXTHF_PIN2_IO_PORT      ((uint32_t)1<<PM_CAL1_HSESEL_POS)
#define CHECK_PM_EXTHF_PIN_IO(CFG)      (((CFG) == PM_EXTHF_PIN1_IO_PORT) \
                                      || ((CFG) == PM_EXTHF_PIN2_IO_PORT))


/**
  * 低功耗模式
  */
#define PM_LOW_POWER_MODE_STOP_0        ((uint32_t)1<<PM_CTL0_LPMS0_POS)
#define PM_LOW_POWER_MODE_STOP_1        ((uint32_t)2<<PM_CTL0_LPMS0_POS)
#define PM_LOW_POWER_MODE_STANDBY       ((uint32_t)3<<PM_CTL0_LPMS0_POS)
#define PM_LOW_POWER_MODE_SHUTDOWN      ((uint32_t)4<<PM_CTL0_LPMS0_POS)
#define CHECK_PM_LOW_POWER_MODE(MODE)   (((MODE) == PM_LOW_POWER_MODE_STOP_0) \
                                      || ((MODE) == PM_LOW_POWER_MODE_STOP_1) \
                                      || ((MODE) == PM_LOW_POWER_MODE_STANDBY) \
                                      || ((MODE) == PM_LOW_POWER_MODE_SHUTDOWN))

/**
  * 内部测试输出BUFFER工作时钟分频
  */
#define PM_BUFFER_SCLK_DIV_1            ((uint32_t)0<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_2            ((uint32_t)1<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_4            ((uint32_t)2<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_8            ((uint32_t)3<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_16           ((uint32_t)4<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_32           ((uint32_t)5<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_64           ((uint32_t)6<<PM_CTL1_BUFCLKDIV0_POS)
#define PM_BUFFER_SCLK_DIV_128          ((uint32_t)7<<PM_CTL1_BUFCLKDIV0_POS)
#define CHECK_PM_BUFFER_SCLK_SCALER(DIV) \
                                        (((DIV) == PM_BUFFER_SCLK_DIV_1) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_2) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_4) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_8) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_16) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_32) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_64) \
                                      || ((DIV) == PM_BUFFER_SCLK_DIV_128))

/**
  * BAT_BOR电压点选择
  */
#define PM_BATTERY_VOLTAGE_1P6V         ((uint32_t)0<<PM_CTL1_BATBORSEL0_POS)
#define PM_BATTERY_VOLTAGE_1P8V         ((uint32_t)1<<PM_CTL1_BATBORSEL0_POS)
#define PM_BATTERY_VOLTAGE_2P06V        ((uint32_t)2<<PM_CTL1_BATBORSEL0_POS)
#define PM_BATTERY_VOLTAGE_2P4V         ((uint32_t)3<<PM_CTL1_BATBORSEL0_POS)
#define PM_BATTERY_VOLTAGE_2P88V        ((uint32_t)4<<PM_CTL1_BATBORSEL0_POS)
#define PM_BATTERY_VOLTAGE_3P6V         ((uint32_t)5<<PM_CTL1_BATBORSEL0_POS)
#define CHECK_PM_BATTERY_VOLTAGE(VOL)   (((VOL) == PM_BATTERY_VOLTAGE_1P6V) \
                                      || ((VOL) == PM_BATTERY_VOLTAGE_1P8V) \
                                      || ((VOL) == PM_BATTERY_VOLTAGE_2P06V) \
                                      || ((VOL) == PM_BATTERY_VOLTAGE_2P4V) \
                                      || ((VOL) == PM_BATTERY_VOLTAGE_2P88V) \
                                      || ((VOL) == PM_BATTERY_VOLTAGE_3P6V))

/**
  * 电压检测点
  */
#define PM_VOLTAGE_DETECTION_2P1V       ((uint32_t)0<<PM_CTL1_PVDS0_POS)
#define PM_VOLTAGE_DETECTION_2P25V      ((uint32_t)1<<PM_CTL1_PVDS0_POS)
#define PM_VOLTAGE_DETECTION_2P4V       ((uint32_t)2<<PM_CTL1_PVDS0_POS)
#define PM_VOLTAGE_DETECTION_2P55V      ((uint32_t)3<<PM_CTL1_PVDS0_POS)
#define PM_VOLTAGE_DETECTION_2P7V       ((uint32_t)4<<PM_CTL1_PVDS0_POS)
#define PM_VOLTAGE_DETECTION_2P85V      ((uint32_t)5<<PM_CTL1_PVDS0_POS)
#define PM_VOLTAGE_DETECTION_2P95V      ((uint32_t)6<<PM_CTL1_PVDS0_POS)
#define CHECK_PM_VOLTAGE_DETECTION(VOL) (((VOL) == PM_VOLTAGE_DETECTION_2P1V) \
                                      || ((VOL) == PM_VOLTAGE_DETECTION_2P25V) \
                                      || ((VOL) == PM_VOLTAGE_DETECTION_2P4V) \
                                      || ((VOL) == PM_VOLTAGE_DETECTION_2P55V) \
                                      || ((VOL) == PM_VOLTAGE_DETECTION_2P7V) \
                                      || ((VOL) == PM_VOLTAGE_DETECTION_2P85V) \
                                      || ((VOL) == PM_VOLTAGE_DETECTION_2P95V))

/**
  * 触发沿选择
  */
#define PM_TRIGGER_RISE_EDGE            ((uint32_t)0)
#define PM_TRIGGER_FALL_EDGE            ((uint32_t)1)
#define CHECK_PM_TRIGGER_EDGE(EDGE)     (((EDGE) == PM_TRIGGER_RISE_EDGE) \
                                      || ((EDGE) == PM_TRIGGER_FALL_EDGE))

/**
  * WKP引脚
  */
#define PM_PIN_WKP1                     ((uint32_t)0)
#define PM_PIN_WKP2                     ((uint32_t)1)
#define PM_PIN_WKP3                     ((uint32_t)2)
#define PM_PIN_WKP4                     ((uint32_t)3)
#define PM_PIN_WKP5                     ((uint32_t)4)
#define CHECK_PM_PIN_WKP(PIN)           (((PIN) == PM_PIN_WKP1) \
                                      || ((PIN) == PM_PIN_WKP2) \
                                      || ((PIN) == PM_PIN_WKP3) \
                                      || ((PIN) == PM_PIN_WKP4) \
                                      || ((PIN) == PM_PIN_WKP5))

/**
  * Stop模式下的外设
  */
#define PM_PERIPHERAL_CAN0              ((uint32_t)0)
#define PM_PERIPHERAL_CCP               ((uint32_t)1)
#define PM_PERIPHERAL_LCD               ((uint32_t)2)
#define PM_PERIPHERAL_UART0             ((uint32_t)3)
#define CHECK_PM_PERIPHERAL(PERIPH)     (((PERIPH) == PM_PERIPHERAL_CAN0) \
                                      || ((PERIPH) == PM_PERIPHERAL_CCP) \
                                      || ((PERIPH) == PM_PERIPHERAL_LCD) \
                                      || ((PERIPH) == PM_PERIPHERAL_UART0))

/**
  * 复位/唤醒事件
  */
#define PM_RESET_IWDT                   (PM_STA0_IWDTF_POS)
#define PM_WAKEUP_RTC_ALARM             (PM_STA0_RTCALRF_POS)
#define PM_WAKEUP_EXTERNAL_PIN          (PM_STA0_WKPPINF_POS)
#define PM_RESET_POR                    (PM_STA0_PORF_POS)
#define PM_RESET_BOR                    (PM_STA0_BORF_POS)
#define PM_RESET_SOFTWARE               (PM_STA0_WSRSTF_POS)
#define PM_WAKEUP_EXTERNAL_PIN_WKP5     (PM_STA1_WKP5STA_POS + (uint32_t)32)
#define PM_WAKEUP_EXTERNAL_PIN_WKP4     (PM_STA1_WKP4STA_POS + (uint32_t)32)
#define PM_WAKEUP_EXTERNAL_PIN_WKP3     (PM_STA1_WKP3STA_POS + (uint32_t)32)
#define PM_WAKEUP_EXTERNAL_PIN_WKP2     (PM_STA1_WKP2STA_POS + (uint32_t)32)
#define PM_WAKEUP_EXTERNAL_PIN_WKP1     (PM_STA1_WKP1STA_POS + (uint32_t)32)
#define CHECK_PM_RESET_WAKEUP(SEL)      (((SEL) == PM_RESET_IWDT) \
                                      || ((SEL) == PM_WAKEUP_RTC_ALARM) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN) \
                                      || ((SEL) == PM_RESET_POR) \
                                      || ((SEL) == PM_RESET_BOR) \
                                      || ((SEL) == PM_RESET_SOFTWARE) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP5) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP4) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP3) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP2) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP1))
#define CHECK_CLEAR_STA0_RESET_WAKEUP(SEL)	 (((SEL) == PM_RESET_IWDT) \
                                      || ((SEL) == PM_WAKEUP_RTC_ALARM) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN) \
                                      || ((SEL) == PM_RESET_POR) \
                                      || ((SEL) == PM_RESET_BOR) \
                                      || ((SEL) == PM_RESET_SOFTWARE))

#define CHECK_CLEAR_STA1_WAKEUP(SEL)	(((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP5) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP4) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP3) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP2) \
                                      || ((SEL) == PM_WAKEUP_EXTERNAL_PIN_WKP1))

/**
  * 零温漂电流档校准
  */
#define PM_CURRENT_OUTPUT_2uA           ((uint32_t)0<<PM_CAL0_ZEROTISTRIM0_POS)
#define PM_CURRENT_REDUCE_10_PERCENT    ((uint32_t)1<<PM_CAL0_ZEROTISTRIM0_POS)
#define PM_CURRENT_INCREASE_10_PERCENT  ((uint32_t)2<<PM_CAL0_ZEROTISTRIM0_POS)
#define PM_CURRENT_INCREASE_20_PERCENT  ((uint32_t)3<<PM_CAL0_ZEROTISTRIM0_POS)
#define CHECK_PM_CURRENT_CONFIG(CFG)    (((CFG) == PM_CURRENT_OUTPUT_2uA) \
                                      || ((CFG) == PM_CURRENT_REDUCE_10_PERCENT) \
                                      || ((CFG) == PM_CURRENT_INCREASE_10_PERCENT) \
                                      || ((CFG) == PM_CURRENT_INCREASE_20_PERCENT))

/**
  * BOR电压点选择
  */
#define PM_BOR_VOLTAGE_2P05V            ((uint32_t)0<<PM_CAL0_BORSEL0_POS)
#define PM_BOR_VOLTAGE_2P25V            ((uint32_t)1<<PM_CAL0_BORSEL0_POS)
#define PM_BOR_VOLTAGE_2P55V            ((uint32_t)2<<PM_CAL0_BORSEL0_POS)
#define PM_BOR_VOLTAGE_2P85V            ((uint32_t)3<<PM_CAL0_BORSEL0_POS)
#define CHECK_PM_BOR_VOLTAGE(CFG)       (((CFG) == PM_BOR_VOLTAGE_2P05V) \
                                      || ((CFG) == PM_BOR_VOLTAGE_2P25V) \
                                      || ((CFG) == PM_BOR_VOLTAGE_2P55V) \
                                      || ((CFG) == PM_BOR_VOLTAGE_2P85V))
/**
  * VREF电压点选择
  */
#define PM_VREF_VOLTAGE_2P0V            ((uint32_t)0<<PM_CTL1_VREFSEL0_POS)
#define PM_VREF_VOLTAGE_1P5V            ((uint32_t)1<<PM_CTL1_VREFSEL0_POS)
#define PM_VREF_VOLTAGE_2P5V            ((uint32_t)2<<PM_CTL1_VREFSEL0_POS)
#define PM_VREF_VOLTAGE_3P0V            ((uint32_t)3<<PM_CTL1_VREFSEL0_POS)
#define CHECK_PM_VREF_VOLTAGE(CFG)      (((CFG) == PM_VREF_VOLTAGE_2P0V) \
                                      || ((CFG) == PM_VREF_VOLTAGE_1P5V) \
                                      || ((CFG) == PM_VREF_VOLTAGE_2P5V) \
                                      || ((CFG) == PM_VREF_VOLTAGE_3P0V))
/**
  * MR电压选择
  */
#define PM_MR_VOLTAGE_1P2V              ((uint32_t)0)
#define PM_MR_VOLTAGE_0P9V              ((uint32_t)1)
#define PM_MR_VOLTAGE_1V                ((uint32_t)2)
#define PM_MR_VOLTAGE_1P32V             ((uint32_t)3)
#define CHECK_PM_MR_VOLTAGE(CFG)        (((CFG) == PM_MR_VOLTAGE_1P2V) \
                                      || ((CFG) == PM_MR_VOLTAGE_0P9V) \
                                      || ((CFG) == PM_MR_VOLTAGE_1V) \
                                      || ((CFG) == PM_MR_VOLTAGE_1P32V))

/**
  * MR或MR_HV模块选择
  */
#define PM_MR_MODULE                    (PM_CAL0_MRTRIM0_POS)
#define PM_MR_HV_MODULE                 (PM_CAL0_MRHVTRIM0_POS)
#define CHECK_PM_MR_MODULE(SEL)         (((SEL) == PM_MR_MODULE) \
                                      || ((SEL) == PM_MR_HV_MODULE))

/**
  * PM校准基准
  */
#define PM_REFERENCE_BUFFER             (PM_CAL0_BGTRIMREBUF0_POS)
#define PM_REFERENCE_LDO12              (PM_CAL0_BGTRIMMRLDO0_POS)
#define PM_REFERENCE_LDO18              (PM_CAL0_BGTRIMFLLDO0_POS)
#define CHECK_PM_REFERENCE(SEL)         (((SEL) == PM_REFERENCE_BUFFER) \
                                      || ((SEL) == PM_REFERENCE_LDO12) \
                                      || ((SEL) == PM_REFERENCE_LDO18))

/**
  * 参考BUFFER基准校准
  */
#define CHECK_PM_BUFFER_CALIBRATION(CFG) \
                                        (((CFG) >> 5) == 0)

/**
  * LDO12基准校准
  */
#define CHECK_PM_LDO12_CALIBRATION(CFG) (((CFG) >> 4) == 0)

/**
  * LDO18基准校准
  */
#define CHECK_PM_LDO18_CALIBRATION(CFG) (((CFG) >> 4) == 0)

/**
  * 内部低频振荡器偏置电流
  */
#define PM_INTLF_BIAS_CURRENT_10nA      ((uint32_t)0<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_12P5nA    ((uint32_t)1<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_15nA      ((uint32_t)2<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_17P5nA    ((uint32_t)3<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_0nA       ((uint32_t)4<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_2P5nA     ((uint32_t)5<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_5nA       ((uint32_t)6<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_INTLF_BIAS_CURRENT_7P5nA     ((uint32_t)7<<PM_CAL1_LSITRIMBIAS0_POS)
#define CHECK_PM_INTLF_BIAS_CURRENT(SEL) \
                                        (((SEL) == PM_INTLF_BIAS_CURRENT_10nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_12P5nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_15nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_17P5nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_0nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_2P5nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_5nA) \
                                      || ((SEL) == PM_INTLF_BIAS_CURRENT_7P5nA))

/**
  * 外部低频振荡器偏置电流
  */
#define PM_EXTLF_BIAS_CURRENT_20nA      ((uint32_t)0<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_25nA      ((uint32_t)1<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_30nA      ((uint32_t)2<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_35nA      ((uint32_t)3<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_0nA       ((uint32_t)4<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_5nA       ((uint32_t)5<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_10nA      ((uint32_t)6<<PM_CAL1_LSITRIMBIAS0_POS)
#define PM_EXTLF_BIAS_CURRENT_15nA      ((uint32_t)7<<PM_CAL1_LSITRIMBIAS0_POS)
#define CHECK_PM_EXTLF_BIAS_CURRENT(SEL) \
                                        (((SEL) == PM_EXTLF_BIAS_CURRENT_20nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_25nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_30nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_35nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_0nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_5nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_10nA) \
                                      || ((SEL) == PM_EXTLF_BIAS_CURRENT_15nA))

/**
  * 内部低频振荡器电容校准
  */
#define CHECK_PM_CAPACITANCE_CALIBRATION(CFG) \
                                        (((CFG) >> 5) == 0)

/**
  * 低功耗偏置电流校准
  */
#define CHECK_PM_LP_BIAS_CALIBRATION(CFG) \
                                        (((CFG) >> 3) == 0)

/**
  * LPBG PUMP校准
  */
#define CHECK_PM_PUMP_CALIBRATION(CFG) (((CFG) >> 3) == 0)

/**
  * 外部低频振荡器N管偏置校准
  */
#define PM_BRANCH_CURRENT_NONE          ((uint32_t)0<<PM_CAL1_LSETRIMNBIAS0_POS)
#define PM_BRANCH_CURRENT_50_PERCENT    ((uint32_t)1<<PM_CAL1_LSETRIMNBIAS0_POS)
#define PM_BRANCH_CURRENT_150_PERCENT   ((uint32_t)2<<PM_CAL1_LSETRIMNBIAS0_POS)
#define PM_BRANCH_CURRENT_200_PERCENT   ((uint32_t)3<<PM_CAL1_LSETRIMNBIAS0_POS)
#define CHECK_PM_BRANCH_CURRENT(SEL)    (((SEL) == PM_BRANCH_CURRENT_NONE) \
                                      || ((SEL) == PM_BRANCH_CURRENT_50_PERCENT) \
                                      || ((SEL) == PM_BRANCH_CURRENT_150_PERCENT) \
                                      || ((SEL) == PM_BRANCH_CURRENT_200_PERCENT))

#if _CLOSE_SERVICE_
/**
  * 功耗模式时序选择
  */
#define PM_POWER_DISSIPATION_SCHEME1    ((uint32_t)0<<PM_CTL0_PDMSEL_POS)
#define PM_POWER_DISSIPATION_SCHEME2    ((uint32_t)1<<PM_CTL0_PDMSEL_POS)
#define CHECK_PM_POWER_DISSIPATION(CFG) (((CFG) == PM_POWER_DISSIPATION_SCHEME1) \
                                      || ((CFG) == PM_POWER_DISSIPATION_SCHEME2))

/**
  * POR12关闭到LDO关闭的延时时间配置
  */
#define PM_POWER_DISSIPATION_500ns      ((uint32_t)1<<PM_CTL0_PDMDELAY0_POS)
#define PM_POWER_DISSIPATION_500ns_1T   ((uint32_t)2<<PM_CTL0_PDMDELAY0_POS)
#define PM_POWER_DISSIPATION_500ns_2P5T ((uint32_t)3<<PM_CTL0_PDMDELAY0_POS)
#define CHECK_PM_POWER_DISSIPATION_DELAY(TIME) \
                                        (((TIME) == PM_POWER_DISSIPATION_500ns) \
                                      || ((TIME) == PM_POWER_DISSIPATION_500ns_1T) \
                                      || ((TIME) == PM_POWER_DISSIPATION_500ns_2P5T))

/**
  * STOP模式唤醒时对系统时钟的控制
  */
#define PM_RELEASE_SYSCLK_WAIT_MR       ((uint32_t)0<<PM_CTL0_STOPSEL_POS)
#define PM_RELEASE_SYSCLK_IMMEDIATELY   ((uint32_t)1<<PM_CTL0_STOPSEL_POS)
#define CHECK_PM_RELEASE_SYSCLK(CFG)    (((CFG) == PM_RELEASE_SYSCLK_WAIT_MR) \
                                      || ((CFG) == PM_RELEASE_SYSCLK_IMMEDIATELY))

/**
  * 主电压调制器MR上电延时
  */
#define PM_RELEASE_SYSCLK_WAIT_MR       ((uint32_t)0<<PM_CTL0_STOPSEL_POS)
#define PM_RELEASE_SYSCLK_IMMEDIATELY   ((uint32_t)1<<PM_CTL0_STOPSEL_POS)
#define CHECK_PM_RELEASE_SYSCLK(CFG)    (((CFG) == PM_RELEASE_SYSCLK_WAIT_MR) \
                                      || ((CFG) == PM_RELEASE_SYSCLK_IMMEDIATELY))

/**
  * LDO18内置模块选择
  */
#define PM_LDO18_CAP                    ((uint32_t)0<<PM_CAL0_LDO18SEL_POS)
#define PM_LDO18_CAPLESS                ((uint32_t)1<<PM_CAL0_LDO18SEL_POS)
#define CHECK_PM_LDO18_MODULE(CFG)      (((CFG) == PM_LDO18_CAPLESS) \
                                      || ((CFG) == PM_LDO18_CAP))

/**
  * MR_BG模式选择
  */
#define PM_LOW_POWER_MODE               ((uint32_t)0<<PM_CAL0_MRBGSEL_POS)
#define PM_HIGH_POWER_MODE              ((uint32_t)1<<PM_CAL0_MRBGSEL_POS)
#define CHECK_PM_MR_BG_MODE(SEL)        (((SEL) == PM_LOW_POWER_MODE) \
                                      || ((SEL) == PM_HIGH_POWER_MODE))

/**
  * LPR内置模块选择
  */
#define PM_LPR_DEFAULT                  ((uint32_t)0<<PM_CAL0_LPRSEL_POS)
#define PM_LPR_BACKUP                   ((uint32_t)1<<PM_CAL0_LPRSEL_POS)
#define CHECK_PM_LPR_MODE(SEL)          (((SEL) == PM_LPR_DEFAULT) \
                                      || ((SEL) == PM_LPR_BACKUP))
#endif /* _CLOSE_SERVICE_ */


/* 电源管理(PM)控制功能函数定义*********************************************/
void PM_IO_Latch_Enable (FunctionalState NewState);
FlagStatus PM_Get_IO_Latch_Status (void);
void PM_Internal_Low_Frequency_Enable (FunctionalState NewState);
void PM_External_Low_Frequency_Enable (FunctionalState NewState);
void PM_External_Low_Frequency_Clock_Enable (FunctionalState NewState);
void PM_Main_Bandgap_Enable (FunctionalState NewState);
void PM_LDO18_Enable (FunctionalState NewState);
void PM_Backup_Registers_Reset_Config (uint32_t BkpReset);
void PM_Independent_Watchdog_Reset_Config (uint32_t IWDTReset);
void PM_SRAM_In_Standby_Work_Mode_Config (uint32_t WorkMode);
void PM_Backup_POR_Delay_Time_Config (uint32_t DelayTime);
void PM_Main_POR_Delay_Time_Config (uint32_t DelayTime);
void PM_Peripheral_IO_Port_Config (uint32_t PeripheralPort);
void PM_Flash_Power_Off_Enable (FunctionalState NewState);
void PM_Backup_Write_And_Read_Enable (FunctionalState NewState);
void PM_LPR_Software_Enable (FunctionalState NewState);
void PM_Low_Power_Mode_Config (uint32_t LowPowerMode);
void PM_BOR_Enable (FunctionalState NewState);
void PM_Low_Power_BOR_Enable (FunctionalState NewState);
void PM_Temperature_Sensor_Enable (FunctionalState NewState);
void PM_Temperature_Sensor_Buffer_Enable (FunctionalState NewState);
void PM_Reference_Voltage_Enable (FunctionalState NewState);
void PM_Internal_Test_Buffer_Clock_Enable (FunctionalState NewState);
void PM_Internal_Test_Buffer_Clock_Scaler_Config (uint32_t SclkScaler);
void PM_CCP0CLKLPEN_Enable (FunctionalState NewState);
void PM_PLL0_Output_Buffer_Enable (FunctionalState NewState);
void PM_PLL1_Output_Buffer_Enable (FunctionalState NewState);
void PM_PLL2_Output_Buffer_Enable (FunctionalState NewState);
void PM_PLL0LDO_Output_Buffer_Enable (FunctionalState NewState);
void PM_PLL1LDO_Output_Buffer_Enable (FunctionalState NewState);
void PM_PLL2LDO_Output_Buffer_Enable (FunctionalState NewState);
void PM_Battery_BOR_Config (uint32_t Voltage);
void PM_Battery_BOR_Enable (FunctionalState NewState);
void PM_Peripheral_Voltage_Monitoring_Enable (FunctionalState NewState);
void PM_Voltage_Detection_Config (uint32_t Voltage);
void PM_Voltage_Detection_Enable (FunctionalState NewState);
void PM_EXTLF_PIN_Selection_Config (uint32_t PeripheralPort);
void PM_EXTHF_PIN_Selection_Config (uint32_t PeripheralPort);
void PM_External_Wakeup_Pin_Enable (uint32_t PinSel, FunctionalState NewState);
void PM_External_Wakeup_Edge_Config (uint32_t PinSel, uint32_t TriggerEdge);
void PM_Stop_Mode_Peripheral_INLF_Enable (uint32_t Peripheral,
                    FunctionalState NewState);
void PM_Peripheral_Reset_Config (uint32_t Peripheral, uint32_t ResetStatus);
#if _CLOSE_SERVICE_
void PM_Vdd_Por_Enable (FunctionalState NewState);
void PM_Low_Power_Bandgap_Enable (FunctionalState NewState);
void PM_Power_Dissipation_Mode_Config (uint32_t Mode);
void PM_Power_Dissipation_Mode_Delay_Config (uint32_t DelayTime);
void PM_Internal_Test_Buffer_Enable (FunctionalState NewState);
#endif /* _CLOSE_SERVICE_ */
/* 电源管理(PM)状态标志管理函数定义*****************************************/
void PM_Clear_Reset_And_Wakeup_Flag (uint32_t EventSel);
FlagStatus PM_Get_IWDT_Reset_Flag (uint32_t EventSel);
void PM_Clear_External_Wakeup_Pin_Flag (uint32_t EventSel);
FlagStatus PM_Get_Low_Power_Running_State (void);
FlagStatus PM_Get_LPR_Status (void);
FlagStatus PM_Get_Peripheral_Voltage_Detection_Status (void);
/* 电源管理(PM)校准控制函数定义*********************************************/
void PM_Zero_Drift_Current_Config (uint32_t Calibration);
void PM_BOR_Voltage_Config (uint32_t Voltage);
void PM_Main_Regulator_Voltage_Config (uint32_t MRSel, uint32_t Voltage);
void PM_Main_Regulator_HV_Enable (FunctionalState NewState);
void PM_Reference_Calibration_Config (uint32_t Reference,
                    uint32_t Calibration);
void PM_INTLF_Bias_Current_Config (uint32_t Calibration);
void PM_EXTLF_Bias_Current_Config (uint32_t Calibration);
void PM_INTLF_Capacitance_Calibration_Config (uint32_t Calibration);
void PM_LP_Bias_Calibration_Config (uint32_t Calibration);
void PM_LPBG_Pump_Calibration_Config (uint32_t Calibration);
void PM_EXTLF_N_Bias_Current_Config (uint32_t Calibration);
#if _CLOSE_SERVICE_
void PM_LDO18_Module_Config (uint32_t LDO18Config);
void PM_Main_Regulator_Bandgap_Config (uint32_t ModeSel);
void PM_LPR_Module_Config (uint32_t ModeSel);
void PM_VREF_SELECT (uint32_t Voltage);
void PM_VREF_Software_Enable (FunctionalState NewState);
#endif /* _CLOSE_SERVICE_ */

#endif /* _KF32A_BASIC_PM_H */
