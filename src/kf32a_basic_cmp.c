/**
  ******************************************************************************
  * 文件名  kf32a_basic_cmp.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了CMP外设功能函数，包含：
  *          + CMP外设初始化及配置函数
  *          + CMP外设数据读写函数
  *          + CMP校验寄存器配置函数
  *
  *********************************************************************
  */

#include "kf32a_basic_cmp.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_cmp
/* CMP私有定义 ----------------------------------------------------*/
/* CMP_CTL0寄存器配置掩码 */
#define CMP_CTL0_INIT_MASK              (CMP_CTL0_PMOD \
                                       | CMP_CTL0_NMOD \
                                       | CMP_CTL0_IFFEN \
                                       | CMP_CTL0_IFREN \
                                       | CMP_CTL0_FLTCS \
                                       | CMP_CTL0_FDIV \
                                       | CMP_CTL0_FLTCNT \
                                       | CMP_CTL0_FLTEN \
                                       | CMP_CTL0_WINEN \
                                       | CMP_CTL0_POL)
/* CMP_CTL1寄存器配置掩码 */
#define CMP_CTL1_INIT_MASK              (CMP_CTL1_PMOD \
                                       | CMP_CTL1_NMOD \
                                       | CMP_CTL1_IFFEN \
                                       | CMP_CTL1_IFREN \
                                       | CMP_CTL1_FLTCS \
                                       | CMP_CTL1_FDIV \
                                       | CMP_CTL1_FLTCNT \
                                       | CMP_CTL1_FLTEN \
                                       | CMP_CTL1_WINEN \
                                       | CMP_CTL1_POL)
/* CMP_CTL2寄存器配置掩码 */
#define CMP_CTL2_INIT_MASK              (CMP_CTL2_PMOD \
                                       | CMP_CTL2_NMOD \
                                       | CMP_CTL2_IFFEN \
                                       | CMP_CTL2_IFREN \
                                       | CMP_CTL2_FLTCS \
                                       | CMP_CTL2_FDIV \
                                       | CMP_CTL2_FLTCNT \
                                       | CMP_CTL2_FLTEN \
                                       | CMP_CTL2_WINEN \
                                       | CMP_CTL2_POL)
/* CMP_CTL3寄存器配置掩码 */
#define CMP_CTL3_INIT_MASK              (CMP_CTL3_PMOD \
                                       | CMP_CTL3_NMOD \
                                       | CMP_CTL3_FLTCS \
                                       | CMP_CTL3_FDIV \
                                       | CMP_CTL3_FLTCNT \
                                       | CMP_CTL3_FLTEN \
                                       | CMP_CTL3_WINEN \
                                       | CMP_CTL3_POL \
                                       | CMP_CTL3_C3EN)



/**
  *   ##### 初始化及配置函数 #####
  */
/**
  * 描述  CMP外设复位，使能外设时钟。
  * 输入  CMPx: 无
  * 返回  无。
  */
void
CMP_Reset ()
{
	RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_CMPRST, TRUE);
	RST_CTL1_Peripheral_Reset_Enable(RST_CTL1_CMPRST, FALSE);
	PCLK_CTL1_Peripheral_Clock_Enable(PCLK_CTL1_CMPCLKEN, TRUE);
}

/**
  * 描述  CMP外设配置。
  * 输入  CMPx: 取值为CMP0_SFR、CMP1_SFR、CMP2_SFR、CMP3_SFR。
  *       CMPInitStruct: CMP配置信息
  * 返回  无。
  */
void
CMP_Configuration (CMP_SFRmap * CMPx, CMP_InitTypeDef* CMPInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP_ALL_PERIPH(CMPx));

    if(CMPx==CMP0_SFR)
    {
    	CHECK_RESTRICTION(CHECK_CMP0_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
    	CHECK_RESTRICTION(CHECK_CMP0_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
    	CHECK_RESTRICTION(CHECK_CMP0_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
    	CHECK_RESTRICTION(CHECK_CMP0_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
    	CHECK_RESTRICTION(CHECK_CMP0_OUTPUT(CMPInitStruct->m_OutputPolarity));
    	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
        /*---------------------------- CMP_CTL0寄存器配置  -----------------*/
        /* 根据结构体成员m_PositiveInput，设置PMOD0位域 */
        /* 根据结构体成员m_NegativeInput，设置NMOD0位域 */
        /* 根据结构体成员m_FallTriggerEnable，设置IFFEN位域 */
        /* 根据结构体成员m_RiseTriggerEnable，设置IFREN位域 */
        /* 根据结构体成员m_Clock，设置FLTCS位域 */
		/* 根据结构体成员m_FrequencyDivision，设置FLTDIV位域 */
        /* 根据结构体成员m_SampleNumber，设置FLTCNT位域 */
        /* 根据结构体成员m_FilterEnable，设置FLTEN位域 */
        /* 根据结构体成员m_ScopecontrolEnable，设置WINEN位域 */
        /* 根据结构体成员m_OutputPolarity，设置POL位域 */

	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
				  (CMPInitStruct->m_NegativeInput) | \
				  (CMPInitStruct->m_FallTriggerEnable << CMP_CTL0_IFFEN_POS) | \
				  (CMPInitStruct->m_RiseTriggerEnable << CMP_CTL0_IFREN_POS) | \
				  (CMPInitStruct->m_Clock) | \
				  (CMPInitStruct->m_FrequencyDivision) | \
				  (CMPInitStruct->m_SampleNumber) | \
				  (CMPInitStruct->m_FilterEnable << CMP_CTL0_FLTEN_POS) | \
				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL0_WINEN_POS) | \
				  (CMPInitStruct->m_OutputPolarity));
	   CMPx->CTL = SFR_Config (CMPx->CTL,
			   	   ~CMP_CTL0_INIT_MASK,
			   	   tmpreg);

	    if ((CMPInitStruct->m_CmpEnable) != FALSE)
	    {
	        /* 使能CMP */
	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
	    }
	    else
	    {
	        /* 禁止CMP */
	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
	    }
    }
    else if(CMPx==CMP1_SFR)
    {
    	CHECK_RESTRICTION(CHECK_CMP1_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
    	CHECK_RESTRICTION(CHECK_CMP1_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
    	CHECK_RESTRICTION(CHECK_CMP1_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
    	CHECK_RESTRICTION(CHECK_CMP1_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
        CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
    	CHECK_RESTRICTION(CHECK_CMP1_OUTPUT(CMPInitStruct->m_OutputPolarity));
    	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
        /*---------------------------- CMP_CTL1寄存器配置  -----------------*/
        /* 根据结构体成员m_PositiveInput，设置PMOD0位域 */
        /* 根据结构体成员m_NegativeInput，设置NMOD0位域 */
        /* 根据结构体成员m_FallTriggerEnable，设置IFFEN位域 */
        /* 根据结构体成员m_RiseTriggerEnable，设置IFREN位域 */
        /* 根据结构体成员m_Clock，设置FLTCS位域 */
		/* 根据结构体成员m_FrequencyDivision，设置FLTDIV位域 */
        /* 根据结构体成员m_SampleNumber，设置FLTCNT位域 */
        /* 根据结构体成员m_FilterEnable，设置FLTEN位域 */
        /* 根据结构体成员m_ScopecontrolEnable，设置WINEN位域 */
        /* 根据结构体成员m_OutputPolarity，设置POL位域 */

	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
				  (CMPInitStruct->m_NegativeInput) | \
				  (CMPInitStruct->m_FallTriggerEnable << CMP_CTL1_IFFEN_POS) | \
				  (CMPInitStruct->m_RiseTriggerEnable << CMP_CTL1_IFREN_POS) | \
				  (CMPInitStruct->m_Clock) | \
				  (CMPInitStruct->m_FrequencyDivision) | \
				  (CMPInitStruct->m_SampleNumber) | \
				  (CMPInitStruct->m_FilterEnable << CMP_CTL1_FLTEN_POS) | \
				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL1_WINEN_POS) | \
				  (CMPInitStruct->m_OutputPolarity));
	   CMPx->CTL = SFR_Config (CMPx->CTL, ~CMP_CTL1_INIT_MASK, tmpreg);

	    if ((CMPInitStruct->m_CmpEnable) != FALSE)
	    {
	        /* 使能CMP */
	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
	    }
	    else
	    {
	        /* 禁止CMP */
	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
	    }

    }
    else if(CMPx==CMP2_SFR)
    {
       	CHECK_RESTRICTION(CHECK_CMP2_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
        	CHECK_RESTRICTION(CHECK_CMP2_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
        	CHECK_RESTRICTION(CHECK_CMP2_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
        	CHECK_RESTRICTION(CHECK_CMP2_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
            CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
        	CHECK_RESTRICTION(CHECK_CMP2_OUTPUT(CMPInitStruct->m_OutputPolarity));
        	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
            /*---------------------------- CMP_CTL2寄存器配置  -----------------*/
            /* 根据结构体成员m_PositiveInput，设置PMOD0位域 */
            /* 根据结构体成员m_NegativeInput，设置NMOD0位域 */
            /* 根据结构体成员m_FallTriggerEnable，设置IFFEN位域 */
            /* 根据结构体成员m_RiseTriggerEnable，设置IFREN位域 */
            /* 根据结构体成员m_Clock，设置FLTCS位域 */
    		/* 根据结构体成员m_FrequencyDivision，设置FLTDIV位域 */
            /* 根据结构体成员m_SampleNumber，设置FLTCNT位域 */
            /* 根据结构体成员m_FilterEnable，设置FLTEN位域 */
            /* 根据结构体成员m_ScopecontrolEnable，设置WINEN位域 */
            /* 根据结构体成员m_OutputPolarity，设置POL位域 */

    	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
    				  (CMPInitStruct->m_NegativeInput) | \
    				  (CMPInitStruct->m_FallTriggerEnable << CMP_CTL2_IFFEN_POS) | \
    				  (CMPInitStruct->m_RiseTriggerEnable << CMP_CTL2_IFREN_POS) | \
    				  (CMPInitStruct->m_Clock) | \
    				  (CMPInitStruct->m_FrequencyDivision) | \
    				  (CMPInitStruct->m_SampleNumber) | \
    				  (CMPInitStruct->m_FilterEnable << CMP_CTL2_FLTEN_POS) | \
    				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL2_WINEN_POS) | \
    				  (CMPInitStruct->m_OutputPolarity));
    	   CMPx->CTL = SFR_Config (CMPx->CTL, ~CMP_CTL2_INIT_MASK, tmpreg);


   	    if ((CMPInitStruct->m_CmpEnable) != FALSE)
   	    {
   	        /* 使能CMP */
   	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
   	    }
   	    else
   	    {
   	        /* 禁止CMP */
   	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
   	    }
    }
    else if(CMPx==CMP3_SFR)
    {
       	CHECK_RESTRICTION(CHECK_CMP3_PositiveINPUT_PIN(CMPInitStruct->m_PositiveInput));
		CHECK_RESTRICTION(CHECK_CMP3_NegativeINPUT_PIN(CMPInitStruct->m_NegativeInput));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FallTriggerEnable));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_RiseTriggerEnable));
		CHECK_RESTRICTION(CHECK_CMP3_CLKSOURCE_SEL(CMPInitStruct->m_Clock));
		CHECK_RESTRICTION(CHECK_CMP3_FLT_SampleNumber_SEL(CMPInitStruct->m_SampleNumber));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_FilterEnable));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_ScopecontrolEnable));
		CHECK_RESTRICTION(CHECK_CMP3_OUTPUT(CMPInitStruct->m_OutputPolarity));
		CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(CMPInitStruct->m_CmpEnable));
		/*---------------------------- CMP_CTL3寄存器配置  -----------------*/
		/* 根据结构体成员m_PositiveInput，设置PMOD0位域 */
		/* 根据结构体成员m_NegativeInput，设置NMOD0位域 */
		/* 根据结构体成员m_FallTriggerEnable，设置IFFEN位域 */
		/* 根据结构体成员m_RiseTriggerEnable，设置IFREN位域 */
		/* 根据结构体成员m_Clock，设置FLTCS位域 */
		/* 根据结构体成员m_FrequencyDivision，设置FLTDIV位域 */
		/* 根据结构体成员m_SampleNumber，设置FLTCNT位域 */
		/* 根据结构体成员m_FilterEnable，设置FLTEN位域 */
		/* 根据结构体成员m_ScopecontrolEnable，设置WINEN位域 */
		/* 根据结构体成员m_OutputPolarity，设置POL位域 */

	   tmpreg = ((CMPInitStruct->m_PositiveInput ) | \
				  (CMPInitStruct->m_NegativeInput) | \
				  (CMPInitStruct->m_Clock) | \
				  (CMPInitStruct->m_FrequencyDivision) | \
				  (CMPInitStruct->m_SampleNumber) | \
				  (CMPInitStruct->m_FilterEnable << CMP_CTL3_FLTEN_POS) | \
				  (CMPInitStruct->m_ScopecontrolEnable << CMP_CTL3_WINEN_POS) | \
				  (CMPInitStruct->m_OutputPolarity) | \
				  (CMPInitStruct->m_CmpEnable << CMP_CTL3_C3EN_POS));
	   CMPx->CTL = SFR_Config (CMPx->CTL, ~CMP_CTL3_INIT_MASK, tmpreg);

  	    if ((CMPInitStruct->m_FallTriggerEnable) != FALSE)
  	    {
  	        /* 使能比较器3下降沿触发中断 */
  	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFFEN_POS);
  	    }
  	    else
  	    {
  	        /* 禁止比较器3下降沿触发中断 */
  	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFFEN_POS);
  	    }

  	    if ((CMPInitStruct->m_RiseTriggerEnable) != FALSE)
  	    {
  	        /* 使能比较器3下降沿触发中断 */
  	        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFREN_POS);
  	    }
  	    else
  	    {
  	        /* 禁止比较器3下降沿触发中断 */
  	        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IFREN_POS);
  	    }
    }
}

/**
  * 描述  初始化CMP配置信息结构体。
  * 输入  CMPInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
CMP_Struct_Init (CMP_InitTypeDef* CMPInitStruct)
{
    /* 初始化 CMP的正端输入选择位 */
    CMPInitStruct->m_PositiveInput = CMP0_PositiveINPUT_PIN_PA0;
    /* 初始化 CMP的负端输入选择位 */
    CMPInitStruct->m_NegativeInput = CMP0_NegativeINPUT_PIN_PA1;
    /* 初始化 CMP下降沿触发中断使能 */
    CMPInitStruct->m_FallTriggerEnable = FALSE;
    /* 初始化 CMP上升沿触发中断使能 */
    CMPInitStruct->m_RiseTriggerEnable = FALSE;
    /* 初始化CMP滤波器滤波时钟源 */
    CMPInitStruct->m_Clock = CMP0_CLKSOURCE_SCLK;
    /* 初始化CMP滤波器滤波时钟分频*/
    CMPInitStruct->m_FrequencyDivision = 0x00;//1分频（值范围0-255）
    /* 初始化 CMP取样数量选择 */
    CMPInitStruct->m_SampleNumber = CMP0_FLT_OFF;
    /* 初始化CMP滤波器使能位 */
    CMPInitStruct->m_FilterEnable = FALSE;
    /* 初始化CMP范围控制使能位*/
    CMPInitStruct->m_ScopecontrolEnable = FALSE;
    /* 初始化 CMP输出极性选择*/
    CMPInitStruct->m_OutputPolarity =CMP0_OUTPUT_Normal;
    /* 初始化CMP比较器3使能 */
    CMPInitStruct->m_CmpEnable =FALSE;
}
/**
  * 描述  CMP0正端输入端口信号选择。
  * 输入选择:
  *           CMP0_PositiveINPUT_PIN_PA0
  *           CMP0_PositiveINPUT_PIN_PA9
  *           CMP0_PositiveINPUT_PIN_PB2
  *           CMP0_PositiveINPUT_PIN_PB9
  *           CMP0_PositiveINPUT_PIN_PC11
  *           CMP0_PositiveINPUT_PIN_AGND
  *           CMP0_PositiveINPUT_PIN_DAC0OUT
  *           CMP0_PositiveINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP0_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP0_PositiveINPUT_PIN(Select));
    /*------------- 设置CMP_CTL0寄存器PMOD位 -------------*/
    CMP_CTL0 = SFR_Config (CMP_CTL0,
                               ~CMP_CTL0_PMOD,
                               Select);
}
/**
  * 描述  CMP0负端输入端口信号选择。
  * 输入选择:
  *           CMP0_NegativeINPUT_PIN_PA1
  *           CMP0_NegativeINPUT_PIN_PA10
  *           CMP0_NegativeINPUT_PIN_PB3
  *           CMP0_NegativeINPUT_PIN_PB10
  *           CMP0_NegativeINPUT_PIN_PC12
  *           CMP0_NegativeINPUT_PIN_AGND
  *           CMP0_NegativeINPUT_PIN_DAC0OUT
  *           CMP0_NegativeINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP0_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP0_NegativeINPUT_PIN(Select));
    /*------------- 设置CMP_CTL0寄存器NMOD位 -------------*/
    CMP_CTL0 = SFR_Config (CMP_CTL0,
                               ~CMP_CTL0_NMOD,
                               Select);
}
/**
  * 描述  CMP1正端输入端口信号选择。
  * 输入选择:
  *           CMP1_PositiveINPUT_PIN_PA0
  *           CMP1_PositiveINPUT_PIN_PA9
  *           CMP1_PositiveINPUT_PIN_PB2
  *           CMP1_PositiveINPUT_PIN_PB11
  *           CMP1_PositiveINPUT_PIN_PG7
  *           CMP1_PositiveINPUT_PIN_AGND
  *           CMP1_PositiveINPUT_PIN_DAC0OUT
  *           CMP1_PositiveINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP1_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP1_PositiveINPUT_PIN(Select));
    /*------------- 设置CMP_CTL1寄存器PMOD位 -------------*/
    CMP_CTL1 = SFR_Config (CMP_CTL1,
                               ~CMP_CTL1_PMOD,
                               Select);
}
/**
  * 描述  CMP1负端输入端口信号选择。
  * 输入选择:
  *           CMP1_NegativeINPUT_PIN_PA1
  *           CMP1_NegativeINPUT_PIN_PA10
  *           CMP1_NegativeINPUT_PIN_PB3
  *           CMP1_NegativeINPUT_PIN_PB12
  *           CMP1_NegativeINPUT_PIN_PC9
  *           CMP1_NegativeINPUT_PIN_AGND
  *           CMP1_NegativeINPUT_PIN_DAC0OUT
  *           CMP1_NegativeINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP1_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP1_NegativeINPUT_PIN(Select));
    /*------------- 设置CMP_CTL1寄存器NMOD位 -------------*/
    CMP_CTL1 = SFR_Config (CMP_CTL1,
                               ~CMP_CTL1_NMOD,
                               Select);
}
/**
  * 描述  CMP2正端输入端口信号选择。
  * 输入选择:
  *           CMP2_PositiveINPUT_PIN_PA0
  *           CMP2_PositiveINPUT_PIN_PA9
  *           CMP2_PositiveINPUT_PIN_PB2
  *           CMP2_PositiveINPUT_PIN_PB13
  *           CMP2_PositiveINPUT_PIN_PC7
  *           CMP2_PositiveINPUT_PIN_AGND
  *           CMP2_PositiveINPUT_PIN_DAC0OUT
  *           CMP2_PositiveINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP2_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP2_PositiveINPUT_PIN(Select));
    /*------------- 设置CMP_CTL2寄存器PMOD位 -------------*/
    CMP_CTL2 = SFR_Config (CMP_CTL2,
                               ~CMP_CTL2_PMOD,
                               Select);
}
/**
  * 描述  CMP2负端输入端口信号选择。
  * 输入选择:
  *           CMP2_NegativeINPUT_PIN_PA1
  *           CMP2_NegativeINPUT_PIN_PA10
  *           CMP2_NegativeINPUT_PIN_PB3
  *           CMP2_NegativeINPUT_PIN_PB14
  *           CMP2_NegativeINPUT_PIN_PC8
  *           CMP2_NegativeINPUT_PIN_AGND
  *           CMP2_NegativeINPUT_PIN_DAC0OUT
  *           CMP2_NegativeINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP2_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP2_NegativeINPUT_PIN(Select));
    /*------------- 设置CMP_CTL2寄存器NMOD位 -------------*/
    CMP_CTL2 = SFR_Config (CMP_CTL2,
                               ~CMP_CTL2_NMOD,
                               Select);
}
/**
  * 描述  CMP3正端输入端口信号选择。
  * 输入选择:
  *  		  CMP3_PositiveINPUT_PIN_OP2OUT
  *           CMP3_PositiveINPUT_PIN_PA0
  *           CMP3_PositiveINPUT_PIN_PA9
  *           CMP3_PositiveINPUT_PIN_PB2
  *           CMP3_PositiveINPUT_PIN_PB15
  *           CMP3_PositiveINPUT_PIN_PC5
  *           CMP3_PositiveINPUT_PIN_AGND
  *           CMP3_PositiveINPUT_PIN_DAC0OUT
  *           CMP3_PositiveINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP3_POSITIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP3_PositiveINPUT_PIN(Select));

    if(Select==CMP3_PositiveINPUT_PIN_OP2OUT)
    {
    	SFR_SET_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_PSEL_POS);
    }
    else
    {
    /*------------- 设置CMP_CTL3寄存器PMOD位 -------------*/
    SFR_CLR_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_PSEL_POS);
    CMP_CTL3 = SFR_Config (CMP_CTL3,
                               ~CMP_CTL3_PMOD,
                               Select);
    }
}
/**
  * 描述  CMP3负端输入端口信号选择。
  * 输入选择:
  *           CMP3_NegativeINPUT_PIN_PA1
  *           CMP3_NegativeINPUT_PIN_PA10
  *           CMP3_NegativeINPUT_PIN_PB3
  *           CMP3_NegativeINPUT_PIN_PF0
  *           CMP3_NegativeINPUT_PIN_PC6
  *           CMP3_NegativeINPUT_PIN_AGND
  *           CMP3_NegativeINPUT_PIN_DAC0OUT
  *           CMP3_NegativeINPUT_PIN_DAC1OUT
  * 返回  无。
  */
void
CMP3_NEGATIVE_INPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP3_NegativeINPUT_PIN(Select));
    /*------------- 设置CMP_CTL3寄存器NMOD位 -------------*/
    CMP_CTL3 = SFR_Config (CMP_CTL3,
                               ~CMP_CTL3_NMOD,
                               Select);
}

/**
  * 描述  CMP0输出极性选择。
  * 输入选择:
  *           CMP0_OUTPUT_Normal
  *           CMP0_OUTPUT_Opposite
  * 返回  无。
  */
void
CMP0_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP0_OUTPUT(Select));
    /*------------- 设置CMP_CTL0寄存器POL位 -------------*/
    CMP_CTL0 = SFR_Config (CMP_CTL0,
                               ~CMP_CTL0_POL,
                               Select);
}
/**
  * 描述  CMP1输出极性选择。
  * 输入选择:
  *           CMP1_OUTPUT_Normal
  *           CMP1_OUTPUT_Opposite
  * 返回  无。
  */
void
CMP1_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP1_OUTPUT(Select));
    /*------------- 设置CMP_CTL1寄存器POL位 -------------*/
    CMP_CTL1 = SFR_Config (CMP_CTL1,
                               ~CMP_CTL1_POL,
                               Select);
}
/**
  * 描述  CMP2输出极性选择。
  * 输入选择:
  *           CMP2_OUTPUT_Normal
  *           CMP2_OUTPUT_Opposite
  * 返回  无。
  */
void
CMP2_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP2_OUTPUT(Select));
    /*------------- 设置CMP_CTL2寄存器POL位 -------------*/
    CMP_CTL2 = SFR_Config (CMP_CTL2,
                               ~CMP_CTL2_POL,
                               Select);
}
/**
  * 描述  CMP3输出极性选择。
  * 输入选择:
  *           CMP3_OUTPUT_Normal
  *           CMP3_OUTPUT_Opposite
  * 返回  无。
  */
void
CMP3_OUTPUT_POL_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP3_OUTPUT(Select));
    /*------------- 设置CMP_CTL2寄存器POL位 -------------*/
    CMP_CTL3 = SFR_Config (CMP_CTL3,
                               ~CMP_CTL3_POL,
                               Select);
}

/**
  * 描述  CMP输出选择。
  * 输入选择:
  * 		  NONE
  *           CMP0_OUTPUT
  *           CMP1_OUTPUT
  *           CMP2_OUTPUT
  * 返回  无。
  */
void
CMP_OUTPUT_SELECT (uint32_t Select)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP_OUTPUT(Select));
    /*------------- 设置CMP_CTL2寄存器POL位 -------------*/
    CMP_CTL4 = SFR_Config (CMP_CTL4,
                               ~CMP_CTL4_CMPOUT,
                               Select);
}
/**
  * 描述  控制CMP0使能位。
  * 输入NewState: CMP使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
CMP0_Cmd (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能CMP */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
    }
    else
    {
        /* 禁止CMP */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0EN_POS);
    }
}
/**
  * 描述  控制CMP1使能位。
  * 输入NewState: CMP使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
CMP1_Cmd (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能CMP */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
    }
    else
    {
        /* 禁止CMP */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1EN_POS);
    }
}
/**
  * 描述  控制CMP2使能位。
  * 输入NewState: CMP使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
CMP2_Cmd (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能CMP */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
    }
    else
    {
        /* 禁止CMP */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2EN_POS);
    }
}
/**
  * 描述  控制CMP3使能位。
  * 输入NewState: CMP使能位配置信息，
  *                 取值为 TRUE 或 FALSE。
  * 返回  无。
  */
void
CMP3_Cmd (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    if (NewState != FALSE)
    {
        /* 使能CMP */
        SFR_SET_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_C3EN_POS);
    }
    else
    {
        /* 禁止CMP */
        SFR_CLR_BIT_ASM(CMP3_SFR->CTL, CMP_CTL3_C3EN_POS);
    }
}
/**
  *   ##### 初始化及配置函数结束 #####
  */


/**
  * 描述  读CMP0输出数据。
  * 输入  : 无
  * 返回  ：比较器输出状态。
  */
FlagStatus
CMP0_Read_Output_State ()
{
    /*---------------- 读取CMP_CTL4寄存器C0OUT位 ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C0OUT)
	{
		/* 比较器输出为1 */
		return SET;
	}
	else
	{
		/* 比较器输出为0 */
		return RESET;
	}
}
/**
  * 描述  读CMP1输出数据。
  * 输入  : 无
  * 返回  ：比较器输出状态。
  */
FlagStatus
CMP1_Read_Output_State ()
{
    /*---------------- 读取CMP_CTL4寄存器C1OUT位 ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C1OUT)
	{
		/* 比较器输出为1 */
		return SET;
	}
	else
	{
		/* 比较器输出为0 */
		return RESET;
	}
}
/**
  * 描述  读CMP2输出数据。
  * 输入  : 无
  * 返回  ：比较器输出状态。
  */
FlagStatus
CMP2_Read_Output_State ()
{
    /*---------------- 读取CMP_CTL4寄存器C2OUT位 ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C2OUT)
	{
		/* 比较器输出为1 */
		return SET;
	}
	else
	{
		/* 比较器输出为0 */
		return RESET;
	}
}
/**
  * 描述  读CMP3输出数据。
  * 输入  : 无
  * 返回  ：比较器输出状态。
  */
FlagStatus
CMP3_Read_Output_State ()
{
    /*---------------- 读取CMP_CTL4寄存器C3OUT位 ----------------*/
	if (CMPCTL4_SFR->CTL & CMP_CTL4_C3OUT)
	{
		/* 比较器输出为1 */
		return SET;
	}
	else
	{
		/* 比较器输出为0 */
		return RESET;
	}
}
/**
  * 描述  读CMP0中断标志。
  * 输入  : 无
  * 返回  ：比较器中断标志状态。
  */
FlagStatus
CMP0_Get_Updata_INT_Flag ()
{
    /*-------------------- 读取CMP_CTL4寄存器C0IF位 --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C0IF)
    {
        /* 检测到比较器0中断 */
        return SET;
    }
    else
    {
        /* 没有检测到比较器0中断 */
        return RESET;
    }
}
/**
  * 描述  读CMP1中断标志。
  * 输入  : 无
  * 返回  ：比较器中断标志状态。
  */
FlagStatus
CMP1_Get_Updata_INT_Flag ()
{
    /*-------------------- 读取CMP_CTL4寄存器C1IF位 --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C1IF)
    {
        /* 检测到比较器1中断 */
        return SET;
    }
    else
    {
        /* 没有检测到比较器1中断 */
        return RESET;
    }
}
/**
  * 描述  读CMP2中断标志。
  * 输入  : 无
  * 返回  ：比较器中断标志状态。
  */
FlagStatus
CMP2_Get_Updata_INT_Flag ()
{
    /*-------------------- 读取CMP_CTL4寄存器C2IF位 --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C2IF)
    {
        /* 检测到比较器2中断 */
        return SET;
    }
    else
    {
        /* 没有检测到比较器2中断 */
        return RESET;
    }
}
/**
  * 描述  读CMP3中断标志。
  * 输入  : 无
  * 返回  ：比较器中断标志状态。
  */
FlagStatus
CMP3_Get_Updata_INT_Flag ()
{
    /*-------------------- 读取CMP_CTL4寄存器C3IF位 --------------------*/
    if (CMPCTL4_SFR->CTL & CMP_CTL4_C3IF)
    {
        /* 检测到比较器3中断 */
        return SET;
    }
    else
    {
        /* 没有检测到比较器3中断 */
        return RESET;
    }
}

/**
  * 描述  配置触发选择位。
  * 输入 TriggerSelect: 触发选择，
  *                      取值范围为：
  *                         CMP_CMPOUT_FlipLatch_INT
  *                         CMP_CMPOUT_Change_INT
  * 返回  无
  */
void
CMP_Trigger_Select_Config (uint32_t TriggerSelect)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CMP_TriggerINT_MODE(TriggerSelect));

    /*-------------------- 设置CMP_CTL4寄存器CIFMS位 --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_CIFMS,
                          TriggerSelect);
}

/**
  * 描述  清除比较器0中断标志。
  * 输入 : 无
  * 返回  无
  */
void
CMP0_Clear_Trigger_INT_Flag ()
{
    /*-------------------- 设置CMPCTL4寄存器C0IC位 --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C0IF)>>CMP_CTL4_C0IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IC_POS);
}
/**
  * 描述  清除比较器1中断标志。
  * 输入 : 无
  * 返回  无
  */
void
CMP1_Clear_Trigger_INT_Flag ()
{
    /*-------------------- 设置CMPCTL4寄存器C1IC位 --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C1IF)>>CMP_CTL4_C1IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IC_POS);
}
/**
  * 描述  清除比较器2中断标志。
  * 输入 : 无
  * 返回  无
  */
void
CMP2_Clear_Trigger_INT_Flag ()
{
    /*-------------------- 设置CMPCTL4寄存器C2IC位 --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C2IF)>>CMP_CTL4_C2IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IC_POS);
}
/**
  * 描述  清除比较器3中断标志。
  * 输入 : 无
  * 返回  无
  */
void
CMP3_Clear_Trigger_INT_Flag ()
{
    /*-------------------- 设置CMPCTL4寄存器C3IC位 --------------------*/
    SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IC_POS);
    while((CMP_CTL4 & CMP_CTL4_C3IF)>>CMP_CTL4_C3IF_POS);
    SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IC_POS);
}

/**
  * 描述  配置CMP中断使能
  * 输入 NewState: 中断取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CMP0_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CMPCTL4寄存器C0IE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许比较器中断 */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IE_POS);
    }
    else
    {
        /* 禁止比较器中断 */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C0IE_POS);
    }
}
void
CMP1_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CMPCTL4寄存器C1IE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许比较器中断 */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IE_POS);
    }
    else
    {
        /* 禁止比较器中断 */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C1IE_POS);
    }
}
void
CMP2_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CMPCTL4寄存器C2IE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许比较器中断 */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IE_POS);
    }
    else
    {
        /* 禁止比较器中断 */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C2IE_POS);
    }
}
void
CMP3_INT_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CMPCTL4寄存器C3IE位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许比较器中断 */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IE_POS);
    }
    else
    {
        /* 禁止比较器中断 */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_C3IE_POS);
    }
}

/**
  * 描述  配置CMP迟滞电压选择位
  * 输入 Select: 取值范围为：
  * CMP_SluggishVoltage_OFF
  * CMP_SluggishVoltage_5mV
  * CMP_SluggishVoltage_10mV
  * CMP_SluggishVoltage_15mV
  * 返回  无
  */
void
CMP_SluggishVoltage_Select (uint32_t Select)
{
    CHECK_RESTRICTION(CHECK_CMP_SluggishVoltage_SEL(Select));

    /*-------------------- 设置CMP_CTL4寄存器TRIM位 --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_TRIM,
                          Select);
}
/**
  * 描述  霍尔模式选择位
  * 输入 Select: 取值范围为：
  * CMP_HALLMODE_SINGLE
  * CMP_HALLMODE_BOTH
  * 返回  无
  */
void
CMP_HALLMODE_Select (uint32_t Select)
{
    CHECK_RESTRICTION(CHECK_CMP_HALLMODE_SEL(Select));

    /*-------------------- 设置CMP_CTL4寄存器TRIM位 --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_HALSEL,
                          Select);
}
/**
  * 描述  反向电动势模式使能
  * 输入 NewState: 中断取值范围为：TRUE 或 FALSE
  * 返回  无
  */
void
CMP_BEMF_Enable (FunctionalState NewState)
{
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*-------------------- 设置CMP_CTL4寄存器BEMFEN位 --------------------*/
    if (NewState != FALSE)
    {
        /* 允许反向电动势模式使能 */
        SFR_SET_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_BEMFEN_POS);
    }
    else
    {
        /* 禁止反向电动势模式使能 */
        SFR_CLR_BIT_ASM(CMPCTL4_SFR->CTL, CMP_CTL4_BEMFEN_POS);
    }
}
/**
  * 描述  滤波器输入选择位
  * 输入 Select: 取值范围为：
  * CMP_FLTINSEL_CMPOUT
  * CMP_FLTINSEL_IO
  * 返回  无
  */
void
CMP_FLTINSEL_Select (uint32_t Select)
{
    CHECK_RESTRICTION(CHECK_CMP_FLTINSEL(Select));

    /*-------------------- 设置CMP_CTL4寄存器FLTINSEL位 --------------------*/
    CMPCTL4_SFR->CTL = SFR_Config (CMPCTL4_SFR->CTL,
                          ~CMP_CTL4_FLTINSEL,
                          Select);
}

#endif //KF32A_Periph_cmp
/**
  *   ##### 数据读写函数结束 #####
  */



/**
  *   ##### CMP校验寄存器配置函数结束 #####
  */
