/**
  *********************************************************************
  * 文件名 KF32A_Config.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.62
  * 日  期  2019-11-16
  * 描  述  KF32A系列外设配置
  *
  *********************************************************************
  */

#ifndef _KF32A_Config_H_
#define _KF32A_Config_H_

#include "__Kungfu32_chipmodel_define.h"

/* 型号定义 */
#ifdef KF32A152MQV 
	#define KF32A152
#endif
#ifdef KF32A153MQV 
	#define KF32A153
#endif

/* 型号定义 */
#ifdef KF32A151KQS
	#define KF32A151
#elif defined KF32A151MQS
    #define KF32A151
#elif defined KF32A151KQT
    #define KF32A151
#elif defined KF32A151MQT
    #define KF32A151
#elif defined KF32A151KQV
	#define KF32A151
#elif defined KF32A151MQV
	#define KF32A151
#endif

/* 型号定义 */
#ifdef KF32A150KQS
	#define KF32A150
#elif defined KF32A150MQS
	#define KF32A150
#elif defined KF32A150KQT
	#define KF32A150
#elif defined KF32A150MQT
	#define KF32A150
#elif defined KF32A150MNT
	#define KF32A150
#elif defined KF32A150KQV
	#define KF32A150
#elif defined KF32A150MQV
	#define KF32A150
#endif


/* 型号定义 */
#ifdef KF32A251KQS
	#define KF32A251
#elif defined KF32A251IQT
	#define KF32A251
#elif defined KF32A251KQT
	#define KF32A251
#elif defined KF32A251MQT
	#define KF32A251
#elif defined KF32A251KQV
	#define KF32A251
#elif defined KF32A251MQV
	#define KF32A251
#endif
#if defined KF32A251KQT || defined KF32A251MQT || defined KF32A251KQV || \
	defined KF32A251MQV
	#define A251_TIME_EX 
#endif

/* 型号定义 */
#ifdef KF32A250GQS
	#define KF32A250
#elif defined KF32A250KQT
	#define KF32A250
#elif defined KF32A250MQT
	#define KF32A250
#elif defined KF32A250MQV
	#define KF32A250
#endif

/* 型号定义 */
#ifdef KF32A141INP
	#define KF32A141
#elif defined KF32A141IQS
	#define KF32A141
#elif defined KF32A141IQT
	#define KF32A141
#endif
#if defined KF32A141IQS || defined KF32A141IQT
	#define A141_CAN_EX 
	#define A141_USART_EX 
#endif

/* 型号定义 */
#ifdef KF32A140INP
	#define KF32A140
#elif defined KF32A140IQS
	#define KF32A140
#elif defined KF32A140KQT
	#define KF32A140
#elif defined KF32A140KQS
	#define KF32A140
#elif defined KF32A140IQT
	#define KF32A140
#endif
#if defined KF32A140IQS || defined KF32A140IQT || defined KF32A140KQS || defined KF32A140KQT
	#define A140_CAN_EX 
	#define A140_USART_EX 
#endif

/* 外设资源 */
#if defined KF32A152 || KF32A153
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    #define KF32A_Periph_can1
	#define KF32A_Periph_can2
    #define KF32A_Periph_can3
    #define KF32A_Periph_can4
    #define KF32A_Periph_can5
    /* EXIC */
    #define KF32A_Periph_exic
    /* CFGL */
    #define KF32A_Periph_cfgl
    /* CMP */
	#define KF32A_Periph_cmp
	/* CRC */
	#define KF32A_Periph_crc
	/* DAC */
	#define KF32A_Periph_dac
	#define KF32A_Periph_dac0
	#define KF32A_Periph_dac1
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	#define KF32A_Periph_i2c2
	#define KF32A_Periph_i2c3
	/* OP */
	#define KF32A_Periph_op
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	#define KF32A_Periph_qei1
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi1
	#define KF32A_Periph_spi2
	#define KF32A_Periph_spi3
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart3
	#define KF32A_Periph_usart4
	#define KF32A_Periph_usart5
	#define KF32A_Periph_usart6
	#define KF32A_Periph_usart7
	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
	#define KF32A_Periph_gptime4
	#define KF32A_Periph_gptime18
	#define KF32A_Periph_gptime19
	#define KF32A_Periph_gptime20
	#define KF32A_Periph_gptime21
	#define KF32A_Periph_gptime22_time23
	#define KF32A_Periph_gptime22
	#define KF32A_Periph_gptime23
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
	#define KF32A_Periph_atime9_time10
	#define KF32A_Periph_atime9
	#define KF32A_Periph_atime10
#endif //KF32A152||153


/* 外设资源 */
#ifdef KF32A151
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    #define KF32A_Periph_can1
    /* CFGL */
    #define KF32A_Periph_cfgl
    /* CMP */
	#define KF32A_Periph_cmp
	/* CRC */
	#define KF32A_Periph_crc
	/* CTOUCH */
	#define KF32A_Periph_ctouch
	/* DAC */
	#define KF32A_Periph_dac
	#define KF32A_Periph_dac0
	#define KF32A_Periph_dac1
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	/* OP */
	#define KF32A_Periph_op
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	#define KF32A_Periph_qei1
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi1
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart4
	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
	#define KF32A_Periph_gptime4
	#define KF32A_Periph_gptime18
	#define KF32A_Periph_gptime19
	#define KF32A_Periph_gptime20
	#define KF32A_Periph_gptime21
	#define KF32A_Periph_gptime22_time23
	#define KF32A_Periph_gptime22
	#define KF32A_Periph_gptime23
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
	#define KF32A_Periph_atime9_time10
	#define KF32A_Periph_atime9
	#define KF32A_Periph_atime10
#endif //KF32A151

/* 外设资源 */
#ifdef KF32A150
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    #define KF32A_Periph_can1
    /* CFGL */
    #define KF32A_Periph_cfgl
    /* CMP */
	#define KF32A_Periph_cmp
	/* CRC */
	#define KF32A_Periph_crc
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	#define KF32A_Periph_qei1
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi1
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart4
	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
	#define KF32A_Periph_gptime4
	#define KF32A_Periph_gptime18
	#define KF32A_Periph_gptime19
	#define KF32A_Periph_gptime20
	#define KF32A_Periph_gptime21
	#define KF32A_Periph_gptime22_time23
	#define KF32A_Periph_gptime22
	#define KF32A_Periph_gptime23
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
	#define KF32A_Periph_atime9_time10
	#define KF32A_Periph_atime9
	#define KF32A_Periph_atime10
#endif //KF32A150

/* 外设资源 */
#ifdef KF32A251
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    #define KF32A_Periph_can1
    /* CFGL */
    #define KF32A_Periph_cfgl
    /* CMP */
	#define KF32A_Periph_cmp
	/* CTOUCH */
	#define KF32A_Periph_ctouch
 	/* LCD */
	#define KF32A_Periph_lcd
	/* CRC */
	#define KF32A_Periph_crc
	/* DAC */
	#define KF32A_Periph_dac
	#define KF32A_Periph_dac0
	#define KF32A_Periph_dac1
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	/* OP */
	#define KF32A_Periph_op
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	#define KF32A_Periph_qei1
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi2
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart3
	#define KF32A_Periph_usart4

	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
#endif //KF32A251
#ifdef A251_TIME_EX
	/* GPTIME */
	#define KF32A_Periph_gptime4
	#define KF32A_Periph_gptime18
	#define KF32A_Periph_gptime19
	#define KF32A_Periph_gptime20
	#define KF32A_Periph_gptime21
	#define KF32A_Periph_gptime22_time23
	#define KF32A_Periph_gptime22
	#define KF32A_Periph_gptime23
    /* ATIME */
	#define KF32A_Periph_atime9_time10
	#define KF32A_Periph_atime9
	#define KF32A_Periph_atime10
#endif //A251_TIME_EX


/* 外设资源 */
#ifdef KF32A250
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
 	/* LCD */
	#define KF32A_Periph_lcd
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    #define KF32A_Periph_can1
    /* CFGL */
    #define KF32A_Periph_cfgl
	/* CRC */
	#define KF32A_Periph_crc
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	#define KF32A_Periph_qei1
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi2
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart4
	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
	#define KF32A_Periph_gptime4
	#define KF32A_Periph_gptime18
	#define KF32A_Periph_gptime19
	#define KF32A_Periph_gptime20
	#define KF32A_Periph_gptime21
	#define KF32A_Periph_gptime22_time23
	#define KF32A_Periph_gptime22
	#define KF32A_Periph_gptime23
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
	#define KF32A_Periph_atime9_time10
	#define KF32A_Periph_atime9
	#define KF32A_Periph_atime10
#endif //KF32A250

#ifdef KF32A141
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    /* CFGL */
    #define KF32A_Periph_cfgl
    /* CMP */
	#define KF32A_Periph_cmp
	/* CRC */
	#define KF32A_Periph_crc
	/* CTOUCH */
	#define KF32A_Periph_ctouch
	/* DAC */
	#define KF32A_Periph_dac
	#define KF32A_Periph_dac0
	#define KF32A_Periph_dac1
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	#define KF32A_Periph_i2c2
	/* OP */
	#define KF32A_Periph_op
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi2
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
    #define KF32A_Periph_gptime4
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
#endif //KF32A141
#ifdef A141_CAN_EX
    #define KF32A_Periph_can1
#endif //A141_CAN_EX
#ifdef A141_USART_EX	
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart4
#endif //A141_USART_EX

#ifdef KF32A140
    /* ADC */
	#define KF32A_Periph_adc
	#define KF32A_Periph_adc0
	#define KF32A_Periph_adc1
	#define KF32A_Periph_adc2
 	/* AES */
	#define KF32A_Periph_aes
	/* CAN */
	#define KF32A_Periph_can
    #define KF32A_Periph_can0
    /* CFGL */
    #define KF32A_Periph_cfgl
    /* CMP */
	#define KF32A_Periph_cmp
	/* CRC */
	#define KF32A_Periph_crc
	/* DMA */
	#define KF32A_Periph_dma
	#define KF32A_Periph_dma0
	#define KF32A_Periph_dma1
	/* I2C */
    #define KF32A_Periph_i2c
    #define KF32A_Periph_i2c0
	#define KF32A_Periph_i2c1
	#define KF32A_Periph_i2c2
	/* QEI */
	#define KF32A_Periph_qei
	#define KF32A_Periph_qei0
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi2
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	/* BTIME */
	#define KF32A_Periph_btime14
	#define KF32A_Periph_btime15
	/* GPTIME */
	#define KF32A_Periph_gptime0
	#define KF32A_Periph_gptime1
	#define KF32A_Periph_gptime2
	#define KF32A_Periph_gptime3
    #define KF32A_Periph_gptime4
	/* ATIME */
	#define KF32A_Periph_atime5_time6
	#define KF32A_Periph_atime5
	#define KF32A_Periph_atime6
#endif //KF32A140
#ifdef A140_CAN_EX
    #define KF32A_Periph_can1
#endif //A140_CAN_EX
#ifdef A140_USART_EX	
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart4
#endif //A140_USART_EX

#endif /* _KF32A_Config_H */
