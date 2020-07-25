////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
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
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here
#include "Queue.h"
// TODO: ADD YOUR OWN STRUCTS HERE
#define MAX_REACHABLE 200
#define NOT_VISITED -1
#define VISITED 1
typedef struct _hunterData {
	int health; 
	PlaceId currLoc;
	PlaceId * moveHistory; 
} HunterData; 

typedef struct _draculaData {
	int health; 
	PlaceId currLoc; 
	PlaceId * moveHistory;
} DraculaData; 

struct gameView {
	// TODO: ADD FIELDS HERE
	int score;
	Round roundNum; 
	Player whoseTurn; 
	PlaceId vampireLocation; 
	PlaceId trapLocs[TRAIL_SIZE];
	HunterData *hunters[NUM_PLAYERS-1];
	DraculaData *dracula; 
	Map gameMap; 
	char *pastPlays; 
	int pastPlaysLength; 
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

static HunterData *createNewHunter(int pastPlaysLength) {
	HunterData *h = malloc(sizeof(struct _hunterData)); 
	if (h == NULL) {
		fprintf(stderr, "Couldn't allocate HunterData!\n");
		exit(EXIT_FAILURE);
	}
	h->health = GAME_START_HUNTER_LIFE_POINTS;
	h->currLoc = NOWHERE; 
	// Should optimize by finding max possible instead of just assigning pastPlaysLength
	h->moveHistory = malloc(sizeof(enum placeId) * pastPlaysLength); 
	return h;
}

static DraculaData *createNewDracula(int pastPlaysLength){
	DraculaData *d = malloc(sizeof(struct _draculaData));
	if (d == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaData!\n");
		exit(EXIT_FAILURE);
	}
	d->health = GAME_START_BLOOD_POINTS;
	d->currLoc = NOWHERE;
	d->moveHistory = malloc(sizeof(enum placeId) * pastPlaysLength);
	return d; 
}

GameView GvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView gv = malloc(sizeof(struct gameView));
	if (gv == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	int pastPlaysLength = strlen(pastPlays);
	gv->pastPlays = pastPlays;
	gv->pastPlaysLength = pastPlaysLength;
	gv->score = GAME_START_SCORE;
	gv->roundNum = 0; 
	gv->whoseTurn = PLAYER_LORD_GODALMING; 
	gv->vampireLocation = NOWHERE;
	for(int i =0; i < TRAIL_SIZE; i++){
		gv->trapLocs[i] = NOWHERE; 
	}
	for(int i =0; i < NUM_PLAYERS-1; i++){
		gv->hunters[i] = createNewHunter(pastPlaysLength);
	}
	gv->dracula = createNewDracula(pastPlaysLength);
	gv->gameMap = MapNew();
	return gv;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(gv->dracula->moveHistory);
	free(gv->dracula); 
	for (int i = 0; i < NUM_PLAYERS-1; i++){
		free(gv->hunters[i]->moveHistory);
		free(gv->hunters[i]);
	}
	MapFree(gv->gameMap);
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->roundNum; 
}

Player GvGetPlayer(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->whoseTurn;
	//return PLAYER_LORD_GODALMING;
}

int GvGetScore(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->score;
}

int GvGetHealth(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (player == PLAYER_DRACULA) {
		// The player is a hunter.
		return gv->dracula->health; 
	} else {
		// The player is a dracula.
		return gv->hunters[player]->health;
	}
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (player == PLAYER_DRACULA) {
		return gv->dracula->currLoc;
	} else {
		return gv->hunters[player]->currLoc;
	}
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return gv->vampireLocation; 
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move
// Currently using DFS traversal + linear probing to find railways. 
// Replace with a better implementation if we find one.
// Not 100% satisfied with the time complexity.

void addNextRailway(GameView gv, PlaceId from, int depth, int maxRailwayDepth, int * visited, int *numReturnedLocs, PlaceId * reachableLocations){
	if (depth >= maxRailwayDepth || visited[from] != NOT_VISITED) return; 
	ConnList curr = MapGetConnections(gv->gameMap, from);
	while(curr != NULL) {
		if (curr->type == RAIL && visited[curr->p] == NOT_VISITED) {
			// change definition of item to placeID.
			reachableLocations[*numReturnedLocs] = curr->p;
			*numReturnedLocs+=1;
			visited[curr->p] = VISITED;
			addNextRailway(gv, curr->p, depth+1, maxRailwayDepth, visited, numReturnedLocs, reachableLocations);
		}
		curr = curr->next;
	}
}



PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	ConnList connections = MapGetConnections(gv->gameMap, from);
	PlaceId *reachableLocations = malloc(sizeof(enum placeId) * MAX_REACHABLE);
	assert(reachableLocations != NULL);
	reachableLocations[0] = from;
	*numReturnedLocs+=1; 
	if (player != PLAYER_DRACULA) {
		// Player is a hunter.
		// Evaluate railways first. 
		int maxRailwayDist = (round + player) % 4;
		int visited[NUM_REAL_PLACES];
		for(int i=0; i < NUM_REAL_PLACES; i++){
			visited[i] = NOT_VISITED;
		}
		if (maxRailwayDist > 0) {
			addNextRailway(gv, from, 0, maxRailwayDist, visited, numReturnedLocs, reachableLocations);
		}
		ConnList curr = connections;
		while(curr!= NULL) {
			if (visited[curr->p] == NOT_VISITED && curr->type != RAIL) {
				reachableLocations[*numReturnedLocs] = curr->p;
				*numReturnedLocs+=1;
			}
			curr = curr->next;
		}
		//dropQueue(railways);
	} else {
		ConnList curr = connections;
		while(curr != NULL) {
			if (curr->p != HOSPITAL_PLACE && curr->type != RAIL){
				reachableLocations[*numReturnedLocs] = curr->p;
				*numReturnedLocs+=1;
			}
			curr = curr->next; 
		}
	}
	return reachableLocations;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0; 
	ConnList connections = MapGetConnections(gv->gameMap, from);
	PlaceId *reachableLocations = malloc(sizeof(enum placeId) * MAX_REACHABLE);
	assert(reachableLocations != NULL);
	reachableLocations[0] = from;
	*numReturnedLocs+=1; 

	if (player != PLAYER_DRACULA) {
		// Player is a hunter.
		// Evaluate railways first. 
		int maxRailwayDist = (round + player) % 4;
		int visited[NUM_REAL_PLACES];
		for(int i=0; i < NUM_REAL_PLACES; i++){
			visited[i] = NOT_VISITED;
		}
		if (maxRailwayDist > 0 && rail) {
			addNextRailway(gv, from, 0, maxRailwayDist, visited, numReturnedLocs, reachableLocations);
		}
		ConnList curr = connections;
		while(curr!= NULL) {
			if (visited[curr->p] == NOT_VISITED && curr->type != RAIL) {
				if ((boat && curr->type == BOAT) || (road && curr->type == ROAD)) {
					reachableLocations[*numReturnedLocs] = curr->p;
					*numReturnedLocs+=1;
				}
			}
			curr = curr->next;
		}
		//dropQueue(railways);
	} else {
		ConnList curr = connections;
		while(curr != NULL) {
			if (curr->p != HOSPITAL_PLACE && curr->type != RAIL){
				if ((boat && curr->type == BOAT) || (road && curr->type == ROAD)) {
					reachableLocations[*numReturnedLocs] = curr->p;
					*numReturnedLocs+=1;
				}
			}
			curr = curr->next; 
		}
	}
	printf("Returned length is %d\n", *numReturnedLocs);
	return reachableLocations;
	/*
	int len = 0; 
	PlaceId * allReachable = GvGetReachable(gv, player, round, from, &len);
	PlaceId * reachableFiltered = malloc(sizeof(enum placeId) * len);
	assert(reachableFiltered != NULL); 
	reachableFiltered[0] = from; 
	printf("len is %d\n", len);
	int retLength = 1; 
	for (int i =1; i < len; i++) {
		PlaceType nodeType = placeIdToType(allReachable[i]);
		if ((nodeType == ROAD && road == true) || (nodeType == SEA && boat == true) || (nodeType == RAIL && rail == true)) {
			reachableFiltered[retLength] = allReachable[i];
			retLength++;
		}
	}
	*numReturnedLocs = retLength;
	return reachableFiltered;
	*/
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
