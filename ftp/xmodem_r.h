/****************************************Copyright(C),2017,xjz***************************************
** 
**  The software is very welcome for learning, teaching, research. 
**  If you are used for business, please tell me.
** 
**  http://my.csdn.net/sdewenking
**  xiejiezhang@gmail.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           xmodem_r.h
** Last modified Date:  2017-09-25
** Last Version:         V1.0
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          xjz
** Created date:        2017-09-25
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
#ifndef __XMODEM_R_H
#define __XMODEM_R_H


#define XMD_DEV_MAX_NUM   1  

#define XMD_TYPE_CRC  ('C')
#define XMD_TYPE_SUM  (0x15)


typedef void (*pfnDatBytesSend_t)(const unsigned char *pdata, unsigned int tlen);

/**
 * sendHandle : Character transmission interface
 * ptiotMs : pointer of timeout param, tiot add one per macrosecond
 * pthBuf: Receive byte of data buffer, 
 *         must be Not less than 128 bytes when 128 TYPE,Not less than 1k bytes when 1K TYPE
 *
 * \note The input reference for this interface requires the user's own implementation
 */
void xmodemConfig(char id,
                  pfnDatBytesSend_t sendHandle,
                  unsigned char *pthBuf,
                  unsigned int  *ptiotMs);


typedef char (*pfnDatRecvCb_t)(unsigned char *pdata, unsigned int rlen);
typedef char (*pfnRecvDone_t)(void *p_arg);
/**
 * Start a transfer task
 * datHandle : Data processing handle, 
 *             The function is executed when the number of bytes received reaches the set threshold
 *             Once the callback is executed, the buffer will be cleared
 * threshole : Receive callback threshold
 * 
 * \note If an error occurs during transmission or overtime, the task is terminated.
 *       Re-accept the need to recall the function.
 */
 void xmodemRecvTaskStart(char id,
                          pfnRecvDone_t  pfnRecvDone,
                          pfnDatRecvCb_t datHandle,
                          unsigned int   threshole);


/**
 * the character receives the function,
 * call it when receive one char
 * example: puts it on the character to receive the interrupt
 */
void xmodemOneCharRecv(char id,unsigned char c);


/**
 * Restart the receiving process
 */
void xmodemRestart(char id);

#endif /* __XMODEML_R_H */
