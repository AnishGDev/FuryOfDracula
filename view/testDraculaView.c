////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testDraculaView.c: test the DraculaView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DraculaView.h"
#include "Game.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for basic functions, "
			   "just before Dracula's first move\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "..."
		};
		
		DraculaView dv = DvNew(trail, messages);
		printf("Created Dracula. Now testing");
		assert(DvGetRound(dv) == 0);
		assert(DvGetScore(dv) == GAME_START_SCORE);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == ZURICH);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE);
		assert(DvGetVampireLocation(dv) == NOWHERE);
		int numTraps = -1;
		printf("Testing traps");
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		printf("Number of Traps is %d\n", numTraps);
		assert(numTraps == 0);
		free(traps);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD.. SAO.... HCD.... MAO....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahah",
			"Aha!", "", "", ""
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);
		assert(DvGetScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(DvGetHealth(dv, PLAYER_LORD_GODALMING) == 5);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == 30);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
		assert(DvGetVampireLocation(dv) == NOWHERE);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula leaving minions 1\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DED.V.. "
			"GST.... SST.... HST.... MST.... DMNT... "
			"GST.... SST.... HST.... MST.... DLOT... "
			"GST.... SST.... HST.... MST.... DHIT... "
			"GST.... SST.... HST.... MST....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 4);
		assert(DvGetVampireLocation(dv) == EDINBURGH);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 3);
		sortPlaces(traps, numTraps);
		printf("Trap loc is %s\n", placeIdToName(traps[0]));
		assert(traps[0] == LONDON);
		assert(traps[1] == LONDON);
		assert(traps[2] == MANCHESTER);
		free(traps);
		
		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		for (int i = 0; i < numMoves; i++) {
			printf("Mov: %s\n", placeIdToName(moves[i]));
		}
		assert(numMoves == 4);
		sortPlaces(moves, numMoves);
		for (int i = 0; i < numMoves; i++) {
			printf("%s\n", placeIdToName(moves[i]));
		}
		assert(moves[0] == GALATZ);
		assert(moves[1] == KLAUSENBURG);
		assert(moves[2] == HIDE);
		assert(moves[3] == DOUBLE_BACK_1);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGo 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DKL.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GGE.... SGE.... HGE.... MGE.... DBCT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == CONSTANTA);
		assert(locs[2] == GALATZ);
		assert(locs[3] == SOFIA);
		free(locs);
		
		printf("Test passed!\n");
		DvFree(dv);
	}

	printf("/////////////END OF Given Tests//////\n");

	{//Testing for Dracula With 1 Move
		printf("Custom Test Could be wrong-Nikhil\n");
		printf("Test for Dracula With No Moves\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DDU.V.. "
			"GST.... SST.... HST.... MST.... DIR.... "
			"GST.... SST.... HST.... MST.... DAO.... "
			"GST.... SST.... HST.... MST.... DGWT... "
			"GST.... SST.... HST.... MST.... DHIT... "
			"GST.... SST.... HST.... MST.... DD5T... "	//Pretty sure its D4
			"GST.... SST.... HST.... MST.... ";


		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 6);
		assert(DvGetVampireLocation(dv) == DUBLIN);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == DUBLIN);

		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 3);
		sortPlaces(traps, numTraps);
		assert(traps[0] == DUBLIN);
		assert(traps[1] == GALWAY);
		assert(traps[2] == GALWAY);
		free(traps);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);

		assert(numMoves == 1);
		assert(moves[0] == DUBLIN);
		free(moves);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Test for traps falling off the trail and if vampires mature properly.
		printf("Custom Test Could be wrong-Nikhil\n");
		printf("Test for traps falling off the trail and if vampires mature properly.\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GST.... SST.... HST.... MST.... DKLT... "
			"GST.... SST.... HST.... MST.... DSZT... "
			"GST.... SST.... HST.... MST.... DBDT... "
			"GST.... SST.... HST.... MST.... DVIT... "
			"GST.... SST.... HST.... MST.... DZAT.V. "
			"GST.... SST.... HST.... MST.... DMUT... "
			"GST.... SST.... HST.... MST....";


		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 7);
		assert(DvGetVampireLocation(dv) == NOWHERE);
		assert(DvGetScore(dv) == GAME_START_SCORE - 7 * SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_VAMPIRE_MATURES);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == MUNICH);

		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 6);
		sortPlaces(traps, numTraps);
		assert(traps[0] == BUDAPEST);
		assert(traps[1] == KLAUSENBURG);
		assert(traps[2] == MUNICH);
		assert(traps[3] == SZEGED);
		assert(traps[4] == VIENNA);
		assert(traps[5] == ZAGREB);
		free(traps);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing Correct Starting Score
		printf("Test for Get Score\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... ";
		
		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);
		
		assert(DvGetScore(dv) == GAME_START_SCORE);
				
		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing 3 Traps on 1 Location
		printf("Testing for 3 encounters on 1 Location\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GST.... SST.... HST.... MST.... DKLT... "
			"GST.... SST.... HST.... MST.... DHIT... "
			"GST.... SST.... HST.... MST.... DD1T... "
			"GST.... SST.... HST.... MST....";
		
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		
		assert(numTraps == 3);
		sortPlaces(traps, numTraps);
		assert(traps[0] == KLAUSENBURG);
		assert(traps[1] == KLAUSENBURG);
		assert(traps[2] == KLAUSENBURG);

		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{//Testing Dracula Dies
		printf("Testing Dracula Dies\n");

		char * trail = 
			"GST.... SPA.... HCF.... MMR.... DGE.V.. "
			"GGEVD.. SGED... HGED... MGED... ";
		
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);
		assert(DvGetVampireLocation(dv) == NOWHERE);

		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
		
		assert(DvGetHealth(dv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(DvGetHealth(dv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(DvGetHealth(dv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(DvGetHealth(dv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - (NUM_PLAYERS-1)*(LIFE_LOSS_HUNTER_ENCOUNTER));
	
		assert(DvGetScore(dv) == GAME_START_SCORE - 1);

		printf("Test Passed!\n");
		DvFree(dv);
	}

	{//Testing GetPlayerLocation for DoubleBack
		printf("Testing GetPlayerLocation for DoubleBack\n");

		char *trail = 
			"GKL.... SGE.... HGE.... MGE.... DCD.V.. "
			"GKL.... SGE.... HGE.... MGE.... DD1T... "
			"GKL.... SGE.... HGE.... MGE.... ";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == CASTLE_DRACULA);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 2*LIFE_GAIN_CASTLE_DRACULA);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing GetPlayerLocation and DvGetValidMoves for DoubleBack and Hide
		printf("Testing GetPlayerLocation for DoubleBack and Hide and DvGetValidMoves for no DOUBLEBACK and HIDE\n");

		char *trail = 
			"GKL.... SGE.... HGE.... MGE.... DCD.V.. "
			"GKL.... SGE.... HGE.... MGE.... DD1T... "
			"GKL.... SGE.... HGE.... MGE.... DHIT... "
			"GKL.... SGE.... HGE.... MGE.... ";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == CASTLE_DRACULA);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS + 3*LIFE_GAIN_CASTLE_DRACULA);

		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 2);
		sortPlaces(moves, numMoves);
		assert(moves[0] == GALATZ);
		assert(moves[1] == KLAUSENBURG);

		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{//Testing WhereCanIGo
		printf("Testing WhereCanIGo\n");

		char *trail = 
			"GKL.... SGE.... HGE.... MGE.... DMU.V.. "
			"GKL.... SGE.... HGE.... MGE....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == MUNICH);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 8);
		sortPlaces(locs, numLocs);

		assert(locs[0] == MILAN);
		assert(locs[1] == MUNICH); // HIDE
		assert(locs[2] == NUREMBURG);
		assert(locs[3] == STRASBOURG);
		assert(locs[4] == VENICE);
		assert(locs[5] == VIENNA);
		assert(locs[6] == ZAGREB);
		assert(locs[7] == ZURICH);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing WhereCanTheyGo
		printf("Testing WhereCanTheyGo\n");

		char *trail = 
			"GKL.... SPA.... HLO.... MBS.... DSN.V.. "
			"GKL.... SPA.... HLO.... MBS....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == SANTANDER);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == KLAUSENBURG);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == PARIS);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == LONDON);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == BLACK_SEA);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanTheyGo(dv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BAY_OF_BISCAY);
		assert(locs[1] == LISBON);
		assert(locs[2] == MADRID);
		printf("it is %s and %s \n",placeIdToName(locs[3]), placeIdToName(locs[4]));
		assert(locs[3] == SANTANDER);
		assert(locs[4] == SARAGOSSA);
		free(locs);

		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_LORD_GODALMING, &numLocs);
		printf("NumLocs is %d\n", numLocs);
		for (int i =0; i < numLocs; i++) {
			printf("Location is %s \n", placeIdToName(locs[i]));
		}
		assert(numLocs == 7); // Should be 7 not 6.
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUCHAREST);
		assert(locs[2] == BUDAPEST);
		assert(locs[3] == CASTLE_DRACULA);
		assert(locs[4] == GALATZ);
		assert(locs[5] == KLAUSENBURG);
		assert(locs[6] == SZEGED);

		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_DR_SEWARD, &numLocs);
		assert(numLocs == 11);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == BRUSSELS);
		assert(locs[2] == CLERMONT_FERRAND);
		assert(locs[3] == COLOGNE);
		assert(locs[4] == GENEVA);
		assert(locs[5] == LE_HAVRE);
		assert(locs[6] == MARSEILLES);
		assert(locs[7] == NANTES);
		assert(locs[8] == PARIS);
		assert(locs[9] == SARAGOSSA);
		assert(locs[10] == STRASBOURG);
		free(locs);
	
		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_VAN_HELSING, &numLocs);
		assert(numLocs == 7);
		sortPlaces(locs, numLocs);
		assert(locs[0] == EDINBURGH);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LIVERPOOL); // Alphabetical order
		assert(locs[3] == LONDON);
		assert(locs[4] == MANCHESTER);
		assert(locs[5] == PLYMOUTH);
		assert(locs[6] == SWANSEA);
		free(locs);

		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_MINA_HARKER, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BLACK_SEA);
		assert(locs[1] == CONSTANTA);
		assert(locs[2] == IONIAN_SEA);
		assert(locs[3] == VARNA);
		free(locs);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing WhereCanTheyGoByType
		printf("Testing WhereCanTheyGoByType\n");

		char *trail = 
			"GKL.... SPA.... HLO.... MBS.... DSN.V.. "
			"GKL.... SPA.... HLO.... MBS....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == SANTANDER);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == KLAUSENBURG);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == PARIS);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == LONDON);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanTheyGoByType(dv, PLAYER_DRACULA, false, false, true, &numLocs);
		assert(numLocs == 2);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BAY_OF_BISCAY);
		assert(locs[1] == SANTANDER);
		free(locs);

		locs = DvWhereCanTheyGoByType(dv, PLAYER_DRACULA, true, false, false, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == LISBON);
		assert(locs[1] == MADRID);
		assert(locs[2] == SANTANDER);
		assert(locs[3] == SARAGOSSA);
		free(locs);

		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_LORD_GODALMING, false, false, true, &numLocs);
		assert(numLocs == 1);
		assert(locs[0] == KLAUSENBURG);
		
		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_LORD_GODALMING, false, true, true, &numLocs);
		assert(numLocs == 1);
		assert(locs[0] == KLAUSENBURG);
		free(locs);

		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_DR_SEWARD, false, true, true, &numLocs);
		assert(numLocs == 7);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == BRUSSELS);
		assert(locs[2] == COLOGNE);
		assert(locs[3] == LE_HAVRE);
		assert(locs[4] == MARSEILLES);
		assert(locs[5] == PARIS);
		assert(locs[6] == SARAGOSSA);
		free(locs);
	
		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_DR_SEWARD, true, false, false, &numLocs);
		assert(numLocs == 7);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == CLERMONT_FERRAND);
		assert(locs[2] == GENEVA);
		assert(locs[3] == LE_HAVRE);
		assert(locs[4] == NANTES);
		assert(locs[5] == PARIS);
		assert(locs[6] == STRASBOURG);
		free(locs);

		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_VAN_HELSING, false, false, true, &numLocs);
		assert(numLocs == 2);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ENGLISH_CHANNEL);
		assert(locs[1] == LONDON);

		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_VAN_HELSING, false, true, false, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == EDINBURGH);
		assert(locs[1] == LIVERPOOL);
		assert(locs[2] == LONDON);
		assert(locs[3] == MANCHESTER);
		assert(locs[4] == SWANSEA);
		free(locs);
		
		numLocs = -1;
		locs = DvWhereCanTheyGoByType(dv, PLAYER_MINA_HARKER, false, false, true, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BLACK_SEA);
		assert(locs[1] == CONSTANTA);
		assert(locs[2] == IONIAN_SEA);
		assert(locs[3] == VARNA);
		free(locs);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing Start of the game
		printf("Start of the game\n");

		char *trail =
			"";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == NOWHERE);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == NOWHERE);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE);

		int numLocs = -1;
		PlaceId *locs = DvGetValidMoves(dv, &numLocs);
		assert(numLocs == 0);
		assert(locs == NULL);
		numLocs = -1;
		locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 0);
		numLocs = -1;
		locs = DvWhereCanIGoByType(dv, true, false, &numLocs);
		assert(numLocs == 0);
		numLocs = -1;
		locs = DvWhereCanIGoByType(dv, true, true, &numLocs);
		assert(numLocs == 0);
		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_LORD_GODALMING, &numLocs);
		assert(numLocs == 0);
		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_MINA_HARKER, &numLocs);
		assert(numLocs == 0);
		numLocs = -1;
		locs = DvWhereCanTheyGo(dv, PLAYER_DR_SEWARD, &numLocs);
		assert(numLocs == 0);

		assert(DvGetRound(dv) == 0);
		assert(DvGetVampireLocation(dv) == NOWHERE);
		assert(DvGetScore(dv) == GAME_START_SCORE);

		assert(DvGetHealth(dv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(DvGetHealth(dv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(DvGetHealth(dv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(DvGetHealth(dv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);

		printf("Test passed!\n");
		free(dv);
	}

	{//Testing WhereCanIGo
		printf("Testing WhereCanIGo2\n");

		char *trail = 
			"GKL.... SGE.... HGE.... MGE.... DMU.V.. "
			"GKL.... SGE.... HGE.... MGE.... DHIT... "
			"GKL.... SGE.... HGE.... MGE....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == MUNICH);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 8);
		sortPlaces(locs, numLocs);

		assert(locs[0] == MILAN);
		assert(locs[1] == MUNICH);
		assert(locs[2] == NUREMBURG);
		assert(locs[3] == STRASBOURG);
		assert(locs[4] == VENICE);
		assert(locs[5] == VIENNA);
		assert(locs[6] == ZAGREB);
		assert(locs[7] == ZURICH);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing WhereCanIGo
		printf("Testing WhereCanIGo3\n");

		char *trail = 
			"GKL.... SGE.... HGE.... MGE.... DMU.V.. "
			"GKL.... SGE.... HGE.... MGE.... DHIT... "
			"GKL.... SGE.... HGE.... MGE.... DD1T... "
			"GKL.... SGE.... HGE.... MGE....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == MUNICH);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 7);
		sortPlaces(locs, numLocs);
		assert(locs[0] == MILAN);
		assert(locs[1] == NUREMBURG);
		assert(locs[2] == STRASBOURG);
		assert(locs[3] == VENICE);
		assert(locs[4] == VIENNA);
		assert(locs[5] == ZAGREB);
		assert(locs[6] == ZURICH);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{//Testing Get Moves Hiding at sea
		printf("Testing GetValidMoves hiding at sea\n");

		char *trail =
			"GKL.... SGE.... HGE.... MGE.... DAO.... "
			"GKL.... SGE.... HGE.... MGE....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - LIFE_LOSS_SEA);

		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		sortPlaces(moves, numMoves);
		assert(numMoves == 9);
		assert(moves[0] == BAY_OF_BISCAY);
		assert(moves[1] == CADIZ);
		assert(moves[2] == ENGLISH_CHANNEL);
		assert(moves[3] == GALWAY);
		assert(moves[4] == IRISH_SEA);
		assert(moves[5] == LISBON);
		assert(moves[6] == MEDITERRANEAN_SEA);
		assert(moves[7] == NORTH_SEA);
		assert(moves[8] == DOUBLE_BACK_1);

		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{//Testing DvWhereCanIGoByType at sea
		printf("Testing DvWhereCanIGoByType at sea\n");

		char *trail =
			"GKL.... SGE.... HGE.... MGE.... DAO.... "
			"GKL.... SGE.... HGE.... MGE....";

		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);

		int numMoves = -1;
		PlaceId *moves = DvWhereCanIGoByType(dv, false, true, &numMoves);
		sortPlaces(moves, numMoves);
		assert(numMoves == 9);
		assert(moves[0] == ATLANTIC_OCEAN);
		assert(moves[1] == BAY_OF_BISCAY);
		assert(moves[2] == CADIZ);
		assert(moves[3] == ENGLISH_CHANNEL);
		assert(moves[4] == GALWAY);
		assert(moves[5] == IRISH_SEA);
		assert(moves[6] == LISBON);
		assert(moves[7] == MEDITERRANEAN_SEA);
		assert(moves[8] == NORTH_SEA);

		printf("Test passed!\n");
		DvFree(dv);
	}
	
	{//Testing Hospital of St Joseph and St Mary
		printf("Testing Hospital of St Joseph and St Mary Conditions\n");

		char *trail = 
			"GKL.... SGE.... HGE.... MGE.... DSZ.V.. "
			"GKL.... SGE.... HGE.... MGE....";
		
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == SZEGED);
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == KLAUSENBURG);
		assert(locs[3] == SZEGED);
		assert(locs[4] == ZAGREB);
		free(locs);

		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 6);
		sortPlaces(moves, numLocs);
		assert(moves[0] == BELGRADE);
		assert(moves[1] == BUDAPEST);
		assert(moves[2] == KLAUSENBURG);
		assert(moves[3] == ZAGREB);
		assert(moves[4] == DOUBLE_BACK_1);
		assert(moves[5] == HIDE);
		free(moves);

		printf("Test passed!\n");
		DvFree(dv);

	}
	
	{//Testing DoubleBacks
		printf("Testing DoubleBack\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DMR.V.. "
			"GKL.... SKL.... HKL.... MKL.... DCFT... "
			"GKL.... SKL.... HKL.... MKL.... DPAT... "
			"GKL.... SKL.... HKL.... MKL.... DSTT... "
			"GKL.... SKL.... HKL.... MKL.... DZUT... "
			"GKL.... SKL.... HKL.... MKL.... DGET... "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 7);
		sortPlaces(moves, numMoves);
		assert(moves[0] == MARSEILLES);
		assert(moves[1] == HIDE);
		assert(moves[2] == DOUBLE_BACK_1);
		assert(moves[3] == DOUBLE_BACK_2);
		assert(moves[4] == DOUBLE_BACK_3);
		assert(moves[5] == DOUBLE_BACK_4);
		assert(moves[6] == DOUBLE_BACK_5);
		free(moves);
		printf("Test passed\n");
		DvFree(dv);
	}

	{//Testing DoubleBacks
		printf("Testing DoubleBack1\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DMR.V.. "
			"GKL.... SKL.... HKL.... MKL.... DCFT... "
			"GKL.... SKL.... HKL.... MKL.... DPAT... "
			"GKL.... SKL.... HKL.... MKL.... DSTT... "
			"GKL.... SKL.... HKL.... MKL.... DZUT... "
			"GKL.... SKL.... HKL.... MKL.... DGET... "
			"GKL.... SKL.... HKL.... MKL.... DD1T.V. "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 3);
		sortPlaces(moves, numMoves);
		assert(moves[0] == CLERMONT_FERRAND);
		assert(moves[1] == MARSEILLES);
		assert(moves[2] == HIDE);
		free(moves);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanTheyGo(dv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 3);
		sortPlaces(locs, numLocs);
		assert(locs[0] == CLERMONT_FERRAND);
		assert(locs[1] == GENEVA);
		assert(locs[2] == MARSEILLES);
		free(locs);

		printf("Test passed\n");
		DvFree(dv);
	}
	
	{//Testing DoubleBacks
		printf("Testing DoubleBack2\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DMR.V.. "
			"GKL.... SKL.... HKL.... MKL.... DCFT... "
			"GKL.... SKL.... HKL.... MKL.... DPAT... "
			"GKL.... SKL.... HKL.... MKL.... DSTT... "
			"GKL.... SKL.... HKL.... MKL.... DZUT... "
			"GKL.... SKL.... HKL.... MKL.... DGET... "
			"GKL.... SKL.... HKL.... MKL.... DD2T.V. "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == ZURICH);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 4);
		sortPlaces(moves, numMoves);
		assert(moves[0] == MARSEILLES);
		assert(moves[1] == MILAN);
		assert(moves[2] == MUNICH);
		assert(moves[3] == HIDE);
		free(moves);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanTheyGo(dv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == MARSEILLES);
		assert(locs[1] == MILAN);
		assert(locs[2] == MUNICH);
		assert(locs[3] == ZURICH);
		free(locs);

		printf("Test passed\n");
		DvFree(dv);
	}
	
	{//Testing DoubleBacks
		printf("Testing DoubleBack3\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DMR.V.. "
			"GKL.... SKL.... HKL.... MKL.... DCFT... "
			"GKL.... SKL.... HKL.... MKL.... DPAT... "
			"GKL.... SKL.... HKL.... MKL.... DSTT... "
			"GKL.... SKL.... HKL.... MKL.... DZUT... "
			"GKL.... SKL.... HKL.... MKL.... DGET... "
			"GKL.... SKL.... HKL.... MKL.... DD3T.V. "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == STRASBOURG);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 6);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BRUSSELS);
		assert(moves[1] == COLOGNE);
		assert(moves[2] == FRANKFURT);
		assert(moves[3] == MUNICH);
		assert(moves[4] == NUREMBURG);
		assert(moves[5] == HIDE);
		free(moves);

		int numLocs = -1;
		PlaceId *locs = DvWhereCanTheyGo(dv, PLAYER_DRACULA, &numLocs);
		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == COLOGNE);
		assert(locs[2] == FRANKFURT);
		assert(locs[3] == MUNICH);
		assert(locs[4] == NUREMBURG);
		assert(locs[5] == STRASBOURG);
		free(locs);

		printf("Test passed\n");
		DvFree(dv);
	}
	
	{//Testing DoubleBacks
		printf("Testing DoubleBack4\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DMR.V.. "
			"GKL.... SKL.... HKL.... MKL.... DCFT... "
			"GKL.... SKL.... HKL.... MKL.... DPAT... "
			"GKL.... SKL.... HKL.... MKL.... DSTT... "
			"GKL.... SKL.... HKL.... MKL.... DZUT... "
			"GKL.... SKL.... HKL.... MKL.... DGET... "
			"GKL.... SKL.... HKL.... MKL.... DD4T.V. "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == PARIS);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 5);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BRUSSELS);
		assert(moves[1] == CLERMONT_FERRAND);
		assert(moves[2] == LE_HAVRE);
		assert(moves[3] == NANTES);
		assert(moves[4] == HIDE);
		free(moves);

		printf("Test passed\n");
		DvFree(dv);
	}
	
	{//Testing DoubleBacks
		printf("Testing DoubleBack5\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DMR.V.. "
			"GKL.... SKL.... HKL.... MKL.... DCFT... "
			"GKL.... SKL.... HKL.... MKL.... DPAT... "
			"GKL.... SKL.... HKL.... MKL.... DSTT... "
			"GKL.... SKL.... HKL.... MKL.... DZUT... "
			"GKL.... SKL.... HKL.... MKL.... DGET... "
			"GKL.... SKL.... HKL.... MKL.... DD5T.V. "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == CLERMONT_FERRAND);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);

		assert(numMoves == 6);
		sortPlaces(moves, numMoves);
		assert(moves[0] == BORDEAUX);
		assert(moves[1] == CLERMONT_FERRAND);
		assert(moves[2] == MARSEILLES);
		assert(moves[3] == NANTES);
		assert(moves[4] == TOULOUSE);
		assert(moves[5] == HIDE);
		free(moves);

		printf("Test passed\n");
		DvFree(dv);
	}
	
	{//Testing Dracula Running out of moves
		printf("Testing No Moves\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DCG.V.. "
			"GKL.... SKL.... HKL.... MKL.... DTS.... "
			"GKL.... SKL.... HKL.... MKL.... DMS.... "
			"GKL.... SKL.... HKL.... MKL.... DD3T... "
			"GKL.... SKL.... HKL.... MKL.... DHIT... "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == CAGLIARI);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);

		assert(numMoves == 0);
		sortPlaces(moves, numMoves);
		assert(moves == NULL);
		free(moves);

		printf("Test passed\n");
		DvFree(dv);
	}

	{//Testing Dracula Running out of moves
		printf("Testing No Moves\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DNS.... "
			"GKL.... SKL.... HKL.... MKL.... DEDT... "
			"GKL.... SKL.... HKL.... MKL.... DMNT... "
			"GKL.... SKL.... HKL.... MKL.... DD2T... "
			"GKL.... SKL.... HKL.... MKL.... DHIT... "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == EDINBURGH);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);

		assert(numMoves == 0);
		sortPlaces(moves, numMoves);
		assert(moves == NULL);
		free(moves);

		printf("Test passed\n");
		DvFree(dv);
	}

	{//Testing Dracula Running out of moves
		printf("Testing No Moves2\n");
		char *trail = 
			"GKL.... SKL.... HKL.... MKL.... DIR.... "
			"GKL.... SKL.... HKL.... MKL.... DDUT... "
			"GKL.... SKL.... HKL.... MKL.... DGWT... "
			"GKL.... SKL.... HKL.... MKL.... DD2T... "
			"GKL.... SKL.... HKL.... MKL.... DHIT... "
			"GKL.... SKL.... HKL.... MKL.... ";
		Message messages[9] = {};

		DraculaView dv = DvNew(trail, messages);

		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == DUBLIN);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);

		assert(numMoves == 0);
		sortPlaces(moves, numMoves);
		assert(moves == NULL);
		free(moves);

		printf("Test passed\n");
		DvFree(dv);
	}

	return EXIT_SUCCESS;
}