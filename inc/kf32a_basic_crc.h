/**
  ******************************************************************************
  * 文件名  kf32a_basic_crc.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了循环冗余校验单元相关功能函数声明及相关宏定义。
  *********************************************************************
  */

#ifndef _KF32A_BASIC_CRC_H_
#define _KF32AA_BASIC_CRC_H_

#include "KF32A_BASIC.h"


/**
  * 循环冗余校验单元(CRC)指针定义
  */
#define CHECK_CRC_PERIPH(PERIPH)     ((PERIPH) == CRC_SFR)

/**
 * CRC 配置信息结构体
 */
typedef struct
{
 uint32_t m_CalUnitReset;       /*  CRC 计算单元复位控制
                                    取宏为“CRC计算单元复位控制”中的一个 */
 uint32_t m_InputSize;          /*  CRC 输入数据格式控制位
                                    取宏为“CRC输入数据格式控制位”中的一个 */
 uint32_t m_InputReverse;       /*  CRC 输入数据反序控制位
                                    取宏为“CRC输入数据反序控制位”中的一个 */
 uint32_t m_ResultReverse;      /*  CRC 结果反序控制
                                    取宏为“CRC结果反序控制”中的一个 */
 uint32_t m_Data;               /*  CRC 输入数据值，取值32位数据，数据和需要右对齐 */
 uint32_t m_Result;             /*  CRC 结果值，取值32q位数据，该值为只读 */
 uint32_t m_InitialData;        /*  CRC 计算初始值，取值32位数据 */
 uint32_t m_Polynomial;         /*  CRC 多项式值，取值32位，该值左对齐 */
 uint32_t m_ResultXOR;          /*  CRC 结果异或j值，取值32位，该值左对齐 */
 uint32_t m_IndepentData;       /*  CRC 独立数据寄存器值，取值32位*/
 uint32_t m_Temp;               /*  CRC 缓存值，取值32位，该值为只读 */
} CRC_InitTypeDef;

/**
 * CRC计算单元复位控制
 */
#define CRC_CAL_RSET_ENABLE             ((uint32_t) 0x1 << CRC_CTL_RSET_POS)
#define CRC_CAL_RSET_DISABLE            ((uint32_t) 0x0 << CRC_CTL_RSET_POS)
#define CHECK_CRC_CAL_RSET(SEL)         (((SEL) == CRC_CAL_RSET_ENABLE) \
                                        || ((SEL) == CRC_CAL_RSET_DISABLE) )

/**
 * CRC输入数据格式控制位
 */
#define CRC_INPUT_SEZE_8                ((uint32_t) 0X0 << CRC_CTL_DATASIZE0_POS)
#define CRC_INPUT_SEZE_16               ((uint32_t) 0X1 << CRC_CTL_DATASIZE0_POS)
#define CRC_INPUT_SEZE_24               ((uint32_t) 0X2 << CRC_CTL_DATASIZE0_POS)
#define CRC_INPUT_SEZE_32               ((uint32_t) 0X3 << CRC_CTL_DATASIZE0_POS)
#define CHECK_CRC_INPUT_SEZE(SIZE)      (((SIZE) == CRC_INPUT_SEZE_8) \
                                        || ((SIZE) == CRC_INPUT_SEZE_16) \
                                        || ((SIZE) == CRC_INPUT_SEZE_24) \
                                        || ((SIZE) == CRC_INPUT_SEZE_32) )

/**
 * CRC输入数据反序控制位
 */
#define CRC_INPUT_REVERSE_ENABLE        ( (uint32_t) 0x1 << CRC_CTL_DATAREV_POS )
#define CRC_INPUT_REVERSE_DISABLE       ( (uint32_t) 0x0 << CRC_CTL_DATAREV_POS )
#define CHECK_CRC_INPUT_REVERSE(SEL)    (((SEL) == CRC_INPUT_REVERSE_ENABLE) \
                                        || ((SEL) == CRC_INPUT_REVERSE_DISABLE) )

/**
 * CRC结果反序控制
 */
#define CRC_RESULT_REVERSE_ENABLE       ((uint32_t) 0x1 << CRC_CTL_RSLTREV_POS)
#define CRC_RESULT_REVERSE_DISABLE      ((uint32_t) 0x0 << CRC_CTL_RSLTREV_POS)
#define CHECK_CRC_RSLTREV_SEL(SEL)      (((SEL) == CRC_RESULT_REVERSE_ENABLE) \
                                        || ((SEL) == CRC_RESULT_REVERSE_DISABLE))

#ifdef  KF32A_Periph_crc
void CRC_Reset ();
void CRC_Configuration (CRC_InitTypeDef* CRCInitStruct);
void CRC_Struct_Init (CRC_InitTypeDef* CRCInitStruct);
void CRC_INPUT_DATA (uint32_t DATA);
uint32_t CRC_GET_RESULT ();
void CRC_SET_INITVALUE (uint32_t DATA);
void CRC_SET_PLN (uint32_t DATA);
void CRC_SET_RXOR (uint32_t DATA);
void CRC_SET_IDATA (uint32_t DATA);
uint32_t CRC_GET_TEMP ();
void CRC_SET_RSET ();
#endif  //KF32A_Periph_crc
#endif /* _KF32A_BASIC_CRC_H */
