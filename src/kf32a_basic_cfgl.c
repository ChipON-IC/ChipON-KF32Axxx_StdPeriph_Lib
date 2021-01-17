/**
  ******************************************************************************
  * 文件名  kf32a_basic_cfgl.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.4
  * 日  期  2019-11-16
  * 描  述  该文件提供了可配置逻辑门(CFLG)相关的功能函数，包含：
  *            + 可配置逻辑门(CFLG)初始化函数
  *            + 可配置逻辑门(CFLG)中断管理函数
  *
  *********************************************************************
  */

#include "kf32a_basic_cfgl.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_cfgl
/* (CFGL)模块私有定义 -------------------------------------------------*/

/* CFGLx_CTL0寄存器初始化配置掩码 */
#define CFGL_CTL0_INIT_MASK		 		(CFGL_CTL0_EN \
                                       | CFGL_CTL0_OE \
                                       | CFGL_CTL0_INTP \
                                       | CFGL_CTL0_INTN \
                                       | CFGL_CTL0_MD \
                                       | CFGL_CTL0_POL \
                                       | CFGL_CTL0_G4POL \
                                       | CFGL_CTL0_G3POL \
                                       | CFGL_CTL0_G2POL \
                                       | CFGL_CTL0_G1POL \
                                       | CFGL_CTL0_D4S \
                                       | CFGL_CTL0_D3S \
                                       | CFGL_CTL0_D2S \
                                       | CFGL_CTL0_D1S \
                                       | CFGL_CTL0_REGIN \
                                       | CFGL_CTL0_TPSEL)

#define CFGL_CTL1_INIT_MASK	0XFFFFFFFF
/**
  *   ##### CFGL模块初始化函数 #####
  */
/**
  * 描述  复位CFGL外设，使能外设时钟。
  * 输入  CFGLx: 指向CFGL内存结构的指针，取值为CFGL0_SFR~CFGL2_SFR。
  * 返回  无。
  */
void
CFGL_Reset ()
{

	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CFGLRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CFGLRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_CFGLCLKEN, TRUE);

}
/**
  * 描述  CFGL模块初始化配置。
  * 输入  CFGLx: 指向CFGL内存结构的指针，取值为CFGL0_SFR~CFGL2_SFR。
  *       CFGLInitStruct: CFGL模块配置信息结构体指针。
  * 返回  无。
  */
void
CFGL_Configuration (CFGL_SFRmap* CFGLx, CFGL_InitTypeDef* CFGLInitStruct)
{
    uint32_t tmpreg = 0;

    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_CFGL_PERIPH(CFGLx));
    CHECK_RESTRICTION(CHECK_CFGL_EN(CFGLInitStruct->m_Module_EN));
    CHECK_RESTRICTION(CHECK_CFGL_OUTPUT(CFGLInitStruct->m_Output_EN));
    CHECK_RESTRICTION(CHECK_CFGL_RISEINT_EN(CFGLInitStruct->m_RaiseINT_EN));
    CHECK_RESTRICTION(CHECK_CFGL_FALLINT_EN(CFGLInitStruct->m_FallINT_EN));
    CHECK_RESTRICTION(CHECK_CFGL_MODE(CFGLInitStruct->m_Mode_Sel));
    CHECK_RESTRICTION(CHECK_CFGL_OUT_POL(CFGLInitStruct->m_Output_Pol));
    CHECK_RESTRICTION(CHECK_G4POL_SEL(CFGLInitStruct->m_G4Output_POL));
    CHECK_RESTRICTION(CHECK_G3POL_SEL(CFGLInitStruct->m_G3Output_POL));
    CHECK_RESTRICTION(CHECK_G2POL_SEL(CFGLInitStruct->m_G2Output_POL));
    CHECK_RESTRICTION(CHECK_G1POL_SEL(CFGLInitStruct->m_G1Output_POL));
    CHECK_RESTRICTION(CHECK_G4INPUT_SEL(CFGLInitStruct->m_G4Input_Sel));
    CHECK_RESTRICTION(CHECK_G3INPUT_SEL(CFGLInitStruct->m_G3Input_Sel));
    CHECK_RESTRICTION(CHECK_G2INPUT_SEL(CFGLInitStruct->m_G2Input_Sel));
    CHECK_RESTRICTION(CHECK_G1INPUT_SEL(CFGLInitStruct->m_G1Input_Sel));
    CHECK_RESTRICTION(CHECK_CH4_INPUT_SEL(CFGLInitStruct->m_CH4Data_Sel));
    CHECK_RESTRICTION(CHECK_CH5CH6_INPUT_SEL(CFGLInitStruct->m_CH5CH6Data_Sel));

    /*---------------- 设置CFGL_CTL0寄存器 ----------------*/
    /* 根据结构体成员m_Module_EN，设置EN位域 */
    /* 根据结构体成员m_Output_EN，设置OE位域 */
    /* 根据结构体成员m_RaiseINT_EN，设置INTP位域 */
    /* 根据结构体成员m_FallINT_EN，设置INTN位域 */
    /* 根据结构体成员m_Mode_Sel，设置MD位域 */
    /* 根据结构体成员m_Output_Pol，设置POL位域 */
    /* 根据结构体成员m_G4Output_POL，设置G4POL位域 */
    /* 根据结构体成员m_G3Output_POL，设置G3POL位域 */
    /* 根据结构体成员m_G2Output_POL，设置G2POL位域 */
    /* 根据结构体成员m_G1Output_POL，设置G1POL位域 */
    /* 根据结构体成员m_G4Input_Sel，设置D4S位域 */
    /* 根据结构体成员m_G3Input_Sel，设置D3S位域 */
    /* 根据结构体成员m_G2Input_Sel，设置D2S位域 */
    /* 根据结构体成员m_G1Input_Sel，设置D1S位域 */
    /* 根据结构体成员m_CH4Data_Sel，设置REGIN位域 */
    /* 根据结构体成员m_CH5CH6Data_Sel，设置TPSEL位域 */
    tmpreg = (uint32_t)(CFGLInitStruct->m_Module_EN | \
                        CFGLInitStruct->m_Output_EN | \
                        CFGLInitStruct->m_RaiseINT_EN | \
                        CFGLInitStruct->m_FallINT_EN | \
                        CFGLInitStruct->m_Mode_Sel | \
                        CFGLInitStruct->m_Output_Pol | \
                        CFGLInitStruct->m_G4Output_POL | \
                        CFGLInitStruct->m_G3Output_POL | \
                        CFGLInitStruct->m_G2Output_POL | \
                        CFGLInitStruct->m_G1Output_POL | \
                        CFGLInitStruct->m_G4Input_Sel | \
                        CFGLInitStruct->m_G3Input_Sel | \
                        CFGLInitStruct->m_G2Input_Sel | \
                        CFGLInitStruct->m_G1Input_Sel | \
                        CFGLInitStruct->m_CH4Data_Sel | \
                        CFGLInitStruct->m_CH5CH6Data_Sel);
    CFGLx->CTL0 = SFR_Config (CFGLx->CTL0, ~CFGL_CTL0_INIT_MASK, tmpreg);

    /*---------------- 设置CFGL_CTL1寄存器 ----------------*/
    /* 根据结构体成员m_G4D4_Inphase_EN，设置G4D4T位域 */
    /* 根据结构体成员m_G4D4_Inverse_EN，设置G4D4N位域 */
    /* 根据结构体成员m_G4D3_Inphase_EN，设置G4D3T位域 */
    /* 根据结构体成员m_G4D3_Inverse_EN，设置G4D3N位域 */
    /* 根据结构体成员m_G4D2_Inphase_EN，设置G4D2T位域 */
    /* 根据结构体成员m_G4D2_Inverse_EN，设置G4D2N位域 */
    /* 根据结构体成员m_G4D1_Inphase_EN，设置G4D1T位域 */
    /* 根据结构体成员m_G4D1_Inverse_EN，设置G4D1N位域 */
    /* 根据结构体成员m_G3D4_Inphase_EN，设置G3D4T位域 */
    /* 根据结构体成员m_G3D4_Inverse_EN，设置G3D4N位域 */
    /* 根据结构体成员m_G3D3_Inphase_EN，设置G3D3T位域 */
    /* 根据结构体成员m_G3D3_Inverse_EN，设置G3D3N位域 */
    /* 根据结构体成员m_G3D2_Inphase_EN，设置G3D2T位域 */
    /* 根据结构体成员m_G3D2_Inverse_EN，设置G3D2N位域 */
    /* 根据结构体成员m_G3D1_Inphase_EN，设置G3D1T位域 */
    /* 根据结构体成员m_G3D1_Inverse_EN，设置G3D1N位域 */
    /* 根据结构体成员m_G2D4_Inphase_EN，设置G2D4T位域 */
    /* 根据结构体成员m_G2D4_Inverse_EN，设置G2D4N位域 */
    /* 根据结构体成员m_G2D3_Inphase_EN，设置G2D3T位域 */
    /* 根据结构体成员m_G2D3_Inverse_EN，设置G2D3N位域 */
    /* 根据结构体成员m_G2D2_Inphase_EN，设置G2D2T位域 */
    /* 根据结构体成员m_G2D2_Inverse_EN，设置G2D2N位域 */
    /* 根据结构体成员m_G2D1_Inphase_EN，设置G2D1T位域 */
    /* 根据结构体成员m_G2D1_Inverse_EN，设置G2D1N位域 */
    /* 根据结构体成员m_G1D4_Inphase_EN，设置G1D4T位域 */
    /* 根据结构体成员m_G1D4_Inverse_EN，设置G1D4N位域 */
    /* 根据结构体成员m_G1D3_Inphase_EN，设置G1D3T位域 */
    /* 根据结构体成员m_G1D3_Inverse_EN，设置G1D3N位域 */
    /* 根据结构体成员m_G1D2_Inphase_EN，设置G1D2T位域 */
    /* 根据结构体成员m_G1D2_Inverse_EN，设置G1D2N位域 */
    /* 根据结构体成员m_G1D1_Inphase_EN，设置G1D1T位域 */
    /* 根据结构体成员m_G1D1_Inverse_EN，设置G1D1N位域 */
    tmpreg = (uint32_t)((CFGLInitStruct->m_G4D4_Inphase_EN << CFGL_CTL1_G4D4T_POS) | \
    					(CFGLInitStruct->m_G4D4_Inverse_EN << CFGL_CTL1_G4D4N_POS) | \
    					(CFGLInitStruct->m_G4D3_Inphase_EN << CFGL_CTL1_G4D3T_POS) | \
    					(CFGLInitStruct->m_G4D3_Inverse_EN << CFGL_CTL1_G4D3N_POS) | \
    					(CFGLInitStruct->m_G4D2_Inphase_EN << CFGL_CTL1_G4D2T_POS) | \
    					(CFGLInitStruct->m_G4D2_Inverse_EN << CFGL_CTL1_G4D2N_POS) | \
    					(CFGLInitStruct->m_G4D1_Inphase_EN << CFGL_CTL1_G4D1T_POS) | \
    					(CFGLInitStruct->m_G4D1_Inverse_EN << CFGL_CTL1_G4D1N_POS) | \
    					(CFGLInitStruct->m_G3D4_Inphase_EN << CFGL_CTL1_G3D4T_POS) | \
						(CFGLInitStruct->m_G3D4_Inverse_EN << CFGL_CTL1_G3D4N_POS) | \
						(CFGLInitStruct->m_G3D3_Inphase_EN << CFGL_CTL1_G3D3T_POS) | \
						(CFGLInitStruct->m_G3D3_Inverse_EN << CFGL_CTL1_G3D3N_POS) | \
						(CFGLInitStruct->m_G3D2_Inphase_EN << CFGL_CTL1_G3D2T_POS) | \
						(CFGLInitStruct->m_G3D2_Inverse_EN << CFGL_CTL1_G3D2N_POS) | \
						(CFGLInitStruct->m_G3D1_Inphase_EN << CFGL_CTL1_G3D1T_POS) | \
						(CFGLInitStruct->m_G3D1_Inverse_EN << CFGL_CTL1_G3D1N_POS) | \
    					(CFGLInitStruct->m_G2D4_Inphase_EN << CFGL_CTL1_G2D4T_POS) | \
						(CFGLInitStruct->m_G2D4_Inverse_EN << CFGL_CTL1_G2D4N_POS) | \
						(CFGLInitStruct->m_G2D3_Inphase_EN << CFGL_CTL1_G2D3T_POS) | \
						(CFGLInitStruct->m_G2D3_Inverse_EN << CFGL_CTL1_G2D3N_POS) | \
						(CFGLInitStruct->m_G2D2_Inphase_EN << CFGL_CTL1_G2D2T_POS) | \
						(CFGLInitStruct->m_G2D2_Inverse_EN << CFGL_CTL1_G2D2N_POS) | \
						(CFGLInitStruct->m_G2D1_Inphase_EN << CFGL_CTL1_G2D1T_POS) | \
						(CFGLInitStruct->m_G2D1_Inverse_EN << CFGL_CTL1_G2D1N_POS) | \
    					(CFGLInitStruct->m_G1D4_Inphase_EN << CFGL_CTL1_G1D4T_POS) | \
						(CFGLInitStruct->m_G1D4_Inverse_EN << CFGL_CTL1_G1D4N_POS) | \
						(CFGLInitStruct->m_G1D3_Inphase_EN << CFGL_CTL1_G1D3T_POS) | \
						(CFGLInitStruct->m_G1D3_Inverse_EN << CFGL_CTL1_G1D3N_POS) | \
						(CFGLInitStruct->m_G1D2_Inphase_EN << CFGL_CTL1_G1D2T_POS) | \
						(CFGLInitStruct->m_G1D2_Inverse_EN << CFGL_CTL1_G1D2N_POS) | \
						(CFGLInitStruct->m_G1D1_Inphase_EN << CFGL_CTL1_G1D1T_POS) | \
						(CFGLInitStruct->m_G1D1_Inverse_EN << CFGL_CTL1_G1D1N_POS));
    CFGLx->CTL1 = SFR_Config (CFGLx->CTL1, ~CFGL_CTL1_INIT_MASK, tmpreg);

}

/**
  * 描述  初始化CFGL配置信息结构体。
  * 输入  CFGLInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
CFGL_Struct_Init (CFGL_InitTypeDef* CFGLInitStruct)
{
    /* 初始化CFGL 逻辑单元使能控制位 */
	CFGLInitStruct->m_Module_EN = CFGL_DISABLE;
    /* 初始化 CFGL 输出使能选择位 */
    CFGLInitStruct->m_Output_EN = CFGL_OUTPUT_DISABLE;
    /* 初始化 CFGL 上升边沿中断选择 */
    CFGLInitStruct->m_RaiseINT_EN = CFGL_RISEINT_DISABLE;
    /* 初始化 CFGL 下降边沿中断选择 */
    CFGLInitStruct->m_FallINT_EN = CFGL_FALLINT_DISABLE;
    /* 初始化CFGL 可配置逻辑单元功能模式选择 */
    CFGLInitStruct->m_Mode_Sel = CFGL_AND_OR;
    /* 初始化 CFGL 输出极性选择位 */
    CFGLInitStruct->m_Output_Pol = CFGL_OUT_INPHASE;
    /* 初始化CFGL门4输出极性选择 */
	CFGLInitStruct->m_G4Output_POL = CFGL_G4POL_INPHASE;
    /* 初始化 CFGL门3输出极性选择 */
    CFGLInitStruct->m_G3Output_POL = CFGL_G3POL_INPHASE;
    /* 初始化 CFGL门2输出极性选择 */
    CFGLInitStruct->m_G2Output_POL = CFGL_G2POL_INPHASE;
    /* 初始化 CFGL门1输出极性选择 */
    CFGLInitStruct->m_G1Output_POL = CFGL_G1POL_INPHASE;
    /* 初始化CFGL门4输入数据通道选择 */
    CFGLInitStruct->m_G4Input_Sel = CFGL_G4INPUT_CFGL1CH3;
    /* 初始化CFGL门3输入数据通道选择 */
    CFGLInitStruct->m_G3Input_Sel = CFGL_G3INPUT_CFGL1CH15;
    /* 初始化CFGL门2输入数据通道选择 */
    CFGLInitStruct->m_G2Input_Sel = CFGL_G2INPUT_CFGL1CH11;
    /* 初始化CFGL门1输入数据通道选择 */
    CFGLInitStruct->m_G1Input_Sel = CFGL_G1INPUT_CFGL1CH7;
    /* 初始化CFGL CH4通道输入数据位选择 */
    CFGLInitStruct->m_CH4Data_Sel = CFGL_CH4_INPUT_0;
    /* 初始化CH5\CH6通道选择 */
    CFGLInitStruct->m_CH5CH6Data_Sel = CFGL_CH5_T21TRGO_CH6_CCP21CH1OUT;

    /* 门4数据4同向允许通道选择 */
    CFGLInitStruct->m_G4D4_Inphase_EN = FALSE;
    /* 门4数据4反向允许通道选择 */
    CFGLInitStruct->m_G4D4_Inverse_EN = FALSE;
    /* 门4数据3同向允许通道选择 */
    CFGLInitStruct->m_G4D3_Inphase_EN = FALSE;
    /* 门4数据3反向允许通道选择 */
    CFGLInitStruct->m_G4D3_Inverse_EN = FALSE;
    /* 门4数据2同向允许通道选择 */
    CFGLInitStruct->m_G4D2_Inphase_EN = FALSE;
    /* 门4数据2反向允许通道选择 */
    CFGLInitStruct->m_G4D2_Inverse_EN = FALSE;
    /* 门4数据1同向允许通道选择 */
    CFGLInitStruct->m_G4D1_Inphase_EN = FALSE;
    /* 门4数据1反向允许通道选择 */
    CFGLInitStruct->m_G4D1_Inverse_EN = FALSE;

    /* 门3数据4同向允许通道选择 */
    CFGLInitStruct->m_G3D4_Inphase_EN = FALSE;
    /* 门3数据4反向允许通道选择 */
    CFGLInitStruct->m_G3D4_Inverse_EN = FALSE;
    /* 门3数据3同向允许通道选择 */
    CFGLInitStruct->m_G3D3_Inphase_EN = FALSE;
    /* 门3数据3反向允许通道选择 */
    CFGLInitStruct->m_G3D3_Inverse_EN = FALSE;
    /* 门3数据2同向允许通道选择 */
    CFGLInitStruct->m_G3D2_Inphase_EN = FALSE;
    /* 门3数据2反向允许通道选择 */
    CFGLInitStruct->m_G3D2_Inverse_EN = FALSE;
    /* 门3数据1同向允许通道选择 */
    CFGLInitStruct->m_G3D1_Inphase_EN = FALSE;
    /* 门3数据1反向允许通道选择 */
    CFGLInitStruct->m_G3D1_Inverse_EN = FALSE;

    /* 门2数据4同向允许通道选择 */
    CFGLInitStruct->m_G2D4_Inphase_EN = FALSE;
    /* 门2数据4反向允许通道选择 */
    CFGLInitStruct->m_G2D4_Inverse_EN = FALSE;
    /* 门2数据3同向允许通道选择 */
    CFGLInitStruct->m_G2D3_Inphase_EN = FALSE;
    /* 门2数据3反向允许通道选择 */
    CFGLInitStruct->m_G2D3_Inverse_EN = FALSE;
    /* 门2数据2同向允许通道选择 */
    CFGLInitStruct->m_G2D2_Inphase_EN = FALSE;
    /* 门2数据2反向允许通道选择 */
    CFGLInitStruct->m_G2D2_Inverse_EN = FALSE;
    /* 门2数据1同向允许通道选择 */
    CFGLInitStruct->m_G2D1_Inphase_EN = FALSE;
    /* 门2数据1反向允许通道选择 */
    CFGLInitStruct->m_G2D1_Inverse_EN = FALSE;

    /* 门1数据4同向允许通道选择 */
    CFGLInitStruct->m_G1D4_Inphase_EN = FALSE;
    /* 门1数据4反向允许通道选择 */
    CFGLInitStruct->m_G1D4_Inverse_EN = FALSE;
    /* 门1数据3同向允许通道选择 */
    CFGLInitStruct->m_G1D3_Inphase_EN = FALSE;
    /* 门1数据3反向允许通道选择 */
    CFGLInitStruct->m_G1D3_Inverse_EN = FALSE;
    /* 门1数据2同向允许通道选择 */
    CFGLInitStruct->m_G1D2_Inphase_EN = FALSE;
    /* 门1数据2反向允许通道选择 */
    CFGLInitStruct->m_G1D2_Inverse_EN = FALSE;
    /* 门1数据1同向允许通道选择 */
    CFGLInitStruct->m_G1D1_Inphase_EN = FALSE;
    /* 门1数据1反向允许通道选择 */
    CFGLInitStruct->m_G1D1_Inverse_EN = FALSE;
}

/**
  * 描述  设置CFGL2 输出同步使能。
  * 输入NewState: 使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
CFGL2_OUT_SYNCHRO_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置CFGLIFR寄存器CFGL2SYNCEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CFGL2 输出同步位 */
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2SYNCEN_POS);

    }
    else
    {
        /* 禁止CFGL2 输出同步*/
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2SYNCEN_POS);
    }
}
/**
  * 描述  设置CFGL1 输出同步使能。
  * 输入NewState: 使能状态，取值为TRUE 或 FALSE。
  * 返回  无。
  */
void
CFGL1_OUT_SYNCHRO_Enable (FunctionalState NewState)
{
    /* 参数校验 */
    CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置CFGLIFR寄存器CFGL1SYNCEN位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CFGL1 输出同步位 */
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1SYNCEN_POS);

    }
    else
    {
        /* 禁止CFGL1 输出同步*/
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1SYNCEN_POS);
    }
}
/**
  * 描述  设置CFGL2 在 CFGL2_OUT中断下降沿设置选择。
  * 输入NewState: 使能状态，取值为TRUE 或 FALSE。
  *
  * 返回  无。
  */
void
CFGL2_FALLINT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置CFGLIFR寄存器CFGL2IE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CFGL2 下降沿中断位 */
    	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);

    }
    else
    {
        /* 禁止CFGL2 下降沿中断*/
    	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);
    }
}
/**
  * 描述  设置CFGL2 在 CFGL2_OUT上升沿中断设置选择。
  * 输入NewState: 使能状态，取值为TRUE 或 FALSE。
  *
  * 返回  无。
  */
void
CFGL2_RISEINT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置CFGLIFR寄存器CFGL2IE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CFGL2 上升沿中断位 */
    	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);

    }
    else
    {
        /* 禁止CFGL2 上升沿中断*/
    	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL2IE_POS);
    }
}
/**
  * 描述  设置CFGL1 在 CFGL1_OUT中断下降沿设置选择。
  * 输入NewState: 使能状态，取值为TRUE 或 FALSE。
  *
  * 返回  无。
  */
void
CFGL1_FALLINT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置CFGLIFR寄存器CFGL2IE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CFGL1 下降沿中断位 */
    	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);

    }
    else
    {
        /* 禁止CFGL1 下降沿中断*/
    	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);
    }
}
/**
  * 描述  设置CFGL1 在 CFGL1_OUT上升沿中断设置选择。
  * 输入NewState: 使能状态，取值为TRUE 或 FALSE。
  *
  * 返回  无。
  */
void
CFGL1_RISEINT_Enable (FunctionalState NewState)
{
    /* 参数校验 */
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(NewState));

    /*---------------- 设置CFGLIFR寄存器CFGL1IE位 ----------------*/
    if (NewState != FALSE)
    {
        /* 使能CFGL1 上升沿中断位 */
    	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
    	SFR_SET_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);

    }
    else
    {
        /* 禁止CFGL1 上升沿中断*/
    	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
        SFR_CLR_BIT_ASM(CFGLIFR_SFR->IFR, CFGL_IFR_CFGL1IE_POS);
    }
}
/**
  * 描述  读取CFGL1 中断标志。
  * 输入：无
  * 返回 FlagStatus
  */
FlagStatus
CFGL1_Get_INT_Flag ()
{
    /*---------------- 读CFGLIFR寄存器CFGL1IF位 ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)
    {
        /* 检测到CFGL1中断 */
        return SET;
    }
    else
    {
        /* 没有检测到CFGL1中断 */
        return RESET;
    }
}
/**
  * 描述  读取CFGL2 中断标志。
  * 输入：无
  * 返回 FlagStatus
  */
FlagStatus
CFGL2_Get_INT_Flag ()
{
    /*---------------- 读CFGLIFR寄存器CFGL2IF位 ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)
    {
        /* 检测到CFGL2中断标志 */
        return SET;
    }
    else
    {
        /* 没有检测到CFGL2中断 */
        return RESET;
    }
}
/**
  * 描述  清CFGL1 上升沿中断标志。
  * 输入：无
  * 返回   无
  */
void
CFGL1_Clear_RISEINT_Flag (void)
{
    /*---------------- 清CFGLIFR寄存器CFGL1IF位 ----------------*/
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)>>CFGL_IFR_CFGL1IF_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * 描述  清CFGL1 下降沿中断标志。
  * 输入：无
  * 返回   无
  */
void
CFGL1_Clear_FALLINT_Flag (void)
{
    /*---------------- 清CFGLIFR寄存器CFGL1IF位 ----------------*/
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)>>CFGL_IFR_CFGL1IF_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
}
/**
  * 描述  清CFGL 上升沿和下降沿中断标志。
  * 输入：无
  * 返回   无
  */
void
CFGL1_Clear_RISEFALLINT_Flag (void)
{
    /*---------------- 清CFGLIFR寄存器CFGL2IF位 ----------------*/
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_CLR_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1IF)>>CFGL_IFR_CFGL1IF_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_SET_BIT_ASM(CFGL1_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * 描述  清CFGL2 上升沿中断标志。
  * 输入：无
  * 返回   无
  */
void
CFGL2_Clear_RISEINT_Flag (void)
{
    /*---------------- 清CFGLIFR寄存器CFGL2IF位 ----------------*/
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)>>CFGL_IFR_CFGL2IF_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * 描述  清CFGL 上升沿和下降沿中断标志。
  * 输入：无
  * 返回   无
  */
void
CFGL2_Clear_RISEFALLINT_Flag (void)
{
    /*---------------- 清CFGLIFR寄存器CFGL2IF位 ----------------*/
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)>>CFGL_IFR_CFGL2IF_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTP_POS);
}
/**
  * 描述  清CFGL2 下降沿中断标志。
  * 输入：无
  * 返回   无
  */
void
CFGL2_Clear_FALLINT_Flag (void)
{
    /*---------------- 清CFGLIFR寄存器CFGL2IF位 ----------------*/
	SFR_CLR_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
	while((CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2IF)>>CFGL_IFR_CFGL2IF_POS);
	SFR_SET_BIT_ASM(CFGL2_SFR->CTL0, CFGL_CTL0_INTN_POS);
}
/**
  * 描述  读取CFGL2 数据状态。
  * 输入：无
  * 返回 FlagStatus
  */
FlagStatus
CFGL2_OUT_STATE ()
{
    /*---------------- 读CFGLIFR寄存器CFGL2OUT位 ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL2OUT)
    {
        /* 检测到CFGL2数据状态为1*/
        return SET;
    }
    else
    {
        /* 检测到CFGL2数据状态为0 */
        return RESET;
    }
}
/**
  * 描述  读取CFGL1 数据状态。
  * 输入：无
  * 返回 FlagStatus
  */
FlagStatus
CFGL1_OUT_STATE ()
{
    /*---------------- 读CFGLIFR寄存器CFGL1OUT位 ----------------*/
    if (CFGLIFR_SFR->IFR & CFGL_IFR_CFGL1OUT)
    {
        /* 检测到CFGL1数据状态为1*/
        return SET;
    }
    else
    {
        /* 检测到CFGL1数据状态为0 */
        return RESET;
    }
}
#endif //KF32A_Periph_cfgl
