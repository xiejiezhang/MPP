
#define REQ_CLRCMD     (0)
#define REQ_UPGRADE    (1)
#define REQ_RUNAPP1    (2)
#define REQ_RUNAPP2    (4)


/**
 * Get the bootloader command code
 */
typedef unsigned char (*pfnBootCmdGet_t)(void *p_arg);

/**
 * Set the bootloader command code
 */
typedef unsigned char (*pfnBootCmdSet_t)(void *p_arg, unsigned char cmd);

/**
 * Execute APP1
 * The code is executed correctly and will never be returned
 */
typedef unsigned char (*pfnApp1Run_t)(void *p_arg);

/**
 * Execute APP2
 * The code is executed correctly and will never be returned
 */
typedef unsigned char (*pfnApp2Run_t)(void *p_arg);

/**
 * Check APP1 code
 * \return 1: Code validation is legal
 * \return 0: illegal code
 */
typedef unsigned char (*pfnApp1Check_t)(void *p_arg);

/**
 * Check APP2 code
 * \return 1: Code validation is legal
 * \return 0: illegal code
 */
typedef unsigned char (*pfnApp2Check_t)(void *p_arg);

/**
 * bootloader driver interface
 */
typedef struct {
    pfnBootCmdGet_t pfnBootCmdGet;
    pfnBootCmdSet_t pfnBootCmdSet;
    pfnApp1Run_t    pfnApp1Run;
    pfnApp1Check_t  pfnApp1Check;
    pfnApp2Run_t    pfnApp2Run;
    pfnApp2Check_t  pfnApp2Check;
}bootDrv_t;


/**
 * Run the Bootloader
 */
void bootloaderRun(void *p_arg, bootDrv_t *pBootDrv);
