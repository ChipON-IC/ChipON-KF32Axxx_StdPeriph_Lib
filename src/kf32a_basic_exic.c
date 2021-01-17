/**
  ******************************************************************************
  * 文件名  kf32a_basic_exic.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.5
  * 日  期  2019-11-16
  * 描  述  该文件提供了模数转换模块(ADC)外设功能函数，包含：
  * 描  述  该文件提供了LDC8080模块(EXIC)相关的功能函数，包含：
  *          + EXIC初始化函数
  *          + EXIC收发函数
  *          + EXIC中断管理函数
  *
  *********************************************************************
  */
#include "kf32a_basic_exic.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef KF32A_Periph_exic
/**
  *   ##### EXIC接口功能初始化函数 #####
  */
/* EXIC_CTL0寄存器初始化配置掩码 */
#define EXIC_CTL0_INIT_MASK		 		(EXIC_CTL0_EXICEN \
                                       | EXIC_CTL0_CSEN \
                                       | EXIC_CTL0_DCSEL \
                                       | EXIC_CTL0_WRITEEN \
                                       | EXIC_CTL0_READEN \
                                       | EXIC_CTL0_TDMAEN \
                                       | EXIC_CTL0_RDMAEN \
                                       | EXIC_CTL0_WENDIN \
                                       | EXIC_CTL0_RENDIE \
                                       | EXIC_CTL0_WRITE1EN \
                                       | EXIC_CTL0_READ1EN \
                                       | EXIC_CTL0_DATATR \
                                       | EXIC_CTL0_MODESEL)
/* EXIC_CTL1寄存器初始化配置掩码 */
#define EXIC_CTL1_INIT_MASK		 		(EXIC_CTL1_EXICCS \
                                       | EXIC_CTL1_PULSEWIDTH)
/**
  * 描述  复位EXIC模块。
  * 输入  无。
  * 返回  无。
  */
void
EXIC_Reset (void)
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_EXICRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_EXICRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_EXICCLKEN, TRUE);
}
/**
  * 描述  EXIC外设配置。
  * 输入  EXICx: EXICInitStruct: EXIC配置信息
  * 返回  无。
  */
void
EXIC_Configuration (EXIC_InitTypeDef* EXICInitStruct)
{
    uint32_t tmpreg = 0;

	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_Module_Enable));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_CS_Enable));
	CHECK_RESTRICTION(CHECK_EXIC_TRANSMIT(EXICInitStruct->m_Data_Control_Sel));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_Write_Enable));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_Read_Enable));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_DMA_Write_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_DMA_Read_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_WriteFnish_INT_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_ReadFnish_INT_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_WriteData_EN));
	CHECK_RESTRICTION(CHECK_FUNCTIONAL_STATE(EXICInitStruct->m_ReadData_EN));
	CHECK_RESTRICTION(CHECK_DATA_DIRECTION(EXICInitStruct->m_Data_Direction_Sel));
	CHECK_RESTRICTION(CHECK_MODE_SEL(EXICInitStruct->m_Mode_Sel));
	CHECK_RESTRICTION(CHECK_EXIC_CLKSOURCE_SEL(EXICInitStruct->m_Source_Clock_Sel));
	CHECK_RESTRICTION(CHECK_PULSEWIDTH_SEL(EXICInitStruct->m_Pulse_Width_Sel));

	/*---------------------------- EXIC_CTL寄存器配置  -----------------*/
	/* 根据结构体成员m_Module_Enable，设置EXICEN位域 */
	/* 根据结构体成员m_CS_Enable，设置CSEN位域 */
	/* 根据结构体成员m_Data_Control_Sel，设置D/CSEL位域 */
	/* 根据结构体成员m_Write_Enable，设置WRITEEN位域 */
	/* 根据结构体成员m_Read_Enable，设置READEN位域 */
	/* 根据结构体成员m_DMA_Write_EN，设置TDMAEN位域 */
	/* 根据结构体成员m_DMA_Read_EN，设置RDMAEN位域 */
	/* 根据结构体成员m_WriteFnish_INT_EN，设置WENDIE位域 */
	/* 根据结构体成员m_ReadFnish_INT_EN，设置RENDIE位域 */
	/* 根据结构体成员m_WriteData_EN，设置WRITE1EN位域 */
	/* 根据结构体成员m_ReadData_EN，设置READ1EN位域 */
	/* 根据结构体成员m_Data_Direction_Sel，设置DATATR位域 */
	/* 根据结构体成员m_Mode_Sel，设置MODESEL位域 */


   tmpreg = ((EXICInitStruct->m_Module_Enable << EXIC_CTL0_EXICEN_POS) | \
		     (EXICInitStruct->m_CS_Enable << EXIC_CTL0_CSEN_POS) | \
		     (EXICInitStruct->m_Data_Control_Sel ) | \
		     (EXICInitStruct->m_Write_Enable << EXIC_CTL0_WRITEEN_POS) | \
		     (EXICInitStruct->m_Read_Enable << EXIC_CTL0_READEN_POS) | \
		     (EXICInitStruct->m_DMA_Write_EN << EXIC_CTL0_TDMAEN_POS) | \
		     (EXICInitStruct->m_DMA_Read_EN << EXIC_CTL0_RDMAEN_POS) | \
		     (EXICInitStruct->m_WriteFnish_INT_EN << EXIC_CTL0_WENDIE_POS) | \
		     (EXICInitStruct->m_ReadFnish_INT_EN << EXIC_CTL0_RENDIE_POS) | \
		     (EXICInitStruct->m_WriteData_EN << EXIC_CTL0_WRITE1EN_POS) | \
		     (EXICInitStruct->m_ReadData_EN << EXIC_CTL0_READ1EN_POS) | \
		     (EXICInitStruct->m_Data_Direction_Sel ) | \
		     (EXICInitStruct->m_Mode_Sel) | \
		     (EXICInitStruct->m_Source_Clock_Sel) | \
		     (EXICInitStruct->m_Pulse_Width_Sel));
   EXIC_SFR->CTL0 = SFR_Config (EXIC_SFR->CTL0,
			   ~EXIC_CTL0_INIT_MASK,
			   tmpreg);
	/*---------------------------- EXIC_CTL1寄存器配置  -----------------*/
	/* 根据结构体成员m_Source_Clock_Sel，设置EXICCS位域 */
	/* 根据结构体成员m_Pulse_Width_Sel，设置PULSEWIDTH位域 */
   tmpreg = ((EXICInitStruct->m_Source_Clock_Sel) | \
		     (EXICInitStruct->m_Pulse_Width_Sel));
   EXIC_SFR->CTL1 = SFR_Config (EXIC_SFR->CTL1,
			   ~EXIC_CTL1_INIT_MASK,
			   tmpreg);
}
/**
  * 描述  初始化EXIC配置信息结构体。
  * 输入  EXICInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
EXIC_Struct_Init (EXIC_InitTypeDef* EXICInitStruct)
{
    /* 初始化EXIC接口使能选择 */
    EXICInitStruct->m_Module_Enable = FALSE;
    /* 初始化EXIC片选使能选择 */
    EXICInitStruct->m_CS_Enable = FALSE;
    /* 初始化 EXIC数据/命令选择 */
    EXICInitStruct->m_Data_Control_Sel = EXIC_TRANSMIT_DATA;
    /* 初始化EXIC写数据使能 */
    EXICInitStruct->m_Write_Enable = FALSE;
    /* 初始化 EXIC读数据使能 */
    EXICInitStruct->m_Read_Enable = FALSE;
    /* 初始化EXIC DMA写使能*/
    EXICInitStruct->m_DMA_Write_EN = FALSE;
    /* 初始化EXIC DMA读使能 */
    EXICInitStruct->m_DMA_Read_EN = FALSE;
    /* 初始化 EXIC高优先级通道外部触发转换模式使能 */
    EXICInitStruct->m_WriteFnish_INT_EN = FALSE;
    /* 初始化 读完成中断使能 */
    EXICInitStruct->m_ReadFnish_INT_EN = FALSE;
    /* 初始化 EXIC写数据使能 */
    EXICInitStruct->m_WriteData_EN = FALSE;
    /* 初始化 EXIC读数据使能 */
    EXICInitStruct->m_ReadData_EN = FALSE;
    /* 初始化EXIC数据方向选择 */
    EXICInitStruct->m_Data_Direction_Sel = EXIC_DATA_INPUT;
    /* 初始化EXIC模式选择 */
	EXICInitStruct->m_Mode_Sel = EXIC_MODE_SEL_W1R1;
	/* 初始化EXIC工作时钟源选择 */
	EXICInitStruct->m_Source_Clock_Sel = EXIC_CLKSOURCE_SCLK;
	/* 初始化 EXIC读写脉宽选择 */
	EXICInitStruct->m_Pulse_Width_Sel = EXIC_PULSEWIDTH_16Tclk;
}
/**
  * 描述  读取EXIC写完成标志。
  * 输入  :无
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
EXIC_Get_Write_Finish_INT_Flag (void)
{
    /*-------------------- 读取EXICSTATE寄存器WENFIF位 --------------------*/
	  if (EXIC_STATE & EXIC_STATE_WENDIF)
    {
        /*  EXIC写完成标志 */
        return SET;
    }
    else
    {
        /*  EXIC未发生完成标志 */
        return RESET;
    }
}
/**
  * 描述  清零EXIC写完成标志。
  * 输入  :无
  * 返回  ：无
  */
void EXIC_Clear_Write_Finish_INT_Flag (void)
{
    /*-------------------- 清零EXICSTATE寄存器WENFIF位 --------------------*/
	while((EXIC_STATE & EXIC_STATE_WENDIF)>>EXIC_STATE_WENDIF_POS)
	{
		EXIC_SFR->STATE &= ~EXIC_STATE_WENDIF;
	}
}
/**
  * 描述  读取EXIC读完成标志。
  * 输入  :无
  * 返回  中断状态，0：未发生中断，1：发生中断
  */
FlagStatus
EXIC_Get_Read_Finish_INT_Flag (void)
{
    /*-------------------- 读取EXICSTATE寄存器RENFIF位 --------------------*/
	if (EXIC_STATE & EXIC_STATE_RENDIF)
    {
        /*  EXIC读完成标志 */
        return SET;
    }
    else
    {
        /*  EXIC未发生读完成标志 */
        return RESET;
    }
}
/**
  * 描述  清零EXIC读完成标志。
  * 输入  :无
  * 返回  ：无
  */
void EXIC_Clear_Read_Finish_INT_Flag (void)
{
    /*-------------------- 清零EXICSTATE寄存器RENFIF位 --------------------*/
	while((EXIC_STATE & EXIC_STATE_RENDIF)>>EXIC_STATE_RENDIF_POS)
	{
		EXIC_SFR->STATE &= ~EXIC_STATE_RENDIF;
	}
}

/**
  * 描述  EXIC写数据缓存。
  * 输入  :DATA位宽是18,大小范围为(0x00-0x3ffff)
  * 返回  :无
  */
void
EXIC_Write_Buff (uint32_t DATA)
{
   EXIC_SFR->WRITEBUF = DATA;
}
/**
  * 描述  EXIC读数据缓存。
  * 输入  :无
  * 返回  :DATA位宽是18,大小范围为(0x00-0x3ffff)
  */
uint32_t
EXIC_Read_Buff (void)
{

   return(EXIC_SFR->READBUF);

}

#endif  // KF32A_Periph_exic
