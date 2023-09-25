#include "nl1c_common.h"
#include "nl1c_ul.h"

u16 NL1cUlAddHsSfnSf(SYSTEM_TIME_S stInputTime, u32 usAddVal, SYSTEM_TIME_S *pstOutputTime)
{
    u16 usSf_Temp = stInputTime.ucSf + usAddVal;
    u16 usSfn_Temp = 0;
    u16 usHsfn_Temp = 0;

    if (usSf_Temp < TOTAL_SF_NUM)
    {
        pstOutputTime->ucSf = usSf_Temp;
        pstOutputTime->usSfn = stInputTime.usSfn;
        pstOutputTime->usHsfn = stInputTime.usHsfn;
    }
    else
    {
        usSfn_Temp = usSf_Temp / TOTAL_SF_NUM;
        pstOutputTime->ucSf = usSf_Temp - usSfn_Temp * TOTAL_SF_NUM;

        usSfn_Temp += stInputTime.usSfn;
        if (usSfn_Temp < TOTAL_SFN_NUM)
        {
            pstOutputTime->usSfn = usSfn_Temp;
            pstOutputTime->usHsfn = stInputTime.usHsfn;
        }
        else
        {
            usHsfn_Temp = usSfn_Temp >> HSFN_SFN_MASK;
            pstOutputTime->usSfn = usSfn_Temp & MAX_SFN;

            usHsfn_Temp += stInputTime.usHsfn;
            if (usHsfn_Temp < TOTAL_HSFN_NUM)
            {
                pstOutputTime->usHsfn = usHsfn_Temp;
            }
            else
            {
                pstOutputTime->usHsfn = usHsfn_Temp & MAX_HSFN;
            }
        }
    }
}

void NL1cUlFmt1TimeCalc(UL_PUSCH_FORMAT_1_PARA_S *pstTranPara, u8 ucHarqId, SYSTEM_TIME_S stFirstTransSfnSf, u16 usIntervalUI, u8 ucNpuschFormat)
{
    /* update next */
#ifdef CONFIG_NL1C_NTN_SUPPORT
#ifdef CONFIG_UIS8821_R15_L1C_SUPPORT
    if (ucNpuschFormat != SPS_FORMAT_PTW)
#endif
    {
        // update koffset in conn if dynamic koffset is valid
        NL1cSysNtnUpdateKoffset();

        NL1cUlAddHsSfnSf(pstTranPara->stFirstTransSfnSf,
                         (g_stNL1cSIDB.usKoffset - g_stNL1cSIDB.usTotMsTa),
                         &(pstTranPara->stFirstTransSfnSf));

        if ((NL1C_UL_3P75K_USBCARRIER_SPACING == stNL1cUlCommonCtrl.ucSubcarrierSpacing) &&
            (((pstTranPara->stFirstTransSfnSf.ucSf + g_stNL1cSIDB.usTotMsTa) % 2) != 0))
        {
            // OSI_NL1C_TRACE(TRA_M2, 0, "UL: usTotMsTa %d", g_stNL1cSIDB.usTotMsTa);
            NL1cUlAddHsSfnSf(pstTranPara->stFirstTransSfnsf, 1, &(pstTranPara->stFirstTransSfnSf));
        }
    }
#else
    if ((NL1C_UL_3P75K_USBCARRIER_SPACING == stNL1cUlCommonCtrl.ucSubcarrierSpacing) &&
        ((pstTranPara->stFirstTransSfnSf.ucSf % 2) != 0))
    {
        /*ul slot*/
        NL1cUlAddHsSfnSf(pstTranPara->stFirstTransSfnsf, 1, &(pstTranPara->stFirstTransSfnSf));
    }
#endif

    /* Anchor carrier and nonanchor carrier overlap both need check */
    if ((TRUE == g_stNL1cSIDB.ucAnchorOvFlag) || (TRUE == g_stNL1cSIDB.ucNonanchorOvFlag))
    {
        NL1cUlPuschPrachScCheck(ucNpuschFormat, &(stNL1cUlDynamicCtrl.ucNeedCheckPrach), ucHarqId);

        if (TRUE == stNL1cUlDynamicCtrl.ucNeedCheckPrach)
        {
            NL1cUlPuschFirstTransPostpone(&(pstTranPara->stFirstTransSfnSf));
        }
    }
    else
    {
        stNL1cUlDynamicCtrl.ucNeedCheckPrach = FALSE;
    }

    /* calc data check time */
    NL1cUlSubtractSfnSf(pstTranPara->stFirstTransSfnSf,
                        NL1C_UL_DSP_DISABLE_ADVANCE,
                        &(pstTranPara->stCheckDataReadySfnSf));
#ifdef CONFIG_UIS8821_R15_L1C_SUPPORT
    /* calc sps data cfg sfn */
    NL1cUlSubtractSfnSf(pstTranPara->stFirstTransSfnSf,
                        NL1C_UL_DATACFG_ADVANCE,
                        &(pstTranPara->stDatacfgScheSfnSf));
#endif
    /* calc DSP sche sfn */
    NL1cUlSubtractSfnSf(pstTranPara->stFirstTransSfnSf,
                        NL1C_UL_DSP_ENABLE_ADVANCE,
                        &(pstTranPara->stDspStartSchSfnSf));
    /** calc RF start sche sfn */
    NL1cUlSubtractSfnSf(pstTranPara->stFirstTransSfnSf,
                        NL1C_UL_RF_SCHEDULE_ADVANCE,
                        &(pstTranPara->stRfStartSchSfnSf));
    /* calc PUSCH finish time, finish time is the last translate sf */
    NL1cUlSubtractSfnSf(pstTranPara->stFirstTransSfnSf,
                        pstTranPara->usTotalSubframeNum - 1,
                        &(pstTranPara->stTransFinishSfnSf));

#ifdef CONFIG_NL1C_NTN_SUPPORT

    if (g_stNL1cSIDB.usTotTsTa > NL1C_UL_DL_TA_THRESH_TS)
    {
        NL1cUlSubtractSfnSf(pstTranPara->stDspStartSchSfnSf,
                            NL1C_UL_NTN_DL_TM_ADJ_ADVANCE,
                            &(pstTranPara->stPuschFm1StartDlTmAdvSchTime));
        NL1cUlSubtractSfnSf(pstTranPara->stDspStartSchSfnSf,
                            1,
                            &(pstTranPara->stNtnTcuWarpReduceAdjTime));
        NL1cUlSubtractSfnSf(pstTranPara->stTransFinishSfnSf,
                            NL1C_UL_NTN_DL_TM_ADJ_ADVANCE,
                            &(pstTranPara->stPuschFm1StartDlTmDlySchTime));
        NL1cUlSubtractSfnSf(pstTranPara->stTransFinishSfnSf,
                            1,
                            &(pstTranPara->stNtnTcuWarpReduceAdjTime));
    }

    OSI_NL1C_TRACE(TRA_M2, 0x10009af4, "UL: NTN: PUSCH_fm1 DL_TM_ADJ(adv) cfg time (%d, %d)", pstTranPara->stPuschFm1StartDlTmAdvSchTime.usSfn, pstTranPara->stPuschFm1StartDlTmAdvSchTime.ucSf);
    OSI_NL1C_TRACE(TRA_M2, 0x10009af5, "UL: NTN: PUSCH_fm1 DL_TM_ADJ(dly) cfg time (%d, %d)", pstTranPara->stPuschFm1StartDlTmDlySchTime.usSfn, pstTranPara->stPuschFm1StartDlTmDlySchTime.ucSf);
#endif

    /* dsp disable 2ms in advanced, */

    /* rf needs 2ms advance config */
    NL1cUlSubtractSfnSf(pstTranPara->stTransFinishSfnSf,
                        (NL1C_UL_RF_SCHEDULE_ADVANCE - 1),
                        &(pstTranPara->stRfCloseScheSfnSf));
    return;
}

u16 NL1cUlFmt1LthCalc(UL_PUSCH_FORMAT_1_PARA_S *pstPuschFormat1Para)
{
    pstPuschFormat1Para->usTotalSlotNum = pstPuschFormat1Para->ucSlotNum * pstPuschFormat1Para->ucRuNum * pstPuschFormat1Para->ucRuRepeatNum;

    if (NL1C_UL_3P75K_SUBCARRIER_SPACING == stNL1cUlCommonCtrl.ucSubcarrierSpacing)
    {
        pstPuschFormat1Para->usTotalSubframeNum = (pstPuschFormat1Para->usTotalSlotNum << 1);
    }
    else if (NL1C_UL_15K_SUBCARRIER_SPACING == stNL1cUlCommonCtrl.ucSubcarrierSpacing)
    {
        pstPuschFormat1Para->usTotalSubframeNum = (pstPuschFormat1Para->usTotalSlotNum >> 1);
    }
    else
    {
        OSI_NL1C_TRACE(TRA_S1, 0x10009b67, "NL1C_UL:ERROR! WRONG data subcarrier spacing: ucSubcarrierSpacing = %d", stNL1cUlCommonCtrl.ucSubcarrierSpacing);
#ifndef NL1C_FOR_CUSTOMER
        PF_ASSERT(FALSE, "fail", ASSERT_RESET);
#endif
        return L1C_FAIL;
    }

    NL1cUlSetTxMode(pstPuschFormat1Para);

    return L1C_SUCC;
}

u16 NL1cUlSetTxMode(UL_PUSCH_FORMAT_1_PARA_S *pstPuschFormat1Para)
{
    if (pstPuschFormat1Para == NULL)
    {
        return L1C_FAIL;
    }

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8821)
    if (pstPuschFormat1Para->ucSubcarrierNum >= 6)
    {
        pstPuschFormat1Para->ucTxConvMode = NL1C_UL_QUDC_MODE;
    }
    else
    {
        pstPuschFormat1Para->ucTxConvMode = NL1C_UL_POLARIQ_MODE;
    }
#else
    if (pstPuschFormat1Para->ucSubcarrierNum > NL1C_UL_QUDC_POLARIQ_TTI_THRESH)
    {
    }
    else if (pstPuschFormat1Para->ucSubcarrierNum == 12)
    {
        if (g_stNL1cSIDB.stRFInfo.usBand <= 3)
        {
            pstPuschFormat1Para->ucTxConvMode = NL1C_UL_POLARIQ_MODE;
        }
        else // QUDC for low band MT@12
        {
            pstPuschFormat1Para->ucTxConvMode = NL1C_UL_QUDC_MODE;
        }
    }
    else
    {
        pstPuschFormat1Para->ucTxConvMode = NL1C_UL_POLARIQ_MODE;
    }

#endif

    return L1C_SUCC;
}

u16 NL1cUlAddHsSfnSf(SYSTEM_TIME_S stInputTime, u32 usAddVal, SYSTEM_TIME_S *pstOutputTime)
{
#if 1
    pstOutputTime->ucSf = SF_ADD_OFFSET(stInputTime.ucSf, usAddVal);
    pstOutputTime->usSfn = SFN_ADD_OFFSET(stInputTime.usSfn, usAddVal);
    pstOutputTime->usHsfn = HSFN_ADD_OFFSET(stInputTime.usHsfn, stInputTime.usSfn, stInputTime.ucSf, usAddVal);
#else
    u32 usSf_Temp = usAddVal + stInputTime.ucSf;
    u16 usSfn_Temp = 0;
    u16 usHsfn_Temp = 0;

    if (usSf_Temp < TOTAL_SF_NUM)
    {
        pstOutputTime->ucSf = usSf_Temp;
        pstOutputTime->usSfn = stInputTime.usSfn;
        pstOutputTime->usHsfn = stInputTime.usHsfn;
    }
    else
    {
        usSfn_Temp = usSf_Temp / TOTAL_SF_NUM;
        pstOutputTime->ucSf = usSf_Temp - usSfn_Temp * TOTAL_SF_NUM;

        usSfn_Temp += stInputTime.usSfn;
        if (usSfn_Temp < TOTAL_SFN_NUM)
        {
            pstOutputTime->usSfn = usSfn_Temp;
            pstOutputTime->usHsfn = stInputTime.usHsfn;
        }
        else
        {
            usHsfn_Temp = usSfn_Temp >> HSFN_SFN_MASK;
            pstOutputTime->usSfn = usSfn_Temp & MAX_SFN;

            usHsfn_Temp += stInputTime.usHsfn;
            if (usHsfn_Temp < TOTAL_HSFN_NUM)
            {
                pstOutputTime->usHsfn = usHsfn_Temp;
            }
            else
            {
                pstOutputTime->usHsfn = usHsfn_Temp & MAX_HSFN;
            }
        }
    }

    u32 ulHsSfnSf = usAddVal + (((u32)stInputTime.usHsfn << HSFN_SFN_MASK + stInputTime.usSfn) * TOTAL_SF_NUM + stInputTime.ucSf);
    NL1cUlCalHsSfnSf(ulHsSfnSf, pstOutputTime);
#endif
}

u16 NL1cUlSubtractSfnSf(SYSTEM_TIME_S stInputTime, u32 usSubtractVal, SYSTEM_TIME_S *pstOutputTime)
{
    u32 ulHsSfnSf = stInputTime.usHsfn * TOTAL_SF_IN_ALL_SFN + stInputTime.usSfn * TOTAL_SF_NUM + stInputTime.ucSf;

    ulHsSfnSf += (ulHsSfnSf < usSubtractVal) ? (NL1C_UL_MAX_HSSFN_SF_NUM - usSubtractVal) : (0 - usSubtractVal);

#if 1
    pstOutputTime->ucSf = SF_ADD_WRAP(ulHsSfnSf, 0);
    pstOutputTime->usSfn = SFN_ADD_WRAP(ulHsSfnSf, 0);
    pstOutputTime->usHsfn = HSFN_ADD_WRAP(ulHsSfnSf, 0);
#else
    NL1cUlCalHsSfnSf(ulHsSfnSf, pstOutputTime);
#endif
    return L1C_SUCC;
}

u16 NL1cUlCalHsSfnSf(u32 ulHsSfnSf, SYSTEM_TIME_S *pstOutputTime)
{
    u32 ulTemp = ulHsSfnSf / TOTAL_SF_NUM;

    pstOutputTime->ucSf = ulHsSfnSf - ulTemp * TOTAL_SF_NUM;
    pstOutputTime->usSfn = ulTemp & MAX_SFN;
    pstOutputTime->usHsfn = (ulTemp >> HSFN_SFN_MASK) & MAX_HSFN;

    return L1C_SUCC;
}
