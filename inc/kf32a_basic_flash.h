/**
  ********************************************************************
  * 文件名  kf32a_basic_flash.h
  * 作  者  ChipON_AE/FAE_Group
  * 版  本  V2.3
  * 日  期  2019-11-16
  * 描  述  该文件提供了FLASH程序存储器(FLASH)相关功能函数声明及相关宏定义。
  *********************************************************************
*/

#ifndef _KF32A_BASIC_FLASH_H_
#define _KF32A_BASIC_FLASH_H_

#include "KF32A_BASIC.h"


/* 锁定解锁状态 */
typedef enum
{
    LOCK = 0,
    UNLOCK = !LOCK
} LockStatus;
#define CHECK_LOCK_STATUS(STATE)        (((STATE) == LOCK) || ((STATE) == UNLOCK))

/**
  * 描述  FLASH CheckSum结果数据结构体
  */
typedef union CheckSumStruct
{
    uint32_t m_ResultWord[4];     /* CheckSum的128位结果，字访问。 */
    uint32_t m_ResultShort[8];    /* CheckSum的128位结果，半字访问。 */
    uint32_t m_ResultByte[16];    /* CheckSum的128位结果，字节访问。 */
}FLASH_CheckSumResult;

/**
  * 描述  FLASH编程信息结构体
  */
typedef struct
{
    uint32_t m_Mode;                  /* 编程模式，
                                         取值宏“FLASH 编程模式”中的一个。 */
    uint32_t m_Zone;                  /* 编程区域，
                                         取值宏“FLASH 编程区域”中的一个。 */
    uint32_t m_Addr;                  /* 编程地址，
                                         取值为0x0~0xFFFFF，硬件忽略低2位。 */
    uint32_t m_WriteSize;             /* 编程长度，单位：双字(64位)
                                         取值为1~63。 */
    uint32_t * m_Data;                /* 编程数据指针，
                                         指向待写数据。 */
}FLASH_ProgramTypeDef;

/**
  * FLASH 编程模式
  */
#define FLASH_PROGRAM_WORD              ((uint32_t)0x15)
#define FLASH_PROGRAM_HALF_PAGE         ((uint32_t)0x05)
#define CHECK_FLASH_PROGRAM_MODE(MODE)  (((MODE) == FLASH_PROGRAM_WORD) \
                                      || ((MODE) == FLASH_PROGRAM_HALF_PAGE))

/**
  * FLASH 编程区域
  */
#define FLASH_PROGRAM_CODE              ((uint32_t)0)
#define FLASH_PROGRAM_CFG           ((uint32_t)1)
#define CHECK_FLASH_PROGRAM_ZONE(ZONE)  (((ZONE) == FLASH_PROGRAM_CODE) \
                                      || ((ZONE) == FLASH_PROGRAM_CFG))

/**
  * FLASH信息区选择
  */
#define FLASH_NVM_CODE_ZONE             ((uint32_t)0<<FLASH_ISPCON0_IFEN_POS)
#define FLASH_NVM_INFORMATION_ZONE      ((uint32_t)1<<FLASH_ISPCON0_IFEN_POS)
#define CHECK_FLASH_NVM_ZONE(SEL)       (((SEL) == FLASH_NVM_CODE_ZONE) \
                                      || ((SEL) == FLASH_NVM_INFORMATION_ZONE))

/**
  * FLASH模式
  */
#define FLASH_MODE_NORMAL               ((uint32_t)0<<FLASH_ISPCON0_STANDBY1_POS)
#define FLASH_MODE_STANDBY1             ((uint32_t)1<<FLASH_ISPCON0_STANDBY1_POS)
#define FLASH_MODE_STANDBY2             ((uint32_t)2<<FLASH_ISPCON0_STANDBY1_POS)
#define FLASH_MODE_STANDBY1_STANDBY2    ((uint32_t)3<<FLASH_ISPCON0_STANDBY1_POS)
#define CHECK_FLASH_MODE(SEL)           (((SEL) == FLASH_MODE_NORMAL) \
                                      || ((SEL) == FLASH_MODE_STANDBY1) \
                                      || ((SEL) == FLASH_MODE_STANDBY2) \
                                      || ((SEL) == FLASH_MODE_STANDBY1_STANDBY2))

/**
  * FLASH-IP选择
  */
#define FLASH_IP1                       ((uint32_t)0<<FLASH_ISPCON1_IPSEL_POS)
#define CHECK_FLASH_IP(SEL)             (((SEL) == FLASH_IP1))

/**
  * FLASH读模式
  */
#define FLASH_READ_MODE_NORMAL          ((uint32_t)0<<FLASH_ISPCON1_RECALLEN_POS)
#define FLASH_READ_MODE_RECALL          ((uint32_t)1<<FLASH_ISPCON1_RECALLEN_POS)
#define CHECK_FLASH_READ_MODE(SEL)      (((SEL) == FLASH_READ_MODE_NORMAL) \
                                      || ((SEL) == FLASH_READ_MODE_RECALL))

/**
  * FLASH 写入的字数
  */
#define CHECK_FLASH_WRITE_SIZE(SIZE)    (((uint32_t)(SIZE) >> 6) == 0)

/**
  * FLASH访问周期
  */
#define CHECK_FLASH_PERIOD(NUM)         (((uint32_t)(NUM) >> 4) == 0)

/**
  * FLASH编程和行擦期间的地址
  */
#define CHECK_FLASH_ADDR_20BITS(ADDR)   (((uint32_t)(ADDR) >> 20) == 0)
#define CHECK_FLASH_ADDR_13BITS(ADDR)   (((uint32_t)(ADDR) >> 13) == 0)

/**
  * FLASH命令选择
  */
#define FLASH_PROGRAM_CMD_WORD          ((uint32_t)0x15<<FLASH_ISPCMD_CMD0_POS)
#define FLASH_PROGRAM_CMD_ALL_CODE      ((uint32_t)0x0A<<FLASH_ISPCMD_CMD0_POS)
#define FLASH_PROGRAM_CMD_PAGE          ((uint32_t)0x1B<<FLASH_ISPCMD_CMD0_POS)
#define FLASH_PROGRAM_CMD_HALF_PAGE     ((uint32_t)0x05<<FLASH_ISPCMD_CMD0_POS)
#define CHECK_FLASH_PROGRAM_CMD(CMD)    (((CMD) == FLASH_PROGRAM_CMD_WORD) \
                                      || ((CMD) == FLASH_PROGRAM_CMD_ALL_CODE) \
                                      || ((CMD) == FLASH_PROGRAM_CMD_PAGE) \
                                      || ((CMD) == FLASH_PROGRAM_CMD_HALF_PAGE))

/**
  * FLASH 擦除模式
  */
#define FLASH_WIPE_CODE_PAGE            ((uint32_t)0)
#define FLASH_WIPE_CFG_PAGE             ((uint32_t)1)
#define FLASH_WIPE_CODE_ALL             ((uint32_t)2)
#define CHECK_FLASH_WIPE_MODE(MODE)     (((MODE) == FLASH_WIPE_CODE_PAGE) \
                                      || ((MODE) == FLASH_WIPE_CFG_PAGE) \
                                      || ((MODE) == FLASH_WIPE_CODE_ALL))


/* FLASH程序存储器(FLASH)功能配置函数定义**************************/
LockStatus FLASH_Get_NonVolatile_Memory_Unlock_Status_RAM (void);
void FLASH_Clear_NonVolatile_Memory_Unlock_Status_RAM (void);
void FLASH_Unlock_ISP_RAM (void);
LockStatus FLASH_Get_Flash_Unlock_Status_RAM (void);
void FLASH_Clear_Flash_Unlock_Status_RAM (void);
void FLASH_Unlock_Code_RAM (void);
LockStatus FLASH_Get_Config_Unlock_Status_RAM (void);
void FLASH_Clear_Config_Unlock_Status_RAM (void);
void FLASH_Unlock_User_Config_RAM (void);
void FLASH_Data_Write_Enable_RAM (FunctionalState NewState);
void FLASH_Zone_Config_RAM (uint32_t ZoneSelect);
void FLASH_Standby_Mode_Config_RAM (uint32_t ModeSelect);
void FLASH_IP_Config_RAM (uint32_t FlashIPSelect);
void FLASH_Read_Mode_Config_RAM (uint32_t ReadMode);
void FLASH_Calibration_Updata_Enable_RAM (FunctionalState NewState);
void FLASH_Information_Zone_Wipe_Unlock_Config_RAM (LockStatus NewState);
void FLASH_Half_Page_Write_Size_Config_RAM (uint32_t WriteSize);
void FLASH_Program_Cmd_Config_RAM (uint32_t CmdSelect);
void FLASH_Executive_Cmd_RAM (void);
void FLASH_NonVolatile_Memory_ECC_Enable_RAM (FunctionalState NewState);
void FLASH_Linear_Prefetch_Enable_RAM (FunctionalState NewState);
void FLASH_Period_Number_Config_RAM (uint32_t PeriodNum);
void FLASH_Program_Addr_Config_RAM (uint32_t ProgramAddr);
FlagStatus FLASH_Get_Program_Status_RAM (void);
FlagStatus FLASH_Get_Wipe_Status_RAM (void);
FlagStatus FLASH_Get_Compute_Complete_Status_RAM (void);
void FLASH_Clear_Compute_Complete_Status_RAM (void);
#if _CLOSE_SERVICE_
void FLASH_Forbid_Remap_Enable_RAM (FunctionalState NewState);
void FLASH_Forbid_SWD_Enable_RAM (FunctionalState NewState);
FlagStatus FLASH_Get_CFG_Error_Flag_RAM (void);
void FLASH_Clear_CFG_Error_Flag_RAM (void);
#endif /* _CLOSE_SERVICE_ */
void FLASH_CheckSum_Addr_Config_RAM(uint32_t StartAddr, uint32_t StopAddr);
void FLASH_Start_SIG_Compute_Enable_RAM (FunctionalState NewState);
void FLASH_Get_CheckSum_Result_RAM (FLASH_CheckSumResult* CheckSumStruct);
/* FLASH程序存储器(FLASH)编程功能函数定义**************************/
void FLASH_Wipe_Configuration_RAM (uint32_t WipeMode,
                    uint32_t WipeAddr);
void FLASH_Program_Configuration_RAM (FLASH_ProgramTypeDef * flashProgramStruct);

void FLASH_Clear_NonVolatile_Memory_Unlock_Status (void);

void FLASH_Unlock_ISP   (void);
void FLASH_Clear_Flash_Unlock_Status (void);
void FLASH_Unlock_Code (void);

void FLASH_Clear_Config_Unlock_Status (void);
void FLASH_Unlock_User_Config(void);

void FLASH_Zone_Config(uint32_t ZoneSelect);
void FLASH_Executive_Cmd(void);

FlagStatus FLASH_Get_Program_Status (void);

FlagStatus FLASH_Get_Wipe_Status (void);

void FLASH_Wipe_Configuration (uint32_t WipeMode,uint32_t WipeAddr);

#endif /* _KF32A_BASIC_FLASH_H */
