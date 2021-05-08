/**
  ******************************************************************************
  * 文件名  kf32a_basic_op.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了模拟运放(OP)外设功能函数，包含：
  *          + 模拟运放(OP)初始化
  *          + 模拟运放(OP)使能
  *          + 模拟运放(OP)输入输出引脚选择
  *
  *********************************************************************
  */

#include "kf32a_basic_op.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_op
/**
  *   ##### 初始化及配置函数 #####
  */
/**
  * 描述 OP外设复位，使能外设时钟。
  * 输入  : 无
  * 返回  无。
  */
void
OP_Reset(void)
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_OPRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_OPRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_OPCLKEN, TRUE);
}

/**
  * 描述 OP校准值。
  * 输入  : OP0/OP1/0P2/0P3
  * 	CAL_VALUE范围位宽6bit位，值0x00-0x3f
  * 返回  无。
  */
void OP_CAL_Configure( uint32_t OPx , uint32_t CAL_VALUE)
{
	 uint32_t tmpreg = 0;
	/* 参数校验 */
    CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
    CHECK_RESTRICTION(CHECK_OP_CAL_VALUE(CAL_VALUE));
    if(OPx==0)
    {
        /*------------- 设置OPCAL寄存器OP0CAL位域 -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP0CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP0CAL,
									tmpreg);
    }
    else if(OPx==1)
    {
        /*------------- 设置OPCAL寄存器OP1CAL位域 -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP1CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP1CAL,
									tmpreg);
    }
    else if(OPx==2)
    {
        /*------------- 设置OPCAL寄存器OP2CAL位域 -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP2CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP2CAL,
									tmpreg);
    }
    else if(OPx==3)
    {
        /*------------- 设置OPCAL寄存器OP3CAL位域 -------------*/
    	tmpreg = CAL_VALUE << OP_CAL_OP3CAL0_POS;
    	OP_SFR->CAL = SFR_Config (OP_SFR->CAL,
									~OP_CAL_OP3CAL,
									tmpreg);
    }
}
/**
  * 描述 OP增益选择。
  * 输入  : OP0/OP1/0P2
  * 	CAL_VALUE范围:GAIN_10/ GAIN_20/ GAIN_40/ GAIN_80
  * 返回  无。
  */
void OP_GAIN_SELSECT(uint32_t OPx , uint32_t GAIN_VALUE)
{
	uint32_t tmpreg = 0;
	/* 参数校验 */
    CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
    CHECK_RESTRICTION(CHECK_OP_GAIN_VALUE(GAIN_VALUE));
    if(OPx==0)
    {
        /*------------- 设置OPCTL0寄存器OP0GAIN位域 -------------*/
    	tmpreg = GAIN_VALUE << OP_CTL0_OP0GAIN10X_POS;
    	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
    								~OP_CTL0_OP0GAIN,
    								tmpreg);
    }
    else if(OPx==1)
    {
        /*------------- 设置OPCTL0寄存器OP1GAIN位域 -------------*/
    	tmpreg = GAIN_VALUE << OP_CTL0_OP1GAIN10X_POS;
    	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
    								~OP_CTL0_OP1GAIN,
    								tmpreg);
    }
    else if(OPx==2)
    {
        /*------------- 设置OPCTL0寄存器OP2GAIN位域 -------------*/
    	tmpreg = GAIN_VALUE << OP_CTL0_OP2GAIN10X_POS;
    	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
    								~OP_CTL0_OP2GAIN,
    								tmpreg);
    }

}
/**
  * 描述  运放3 正端输入选择。
  * 输入  : OP3_POSITIVE_INPUT_AVDD
  * 	OP3_POSITIVE_INPUT_VREF2V
  * 返回  无。
  */
void OP3_POSITIVE_INPUT_SELSECT(uint32_t SEL)
{
	/* 参数校验 */
	CHECK_RESTRICTION(CHECK_OP3_POSITIVE_INPUT_SEL(SEL));
    /*------------- 设置OPCTL0寄存器OP3VINPSEL位域 -------------*/
	OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
								~OP_CTL0_OP3VINPSEL,
								SEL);
}
/**
  * 描述  运放输出使能选择。
  * 输入  : OP0/OP1/OP2/OP3
  * 	TRUE/FALSE
  * 返回  无。
  */
void OP_OUTPUT_EN(uint32_t OPx,FunctionalState NewState)
{
	uint32_t tmpreg = 0;
	/* 参数校验 */
	 CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
	 CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));
	 if(OPx==0)
	 {
		/*------------- 设置OPCTL0寄存器OP0OUTEN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP0OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP0OUTEN,
									tmpreg);
	 }
	 if(OPx==1)
	 {
		/*------------- 设置OPCTL0寄存器OP1OUTEN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP1OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP1OUTEN,
									tmpreg);
	 }
	 if(OPx==2)
	 {
		/*------------- 设置OPCTL0寄存器OP2OUTEN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP2OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP2OUTEN,
									tmpreg);
	 }
	 if(OPx==3)
	 {
		/*------------- 设置OPCTL0寄存器OP3OUTEN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP3OUTEN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP3OUTEN,
									tmpreg);
	 }
}
/**
  * 描述  运放模块使能选择。
  * 输入  : OP0/OP1/OP2/OP3
  * 	TRUE/FALSE
  * 返回  无。
  */
void OP_MODULE_EN(uint32_t OPx,FunctionalState NewState)
{
	uint32_t tmpreg = 0;
	/* 参数校验 */
	 CHECK_RESTRICTION(CHECK_OP_ALL_PERIPH(OPx));
	 CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));
	 if(OPx==0)
	 {
		/*------------- 设置OPCTL0寄存器OP0EN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP0EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP0EN,
									tmpreg);
	 }
	 if(OPx==1)
	 {
		/*------------- 设置OPCTL0寄存器OP1EN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP1EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP1EN,
									tmpreg);
	 }
	 if(OPx==2)
	 {
		/*------------- 设置OPCTL0寄存器OP2EN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP2EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP2EN,
									tmpreg);
	 }
	 if(OPx==3)
	 {
		/*------------- 设置OPCTL0寄存器OP3EN位域 -------------*/
		 tmpreg = NewState << OP_CTL0_OP3EN_POS;
		OP_SFR->CTL0 = SFR_Config (OP_SFR->CTL0,
									~OP_CTL0_OP3EN,
									tmpreg);
	 }
}

#endif //KF32A_Periph_op
