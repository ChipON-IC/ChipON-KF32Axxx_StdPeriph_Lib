/**
  ******************************************************************************
  * 文件名  kf32a_basic_cfgl.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描  述  该文件提供了逻辑单元模块(CFGL)相关功能函数声明及相关宏定义。
  *********************************************************************
  */
#ifndef _KF32A_BASIC_CFGL_H_
#define _KF32A_BASIC_CFGL_H_

#include "KF32A_BASIC.h"


/**
  * CFGL加密模块（CFGL）指针定义
  */
#define CHECK_CFGL_PERIPH(PERIPH)     (((PERIPH) == CFGL1_SFR)\
									||((PERIPH) == CFGL2_SFR))

/**
 * CFGL 配置信息结构体
 */
typedef struct
{
 uint32_t m_Module_EN;                 /*CFGL 逻辑单元使能
                                 取值为宏“CFGL 逻辑单元使能禁止选择”中的一个*/
 uint32_t m_Output_EN;         /*CFGL 输出使能选择
                                 取值为宏“CFGL 输出使能”中的一个*/
 uint32_t m_RaiseINT_EN;            /*CFGL 上升边沿中断选择
                                 取值为宏“CFGL 上升边沿中断选择”中的一个*/
 uint32_t m_FallINT_EN;             /*CFGL 下降边沿中断选择
                                 取值为宏“CFGL 下降边沿中断选择”中的一个*/
 uint32_t m_Mode_Sel;         /*CFGL 可配置逻辑单元功能模式选择
                                 取值为宏“CFGL 可配置逻辑单元功能模式”中的一个*/
 uint32_t m_Output_Pol;     /*CFGL输出极性选择位
                                 取值为宏“CFGL输出极性”中的一个*/
 uint32_t m_G4Output_POL;     /*CFGL门4输出极性选择
                                 取值为宏“CFGL门4输出极性”中的一个*/
 uint32_t m_G3Output_POL;     /*CFGL门3输出极性选择
                                 取值为宏“CFGL门3输出极性”中的一个*/
 uint32_t m_G2Output_POL;     /*CFGL门2输出极性选择
                                 取值为宏“CFGL门2输出极性”中的一个*/
 uint32_t m_G1Output_POL;     /*CFGL门1输出极性选择
                                 取值为宏“CFGL门1输出极性”中的一个*/
 uint32_t m_G4Input_Sel;     /*CFGL门4输入数据通道选择
                                 取值为宏“CFGL门4输入数据”中的一个*/
 uint32_t m_G3Input_Sel;     /*CFGL门3输入数据通道选择
                                 取值为宏“CFGL门3输入数据”中的一个*/
 uint32_t m_G2Input_Sel;     /*CFGL门2输入数据通道选择
                                 取值为宏“CFGL门2输入数据”中的一个*/
 uint32_t m_G1Input_Sel;     /*CFGL门1输入数据通道选择位
                                 取值为宏“CFGL门1输入数据”中的一个*/
 uint32_t m_CH4Data_Sel;     /*CFGL CH4通道输入数据位选择
                                 取值为宏“CFGL CH4通道输入数据位”中的一个*/
 uint32_t m_CH5CH6Data_Sel;     /*CH5\CH6通道选择
                                 取值为宏“CFGLCH5\CH6通道选择”中的一个*/
 FunctionalState m_G4D4_Inphase_EN;/*门4数据4同向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D4_Inverse_EN;/*门4数据4反向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D3_Inphase_EN;/*门4数据3同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D3_Inverse_EN;/*门4数据3反向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D2_Inphase_EN;/*门4数据2同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D2_Inverse_EN;/*门4数据2反向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D1_Inphase_EN;/*门4数据1同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G4D1_Inverse_EN;/*门4数据1反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D4_Inphase_EN;/*门3数据4同向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D4_Inverse_EN;/*门3数据4反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D3_Inphase_EN;/*门3数据3同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D3_Inverse_EN;/*门3数据3反向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D2_Inphase_EN;/*门3数据2同向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D2_Inverse_EN;/*门3数据2反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D1_Inphase_EN;/*门3数据1同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G3D1_Inverse_EN;/*门3数据1反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D4_Inphase_EN;/*门2数据4同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D4_Inverse_EN;/*门2数据4反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D3_Inphase_EN;/*门2数据3同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D3_Inverse_EN;/*门2数据3反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D2_Inphase_EN;/*门2数据2同向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D2_Inverse_EN;/*门2数据2反向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D1_Inphase_EN;/*门2数据1同向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G2D1_Inverse_EN;/*门2数据1反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D4_Inphase_EN;/*门1数据4同向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D4_Inverse_EN;/*门1数据4反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D3_Inphase_EN;/*门1数据3同向允许通道选择
                                 取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D3_Inverse_EN;/*门1数据3反向允许通道选择
                                  取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D2_Inphase_EN;/*门1数据2同向允许通道选择
                                取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D2_Inverse_EN;/*门1数据2反向允许通道选择
                                取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D1_Inphase_EN;/*门1数据1同向允许通道选择
                                取值为宏 取值为TRUE或FALSE */
 FunctionalState m_G1D1_Inverse_EN;/*门1数据1反向允许通道选择
                                取值为宏 取值为TRUE或FALSE */
} CFGL_InitTypeDef;



/**
 * CFGL 逻辑单元使能、禁止选择
 */
#define CFGL_ENABLE                 ((uint32_t) 0x1 << CFGL_CTL0_EN_POS)
#define CFGL_DISABLE                ((uint32_t) 0x0 << CFGL_CTL0_EN_POS)
#define CHECK_CFGL_EN(SEL)          (((SEL) == CFGL_ENABLE) \
                                    || ((SEL) == CFGL_DISABLE))

/**
 * CFGL 输出使能选择
 */
#define CFGL_OUTPUT_ENABLE          ((uint32_t) 0x1 << CFGL_CTL0_OE_POS)
#define CFGL_OUTPUT_DISABLE         ((uint32_t) 0x0 << CFGL_CTL0_OE_POS)
#define CHECK_CFGL_OUTPUT(SEL)      (((SEL) == CFGL_OUTPUT_ENABLE) \
                                    || ((SEL) == CFGL_OUTPUT_DISABLE))

/**
 * CFGL 上升边沿中断允许选择
 */
#define CFGL_RISEINT_DISABLE        ((uint32_t) 0x0 << CFGL_CTL0_INTP_POS)
#define CFGL_RISEINT_ENABLE         ((uint32_t) 0x1 << CFGL_CTL0_INTP_POS)
#define CHECK_CFGL_RISEINT_EN(SEL)  (((SEL) == CFGL_RISEINT_DISABLE) \
                                    || ((SEL) == CFGL_RISEINT_ENABLE))

/**
 * CFGL 下降边沿中断允许选择
 */
#define CFGL_FALLINT_ENABLE         ((uint32_t) 0x1 << CFGL_CTL0_INTN_POS)
#define CFGL_FALLINT_DISABLE        ((uint32_t) 0x0 << CFGL_CTL0_INTN_POS)
#define CHECK_CFGL_FALLINT_EN(SEL)  (((SEL) == CFGL_FALLINT_ENABLE) \
                                    || ((SEL) == CFGL_FALLINT_DISABLE) )

#define CHECK_CFGL_INT_EN(SEL)      (((SEL) == CFGL_RISEINT_DISABLE) \
                                    || ((SEL) == CFGL_RISEINT_ENABLE)\
                                    || ((SEL) == CFGL_FALLINT_ENABLE) \
                                    || ((SEL) == CFGL_FALLINT_DISABLE))
/**
 * CFGL 配置逻辑单元功能模式
 */
#define CFGL_FLIP_LATCH_ONE_INPUT    ((uint32_t) 0x7 << CFGL_CTL0_MD0_POS)
#define CFGL_JK_TRIGGER       		 ((uint32_t) 0x6 << CFGL_CTL0_MD0_POS)
#define CFGL_D_TRIGGER_TWO_INPUT     ((uint32_t) 0x5 << CFGL_CTL0_MD0_POS)
#define CFGL_D_TRIGGER_ONE_INPUT     ((uint32_t) 0x4 << CFGL_CTL0_MD0_POS)
#define CFGL_SR_LATCH        		 ((uint32_t) 0x3 << CFGL_CTL0_MD0_POS)
#define CFGL_AND_FOUR_INPUT       	 ((uint32_t) 0x2 << CFGL_CTL0_MD0_POS)
#define CFGL_OR_XOR         		 ((uint32_t) 0x1 << CFGL_CTL0_MD0_POS)
#define CFGL_AND_OR       	 		 ((uint32_t) 0x0 << CFGL_CTL0_MD0_POS)
#define CHECK_CFGL_MODE(SEL)         (((SEL) == CFGL_FLIP_LATCH_ONE_INPUT) \
								  || ((SEL) == CFGL_JK_TRIGGER) \
								  || ((SEL) == CFGL_D_TRIGGER_TWO_INPUT) \
								  || ((SEL) == CFGL_D_TRIGGER_ONE_INPUT) \
								  || ((SEL) == CFGL_SR_LATCH) \
								  || ((SEL) == CFGL_AND_FOUR_INPUT) \
								  || ((SEL) == CFGL_OR_XOR) \
                                  || ((SEL) == CFGL_AND_OR) )
/**
 * CFGL 输出极性选择
 */
#define CFGL_OUT_INPHASE             ((uint32_t) 0x0 << CFGL_CTL0_POL_POS)
#define CFGL_OUT_INVERSE             ((uint32_t) 0x1 << CFGL_CTL0_POL_POS)
#define CHECK_CFGL_OUT_POL(SEL)      (((SEL) == CFGL_OUT_INPHASE) \
                                     || ((SEL) == CFGL_OUT_INVERSE) )
/**
 * CFGL 门4 输出极性选择
 */
#define CFGL_G4POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G4POL_POS)
#define CFGL_G4POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G4POL_POS)
#define CHECK_G4POL_SEL(SEL)     	 (((SEL) == CFGL_G4POL_INPHASE) \
                                     || ((SEL) == CFGL_G4POL_INVERSE) )
/**
 * CFGL 门3 输出极性选择
 */
#define CFGL_G3POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G3POL_POS)
#define CFGL_G3POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G3POL_POS)
#define CHECK_G3POL_SEL(SEL)     	 (((SEL) == CFGL_G3POL_INPHASE) \
                                     || ((SEL) == CFGL_G3POL_INVERSE) )
/**
 * CFGL 门2 输出极性选择
 */
#define CFGL_G2POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G2POL_POS)
#define CFGL_G2POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G2POL_POS)
#define CHECK_G2POL_SEL(SEL)     	 (((SEL) == CFGL_G2POL_INPHASE) \
                                     || ((SEL) == CFGL_G2POL_INVERSE) )
/**
 * CFGL 门1 输出极性选择
 */
#define CFGL_G1POL_INPHASE           ((uint32_t) 0x0 << CFGL_CTL0_G1POL_POS)
#define CFGL_G1POL_INVERSE           ((uint32_t) 0x1 << CFGL_CTL0_G1POL_POS)
#define CHECK_G1POL_SEL(SEL)     	 (((SEL) == CFGL_G1POL_INPHASE) \
                                     || ((SEL) == CFGL_G1POL_INVERSE) )

/**
 * CFGL 门4 输入数据通道选择
 */
#define CFGL_G4INPUT_CFGL1CH3        ((uint32_t) 0x7 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH2        ((uint32_t) 0x6 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH1        ((uint32_t) 0x5 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH0        ((uint32_t) 0x4 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH15       ((uint32_t) 0x3 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH14       ((uint32_t) 0x2 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH13       ((uint32_t) 0x1 << CFGL_CTL0_D4S0_POS)
#define CFGL_G4INPUT_CFGL1CH12       ((uint32_t) 0x0 << CFGL_CTL0_D4S0_POS)
#define CHECK_G4INPUT_SEL(SEL)       (((SEL) == CFGL_G4INPUT_CFGL1CH3) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH2) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH1) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH0) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH15) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH14) \
									 || ((SEL) == CFGL_G4INPUT_CFGL1CH13) \
                                     || ((SEL) == CFGL_G4INPUT_CFGL1CH12) )
/**
 * CFGL 门3 输入数据通道选择
 */
#define CFGL_G3INPUT_CFGL1CH15        ((uint32_t) 0x7 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH14        ((uint32_t) 0x6 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH13        ((uint32_t) 0x5 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH12        ((uint32_t) 0x4 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH11        ((uint32_t) 0x3 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH10        ((uint32_t) 0x2 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH9         ((uint32_t) 0x1 << CFGL_CTL0_D3S0_POS)
#define CFGL_G3INPUT_CFGL1CH8         ((uint32_t) 0x0 << CFGL_CTL0_D3S0_POS)
#define CHECK_G3INPUT_SEL(SEL)        (((SEL) == CFGL_G3INPUT_CFGL1CH15) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH14) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH13) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH12) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH11) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH10) \
									  || ((SEL) == CFGL_G3INPUT_CFGL1CH9) \
                                      || ((SEL) == CFGL_G3INPUT_CFGL1CH8) )
/**
 * CFGL 门2 输入数据通道选择
 */
#define CFGL_G2INPUT_CFGL1CH11        ((uint32_t) 0x7 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH10        ((uint32_t) 0x6 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH9         ((uint32_t) 0x5 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH8         ((uint32_t) 0x4 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH7         ((uint32_t) 0x3 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH6         ((uint32_t) 0x2 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH5         ((uint32_t) 0x1 << CFGL_CTL0_D2S0_POS)
#define CFGL_G2INPUT_CFGL1CH4         ((uint32_t) 0x0 << CFGL_CTL0_D2S0_POS)
#define CHECK_G2INPUT_SEL(SEL)        (((SEL) == CFGL_G2INPUT_CFGL1CH11) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH10) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH9) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH8) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH7) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH6) \
									  || ((SEL) == CFGL_G2INPUT_CFGL1CH5) \
                                      || ((SEL) == CFGL_G2INPUT_CFGL1CH4) )
/**
 * CFGL 门1 输入数据通道选择
 */
#define CFGL_G1INPUT_CFGL1CH7         ((uint32_t) 0x7 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH6         ((uint32_t) 0x6 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH5         ((uint32_t) 0x5 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH4         ((uint32_t) 0x4 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH3         ((uint32_t) 0x3 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH2         ((uint32_t) 0x2 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH1         ((uint32_t) 0x1 << CFGL_CTL0_D1S0_POS)
#define CFGL_G1INPUT_CFGL1CH0         ((uint32_t) 0x0 << CFGL_CTL0_D1S0_POS)
#define CHECK_G1INPUT_SEL(SEL)        (((SEL) == CFGL_G1INPUT_CFGL1CH7) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH6) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH5) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH4) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH3) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH2) \
									  || ((SEL) == CFGL_G1INPUT_CFGL1CH1) \
                                      || ((SEL) == CFGL_G1INPUT_CFGL1CH0) )
/**
 * CFGL CH4通道输入数据选择
 */
#define CFGL_CH4_INPUT_0              ((uint32_t) 0x0 << CFGL_CTL0_REGIN_POS)
#define CFGL_CH4_INPUT_1              ((uint32_t) 0x1 << CFGL_CTL0_REGIN_POS)
#define CHECK_CH4_INPUT_SEL(SEL)      (((SEL) == CFGL_CH4_INPUT_0) \
                                      || ((SEL) == CFGL_CH4_INPUT_1) )

/**
 * CFGL CH5和CFGL1CH6通道选择
 */
#define CFGL_CH5_T21TRGO_CH6_CCP21CH1OUT    ((uint32_t) 0x7 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T20TRGO_CH6_CCP20CH1OUT    ((uint32_t) 0x6 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T5TRGO_CH6_CCP5CH1OUT      ((uint32_t) 0x5 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T4TRGO_CH6_CCP4CH1OUT      ((uint32_t) 0x4 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T3TRGO_CH6_CCP3CH1OUT      ((uint32_t) 0x3 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T2TRGO_CH6_CCP2CH1OUT      ((uint32_t) 0x2 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T1TRGO_CH6_CCP1CH1OUT      ((uint32_t) 0x1 << CFGL_CTL0_TPSEL0_POS)
#define CFGL_CH5_T0TRGO_CH6_CCP0CH1OUT      ((uint32_t) 0x0 << CFGL_CTL0_TPSEL0_POS)
#define CHECK_CH5CH6_INPUT_SEL(SEL)         (((SEL) == CFGL_CH5_T21TRGO_CH6_CCP21CH1OUT) \
										    || ((SEL) == CFGL_CH5_T20TRGO_CH6_CCP20CH1OUT) \
										    || ((SEL) == CFGL_CH5_T5TRGO_CH6_CCP5CH1OUT) \
										    || ((SEL) == CFGL_CH5_T4TRGO_CH6_CCP4CH1OUT) \
										    || ((SEL) == CFGL_CH5_T3TRGO_CH6_CCP3CH1OUT) \
										    || ((SEL) == CFGL_CH5_T2TRGO_CH6_CCP2CH1OUT) \
										    || ((SEL) == CFGL_CH5_T1TRGO_CH6_CCP1CH1OUT) \
                                    	    || ((SEL) == CFGL_CH5_T0TRGO_CH6_CCP0CH1OUT) )

/**
 * CFGL CFGL2输出同步使能选择
 */
#define CFGL2_OUT_SYNCHRO_ENABLE            ((uint32_t) 0x1 << CFGL_IFR_CLC2SYNCEN_POS)
#define CFGL2_OUT_SYNCHRO_DISABLE           ((uint32_t) 0x0 << CFGL_IFR_CLC2SYNCEN_POS)
#define CHECK_CFGL2_OUT_SYNCHRO(SEL)     	(((SEL) == CFGL2_OUT_SYNCHRO_ENABLE) \
                                    		|| ((SEL) == CFGL2_OUT_SYNCHRO_DISABLE) )
/**
 * CFGL CFGL1输出同步使能选择
 */
#define CFGL1_OUT_SYNCHRO_ENABLE            ((uint32_t) 0x1 << CFGL_IFR_CLC1SYNCEN_POS)
#define CFGL1_OUT_SYNCHRO_DISABLE           ((uint32_t) 0x0 << CFGL_IFR_CLC1SYNCEN_POS)
#define CHECK_CFGL1_OUT_SYNCHRO(SEL)     	(((SEL) == CFGL1_OUT_SYNCHRO_ENABLE) \
                                    		|| ((SEL) == CFGL1_OUT_SYNCHRO_DISABLE) )

#ifdef KF32A_Periph_cfgl
FlagStatus CFGL1_OUT_STATE ();
FlagStatus CFGL2_OUT_STATE ();
FlagStatus CFGL2_Get_INT_Flag ();
FlagStatus CFGL1_Get_INT_Flag ();
void CFGL1_FALLINT_Enable (FunctionalState NewState);
void CFGL1_RISEINT_Enable (FunctionalState NewState);
void CFGL2_FALLINT_Enable (FunctionalState NewState);
void CFGL2_RISEINT_Enable (FunctionalState NewState);
void CFGL1_OUT_SYNCHRO_Enable (FunctionalState NewState);
void CFGL2_OUT_SYNCHRO_Enable (FunctionalState NewState);
void CFGL_Configuration (CFGL_SFRmap* CFGLx, CFGL_InitTypeDef* CFGLInitStruct);
void CFGL_Reset ();
void CFGL1_Clear_RISEINT_Flag (void);
void CFGL1_Clear_FALLINT_Flag (void);
void CFGL1_Clear_RISEFALLINT_Flag (void);
void CFGL2_Clear_RISEINT_Flag (void);
void CFGL2_Clear_FALLINT_Flag (void);
void CFGL2_Clear_RISEFALLINT_Flag (void);
void CFGL_Struct_Init (CFGL_InitTypeDef* CFGLInitStruct);
#endif //KF32A_Periph_cfgl
#endif /* _KF32A_BASIC_CFGL_H_ */
