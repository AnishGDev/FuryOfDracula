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

HunterView HvNew(char *pastPlays, Message messages[]) {
	HunterView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays, messages);

	return new;
}

void HvFree(HunterView hv) {
	free(hv->gv);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv) {
	return GvGetRound(hv->gv);
}

Player HvGetPlayer(HunterView hv) {
	return GvGetPlayer(hv->gv);
}

int HvGetScore(HunterView hv) {
	return GvGetScore(hv->gv);
}

int HvGetHealth(HunterView hv, Player player) {
	return GvGetHealth(hv->gv, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player) {
	return GvGetPlayerLocation(hv->gv, player);
}

PlaceId HvGetVampireLocation(HunterView hv) {
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round) {
	// PlaceId result = GvGetLastKnownDraculaLocation(hv->gv, round);
	// if (result == CITY_UNKNOWN) {
	// 	return NOWHERE; 
	// }
	// return result;

	PlaceId result = NOWHERE;
	
	int n = 0;
	bool canFree = false;
	PlaceId *history = GvGetLocationHistory(
		hv->gv, PLAYER_DRACULA, &n, &canFree
	);

	// Iterate backwards until a location is found
	for (int i = n - 1; i >= 0; i--) {
		if (placeIsReal(history[i])) {
			*round = i;
			result = history[i];
			break;
		}
	}

	if (canFree) free(history);
	
	return result; 
}

// Written by Luke Fisk-Lennon for COMP2521 Lab 7
void reverse(int *array, int n) {
	for (int i = 0; i < n / 2; i++) {
		int temp = array[i];
		array[i] = array[n - i - 1];
		array[n - i - 1] = temp;
	}
}

// BFS algorithm to find shortest path
PlaceId *HvGetShortestPathTo(
	HunterView hv, Player hunter, PlaceId dest, int *pathLength
) {
	PlaceId *path = NULL;
	*pathLength = 0;
	
	PlaceId src = GvGetPlayerLocation(hv->gv, hunter);

	if (src != NOWHERE) {
		Round currentRound = GvGetRound(hv->gv);

		// Synchronised queues, needed due to the lack of type
		// parameterisation in C
		Queue seen = newQueue();
		Queue rounds = newQueue();
		QueueJoin(seen, src);
		QueueJoin(rounds, currentRound);

		// Array storing links between nodes in shortest path
		PlaceId links[NUM_REAL_PLACES];
		for (int i = 0; i < NUM_REAL_PLACES; i++) {
			links[i] = NOWHERE;
		}
		links[src] = src;

		while (!QueueIsEmpty(seen)) {
			PlaceId place = QueueLeave(seen);

			if (place == dest) {
				// Found!

				path = malloc(NUM_REAL_PLACES * sizeof(enum placeId));

				// Reconstruct the path in dest to src order
				int i = 0;
				for (PlaceId place = dest; place != src; place = links[place]) {
					path[i] = place;
					i++;
				}

				// Reverse the path to become src to dest order
				reverse(path, i);

				*pathLength = i;

				break;
			} else {
				// Not found yet

				Round round = QueueLeave(rounds);

				int n = 0;
				PlaceId *reachable = GvGetReachable(
					hv->gv, hunter, round, place, &n
				);

				// Incremented round which will be added to queue
				round++;

				// Add all reachable from current node to queue
				for (int i = 0; i < n; i++) {
					PlaceId connected = reachable[i];

					if (links[connected] == NOWHERE) {
						links[connected] = place;
						QueueJoin(seen, connected);
						QueueJoin(rounds, round);
					}
				}
			}
		}

		dropQueue(seen);
		dropQueue(rounds);
	}

	return path;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs) {
	return HvWhereCanTheyGoByType(
		hv, HvGetPlayer(hv),
		true, true, true,
		numReturnedLocs
	);
}

PlaceId *HvWhereCanIGoByType(
	HunterView hv, bool road, bool rail, bool boat, int *numReturnedLocs
) {
	return HvWhereCanTheyGoByType(
		hv, HvGetPlayer(hv),
		road, rail, boat,
		numReturnedLocs
	);
}

PlaceId *HvWhereCanTheyGo(
	HunterView hv, Player player, int *numReturnedLocs
) {
	return HvWhereCanTheyGoByType(
		hv, player,
		true, true, true,
		numReturnedLocs
	);
}

PlaceId *HvWhereCanTheyGoByType(
	HunterView hv, Player player,
	bool road, bool rail, bool boat,
	int *numReturnedLocs
) {
	*numReturnedLocs = 0;

	PlaceId location = GvGetPlayerLocation(hv->gv, player);
	if (location == NOWHERE) {
		return NULL;
	}

	// If the requested player id Dracula, but he hasn't been revealed
	// don't return any info
	if (player == PLAYER_DRACULA) {
		Round round;
		if (HvGetLastKnownDraculaLocation(hv, &round) == NOWHERE) {
			return NULL;
		}
	}

	PlaceId *result = NULL;

	int n = 0;
	bool canFree = false;
	PlaceId *history = GvGetMoveHistory(hv->gv, player, &n, &canFree);
	Round round = GvGetRound(hv->gv);

	// If player already had their turn this round, give info
	// for next round
	if (GvGetPlayer(hv->gv) > player) {
		round = (round + 1) % NUM_PLAYERS;
	}

	if (n != 0) {
		result = GvGetReachableByType(
			hv->gv, player, round,
			location,
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
