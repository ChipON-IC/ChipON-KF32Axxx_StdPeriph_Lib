/**
  ******************************************************************************
  * 文件名  main.c
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.61
  * 日  期  2019-11-16
  * 描  述  该文件提供了基于外设库的最小代码
  *
  *********************************************************************
  */
#include "system_init.h"

/**
  * 描述   主函数
  * 输入   无
  * 返回   无
  */
void main()
{
	/*上电默认时钟为16M的128分频，如初始化RAM时间过长，系统时钟初始化可放在startup.c
	 * 的startup()函数初始化RAM之前*/
	//系统时钟,外设高频时钟配置
	SystemInit();
	/* systick初始化*/
	//systick_delay_init();
	while(1)
	{

	}		
}

/**
  * 描述   报告校验发生错误的文件和行
  * 输入   file: 指向文件名称的指针
  *      line： 校验错误的行
  * 返回  无。
  */
void check_failed(uint8_t* file, uint32_t line)
{
	  /* 用户可以添加自己的代码实现报告文件名和行号,
	             例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	  while(1)
	  {
		  ;
	  }
};
