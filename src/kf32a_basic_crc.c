/**
  ******************************************************************************
  * 文件名  kf32a_basic_crc.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了多项式校验模块(CRC)相关的功能函数，包含：
  *            + 多项式校验模块(CRC)初始化函数
  *			   + 多项式校验模块(CRC)数据配置
  *			   + 多项式校验模块(CRC)计算结果获取
  *
  *********************************************************************
  */

#include "kf32a_basic_crc.h"
#include "kf32a_basic_rst.h"
#include "kf32a_basic_pclk.h"

#ifdef  KF32A_Periph_crc
/* CRC私有定义 ----------------------------------------------------*/
/* CRC_CTL寄存器配置掩码 */
#define CRC_CTL_INIT_MASK               (CRC_CTL_RSLTREV \
                                       | CRC_CTL_DATAREV \
                                       | CRC_CTL_DATASIZE \
                                       | CRC_CTL_RSET)

/**
  *   ##### 初始化及配置函数 #####
  */
/**
  * 描述 CRC外设复位，使能外设时钟。
  * 输入  : 无
  * 返回  无。
  */
void
CRC_Reset ()
{
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CRCRST, TRUE);
	RST_CTL3_Peripheral_Reset_Enable(RST_CTL3_CRCRST, FALSE);
	PCLK_CTL3_Peripheral_Clock_Enable(PCLK_CTL3_CRCCLKEN, TRUE);
}

/**
  * 描述  CRC外设配置。
  * 输入  CRCx: CRCInitStruct: CRC配置信息
  * 返回  无。
  */
void
CRC_Configuration (CRC_InitTypeDef* CRCInitStruct)
{
    uint32_t tmpreg = 0;

	CHECK_RESTRICTION(CHECK_CRC_CAL_RSET(CRCInitStruct->m_CalUnitReset));
	CHECK_RESTRICTION(CHECK_CRC_INPUT_SEZE(CRCInitStruct->m_InputSize));
	CHECK_RESTRICTION(CHECK_CRC_INPUT_REVERSE(CRCInitStruct->m_InputReverse));
	CHECK_RESTRICTION(CHECK_CRC_RSLTREV_SEL(CRCInitStruct->m_ResultReverse));

	/*---------------------------- CRC_CTL寄存器配置  -----------------*/
	/* 根据结构体成员m_CalUnitReset，设置RSET位域 */
	/* 根据结构体成员m_InputSize，设置DATASIZE位域 */
	/* 根据结构体成员m_InputReverse，设置DATAREV位域 */
	/* 根据结构体成员m_ResultReverse，设置RSLTREV位域 */

   tmpreg = ((CRCInitStruct->m_CalUnitReset ) | \
			  (CRCInitStruct->m_InputSize) | \
			  (CRCInitStruct->m_InputReverse) | \
			  (CRCInitStruct->m_ResultReverse));
   CRC_SFR->CTL = SFR_Config (CRC_SFR->CTL,
			   ~CRC_CTL_INIT_MASK,
			   tmpreg);
}

/**
  * 描述  初始化CRC配置信息结构体。
  * 输入  CRCInitStruct: 指向待初始化的结构体指针。
  * 返回  无。
  */
void
CRC_Struct_Init (CRC_InitTypeDef* CRCInitStruct)
{
    /* 初始化CRC计算单元复位控制位 */
    CRCInitStruct->m_CalUnitReset = CRC_CAL_RSET_DISABLE;
    /* 初始化 CRC输入数据格式控制位 */
    CRCInitStruct->m_InputSize = CRC_INPUT_SEZE_8;
    /* 初始化 CRC输入数据位反序控制位 */
    CRCInitStruct->m_InputReverse = CRC_INPUT_REVERSE_DISABLE;
    /* 初始化 CRC结果反序控制位 */
    CRCInitStruct->m_ResultReverse = CRC_RESULT_REVERSE_DISABLE;
}

/**
  * 描述  CRC数据寄存器配置
  * 输入  DATA: 数据值。
  * 返回  无。
  */
void
CRC_INPUT_DATA (uint32_t DATA)
{
    /* 配置CRC数据寄存器 */
	CRC_SFR->DATA = DATA;
}
/**
  * 描述  读取CRC结果
  * 输入  : 无
  * 返回  uint32_t位宽的值
  */
uint32_t
CRC_GET_RESULT ()
{
    /* 读取CRC结果寄存器 */
	uint32_t DATA;
	DATA=CRC_SFR->RSLT;
	return DATA;
}
/**
  * 描述  设置CRC计算的初始值
  * 输入  : 无
  * 返回  uint32_t位宽的值
  */
void
CRC_SET_INITVALUE (uint32_t DATA)
{
    /* 设置CRC计算的初始值寄存器 */
	CRC_SFR->INIT = DATA;
}
/**
  * 描述  设置CRC多项式
  * 输入  : 无
  * 返回  uint32_t位宽的值
  */
void
CRC_SET_PLN (uint32_t DATA)
{
    /* 设置CRC多项式寄存器 */
	CRC_SFR->PLN = DATA;
}
/**
  * 描述  设置CRC结果异或值
  * 输入  : 无
  * 返回  uint32_t位宽的值
  */
void
CRC_SET_RXOR (uint32_t DATA)
{
    /* 设置结果异或值寄存器 */
	CRC_SFR->RXOR = DATA;
}
/**
  * 描述  设置CRC独立数据
  * 输入  : 无
  * 返回  uint32_t位宽的值
  */
void
CRC_SET_IDATA (uint32_t DATA)
{
    /* 设置独立数据寄存器 */
	CRC_SFR->IDATA = DATA;
}
/**
  * 描述  设置CRC缓存
  * 输入  : 无
  * 返回  uint32_t位宽的值
  */
uint32_t
CRC_GET_TEMP ()
{
    /* 设置缓存寄存器 */
	uint32_t DATA;
	DATA=CRC_SFR->TEMP;
	return DATA;
}
/**
  * 描述  CRC 计算单元复位
  * 输入  : 无
  * 返回
  */
void
CRC_SET_RSET ()
{
    /* 设置缓存寄存器 */
	CRC_SFR->CTL|=CRC_CTL_RSET;
}

#endif  //KF32A_Periph_crc
