////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"

struct hunterView {
	GameView gv;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays, messages);

	return new;
}

void HvFree(HunterView hv)
{
	free(hv->gv);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	return GvGetRound(hv->gv);
}

Player HvGetPlayer(HunterView hv)
{
	return GvGetPlayer(hv->gv);
}

int HvGetScore(HunterView hv)
{
	return GvGetScore(hv->gv);
}

int HvGetHealth(HunterView hv, Player player)
{
	return GvGetHealth(hv->gv, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	return GvGetPlayerLocation(hv->gv, player);
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	PlaceId result = NOWHERE;

	int n = 0;
	bool canFree = false;
	PlaceId *history = GvGetLocationHistory(hv->gv, PLAYER_DRACULA, &n, &canFree);

	// Iterate backwards until a location is found
	for (int i = n - 1; i >= 0; i++) {
		if (placeIsReal(history[i])) {
			*round = i;
			result = history[i];
			break;
		}
	}

	if (canFree) free(history);

	return result;
}


PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	PlaceId *path = NULL;
	*pathLength = 0;
	
	// swap so that path is in correct order when backtracking
	PlaceId from = dest;
	PlaceId to = GvGetPlayerLocation(hv->gv, hunter);

	Round currentRound = GvGetRound(hv->gv);

	// synchronised queues, needed due to the lack of type
	// parameterisation in C
	Queue seen = newQueue();
	Queue rounds = newQueue();
	QueueJoin(seen, from);
	QueueJoin(rounds, currentRound);

	PlaceId links[NUM_REAL_PLACES];
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		links[i] = NOWHERE;
	}
	links[from] = from;

	while (!QueueIsEmpty(seen)) {
		PlaceId place = QueueLeave(seen);
		Round round = QueueLeave(rounds);

		if (place == to) {
			// found!

			PlaceId *path = malloc(NUM_REAL_PLACES * sizeof(PlaceId));

			int i = 0;
			for (PlaceId place = to; place != from; place = links[place]) {
				path[i] = place;
				i++;
			}
			*pathLength = i;

			break;
		} else {
			int n = 0;
			PlaceId *reachable = GvGetReachable(hv->gv, hunter, round, place, &n);

			for (int i = 0; i < n; i++) {
				PlaceId connected = reachable[i];

				if (links[connected] == NOWHERE) {
					links[connected] = place;
					QueueJoin(seen, connected);
					QueueJoin(rounds, ++round);
				}
			}
		}
	}

	dropQueue(seen);
	dropQueue(rounds);

	return path;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	return HvWhereCanTheyGoByType(
		hv,
		HvGetPlayer(hv),
		true, true, true,
		numReturnedLocs
	);
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	return HvWhereCanTheyGoByType(
		hv,
		HvGetPlayer(hv),
		road, rail, boat,
		numReturnedLocs
	);
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	return HvWhereCanTheyGoByType(
		hv,
		player,
		true, true, true,
		numReturnedLocs
	);
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	if (player == PLAYER_DRACULA) {
		Round round;
		if (HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE) {
			*numReturnedLocs = 0;
			return NULL;
		}
	}

	PlaceId *result = NULL;

	int n = 0;
	bool canFree = false;
	PlaceId *history = GvGetMoveHistory(hv->gv, player, &n, &canFree);

	if (n == 0) {
		*numReturnedLocs = 0;
	} else {
		result = GvGetReachableByType(
			hv->gv,
			player,
			HvGetRound(hv),
			HvGetPlayerLocation(hv, player),
			road, rail, boat,
			numReturnedLocs
		);
	}

	if (canFree) free(history);

	return result;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
