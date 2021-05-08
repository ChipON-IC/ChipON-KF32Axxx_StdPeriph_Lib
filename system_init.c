/**
  ******************************************************************************
  * 文件名  system_init.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了外设时钟与系统时钟初始化
  *
  ******************************************************************************
  */
#include "system_init.h"

#define INTHF_FREQ_16MHz	   16000000
#define INTLF_FREQ_32KHz	   32000
#define EXTHF_FREQ_16MHz	   16000000
#define EXTLF_FREQ_32768Hz	   32768
#define LP4M_FREQ_4MHz		   4000000

static uint8_t 	fac_us;
static uint16_t fac_ms;

/**
  * 描述  时钟默认配置。
  * 输入  无。
  * 返回  无。
*/
static void SetSysClock(void)
{
	volatile unsigned int delay_C;
	/* 开启内部低频时钟 */
	OSC_INTLF_Software_Enable(TRUE);
	/* 使能内部高频时钟 */
	OSC_INTHF_Software_Enable(TRUE);
	FLASH_CFG = 0xC7;                         //恢复默认的FLASH延时配置
	OSC_SCK_Division_Config(SCLK_DIVISION_1); //主时钟分频选择
	OSC_SCK_Source_Config(SCLK_SOURCE_INTHF); //选择内部高频作为系统时钟

	while (OSC_Get_INTHF_INT_Flag() != SET);
	/* PLL时钟源选择 */
#ifdef SYSCLK_FREQ_HSI
	OSC_PLL_Input_Source_Config(PLL_INPUT_INTHF);
#else
	/* 开放备份域 */
	SFR_SET_BIT_ASM(OSC_CTL0, OSC_CTL0_PMWREN_POS);  //配置整个备份域可读写
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPREGCLR_POS); //备份域退出复位状态
	SFR_SET_BIT_ASM(PM_CTL0, PM_CTL0_BKPWR_POS);     //备份区寄存器可读写
	SFR_CLR_BIT_ASM(PM_CTL0, PM_CTL0_IOLATCH_POS);   //IO解锁

	PM_EXTHF_PIN_Selection_Config(PM_EXTHF_PIN2_IO_PORT); //外部高频晶振PD9与PD10中晶振引脚
	OSC_EXTHF_Software_Enable(TRUE);                      //外部高频晶振使能
	OSC_EXTHF_Start_Delay_Config(EXT_START_DELAY_0);
	OSC_PLL_Input_Source_Config(PLL_INPUT_EXTHF);
	delay_C = 0xA39;
	while (delay_C--); //等待外部高频时钟使能约4ms        /* while(OSC_Get_EXTHF_INT_Flag() != SET); */
	
#endif
	/* PLL倍频时钟选择 */
#ifdef SYSCLK_FREQ_48MHz
	FLASH_CFG = 0xC3;
	OSC_PLL_Multiple_Value_Select(48, 2, 8);
#elif defined SYSCLK_FREQ_64MHz
	FLASH_CFG = 0xC3;
	OSC_PLL_Multiple_Value_Select(32,2,4);
#elif defined SYSCLK_FREQ_72MHz
	FLASH_CFG = 0xC3;
	OSC_PLL_Multiple_Value_Select(36, 2, 4);
#elif defined SYSCLK_FREQ_96MHz
	FLASH_CFG = 0xC3;
	OSC_PLL_Multiple_Value_Select(12, 1, 2);
#elif defined SYSCLK_FREQ_120MHz
	FLASH_CFG = 0xC4;
	OSC_PLL_Multiple_Value_Select(30, 2, 2);
#endif
	OSC_PLL_Start_Delay_Config(PLL_START_DELAY_8192);
	OSC_PLL_Software_Enable(TRUE);
	OSC_PLL_RST();
	delay_C = 0x27f;
	while (delay_C--) ; //等待PLL使能约1ms        /* while (OSC_Get_PLL_INT_Flag() != SET); */
	
		
	/* 主时钟配置 */
	OSC_SCK_Division_Config(SCLK_DIVISION_1);
	OSC_SCK_Source_Config(SCLK_SOURCE_PLL);
	/* 外设高速时钟 */
	OSC_HFCK_Division_Config(HFCK_DIVISION_1);
#ifdef SYSCLK_FREQ_HSI
	OSC_HFCK_Source_Config(HFCK_SOURCE_INTHF);
#else
	OSC_HFCK_Source_Config(HFCK_SOURCE_EXTHF);
#endif
	OSC_HFCK_Enable(TRUE);
	/* 外设低速时钟 */
	OSC_LFCK_Division_Config(LFCK_DIVISION_1);
	OSC_LFCK_Source_Config(LFCK_INPUT_INTLF);
	OSC_LFCK_Enable(TRUE);
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
}

/**
  * 描述  systick_delay初始化
  * 输入  无。
  * 返回  无。
*/
void systick_delay_init(void)
{
#ifdef SYSCLK_FREQ_48MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_48MHz;
#elif defined SYSCLK_FREQ_64MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_64MHz;
#elif defined SYSCLK_FREQ_72MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_72MHz;
#elif defined SYSCLK_FREQ_96MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_96MHz;
#elif defined SYSCLK_FREQ_120MHz
	uint32_t SystemCoreClock = SYSCLK_FREQ_120MHz;
#endif
	SYSTICK_Cmd(FALSE);
	SYSTICK_Clock_Config(SYSTICK_SYS_CLOCK_DIV_2);
	SFR_CLR_BIT_ASM(ST_CALI, ST_CALII_STCLKREF_POS);
	fac_us = SystemCoreClock / 2000000;
	fac_ms = fac_us * 1000;
}


/**
  * 描述  us延时,最大延时 nus<=0xffffff*1000*1000/SYSCLK/2
  * 	48M： 699050us
  * 	72M： 466033ms
  * 	96M： 349525ms
  * 	120M：279620ms
  * 输入  nus
  * 返回  无
*/
void systick_delay_us(unsigned int nus)
{
	uint32_t temp;
	SYSTICK_SFR->RELOAD = fac_us * nus;
	SYSTICK_SFR->CV = 0;
	SYSTICK_SFR->CTL |= ST_CTL_STEN;
	do
	{
		temp = ST_CTL;
	} while ((temp & 0x01) && !(temp & (1 << 16)));
	SYSTICK_SFR->CTL &= ~ST_CTL_STEN;
	SYSTICK_SFR->CV = 0;
}

/**
  * 描述  ms延时,最大延时 nms<=0xffffff*1000/SYSCLK/2
  * 	48M： 699ms
  * 	72M： 466ms
  * 	96M： 349ms
  * 	120M：279ms
  * 输入  mus
  * 返回  无
*/
void systick_delay_ms(unsigned int nms)
{
	uint32_t temp;
	SYSTICK_SFR->RELOAD = (uint32_t)nms * fac_ms;
	SYSTICK_SFR->CV = 0;
	SYSTICK_SFR->CTL |= ST_CTL_STEN;
	do
	{
		temp = ST_CTL;
	} while ((temp & 0x01) && !(temp & (1 << 16)));
	SYSTICK_SFR->CTL &= ~ST_CTL_STEN;
	SYSTICK_SFR->CV = 0;
}

/**
 *  功 能 :	查询当前主时钟频率
 *  参 数 :	无
 *  返 回 :	当前时钟
 */
uint32_t SystemCoreClockUpdate(void)
{
	uint32_t SystemCoreClock;
    uint32_t tmp = 0, pll_n = 1, pll_m = 0, pll_no = 1;
	uint32_t sckdivfactor = 1;
	uint32_t pllsource = INTHF_FREQ_16MHz;
	tmp = ((OSC_CTL0 >> OSC_CTL0_SCKS0_POS) & 0x07);
	switch (tmp) {
		case SCLK_SOURCE_INTHF:
			SystemCoreClock = INTHF_FREQ_16MHz;
			break;
		case SCLK_SOURCE_INTLF:
			SystemCoreClock = INTLF_FREQ_32KHz;
			break;
		case SCLK_SOURCE_EXTHF:
			SystemCoreClock = EXTHF_FREQ_16MHz;
			break;
		case SCLK_SOURCE_EXTLF:
			SystemCoreClock = EXTLF_FREQ_32768Hz;
			break;
		case SCLK_SOURCE_LP4M:
			SystemCoreClock = LP4M_FREQ_4MHz;
			break;
		case SCLK_SOURCE_PLL:
			if ((OSC_CTL0 >> OSC_CTL0_PLL0CKS_POS) & 0x01) {
				pllsource = EXTHF_FREQ_16MHz;
			} else {
				pllsource = INTHF_FREQ_16MHz;
			}
			pll_m = ((PLL_CTL >> PLL_CTL_M0_POS) & 0x3fff);
			pll_n = ((PLL_CTL >> PLL_CTL_N0_POS) & 0x0f);
			pll_no = (0x01 << ((PLL_CTL >> PLL_CTL_OD0_POS) & 0x03));
			sckdivfactor = (0x01 << ((OSC_CTL0 >> OSC_CTL0_SCKDIV0_POS) & 0x07));
			SystemCoreClock = pllsource * pll_m / (pll_n * pll_no * sckdivfactor);
			break;
		default:
			SystemCoreClock = INTHF_FREQ_16MHz;
			break;
	}
	return SystemCoreClock;
}
