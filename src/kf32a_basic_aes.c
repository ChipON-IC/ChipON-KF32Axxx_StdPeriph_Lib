/**
  ******************************************************************************
  * 文件名  kf32a_basic_aes.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.2
  * 日  期  2019-11-16
  * 描  述  该文件提供了加密模块(AE)外设功能函数，包含：
  *          + AES外设配置
  *
  *********************************************************************
  */


#include "kf32a_basic_aes.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_aes

/* AES私有定义 ----------------------------------------------------*/
/* AES_CTL寄存器配置掩码 */
#define AES_CTL_INIT_MASK               (AES_CTL_OFBEN \
                                       | AES_CTL_AESCS \
                                       | AES_CTL_AESDIV \
                                       | AES_CTL_AESRUN \
                                       | AES_CTL_AESFIE \
                                       | AES_CTL_AESFIC)
/**
  *   ##### 初始化及配置函数 #####
  */
/**
  * 描述AES外设复位，使能外设时钟。
  * 输入  : 无
  * 返回  无。
  */
void
AES_Reset ()
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_AESRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_AESRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_AESCLKEN, TRUE);
}

/**
  * 描述 AES外设配置。
  * 输入  AES: AESInitStruct: AES配置信息
  * 返回  无。
  */
void
AES_Configuration (AES_InitTypeDef* AESInitStruct)
{
    uint32_t tmpreg = 0;

	CHECK_RESTRICTION(CHECK_AES_OFB(AESInitStruct->m_OFB));
	CHECK_RESTRICTION(CHECK_AES_CLKSOURCE(AESInitStruct->m_ClockSelect));
	CHECK_RESTRICTION(CHECK_AES_CLK_DIV(AESInitStruct->m_ClockDiv));
	CHECK_RESTRICTION(CHECK_AES_RUN(AESInitStruct->m_Run));
	CHECK_RESTRICTION(CHECK_AES_FINISH_INT(AESInitStruct->m_FinishIntEn));
	CHECK_RESTRICTION(CHECK_AES_FINISH_INT_CLEAR(AESInitStruct->m_FinishIntClr));

	/*---------------------------- AES_CTL寄存器配置  -----------------*/
	/* 根据结构体成员m_OFB，设置OFBEN位域 */
	/* 根据结构体成员m_ClockSelect，设置AESCS位域 */
	/* 根据结构体成员m_ClockDiv，设置AESDIV位域 */
	/* 根据结构体成员m_Run，设置AESRUN位域 */
	/* 根据结构体成员m_FinishIntEn，设置AESFIE位域 */
	/* 根据结构体成员m_FinishIntClr，设置AESFIC位域 */
   tmpreg = ((AESInitStruct->m_OFB ) | \
			  (AESInitStruct->m_ClockSelect) | \
			  (AESInitStruct->m_ClockDiv) | \
			  (AESInitStruct->m_Run) | \
			  (AESInitStruct->m_FinishIntEn) | \
			  (AESInitStruct->m_FinishIntClr));

   AES_SFR->CTL = SFR_Config (AES_SFR->CTL,
			   ~AES_CTL_INIT_MASK,
			   tmpreg);

}
/**
  * 描述  初始化AES配置信息结构体。
  * 输入  AESInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
AES_Struct_Init (AES_InitTypeDef* AESInitStruct)
{
    /* 初始化AES OFB模式输入控制位 */
    AESInitStruct->m_OFB = AES_OFB_DISABLE;
    /* 初始化 AES工作时钟源选择位 */
    AESInitStruct->m_ClockSelect = AES_CLK_SEL_SCLK;
    /* 初始化 AES时钟分频控制位 */
    AESInitStruct->m_ClockDiv = AES_CLK_DIV_1;
    /* 初始化 AES运行使能位 */
    AESInitStruct->m_Run = AES_RUN_DISABLE;
    /* 初始化 AES加密完成中断使能位 */
    AESInitStruct->m_FinishIntEn = AES_FINISH_INT_DISABLE;
    /* 初始化 AES加密完成清零位 */
    AESInitStruct->m_FinishIntClr = AES_FINISH_INT_CLEAR_DISABLE;

}
/**
  * 描述 AES密钥寄存器。
  * 输入  KEYDATA:位宽bit32
  * 返回  无。
  */
void
AES_KEY0_DATA (uint32_t DATA)
{

	AES_SFR->KEY0=DATA;
}
void
AES_KEY1_DATA (uint32_t DATA)
{

	AES_SFR->KEY1=DATA;
}
void
AES_KEY2_DATA (uint32_t DATA)
{

	AES_SFR->KEY2=DATA;
}
void
AES_KEY3_DATA (uint32_t DATA)
{

	AES_SFR->KEY3=DATA;
}
/**
  * 描述 AES输入寄存器。
  * 输入  INPUTDATA:位宽bit32
  * 返回  无。
  */
void
AES_INPUT0_DATA (uint32_t DATA)
{

	AES_SFR->INPUT0=DATA;
}
void
AES_INPUT1_DATA (uint32_t DATA)
{

	AES_SFR->INPUT1=DATA;
}
void
AES_INPUT2_DATA (uint32_t DATA)
{

	AES_SFR->INPUT2=DATA;
}
void
AES_INPUT3_DATA (uint32_t DATA)
{

	AES_SFR->INPUT3=DATA;
}
/**
  * 描述 AES输入寄存器。
  * 输入 :无
  * 输出： OUTPUTDATA:位宽bit32
  * 返回  无。
  */
uint32_t
GET_AES_OUTPUT0_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT0;

	return	DATA;
}
uint32_t
GET_AES_OUTPUT1_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT1;

	return	DATA;
}
uint32_t
GET_AES_OUTPUT2_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT2;

	return	DATA;
}
uint32_t
GET_AES_OUTPUT3_DATA (void)
{
	uint32_t DATA;
	DATA=AES_SFR->OUTPUT3;

	return	DATA;
}

FlagStatus
AES_Get_Flag_Status (void)
{
    if ((AES_SFR->CTL & AES_CTL_AESFIF) != RESET)
    {
        /* AES 加密完成 */
        return SET;
    }
    else
    {
        /* AES 加密未完成 */
        return RESET;
    }
}

/**
  * 描述 AES开始加密
  * 输入 :无
  * 输出 :无
  * 返回 :无
  */
void
AES_Start ()
{
	/* AES 开始加密 */
	 AES_SFR->CTL |= AES_CTL_AESRUN;
}

/**
  * 描述 AES清标志位
  * 输入 :无
  * 输出 :无
  * 返回 :无
  */
void
AES_CLR_Flag_Status ()
{
//	/* AES清标志位 */
	 AES_SFR->CTL |= 0x00800000;
	 while(AES_Get_Flag_Status());//判断标志为0
     AES_SFR->CTL &= 0xFF7FFFFF;
}

#endif //KF32A_Periph_aes
