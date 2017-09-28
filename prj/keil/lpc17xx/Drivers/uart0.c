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
** File name:           uart0.c
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
#include "uart0.h"
//#include "ucos_ii.h"
#include "stdlib.h"


/*********************************************************************************************************
  外部函数
*********************************************************************************************************/
extern uint32_t SystemFrequency;

/*********************************************************************************************************
  全局变量
*********************************************************************************************************/
void (*irqCallBack)(void);                                               /* 串口二中断回调函数          */

/*********************************************************************************************************
** Function name:       uart0Init
** Descriptions:        串口初始化，设置为8位数据位，1位停止位，无奇偶校验
**                      使能P2.8->TXD2, P2.9->RXD2与zigbee通讯，波特率采用115200bps
** input parameters:    中断处理回调函数
** output parameters:   无
** Returned value:      返回 -1：有误， 0：成功
**********************************************************************************************************/
int uart0Init (void (*UART0_IRQHandler)(void))
{   
    uint16_t usFdiv;
    LPC_PINCON->PINSEL0 |= (0x01 << 4)|(0x01 << 6); 

    LPC_SC->PCONP = LPC_SC->PCONP|0x08;                                 /* ????0??                */
    LPC_UART0->LCR  = 0x83;                                             /* ???????               */
    usFdiv = (SystemFrequency / 4 / 16) / USART_BPS;                    /* ?????                   */
    LPC_UART0->DLM  = usFdiv / 256;
    LPC_UART0->DLL  = usFdiv % 256; 
    LPC_UART0->LCR  = 0x03;                                             /* ?????                   */
    LPC_UART0->FCR  = 0x06;

    NVIC_EnableIRQ(UART0_IRQn);                                         /* ??UART??,??????   */
    NVIC_SetPriority(UART0_IRQn, 1);
    LPC_UART0->IER  = 0x01;                                             /* ??????                 */

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
    LPC_UART0->THR = ucDat;                                              /*  写入数据                    */
    while ((LPC_UART0->LSR & 0x40) == 0);                                /*  等待数据发送完毕            */
}

/*********************************************************************************************************
** Function name:       usartSendDat
** Descriptions:        向串口发送指定字节数据
** input parameters:    pucDat:   要发送的数据指针 
**                      ulNum:    要发送的数据长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void usartSendDat (uint8_t  const *pucDat,  uint32_t  ulNum)
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
    
    while ((LPC_UART0->LSR & 0x01) == 0) {                              /* 等待接收标志置位             */
#if 0
        OSTimeDly(5 / (1000 / OS_TICKS_PER_SEC));
        uiCnt++;
        if (uiCnt == OS_TICKS_PER_SEC) {
            return -1;
        }
#endif
        ;
    }
    
    *dat = LPC_UART0->RBR;                                              /* 读取数据                     */
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
void UART0_IRQHandler (void)
{
    extern void xmodemOneCharRecv(char id, unsigned char c);
    while ((LPC_UART0->IIR & 0x01) == 0) {                               /* ?????????          */
        switch (LPC_UART0->IIR & 0x0E) {                                 /* ??????                */
            case 0x04:                                                   /* RDA??????             */
            case 0x0C:                                                   /* CTI??????             */
                while ((LPC_UART0->LSR & 0x01) == 0x01) {                /* ??????????        */
                    xmodemOneCharRecv(0, (unsigned char)LPC_UART0->RBR);           /* ??????????        */
                }
                break;
            case 0x02:                                                   /* THRE??FIFO????         */
                break;
            default:
                break;
        }
    }
}
