;****************************************Copyright (c)****************************************************
;**                               Guangzou ZLG-MCU Development Co.,LTD.
;**                                      graduate school
;**                                 http://www.zlgmcu.com
;**
;**--------------File Info--------------------------------------------------------------------------------
;** File name:                 os_cpu_a.asm
;** Last modified Date:         2007.01.18
;** Last Version:               1.0
;** Descriptions:               The assembly functions that a uC/OS-II port requires
;**                             uC/OS-II��ֲ����Ҫ�Ļ�ຯ��  
;**-------------------------------------------------------------------------------------------------------
;** Created By:                 Steven Zhou ���ܸ�
;** Created date:               2007.01.18
;** Version:                    1.0
;** Descriptions:               The original version
;**
;**-------------------------------------------------------------------------------------------------------
;** Modified by:                Zhengbailiang
;** Modified date:              2011.07.06
;** Version:                    1.1
;** Description:                The second version �ڶ���
;**
;*********************************************************************************************************

;*********************************************************************************************************
;  Declarations for the interrupt handlers that are used by the application.              
;  ��Ӧ�����õ�������                                       
;*********************************************************************************************************
                EXTERN  OSRunning                
                EXTERN  OSPrioCur
                EXTERN  OSPrioHighRdy
                EXTERN  OSTCBCur
                EXTERN  OSTCBHighRdy
                EXTERN  OSIntNesting
                EXTERN  OSIntExit
                EXTERN  OSTaskSwHook
                EXTERN  OsEnterSum
                IMPORT  sysTickInit


;*********************************************************************************************************
;  Declarations for the exported functions             
;  ����ⲿ����                          
;*********************************************************************************************************
                EXPORT  OSStartHighRdy
                EXPORT  OSCtxSw
                EXPORT  OSIntCtxSw
                EXPORT  PendSV_Handler
                EXPORT  OS_ENTER_CRITICAL
                EXPORT  OS_EXIT_CRITICAL                  
                EXPORT  intDisAll

;*********************************************************************************************************
;  Registers or macros used by this file                                
;  ���ļ��õ��ļĴ����ͺ�                             
;*********************************************************************************************************                                
NVIC_INT_CTRL           EQU     0xE000ED04                              ;  Interrupt control state       PEND�����Ĵ���
                                                                        ;  register.
                                                                        ;  �жϿ��ƼĴ���

NVIC_SYSPRI2            EQU     0xE000ED20                              ;  System priority register (2)  ϵͳ������PEND���ȼ�
                                                                        ;  ϵͳ���ȼ��Ĵ�����2��

NVIC_PENDSV_PRI         EQU     0xFFFF0000                              ;  PendSV and system tick priority   ����ϵͳ�������ȼ����
                                                                        ;  (Both are lowest,0xff)  
                                                                        ;  ����жϺ�ϵͳ�����ж�
                                                                        ;  ����Ϊ��ͣ�0xff��.
NVIC_PENDSVSET          EQU     0x10000000                              ;  Value to trigger PendSV      PEND�����Ĵ�����Ч����ֵ
                                                                        ;  exception.��������жϵ�ֵ.

OS_CRITICAL_INT_PRIO    EQU     (0 << 0)                                ;  Critical setions manage prio. 
                                                                        ; (equal or bigger number managed) 
                                                                        ;  �ٽ�����������ȼ�����ֵ����
        PRESERVE8                                                       ;  ����ڱ�����
             
        AREA    UCOSII_ASM, CODE, READONLY
        THUMB

;*********************************************************************************************************
;** Function name:          OS_ENTER_CRITICAL
;** Descriptions:           Enter the critical section  �����ٽ���                         
;** Input parameters:       None ��
;** Output parameters:      None ��
;** Returned value:         None ��
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.01.18
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            Ni Likao ������
;** Modified date:          2007.11.02
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
OS_ENTER_CRITICAL
    IF OS_CRITICAL_INT_PRIO > 0
        PUSH    {R1,R2}                     
        LDR     R1, =OS_CRITICAL_INT_PRIO                               ;  Disable priorities between 
                                                                        ;  OS_CRITICAL_INT_PRIO & 0xFF 
                                                                                                          
        MSR     BASEPRI,  R1                                            ;  �������ȼ���OS_CRITICAL_INT 
                                                                        ;  _PRIO��0xFF���ж�                                    
    ELSE
        CPSID   I                                                       ;  Disable all the interrupts
                                                                        ;  ���������ж�
        PUSH    {R1,R2}      
    ENDIF
    
        LDR     R1, =OsEnterSum                                         ;  OsEnterSum++
        LDRB    R2, [R1]
        ADD     R2, R2, #1
        STRB    R2, [R1]
        POP     {R1,R2}
        BX      LR


;*********************************************************************************************************
;** Function name:          OS_EXIT_CRITICAL
;** Descriptions:           Exit the critical section  �˳��ٽ���
;** Output parameters:      None ��
;** Input parameters:       None ��
;** Returned value:         None ��
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.01.18
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            Ni Likao ������
;** Modified date:          2007.11.02
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
OS_EXIT_CRITICAL
        PUSH    {R1, R2}
        LDR     R1, =OsEnterSum                                         ;  OsEnterSum--
        LDRB    R2, [R1]
        SUB     R2, R2, #1
        STRB    R2, [R1]

        MOV     R1,  #0          
        CMP     R2,  #0                                                 ;  if OsEnterSum=0,enable 
                                                                        ;  interrupts���OsEnterSum=0��
    IF OS_CRITICAL_INT_PRIO > 0                                         ;  ���ж�
        IT EQ
		MSREQ   BASEPRI, R1                                             ;  �������0 ���򲻹ر��κ��ж�
    ELSE
        IT EQ
		MSREQ   PRIMASK, R1                                             ;  CPSIE I  ;PRIMASK=0��  ;���ж�
    ENDIF
        POP     {R1, R2}
        BX LR


;*********************************************************************************************************
;** Function name:          OSStartHighRdy
;** Descriptions:           Uses the context switcher  to cause the first task to start.
;**                         ʹ�õ��������е�һ������   
;** Input parameters:       None ��
;** Output parameters:      None ��
;** Returned value:         None ��
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.01.18
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            Ni Likao ������
;** Modified date:          2007.11.02
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
OSStartHighRdy

        LDR     R4, =NVIC_SYSPRI2                                       ;  set the PendSV exception 
                                                                        ;  priority����PendSV���ȼ�
        LDR     R5, =NVIC_PENDSV_PRI
        STR     R5, [R4]                                                ;  ����  ϵͳ������PENDSV���ȼ�Ϊ���->32
        
        MOV     R4, #0                                                  ;  set the PSP to 0 for initial  
                                                                        ;  context switch call ʹPSP����0
        MSR     PSP, R4

        LDR     R4, =OSRunning                                          ;  OSRunning = TRUE
        MOV     R5, #1
        STRB    R5, [R4]

        LDR     R4, =NVIC_INT_CTRL                                      ;  trigger the PendSV exception
                                                                        ;  ��������ж�
        LDR     R5, =NVIC_PENDSVSET
        STR     R5, [R4]

        CPSIE   I                                                       ;  enable interrupts at processor 
                                                                        ;  levelʹ���������ȼ����ж�
OSStartHang
        B       OSStartHang             


;*********************************************************************************************************
;** Function name:          OSCtxSw
;** Descriptions:           Perform a contex switch from task level  �����������л�                          
;** Input parameters:       None ��
;** Output parameters:      None ��
;** Returned value:         None ��        
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.01.18
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            Ni Likao ������
;** Modified date:          2007.11.02
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
OSCtxSw
        PUSH    {R4, R5}
        LDR     R4, =NVIC_INT_CTRL                                      ;  trigger the PendSV exception
                                                                        ;  ��������ж�
        LDR     R5, =NVIC_PENDSVSET
        STR     R5, [R4]
        POP     {R4, R5}
        BX      LR


;*********************************************************************************************************
;** Function name:          OSIntCtxSw
;** Descriptions:           called by OSIntExit() when it determines a context switch is needed as the
;**                         result of an interrupt.
;**                         �жϼ������л�                 
;** Input parameters:       None ��
;** Output parameters:      None ��
;** Returned value:         None ��
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.01.18
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            Ni Likao ������
;** Modified date:          2007.11.02
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
OSIntCtxSw
        PUSH    {R4, R5}
        LDR     R4, =NVIC_INT_CTRL                                      ;  trigger the PendSV exception
                                                                        ;  ��������ж�
        LDR     R5, =NVIC_PENDSVSET
        STR     R5, [R4]
        POP     {R4, R5}
        BX      LR
        NOP


;*********************************************************************************************************
;** Function name:          OSPendSV
;** Descriptions:           Used to cause a context switch �����������л�
;** Input parameters:       None ��
;** Output parameters:      None ��
;** Returned value:         None ��
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.01.18
;**-------------------------------------------------------------------------------------------------------
;** Modified by:            ֣����
;** Modified date:          2011.07.06
;** Descriptions:           �����˵�һ��ʱ��ʼ��ϵͳ���Ķ�ʱ��
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
PendSV_Handler
    IF OS_CRITICAL_INT_PRIO > 0                                     ;  disable interupt �����ж�
        MRS     R3, BASEPRI
        LDR     R1, =OS_CRITICAL_INT_PRIO
        MSR     BASEPRI, R1
    ELSE
        MRS     R3, PRIMASK                
        CPSID   I
    ENDIF


        MRS     R0, PSP                                                 ;  PSP is process stack pointer  
                                                                        ;  PSP������Ķ�ջָ��
        CBZ     R0, OSPendSV_sysTickInit                                ;  skip register save the first 
                                                                        ;  time��һ���������沢��ʼ��ϵͳ����

        LDR     R1, =OSPrioCur                                          ;  OSPrioCur = OSPrioHighRdy
        LDR     R2, =OSPrioHighRdy     
        LDRB    R1, [R1]
        LDRB    R2, [R2]
        CMP     R1, R2
        BEQ     OSPendSV_Exit

        SUB     R0, R0, #0x20                                           ;  save remaining regs r4-11 on 
                                                                        ;  process stack ����r4-r11
        STM     R0, {R4-R11}

        LDR     R4, =OSTCBCur                                           ;  OSTCBCur->OSTCBStkPtr = SP;
        LDR     R4, [R4]
        STR     R0, [R4]                                                ;  R0 is SP of process being 
                                                                        ;  switched outR0�Ǳ��л���������
                                                                        ;  �Ķ�ջָ��
OSPendSV_nosave
        PUSH    {R14}                                                   ;  need to save LR exc_return 
                                                                        ;  value����LR����ֵ

        LDR     R0, =OSTaskSwHook                                       ;  OSTaskSwHook();
        BLX     R0
        POP     {R14}
       
        LDR     R4, =OSPrioCur                                          ;  OSPrioCur = OSPrioHighRdy
        LDR     R5, =OSPrioHighRdy     
        LDRB    R6, [R5]
        STRB    R6, [R4]

        LDR     R4, =OSTCBCur                                           ;  OSTCBCur  = OSTCBHighRdy;
        LDR     R6, =OSTCBHighRdy      
        LDR     R6, [R6]
        STR     R6, [R4]


        LDR     R0, [R6]                                                ;  SP = OSTCBHighRdy->OSTCBStkPtr;
        LDM     R0, {R4-R11}                                            ;  restore r4-11 from new process
                                                                        ;  stack ��������Ķ�ջ�ָ�r4-r11
        ADD     R0, R0, #0x20
        MSR     PSP, R0                                                 ;  load PSP with new process SP
                                                                        ;  ��������Ķ�ջ�ָ�PSP
            
        ORR     LR, LR, #0x04                                           ;  ensure exception return uses 
                                                                        ;  PSPȷ�����غ�ʹ��PSP
        
OSPendSV_Exit
        IF OS_CRITICAL_INT_PRIO > 0                                     ;  restore interrupts �ָ��ж�  
            MSR     BASEPRI,  R3
        ELSE
            MSR     PRIMASK, R3
        ENDIF
           
        BX      LR                                                      ; exception return will restore 
                                                                        ;  remaining context 
                                                                        ;  ����ʱ��ָ�ʣ�µ�������

OSPendSV_sysTickInit
        PUSH    {R14}
        BL      sysTickInit                                             ;  ��ʼ��ϵͳ���Ķ�ʱ��
        POP     {R14}
        B       OSPendSV_nosave
        NOP

 
;*********************************************************************************************************
;** Function name:          IntDisAll
;** Descriptions:           Disable all interrupts from the interrupt controller  �ر��жϿ������������ж�
;** Input parameters:       None ��
;** Output parameters:      None ��
;** Returned value:         None ��
;** Created by:             Steven Zhou ���ܸ�
;** Created Date:           2007.12.12
;**-------------------------------------------------------------------------------------------------------
;** Modified by:         
;** Modified date:       
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************
intDisAll
    CPSID   I
    BX      LR

    ALIGN


    END
;*********************************************************************************************************
;  END FILE                                               
;*********************************************************************************************************
