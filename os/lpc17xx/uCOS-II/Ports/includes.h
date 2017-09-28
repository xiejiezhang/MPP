/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                        http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                          ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               includes.h
** Last modified date:      2013-03-11
** Last version:            V1.0
** Descriptions:            uCOS-II's include file   uCOS-II的头文件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Li Weifeng
** Created date:            2013-09-29
** Version:                 V1.0
** Descriptions:            The original version 初始版本
**--------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      
** Version:            
** Descriptions:       
**                      
*********************************************************************************************************/

#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  Standard header files 标准头文件
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


/*********************************************************************************************************
  Ports header files 移植头文件
*********************************************************************************************************/
#include <os_cfg.h>
#include <os_cpu.h>  
#include <ucos_ii.h>


/*********************************************************************************************************
  BSP header files  BSP头文件 
*********************************************************************************************************/
#include "LPC17xx.h"


/*********************************************************************************************************
  User's header files 用户头文件
*********************************************************************************************************/
#include "main.h"
#include "sys_cfg.h"
#include "zgbRcv.h"
#include "zgbSnt.h"
#include "softTime.h"
#include "lwtpStart.h"
#include "getCarMsg.h"
#include "save.h"
#include "carRcv.h"
#include "debug_printf.h"
#include "timeSyn.h"
#include "IWDT.h"
#include "bootloader_cfg.h"
#include "sysChecker.h"
#include "Sys_Debug.h"
#include "charger.h"
/*********************************************************************************************************
  Old error code names before V2.84   V2.84版本之前的旧错误代码
*********************************************************************************************************/
#if OS_VERSION >= 284u

#define OS_NO_ERR                       OS_ERR_NONE
#define OS_TIMEOUT                      OS_ERR_TIMEOUT
#define OS_TASK_NOT_EXIST               OS_ERR_TASK_NOT_EXIST
#define OS_MBOX_FULL                    OS_ERR_MBOX_FULL
#define OS_Q_FULL                       OS_ERR_Q_FULL
#define OS_Q_EMPTY                      OS_ERR_Q_EMPTY
#define OS_PRIO_EXIST                   OS_ERR_PRIO_EXIST
#define OS_PRIO_ERR                     OS_ERR_PRIO
#define OS_PRIO_INVALID                 OS_ERR_PRIO_INVALID
#define OS_SEM_OVF                      OS_ERR_SEM_OVF
#define OS_TASK_DEL_ERR                 OS_ERR_TASK_DEL
#define OS_TASK_DEL_IDLE                OS_ERR_TASK_DEL_IDLE
#define OS_TASK_DEL_REQ                 OS_ERR_TASK_DEL_REQ
#define OS_TASK_DEL_ISR                 OS_ERR_TASK_DEL_ISR
#define OS_NO_MORE_TCB                  OS_ERR_TASK_NO_MORE_TCB
#define OS_TIME_NOT_DLY                 OS_ERR_TIME_NOT_DLY
#define OS_TIME_INVALID_MINUTES         OS_ERR_TIME_INVALID_MINUTES
#define OS_TIME_INVALID_SECONDS         OS_ERR_TIME_INVALID_SECONDS
#define OS_TIME_INVALID_MS              OS_ERR_TIME_INVALID_MS
#define OS_TIME_ZERO_DLY                OS_ERR_TIME_ZERO_DLY
#define OS_TASK_SUSPEND_PRIO            OS_ERR_TASK_SUSPEND_PRIO
#define OS_TASK_SUSPEND_IDLE            OS_ERR_TASK_SUSPEND_IDLE
#define OS_TASK_RESUME_PRIO             OS_ERR_TASK_RESUME_PRIO
#define OS_TASK_NOT_SUSPENDED           OS_ERR_TASK_NOT_SUSPENDED
#define OS_MEM_INVALID_PART             OS_ERR_MEM_INVALID_PART
#define OS_MEM_INVALID_BLKS             OS_ERR_MEM_INVALID_BLKS
#define OS_MEM_INVALID_SIZE             OS_ERR_MEM_INVALID_SIZE
#define OS_MEM_NO_FREE_BLKS             OS_ERR_MEM_NO_FREE_BLKS
#define OS_MEM_FULL                     OS_ERR_MEM_FULL
#define OS_MEM_INVALID_PBLK             OS_ERR_MEM_INVALID_PBLK
#define OS_MEM_INVALID_PMEM             OS_ERR_MEM_INVALID_PMEM
#define OS_MEM_INVALID_PDATA            OS_ERR_MEM_INVALID_PDATA
#define OS_MEM_INVALID_ADDR             OS_ERR_MEM_INVALID_ADDR
#define OS_MEM_NAME_TOO_LONG            OS_ERR_MEM_NAME_TOO_LONG
#define OS_TASK_OPT_ERR                 OS_ERR_TASK_OPT
#define OS_FLAG_INVALID_PGRP            OS_ERR_FLAG_INVALID_PGRP
#define OS_FLAG_ERR_WAIT_TYPE           OS_ERR_FLAG_WAIT_TYPE
#define OS_FLAG_ERR_NOT_RDY             OS_ERR_FLAG_NOT_RDY
#define OS_FLAG_INVALID_OPT             OS_ERR_FLAG_INVALID_OPT
#define OS_FLAG_GRP_DEPLETED            OS_ERR_FLAG_GRP_DEPLETED

#else

#define OS_ERR_NONE                     OS_NO_ERR                    
#define OS_ERR_TIMEOUT                  OS_TIMEOUT                   
#define OS_ERR_TASK_NOT_EXIST           OS_TASK_NOT_EXIST            
#define OS_ERR_MBOX_FULL                OS_MBOX_FULL                 
#define OS_ERR_Q_FULL                   OS_Q_FULL                    
#define OS_ERR_Q_EMPTY                  OS_Q_EMPTY                   
#define OS_ERR_PRIO_EXIST               OS_PRIO_EXIST                
#define OS_ERR_PRIO                     OS_PRIO_ERR                  
#define OS_ERR_PRIO_INVALID             OS_PRIO_INVALID              
#define OS_ERR_SEM_OVF                  OS_SEM_OVF                   
#define OS_ERR_TASK_DEL                 OS_TASK_DEL_ERR              
#define OS_ERR_TASK_DEL_IDLE            OS_TASK_DEL_IDLE             
#define OS_ERR_TASK_DEL_REQ             OS_TASK_DEL_REQ              
#define OS_ERR_TASK_DEL_ISR             OS_TASK_DEL_ISR              
#define OS_ERR_TASK_NO_MORE_TCB         OS_NO_MORE_TCB               
#define OS_ERR_TIME_NOT_DLY             OS_TIME_NOT_DLY              
#define OS_ERR_TIME_INVALID_MINUTES     OS_TIME_INVALID_MINUTES      
#define OS_ERR_TIME_INVALID_SECONDS     OS_TIME_INVALID_SECONDS      
#define OS_ERR_TIME_INVALID_MS          OS_TIME_INVALID_MS           
#define OS_ERR_TIME_ZERO_DLY            OS_TIME_ZERO_DLY             
#define OS_ERR_TASK_SUSPEND_PRIO        OS_TASK_SUSPEND_PRIO         
#define OS_ERR_TASK_SUSPEND_IDLE        OS_TASK_SUSPEND_IDLE         
#define OS_ERR_TASK_RESUME_PRIO         OS_TASK_RESUME_PRIO          
#define OS_ERR_TASK_NOT_SUSPENDED       OS_TASK_NOT_SUSPENDED        
#define OS_ERR_MEM_INVALID_PART         OS_MEM_INVALID_PART          
#define OS_ERR_MEM_INVALID_BLKS         OS_MEM_INVALID_BLKS          
#define OS_ERR_MEM_INVALID_SIZE         OS_MEM_INVALID_SIZE          
#define OS_ERR_MEM_NO_FREE_BLKS         OS_MEM_NO_FREE_BLKS          
#define OS_ERR_MEM_FULL                 OS_MEM_FULL                  
#define OS_ERR_MEM_INVALID_PBLK         OS_MEM_INVALID_PBLK          
#define OS_ERR_MEM_INVALID_PMEM         OS_MEM_INVALID_PMEM          
#define OS_ERR_MEM_INVALID_PDATA        OS_MEM_INVALID_PDATA         
#define OS_ERR_MEM_INVALID_ADDR         OS_MEM_INVALID_ADDR          
#define OS_ERR_MEM_NAME_TOO_LONG        OS_MEM_NAME_TOO_LONG         
#define OS_ERR_TASK_OPT                 OS_TASK_OPT_ERR              
#define OS_ERR_FLAG_INVALID_PGRP        OS_FLAG_INVALID_PGRP         
#define OS_ERR_FLAG_WAIT_TYPE           OS_FLAG_ERR_WAIT_TYPE        
#define OS_ERR_FLAG_NOT_RDY             OS_FLAG_ERR_NOT_RDY          
#define OS_ERR_FLAG_INVALID_OPT         OS_FLAG_INVALID_OPT          
#define OS_ERR_FLAG_GRP_DEPLETED        OS_FLAG_GRP_DEPLETED         

#endif

      
#ifdef __cplusplus
}
#endif

#endif


/*********************************************************************************************************
  End of File
*********************************************************************************************************/
