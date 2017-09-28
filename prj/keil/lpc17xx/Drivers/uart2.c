/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZZ Technology Co., Ltd.
**
**                                        http://www.wewills.com
**
**      广州众志物联网科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                          ——广州众志物联网科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           uart2.c
** Last modified Date:  2015-05-19
** Last Version:        V1.0
** Descriptions:        队列处理函数
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
  外部函数
*********************************************************************************************************/
extern uint32_t SystemFrequency;

/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
void (*irqCallBack)(void);                                               /* 串口二中断回调函数          */

/*********************************************************************************************************
** Function name:       uart2Init
** Descriptions:        串口初始化，设置为8位数据位，1位停止位，无奇偶校验
**                      使能P2.8->TXD2, P2.9->RXD2与zigbee通讯，波特率采用115200bps
** input parameters:    中断处理回调函数
** output parameters:   无
** Returned value:      返回 -1：有误， 0：成功
**********************************************************************************************************/
__ZGB_WEAK_FUN int uart2Init (void (*UART2_IRQHandler)(void))
{   
    uint16_t usFdiv;

    LPC_SC->PCONP |= (1<<24);
    
    LPC_UART2->IER  = 0;
    LPC_PINCON->PINSEL4 |= (0x02 << 16)|(0x02 << 18);                    /* 使能P2.8->TXD2, P2.9->RXD2   */

    LPC_UART2->LCR = 0x83;                                             
    usFdiv = (SystemFrequency / 4 / 16) / USART_BPS;                     /* 设置波特率蜺SART_BPS         */
    LPC_UART2->DLM = usFdiv / 256;
    LPC_UART2->DLL = usFdiv % 256; 
    LPC_UART2->LCR = 0x03;                                         
	LPC_UART2->FCR = 0x07;
        
#if USART_FIFO_EN
    LPC_UART2->FCR |= 0x80;                                              /* 使能FIFO，设置8个字节触发点  */
#endif

#if USART_IRQ_EN
    if (NULL != UART2_IRQHandler) {
        irqCallBack = UART2_IRQHandler;
        
        NVIC_ClearPendingIRQ(UART2_IRQn);
        NVIC_EnableIRQ(UART2_IRQn);                                      /* 使能UART中断，并配置优先级   */
        NVIC_SetPriority(UART2_IRQn, 1);
        LPC_UART2->IER  = 0x01;                                          /* 使能接收中断                 */
    }
#endif

    return 0;
}

/*********************************************************************************************************
** Function name:       usartSendByte
** Descriptions:        向串口发送1字节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
**********************************************************************************************************/
void usartSendByte (uint8_t  ucDat)
{
    LPC_UART2->THR = ucDat;                                              /*  写入数据                    */
    while ((LPC_UART2->LSR & 0x40) == 0);                                /*  等待数据发送完毕            */
}

/*********************************************************************************************************
** Function name:       usartSendDat
** Descriptions:        向串口发送指定字节数据
** input parameters:    pucDat:   要发送的数据指针 
**                      ulNum:    要发送的数据长度
** output parameters:   无
** Returned value:      无
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
** Descriptions:        向串口发送字符串
** input parameters:    puiStr:   要发送的字符串指针 ulNum:  要发送的数据长度
** output parameters:   无
** Returned value:      无
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
** Descriptions:        获取一个字节
** input parameters:    无
** output parameters:   无
** Returned value:      返回 -1：有误， 0：成功
*********************************************************************************************************/
int usartGetByte (unsigned char *dat) 
{
    unsigned int uiCnt = 0;
    
    while ((LPC_UART2->LSR & 0x01) == 0) {                              /* 等待接收标志置位             */
#if 0
        OSTimeDly(5 / (1000 / OS_TICKS_PER_SEC));
        uiCnt++;
        if (uiCnt == OS_TICKS_PER_SEC) {
            return -1;
        }
#endif
        ;
    }
    
    *dat = LPC_UART2->RBR;                                              /* 读取数据                     */
    return 0;
}

/*********************************************************************************************************
** Function name:       usartGetNByte
** Descriptions:        获取多个字节
** input parameters:    buf：数据缓冲
                        bufLen:缓冲区长度
                        byteNum:请求的提取数据长度
** output parameters:   无
** Returned value:      返回 -1：有误， 0：成功
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
* Description:          UART中断服务函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void UART2_IRQHandler (void)
{
    irqCallBack();                                                      /*  回断服务回调                */
}
