////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
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
#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"

static bool hiddenInLast5(DraculaView dv, PlaceId *moveHist, int numHistMoves);
static bool doubleInLast5(DraculaView dv, PlaceId *moveHist, int numHistMoves);
static PlaceId *ReplaceWithDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
);
static PlaceId *RemoveDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
);
// Cache trail info to reduce speed for repeated calls. 
#define VISITED 1
#define NOT_VISITED -1
struct _trailInfo {
	PlaceId * trail;
	int trailLen;
	bool isRealTrail; 
	bool canFree;
};

// The basic idea is to mirror the gameView struct but allow the AI
// to modify it so it can store future versions of this game and evaluate them.
// This ensures that the main gameView itself is not modified so we can get original information 
// from it
struct draculaView {
	GameView gv;
	Trail trailInfo;
	// Internal representation which can be modified to get a theoretical state.
	// This allows dracula AI to modify states and explore options without modifying actual state.
	PlaceId whereIsPlayer[NUM_PLAYERS]; // idk why but whereIsPlayer[i] is not working.
	Round round; // internal storage of round.
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[]) {
	DraculaView new = malloc(sizeof(struct draculaView));

	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}
	new->gv = GvNew(pastPlays,messages);
	new->trailInfo = malloc(sizeof(struct _trailInfo));
	new->trailInfo->trail = GvGetLastMoves(new->gv, PLAYER_DRACULA, 
							TRAIL_SIZE-1, &(new->trailInfo->trailLen), &(new->trailInfo->canFree));
							
	//new->whereIsPlayer = malloc(sizeof(enum placeId) * NUM_PLAYERS); 
	for (int i =0; i < NUM_PLAYERS; i++) {
		new->whereIsPlayer[i] = GvGetPlayerLocation(new->gv, i);
	}
	new->round = GvGetRound(new->gv);
	//new->isRealTrail = true;
	return new;
}

void DvFree(DraculaView dv) {
	// getLocation functions handles the freeing for trail. The struct simply stores it. 
	//free(dv->trailInfo);
	//free(dv->whereIsPlayer);
	GvFree(dv->gv);
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv) {
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv) {
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player) {
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player) {
	return GvGetPlayerLocation(dv->gv, player);
}

PlaceId DvGetVampireLocation(DraculaView dv) {
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps) {
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves) {
	// Check if dracula has made a move
	if (DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE) {
		*numReturnedMoves = 0;
		return NULL;
	}

	PlaceId *moves = DvWhereCanIGoByType(dv, true, true, numReturnedMoves);


	int numHistMoves;
	bool canFree = false;
	//int numMoves = 5;
	PlaceId * trailMoves = returnCurrentTrail(dv, &numHistMoves, &canFree);
	/*
	PlaceId *trailMoves = GvGetLastMoves(
		dv->gv, PLAYER_DRACULA, numMoves, &numHistMoves, &canFree
	);
	*/
	bool doubleBackInTrail = doubleInLast5(dv, trailMoves, numHistMoves);
	
	if (!doubleBackInTrail) {	// Add Double backs as moves
		moves = ReplaceWithDoubleBack(moves, trailMoves, numHistMoves, numReturnedMoves);
	} else {
		moves = RemoveDoubleBack(moves, trailMoves, numHistMoves, numReturnedMoves);
	}

	bool hiddenInTrail = hiddenInLast5(dv, trailMoves, numHistMoves);

	if (placeIdToType(DvGetPlayerLocation(dv, PLAYER_DRACULA)) != SEA) { // Can't Hide at sea
		if (!hiddenInTrail) { // Add Hide as a move
			*numReturnedMoves += 1;
			moves = realloc(moves, sizeof(PlaceId) * (*numReturnedMoves));
			moves[*numReturnedMoves-1] = HIDE;
		}
	}
	if (canFree) free(trailMoves);
	if (*numReturnedMoves == 0) {
		free(moves);
		return NULL;
	}
	return moves;
}


int calculateHunterDistFromDrac(DraculaView dv,Player player, Round round, PlaceId from, PlaceId dest) {
	Queue q = newQueue();
	QueueJoin(q, from);
	//int ret_distance = 0;
	PlaceId visited[NUM_REAL_PLACES]; 
	int distance[NUM_REAL_PLACES] = {0};
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		visited[i] = -1; 
		//distance[i] = -1;
	}
	visited[from] = from; 
	while(!QueueIsEmpty(q)) {
		Item popped = QueueLeave(q);
		if(popped == dest) {
			break; 
		}
		//printf("Curent is %s\n", placeIdToName(popped));
		int length = -1; 
		PlaceId * possibleLocs = GvGetReachable(dv->gv, player, round, popped, &length);
		for (int i = 0; i < length; i++) {
			if(visited[possibleLocs[i]] == NOT_VISITED) {
				distance[possibleLocs[i]] = distance[popped] + 1; 
				visited[possibleLocs[i]] = popped;
				QueueJoin(q, possibleLocs[i]);
			}
		}
	}
	printf("%s<-", placeIdToName(dest));
	PlaceId curr = visited[dest];
	while(curr != from) {
		printf("%s<-", placeIdToName(curr));
		curr = visited[curr];
	}
	printf("%s", placeIdToName(curr));
	printf("\n");
	return distance[dest]; // Something went wrong. 
}

void setTheoreticalState(DraculaView dv, PlaceId * trailToSet, Round round, PlaceId whereIsPlayer[]) {
	// Set internal representation to what the Dracula AI wants to evaluate future states. 
	dv->round = round;
	memcpy(dv->whereIsPlayer, whereIsPlayer, sizeof(PlaceId) * NUM_PLAYERS);
	memcpy(dv->trailInfo->trail, trailToSet, sizeof(PlaceId) * TRAIL_SIZE);
	//dv->trailInfo->trail = trailToSet;
}

void resetTheoreticalState(DraculaView dv) {
	dv->trailInfo->trail = GvGetLastMoves(dv->gv, PLAYER_DRACULA, 
							TRAIL_SIZE-1, &(dv->trailInfo->trailLen), &(dv->trailInfo->canFree));
}

PlaceId * returnCurrentTrail(DraculaView dv, int *numReturnedLocs, bool * canFree) {
	// Return cached trail locations. Can be used to change represent future states by AI. 
	*numReturnedLocs = dv->trailInfo->trailLen;
	*canFree = dv->trailInfo->canFree; 
	return dv->trailInfo->trail;
} 



PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	return DvWhereCanIGoByType(dv, true, true, numReturnedLocs);
}

PlaceId *DvWhereCanIGoByType(
	DraculaView dv, bool road, bool boat, int *numReturnedLocs
) {
	return DvWhereCanTheyGoByType(
		dv, PLAYER_DRACULA, road, false, boat, numReturnedLocs
	);
}

PlaceId *DvWhereCanTheyGo(
	DraculaView dv, Player player, int *numReturnedLocs
) {
	if (player == PLAYER_DRACULA) {
		return DvWhereCanTheyGoByType(
			dv, PLAYER_DRACULA, true, false, true, numReturnedLocs
		);
	} else {
		return DvWhereCanTheyGoByType(
			dv, player, true, true, true, numReturnedLocs
		);
	}
}

// Subset of Gv such that we can modify individual player locations
// This allows us to set "future states" of the game and evaluate them in the AI. 


PlaceId *DvWhereCanTheyGoByType(
	DraculaView dv, Player player,
	bool road, bool rail, bool boat,
	int *numReturnedLocs
) {
	// Check if player has made a move
	if (GvGetPlayerLocation(dv->gv, player) == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}

	if (player == PLAYER_DRACULA) {
		PlaceId *places = GvGetReachableByType(dv->gv, player, dv->round, dv->whereIsPlayer[PLAYER_DRACULA], road, rail, boat, numReturnedLocs);
		/*
		PlaceId *places = GvGetReachableByType(dv->gv, player, DvGetRound(dv),
												DvGetPlayerLocation(dv, player),road, 
												rail, boat, numReturnedLocs);
		*/
		// Account for trail Restrictions
	
		int numHistMoves;
		bool canFree = false;
		//int numMoves = 5;
		PlaceId * trailMoves = returnCurrentTrail(dv, &numHistMoves, &canFree);
		/*
		PlaceId *trailMoves = GvGetLastMoves(
			dv->gv, PLAYER_DRACULA, numMoves, &numHistMoves, &canFree
		);
		*/
		bool doubleBackInTrail = doubleInLast5(dv, trailMoves, numHistMoves);

		if (doubleBackInTrail) { // Remove Double back locations
			places = RemoveDoubleBack(places, trailMoves, numHistMoves, numReturnedLocs);
			
			// If still can hide, add current location Back
			bool hiddenInTrail = hiddenInLast5(dv, trailMoves, numHistMoves);
			if (!hiddenInTrail) {
				PlaceId currLoc = DvGetPlayerLocation(dv, PLAYER_DRACULA);
				bool in = false;

				for (int i = 0; i < *numReturnedLocs; i++) {
					if (places[i] == currLoc) {
						in = true;
						break;
					}
				}

				if (!in) {
					*numReturnedLocs += 1;
					places = realloc(places, sizeof(PlaceId) * (*numReturnedLocs));
					places[*numReturnedLocs - 1] = currLoc;
				}
			}
		}

		if (canFree) free(trailMoves);
		return places;
	} else {
		return GvGetReachableByType(
			dv->gv, player, dv->round,
			dv->whereIsPlayer[player],
			road, rail, boat,
			numReturnedLocs
		);
	}
}

// Checks if dracula has used a Hide in the last 5 moves
static bool hiddenInLast5 (DraculaView dv, PlaceId *moveHist, int numHistMoves) {
	bool found = false;

	for (int i = 0; i < numHistMoves; i++) {
		if (moveHist[i] == HIDE) {
			found = true;
			break;
		}
	}

	return found;
}

//Checks if dracula has used a doubleback in the last 5 moves
static bool doubleInLast5 (DraculaView dv, PlaceId *moveHist, int numHistMoves) {
	bool found = false;

	for (int i = 0; i < numHistMoves; i++) {
		//IF move is DOUBLE_BACK_1 or 2 or 3 or 4 or 5
		if (moveHist[i] >= DOUBLE_BACK_1 && moveHist[i] <= DOUBLE_BACK_5) {
			found = true;
			break;
		}
	}

	return found;
}

// Replaces Items in locations with Double Backs; assumes Double Back isn't in trail
static PlaceId *ReplaceWithDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
) {
	// Go through places and see if they occur in Dracula's trail, if they do, replace
	// them with double_back_n
	for (int i = 0; i < numHistMoves; i++) {
		for (int j = 0; j < *numReturnedLocs; j++) {
			if (trailMoves[i] == locations[j]) {
				// Move to somewere 1 moves ago -- how many moves before it was
				locations[j] = DOUBLE_BACK_1 + i; // trailMoves[i]; 
			}
		}
	}

	return locations;
}

static PlaceId *RemoveDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
) {
	PlaceId remove = -3;

	// Go through places and see if they occur in Dracula's trail, if they do, remove them
	for (int i = 0; i < numHistMoves; i++) {
		for (int j = 0; j < *numReturnedLocs; j++) {
			if (trailMoves[i] == locations[j]) {
				// Move to somewere 5 moves ago - how many moves before it was
				locations[j] = remove;
			}
		}
	}

	// Rearrange the array

	int numShift = 0;
	for (int i = 0; i < *numReturnedLocs; i++) {
		if (locations[i] == remove) {
			numShift++;
		} else {
			locations[i-numShift] = locations[i];
		}
	}

	*numReturnedLocs -= numShift;
	if (numReturnedLocs == 0) {
		free(locations);
		locations = NULL;
	}

	return locations;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
