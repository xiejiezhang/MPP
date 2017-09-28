#include "xmodem_r.h"
#include "lpc17xx.h"

extern void SystemInit (void);


int main(void)
{
    /**
     *
     */
    extern void mrecInit();
    extern void xmodelRecvProcess(void *p_arg);
    
    
    SystemInit();
    
    
    mrecInit() ;
    while (1) {
       xmodelRecvProcess(0);
    }
    return 0;
}