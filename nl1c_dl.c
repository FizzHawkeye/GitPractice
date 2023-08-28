#define READ_BITS(size, result)                                        \
    do                                                                 \
    {                                                                  \
        *(result) = (Content >> (Len - (size))) & ((1 << (size)) - 1); \
        Len -= (size);                                                 \
    } while (0);

u16 NL1cDlStartDoing(u8 ucProcId)
{
    if (stDlCtrl.ucDoingProcId != U8_INFINITY)
    {
        // another process is in DOING state, error
        PF_ASSERT(FALSE, "fail", ASSERT_RESET);
        return L1C_FAIL;
    }

    NL1cDlCtrlCfg(1, stDlCtrl.astProc[ucProcId].ucBufferIdx);

    stDlCtrl.astProc[ucProcId].ucState = DL_PROCSTATE_DOING;
    stDlCtrl.astProc[ucProcId].usRecvCnt++;
    // move this process to the head
    list_move(&stDlCtrl.astProc[ucProcId].list, &stDlCtrl.EnabledProcList);

    stDlCtrl.ucDoingProcId = ucProcId;
    stDlCtrl.astViterbiProcBuffer[stDlCtrl.ucViterbiProcCnt].ucProcId = ucProcId;
    stDlCtrl.astViterbiProcBuffer[stDlCtrl.ucViterbiProcCnt].usRecvCnt = stDlCtrl.astProc[ucProcId].usRecvCnt;
    stDlCtrl.astViterbiProcBuffer[stDlCtrl.ucViterbiProcCnt].ucViterbiLeftSf = stDlCtrl.astProc[ucProcId].ucViterbiNeedSf;
    stDlCtrl.astViterbiProcBuffer[stDlCtrl.ucViterbiProcCnt].stTime = stDlCtrl.stAirTime;
    return L1C_SUCC;
}

u16 NL1cD1FindNoPdcchSf(SYSTEM_TIME_S stInputTime, u8 ucSsType, SYSTEM_TIME_S *pstOutputTime, u32 *pulCount)
{
    u8 ucCarrier;
    u8 ucGapEnable;
    u16 usTmpBm;
    u8 ucTmpSf;
    u8 ucPdcchInfo;
    u8 ucPdcchSfNum;
    u8 ucPdcchSfIdx;
    u16 usPdcchInfoPeriod;
    u8 ucStartSf;
    u32 ulInputFrameIdx;
    u32 ulOutputFrameIdx;
    u8 *pucPdcchInfo;
    ucCarrier = stDlCtrl.astSSInfo[ucSsType].ucCarrier;
    ucGapEnable = stDlCtrl.astSSInfo[ucSsType].ucGapEnable;
    if ((ucCarrier != NL1C_ANCHOR_CARRIER) && (ucGapEnable == 0) && (stDlCtrl.astCarrierPara[ucCarrier].ucBmPresent == 0))
    {
        // for non-anchor carrier, if gap not enabled, bitmap not enabled, can't find a no pdcch
        return L1C_FAIL;
    }
    if (ucCarrier == NL1C_ANCHOR_CARRIER)
    {
        if (ucGapEnable == 1)
        {
            pucPdcchInfo = stDlCtrl.aucAnchorPdcchInfoGap;
            usPdcchInfoPeriod = stDlCtrl.usLCM5iGap;
        }
        else
        {
            pucPdcchInfo = stDlCtrl.aucAnchorPdcchInfoNoGap;
            usPdcchInfoPeriod = stDlCtrl.uslaxsiPeriod;
        }
    }
    else
    {
        if (ucGapEnable == 1)
        {
            pucPdcchInfo = &stDlCtrl.aucNonAnchorPdcchInfoGap[ucCarrier - 1][0];
            usPdcchInfoPeriod = stDlCtrl.ausNonAnchorGapFramePeriod[ucCarrier - 1];
        }
        else
        {
            pucPdcchInfo = &stDlCtrl.aucNonAnchorPdcchInfoNoGap[ucCarrier - 1][0];
            usPdcchInfoPeriod = 4;
        }
    }

    ulInputFrameIdx = stInputTime.usHsfn * TOTAL_SFN_NUM + stInputTime.usSfn;
    ulOutputFrameIdx = ulInputFrameIdx;
    ucStartSf = stInputTime.ucSf;
    while (1)
    {
        ucPdcchInfo = pucPdcchInfo[ulOutputFrameIdx & (usPdcchInfoPeriod - 1)];
        ucPdcchSfNum = ucPdcchInfo & 0xF;
        if (ucPdcchSfNum < TOTAL_SF_NUM)
        {
            ucPdcchSfIdx = ucPdcchInfo >> 4;

            if (ucPdcchSfNum > 0)
                // recover pdcch sf bitmap for this frame
                usTmpBm = NL1cD1ValidSfBmGet(ucCarrier, ulOutputFrameIdx) & (~((1 << ucPdcchSfIdx) - 1));
            for (ucTmpSf = ucPdcchSfIdx; ucTmpSf < TOTAL_SF_NUM; ucTmpSf++)
            {
                if ((usTmpBm & (1 << ucTmpSf)) > 0)
                {
                    ucPdcchSfNum--;
                    if (ucPdcchSfNum = 0)
                    {
                        break;
                    }

                    usTmpBm &= ((1 << (ucTmpSf + 1)) - 1); // ucTmpSf is the last pdcch sf in this frame
                }
                else
                {

                    // all sfs in this frame are not pdech sf
                    usTmpBm = 0;
                    for (ucTmpSf = ucStartSf; ucTmpSf < TOTAL_SF_NUM; ucTmpSf++)
                    {
                        if ((usTmpBm & (1 << ucTmpSf)) = 0)
                        {
                            // Find a no pdcch sf
                            *pulCount = (ulOutputFrameIdx - ulInputFrameIdx) * TOTAL_SF_NUM + ucTmpSf - stInputTime.ucsf;
                            pstOutputTime->usHsfn = (ulOutputFrameIdx / TOTAL_SFN_NUM) % TOTAL_HSFN_NUM;
                            pstOutputTime->usSfn = ulOutputFrameIdx % TOTAL_SFN_NUM;
                            pstOutputTime->ucsf = ucTmpSf;
                            return L1C_SUCC;
                        }
                    }
                }
                // can't find a no pdcch in this frame, goto next frame ulOutputFrameIdx++;
                ucStartSf = 0;
            }
        }
    }
}
