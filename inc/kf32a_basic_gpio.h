/**
  ********************************************************************
  * 文件名  kf32a_basic_gpio.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  该文件提供了通用输入输出端口(GPIO)相关功能函数声明及相关宏定义。
  *
  *********************************************************************
*/

#ifndef _KF32A_BASIC_GPIO_H_
#define _KF32A_BASIC_GPIO_H_

#include "KF32A_BASIC.h"


/**
  * GPIO内存指针
  */
#define CHECK_GPIO_ALL_PERIPH(PERIPH)   (((PERIPH) == GPIOA_SFR) \
                                      || ((PERIPH) == GPIOB_SFR) \
                                      || ((PERIPH) == GPIOC_SFR) \
                                      || ((PERIPH) == GPIOD_SFR) \
                                      || ((PERIPH) == GPIOE_SFR) \
                                      || ((PERIPH) == GPIOF_SFR) \
                                      || ((PERIPH) == GPIOG_SFR) \
                                      || ((PERIPH) == GPIOH_SFR))

/**
  * GPIO上拉/下拉
  */
typedef enum
{
    GPIO_NOPULL = 0x00,
    GPIO_PULLUP = 0x01
}GPIOPU_TypeDef, GPIOPD_TypeDef;
#define CHECK_GPIO_PU(PU)               (((PU) == GPIO_NOPULL) \
                                      || ((PU) == GPIO_PULLUP))
#define GPIO_PULLDOWN  GPIO_PULLUP
#define CHECK_GPIO_PD(PD)               (((PD) == GPIO_NOPULL) \
                                      || ((PD) == GPIO_PULLDOWN))

/**
  * GPIO开漏输出类型
  */
typedef enum
{
    GPIO_POD_PP = 0x00,
    GPIO_POD_OD = 0x01
}GPIOPOD_TypeDef;
#define CHECK_GPIO_POD(OTYPE)           (((OTYPE) == GPIO_POD_PP) \
                                      || ((OTYPE) == GPIO_POD_OD))

/**
  * GPIO输出模式控制
  */
typedef enum
{
    GPIO_MODE_IN   = 0x00,              /* 通用IO口输入模式 */
    GPIO_MODE_OUT  = 0x01,              /* 通用IO口输出模式 */
    GPIO_MODE_RMP  = 0x02,              /* 重映射IO口功能模式 */
    GPIO_MODE_AN   = 0x03               /* 模拟模式 */
}GPIOMode_TypeDef;
#define CHECK_GPIO_MODE(MODE)           (((MODE) == GPIO_MODE_IN) \
                                      || ((MODE) == GPIO_MODE_OUT) \
                                      || ((MODE) == GPIO_MODE_RMP) \
                                      || ((MODE) == GPIO_MODE_AN))

/**
  * GPIO端口输出速度控制
  */
typedef enum
{
    GPIO_LOW_SPEED  = 0,
    GPIO_HIGH_SPEED = 1
}GPIOSpeed_TypeDef;
#define CHECK_GPIO_SPEED(SPEED)         (((SPEED) == GPIO_LOW_SPEED) \
                                      || ((SPEED) == GPIO_HIGH_SPEED))

/**
  * 描述  GPIO配置信息结构体
  */
typedef struct
{
    uint32_t m_Pin;                     /* GPIO端口掩码，
                                           取值为宏“GPIO端口掩码”中的一个或多个组合 */

    GPIOMode_TypeDef m_Mode;            /* GPIO输出模式，
                                           取值为枚举类型GPIOMode_TypeDef中的一个 */

    GPIOSpeed_TypeDef m_Speed;          /* GPIO输出速度。
                                           取值为枚举类型GPIOSpeed_TypeDef中的一个 */

    GPIOPOD_TypeDef m_OpenDrain;        /* GPIO开漏控制，
                                           取值为枚举类型GPIOPOD_TypeDef中的一个 */

    GPIOPU_TypeDef m_PullUp;            /* GPIO上拉配置，
                                           取值为枚举类型GPIOPU_TypeDef中的一个 */

    GPIOPD_TypeDef m_PullDown;          /* GPIO下拉配置，
                                           取值为枚举类型GPIOPU_TypeDef中的一个 */
}GPIO_InitTypeDef;

/**
  * GPIO端口配置
  */
typedef enum
{
    Bit_RESET = 0,
    Bit_SET = !Bit_RESET
}BitAction;
#define CHECK_GPIO_BIT_ACTION(ACTION)   (((ACTION) == Bit_RESET) \
                                      || ((ACTION) == Bit_SET))

/**
  * GPIO端口编号
  */
#define GPIO_Pin_Num_0                   ((uint32_t)0x0)
#define GPIO_Pin_Num_1                   ((uint32_t)0x1)
#define GPIO_Pin_Num_2                   ((uint32_t)0x2)
#define GPIO_Pin_Num_3                   ((uint32_t)0x3)
#define GPIO_Pin_Num_4                   ((uint32_t)0x4)
#define GPIO_Pin_Num_5                   ((uint32_t)0x5)
#define GPIO_Pin_Num_6                   ((uint32_t)0x6)
#define GPIO_Pin_Num_7                   ((uint32_t)0x7)
#define GPIO_Pin_Num_8                   ((uint32_t)0x8)
#define GPIO_Pin_Num_9                   ((uint32_t)0x9)
#define GPIO_Pin_Num_10                  ((uint32_t)0xA)
#define GPIO_Pin_Num_11                  ((uint32_t)0xB)
#define GPIO_Pin_Num_12                  ((uint32_t)0xC)
#define GPIO_Pin_Num_13                  ((uint32_t)0xD)
#define GPIO_Pin_Num_14                  ((uint32_t)0xE)
#define GPIO_Pin_Num_15                  ((uint32_t)0xF)
#define CHECK_GPIO_PIN_NUM(PIN)          (((PIN) >> 4) == 0x00)

/**
  * GPIO端口掩码
  */
#define GPIO_PIN_MASK_0                  ((uint16_t)0x0001)
#define GPIO_PIN_MASK_1                  ((uint16_t)0x0002)
#define GPIO_PIN_MASK_2                  ((uint16_t)0x0004)
#define GPIO_PIN_MASK_3                  ((uint16_t)0x0008)
#define GPIO_PIN_MASK_4                  ((uint16_t)0x0010)
#define GPIO_PIN_MASK_5                  ((uint16_t)0x0020)
#define GPIO_PIN_MASK_6                  ((uint16_t)0x0040)
#define GPIO_PIN_MASK_7                  ((uint16_t)0x0080)
#define GPIO_PIN_MASK_8                  ((uint16_t)0x0100)
#define GPIO_PIN_MASK_9                  ((uint16_t)0x0200)
#define GPIO_PIN_MASK_10                 ((uint16_t)0x0400)
#define GPIO_PIN_MASK_11                 ((uint16_t)0x0800)
#define GPIO_PIN_MASK_12                 ((uint16_t)0x1000)
#define GPIO_PIN_MASK_13                 ((uint16_t)0x2000)
#define GPIO_PIN_MASK_14                 ((uint16_t)0x4000)
#define GPIO_PIN_MASK_15                 ((uint16_t)0x8000)
#define GPIO_PIN_MASK_All                ((uint16_t)0xFFFF)
#define CHECK_GPIO_PIN_MASK(PIN)         (((PIN) >> 16 ) == (uint32_t)0x00)

/**
  * GPIO引脚重映射
  */
#define GPIO_RMP_AF0_SYSTEM              ((uint8_t)0x00)
#define GPIO_RMP_AF1_T0                  ((uint8_t)0x01)
#define GPIO_RMP_AF1_T1                  ((uint8_t)0x01)
#define GPIO_RMP_AF1_T2                  ((uint8_t)0x01)
#define GPIO_RMP_AF1_T3                  ((uint8_t)0x01)
#define GPIO_RMP_AF1_T4                  ((uint8_t)0x01)
#define GPIO_RMP_AF2_T5                  ((uint8_t)0x02)
#define GPIO_RMP_AF2_T6                  ((uint8_t)0x02)
#define GPIO_RMP_AF2_T9                  ((uint8_t)0x02)
#define GPIO_RMP_AF2_T10                 ((uint8_t)0x02)
#define GPIO_RMP_AF3_T9                  ((uint8_t)0x03)
#define GPIO_RMP_AF3_T20                 ((uint8_t)0x03)
#define GPIO_RMP_AF3_T21                 ((uint8_t)0x03)
#define GPIO_RMP_AF3_T23                 ((uint8_t)0x03)
#define GPIO_RMP_AF3_QEI1                ((uint8_t)0x03)
#define GPIO_RMP_AF4_T9                  ((uint8_t)0x04)
#define GPIO_RMP_AF4_T14                 ((uint8_t)0x04)
#define GPIO_RMP_AF4_T15                 ((uint8_t)0x04)
#define GPIO_RMP_AF4_T18                 ((uint8_t)0x04)
#define GPIO_RMP_AF4_T19                 ((uint8_t)0x04)
#define GPIO_RMP_AF4_T22                 ((uint8_t)0x04)
#define GPIO_RMP_AF4_QEI0                ((uint8_t)0x04)
#define GPIO_RMP_AF5_USART0              ((uint8_t)0x05)
#define GPIO_RMP_AF5_USART1              ((uint8_t)0x05)
#define GPIO_RMP_AF5_USART2              ((uint8_t)0x05)
#define GPIO_RMP_AF6_USART3              ((uint8_t)0x06)
#define GPIO_RMP_AF6_USART4              ((uint8_t)0x06)
#define GPIO_RMP_AF6_USART5              ((uint8_t)0x06)
#define GPIO_RMP_AF6_USART6              ((uint8_t)0x06)
#define GPIO_RMP_AF6_USART7              ((uint8_t)0x06)
#define GPIO_RMP_AF7_SPI0                ((uint8_t)0x07)
#define GPIO_RMP_AF7_SPI1                ((uint8_t)0x07)
#define GPIO_RMP_AF7_SPI2                ((uint8_t)0x07)
#define GPIO_RMP_AF7_SPI3                ((uint8_t)0x07)
#define GPIO_RMP_AF8_I2C0                ((uint8_t)0x08)
#define GPIO_RMP_AF8_I2C1                ((uint8_t)0x08)
#define GPIO_RMP_AF8_I2C2                ((uint8_t)0x08)
#define GPIO_RMP_AF8_I2C3                ((uint8_t)0x08)
#define GPIO_RMP_AF9_CAN0                ((uint8_t)0x09)
#define GPIO_RMP_AF9_CAN1                ((uint8_t)0x09)
#define GPIO_RMP_AF9_CAN2                ((uint8_t)0x09)
#define GPIO_RMP_AF9_CAN3                ((uint8_t)0x09)
#define GPIO_RMP_AF9_CAN4                ((uint8_t)0x09)
#define GPIO_RMP_AF9_CAN5                ((uint8_t)0x09)
#define GPIO_RMP_AF9_FLT                 ((uint8_t)0x09)
#define GPIO_RMP_AF10_CCP                ((uint8_t)0x0A)
#define GPIO_RMP_AF10_SPI                ((uint8_t)0x0A)
#define GPIO_RMP_AF10_I2C                ((uint8_t)0x0A)
#define GPIO_RMP_AF11_USART              ((uint8_t)0x0B)
#define GPIO_RMP_AF11_CCP		         ((uint8_t)0x0B)
#define GPIO_RMP_AF11_SPI		         ((uint8_t)0x0B)
#define GPIO_RMP_AF11_I2C		         ((uint8_t)0x0B)
#define GPIO_RMP_AF12_LED		         ((uint8_t)0x0C)
#define GPIO_RMP_AF12_CFGL		         ((uint8_t)0x0C)
#define GPIO_RMP_AF13_EXIC		         ((uint8_t)0x0D)
#define GPIO_RMP_AF14_LED		         ((uint8_t)0x0E)
#define GPIO_RMP_AF15_TESTPAD	         ((uint8_t)0x0F)
#define CHECK_GPIO_RMP(RMP)              (((RMP) == GPIO_RMP_AF0_SYSTEM  ) \
                                      || ((RMP) == GPIO_RMP_AF1_T0      )     \
                                      || ((RMP) == GPIO_RMP_AF1_T1      )     \
                                      || ((RMP) == GPIO_RMP_AF1_T2      )     \
                                      || ((RMP) == GPIO_RMP_AF1_T3      )     \
                                      || ((RMP) == GPIO_RMP_AF1_T4      )     \
                                      || ((RMP) == GPIO_RMP_AF2_T5      )     \
                                      || ((RMP) == GPIO_RMP_AF2_T6      )     \
                                      || ((RMP) == GPIO_RMP_AF2_T9      )     \
                                      || ((RMP) == GPIO_RMP_AF2_T10     )    \
                                      || ((RMP) == GPIO_RMP_AF3_T9      )    \
                                      || ((RMP) == GPIO_RMP_AF3_T20     )    \
                                      || ((RMP) == GPIO_RMP_AF3_T21     )    \
                                      || ((RMP) == GPIO_RMP_AF3_T23     )    \
                                      || ((RMP) == GPIO_RMP_AF3_QEI1    )    \
                                      || ((RMP) == GPIO_RMP_AF4_T14     )    \
                                      || ((RMP) == GPIO_RMP_AF4_T15     )    \
                                      || ((RMP) == GPIO_RMP_AF4_T18     )    \
                                      || ((RMP) == GPIO_RMP_AF4_T19     )    \
                                      || ((RMP) == GPIO_RMP_AF4_T22     )    \
                                      || ((RMP) == GPIO_RMP_AF4_QEI0    )   \
                                      || ((RMP) == GPIO_RMP_AF5_USART0  )   \
                                      || ((RMP) == GPIO_RMP_AF5_USART1  ) \
                                      || ((RMP) == GPIO_RMP_AF5_USART2  ) \
                                      || ((RMP) == GPIO_RMP_AF6_USART3  ) \
                                      || ((RMP) == GPIO_RMP_AF6_USART4  ) \
                                      || ((RMP) == GPIO_RMP_AF6_USART5  ) \
                                      || ((RMP) == GPIO_RMP_AF6_USART6  ) \
                                      || ((RMP) == GPIO_RMP_AF6_USART7  ) \
                                      || ((RMP) == GPIO_RMP_AF7_SPI0    ) \
                                      || ((RMP) == GPIO_RMP_AF7_SPI1    ) \
                                      || ((RMP) == GPIO_RMP_AF7_SPI2    )   \
                                      || ((RMP) == GPIO_RMP_AF7_SPI3    )   \
                                      || ((RMP) == GPIO_RMP_AF8_I2C0    )   \
                                      || ((RMP) == GPIO_RMP_AF8_I2C1    )   \
                                      || ((RMP) == GPIO_RMP_AF8_I2C2    )   \
                                      || ((RMP) == GPIO_RMP_AF8_I2C3    )   \
                                      || ((RMP) == GPIO_RMP_AF9_CAN0    )   \
                                      || ((RMP) == GPIO_RMP_AF9_CAN1    )   \
                                      || ((RMP) == GPIO_RMP_AF9_CAN2    )   \
                                      || ((RMP) == GPIO_RMP_AF9_CAN3    )   \
                                      || ((RMP) == GPIO_RMP_AF9_CAN4    )   \
                                      || ((RMP) == GPIO_RMP_AF9_CAN5    )   \
                                      || ((RMP) == GPIO_RMP_AF9_FLT     )   \
                                      || ((RMP) == GPIO_RMP_AF10_CCP    )   \
                                      || ((RMP) == GPIO_RMP_AF10_SPI    ) \
                                      || ((RMP) == GPIO_RMP_AF10_I2C    ) \
                                      || ((RMP) == GPIO_RMP_AF11_USART  )   \
                                      || ((RMP) == GPIO_RMP_AF11_CCP		)   \
                                      || ((RMP) == GPIO_RMP_AF11_SPI		)   \
                                      || ((RMP) == GPIO_RMP_AF11_I2C		)   \
                                      || ((RMP) == GPIO_RMP_AF12_LED		)   \
                                      || ((RMP) == GPIO_RMP_AF12_CFGL		)   \
                                      || ((RMP) == GPIO_RMP_AF13_EXIC		)   \
                                      || ((RMP) == GPIO_RMP_AF14_LED		)   \
                                      || ((RMP) == GPIO_RMP_AF15_TESTPAD))


/* 通用输入输出端口(GPIO)初始化及配置函数定义*******************************/
void GPIO_Reset (GPIO_SFRmap* GPIOx);
void GPIO_Configuration (GPIO_SFRmap* GPIOx, GPIO_InitTypeDef* gpioInitStruct);
void GPIO_Struct_Init (GPIO_InitTypeDef* gpioInitStruct);
void GPIO_Pin_Lock_Config (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, FunctionalState NewState);
void GPIO_Pull_Up_Enable (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, FunctionalState NewState);
void GPIO_Toggle_Pull_Up_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
void GPIO_Pull_Down_Enable (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, FunctionalState NewState);
void GPIO_Toggle_Pull_Down_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
void GPIO_Open_Drain_Enable (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, GPIOPOD_TypeDef NewState);
void GPIO_Toggle_Open_Drain_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
void GPIO_Write_Mode_Bits (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, GPIOMode_TypeDef NewState);
void GPIO_Speed_Config (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, GPIOSpeed_TypeDef NewState);
void GPIO_Toggle_Speed_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
/* 通用输入输出端口(GPIO)读写函数定义***************************************/
BitAction GPIO_Read_Input_Data_Bit (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
uint16_t GPIO_Read_Input_Data (GPIO_SFRmap* GPIOx);
BitAction GPIO_Read_Output_Data_Bit (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
uint16_t GPIO_Read_Output_Data (GPIO_SFRmap* GPIOx);
void GPIO_Set_Output_Data_Bits (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPin, BitAction BitsValue);
void GPIO_Toggle_Output_Data_Config (GPIO_SFRmap* GPIOx, uint16_t GpioPin);
/* 通用输入输出端口(GPIO)重映射配置函数定义*********************************/
void GPIO_Pin_RMP_Config (GPIO_SFRmap* GPIOx,
                    uint16_t GpioPinNum, uint8_t PinRemap);

#endif /* _KF32A_BASIC_GPIO_H */
