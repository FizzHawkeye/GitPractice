void NR_UL_SrsConfig(NR_UL_Sounding_T*srs_p,uint16SFN,uint16N_slot,uint32location,uint16ses)
uint16 nb; uint16 ktc_p;
uint16 k0_pbar;
#ifdef TARGET_ SOC_ UIS8520
uint16 k0_posSrsbar[SRS_MAX_SYMBNUM];
#endif uint16 temp_sum=0; uint16 bSrs_cnt; uint16 bHop_cnt; uint16 temp_msc_rs;
uint16 temp_msrs,temp_NB;
wint16 sym_cnt; uint16 n_srs;
uint16 n_ srs_temp; uint16 F_b;
uint16 temp_nb;
uint16 temp_n_b; uint16 ifft_size;
uint16 real_sym_num=0; uint16 real_first_sym=0;
UTEST_FUNC_ENTRY(NR_UL_SrsConfig, void(**)(NR_UI_Sounding_T *_srs_P, wint16 _SFN, uint16 _N_slot, uint32 _location, uint16 _scs), (srs_P, SFN, N_slot, location,scs));
ifft_ size = 1 << (srs_p-sifft_len_sel + 7);
ktc_p = srs_p->kTc_indx;
#ifdef TARGET_SOC_UIS8520
if(srs_p->isPosSrsFIg == 1)
{
for (sym_cnt = 0; sym_cnt < srs_p- >num_symb; sym_cnt++)
{
kO_posSrsbar[sym_cnt]=((srs_p-sfreq_shift)*NSC_RB)+((ktc_P+srs_p->koffset[sym_cnt])%srs_p-=kTc);
else #endif
kO_pbar = ((srs_p- >freq_shift) * NSC_RB) + kto_p;
//211 6.4.1.4.3
if((srs_p->bHop) < (srs_p-›bSrs))//frequency hopping is enabled
for(sym_cnt = 0; sym_cnt < (srs_p- >num_symb_ori); sym_cnt++)
{
if(((srs_p->bitmap)>> (sym_cnt+srs_p-›start_symb)) & 0x1) // after conflict, need to send
if (0 == real_sym_num || (real_sym_num=0 &&( (sym_cnt/srs_p-=repetion) ! =((sym_cnt-1)/ srs_p- srepetion)) ))
if(0 == real_sym_num)
real_first_sym =sym_cnt+srs_p- >start_symb;
temp_sum = 0;
temp msc rs = u,
for(bSrs_cnt = 0; bSrs_cnt <= (srs_p- >bSrs); bSrs_cnt++) //b
temp_msrs = g_nr_SRSbandwidthconfig[srs_p->cSrs][bSrs_ent*2]; //m_srs_b
temp_NB = 9_mr_SRSbandwidthconfig[srs_p->cSrs][bSrs_cnt*2+1]; //Nb
if(bSrs_ent » (srs_p-=bHop))
n_srs_temp = sym_cnt/ (srs_p- srepetion);
if(NR.
_SRS_AP == srs_p->type)
n_srs = n_srs_ temp;

else
{
n_srs = (((g_nr_slot_countMode(scs]*SFN) + N_slot-(srs_p- »Toffset))/ (srs_p-»Tsrs))*((srs_p-»num_symb_ori)/ (srs_p- »repetion))+n_srs_terr
temp_nb = 1;
// from b hop to b
temp_n_b = 1; // from b_hop to b-1
for(bHop_cnt = (srs_p-»bHop+1) ; bHop_cnt < (bSrs_cnt+1); bHop_cnt++)
temp_b = temp_nb * (g_nr_SRSbandwidthconfig[srs_p->cSrs][bHop_ent * 2 + 11);
if(bHop_cnt<bSrs_cnt)
{
temp_n_b = temp_n_b * (g_nr_SRSbandwidthconfig|srs_p->cSrs][bHop_cnt*2+1]);
if(temp_NB % 2)//odd
{
F_b = (temp_NB / 2) * (n_srs / temp_n_b);
else
{
}
F_b = (temp_NB / 2) * ((n_srs % temp_b) / temp_n_b) + (n_srs % temp_nb) / (2 * temp_n_b);
nb = (F_b + 1 * (srs_p->freq_pos) / temp_msrs) % temp_NB;
} ? end if bSrs_ent> (srs_p- »bHop) ?
nb = ((1* (srs_p- »freq_pos)/temp_msrs)% (temp_NB));
temp_msc_rs = (NSC_RB * temp_msrs) / (srs_p- >kTc);
temp_sum += (srs_p- >kTc) * nb * temp_msc_rs;
} ? end for bSrs_cnt=0;bSrs_ent<s...
NR_UL_SrsKOCalc(srs_p, location, temp_sum, kO_pbar, ifft_size, real_sym_num);
} ? end if Omareal_sym_num|| (rea... ?
else // temp_sum is the same, no need to calc
{
srs_p->SymbInfo[real_sym_num].ko[o] = srs_p->SymbInfo[real_sym_num- 1].ko[ol;
if(2 == srs_p->num_ports)
srs_p->SymbInfo[real_sym_num].k0[1] = srs_p->SymbInfo[real_sym_num].K0[0];
real_sym_num++;
} ? end if ((srs_p- >bitmap)>>(sy... ?
} ? end for sym_ent=0;sym_ent<(sr... ?
) ? end if (srs_p- >bHop)<(srs_p-... ?
else// frequency hopping is disabled
temp_sum
temp_msc_rs = 0;
real_ sym_num =0;
#ifdef TARGET SOC_ UIS8520
it(srs_p->IsPosSrsFig ==
0)
#endif
for(bSrs_cnt = 0; bSrs_cnt < = (srs_p->bSrs); bSrs_cnt++)
eme- Ners a r nsisbandctlcopalsrs Pracsrellbsrsent 2J;
temp NB = g nr SRSbandwidthconfig srs p->cSrs][bSrs cnt * 2 + 17;:
nb = (4 * (srs_p- >freq_pos) / temp_msrs) % (temp_B);
temp_msc_rs = (NSC_RB * temp_msrs) / (srs _p- >kTc);
temp_sum += (srs_p->kTc) * nb * temp_msc_rs;

for(sym_cnt = 0; sym_cnt < (srs_p- >num_symb); sym_cnt++)
if(((srs_p->bitmap)>>(sym_cnt+srs_p->start_symb)) & Oxl) // after conflict, need to send
if (o == real_sym_num)
real_first_sym =sym_cnt+srs_p->start_symb;
#ifdef TARGET_SOC_UIS8520
if(srs_p- ›isPosSrsFlg == 1)
NR_UL_SrsKOCalc(srs_p, location, temp_sum, k0_posSrsbar[real_sym_num], ifft_size, real_sym_num);
else
#endif
NR_UL_SrsKOCalc(srs_p, location, temp_sum, kO_pbar, ifft_size, real_sym_num);
了
(seto same as the hest one
#ifdef TARGET_ SOC_UIS8520
if(srs_p-›isPosSrsflg == 1)
NR_UL_SrsKOCalc(srs_p, location, temp_sum, kO_posSrsbar[real_sym_num], ifft_size, real_sym_num);
else
#endif
}
srs_p->SymbInfo[real_sym_num].ko[o] = srs_p->SymbInfo[o].ko[ol;
if(2 as Srs_p- >num_ports)
srs_p->Symbinfo[real_sym_numj.ko[a] = srs_p->SymbInfo[real_sym_num].ko[o);
real_sym_num++;
end if ((srs_p- ›bitmap)>>(sy... ? end for sym_cnt=0;sym_cnt<(sr...
} ? end else ?
srs_p->start_symb = real_first_sym;
srs_p- >num_symb = real_sym_num
