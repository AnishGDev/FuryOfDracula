////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testHunterView.c: test the HunterView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
		printf("HvNew: ");

		HvFree(HvNew(
			"",
			(Message[0]) {}
		));

		HvFree(HvNew(
			"GST....",
			(Message[1]) {}
		));

		HvFree(HvNew(
			"GST.... SAO.... HZU....",
			(Message[3]) {}
		));

		HvFree(HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		));

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetRound: ");

		HunterView hv;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST....",
			(Message[1]) {}
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU....",
			(Message[3]) {}
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB....",
			(Message[4]) {}
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetRound(hv) == 1);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE....",
			(Message[34]) {}
		);
		assert(HvGetRound(hv) == 6);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.",
			(Message[35]) {}
		);
		assert(HvGetRound(hv) == 7);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetPlayer: ");

		HunterView hv;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		HvFree(hv);

		hv = HvNew(
			"GST....",
			(Message[1]) {}
		);
		assert(HvGetPlayer(hv) == PLAYER_DR_SEWARD);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU....",
			(Message[3]) {}
		);
		assert(HvGetPlayer(hv) == PLAYER_MINA_HARKER);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB....",
			(Message[4]) {}
		);
		assert(HvGetPlayer(hv) == PLAYER_DRACULA);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetScore: ");

		HunterView hv;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		HvFree(hv);

		hv = HvNew(
			"GST....",
			(Message[1]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB....",
			(Message[4]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....",
			(Message[10]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD", 
			(Message[21]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - (
			4 * SCORE_LOSS_DRACULA_TURN + SCORE_LOSS_HUNTER_HOSPITAL
		));
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.",
			(Message[35]) {}
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - (
			 7 * SCORE_LOSS_DRACULA_TURN + SCORE_LOSS_VAMPIRE_MATURES
		));
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetHealth: ");

		HunterView hv;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..",
			(Message[6]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == (
			GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER
		));
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == (
			GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER
		));
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....",
			(Message[10]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == (
			GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA)
		));
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD",
			(Message[21]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == 0);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == (
			GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER
		));
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...",
			(Message[10]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == (
			GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA)
		));
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGET...",
			(Message[6]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == (
			GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER
		));
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGETT.. SGE.... HGE.... MGE.... DC?.... "
			"GGE....",
			(Message[16]) {}
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == (
			GAME_START_HUNTER_LIFE_POINTS - (LIFE_LOSS_TRAP_ENCOUNTER * 2) + LIFE_GAIN_REST
		));
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetPlayerLocation: ");

		HunterView hv;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST....",
			(Message[1]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..",
			(Message[6]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == CASTLE_DRACULA);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == GENEVA);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....",
			(Message[10]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == SEA_UNKNOWN);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD",
			(Message[21]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == STRASBOURG);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...",
			(Message[10]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CASTLE_DRACULA);
		HvFree(hv);

		hv = HvNew(
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE....",
			(Message[18]) {}
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == SZEGED);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetVampireLocation: ");

		HunterView hv;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetVampireLocation(hv) == CITY_UNKNOWN);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..",
			(Message[6]) {}
		);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE....",
			(Message[18]) {}
		);
		assert(HvGetVampireLocation(hv) == CASTLE_DRACULA);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.",
			(Message[35]) {}
		);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetLastKnownDraculaLocation: ");

		HunterView hv;
		Round round;

		hv = HvNew(
			"",
			(Message[0]) {}
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..",
			(Message[6]) {}
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == GENEVA);
		assert(round == 0);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HVI.... MGE.... DCD.V.. "
			"GGE.... SGE.... HBD.... MGE.... DKLT... "
			"GGE.... SGE.... HSZ.... MGE.... DC?T... "
			"GGE.... SGE.... HKLT... MGE.... DC?T... "
			"GGE.... SGE.... HCDV... MGE.... DD1T...",
			(Message[25]) {}
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == KLAUSENBURG);
		assert(round == 1);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetShortestPathTo: ");

		HunterView hv;
		PlaceId *path = NULL;
		int length = -1;

		hv = HvNew(
			"GLS.... SLS.... HSW.... MMR.... DCD.V..",
			(Message[5]) {}
		);

		path = HvGetShortestPathTo(
			hv, PLAYER_LORD_GODALMING, BARCELONA, &length
		);
		assert(length == 2);
		assert(path[0] == MADRID);
		assert(path[1] == BARCELONA);
		free(path);

		path = HvGetShortestPathTo(
			hv, PLAYER_LORD_GODALMING, COLOGNE, &length
		);
		assert(length == 3);
		assert(path[0] == MADRID);
		assert(path[1] == BORDEAUX);
		assert(path[2] == COLOGNE);
		free(path);

		path = HvGetShortestPathTo(
			hv, PLAYER_VAN_HELSING, HAMBURG, &length
		);
		assert(length == 3);
		assert(path[0] == EDINBURGH);
		assert(path[1] == NORTH_SEA);
		assert(path[2] == HAMBURG);
		free(path);

		path = HvGetShortestPathTo(
			hv, PLAYER_MINA_HARKER, CONSTANTA, &length
		);
		assert(length == 4);
		assert(path[0] == GENOA || path[0] == MILAN);
		assert(path[1] == VENICE);
		assert(path[2] == BUDAPEST);
		assert(path[3] == CONSTANTA);
		free(path);

		path = HvGetShortestPathTo(
			hv, PLAYER_DR_SEWARD, CASTLE_DRACULA, &length
		);
		assert(length == 7);
		assert(path[0] == SARAGOSSA);
		assert(path[1] == MARSEILLES);
		assert(path[2] == GENOA || path[2] == MILAN);
		assert(path[3] == VENICE);
		assert(path[4] == BUDAPEST);
		assert(path[5] == GALATZ || path[5] == KLAUSENBURG);
		assert(path[6] == CASTLE_DRACULA);
		free(path);

		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanIGo: ");

		HunterView hv;
		PlaceId *locs;
		int num = -1;

		hv = HvNew(
			"GVE.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[5]) {}
		);

		locs = HvWhereCanIGo(hv, &num);
		assert(num == 7);
		sortPlaces(locs, num);
		assert(locs[0] == ADRIATIC_SEA);
		assert(locs[1] == FLORENCE);
		assert(locs[2] == GENOA);
		assert(locs[3] == MILAN);
		assert(locs[4] == MUNICH);
		assert(locs[5] == VENICE);
		assert(locs[6] == VIENNA);
		free(locs);

		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanIGoByType: ");

		HunterView hv;
		PlaceId *locs;
		int num = -1;

		hv = HvNew(
			"GVE.... SAO.... HZU.... MBB.... DC?.V.. "
			"GVE.... SAO.... HZU.... MBB.... DC?.V.. "
			"GVE.... SAO.... HZU.... MBB.... DC?.V.. "
			"GVE.... SAO.... HZU.... MBB.... DC?.V..",
			(Message[20]) {}
		);

		printf("\nHvGetPlayerLocation(hv, HvGetPlayer(hv))): %d\n", HvGetPlayerLocation(hv, HvGetPlayer(hv)));
		locs = HvWhereCanIGoByType(hv, true, false, false, &num);
		assert(num == 5);
		sortPlaces(locs, num);
		assert(locs[0] == FLORENCE);
		assert(locs[1] == GENOA);
		assert(locs[2] == MILAN);
		assert(locs[3] == MUNICH);
		assert(locs[4] == VENICE);
		free(locs);

		// Currently no rail travel since round is 4 and 0th player.
		locs = HvWhereCanIGoByType(hv, false, true, false, &num);
		assert(num == 1);
		sortPlaces(locs, num);
		assert(locs[0] == VENICE);
		free(locs);

		locs = HvWhereCanIGoByType(hv, false, false, true, &num);
		assert(num == 2); // num will be two since its adjacent to sea and it returns itself
		sortPlaces(locs, num);
		assert(locs[0] == ADRIATIC_SEA);
		free(locs);

		locs = HvWhereCanIGoByType(hv, true, true, true, &num);
		assert(num == 6);
		sortPlaces(locs, num);
		assert(locs[0] == ADRIATIC_SEA);
		assert(locs[1] == FLORENCE); 
		assert(locs[2] == GENOA);
		assert(locs[3] == MILAN);
		assert(locs[4] == MUNICH);
		assert(locs[5] == VENICE);
		//assert(locs[5] == VIENNA);
		free(locs);

		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanTheyGo: ");

		HunterView hv;
		PlaceId *locs;
		int num = -1;

		hv = HvNew(
			"GLE....",
			(Message[1]) {}
		);

		locs = HvWhereCanTheyGo(hv, PLAYER_LORD_GODALMING, &num);
		assert(num == 5);
		sortPlaces(locs, num);
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);
		assert(locs[4] == PARIS);
		free(locs);

		HvFree(hv);

		hv = HvNew(
			"GLE.... SLE....",
			(Message[2]) {}
		);

		locs = HvWhereCanTheyGo(hv, PLAYER_LORD_GODALMING, &num);
		assert(num == 5);
		sortPlaces(locs, num);
		// Since there is no rail travel this round should remove 
		// Bordeaux and Marseilles
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);
		assert(locs[4] == PARIS);
		free(locs);

		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanTheyGoByType: ");

		HunterView hv;
		PlaceId *locs;
		int num = -1;

		hv = HvNew(
			"GGA....",
			(Message[1]) {}
		);

		locs = HvWhereCanTheyGoByType(
			hv, PLAYER_LORD_GODALMING, true, false, false, &num
		);
		assert(num == 5);
		sortPlaces(locs, num);
		assert(locs[0] == BUCHAREST);
		assert(locs[1] == CASTLE_DRACULA);
		assert(locs[2] == CONSTANTA);
		assert(locs[3] == GALATZ);
		assert(locs[4] == KLAUSENBURG);
		free(locs);

		HvFree(hv);

		hv = HvNew(
			"GPA....",
			(Message[1]) {}
		);

		locs = HvWhereCanTheyGoByType(
			hv, PLAYER_LORD_GODALMING, false, true, false, &num
		);
		assert(num == 5);
		sortPlaces(locs, num);
		assert(locs[0] == BORDEAUX);
		assert(locs[1] == BRUSSELS);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == MARSEILLES);
		assert(locs[4] == PARIS);
		free(locs);

		HvFree(hv);

		hv = HvNew(
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V..",
			(Message[5]) {}
		);

		locs = HvWhereCanTheyGoByType(
			hv, PLAYER_DRACULA, true, false, false, &num
		);
		assert(num == 5);
		sortPlaces(locs, num);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == BUDAPEST);
		assert(locs[2] == KLAUSENBURG);
		assert(locs[3] == SZEGED);
		assert(locs[4] == ZAGREB);
		free(locs);

		HvFree(hv);

		hv = HvNew(
			"GLE....",
			(Message[1]) {}
		);

		locs = HvWhereCanTheyGoByType(
			hv, PLAYER_LORD_GODALMING, true, true, true, &num
		);
		assert(num == 5);
		sortPlaces(locs, num);
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);
		assert(locs[4] == PARIS);
		free(locs);

		HvFree(hv);

		hv = HvNew(
			"GLE.... SLE....",
			(Message[2]) {}
		);

			locs = HvWhereCanTheyGo(hv, PLAYER_LORD_GODALMING, &num);
		assert(num == 5);
		sortPlaces(locs, num);
		// Since there is no rail travel this round should remove 
		// Bordeaux and Marseilles
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);
		assert(locs[4] == PARIS);
		free(locs);

		HvFree(hv);

		printf("passed\n");
	}
	
	return EXIT_SUCCESS;
}
