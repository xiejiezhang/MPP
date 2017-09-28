/****************************************Copyright(C),2017,xjz***************************************
** 
**  The software is very welcome for learning, teaching, research. 
**  If you are used for business, please tell me.
** 
**  http://my.csdn.net/sdewenking
**  xiejiezhang@gmail.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           mdev.h
** Last modified Date:  2017-09-05
** Last Version:        V1.0
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          xjz
** Created date:        2017-09-05
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

#ifndef __MDEV_H 
#define __MDEV_H

/**
 * \brief The module manager manages all the modules on the device.
 */
 typedef struct __mMod {
    /**
     * Point to the next module manager
     */
    struct __mMod *next;

    /**
     * Points to the first address of the module file store
     * 0 indicates that the module is invalid
     */
    uint32_t modBaseAddr;

    /**
     * Describes the module disable enable, as well as the operation
     */
#define MRF_EN_MASK  (0x01)

    uint8_t  modRunFlag;

 }mMod_t;



/**
 * \brief The Device Manager monitors the status of the entire device, 
 * including storage information and module registration.
 */
 typedef struct __mdev {
     /*
    * The free space of the current device (not limited to Flash and RAM),
    * which is used to store module firmware data
    */
    uint32_t unusedCodeSize;   
    

     /**
    * The available program space address for the current device
    */
    uint32_t unusedCodeAddrBase;


     /**
    * The total program space of the device
    */
    uint32_t totalCodeSize;

     /*
    * The available data bytes for the current device
    */
    uint32_t unusedRamSize;   
    

     /**
    * The available data space address for the current device
    */
    uint32_t unusedRamAddrBase;


     /**
    *  The total data space of the device
    */
    uint32_t totalRamSize;


    /**
    * Module storage space occupied by the smallest, can be understood as Flash sector
    * \note this value must be 2 n th power
    */
    uint32_t smallestCodeUnit;

    /**
    * The number of modules that have been registered for the device, 
    * including modules that have been disabled
    */
    uint32_t numberOfRegisteredMod;

    /**
    * The maximum number of modules allowed to register
    */
    uint32_t maxNumberMod;

    /**
    * Pointer to the module manager list
    */
    struct __mMod *   mModpointer;
 }mdev_t;


/**
 * \brief The unified module's file format makes it easier for the framework
 *        to identify modules (stored in small end format).
 */
 typedef struct __modFileHeader {

    /**
    * Fixed to 0xE1301ECA
    */
#define MFH_IDENTIFIER  (0xE1301ECA)
    uint32_t identifier;


    /**
    * The address number of the module
    */
    uint16_t id;

    /**
    * Store the module disable enable flag
    */
    uint16_t flag;

    /**
    * The name of modules
    * stored in string format, not enough to fill zero
    */
#define MFH_NAME_MAX_CHAHS 16
    char name[MFH_NAME_MAX_CHAHS];


    /**
    * Module entry address
    */
    uint32_t entryAddr;

    /**
    * The total number of bytes of the module firmware includes the file format header bytes
    */
    uint32_t codeSize;

    /**
    * The number of bytes of data occupied by the entire module (the bytes occupied by the global variable)
    */
    uint32_t ramSize;
    

    /**
    * reserve
    */
    char reserve[12];

    /**
    * The entire module file checks the bytes
    */
    uint32_t crc;
 }modFileHeader_t;



/**
 * \brief Add a new module to your device
 * \return 0, success, others value indicate failed.
 */
int8_t registerMod(mMod_t *pMod);

/**
 * \brief Unmount existing modules from the device
 * \return 0, success, others value indicate failed.
 */
int8_t unregisterMod(mMod_t *pMod);

/**
 * \brief Disable module operation
 */
int8_t enableMod(mMod_t *pMod);

/**
 * \brief Enable the module to run
 * \return 0, success, others value indicate failed.
 */
int8_t disableMod(mMod_t *pMod);


/**
 * \brief Fill the relevant device information.
 * \return 0, success, others value indicate failed.
 */
int8_t initMDev(mdev_t *pMDev,
                uint32_t unusedCodeAddrBase,
                uint32_t totalCodeSize, 
                uint32_t unusedRamAddrBase,
                uint32_t totalRamSize,
                uint32_t maxNumberMod,
                uint32_t smallestCodeUnit);


/**
 * \brief show the device info
 */
 void showDeviceInfo(int8_t (*pfnStrOutput)(char *pStr, uint8_t len));


/**
 * \brief Check whether the module file is valid
 * \retval 0 : valid
 * \retval -1: invaild
 */
int8_t checkModFile(uint32_t modFileBase);




/**
 * \brief The module memory is allocated from the module manager
 * \return the pointer of new module
 */
mMod_t *allocMod(void);

/**
 * \brief Return the module memory to the module manager
 * \return 0, success, others value indicate failed.
 */
int8_t freeMod(mMod_t *pMod);

/**
 * \brief Return the module memory to the module manager
 * \return 0, success, others value indicate failed.
 */
int8_t initMod(mMod_t **ppMod, uint32_t modFileBase);

#endif /* __MTYPES_H */

