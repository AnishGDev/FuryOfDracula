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

# define xPastPlays "GZA.... SED.... HZU.... MZU...."
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
#define xPastPlays "GED.... SMA.... HRO.... MGA.... DZU.V.. GMN.... SBO.... HMI.... MCD.... DC?T... GLO.... SNA.... HZUV... MGA.... DCOT... GEC.... SCF.... HMR.... MKL.... DD2T... GAO.... SBO.... HGO.... MBE.... DMUT... GNS.... SBU.... HFL.... MSZ.... DHIT... GAM.... SPA.... HRO.... MZA.... DC?T... GCOT... SGE.... HFL.... MMUTT.. DMRT.M. GAM.... SCF.... HVE.... MVI.... DS?.... GBU.... SBO.... HSZ.... MBD.... DS?..M. GMRT... SNA.... HBE.... MKL.... DC?T... GCF.... SCF.... HBC.... MBD.... DC?T... GNA.... SGE.... HSZ.... MVE.... DHIT.M. GBB.... SFL.... HVI.... MMU.... DS?.... GAO.... SBI.... HMU.... MZU.... DD3T... GCA.... SNP.... HST.... MGO.... DTPT... GGR.... STS.... HMU.... MVE.... DC?T.M. GAL.... SIO.... HMI.... MFL.... DC?T.M. GMS.... SAT.... HMU.... MVE.... DC?T.M. GMR.... SVA.... HMI.... MMU.... DCDT... GMI.... SAT.... HMU.... MMI.... DD1T.M. GMU.... SVA.... HMI.... MMU.... DHIT.M. GMI.... SAT.... HMU.... MMI.... DC?T.M. GMR.... SVA.... HMI.... MMU.... DC?T.M. GMI.... SAT.... HMU.... MMI.... DBCT.M. GMU.... SVA.... HMI.... MMU.... DKLT.M. GZA.... SSO.... HVE.... MZA.... DCD.VM. GBD.... SBE.... HFL.... MBD.... DD1T.M. GKLT... SBCT... HVE.... MKL.... DHIT.M. GBE.... SKL.... HFL.... MBE.... DC?T.M. GKL.... SBE.... HGO.... MBC.... DCNT... GBE.... SBC.... HFL.... MCNTD.. DS?.... GBC.... SCN.... HVE.... MBE.... DS?..V. GBE.... SVR.... HSZ.... MBC.... DS?..M. GKL.... SCN.... HBE.... MBE.... DMS..M. GCD.... SBS.... HSO.... MSA.... DBAT.M. GCD.... SIO.... HSA.... MIO.... DC?T... GCD.... STS.... HSO.... MAT.... DSNT... GCD.... SMS.... HSJ.... MIO.... DD1T... GCD.... SAL.... HZA.... MTS.... DHI.V.. GCD.... SAL.... HZA.... MTS.... DS?.... GCD.... SSNTTV. HMU.... MCG.... DNAT.M. GGA.... SBB.... HMI.... MMS.... DC?T.M. GBD.... SSN.... HMR.... MAL.... DS?.... GKL.... SMA.... HBO.... MBO.... DC?T... GCD.... SCA.... HNAT... MCF.... DMNT... GCD.... SLS.... HPA.... MTO.... DD1T... GCD.... SMA.... HBU.... MSR.... DHIT... GCD.... SGR.... HAM.... MPA.... DC?T.M. GCD.... SMA.... HCO.... MGE.... DC?T... GCD.... SBA.... HLI.... MMI.... DLOT.M. GCD.... SBA.... HLI.... MMI.... DMNT.M. GCD.... SMS.... HCO.... MMR.... DD1.VM. GCD.... SAO.... HLE.... MMS.... DHIT.M. GCD.... SNS.... HEC.... MAO.... DLVT.M. GCD.... SED.... HLOT... MIR.... DS?..M. GCD.... SMNTTV. HED.... MLVT... DS?.... GGA.... SMN.... HSW.... MMN.... DCAT... GKL.... SED.... HLV.... MLO.... DGRT... GCD.... SNS.... HIR.... MEC.... DC?T... GCD.... SAO.... HAO.... MAO.... DALT... GCD.... SMS.... HCAT... MCA.... DC?T... GCD.... SALT... HGRT... MGR.... DC?T... GGA.... SGR.... HAL.... MAL.... DMRT... GBC.... SAL.... HMS.... MBO.... DC?T... GGA.... SBA.... HMRT... MCF.... DC?.VM. GKL.... SMS.... HCF.... MBO.... DZAT... GBE.... SAL.... HBO.... MLE.... DBDT.M. GKL.... SSR.... HBU.... MNA.... DD2T.M. GCD.... SMA.... HAM.... MPA.... DC?T... GCD.... SCA.... HCO.... MGE.... DC?T.M. GCD.... SGR.... HFR.... MGO.... DC?T.V. GCD.... SMA.... HLI.... MFL.... DCNT.M. GCD.... SMA.... HLI.... MFL.... DHIT.M. GCD.... SBO.... HBR.... MMI.... DC?T.M. GCD.... SCF.... HPR.... MMU.... DD5T.M. GCD.... SPA.... HHA.... MNU.... DC?T.M. GCD.... SBU.... HVI.... MPR.... DSAT.M. GCD.... SBU.... HVI.... MPR.... DIO..M. GCD.... SST.... HVE.... MBD.... DS?..M. GCD.... SZU.... HAS.... MVE.... DS?..M. GCD.... SGO.... HIO.... MGO.... DBAT.M. GCD.... SRO.... HAT.... MTS.... DC?T.M. GCD.... SRO.... HAT.... MTS.... DC?T.M. GCD.... SFL.... HIO.... MMS.... DD1T... GCD.... SMI.... HVA.... MAL.... DLST... GCD.... SRO.... HSA.... MBAT... DCAT... GCD.... STS.... HIO.... MMA.... DS?.... GCD.... SMS.... HTS.... MLST... DS?..M. GGA.... SAL.... HCG.... MCAT... DNAT.M. GCN.... SMS.... HMS.... MLS.... DBOT.M. GGA.... SBA.... HMR.... MAO.... DD2.V.. GCD.... SAL.... HBU.... MIR.... DC?T... GCD.... SGR.... HAM.... MDU.... DC?T... GCD.... SMA.... HCO.... MIR.... DC?T... GCD.... SSR.... HFR.... MDU.... DVET.M. GCD.... SSR.... HFR.... MDU.... DFLT.M. GCD.... SBA.... HBR.... MIR.... DC?T.V. GCD.... SBO.... HHA.... MAO.... DHIT.M. GCD.... SCF.... HLI.... MMS.... DC?T.M. GCD.... SGE.... HMU.... MAL.... DAS..M. GCD.... SGE.... HMU.... MAL.... DIO..M. GCD.... SGO.... HVE.... MMS.... DVAT.M. GCD.... STS.... HMI.... MBA.... DC?T.M. GCD.... SIO.... HFL.... MMA.... DC?.VM. GCD.... SSA.... HST.... MCA.... DD1T.M. GCD.... SSA.... HST.... MCA.... DCNT... GCD.... SVAT... HMU.... MAO.... DVRT... GCD.... SAT.... HZA.... MMS.... DHIT... GCD.... SVA.... HSJ.... MMR.... DS?..M. GCD.... SIO.... HVA.... MMS.... DS?..V. GCD.... SAS.... HIO.... MAO.... DS?..M. GCD.... SAS.... HIO.... MAO.... DGOT.M. GCD.... SBI.... HTS.... MMS.... DC?T.M. GCD.... SFL.... HRO.... MTS.... DMUT.M. GCD.... SGOT... HTS.... MCG.... DC?T... GGA.... SMR.... HCG.... MMS.... DNUT... GCD.... SBU.... HTS.... MMR.... DD3.V.. GCD.... SAM.... HMS.... MPA.... DC?T... GCD.... SCO.... HTS.... MBO.... DHIT.M. GCD.... SFR.... HRO.... MBA.... DC?T.M. GCD.... SBR.... HBI.... MTO.... DSOT.M. GCD.... SBR.... HBI.... MTO.... DVRT.M. GCD.... SHA.... HRO.... MSR.... DC?T.V. GCD.... SLI.... HFL.... MBU.... DC?T.M. GCD.... SCO.... HGE.... MLE.... DC?T.M. GCD.... SMR.... HST.... MNA.... DBET.M. GCD.... SMR.... HST.... MNA.... DSJT.M. GCD.... SZU.... HMU.... MPA.... DC?T.M. GCD.... SGO.... HZA.... MST.... DC?T.M. GCD.... SMI.... HSJT... MMU.... DHI.VM. GGA.... SGO.... HBET... MVE.... DBS..M. GCD.... STS.... HSA.... MAS.... DCNT... GCD.... SIO.... HIO.... MBI.... DC?T... GCD.... SAT.... HTS.... MRO.... DGAT.M. GCD.... SVA.... HGO.... MNP.... DD3T.M. GCD.... SSO.... HVE.... MMI.... DVRT.V. GCD.... SSO.... HVE.... MMI.... DS?.... GCD.... SSJ.... HFL.... MMU.... DS?..M. GGAT... SSO.... HGO.... MMI.... DS?..M. GCD.... SVRT... HVE.... MNP.... DVET... GGA.... SCNT... HMI.... MRO.... DGOT... GKL.... SBS.... HGOTD.. MBI.... DMRT... GBD.... SCN.... HMRT... MAS.... DC?.V.. GKL.... SBC.... HSJ.... MVET... DC?T... GBE.... SSO.... HBE.... MGO.... DD1T... GBC.... SSJ.... HBC.... MMR.... DSNT... GGA.... SZA.... HSZ.... MPA.... DLST... GCD.... SVI.... HBD.... MBU.... DC?T... GCD.... SVE.... HSO.... MAM.... DC?T.V. GCD.... SBR.... HSA.... MBU.... DALT.M. GCD.... SHA.... HVA.... MPA.... DD1T.M. GCD.... SHA.... HVA.... MPA.... DSRT.M. GCD.... SNS.... HIO.... MNA.... DHIT.M. GCD.... SAO.... HTS.... MBO.... DC?T.M. GCD.... SMS.... HMS.... MCF.... DMAT.M. GCD.... SALTT.. HBA.... MTO.... DCA.V.. GCD.... SSRTT.. HTO.... MBA.... DLST... GCD.... SZA.... HSR.... MSR.... DD2T... GCD.... SMU.... HMAT... MBA.... DS?.... GCD.... SZU.... HLST... MAL.... DS?..M. GGA.... SMR.... HCATV.. MMA.... DC?T... GCN.... SGO.... HGR.... MCA.... DMNT... GGA.... SVE.... HMA.... MAO.... DHIT... GCD.... SFL.... HBO.... MIR.... DD2T... GCD.... SVE.... HBU.... MDU.... DC?T... GCD.... SBD.... HAM.... MIR.... DC?T... GCD.... SVI.... HCO.... MDU.... DS?..M. GCD.... SVI.... HCO.... MDU.... DS?..M. GCD.... SPR.... HHA.... MIR.... DC?.VM. GCD.... SHA.... HNS.... MDU.... DC?T.M. GCD.... SBR.... HAO.... MIR.... DALT.M. GCD.... SLI.... HMS.... MLV.... DSRT.M. GCD.... SCO.... HALT... MSW.... DHIT... GGA.... SLE.... HSRTT.. MLV.... DC?T... GCN.... SBU.... HBE.... MSW.... DD3T.V. GBC.... SPA.... HSA.... MIR.... DC?T.M. GBE.... SGE.... HIO.... MAO.... DALT... GSA.... SCF.... HTS.... MBB.... DGRT... GIO.... SBO.... HCG.... MAO.... DCAT... GBS.... SSRT... HTS.... MCATD.. DLST.M. GIO.... SLSTD.. HCG.... MGRT... DC?T... GAS.... SAO.... HMS.... MALT... DD1.VM. GBI.... SEC.... HAO.... MBE.... DHIT... GNP.... SAO.... HCA.... MVI.... DS?.... GTS.... SBB.... HAO.... MBD.... DBOT... GMS.... SBOT... HMS.... MZA.... DC?T... GAO.... SSJ.... HAO.... MBD.... DSRT.M. GCA.... SBE.... HCA.... MBR.... DBAT.V. GGR.... SBC.... HLS.... MHA.... DD3T.M. GAL.... SGA.... HAO.... MLI.... DC?T... GBAT... SBC.... HBB.... MHA.... DC?T..."
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
