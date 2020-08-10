////////////////////////////////////////////////////////////////////////
// Runs a player's game turn ...
//
// Can  produce  either a hunter player or a Dracula player depending on
// the setting of the I_AM_DRACULA #define
//
// This  is  a  dummy  version of the real player.c used when you submit
// your AIs. It is provided so that you can test whether  your  code  is
// likely to compile ...
//
// Note that this is used to drive both hunter and Dracula AIs. It first
// creates an appropriate view, and then invokes the relevant decideMove
// function,  which  should  use the registerBestPlay() function to send
// the move back.
//
// The real player.c applies a timeout, and will halt your  AI  after  a
// fixed  amount of time if it doesn 't finish first. The last move that
// your AI registers (using the registerBestPlay() function) will be the
// one used by the game engine. This version of player.c won't stop your
// decideMove function if it goes into an infinite loop. Sort  that  out
// before you submit.
//
// Based on the program by David Collien, written in 2012
//
// 2017-12-04	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.3	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#ifdef I_AM_DRACULA
# include "dracula.h"
# include "DraculaView.h"
#else
# include "hunter.h"
# include "HunterView.h"
#endif

// Moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

// A pseudo-generic interface, which defines
// - a type `View',
// - functions `ViewNew', `decideMove', `ViewFree',
// - a trail `xtrail', and a message buffer `xmsgs'.
#ifdef I_AM_DRACULA

typedef DraculaView View;

# define ViewNew DvNew
# define decideMove decideDraculaMove
# define ViewFree DvFree

# define xPastPlays "GED.... SMA.... HRO.... MGA.... DBE.V.. GMN.... SBO.... HMI.... MCD.... DSOT... GLO.... SNA.... HZU.... MKL.... DVAT... GSW.... SLE.... HST.... MBEV... DATT... GIR.... SPA.... HMU.... MBC.... DD1T... GAO.... SST.... HZA.... MCN.... DHIT... GMS.... SBR.... HSJ.... MBS.... DIO.... GMR.... SPR.... HSOT... MVR.... DTS.... GMS.... SVI.... HSO.... MSO.... DGOT.M. GTS.... SBD.... HSA.... MSO.... DMRT.M. GIO.... SVE.... HSO.... MSO.... DTOT.M. GAS.... SGOT... HSA.... MSZ.... DSRT.M. GVE.... SMRT... HIO.... MVE.... DSNT... GFL.... SBO.... HTS.... MMU.... DMA.V.. GVE.... SBA.... HRO.... MNU.... DCAT... GBR.... SMS.... HBI.... MMU.... DLST... GLI.... SMR.... HNP.... MZA.... DD4T.M. GCO.... SBO.... HMI.... MSZ.... DBB..M. GAM.... SCF.... HVE.... MBE.... DSNT.M. GNS.... SNA.... HAS.... MSZ.... DMAT.V. GAO.... SPA.... HIO.... MCN.... DALT.M. GAO.... SPA.... HIO.... MCN.... DHIT.M. GMS.... SBU.... HTS.... MBS.... DD3T.M. GALTT.. SPA.... HMS.... MIO.... DLST... GMATT.. SBO.... HBA.... MTS.... DAO..M. GLST... SMA.... HMA.... MMS.... DEC.... GZA.... SBO.... HCA.... MAO.... DLO.V.. GMU.... SSR.... HLS.... MNS.... DHIT... GST.... SAL.... HSN.... MHA.... DD2T... GCO.... SLS.... HBO.... MNS.... DPLT... GAM.... SMA.... HBB.... MEC.... DTPT... GNS.... SAL.... HNA.... MNS.... DCDT... GHA.... SMS.... HPA.... MHA.... DGAT.V. GBR.... STS.... HMR.... MBR.... DBCT.M. GVI.... SIO.... HMI.... MPR.... DSOT.M. GBE.... SBS.... HVE.... MBD.... DSAT.M. GBCT... SVR.... HVI.... MBC.... DIO..M. GCN.... SSOT... HBC.... MSO.... DAS..M. GBS.... SSO.... HSO.... MSO.... DVET.M. GIO.... SSO.... HSO.... MSO.... DMI.V.. GAS.... SBC.... HSAT... MSA.... DMRT... GBI.... SBE.... HVA.... MSA.... DCFT... GNP.... SGA.... HAT.... MSA.... DNAT... GFL.... SKL.... HIO.... MVR.... DLET... GVET... SCD.... HVA.... MCN.... DEC.... GVI.... SKL.... HIO.... MBS.... DLOT.V. GBR.... SBD.... HTS.... MIO.... DD1T.M. GST.... SZA.... HMS.... MAS.... DMNT.M. GZU.... SVI.... HTS.... MVE.... DHIT.M. GMR.... SPR.... HRO.... MMU.... DLVT.M. GMR.... SPR.... HRO.... MMU.... DSWT... GBO.... SNU.... HFL.... MZA.... DLOT.M. GTO.... SST.... HGO.... MSZ.... DD5.VM. GSR.... SBU.... HRO.... MBE.... DMNT.M. GAL.... SAM.... HBI.... MBC.... DHIT.M. GBA.... SCO.... HRO.... MCN.... DLVT.M. GBA.... SCO.... HRO.... MCN.... DSWT.M. GMS.... SBU.... HTS.... MBS.... DLOT.M. GAO.... SLE.... HMS.... MIO.... DD5T.V. GEC.... SEC.... HAO.... MTS.... DEDT.M. GNS.... SLOT... HIR.... MMS.... DNS..M. GEC.... SMNT... HLVT... MAO.... DHAT... GNS.... SSWT... HMN.... MNS.... DCOT... GHAT... SIR.... HEDT... MHA.... DSTT... GNS.... SAO.... HNS.... MFR.... DMUT... GHA.... SNS.... HHA.... MLI.... DZA.V.. GBR.... SHA.... HHA.... MHA.... DSJT... GVE.... SCOT... HCO.... MCO.... DBET... GFL.... SAM.... HSTT... MBO.... DKLT... GVE.... SBU.... HGE.... MNA.... DCDT... GBD.... SMR.... HZU.... MPA.... DD1T.M. GGA.... SMI.... HMI.... MST.... DHIT.V. GCDTTTD SVE.... HVE.... MMU.... DGAT.M. GSZ.... SBD.... HBD.... MZA.... DCNT.M. GBE.... SGAT... HKLT... MSZ.... DBS.... GBC.... SSZ.... HCD.... MGA.... DIO.... GCNT... SBC.... HGA.... MCN.... DTS.... GBS.... SCN.... HCN.... MCN.... DGOT... GIO.... SVR.... HCN.... MCN.... DMR.V.. GAS.... SSO.... HCN.... MCN.... DTOT... GBI.... SBE.... HCN.... MCN.... DSRT... GNP.... SBD.... HCN.... MCN.... DBOT... GRO.... SSO.... HCN.... MCN.... DD1T... GGE.... SSA.... HCN.... MSZ.... DHIT.M. GMRV... SIO.... HSZ.... MPR.... DBB.... GMS.... STS.... HVI.... MVI.... DSNT.M. GAL.... SIO.... HMU.... MVE.... DLST.M. GBA.... SAS.... HNU.... MFL.... DCAT.M. GTO.... SVE.... HLI.... MZU.... DGRT.M. GSR.... SGO.... HBR.... MMR.... DHIT.M. GSR.... SGO.... HBR.... MMR.... DD3T... GBU.... SMI.... HLI.... MMS.... DMA.VM. GPA.... SFL.... HCO.... MAO.... DSRT.M. GBO.... SVE.... HAM.... MCATT.. DALT... GSRT... SMU.... HNS.... MMAV... DMS..M. GALT... SZU.... HAO.... MGRT... DTS.... GMS.... SMR.... HMS.... MAL.... DNPT... GAO.... SGO.... HCG.... MAL.... DBIT... GCA.... SRO.... HMS.... MAL.... DAS.... GAO.... SBIT... HTS.... MMS.... DVET... GMS.... SAS.... HRO.... MTS.... DMUT... GTS.... SVET... HMI.... MGO.... DZAT... GRO.... SBD.... HMR.... MVE.... DSJT.M. GFL.... SKL.... HCF.... MAS.... DSOT... GVE.... SBE.... HBO.... MBI.... DBC.V.. GVI.... SBCVD.. HPA.... MAS.... DGAT... GBD.... SCN.... HST.... MVE.... DD1T.M. GBE.... SBS.... HMU.... MVI.... DCNT.M. GKL.... SCNT... HVI.... MBC.... DBS..M. GBE.... SSZ.... HBC.... MSOT... DIO.... GSA.... SCN.... HCN.... MVR.... DTS.... GIO.... SCN.... HCN.... MCN.... DGOT.M. GAS.... SCN.... HCN.... MCN.... DMRT.M. GBI.... SBS.... HGA.... MCN.... DCFT... GRO.... SVR.... HCD.... MCN.... DBOT... GFL.... SSO.... HKL.... MCN.... DSRT... GGOT... SSA.... HSZ.... MBD.... DSNT... GMRT... SIO.... HVE.... MZA.... DLS.V.. GBOT... STS.... HMI.... MMU.... DCAT... GZA.... SMS.... HMR.... MST.... DGRT.M. GMU.... SAL.... HBO.... MGE.... DD2T... GNU.... SLSV... HBA.... MPA.... DAO..M. GLI.... SBO.... HMS.... MBO.... DIR..M. GHA.... SNA.... HAO.... MSR.... DSWT... GNS.... SBB.... HCATT.. MAL.... DLOT... GHA.... SNA.... HAO.... MGRT... DMNT... GNS.... SCF.... HIR.... MMA.... DD2T... GHA.... SGE.... HDU.... MBA.... DPLT... GBR.... SST.... HIR.... MLS.... DHIT... GPR.... SBU.... HDU.... MSN.... DEC..M. GBD.... SAM.... HIR.... MSR.... DLO.VM. GVE.... SNS.... HLV.... MMA.... DSWT.M. GVE.... SNS.... HLV.... MMA.... DD1T.M. GMI.... SAO.... HMN.... MLS.... DIR..M. GGE.... SBB.... HED.... MAO.... DDUT.M. GPA.... SAO.... HMN.... MEC.... DHIT... GCF.... SMS.... HLOV... MNS.... DGWT... GBO.... SAO.... HPL.... MAM.... DAO..M. GBU.... SIR.... HLO.... MNS.... DD2T.M. GLE.... SDUTT.. HSW.... MAO.... DTPT... GEC.... SIR.... HLO.... MNS.... DCDT... GNS.... SAO.... HED.... MHA.... DHIT... GHA.... SNS.... HNS.... MBR.... DGAT.M. GVI.... SHA.... HHA.... MPR.... DCN.V.. GZA.... SBR.... HBR.... MBD.... DVRT.M. GSZ.... SPR.... HBD.... MKL.... DBS..M. GBE.... SVI.... HKL.... MCDTT.. DIO.... GCNV... SZA.... HSZ.... MGAT... DTS.... GBS.... SSZ.... HCN.... MCN.... DMS.... GIO.... SCN.... HCN.... MCN.... DALT... GAS.... SCN.... HCN.... MCN.... DSRT.M. GIO.... SCN.... HCN.... MCN.... DD1T... GTS.... SBC.... HVR.... MCN.... DSNT... GMS.... SSZ.... HCN.... MCN.... DLST... GALT... SVE.... HBS.... MBS.... DCAT... GLST... SMU.... HIO.... MIO.... DGRT... GCAT... SST.... HTS.... MTS.... DMA.VM. GMAVD.. SPA.... HMS.... MMS.... DD5T.M. GZA.... SSR.... HBA.... MAL.... DBB..M. GMU.... SBO.... HSR.... MGRT... DAO.... GZU.... SBB.... HMA.... MMA.... DEC.... GMR.... SAO.... HMA.... MMA.... DLOT... GBU.... SCA.... HSNT... MSN.... DMNT... GLI.... SAO.... HSR.... MSN.... DHIT... GCO.... SIR.... HBO.... MLS.... DD3T... GAM.... SDU.... HCF.... MSN.... DSWT... GCO.... SIR.... HGE.... MSN.... DIR.... GBR.... SAO.... HMI.... MSN.... DLVT.M. GHA.... SMS.... HFL.... MBO.... DMNT.M. GHA.... SMS.... HFL.... MBO.... DHI.VM. GNS.... SAO.... HGO.... MPA.... DD2T.M. GED.... SEC.... HMR.... MBU.... DLOT.M. GMNTTV. SLOTD.. HLE.... MLE.... DSWT... GLO.... SSWTD.. HEC.... MEC.... DIR..M. GLV.... SZA.... HNS.... MLO.... DDUT... GIR.... SVI.... HEC.... MSW.... DGWT... GDUT... SPR.... HAO.... MIR.... DD1T... GDU.... SHA.... HIR.... MDU...."
//# define xPastPlays "GZA.... SED.... HZU.... MZU.... DCD.V.. GSZ.... SNS.... HMU.... MMU...."
//# define xPastPlays "GGW.... SPL.... HCA.... MCG.... DST.V.. GDU.... SLO.... HLS.... MTS...."
//# define xPastPlays "GMN.... SPL.... HAM.... MPA.... DZU.V.. GLV.... SLO.... HNS.... MST...."
// Very far away near CD. 
//#define xPastPlays "GMN.... SPL.... HAM.... MPA.... DGA.V.. GLV.... SLO.... HNS.... MST...."
//#define xPastPlays "GED.... SMA.... HRO.... MGA.... DCD.V.. GNS.... SSR.... HFL.... MCDVD.. DKLT... GHA.... SMR.... HVE.... MGA.... DGAT... GVI.... SMI.... HVI.... MGATD.. DCNT... GZA.... SVE.... HBD.... MBE.... DVRT... GSZ.... SBD.... HGA.... MKLT... DD1T... GBC.... SGA.... HGA.... MGA.... DHIT... GCNT... SCD.... HCN.... MCN.... DSOT... GVRTTT. SKL.... HVR.... MVR.... DSJT... GSOT... SSZ.... HSO.... MSO.... DZAT... GSJT... SZATD.. HSJ.... MSJ.... DMUT... GZA.... SMUTD.. HZA.... MZA.... DSTT... GMU.... SZA.... HMU.... MMU.... DPAT... GZU.... SVI.... HVE.... MVE.... DNA.V.. GGE.... SBE.... HMU.... MMU.... DBOT... GCF.... SSZ.... HZU.... MLI.... DBB.... GNAV... SBD.... HMR.... MSTT... DSNT... GBOT... SVI.... HSR.... MPAT... DLST... GLE.... SVE.... HMA.... MCF.... DAO.... GBO.... SMI.... HSR.... MBO.... DGWT... GBB.... SMR.... HLST... MMA.... DD1T... GAO.... SBO.... HSNT... MLS.... DDUT... GCA.... SAL.... HLS.... MLS.... DIR.... GLS.... SMS.... HLS.... MLS.... DLVT... GAO.... SBA.... HLS.... MLS.... DSWT... GIR.... SSR.... HLS.... MLS.... DLOT.M. GDUT... SLE.... HAO.... MAO.... DD1.VM. GGW.... SEC.... HIR.... MIR.... DEC.... GDU.... SAO.... HDU.... MDU.... DPLT... GIR.... SCA.... HDU.... MDU.... DHIT.M. GAO.... SGR.... HDU.... MDU.... DTPT.M. GNS.... SAL.... HIR.... MIR.... DCDT.M. GHA.... SMS.... HAO.... MAO.... DGAT.V. GBR.... STS.... HNS.... MNS.... DKLT... GPR.... SIO.... HHA.... MHA.... DBCT.M. GSZ.... SBS.... HBR.... MBR.... DD1T.M. GBE.... SCN.... HVI.... MBD.... DSOT.M. GBCTT.. SBC.... HBC.... MZA.... DVAT.M. GSJ.... SSOT... HSO.... MSJ.... DIO..M. GVAT... SVA.... HVA.... MVA.... DBS..M. GAT.... SAT.... HAT.... MAT.... DCNT... GIO.... SVA.... HVA.... MVA.... DVRT... GAS.... SVA.... HVA.... MVA.... DD3.... GIO.... SVA.... HVA.... MVA.... DTPT... GBS.... SSO.... HSO.... MSO.... DCDT... GCNT... SBE.... HBE.... MBC.... DKLT... GGA.... SGA.... HKLTD.. MKLD... DSZT... GCDTT.. SCD.... HCD.... MCD.... DZAT.M. GKL.... SKL.... HKL.... MKL.... DMUT... GBE.... SBD.... HGA.... MKL.... DZUT... GBC.... SVI.... HKL.... MKL.... DMRT... GCN.... SPR.... HKL.... MKL.... DTOT... GVR.... SBR.... HSZT... MSZ.... DBO.V.. GSO.... SLI.... HVE.... MZAT... DD1T... GBE.... SCO.... HFL.... MMUT... DSRT... GGA.... SAM.... HMI.... MNU.... DHIT.M. GCD.... SBU.... HGE.... MFR.... DSNT.M. GKL.... SCO.... HCF.... MST.... DMAT.M. GSZ.... SBOTV.. HBO.... MZU.... DGRT... GPR.... SSRTT.. HPA.... MMR.... DCAT... GVI.... SAL.... HBU.... MGE.... DD1T... GVE.... SBA.... HAM.... MPA.... DAO.... GMI.... SSNT... HNS.... MBO.... DIR.... GGO.... SSN.... HAO.... MSR.... DSWT.M. GTS.... SLS.... HNS.... MSN.... DLOT.M. GMS.... SCATT.. HHA.... MMA.... DEC.... GAO.... SZA.... HNS.... MCA.... DLET... GMS.... SMU.... HEC.... MCA.... DBUT... GAL.... SZU.... HAO.... MCA.... DPAT... GGR.... SFL.... HIR.... MCA.... DD1T.M. GMA.... SBI.... HDU.... MCA.... DSTT.M. GPAT... SAS.... HIR.... MMA.... DGET... GZA.... SVE.... HAO.... MPAT... DMRT.M. GMU.... SFL.... HMS.... MNA.... DTOT.M. GLI.... SGET... HMRT... MCF.... DSRT... GSTT... SPA.... HPA.... MTOT... DMAT... GPA.... SBO.... HBO.... MSRT... DCAT... GBO.... SMAT... HMA.... MMA.... DAO.... GMA.... SGR.... HGR.... MGR.... DGW.V.. GAL.... SMA.... HMA.... MMA.... DDUT... GMS.... SLS.... HSR.... MMA.... DIR.... GAO.... SMA.... HMA.... MMA.... DD2T... GCAT... SBO.... HCA.... MCA.... DHIT... GAO.... SCF.... HCA.... MCA.... DTPT... GNS.... SPA.... HAO.... MAO.... DCDT.V. GHA.... SST.... HNS.... MNS.... DKLT.M. GBR.... SMU.... HHA.... MHA.... DSZT... GBD.... SZA.... HBR.... MBR.... DBET.M. GSZT... SSZ.... HVI.... MBD.... DSJT.M. GBC.... SSO.... HBC.... MZA.... DD1T.M. GBET... SVA.... HSO.... MSJTTD. DZAT.M. GBE.... SSO.... HBE.... MBE.... DMU.VM. GSZ.... SSJ.... HSZ.... MVI.... DSTT... GBC.... SVA.... HBD.... MMUV... DPAT... GSZ.... SIO.... HZAT... MSTT... DBUT... GVI.... STS.... HMU.... MZA.... DD1T... GPR.... SRO.... HLI.... MBD.... DLET... GBR.... SFL.... HBUTT.. MZA.... DNAT... GFR.... SST.... HZA.... MMU.... DCFT... GCO.... SGE.... HVI.... MMI.... DTOT.M. GAM.... SPA.... HPR.... MGO.... DSRT... GBU.... SBO.... HHA.... MTS.... DD1T... GLET... SCFT... HCO.... MMS.... DMAT... GMR.... SNAT... HFR.... MAL.... DSNT... GMI.... SCF.... HBR.... MBA.... DHI.V.. GFL.... SPA.... HLI.... MTOT... DLST... GGE.... SBU.... HNU.... MTO.... DCAT.M. GRO.... SAM.... HMU.... MSRT... DAO.... GTS.... SNS.... HZU.... MSR.... DIR..M. GIO.... SAO.... HMR.... MSR.... DDUT.M. GAT.... SCAT... HMS.... MBO.... DGWT.V. GIO.... SZA.... HAL.... MBU.... DD2T.M. GTS.... SVI.... HBA.... MAM.... DHIT... GGO.... SBR.... HMA.... MCO.... DTPT... GVE.... SBD.... HAL.... MFR.... DGAT... GBD.... SKL.... HMS.... MBR.... DCNT.M. GSZ.... SSZ.... HTS.... MBD.... DVRT.M. GBE.... SCNT... HIO.... MSZ.... DSO.VM. GKL.... SGAT... HBS.... MKL.... DSJT.M. GBC.... SCDT... HVRT... MBC.... DZAT... GSOV... SKL.... HSO.... MSO.... DMUT... GBE.... SBD.... HSA.... MSO.... DZUT... GSJT... SCN.... HVA.... MSJ.... DGET... GZAT... SBC.... HAT.... MZA.... DMRT... GVI.... SGA.... HVA.... MSZ.... DTOT... GPR.... SCD.... HSO.... MZA.... DD1T... GBR.... SKL.... HVR.... MMUT... DBOT... GFR.... SBE.... HSO.... MVE.... DSRT.M. GCO.... SSZ.... HBE.... MFL.... DSNT.M. GAM.... SVI.... HVI.... MGO.... DMAT.M. GBU.... SHA.... HBD.... MMI.... DCA.VM. GBOT... SLI.... HVI.... MGE.... DD1T.M. GSRT... SFR.... HVE.... MCF.... DLST... GAL.... SCO.... HMU.... MMR.... DAO.... GSNT... SBO.... HZU.... MMS.... DNS.... GSN.... SBB.... HMR.... MTS.... DHAT.M. GSR.... SAO.... HBO.... MCG.... DBRT.V. GBA.... SNS.... HMR.... MTS.... DD1T.M. GMA.... SAM.... HMI.... MRO.... DLIT.M. GCA.... SNS.... HFL.... MBI.... DCOT... GCA.... SNS.... HFL.... MBI.... DFRT... GAO.... SHAT... HGE.... MAS.... DSTT... GNS.... SBRTT.. HMR.... MVE.... DZUT... GHA.... SZA.... HGO.... MVI.... DD2.V.. GCOT... SMU.... HMR.... MLIT... DPAT... GAM.... SZA.... HCF.... MFRT... DLET... GBU.... SBD.... HNA.... MNU.... DEC.... GMR.... SZA.... HBB.... MFR.... DLOT.M. GMS.... SVI.... HBO.... MFR.... DPLT.M. GAL.... SVE.... HLET... MSTV... DD1T... GSR.... SMU.... HBU.... MGE.... DHIT.M. GLS.... SMI.... HAM.... MCF.... DTPT... GAO.... SVE.... HCO.... MMR.... DGAT... GNS.... SBD.... HST.... MMI.... DBCT.M. GHA.... SGAT... HNU.... MVE.... DSOT.M. GVI.... SCDT... HPR.... MBD.... DSAT.M. GZA.... SKL.... HBD.... MCN.... DIO..M. GSZ.... SGA.... HGA.... MBS.... DTS.... GGA.... SGA.... HGA.... MIO.... DGOT... GGA.... SGA.... HGA.... MAT.... DMRT.M. GBC.... SGA.... HGA.... MVA.... DD1T.M. GBE.... SGA.... HGA.... MSAT... DZUT... GSZ.... SCD.... HKL.... MIO.... DSTT... GVE.... SKL.... HBE.... MSA.... DPAT... GAS.... SBD.... HSJ.... MVR.... DHIT.M. GBI.... SVE.... HSO.... MSO.... DNAT.M. GRO.... SGO.... HSJ.... MSA.... DBOT.M. GRO.... SGO.... HSJ.... MSA.... DSRT.M. GFL.... SMR.... HZA.... MSZ.... DSNT.M. GMI.... SBOT... HMU.... MZA.... DLS.VM. GMR.... SAL.... HZU.... MMU.... DHIT.M. GSRT... SGR.... HGE.... MZU.... DAO..M. GSNT... SAL.... HPA.... MMR.... DNS.... GBB.... SMS.... HSR.... MMS.... DHAT... GAO.... SCG.... HSN.... MAL.... DBRT... GNS.... SMS.... HLSTV.. MGR.... DPRT... GAM.... SBA.... HCA.... MMA.... DVIT... GNS.... SBO.... HLS.... MLS.... DBDT... GHAT... SBU.... HAO.... MAO.... DKLT... GBRT... SST.... HNS.... MNS.... DCDT... GPRT... SFR.... HHA.... MHA.... DD1T... GSZ.... SBR.... HVIT... MBR.... DHIT... GGA.... SBDT... HBD.... MPR.... DGA.V.. GGAVD.. SKLT... HSZ.... MBD.... DCNT... GBC.... SBC.... HBC.... MBC.... DBS.... GCNT... SSZ.... HSO.... MSO.... DIO..M. GGA.... SGA.... HBC.... MBE.... DTS..M. GCN.... SCN.... HCN.... MSZ.... DGOT.M. GBS.... SCN.... HCN.... MVE.... DMRT... GIO.... SVR.... HCN.... MFL.... DTOT... GAS.... SBE.... HCN.... MGOT... DBOT... GVE.... SSZ.... HBC.... MGE.... DSRT... GMI.... SBD.... HSZ.... MST.... DMAT... GMRT... SVI.... HVE.... MBU.... DLST... GCF.... SBD.... HMU.... MAM.... DD2T... GTOT... SVI.... HZU.... MCO.... DGR.V.. GSRT... SVE.... HMR.... MBOT... DHIT... GZA.... SPR.... HSR.... MNA.... DCAT... GSJ.... SVE.... HAL.... MPA.... DAO..M. GZA.... SAS.... HMS.... MSR.... DNS..M. GMU.... SIO.... HAO.... MSR.... DEDT.M. GZU.... SAT.... HCAT... MAL.... DMNT.V. GST.... SIO.... HAO.... MBA.... DHIT.M. GCO.... STS.... HIR.... MMA.... DD3T... GAM.... SGO.... HDU.... MBO.... DTPT... GNS.... SVE.... HIR.... MTO.... DKLT... GHA.... SBD.... HAO.... MMR.... DGAT.M. GVI.... SKLT... HNS.... MZU.... DCNT.M. GBD.... SCDT... HHA.... MMU.... DBS..M. GKL.... SKL.... HVI.... MZA.... DIO..M. GSZ.... SKL.... HBD.... MSZ.... DTS.... GVE.... SKL.... HKL.... MCNT... DMS.... GAS.... SSZ.... HSZ.... MCN.... DCGT.M. GIO.... SCN.... HCN.... MCN.... DD1T... GTS.... SBE.... HCN.... MCN.... DHIT... GMS.... SBC.... HVR.... MCN.... DTPT... GTS.... SGA.... HBE.... MGA.... DCDT... GIO.... SGA.... HBE.... MGA...."
// Got caught in first round
//#define xPastPlays "GED.... SGE.... HZU.... MCA.... DCF.V.. GMN.... SCFVD.. HGE.... MLS...."
// Another far away near CD.
//#define  xPastPlays "GSW.... SLS.... HMR.... MHA.... DSJ.V.. GLO.... SAL.... HCO.... MBR...."
//#define xPastPlays "GAO.... SAO.... HAO.... MCD...."
//#define xPastPlays "GSW.... SLS.... HMR.... MHA.... DCD.V.. GSW.... SLS.... HMR.... MHA.... DGA.V.. GSW.... SLS.... HMR.... MHA.... DKL.V.. GSW.... SLS.... HMR.... MHA.... DD3.V.. GSW.... SLS.... HMR.... MHA.... DHI.V.." 
					//"GSW.... SLS.... HMR.... MHA.... DSJ.V.. GLO.... SAL.... HCO.... MBR.... DBET... GED.... SBO.... HLI.... MPR.... DKLT... GLV.... SNA.... HNU.... MBD.... DCDT... GIR.... SPA.... HPR.... MKLT..." 


//#define xPastPlays "GED.... SMA.... HRO.... MGA...."
// PLAYING AGAINST G70
//#define xPastPlays "GGA.... SRO.... HMA.... MBU.... DCD.V.. GCDVD.. SMI.... HPA.... MCO.... DD1T... GCDTD.. SMU.... HGE.... MFR.... DHIT... GBE.... SVI.... HMI.... MBR.... DGAT... GKL.... SBD.... HVE.... MBD.... DCNT... GCDT... SKL.... HBD.... MKL.... DBS.... GCD.... SCD.... HKL.... MCD.... DIO.... GCD.... SGAT... HGA.... MGA.... DVAT... GCD.... SCD.... HCNT... MCN...."
#define xMsgs { "", "", "", "", "", "","","","","","","","","","","","","","","" }

#else

typedef HunterView View;

# define ViewNew HvNew
# define decideMove decideHunterMove
# define ViewFree HvFree
/*
# define xPastPlays (\
	"GED.... STS.... HAO.... MBS.... DCD.V.." \
)
*/
#define xPastPlays "GED.... SMA.... HRO.... MGA.... DZA.V.. GMN.... SBO.... HMI.... MCD.... DSJT... GLO.... SNA.... HZU.... MKL.... DVAT... GSW.... SLE.... HST.... MBE.... DATT... GLV.... SPA.... HLI.... MBD.... DHIT... GLV.... SPA.... HLI.... MBD.... DIO.... GLV.... SMR.... HNU.... MVI.... DSAT.V. GED.... SMI.... HMU.... MVE.... DSOT.M. GNS.... SVE.... HVE.... MAS.... DVRT.M. GHA.... SAS.... HAS.... MAS.... DCNT.M. GHA.... SAS.... HAS.... MVE.... DHIT.M. GVI.... SAS.... HAS.... MAS.... DGAT... GMU.... SAS.... HAS.... MAS.... DCDT.M. GZA.... SVE.... HIO.... MVE.... DKL.VM. GSZ.... SBD.... HBS.... MVI.... DBCT.M. GGAT... SKLV... HCNTT.. MBD.... DSOT... GCDT... SCD.... HGA.... MGA.... DVAT... GKL.... SKL.... HSZ.... MKL.... DATT... GSZ.... SBD.... HZA.... MBD.... DHIT... GVE.... SVI.... HVI.... MVE.... DIO.... GAS.... SVE.... HVE.... MAS.... DSAT.M. GAS.... SAS.... HAS.... MAS.... DSOT.M. GAS.... SAS.... HAS.... MAS.... DVRT.M. GAS.... SAS.... HAS.... MAS.... DCNT.M. GIO.... SVE.... HIO.... MIO.... DHIT.M. GAS.... SAS.... HAS.... MAS.... DGAT... GAS.... SAS.... HAS.... MAS.... DCD.VM. GIO.... SVE.... HVE.... MVE.... DKLT.M. GBS.... SVI.... HBD.... MBD.... DSZT.M. GCNTT.. SBD.... HGAT... MKLT... DZAT... GGA.... SGA.... HCDV... MCD.... DMUT... GSZT... SKL.... HKL.... MKL.... DZUT... GZAT... SSZ.... HSZ.... MSZ.... DMRT... GMUT... SVI.... HVI.... MZA.... DTOT... GZA.... SMU.... HMU.... MMU.... DHIT... GMU.... SVE.... HVE.... MVE.... DBAT... GVE.... SAS.... HAS.... MAS.... DSRT... GAS.... SAS.... HAS.... MAS.... DSNT.M. GAS.... SAS.... HAS.... MAS.... DMAT.M. GAS.... SAS.... HAS.... MAS.... DLS.VM. GIO.... SVE.... HVE.... MVE.... DHIT.M. GAS.... SAS.... HAS.... MAS.... DCAT.M. GAS.... SAS.... HAS.... MAS.... DGRT.M. GAS.... SAS.... HAS.... MAS.... DALT.M. GAS.... SAS.... HAS.... MAS.... DMAT.M. GAS.... SAS.... HAS.... MAS.... DSNT.V. GIO.... SVE.... HIO.... MIO.... DHIT.M. GAS.... SAS.... HAS.... MAS.... DLST.M. GAS.... SAS.... HAS.... MAS.... DCAT.M. GAS.... SAS.... HAS.... MAS.... DGRT.M. GAS.... SAS.... HAS.... MAS.... DMAT.M. GAS.... SAS.... HAS.... MAS.... DSNT.M. GIO.... SIO.... HIO.... MIO.... DHI.VM. GAS.... SAS.... HAS.... MAS.... DLST.M. GAS.... SAS.... HAS.... MAS.... DCAT.M. GAS.... SAS.... HAS.... MAS.... DGRT.M. GAS.... SAS.... HAS.... MAS.... DMAT.M. GAS.... SAS.... HAS.... MAS.... DSNT.M. GIO.... SVE.... HIO.... MIO.... DHIT.V. GAS.... SAS.... HAS.... MAS.... DLST.M. GAS.... SAS.... HAS.... MAS.... DCAT.M. GAS.... SAS.... HAS.... MAS.... DGRT.M. GAS.... SAS.... HAS.... MAS.... DMAT.M. GAS.... SAS.... HAS.... MAS.... DSNT.M. GIO.... SIO.... HIO.... MIO.... DHIT.M. GAS.... SAS.... HAS.... MAS.... DLS.VM. GAS.... SAS.... HAS.... MAS.... DCAT.M. GAS.... SAS.... HAS.... MAS.... DGRT.M. GAS.... SAS.... HAS.... MAS.... DMAT.M. GAS.... SAS.... HAS.... MAS.... DSNT.M. GIO.... SVE.... HIO.... MIO.... DHIT.M. GAS.... SAS.... HAS.... MAS.... DLST.V. GAS.... SAS.... HAS.... MAS.... DCAT.M. GAS.... SAS.... HAS.... MAS.... DGRT.M. GAS.... SAS...."
# define xMsgs {}

#endif

int main(void)
{
	char *pastPlays = xPastPlays;
	Message msgs[] = xMsgs;

	View state = ViewNew(pastPlays, msgs);
	decideMove(state);
	ViewFree(state);

	printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
	return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay(char *play, Message message)
{
	strncpy(latestPlay, play, MOVE_SIZE - 1);
	latestPlay[MOVE_SIZE - 1] = '\0';

	strncpy(latestMessage, message, MESSAGE_SIZE - 1);
	latestMessage[MESSAGE_SIZE - 1] = '\0';
}
