#ifndef __PROGRAM_H
#define __PROGRAM_H

typedef struct {
    void (*pfnAppErase)(void *p_arg);
    unsigned int (*pfnAppCheck)(void *p_arg);
    char (*pfnAppWriteCode)(void *p_arg, unsigned char *pBuf, unsigned int len, unsigned char isReWrite);
    void (*pfnAppFinishWrit)(void *p_arg);
}appProgramDrv_t;

typedef struct 
{
    appProgramDrv_t app1Drv;
    appProgramDrv_t app2Drv;
    unsigned int (*pfnPageSizeGet)(void *p_arg);
}programDrv_t;

typedef struct {
    void *p_arg;
    programDrv_t drv;

    unsigned int app1SecPos;
    unsigned int app2SecPos;
}programDev_t;

#endif /* __PROGRAM_H */