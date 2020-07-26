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

		Message m[] = {};

		HvFree(HvNew(
			"", m
		));
		HvFree(HvNew(
			"GST....", m
		));
		HvFree(HvNew(
			"GST.... SAO.... HZU....", m
		));
		HvFree(HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		));

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetRound: ");

		HunterView hv;
		Message m[] = {};

		hv = HvNew(
			"", m
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST....", m
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU....", m
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB....", m
		);
		assert(HvGetRound(hv) == 0);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
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
			"GGE.... SGE.... HGE.... MGE....", m
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
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.", m
		);
		assert(HvGetRound(hv) == 7);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetPlayer: ");

		HunterView hv;
		Message m[] = {};

		hv = HvNew(
			"", m
		);
		assert(HvGetPlayer(hv) == PLAYER_LORD_GODALMING);
		HvFree(hv);

		hv = HvNew(
			"GST....", m
		);
		assert(HvGetRound(hv) == PLAYER_DR_SEWARD);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU....", m
		);
		assert(HvGetRound(hv) == PLAYER_MINA_HARKER);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB....", m
		);
		assert(HvGetRound(hv) == PLAYER_DRACULA);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		);
		assert(HvGetRound(hv) == PLAYER_LORD_GODALMING);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetScore: ");

		HunterView hv;
		Message m[] = {};

		hv = HvNew(
			"", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		HvFree(hv);

		hv = HvNew(
			"GST....", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB....", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - (
			4 * SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_HUNTER_HOSPITAL
		));
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.", m
		);
		assert(HvGetScore(hv) == GAME_START_SCORE - (
			 7 * SCORE_LOSS_DRACULA_TURN - SCORE_LOSS_VAMPIRE_MATURES
		));
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetHealth: ");

		HunterView hv;
		Message m[] = {};

		hv = HvNew(
			"", m
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..", m
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
			"GST.... SST.... HST.... MST.... DD1....", m
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
			"GSTTTTD", m
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
			"GGE.... SGE.... HGE.... MGE.... DD1T...", m
		);
		assert(HvGetHealth(hv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
		assert(HvGetHealth(hv, PLAYER_DRACULA) == (
			GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA)
		));
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetPlayerLocation: ");

		HunterView hv;
		Message m[] = {};

		hv = HvNew(
			"", m
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST....", m
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == NOWHERE);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == ZURICH);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == CITY_UNKNOWN);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..", m
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == CASTLE_DRACULA);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == ATLANTIC_OCEAN);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == GENEVA);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....", m
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
			"GSTTTTD", m
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(HvGetPlayerLocation(hv, PLAYER_DR_SEWARD) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_VAN_HELSING) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_MINA_HARKER) == GENEVA);
		assert(HvGetPlayerLocation(hv, PLAYER_DRACULA) == STRASBOURG);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...", m
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
			"GSZ.... SGE.... HGE....", m
		);
		assert(HvGetPlayerLocation(hv, PLAYER_LORD_GODALMING) == GENEVA);
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
		Message m[] = {};

		hv = HvNew(
			"", m
		);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		);
		assert(HvGetVampireLocation(hv) == CITY_UNKNOWN);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..", m
		);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE.... MGE.... DC?T... "
			"GSZ.... SGE.... HGE....", m
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
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.", m
		);
		assert(HvGetVampireLocation(hv) == NOWHERE);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetLastKnownDraculaLocation: ");

		HunterView hv;
		Message m[] = {};
		Round round;

		hv = HvNew(
			"", m
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HZU.... MBB.... DC?.V..", m
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE);
		HvFree(hv);

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..", m
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == GENEVA);
		assert(round == 0);
		HvFree(hv);

		hv = HvNew(
			"GGE.... SGE.... HVI.... MGE.... DCD.V.. "
			"GGE.... SGE.... HBD.... MGE.... DKLT... "
			"GGE.... SGE.... HSZ.... MGE.... DC?T... "
			"GGE.... SGE.... HKLT... MGE.... DC?T... "
			"GGE.... SGE.... HCDV... MGE.... DD1T...", m
		);
		assert(HvGetLastKnownDraculaLocation(hv, &round) == KLAUSENBURG);
		assert(round == 1);
		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvGetShortestPathTo: ");

		HunterView hv;
		Message m[] = {};
		PlaceId *path = NULL;
		int length = -1;

		hv = HvNew(
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..", m
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
		free(path);

		HvFree(hv);

		printf("passed\n");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanIGo: ");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanIGoByType: ");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanTheyGo: ");
	}

	{///////////////////////////////////////////////////////////////////
		printf("HvWhereCanTheyGoByType: ");

		HunterView hv;
		Message m[] = {};
		PlaceId *locs;
		int num = -1;

		///////////////////////////////////////////////////////////////////

		hv = HvNew(
			"GGA....", m
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

		///////////////////////////////////////////////////////////////////

		hv = HvNew(
			"GPA....", m
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

		///////////////////////////////////////////////////////////////////

		hv = HvNew(
			"GSZ.... SGE.... HGE.... MGE.... DSZ.V..", m
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

		///////////////////////////////////////////////////////////////////

		printf("passed\n");
	}
	
	return EXIT_SUCCESS;
}
