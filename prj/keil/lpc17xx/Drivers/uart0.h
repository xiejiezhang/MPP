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
** File name:           uart2.h
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
#ifndef __USART0_H
#define __USART0_H


#include "stdint.h"                                                   

/*********************************************************************************************************
  宏定义
*********************************************************************************************************/
#define    USART_IRQ_EN              1                                  /* 是否开启串口中断             */
#define    USART_FIFO_EN             0                                  /* 是否开启FIFO功能(触点为8)    */
#define    USART_BPS                 115200                             /* 串口通信波特率               */


/*********************************************************************************************************
  类型定义
*********************************************************************************************************/

/*********************************************************************************************************
  全局变量
*********************************************************************************************************/

/*********************************************************************************************************
  对外链接函数
*********************************************************************************************************/
/*********************************************************************************************************
** Function name:       uart2Init
** Descriptions:        串口初始化，设置为8位数据位，1位停止位，无奇偶校验
**                      使能P2.8->TXD2, P2.9->RXD2与zigbee通讯，波特率采用115200bps
** input parameters:    无
** output parameters:   无
** Returned value:      返回 -1：有误， 0：成功
**********************************************************************************************************/
extern int uart2Init (void (*UART2_IRQHandler)(void));

/*********************************************************************************************************
** Function name:       usartSendByte
** Descriptions:        向串口发送子节数据，并等待数据发送完成，使用查询方式
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usartSendByte (uint8_t  ucDat);

/*********************************************************************************************************
** Function name:       usartSendDat
** Descriptions:        向串口发送数据流
** input parameters:    pucDat:   要发送的数据指针 
**                      ulNum:    要发送的数据长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usartSendDat (uint8_t  const *pucDat,  uint32_t  ulNum);

/*********************************************************************************************************
** Function name:       usartSendStr
** Descriptions:        向串口发送字符串
** input parameters:    puiStr:   要发送的字符串指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usartSendStr (uint8_t  const *pucStr);

#if USART_IRQ_EN

/*********************************************************************************************************
* Function Name:        UART_IRQHandler
* Description:          UART中断服务函数
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
extern void UART2_IRQHandler (void);

#else

/*********************************************************************************************************
** Function name:       usartGetByte
** Descriptions:        从串口接收1字节数据，使用查询方式接收
** input parameters:    无
** output parameters:   无
** Returned value:      ucRcvData:   接收到的数据
*********************************************************************************************************/
extern uint8_t usartGetByte (void);

/*********************************************************************************************************
** Function name:       usartGetStr
** Descriptions:        串口接收字符串
** input parameters:    pucStr:  指向接收数据数组的指针
**                      ulNum:   接收数据的个数
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern void usartGetStr (uint8_t  *pucStr, uint32_t  ulNum);

#endif


/*********************************************************************************************************
** Function name:       usartGetByte
** Descriptions:        获取一个字节
** input parameters:    无
** output parameters:   无
** Returned value:      一个字节数据
*********************************************************************************************************/
extern int usartGetByte(unsigned char *dat);

/*********************************************************************************************************
** Function name:       usartGetNByte
** Descriptions:        获取多个字节
** input parameters:    buf：数据缓冲
                        bufLen:缓冲区长度
                        byteNum:请求的提取数据长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
extern int usartGetNByte (unsigned char *buf, unsigned int bufLen, unsigned int byteNum);

#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
