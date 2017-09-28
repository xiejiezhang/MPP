/****************************************Copyright(C),2017,xjz***************************************
** 
**  The software is very welcome for learning, teaching, research. 
**  If you are used for business, please tell me.
** 
**  http://my.csdn.net/sdewenking
**  xiejiezhang@gmail.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           mcrc.h
** Last modified Date:  2017-09-06
** Last Version:        V1.0
** Descriptions:        CRC check declaration
**
**--------------------------------------------------------------------------------------------------------
** Created by:          xjz
** Created date:        2017-09-06
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
#include "std.h"

#ifndef __MCRC_H
#define __MCRC_H

/**
 * \brief CRC32 check algorithm
 * \param[in] crc : Initial value
 * \param[in] pc  : bytes to be calculated
 * \param[in] len : The length of the byte to be calculated
 * \return   the result of crc32
 */
uint32_t mcrc32(uint32_t crc, const uint8_t *pc , uint32_t len);


#endif /* __MCRC_H */