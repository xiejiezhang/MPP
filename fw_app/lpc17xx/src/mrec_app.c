#include "xmodem_r.h"
#include "uart0.h"
#include "lpc17xx.h"

static unsigned char gMdevBuf[1024];
static unsigned int  gMdevCnt = 0;
/*********************************************************************************************************
  外部函数
*********************************************************************************************************/
extern uint32_t SystemFrequency;
#define TRANS_DELAY_UNIE              (6)                       /* 转发延时单位  ms   */               

#define LED_RED (1<<14)
#define LED_RED_OFF()       LPC_GPIO1->FIOSET |= LED_RED
#define LED_RED_ON()        LPC_GPIO1->FIOCLR |= LED_RED

void LedFlash(void)
{
    LPC_PINCON->PINSEL2 &= ~(0x3 << 28);                                /* 将P0.14初始化为GPIO功能      */
    LPC_GPIO1->FIODIR    |= LED_RED;                                    /* 将P0.14方向设置为输出        */
    LPC_GPIO1->FIOSET    |= LED_RED;                                    /* 将P0.14初始化输出高电平      */
}

void timer0Init (void)
{
    LPC_SC->PCONP  |= 1 << 1;                                           /* 打开定时器0的功率控制位      */
    
    LPC_TIM0->TCR  = 0x02;
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;
    LPC_TIM0->MR0  = SystemFrequency / 4 / (1000 / TRANS_DELAY_UNIE) -1;/* TRANS_DELAY_UNIE中断1次      */
    LPC_TIM0->MCR  = 0x03;                                              /* 匹配后产生中断               */
    
    NVIC_EnableIRQ(TIMER0_IRQn);                                        /* 设置中断并使能               */
    NVIC_SetPriority(TIMER0_IRQn, 3);
    LPC_TIM0->TCR  = 0x01;                                              /* 启动定时器                   */
}

static uint32_t gledCnt = 0, flag;

void TIMER0_IRQHandler (void)
{
    
    LPC_TIM0->IR         = 0x01;                                        /* 清除中断标志                 */
    gMdevCnt += TRANS_DELAY_UNIE;
    gledCnt += TRANS_DELAY_UNIE;
    if (gledCnt >= 1000) {
        gledCnt= 0;
        if(flag) {
           LED_RED_ON();
        } else {
           LED_RED_OFF();
        }
        flag = !flag;
    }
}

char dataHandle(unsigned char *pdat, unsigned int len)
{
    return 0;
}


void mrecInit()
{
    LedFlash();
    timer0Init();
    uart0Init();
    usartSendDat("Start mrecInit\r\n", 16);
    xmodemConfig(0, usartSendDat, gMdevBuf, &gMdevCnt);
    
    xmodemRecvTaskStart(0, 0, dataHandle, 1024);
}
