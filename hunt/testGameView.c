////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testGameView.c: test the GameView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Basic initialisation\n");

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
		printf("\t====== CUSTOM TESTS =====\n");
		printf("\t====== Testing GvGetReachable =====\n");
		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1; 
		PlaceId *locs = GvGetReachable(gv, PLAYER_DR_SEWARD, 0, MADRID, &numLocs);
		printf("Num locks is %d\n", numLocs);
		assert(numLocs == 7);
		sortPlaces(locs, numLocs);
		assert(locs[0] == ALICANTE); 
		free(locs);
		locs = GvGetReachable(gv, PLAYER_DRACULA, 0, ZURICH, &numLocs);
		assert(numLocs == 6);
		sortPlaces(locs, numLocs);
		assert(locs[0] == GENEVA);
		free(locs);
		GvFree(gv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing connections\n");
		
		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		{
			printf("\tChecking Galatz road connections "
				   "(Lord Godalming, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 1, GALATZ, true, false,
												 false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections "
				   "(Lord Godalming, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 1, IONIAN_SEA, false, false,
												 true, &numLocs);
			
			printf("Num locs is %d\n", numLocs);
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Paris rail connections "
				   "(Lord Godalming, Round 2)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 2, PARIS, false, true,
												 false, &numLocs);
			
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
		}
		
		{
			printf("\tChecking Athens rail connections (none)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 1, ATHENS, false, true,
												 false, &numLocs);
			
			assert(numLocs == 1);
			assert(locs[0] == ATHENS);
			free(locs);
		}
		///////////////////////////////////////////////////////////////////

		GvFree(gv);
		printf("Test passed!\n");
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("After Lord Godalming's turn\n");

		char *trail =
			"GST....";
		
		Message messages[1] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DR_SEWARD);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == NOWHERE);

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Mina Harker's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[4] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == NOWHERE);

		GvFree(gv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("After Dracula's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB.... DC?.V..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 1);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == CITY_UNKNOWN);

		GvFree(gv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha",
			"Aha!"
		};
		
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GENEVA);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		GvFree(gv);
		printf("Test passed\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
			   "and losing blood points (Hunter View)\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....";
		
		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == SEA_UNKNOWN);

		GvFree(gv);
		printf("Test passed!\n");
	}


	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
			   "and losing blood points (Dracula View)\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DEC.... "
			"GST.... SST.... HST.... MST.... DD1.... "
			"GFR.... SFR.... HFR.... MFR....";
		
		Message messages[14] = {
			"Hello", "Rubbish", "Stuff", "", "Mwahahah",
			"Aha!", "", "", "", "Back I go"};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == ENGLISH_CHANNEL);

		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking that hunters' health points are capped\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE....";
	
		Message messages[6] = {};
		GameView gv = GvNew(trail, messages);
	
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing a hunter 'dying'\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD";
		
		Message messages[21] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetScore(gv) == GAME_START_SCORE
								 - 4 * SCORE_LOSS_DRACULA_TURN
								 - SCORE_LOSS_HUNTER_HOSPITAL);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == 0);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == STRASBOURG);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing Dracula doubling back to Castle Dracula\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...";
		
		Message messages[10] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CASTLE_DRACULA);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing vampire/trap locations\n");
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GSZ.... SGE.... HGE.... MGE....";
		
		Message messages[19] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GALATZ);
		assert(GvGetVampireLocation(gv) == CASTLE_DRACULA);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == GALATZ && traps[1] == KLAUSENBURG);
		free(traps);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing trap locations after one is destroyed\n");
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DBC.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GBE.... SGE.... HGE.... MGE.... DCNT... "
			"GKLT... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == KLAUSENBURG);
		assert(GvGetVampireLocation(gv) == BUCHAREST);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == CONSTANTA && traps[1] == GALATZ);
		free(traps);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing a vampire maturing\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.";
		
		Message messages[35] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetScore(gv) == GAME_START_SCORE
								 - 7 * SCORE_LOSS_DRACULA_TURN
								 - SCORE_LOSS_VAMPIRE_MATURES);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		
		GvFree(gv);
		printf("Test passed!\n");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing move/location history\n");
		
		char *trail =
			"GLS.... SGE.... HGE.... MGE.... DST.V.. "
			"GCA.... SGE.... HGE.... MGE.... DC?T... "
			"GGR.... SGE.... HGE.... MGE.... DC?T... "
			"GAL.... SGE.... HGE.... MGE.... DD3T... "
			"GSR.... SGE.... HGE.... MGE.... DHIT... "
			"GSN.... SGE.... HGE.... MGE.... DC?T... "
			"GMA.... SSTTTV.";
		
		Message messages[32] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_DR_SEWARD) ==
				GAME_START_HUNTER_LIFE_POINTS - 2 * LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		
		// Lord Godalming's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,
											  &numMoves, &canFree);
			assert(numMoves == 7);
			assert(moves[0] == LISBON);
			assert(moves[1] == CADIZ);
			assert(moves[2] == GRANADA);
			assert(moves[3] == ALICANTE);
			assert(moves[4] == SARAGOSSA);
			assert(moves[5] == SANTANDER);
			assert(moves[6] == MADRID);
			if (canFree) free(moves);
		}
		
		// Dracula's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_DRACULA,
											  &numMoves, &canFree);
			assert(numMoves == 6);
			assert(moves[0] == STRASBOURG);
			assert(moves[1] == CITY_UNKNOWN);
			assert(moves[2] == CITY_UNKNOWN);
			assert(moves[3] == DOUBLE_BACK_3);
			assert(moves[4] == HIDE);
			assert(moves[5] == CITY_UNKNOWN);
			if (canFree) free(moves);
		}
		
		{
			int numLocs = 0; bool canFree = false;
			PlaceId *locs = GvGetLocationHistory(gv, PLAYER_DRACULA,
												 &numLocs, &canFree);
			assert(numLocs == 6);
			assert(locs[0] == STRASBOURG);
			assert(locs[1] == CITY_UNKNOWN);
			assert(locs[2] == CITY_UNKNOWN);
			assert(locs[3] == STRASBOURG);
			assert(locs[4] == STRASBOURG);
			assert(locs[5] == CITY_UNKNOWN);
			if (canFree) free(locs);
		}
		
		GvFree(gv);
		printf("Test passed!\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing connections\n");
		
		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		{
			printf("\tChecking Galatz road connections "
				   "(Lord Godalming, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 1, GALATZ, true, false,
												 false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections "
				   "(Lord Godalming, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 1, IONIAN_SEA, false, false,
												 true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Paris rail connections "
				   "(Lord Godalming, Round 2)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 2, PARIS, false, true,
												 false, &numLocs);
			
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
		}
		
		{
			printf("\tChecking Athens rail connections (none)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
												 1, ATHENS, false, true,
												 false, &numLocs);
			
			assert(numLocs == 1);
			assert(locs[0] == ATHENS);
			free(locs);
		}
		///////////////////////////////////////////////////////////////////

		GvFree(gv);
		printf("Test passed!\n");
	}

	{
		printf("CUSTOM TEST: Testing deletion of trap in middle and then trapLocs return");
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DBC.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GBE.... SGE.... HGE.... MGE.... DCNT... "
			"GGAT... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == GALATZ);
		assert(GvGetVampireLocation(gv) == BUCHAREST);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == CONSTANTA && traps[1] == KLAUSENBURG);
		free(traps);
		
		GvFree(gv);
		printf("Test passed!\n");
	}

	{
	printf("Testing hunter's getLastLocation and getLastMoves\n");
		char *trail =
			"GLS.... SGE.... HGE.... MGE.... DST.V.. "
			"GCA.... SGE.... HGE.... MGE.... DC?T... "
			"GGR.... SGE.... HGE.... MGE.... DC?T... "
			"GAL.... SGE.... HGE.... MGE.... DD3T... "
			"GSR.... SGE.... HGE.... MGE.... DHIT... "
			"GSN.... SGE.... HGE.... MGE.... DC?T... "
			"GMA.... SSTTTV.";
		Message messages[32] = {};
		GameView gv = GvNew(trail, messages);
		int numReturned = -1; 
		bool canFree; 
		printf("\t Checking PLAYER_LORD_GODALMING\n");
		PlaceId *locHistory = GvGetLastLocations(gv, PLAYER_LORD_GODALMING, 3, &numReturned, &canFree);
		PlaceId *movHistory = GvGetLastMoves(gv, PLAYER_LORD_GODALMING, 3, &numReturned, &canFree);
		assert(numReturned == 3); 
		printf("\t\t Checking Location History\n");
		assert(locHistory[0] == SARAGOSSA);
		assert(locHistory[1] == SANTANDER);
		assert(locHistory[2] == MADRID);
		printf("\t\t Checking Move History\n");
		assert(movHistory[0] == SARAGOSSA);
		assert(movHistory[1] == SANTANDER);
		assert(movHistory[2] == MADRID);
		printf("\t Checking PLAYER_DR_SEWARD\n");
		locHistory = GvGetLastLocations(gv, PLAYER_DR_SEWARD, 3, &numReturned, &canFree);
		movHistory = GvGetLastMoves(gv, PLAYER_DR_SEWARD, 3, &numReturned, &canFree);
		assert(numReturned == 3); 
		printf("\t\t Checking Location History\n");
		assert(locHistory[0] == GENEVA);
		assert(locHistory[1] == GENEVA);
		assert(locHistory[2] == STRASBOURG);
		printf("\t\t Checking Move History\n");
		assert(movHistory[0] == GENEVA);
		assert(movHistory[1] == GENEVA);
		assert(movHistory[2] == STRASBOURG);
		printf("\t Checking PLAYER_VAN_HELSING\n");
		locHistory = GvGetLastLocations(gv, PLAYER_VAN_HELSING, 3, &numReturned, &canFree);
		movHistory = GvGetLastMoves(gv, PLAYER_VAN_HELSING, 3, &numReturned, &canFree);
		assert(numReturned == 3); 
		printf("\t\t Checking Location History\n");
		assert(locHistory[0] == GENEVA);
		assert(locHistory[1] == GENEVA);
		assert(locHistory[2] == GENEVA);
		printf("\t\t Checking Move History\n");
		assert(movHistory[0] == GENEVA);
		assert(movHistory[1] == GENEVA);
		assert(movHistory[2] == GENEVA);
		printf("\t Checking PLAYER_MINA_HARKER\n");
		locHistory = GvGetLastLocations(gv, PLAYER_MINA_HARKER, 3, &numReturned, &canFree);
		movHistory = GvGetLastMoves(gv, PLAYER_MINA_HARKER, 3, &numReturned, &canFree);
		assert(numReturned == 3); 
		printf("\t\t Checking Location History\n");
		assert(locHistory[0] == GENEVA);
		assert(locHistory[1] == GENEVA);
		assert(locHistory[2] == GENEVA);
		printf("\t\t Checking Move History\n");
		assert(movHistory[0] == GENEVA);
		assert(movHistory[1] == GENEVA);
		assert(movHistory[2] == GENEVA);
		if (canFree){
			free(locHistory);
		}
		GvFree(gv); 
		printf("Testing Dracula's getLastLocation amd getLastMoves\n");
				trail = "GLS.... SGE.... HGE.... MGE.... DST.V.. "
							"GCA.... SGE.... HGE.... MGE.... DZUT... "
							"GGR.... SGE.... HGE.... MGE.... DMUT... "
							"GAL.... SGE.... HGE.... MGE.... DD3T... "
							"GSR.... SGE.... HGE.... MGE.... DHIT... "
							"GSN.... SGE.... HGE.... MGE.... DMIT... "
							"GMA.... SSTTTV. HGE.... MGE....";
		// Reconstruct game view in point of Dracula. 
		gv = GvNew(trail, messages);
		locHistory = GvGetLastLocations(gv, PLAYER_DRACULA, 3, &numReturned, &canFree);
		movHistory = GvGetLastMoves(gv, PLAYER_DRACULA, 3, &numReturned, &canFree);
		assert(numReturned == 3); 
		printf("\t\t Checking Location History\n");
		assert(locHistory[0] == STRASBOURG);
		assert(locHistory[1] == STRASBOURG);
		assert(locHistory[2] == MILAN);
		printf("\t\t Checking Move History\n");
		assert(movHistory[0] == DOUBLE_BACK_3);
		assert(movHistory[1] == HIDE);
		assert(movHistory[2] == MILAN);
		if (canFree){
			free(locHistory);
		}
		GvFree(gv);
	}	
	{////////////////// Rishabh's Tests ///////////////
		printf("Testing whether correct location is recovered.\n");
		char* trail = "GLS.... SGE.... HGE.... MGE.... DST.V.. "
					"GCA.... SGE.... HGE.... MGE.... DHIT... "
					"GGR.... SGE.... HGE.... MGE.... DFRT... "
					"GAL.... SGE.... HGE.... MGE.... DLIT... "
					"GSR.... SGE.... HGE.... MGE.... DCOT... "
					"GSN.... SGE.... HGE.... MGE.... DD4T... "
					"GMA.... SSTTTV. HGE.... MGE....";
		Message messages[32] = {}; 

		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locHistory;
		bool canFree;

		printf("\tTesting Dracula's current location.\n");
		locHistory = GvGetLocationHistory(gv, PLAYER_DRACULA, &numLocs, &canFree);
		printf("\t\tChecking whether it actually returns a Location History.\n");
		assert(locHistory != NULL);
		printf("\t\tChecking for whether number of returned locations is correct.\n");
		assert(numLocs == 6);
		printf("\t\tChecking whether the locHistory is actually correct.\n");
		assert(locHistory[0] == STRASBOURG);
		assert(locHistory[1] == STRASBOURG);
		assert(locHistory[5] == STRASBOURG);

		GvFree(gv);
		printf("Test Passed!\n");

	}
	printf("CUSTOM TEST: Trap Tests!\n");
	{
		printf("\tTesting more than 6 traps.\n");
		char* trail = "GLS.... SGE.... HGE.... MGE.... DST.V.. "
					"GCA.... SGE.... HGE.... MGE.... DHIT... "
					"GGR.... SGE.... HGE.... MGE.... DFRT... "
					"GAL.... SGE.... HGE.... MGE.... DLIT... "
					"GSR.... SGE.... HGE.... MGE.... DCOT... "
					"GSN.... SGE.... HGE.... MGE.... DBUT... "
					"GMA.... SSTV... HGE.... MGE.... DPAT... "
					"GSN.... SGE.... HGE.... MGE.... DLET.M. ";
		Message messages[32] = {}; 

		GameView gv = GvNew(trail, messages);
		int numTraps = -1;
		PlaceId *trapLocs;

		trapLocs = GvGetTrapLocations(gv, &numTraps);
		printf("\t\tChecking number of traps is correct.\n");
		assert(numTraps == 6);
		printf("\t\tChecking trap locations are actually correct.\n");
		assert(trapLocs[0] == LE_HAVRE);
		assert(trapLocs[5] == FRANKFURT);

		GvFree(gv);
		free(trapLocs);
	}
	{
		printf("\tTesting Trap Malfunctioning.\n");
		char* trail = "GLS.... SGE.... HGE.... MGE.... DST.V.. "
					"GCA.... SGE.... HGE.... MGE.... DHIT... "
					"GGR.... SGE.... HGE.... MGE.... DFRT... "
					"GAL.... SGE.... HGE.... MGE.... DLIT... "
					"GSR.... SGE.... HGE.... MGE.... DCOT.M. ";
		Message messages[32] = {}; 

		GameView gv = GvNew(trail, messages);
		int numTraps = -1;
		PlaceId *trapLocs;

		trapLocs = GvGetTrapLocations(gv, &numTraps);
		printf("\t\tChecking number of traps is correct.\n");
		assert(numTraps == 3);
		printf("\t\tChecking location of traps is correct.\n");
		assert(trapLocs[0] == COLOGNE);
		assert(trapLocs[2] == FRANKFURT);
		GvFree(gv);
		free(trapLocs);

		printf("\tTesting trap malfunctioning when a hunter recently eliminated traps.\n");
		trail = "GLS.... SGE.... HGE.... MGE.... DST.V.. "
				"GCA.... SGE.... HGE.... MGE.... DHIT... "
				"GGR.... SGE.... HGE.... MGE.... DFRT... "
				"GAL.... SGE.... HGE.... MGE.... DLIT... "
				"GSR.... SSTT... HLIT... MGE.... DCOT.M. ";
		
		gv = GvNew(trail, messages);
		numTraps = -1;

		trapLocs = GvGetTrapLocations(gv, &numTraps);
		printf("\t\tChecking number of traps is correct.\n");
		assert(numTraps == 1);
		printf("\t\tChecking trap locations are correct.\n");
		assert(trapLocs[0] == COLOGNE);

		GvFree(gv);
		free(trapLocs);
	}
	printf("ALL TESTS PASSED SUCCESSFULLY!!!\n");

	{
		char *trail = "GGW.... SPL.... HCA.... MCG.... DST.V.. GDU.... SLO.... HLS.... MTS....";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);
		int numLocs = -1;
		PlaceId *locs = GvGetReachableByType(gv, PLAYER_MINA_HARKER,
												 3, ROME, true, true,
												 true, &numLocs);
		sortPlaces(locs, numLocs);
		for(int i = 0; i < numLocs; i++) {
			printf("%s\n", placeIdToName(locs[i]));
		}
	}
	return EXIT_SUCCESS;
}

