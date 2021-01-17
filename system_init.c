/**
  ******************************************************************************
  * 文件名  system_init.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描  述  该文件提供了外设时钟与系统时钟初始化
  *
  ******************************************************************************
  */
#include "system_init.h"
/**
  * 描述  时钟默认配置。
  * 输入  无。
  * 返回  无。
*/
static void SetSysClock(void)
{
	/* 使能内部高频时钟*/
	OSC_INTHF_Software_Enable(TRUE);
	while(OSC_Get_INTHF_INT_Flag() != SET);
	/* PLL时钟源选择 */
#ifdef SYSCLK_FREQ_HSI
	OSC_PLL_Input_Source_Config(PLL_INPUT_INTHF);
#else
	OSC_PLL_Input_Source_Config(PLL_INPUT_EXTHF);
#endif
	/* PLL倍频时钟选择 */
#ifdef SYSCLK_FREQ_48MHz
	OSC_PLL_Multiple_Value_Select(48,2,8);
#elif defined SYSCLK_FREQ_64MHz
	OSC_PLL_Multiple_Value_Select(32,2,4);
#elif defined SYSCLK_FREQ_72MHz
	OSC_PLL_Multiple_Value_Select(36,2,4);
#elif defined SYSCLK_FREQ_96MHz
	OSC_PLL_Multiple_Value_Select(12,1,2);
#elif defined SYSCLK_FREQ_120MHz
	OSC_PLL_Multiple_Value_Select(30,2,2);
#endif
	OSC_PLL_Start_Delay_Config(PLL_START_DELAY_1024);
	OSC_PLL_Software_Enable(TRUE);
	OSC_PLL_RST();
	while(OSC_Get_PLL_INT_Flag() != SET);
	/* 主时钟配置 */
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_PLL);
	/* 外设高速时钟 */
	OSC_HFCK_Division_Config(HFCK_DIVISION_1);
	OSC_HFCK_Source_Config(HFCK_SOURCE_INTHF);
	OSC_HFCK_Enable(TRUE);
}

/**
  * 描述  系统初始化。
  * 输入  无。
  * 返回  无。
*/
void SystemInit(void)
{
	/* 时钟配置 */
	SetSysClock();
	FLASH_CFG=0xC3;//FLASH读取周期4周期
}

