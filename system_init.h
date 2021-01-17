/**
  ********************************************************************
  * 文件名  system_init.h
  * 作  者   ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2018-3-15
  * 描  述  该文件提供了外设库包含的头文件与系统时钟配置相关的宏定义。
  *
  *********************************************************************
*/
#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_

#include <string.h>
#include <stdint.h>

#include "KF32A_BASIC.h"
#include "kf32a_basic_adc.h"
#include "kf32a_basic_aes.h"
#include "kf32a_basic_bkp.h"
#include "kf32a_basic_can.h"
#include "kf32a_basic_cfgl.h"
#include "kf32a_basic_cmp.h"
#include "kf32a_basic_crc.h"
#include "kf32a_basic_dac.h"
#include "kf32a_basic_dma.h"
#include "kf32a_basic_flash.h"
#include "kf32a_basic_gpio.h"
#include "kf32a_basic_i2c.h"
#include "kf32a_basic_int.h"
#include "kf32a_basic_iwdt.h"
#include "kf32a_basic_led.h"
#include "kf32a_basic_lcd.h"
#include "kf32a_basic_op.h"
#include "kf32a_basic_osc.h"
#include "kf32a_basic_pclk.h"
#include "kf32a_basic_pm.h"
#include "kf32a_basic_qei.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_rtc.h"
#include "kf32a_basic_spi.h"
#include "kf32a_basic_sysctl.h"
#include "kf32a_basic_systick.h"
#include "kf32a_basic_tim.h"
#include "kf32a_basic_usart.h"
#include "kf32a_basic_usb.h"
#include "kf32a_basic_wwdt.h"


/* 系统时钟选择 */
#ifndef SYSCLK_FREQ_HSE
	#define SYSCLK_FREQ_HSI         //内部高频
#endif

/* 系统时钟选择 */
#ifdef KF32A150
//#define SYSCLK_FREQ_48MHz	48000000
//#define SYSCLK_FREQ_72MHz	72000000
//#define SYSCLK_FREQ_96MHz	96000000
#define SYSCLK_FREQ_120MHz	120000000
#endif
#ifdef KF32A151
//#define SYSCLK_FREQ_48MHz	48000000
//#define SYSCLK_FREQ_72MHz	72000000
//#define SYSCLK_FREQ_96MHz	96000000
#define SYSCLK_FREQ_120MHz	120000000
#endif
#ifdef KF32A152
//#define SYSCLK_FREQ_48MHz	48000000
//#define SYSCLK_FREQ_72MHz	72000000
//#define SYSCLK_FREQ_96MHz	96000000
#define SYSCLK_FREQ_120MHz	120000000
#endif

#ifdef KF32A140
#define SYSCLK_FREQ_48MHz	48000000
#endif
#ifdef KF32A141
#define SYSCLK_FREQ_48MHz	48000000
#endif

/* 系统时钟选择 */
#ifdef KF32A250
//#define SYSCLK_FREQ_48MHz	48000000
#define SYSCLK_FREQ_64MHz	64000000
#endif
#ifdef KF32A251
//#define SYSCLK_FREQ_48MHz	48000000
#define SYSCLK_FREQ_64MHz	64000000
#endif


static void SetSysClock(void);
void SystemInit(void);
void systick_delay_init();
void systick_delay_us(unsigned int nus);
void systick_delay_ms(unsigned int mus);
#endif /* SYS_H_ */
