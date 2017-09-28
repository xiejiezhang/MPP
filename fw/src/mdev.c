/****************************************Copyright(C),2017,xjz***************************************
**
**  The software is very welcome for learning, teaching, research. 
**  If you are used for business, please tell me.
** 
**  http://my.csdn.net/sdewenking
**  xiejiezhang@gmail.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           mdev.c
** Last modified Date:  2017-09-05
** Last Version:        V1.0
** Descriptions:        Framework management implementation
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
#include "mdev.h"
#include "mcrc.h"

static mdev_t   __gmdev;
static mdev_t * __gpmdev;

/**
 * convert base addr to device 
 */
#define __MDEV_CGET(base)   ((const mdev_t *)((void *)(base)))

/**
 * \brief Automatically traversal the device to find the currently registered module
 * \return 0, success, others value indicate failed.
 */
static 
int8_t initModList(mdev_t *pMDev,
                   uint32_t firstModFileBase,
                   uint32_t smallestCodeUnit);

/**
 * \brief Run the function entry of the module in the list
 * \return 0, success, others value indicate failed.
 */
static 
int8_t runModList(mdev_t *pMDev);

/**
 * \brief Call the module to run the entry function
 * \return 0, success, others value indicate failed.
 */
static 
int8_t runModEntry(mMod_t *pMod);

/**
 * \brief Fill the relevant device information and call the initialization module list function
 * \return 0, success, others value indicate failed.
 */
int8_t initMDev(mdev_t *pMDev,
                uint32_t unusedCodeAddrBase,
                uint32_t totalCodeSize, 
                uint32_t unusedRamAddrBase,
                uint32_t totalRamSize,
                uint32_t maxNumberMod,
                uint32_t smallestCodeUnit)
{
    mdev_t *pMDev = &__gmdev;
    //STD_APCHK(pMDev != NULL);

    pMDev->unusedCodeAddrBase = STD_ALIGN(unusedCodeAddrBase, smallestCodeUnit);
    pMDev->totalCodeSize      = totalCodeSize;
    pMDev->unusedRamAddrBase  = STD_ALIGN4(unusedRamAddrBase);
    pMDev->totalRamSize       = totalRamSize;

    pMDev->unusedCodeSize     = totalCodeSize;
    pMDev->unusedRamSize      = totalRamSize;
    

    pMDev->maxNumberMod       = maxNumberMod;
    pMDev->mModpointer        = (__mMod * )((void *)0);

    pMDev->numberOfRegisteredMod = 0;

    initModList(pMDev, pMDev->unusedCodeAddrBase, smallestCodeUnit);

    runModList(pMDev);

    __gpmdev = pMDev;

    return 0;
}



/**
 * \brief Automatically traversal the device to find the currently registered module
 * \return 0, success, others value indicate failed.
 */
static 
int8_t initModList(mdev_t  *pMDev,
                   uint32_t firstModFileBase,
                   uint32_t smallestCodeUnit)
{
    uint32_t uncCode       = pMDev->totalCodeSize;
    uint32_t checkAddrBase = firstModFileBase;
    mMod_t  *pMod          = 0;

    for(; uncCode > 0 && uncCode > smallestCodeUnit; uncCode -= smallestCodeUnit) {
        
        if (0 == checkModFile(checkAddrBase)) {
            //get the correct module file
            if(0 == initMod(&pMod, checkAddrBase)) {
                registerMod(pMod);
            }
        }
        checkAddrBase += smallestCodeUnit;
    }
}

/**
 * \brief Run the function entry of the module in the list
 * \return 0, success, others value indicate failed.
 */
static 
int8_t runModList(mdev_t *pMDev)
{
     mMod_t *pMod = pMDev->mModpointer;

     while(pMod) {
         if(pMod->modRunFlag & MRF_EN_MASK) {
             runModEntry(pMod);
         }
         pMod = pMod->next;
     }
     return 0;
}


/**
 * \brief Check whether the module file is valid
 * \retval 0 : valid
 * \retval -1: invaild
 */
 int8_t checkModFile(uint32_t modFileBase)
 {
#define MCRC_DUMMY_DATA 0xAA551234
    const modFileHeader_t *pmfh = (const modFileHeader_t *)((void *)modFileBase);
    const uint8_t         *pdat = (const uint8_t *)((void *)modFileBase);
    int8_t   ret = -1;
    uint32_t calCrc32;
    uint32_t dummyCrc = MCRC_DUMMY_DATA;

    STD_APCHK(pMDev != NULL);
    
    if (MFH_IDENTIFIER != pmfh->identifier) {
        return ret;
    }
    calCrc32 = mcrc32(0, pdat, sizeof(modFileHeader_t) - sizeof(pmfh->crc));
    calCrc32 = mcrc32(calCrc32, (const uint8_t *)&dummyCrc, sizeof(pmfh->crc));

    pdat     = (const uint8_t *)(pdat+sizeof(modFileHeader_t));
    calCrc32 = mcrc32(calCrc32,  pdat, pmfh->codeSize - sizeof(modFileHeader_t));
    
    if (pmfh->crc != calCrc32)
    {
       return ret;
    }
    ret = 0;

    return ret;
 }

 /**
 * \brief Return the module memory to the module manager
 * \return 0, success, others value indicate failed.
 */
int8_t initMod(mMod_t **ppMod, uint32_t modFileBase)
{
    const modFileHeader_t *pmfh = (const modFileHeader_t *)((void *)modFileBase);
    
    STD_APCHK(__gpmdev != NULL);
    STD_APCHK(ppMod    != NULL);

    if(__gpmdev->unusedRamSize < STD_ALIGN4(pmfh->ramSize + sizeof(mMod_t))) {
        return -1;
    }

    if(__gpmdev->unusedCodeSize < STD_ALIGN4(pmfh->codeSize)) {
        return -2;
    }

    *ppMod = allocMod();
    (*ppMod)->next = 0;
    (*ppMod)->modBaseAddr = modFileBase;
    (*ppMod)->modRunFlag  = pmfh->flag;
    
    //align 4 bytes
    __gpmdev->unusedRamSize  -= STD_ALIGN4((pmfh->ramSize));
    __gpmdev->unusedCodeSize -= STD_ALIGN4((pmfh->codeSize));
    
    __gpmdev->unusedCodeAddrBase += __gpmdev->unusedCodeSize;
    __gpmdev->unusedRamAddrBase  += __gpmdev->unusedRamSize;

    return 0;
}


/**
 * \brief Add a new module to your device
 * \return 0, success, others value indicate failed.
 */
int8_t registerMod(mMod_t *pMod)
{
    mMod_t *pModTmp = 0;

    STD_APCHK(__gpmdev != NULL);
    STD_APCHK(pMod     != NULL);

    if (__gpmdev->mModpointer)
    {
        pModTmp = __gpmdev->mModpointer;
        //find the tail of List
        while(pModTmp->next) {
            pModTmp = pModTmp->next;
        }
        pModTmp->next = pMod;
    } else {
        __gpmdev->mModpointer = pMod;
    }
    return 0;
}

/**
 * \brief Unmount existing modules from the device
 * \return 0, success, others value indicate failed.
 */
int8_t unregisterMod(mMod_t *pMod)
{
    mMod_t *pModTmp = 0;

    STD_APCHK(__gpmdev != NULL);
    STD_APCHK(pMod     != NULL);

    pModTmp = pMDev->mModpointer;
    while(pModTmp) {
        if (pMod == pModTmp) {
            //ideal mode memory
            break;
        }
        pModTmp = pModTmp->next;
    }
    return freeMod(pModTmp);
}

/**
 * \brief Disable module operation
 */
int8_t enableMod(mMod_t *pMod)
{
    STD_APCHK(pMod     != NULL);

    pMod-> modRunFlag |= MRF_EN_MASK;
    return 0;
}

/**
 * \brief Enable the module to run
 * \return 0, success, others value indicate failed.
 */
int8_t disableMod(mMod_t *pMod)
{
    STD_APCHK(pMod     != NULL);

    pMod-> modRunFlag &= ~MRF_EN_MASK;
    return 0;
}


/**
 * \brief Call the module to run the entry function
 * \return 0, success, others value indicate failed.
 */
static 
int8_t runModEntry(mMod_t *pMod)
{
    const modFileHeader_t *pmfh;
    STD_APCHK(pMod     != NULL);

    pmfh = (const modFileHeader_t *)((void *)(pMod->modBaseAddr));
    if (pmfh->entryAddr)
    {
        //run entry
        pmfh->entryAddr((void *)pmfh);
        return 0;
    }
    return -1;
}



/**
 * \brief The module memory is allocated from the module manager
 * \return the pointer of new module
 */
mMod_t *allocMod(void)
{
    mMod_t *pMod = NULL;
    uint8_t size = STD_ALIGN4(sizeof(mMod_t));

    STD_APCHK(__gpmdev != NULL);
    
    pMod = pMDev->mModpointer;
    while(pMod) {
        if (0 == pMod->modBaseAddr) {
            //ideal mode memory
            return pMod;
        }
        pMod = pMod->next;
    }
     
    if(__gpmdev->unusedRamSize > size) {
         pMod = ((mMod_t *) (__gpmdev->unusedRamAddrBase))
         __gpmdev->unusedRamSize      -= size;
         __gpmdev->unusedRamAddrBase  += size;
         return pMod;
    }
    
    return NULL;
}


/**
 * \brief Return the module memory to the module manager
 * \return 0, success, others value indicate failed.
 */
int8_t freeMod(mMod_t *pMod)
{
    STD_APCHK(pMod != NULL);
    
    pMod->modBaseAddr = 0;
    return 0;
}