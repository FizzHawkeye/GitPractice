u16 NL1cRachRaReqHd1(T_EL1_PRACH_SEND_REQ*pstisg)
#ifdef FPGA_ TEST_ ENABLE INPUT_ FEED
T_EL1_RAR_RESULT_IND stRarResultInd = {0};
NL1cSysMstateChange(NL1C_MSTATE_RACH);
/*send the rar indication to
stRarResultInd.usSuccessFlag
stRarResultInd.ulTempCrnti= 10;
famsg3 tb size fixed to 88 bits*/
stRarResultInd.usTbSize=NL1C_UL_MSG3_TB_SIZE»>3;
/PHR*/
stRarResultInd.uCPHR= 2;
MLICLIifPsMseSend (OP _EL1_RARLRESULT_ IND_NB， (u8 +)&5tRarResultInd, sizeof(stRarResultInd) );
return L1C_ SUCC;
#endif
if (NLIC SSTATE RACH NONE != eNL1cRachState)
OSIINLIC_TRACE(TRA_ N2, Ox100022d9，“RACH: Ingnore this rach request, eNL1cRachstate = xd, PrachCause = xd,", eAL 1cRachstate, pstrsg->usPrachType);
return LIC_SUCC;
#ifdef CONFIG_UIS8811_R15_SUPPORT
if (pstMsg-›usPrachType > EMAC_ PRACH_UP _EDT)
#else
if (pstMsg-›usPrachType > EMAC PRACH PDCCH ORDER)
#endif
OST_NLIC_TRACE (TRA_S1, Ox100022da, "RACH: ERROR, HRONG PrachType-%d", pstMsg-›usPrachType);
PF_ASSERT (FALSE, "fail", ASSERT_RESET);
1/save msg
memCpy (&(StNL1cRachData•stRaReqyse)， pstNsg, SizeOf (T_EL1_PRACHL SEND_REQ))；
#ifdef CONFIG_UIS8811_R15_SUPPORT
if (ENAC_ PRACHLUL_ DATA == pstHSg->usPrachType I1 ENAC PRACHLUL_SR =- pstNSg->uSPrachType)
tellse
#endif
if (EMAC_PRACH_UL_DATA == pstMsg-›usPrachType)
if (0 == g_stNL1cSIDB.stDedi(fg.ucValidFlag)
OSI_NLIC_TRACE (TRA_S1, 0x100050d2, "RACH: CONNECTED RA but no dedicate config, ignore!!!!");
return L1C_FAIL;
#ifdef CONFIG_UIS8811_R15_SUPPORT
#else
if (EMAC PRACHUL DATA == pstHsg->usPrachType）
#endif
if (EMAC_PRACH_UL_DATA == pstisg-›usPrachType || EMAC_PRACH_CP_EDT == pstMsg-›usPrachType |Il EMAC_PRACH_UP_EDT == pstMsg-›usPrachType II (TRUE != g_stNL1CSIDB.stDediCfg.stDchCfg.nbSRConfig. srwithHarqAck &8 EMAC_PRACHLUL_SR =-
u8 uCDIPendRach = 日， ucUIPendRach =日;
