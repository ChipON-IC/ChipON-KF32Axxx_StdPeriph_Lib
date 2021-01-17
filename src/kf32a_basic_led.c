/**
  ******************************************************************************
  * 文件名  kf32a_basic_led.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了LED驱动模块外设功能函数，包含：
  *          + LED驱动模块外设功初始化
  *          + LED驱动模块数据显示设置
  *          + LED驱动模块状态获取
  *
  *********************************************************************
  */
#include "kf32a_basic_led.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_led
/**
  *   ##### LED驱动模块外设功初始化函数 #####
  */

/* LED_CTL0寄存器初始化配置掩码 */
#define LED_CTL0_INIT_MASK              (LED_CTL0_LEDEN \
                                       | LED_CTL0_LEDBR \
                                       | LED_CTL0_PNS \
                                       | LED_CTL0_CS \
                                       | LED_CTL0_CKS1 \
                                       | LED_CTL0_CKS0 )
/* LED_CTL1寄存器初始化配置掩码 */
#define LED_CTL1_INIT_MASK              (LED_CTL1_SEG16EN \
                                       | LED_CTL1_SEG12EN \
                                       | LED_CTL1_SEG8EN \
                                       | LED_CTL1_CCOMEN \
                                       | LED_CTL1_LUM )

/**
  * 描述  复位LED外设，使能外设时钟。
  * 输入 :无。
  * 返回  无。
  */
void
LED_Reset (void)
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_LEDRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_LEDRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_LEDCLKEN, TRUE);

}

/**
  * 描述  模数转换模块(LED)初始化配置。
  * 输入  LEDx: 指向LED内存结构的指针，取值为LED0_SFR~LED2_SFR。
  *       ledInitStruct: LED模块配置信息结构体指针。
  * 返回  无。
  */
void
LED_Configuration (LED_InitTypeDef* ledInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Module_Enable));
    CHECK_RESTRICTION(CHECK_OUTPUT_CONTROL_SEL(ledInitStruct->m_Output_Control_Select));
    CHECK_RESTRICTION(CHECK_WORK_MODE_SEL(ledInitStruct->m_Anodic_Cathodal_Sel));
    CHECK_RESTRICTION(CHECK_LED_CLKSOURCE_SEL(ledInitStruct->m_Source_Clock_Sel));
    CHECK_RESTRICTION(CHECK_CLK_DIVISION_SEL(ledInitStruct->m_Clock_Division));
    CHECK_RESTRICTION(CHECK_CLK_PREDIVISION_SEL(ledInitStruct->m_Clock_PreDivision));

    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Output_SEG12_15_Enable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Output_SEG8_11_Enable));
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(ledInitStruct->m_Output_SEG0_7_Enable));
    CHECK_RESTRICTION(CHECK_COM_NUMBER_SEL(ledInitStruct->m_COM_Number_Select));
    CHECK_RESTRICTION(CHECK_LUM_SEL(ledInitStruct->m_Lum_Select));

    /*---------------- 设置LED_CTL0寄存器 ----------------*/
    /* 根据结构体成员m_Module_Enable，设置LEDEN位域 */
    /* 根据结构体成员m_Output_Control_Select，设置LEDBR位域 */
    /* 根据结构体成员m_Anodic_Cathodal_Sel，设置PNS位域 */
    /* 根据结构体成员m_Source_Clock_Sel，设置CS位域 */
    /* 根据结构体成员m_Clock_Division，设置CKS1位域 */
    /* 根据结构体成员m_Clock_PreDivision，设置CKS0位域 */
    tmpreg = (uint32_t) ((uint32_t)(ledInitStruct->m_Module_Enable << LED_CTL0_LEDEN_POS) | \
                        (ledInitStruct->m_Output_Control_Select) | \
                        (ledInitStruct->m_Anodic_Cathodal_Sel) | \
                        (ledInitStruct->m_Source_Clock_Sel ) | \
                        (ledInitStruct->m_Clock_Division) | \
                        (ledInitStruct->m_Clock_PreDivision)
                        );
    LED_SFR->CTL0 = SFR_Config (LED_SFR->CTL0, ~LED_CTL0_INIT_MASK, tmpreg);

    /*---------------- 设置LED_CTL1寄存器 ----------------*/
    /* 根据结构体成员m_Output_SEG12_15_Enable，设置SEG16EN位域 */
    /* 根据结构体成员m_Output_SEG8_11_Enable，设置SEG12EN位域 */
    /* 根据结构体成员m_Output_SEG0_7_Enable，设置SEG8EN位域 */
    /* 根据结构体成员m_COM_Number_Select，设置COMEN位域 */
    /* 根据结构体成员m_Lum_Select，设置LUM位域 */
    tmpreg = (uint32_t)(((uint32_t)ledInitStruct->m_Output_SEG12_15_Enable << LED_CTL1_SEG16EN_POS) | \
                         ((uint32_t)ledInitStruct->m_Output_SEG8_11_Enable << LED_CTL1_SEG12EN_POS) | \
                         ((uint32_t)ledInitStruct->m_Output_SEG0_7_Enable << LED_CTL1_SEG8EN_POS) | \
                         ledInitStruct->m_COM_Number_Select | \
                         ledInitStruct->m_Lum_Select);
    LED_SFR->CTL1 = SFR_Config (LED_SFR->CTL1, ~LED_CTL1_INIT_MASK, tmpreg);

}

/**
  * 描述  初始化LED配置信息结构体。
  * 输入  LEDInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
LED_Struct_Init (LED_InitTypeDef* LEDInitStruct)
{
    /* 初始化LED显示驱动使能选择 */
    LEDInitStruct->m_Module_Enable = FALSE;
    /* 初始化LED波形输出控制选择 */
    LEDInitStruct->m_Output_Control_Select = LEDSEG_OUTPUT_DISABLE;
    /* 初始化LED共阴共阳选择 */
    LEDInitStruct->m_Anodic_Cathodal_Sel = LED_CATHODAL_DIRECT_MODE;
    /* 初始化LED工作时钟源选择 */
    LEDInitStruct->m_Source_Clock_Sel = LED_CLKSOURCE_HFCLK;
    /* 初始化LED时钟源分频选择 */
    LEDInitStruct->m_Clock_Division = LED_CLK_DIVISION_2;
    /* 初始化LED预分频比选择 */
    LEDInitStruct->m_Clock_PreDivision = LED_CLK_PREDIVISION_16;
    /* 初始化LED通道SEG<15:12>输出使能 */
    LEDInitStruct->m_Output_SEG12_15_Enable = FALSE;
    /* 初始化 LED通道SEG<11:8>输出使能 */
    LEDInitStruct->m_Output_SEG8_11_Enable = FALSE;
    /* 初始化LED通道SEG<7:0>输出使能 */
    LEDInitStruct->m_Output_SEG0_7_Enable = FALSE;
    /* 初始化LED COM 使能数量选择 */
    LEDInitStruct->m_COM_Number_Select = LED_COM_NUM_1;
    /* 初始化LED COM 辉度调节设置选择 */
    LEDInitStruct->m_Lum_Select = LED_COM_LUM_1;

}

/**
  * 描述  获取LED显示帧标志。
  * 输入 :无
  * 返回  中断响应状态，1：完成1次周期扫描，0：未完成周期扫描。
  */
INTStatus
LED_Get_Frame_Flag (void)
{

    uint32_t tmpreg = 0;

    /*------------- 获取LED_CTL1寄存器  FRAMEF位-------------*/
    tmpreg = LED_SFR->CTL1;
    tmpreg = (tmpreg & 0x01 );

    /* 检查中断使能和标志 */
    if (tmpreg)
    {
        /*  LED完成1次周期扫描（1帧） */
        return SET;
    }
    else
    {
        /* LED未完成周期扫描 */
        return RESET;
    }
}
/**
  * 描述  获取显示驱动数据。
  * 输入 :DATAx, (LED_DATA0-LED_DATA7)
  * 返回 ：显示驱动数据，16位有效数据。
  */
uint16_t
LED_Display_Data_Value (uint32_t LED_DATAx)
{
    uint32_t tmpreg = 0;

    /*---------------- 配置LED_DATA寄存器DATA位 ----------------*/
    if(LED_DATAx==LED_DATA0)
    {
    	tmpreg = LED_SFR->DATA0;
    }
    else if(LED_DATAx==LED_DATA1)
    {
    	tmpreg = LED_SFR->DATA1;
    }
    else if(LED_DATAx==LED_DATA2)
    {
    	tmpreg = LED_SFR->DATA2;
    }
    else if(LED_DATAx==LED_DATA3)
    {
    	tmpreg = LED_SFR->DATA3;
    }
    else if(LED_DATAx==LED_DATA4)
    {
    	tmpreg = LED_SFR->DATA4;
    }
    else if(LED_DATAx==LED_DATA5)
    {
    	tmpreg = LED_SFR->DATA5;
    }
    else if(LED_DATAx==LED_DATA6)
    {
    	tmpreg = LED_SFR->DATA6;
    }
    else if(LED_DATAx==LED_DATA7)
    {
    	tmpreg = LED_SFR->DATA7;
    }
    return (uint16_t) tmpreg;
}

/**
  * 描述 : 设置显示数据
  * 输入 : LED_Datax_Sel：Data0-7寄存器
  * 	   LED_Datax_Data：要写入的数据，取值为16位任意数据
  * 	   LED_Datax_Mask：写入数据掩码，取值为16位任意数据
  * 	   				掩码位对应数据位，取1更新数据，取0不更新数据
  * 返回 ：无
  */
void
LED_SET_Datareg (uint8_t LED_Datax_Sel,uint16_t LED_Datax_Data,uint16_t LED_Datax_Mask)
{
	uint16_t tmpreg;
	/*----------------检查是否为Data0-7 ----------------*/
	CHECK_RESTRICTION(CHECK_LED_COM_SEL(LED_Datax_Sel));

	/*----------------写入LED_DATA寄存器DATA位 ----------------*/
	tmpreg = LED_Datax_Data & LED_Datax_Mask;
	switch (LED_Datax_Sel)
	{
	case 0:
		tmpreg |= LED_DATA0 & (~LED_Datax_Mask);
		LED_DATA0 = tmpreg ;
		break;
	case 1:
		tmpreg |= LED_DATA1 & (~LED_Datax_Mask);
		LED_DATA1 = tmpreg ;
		break;
	case 2:
		tmpreg |= LED_DATA2 & (~LED_Datax_Mask);
		LED_DATA2 = tmpreg ;
		break;
	case 3:
		tmpreg |= LED_DATA3 & (~LED_Datax_Mask);
		LED_DATA3 = tmpreg ;
		break;
	case 4:
		tmpreg |= LED_DATA4 & (~LED_Datax_Mask);
		LED_DATA4 = tmpreg ;
		break;
	case 5:
		tmpreg |= LED_DATA5 & (~LED_Datax_Mask);
		LED_DATA5 = tmpreg ;
		break;
	case 6:
		tmpreg |= LED_DATA6 & (~LED_Datax_Mask);
		LED_DATA6 = tmpreg ;
		break;
	case 7:
		tmpreg |= LED_DATA7 & (~LED_Datax_Mask);
		LED_DATA7 = tmpreg ;
		break;
	default:
		break;
	}
}
#endif //KF32A_Periph_led
