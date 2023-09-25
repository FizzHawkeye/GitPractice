#include "nl1c_sidb.h"

#define TOTAL_SF_NUM (10)
#define TOTAL_SFN_NUM (1024)
#define TOTAL_HSFN_NUM (1024)

#define MAX_SF (TOTAL_SF_NUM - 1)
#define MAX_SFN (TOTAL_SFN_NUM - 1)
#define MAX_HSFN (TOTOTAL_HSFN_NUM - 1)

#define HSFN_SFN_MASK (10) // 2^10 = 1024

#define TOTAL_SF_IN_ALL_SFN (TOTAL_SF_NUM * TOTAL_SFN_NUM)

/*Macro for subframe add wrap. startsf is the input time's(sf), offset unit ms*/
#define SF_ADD_OFFSET(startsf, offset) \
    ((startsf + offset) % TOTAL_SF_NUM)

/*Macro for radio frame add wrap. input startsfn and startsf is the input time's (sfn, sf)， offset unit ms*/
#define SFN_ADD_OFFSET(startsfn, startsf, offset) \
    (((startsf + offset) >= TOTAL_SF_NUN) ? ((startsfn + (startsf + offset) / TOTAL_SF_NUM) & MAX_SFN) : startsfn)

/* Macro for hyper radio frame add wrap. starthsfn, startsfn and startsf is the input time's (hsfn,stfn,sf)， offset unit ms*/
#define HSFN_ADD_OFFSET(starthsfn, startsfn, startsf, offset) \
    (((startsfn * TOTAL_SFN_NUM + startsf + offset) >= TOTAL_SF_IN_ALL_SFN) ? ((startsfn + (startsfn * TOTAL_SF_NUM + startsf + offset) / TOTAL_SF_IN_ALL_SFN) & MAX_HSFN) : starthsfn)
