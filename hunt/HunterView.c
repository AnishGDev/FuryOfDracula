////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
#include "Queue.h"

#define ROUND_CHARS 40
#define PATH_SIZE NUM_REAL_PLACES * sizeof(enum placeId)

typedef struct _Path {
	PlaceId *array;
	int length;
} Path;

struct hunterView {
	GameView gv;
	char *pastPlays;
	bool canFreePastPlays;
	// -1 to exclude Dracula
	Path pathCache[NUM_PLAYERS - 1][NUM_REAL_PLACES];
};

static void dfsHelper(HunterView hv, Map m, PlaceId from, int maxDepth, int depth, int *visited, int *numLocs, PlaceId **hunterMoves, int *numRetMoves);

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNewGeneric(
	char *pastPlays, Message messages[], bool canFreePastPlays
) {
	HunterView hv = malloc(sizeof(*hv));
	if (hv == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}

	hv->gv = GvNew(pastPlays, messages);
	hv->pastPlays = pastPlays;
	hv->canFreePastPlays = canFreePastPlays;

	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		for (int j = 0; j < NUM_REAL_PLACES; j++) {
			hv->pathCache[i][j].array = NULL; // No path
		}
	}

	return hv;
}

HunterView HvNew(char *pastPlays, Message messages[]) {
	return HvNewGeneric(pastPlays, messages, false);
}

void HvFree(HunterView hv) {
	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		for (int j = 0; j < NUM_REAL_PLACES; j++) {
			if (hv->pathCache[i][j].array != NULL) {
				free(hv->pathCache[i][j].array);
			}
		}
	} 

	if (hv->canFreePastPlays) {
		free(hv->pastPlays);
	}

	GvFree(hv->gv);
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

// Array of place names for debugging; TODO: remove
char *e[] = {"ADRIATIC_SEA", "ALICANTE", "AMSTERDAM", "ATHENS", "ATLANTIC_OCEAN", "BARCELONA", "BARI", "BAY_OF_BISCAY", "BELGRADE", "BERLIN", "BLACK_SEA", "BORDEAUX", "BRUSSELS", "BUCHAREST", "BUDAPEST", "CADIZ", "CAGLIARI", "CASTLE_DRACULA", "CLERMONT_FERRAND", "COLOGNE", "CONSTANTA", "DUBLIN", "EDINBURGH", "ENGLISH_CHANNEL", "FLORENCE", "FRANKFURT", "GALATZ", "GALWAY", "GENEVA", "GENOA", "GRANADA", "HAMBURG", "IONIAN_SEA", "IRISH_SEA", "KLAUSENBURG", "LE_HAVRE", "LEIPZIG", "LISBON", "LIVERPOOL", "LONDON", "MADRID", "MANCHESTER", "MARSEILLES", "MEDITERRANEAN_SEA", "MILAN", "MUNICH", "NANTES", "NAPLES", "NORTH_SEA", "NUREMBURG", "PARIS", "PLYMOUTH", "PRAGUE", "ROME", "SALONICA", "SANTANDER", "SARAGOSSA", "SARAJEVO", "SOFIA", "ST_JOSEPH_AND_ST_MARY", "STRASBOURG", "SWANSEA", "SZEGED", "TOULOUSE", "TYRRHENIAN_SEA", "VALONA", "VARNA", "VENICE", "VIENNA", "ZAGREB", "ZURICH"};

// Memoised BFS algorithm to find shortest path
PlaceId *HvGetShortestPathTo(
	HunterView hv, Player hunter, PlaceId dest, int *pathLength
) {
	PlaceId src = GvGetPlayerLocation(hv->gv, hunter);
	PlaceId *path = NULL;
	*pathLength = 0;

	// printf("from %s to %s\n", e[src], e[dest]);

	if (src == dest) {
		*pathLength = 1;
		path = malloc(sizeof(enum placeId));
		path[0] = dest;

		return path;
	}

	// Check cache first
	if (placeIsReal(dest)) {
		if (hv->pathCache[hunter][dest].array != NULL) {
			path = malloc(PATH_SIZE);
			memmove(path, hv->pathCache[hunter][dest].array, PATH_SIZE);
			*pathLength = hv->pathCache[hunter][dest].length;

			return path;
		}
	}

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

				path = malloc(PATH_SIZE);

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

				free(reachable);
			}
		}

		dropQueue(seen);
		dropQueue(rounds);
	}

	// Add to cache
	if (placeIsReal(dest)) {
		hv->pathCache[hunter][dest].array = malloc(PATH_SIZE);
		memmove(hv->pathCache[hunter][dest].array, path, PATH_SIZE);
		hv->pathCache[hunter][dest].length = *pathLength;
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
		location = HvGetLastKnownDraculaLocation(hv, &round);
		if (location == NOWHERE) {
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

HunterView HvWaybackMachine(HunterView hv, Round round) {
	Round currentRound = GvGetRound(hv->gv);
	if (round > currentRound) round = currentRound;

	int length = round * ROUND_CHARS;
	char *substring = malloc(length + 1); // + 1 for \0

	memmove(substring, hv->pastPlays, length);
	substring[length] = '\0';

	return HvNewGeneric(substring, NULL, true); // TODO: is NULL ok?
}

PlaceId *locationsNNodesAway(HunterView hv, PlaceId from, int maxDepth, int *numLocs) {
	// Uses a DFS to get locations at max depth away
	// I think should be a BFS
	int numRetLocs[NUM_PLAYERS-1];
	bool canfree[NUM_PLAYERS-1];
	PlaceId *hunterPrevMoves[NUM_PLAYERS-1];
	for (int i = 0; i < NUM_PLAYERS-1; i++) {
		hunterPrevMoves[i] = GvGetLocationHistory(hv->gv, i, &numRetLocs[i], &canfree[i]);
	}
	

	int visited[NUM_REAL_PLACES] = {0};
	int depth = 1;
	visited[from] = depth;
	Map m = MapNew();
	maxDepth--;
	*numLocs = 0;
	dfsHelper(hv, m, from, maxDepth, depth, visited, numLocs, hunterPrevMoves, numRetLocs);

	// Free Hunter Moves
	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		if (canfree[i]) {
			free(hunterPrevMoves[i]);
			hunterPrevMoves[i] = NULL;
		}
	}
	
	PlaceId *locations = malloc(sizeof(PlaceId) * NUM_REAL_PLACES);
	int freeLocIndex = 0;
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		if (visited[i] == maxDepth) {
			locations[freeLocIndex] = i;
			freeLocIndex++;
		}
	}

	MapFree(m);
	return locations;
}

static void dfsHelper(HunterView hv, Map m, PlaceId from, int maxDepth, int depth, int *visited, int *numLocs, PlaceId **hunterMoves, int *numRetLoc) {
	
	ConnList toVisit = MapGetConnections(m, from);
	depth++;
	visited[from] = depth;

	while (toVisit != NULL) {
		if (visited[toVisit->p] == 0) {
			visited[toVisit->p] = depth;

			// Check if a Hunter would've been at that location
			// after dracula

			
			for (int i = 0; i < NUM_PLAYERS - 1; i++) {
				if (toVisit->p == HvGetPlayerLocation(hv, i)) {
					visited[toVisit->p] = depth-1;
				}
			}
			/*
			if (maxDepth - depth <= TRAIL_SIZE) { // If dracula has been here in the last 6 moves
				int maxHistIndex = 0;
				for (int i = 0; i < NUM_PLAYERS -1; i++) {
					maxHistIndex = (numRetLoc[i] > TRAIL_SIZE) ? TRAIL_SIZE : numRetLoc[i]; // Whichevers smaller
					for (int j = 0; j < maxHistIndex; j++) {
						if (hunterMoves[i][j] == toVisit->p) {
							visited[toVisit->p] = 0;
							return; // Do not persue this path, if dracula went here a hunter wouldve caught it
						}
					}
				}
			}*/
			
			if (depth == maxDepth) {
				*numLocs += 1;
			}
			else {
				dfsHelper(hv, m, toVisit->p, maxDepth, depth, visited, numLocs, hunterMoves, numRetLoc);
			}
		}
		toVisit = toVisit->next;
	}

}

PlaceId *HvGetReachable(
	HunterView hv, Player player, Round round, PlaceId from,
	int *numReturnedLocs
) {
	return GvGetReachable(hv->gv, player, round, from, numReturnedLocs);
}