#include "xmodem_r.h"
#include "uart0.h"
#include "lpc17xx.h"

static unsigned char gMdevBuf[1024];
static unsigned int  gMdevCnt = 0;
/*********************************************************************************************************
  �ⲿ����
*********************************************************************************************************/
extern uint32_t SystemFrequency;
#define TRANS_DELAY_UNIE              (6)                       /* ת����ʱ��λ  ms   */               

#define LED_RED (1<<14)
#define LED_RED_OFF()       LPC_GPIO1->FIOSET |= LED_RED
#define LED_RED_ON()        LPC_GPIO1->FIOCLR |= LED_RED

void LedFlash(void)
{
    LPC_PINCON->PINSEL2 &= ~(0x3 << 28);                                /* ��P0.14��ʼ��ΪGPIO����      */
    LPC_GPIO1->FIODIR    |= LED_RED;                                    /* ��P0.14��������Ϊ���        */
    LPC_GPIO1->FIOSET    |= LED_RED;                                    /* ��P0.14��ʼ������ߵ�ƽ      */
}

void timer0Init (void)
{
    LPC_SC->PCONP  |= 1 << 1;                                           /* �򿪶�ʱ��0�Ĺ��ʿ���λ      */
    
    LPC_TIM0->TCR  = 0x02;
    LPC_TIM0->IR   = 1;
    LPC_TIM0->CTCR = 0;
    LPC_TIM0->TC   = 0;
    LPC_TIM0->PR   = 0;
    LPC_TIM0->MR0  = SystemFrequency / 4 / (1000 / TRANS_DELAY_UNIE) -1;/* TRANS_DELAY_UNIE�ж�1��      */
    LPC_TIM0->MCR  = 0x03;                                              /* ƥ�������ж�               */
    
    NVIC_EnableIRQ(TIMER0_IRQn);                                        /* �����жϲ�ʹ��               */
    NVIC_SetPriority(TIMER0_IRQn, 3);
    LPC_TIM0->TCR  = 0x01;                                              /* ������ʱ��                   */
}

static uint32_t gledCnt = 0, flag;

void TIMER0_IRQHandler (void)
{
    
    LPC_TIM0->IR         = 0x01;                                        /* ����жϱ�־                 */
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
