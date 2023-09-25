typedef struct
{
    u16 usHsfn; /* Hyper SFN: 0,1,...1023 */
    u16 usSfn;  /* system frame number */
    u8 ucSf;    /* subframe: 0,1,...9 */
} SYSTEM_TIME_S;
