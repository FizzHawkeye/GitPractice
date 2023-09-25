#include "nl1c_common.h"

#define NL1C_UL_QUDC_MODE (0)
#define NL1C_UL_POLARIQ_MODE (1)
#define NL1C_UL_QUDC_POLARIQ_TTI_THRESH (32)

typedef struct
{
    // start time
    SYSTEM_TIME_S stFirstTransSfnSf; /* *transmission sfn: 0~10239 */

    SYSTEM_TIME_S stDspStartScheSfnSf /* *schedule 4ms in advanced */

} UL_PUSCH_FORMAT_1_PARA_S;
