/**
  *********************************************************************
  * 文件名 KF32A_Config.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描  述  KF32A系列外设配置
  *
  *********************************************************************
  */

#ifndef _KF32A_Config_H_
#define _KF32A_Config_H_

/* 型号定义 */
#ifdef KF32A151KQS
	#define KF32A151
#elif defined KF32A151MQS
    #define KF32A151
#elif defined KF32A151KQV
	#define KF32A151
#elif defined KF32A151MQV
	#define KF32A151
#endif

/* 型号定义 */
#ifdef KF32A141IQS
	#define KF32A141
#endif
//
#ifdef KF32A250KQT
	#define KF32A250
#elif defined KF32A250MQT
	#define KF32A250
#elif defined KF32A250MQV
	#define KF32A250
#endif

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
	#define KF32A_Periph_usart3
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
	#define KF32A_Periph_usart3
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
#endif //KF32A141

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
	/* SPI */
	#define KF32A_Periph_spi
	#define KF32A_Periph_spi0
	#define KF32A_Periph_spi1
    /* USART */
	#define KF32A_Periph_usart
	#define KF32A_Periph_usart0
	#define KF32A_Periph_usart1
	#define KF32A_Periph_usart2
	#define KF32A_Periph_usart3
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

#endif /* _KF32A_Config_H */
