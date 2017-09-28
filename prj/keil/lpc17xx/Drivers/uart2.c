/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZZ Technology Co., Ltd.
**
**                                        http://www.wewills.com
**
**      ������־�������Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                          ����������־�������Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart2.c
** Last modified Date:  2015-05-19
** Last Version:        V1.0
** Descriptions:        ���д�����
**
**--------------------------------------------------------------------------------------------------------
** Created by:          liweifeng
** Created date:        2015-05-19
** Version:             V1.0
** Descriptions:        Create This File
**
*********************************************************************************************************/

#include "LPC17xx.h"
#include "uart2.h"
#include "ucos_ii.h"
#include "stdlib.h"
#include "RFNetDrv.h"

/*********************************************************************************************************
  �ⲿ����
*********************************************************************************************************/
extern uint32_t SystemFrequency;

/*********************************************************************************************************
  ȫ�ֱ���
*********************************************************************************************************/
void (*irqCallBack)(void);                                               /* ���ڶ��жϻص�����          */

/*********************************************************************************************************
** Function name:       uart2Init
** Descriptions:        ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ��
**                      ʹ��P2.8->TXD2, P2.9->RXD2��zigbeeͨѶ�������ʲ���115200bps
** input parameters:    �жϴ���ص�����
** output parameters:   ��
** Returned value:      ���� -1������ 0���ɹ�
**********************************************************************************************************/
__ZGB_WEAK_FUN int uart2Init (void (*UART2_IRQHandler)(void))
{   
    uint16_t usFdiv;

    LPC_SC->PCONP |= (1<<24);
    
    LPC_UART2->IER  = 0;
    LPC_PINCON->PINSEL4 |= (0x02 << 16)|(0x02 << 18);                    /* ʹ��P2.8->TXD2, P2.9->RXD2   */

    LPC_UART2->LCR = 0x83;                                             
    usFdiv = (SystemFrequency / 4 / 16) / USART_BPS;                     /* ���ò������USART_BPS         */
    LPC_UART2->DLM = usFdiv / 256;
    LPC_UART2->DLL = usFdiv % 256; 
    LPC_UART2->LCR = 0x03;                                         
	LPC_UART2->FCR = 0x07;
        
#if USART_FIFO_EN
    LPC_UART2->FCR |= 0x80;                                              /* ʹ��FIFO������8���ֽڴ�����  */
#endif

#if USART_IRQ_EN
    if (NULL != UART2_IRQHandler) {
        irqCallBack = UART2_IRQHandler;
        
        NVIC_ClearPendingIRQ(UART2_IRQn);
        NVIC_EnableIRQ(UART2_IRQn);                                      /* ʹ��UART�жϣ����������ȼ�   */
        NVIC_SetPriority(UART2_IRQn, 1);
        LPC_UART2->IER  = 0x01;                                          /* ʹ�ܽ����ж�                 */
    }
#endif

    return 0;
}

/*********************************************************************************************************
** Function name:       usartSendByte
** Descriptions:        �򴮿ڷ���1�ֽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
**********************************************************************************************************/
void usartSendByte (uint8_t  ucDat)
{
    LPC_UART2->THR = ucDat;                                              /*  д������                    */
    while ((LPC_UART2->LSR & 0x40) == 0);                                /*  �ȴ����ݷ������            */
}

/*********************************************************************************************************
** Function name:       usartSendDat
** Descriptions:        �򴮿ڷ���ָ���ֽ�����
** input parameters:    pucDat:   Ҫ���͵�����ָ�� 
**                      ulNum:    Ҫ���͵����ݳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
__ZGB_WEAK_FUN void usartSendDat (uint8_t  const *pucDat,  uint32_t  ulNum)
{
    if(NULL == pucDat) {
        return ;
    }


    while (ulNum--) {
        usartSendByte (*pucDat++);
    }

}

/*********************************************************************************************************
** Function name:       usartSendStr
** Descriptions:        �򴮿ڷ����ַ���
** input parameters:    puiStr:   Ҫ���͵��ַ���ָ�� ulNum:  Ҫ���͵����ݳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void usartSendStr (uint8_t  const *pucStr)
{
    if(NULL == pucStr) {
        return ;
    }
    
    while (*pucStr) {
        usartSendByte (*pucStr++);
    }
}

/*********************************************************************************************************
** Function name:       usartGetByte
** Descriptions:        ��ȡһ���ֽ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ���� -1������ 0���ɹ�
*********************************************************************************************************/
int usartGetByte (unsigned char *dat) 
{
    unsigned int uiCnt = 0;
    
    while ((LPC_UART2->LSR & 0x01) == 0) {                              /* �ȴ����ձ�־��λ             */
#if 0
        OSTimeDly(5 / (1000 / OS_TICKS_PER_SEC));
        uiCnt++;
        if (uiCnt == OS_TICKS_PER_SEC) {
            return -1;
        }
#endif
        ;
    }
    
    *dat = LPC_UART2->RBR;                                              /* ��ȡ����                     */
    return 0;
}

/*********************************************************************************************************
** Function name:       usartGetNByte
** Descriptions:        ��ȡ����ֽ�
** input parameters:    buf�����ݻ���
                        bufLen:����������
                        byteNum:�������ȡ���ݳ���
** output parameters:   ��
** Returned value:      ���� -1������ 0���ɹ�
*********************************************************************************************************/
int usartGetNByte (unsigned char *buf, unsigned int bufLen, unsigned int byteNum)
{
    unsigned int uiCnt = 0;
    int ret = 0;
    
    if (NULL == buf || byteNum > bufLen) {
        return -1;
    }
    
    for (uiCnt = 0; uiCnt < byteNum; uiCnt++) {
        ret = usartGetByte(buf + uiCnt);
        if (ret != 0) {
            return -1;
        }
    }
    
    return 0;
}

/*********************************************************************************************************
* Function Name:        UART_IRQHandler
* Description:          UART�жϷ�����
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void UART2_IRQHandler (void)
{
    irqCallBack();                                                      /*  �ضϷ���ص�                */
}
