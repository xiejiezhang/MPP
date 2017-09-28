/****************************************Copyright(C),2017,xjz***************************************
** 
**  The software is very welcome for learning, teaching, research. 
**  If you are used for business, please tell me.
** 
**  http://my.csdn.net/sdewenking
**  xiejiezhang@gmail.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           bootloader.c
** Last modified Date:  2017-09-25
** Last Version:         V1.0
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          xjz
** Created date:        2017-09-28
** Version:             V1.00
** Descriptions:        Create the file
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
*********************************************************************************************************/
#include "bootloader.h"

static bootDrv_t gbootDrv;

/**
 * Mode 1: The program contains only the bootloader and APP1
 */
void mode1UpgradeAndRun(void *p_arg)
{
    unsigned char cmd = gbootDrv.pfnBootCmdGet(p_arg);
    
    if ( cmd == REQ_UPGRADE) {
        gbootDrv.pfnBootCmdSet(p_arg, REQ_CLRCMD);
    } else {
        gbootDrv.pfnBootCmdSet(p_arg, REQ_UPGRADE);
        if (gbootDrv.pfnApp1Check(p_arg)) {
            gbootDrv.pfnApp1Run(p_arg);
        }
    }
}

/**
 * Mode 2: The program contains only bootloader and APP1, 
 * APP2 three parts, APP2 can be used as backup of APP1
 */
void mode2UpgradeAndRun(void *p_arg)
{
    unsigned char cmd = gbootDrv.pfnBootCmdGet(p_arg);
    
    if ( cmd == REQ_UPGRADE) {
        gbootDrv.pfnBootCmdSet(p_arg, REQ_CLRCMD);
    } else {
       if (cmd == REQ_RUNAPP2) {
            if (gbootDrv.pfnApp2Check(p_arg)) {
                gbootDrv.pfnBootCmdSet(p_arg, REQ_UPGRADE);
                gbootDrv.pfnApp2Run(p_arg);
            }
        } else {
            gbootDrv.pfnBootCmdSet(p_arg, REQ_UPGRADE);
            if (gbootDrv.pfnApp1Check(p_arg)) {
                gbootDrv.pfnBootCmdSet(p_arg, REQ_RUNAPP2);
                gbootDrv.pfnApp1Run(p_arg);
            } else {
                gbootDrv.pfnBootCmdSet(p_arg, REQ_UPGRADE);
                if (gbootDrv.pfnApp2Check(p_arg)) {
                    gbootDrv.pfnBootCmdSet(p_arg, REQ_UPGRADE);
                    gbootDrv.pfnApp2Run(p_arg);
                }
            }
        }
    }
}

typedef void (*pfnUpgradeCheck_t)(void *);

/**
 * Initialize the bootloader driver
 */
pfnUpgradeCheck_t bootDrvInit(bootDrv_t *pBootDrv)
{
    if (pBootDrv->pfnApp2Run && pBootDrv->pfnApp2Check) {
        return mode2UpgradeAndRun;
    } else {
        return mode1UpgradeAndRun;
    }
}

void bootloaderRun(void *p_arg, bootDrv_t *pBootDrv)
{
    pfnUpgradeCheck_t pfnRun;

    pfnRun = bootDrvInit(pBootDrv);
    pfnRun(p_arg);
}