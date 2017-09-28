/****************************************Copyright (c)****************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File Name:               os_cpu_c.c
** Last modified Date:      2007.01.18
** Last Version:            1.0
** Description:             The C functions that a uC/OS-II port requires    uC/OS-II��ֲ����Ҫ��C����		 
** 
**--------------------------------------------------------------------------------------------------------
** Created By:              Steven Zhou ���ܸ�
** Created date:            2007.01.18
** Version:                 1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao
** Modified date:           2007.10.29
** Version:                 1.1
** Description:             The second version �ڶ���
**
*********************************************************************************************************/
#define  OS_CPU_GLOBALS

#include "LPC17xx.h"
#include <includes.h>

extern unsigned int uiCounterMs;
extern void addZgbTimeCounter (unsigned int timeMs);
extern int addCarBaseTime (unsigned int timeMs);
extern void detA71xxxGio1Input(void);

/********************************************************************************************************
  Local variables �ֲ�����
********************************************************************************************************/
#if OS_TMR_EN > 0
    static INT16U OSTmrCtr;
#endif

/*********************************************************************************************************
** Function name:			OSInitHookBegin
** Descriptions:			The hook function called by OSInit() at the beginning of OSInit()
**                          OSInit()�����տ�ʼִ��ʱ���õĹ��Ӻ���    
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
    void OSInitHookBegin (void)
    {
        #if OS_TMR_EN > 0
            OSTmrCtr = 0;
        #endif
    }
#endif


/*********************************************************************************************************
** Function name:			OSInitHookEnd
** Descriptions:			The hook function called by OSInit() at the end of OSInit()
**                          OSInit()��������ǰ���õĹ��Ӻ���    
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
    void OSInitHookEnd (void)
    {
    }
#endif

/*********************************************************************************************************
** Function name:			OSTaskCreateHook
** Descriptions:			The hook function called when a task is created
**                          ��ʼ������ʱ���õĹ��Ӻ���
** Input parameters:		ptcb
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
    void OSTaskCreateHook (OS_TCB  *ptcb)
    {
	    (void)ptcb;                                                     /*  Prevent compiler warning    */
                                                                        /*  ��ֹ���뾯��                */
    }
#endif

/*********************************************************************************************************
** Function name:			OSTaskDelHook
** Descriptions:			The hook function called when a task is deleted
**                          ɾ������ʱ���õĹ��Ӻ���
** Input parameters:		ptcb
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
    void  OSTaskDelHook (OS_TCB  *ptcb)
    {
        (void)ptcb;                                                     /*  Prevent compiler warning    */
                                                                        /*  ��ֹ���뾯��                */
    }
#endif

/*********************************************************************************************************
** Function name:			OSTaskIdleHook
** Descriptions:			The hook function called by the idle task
**                          ����������õĹ��Ӻ���
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
    void OSTaskIdleHook (void)
    {
     }
#endif

/*********************************************************************************************************
** Function name:			OSTaskStatHook
** Descriptions:			The hook function called every second by the statistics task 
**                          ͳ������ÿ���ӵ��õĹ��Ӻ���
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if OS_CPU_HOOKS_EN > 0
    void OSTaskStatHook (void)
    {
    }
#endif

/*********************************************************************************************************
** Function name:			SysTick_Handler
** Descriptions:			ϵͳ���Ķ�ʱ���жϷ�����
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��        
** Created by:				Zhengbailiang
** Created Date:			2011.07.06
**--------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date: 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
static INT32U GuiFeedCount = 0;
static INT32U GuiFeedStartCount = 0;
void GuiFeedCountClr()
{
    OS_ENTER_CRITICAL();                         
    GuiFeedCount = 0;
    OS_EXIT_CRITICAL();
}    
void GuiFeedCountStart()
{
    OS_ENTER_CRITICAL();                         
    GuiFeedStartCount = 1;
    OS_EXIT_CRITICAL();
}    
void  SysTick_Handler(void)
{
    #if OS_CRITICAL_METHOD == 3
        OS_CPU_SR cpu_sr;
    #endif 

    OS_ENTER_CRITICAL();                         
    OSIntNesting++;
	if(GuiFeedStartCount) {
        GuiFeedCount += 5;
        FEED_MASK(GuiFeedCount);
    }
    OS_EXIT_CRITICAL();
    
    uiCounterMs++;
    addZgbTimeCounter(1000 / OS_TICKS_PER_SEC);
    addCarBaseTime(1000 / OS_TICKS_PER_SEC);

   // detA71xxxGio1Input();
	// mdy by hejun 2015-11-03
    OSTimeTick();                                                       /*  Call uC/OS-II's OSTimeTick()*/

    OSIntExit();                                 
}

/*********************************************************************************************************
** Function name:			sysTickInit
** Descriptions:			ϵͳ���Ķ�ʱ����ʼ������
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��        
** Created by:				Zhengbailiang
** Created Date:			2011.07.06
**--------------------------------------------------------------------------------------------------------
** Modified by: 
** Modified date: 
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void  sysTickInit(void)
{
    SysTick_Config(SystemFrequency / OS_TICKS_PER_SEC);
}

/*********************************************************************************************************
** Function name:			OSTaskStkInit
** Descriptions:			This function is called by either OSTaskCreate() or OSTaskCreateExt() to 
**                          initialize the stack frame of the task being created.  
**                          This function is highly processor specific.
**                          OSTaskCreate() �� OSTaskCreateExt()���ñ�������ʼ����������������Ķ�ջ
**                          �������;���Ĵ������й� 
** Input parameters:		task��  A pointer to the task code   ��������ָ��
**                          p_arg�� A pointer to a user supplied data area that will be passed to the task
**                                  when the task first executes.
**                                  �û��ṩ�����ݿռ��ָ�룬��������ʱ�ᱻ���ݸ����� 
**                          ptos:   A pointer to the top of the stack. For this port, it's the highest  
**                                  valid address of the stack. 
**                                  ջ����ָ�롣���ڱ���ֲ����ָ���ջ�������Ч��ַ��
**                          opt:    Specified options that can be used to alter the behavior of 
*                                   OSTaskStkInit().(see uCOS_II.H for OS_TASK_OPT_xxx).
**                                  �ض�ѡ����Ըı��OSTaskStkInit()��ʹ�÷�����
**                                  (�ο� uCOS_II.H �е� OS_TASK_OPT_xxx).
** Output parameters:		None ��
** Returned value:	        The new location of top-of-stack  ��ջ��ָ��λ��         
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
OS_STK *OSTaskStkInit (void  (*task)(void  *parg), void  *parg, OS_STK  *ptos, INT16U  opt)
{
    OS_STK *stk;


    (void)opt;                                                          /*  'opt' is not used, prevent  */
                                                                        /*  warning û����'opt'��       */
                                                                        /*  ��ֹ���뾯��                */
                                                                            
    stk = ptos;                                                         /*  Load stack pointer          */
                                                                        /*  װ�ض�ջָ��                */                                    

                                                                        /*  Registers stacked as if     */
                                                                        /*  auto-saved on exception     */             
	                                                                    /*  ģ����쳣���Զ��ѼĴ���ѹջ*/

    *(stk) = (INT32U)0x01000000L;                                       /*  xPSR                        */ 
    *(--stk) = (INT32U)task;                                            /*  Entry Point of the task     */
                                                                        /*  ������ڵ�ַ                */
    *(--stk) = (INT32U)0xFFFFFFFEL;                                     /*  R14 (LR)  (init value will  */
                                                                        /*  cause fault if ever used)   */
                                                                           
    *(--stk) = (INT32U)0x12121212L;                                     /*  R12                         */
    *(--stk) = (INT32U)0x03030303L;                                     /*  R3                          */
    *(--stk) = (INT32U)0x02020202L;                                     /*  R2                          */
    *(--stk) = (INT32U)0x01010101L;                                     /*  R1                          */
    *(--stk) = (INT32U)parg;                                            /*  R0 : argument  �������     */
                                                                        /*  Remaining registers saved on*/
                                                                        /*  process stack               */   
	                                                                    /*  ʣ�µļĴ������浽��ջ      */

    *(--stk) = (INT32U)0x11111111L;                                     /*  R11                         */
    *(--stk) = (INT32U)0x10101010L;                                     /*  R10                         */
    *(--stk) = (INT32U)0x09090909L;                                     /*  R9                          */
    *(--stk) = (INT32U)0x08080808L;                                     /*  R8                          */
    *(--stk) = (INT32U)0x07070707L;                                     /*  R7                          */
    *(--stk) = (INT32U)0x06060606L;                                     /*  R6                          */
    *(--stk) = (INT32U)0x05050505L;                                     /*  R5                          */
    *(--stk) = (INT32U)0x04040404L;                                     /*  R4                          */

    return(stk);
}

/*********************************************************************************************************
** Function name:			OSTaskSwHook
** Descriptions:			The hook function called when a task switch is performed.
**                          �����л�ʱǰ���õĹ��Ӻ���    
** Input parameters:		None ��
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
void OSTaskSwHook (void)
{
    if (OSPrioHighRdy != OSTCBHighRdy->OSTCBPrio) {
         while (1);
    }
}

/*********************************************************************************************************
** Function name:			OSTCBInitHook
** Descriptions:			The hook function called by OS_TCBInit()
**                          OS_TCBInit()���õĹ��Ӻ���    
** Input parameters:		ptcb
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if (OS_CPU_HOOKS_EN > 0) && (OS_VERSION > 203)
    void OSTCBInitHook (OS_TCB  *ptcb)
    {
        (void)ptcb;                                                     /*  Prevent compiler warning    */
                                                                        /*  ��ֹ���뾯��                */
    }
#endif

/*********************************************************************************************************
** Function name:			OSTimeTickHook
** Descriptions:			The hook function called every tick
**                          ÿ�����ĵ��õĹ��Ӻ���    
** Input parameters:		ptcb
** Output parameters:		None ��
** Returned value:		    None ��
** Created by:				Steven Zhou ���ܸ�
** Created Date:			2007.01.18
**--------------------------------------------------------------------------------------------------------
** Modified by:             Ni Likao ������
** Modified date:           2007.10.29
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#if ((OS_CPU_HOOKS_EN > 0) && (OS_TIME_TICK_HOOK_EN > 0)) || (OS_VERSION < 283)
    void OSTimeTickHook (void)
    {
        #if OS_TMR_EN > 0
	        OSTmrCtr++;
		    if (OSTmrCtr >= (OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC)) {
                OSTmrCtr = 0;
                OSTmrSignal();
		    }
	    #endif
    }
#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/

