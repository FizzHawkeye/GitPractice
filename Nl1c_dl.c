u16 NL1cD1FindNoPdcchSf(SYSTEM_TIME_S stInputTime, u8 ucSsType, SYSTEM_TIME_S *pstOutputTime, u32 *pulCount)
u8 ucCarrier;
u8 ucGapEnable;
u16 usTmpBm;
u8 uCTmpsf;
u8 ucPdcchInfo;
u8 ucPdcchsfNum;
u8 ucPdcchSfIdx;
u16 usPdcchInfoPeriod;
u8 ucStartSf;
u32 ulInputFrameIdx;
uloutputFrameIdx;
u8 *pucPdcchInfo;
ucCarrier
stD1Ctr1.astSSInfo[ucSsType].ucCarrier;
ucGapEnable = stDICtr1.astSSInfo [ucSsType].ucGapEnable;
if ((ucCarrier != NL1C_ANCHOR_CARRIER) && (ucGapEnable == 0) && (stD1Ctri.astCarrierPara[ucCarrier] .ucBmPresent == 0))
//for non-anchor carrier, if gap not enabled, bitmap not enabled, can't find a no pdcch
return L1C_FAIL;
if (ucCarrier == NL1C_ANCHOR_CARRIER)
if (ucGapEnable
pucPdcchInfo = stD1Ctrl.aucAnchorPdcchInfoGap;
usPdcchInfoperiod = stDIctr1.usLCM5iGap;
else
pucPdcchInfo = stDICtr1.aucAnchorPdcchInfoNoGap;
usPdcchInfoPeriod = stD1Ctr1.uslaxsiPeriod;
else
if (ucGapEnable
puCPdcchInfo = &stD1Ctr1.aucNonAnchorPdcchInfoGap [ucCarrier - 1][0];
usPdcchInfoperiod = stD1Ctr1.ausNonAnchorGapFramePeriod [ucCarrier
- 1];
else
pucPdcchInfo=&stDICtr1.aucNonAnchorPdcchInfoNoGap[ucCarrier-1][0];
usPdcchInfoPeriod = 4;
ulInputFrameIdx = stInputTime . usHs fn * TOTAL SFN_ NUM + StInputTime . ussfn;
uloutputFrameIdx = ulInputFrameIdx;
ucStartsf = stInputTime . ucsf;
while (1)
ucPdcChInfo = pucPdcchInforulOutputFrame Idx & (usPdcchInfoPeriod - 1)];
ucPdcChSfNum = ucPdcchInfo & OxF；
if (ucPdcchSFNum < TOTAL SF NUM)
ucPdcchSfIdx = ucPdcchInfo >>4;

if (ucPdcchsfNum > 0）
1/recover pdcch sf bitmap for this frame
uSTmpBm = NL1cD1ValidSfBnGet (ucCarriers ulOutputFramerdx) & (~((1 << ucPacchsfIdx) - 1）)；
for (ucTmpsf = ucPdcchSfTdx; ucTmpSf < TOTAL_ SF_MUM: uCTmpsf++)
if ((usTmpBm & (1 << ucTmpsf)) > @)
ucPdcchSfhum--;
if (ucPdcchSfNum = e)
{ break;

usTmp&m &- ((1 <‹ (ucTmpSf + 1)) - 1); / /ucTmpSf is the last pdcch sf in this frame
else
//all sfs in this frame are not pdech sf
UsTmpBm = 0;
for (ucTmpsf a uestartsf; uCTmpsf < TOTAL_SF_NUM; ucTmpsf++)
if ((usTmpBm & (1 << ucTmpsf)) = 0)
I/Find a no pdcch sf
*pulCount • (uzOutputFrameIdx - wIInputFrame Tdx) • TOTAL_SF_NUM + uCTmpSf - stInputTime. ucsf;
pstoutputTime-susHsfn • (ulOutputFrame Idx / TOTAL_SFNLMUN) S6 TOTAL_HSFNLNUN;
pstoutputT ime->usSfn • uloutputFrame Idx % TOTAL SFNINUM;
pstoutputTime->ucsf • ucTmpSfreturn L1C_SUCC;
//can't find a no pdcch in this frame, goto next frame ulOutputFrameIdx++;
ucStartsf = 0;
