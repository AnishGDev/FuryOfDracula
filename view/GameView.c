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
#define CURR_HUNTER gv->hunters[gv->whoseTurn]

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
	// optimise.
	h->moveHistory = malloc(sizeof(enum PlaceId) * (pastPlaysLength/40+1)); 
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
	d->moveHistory = malloc(sizeof(enum PlaceId) * (pastPlaysLength/40+1));
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
	gv->roundNum = -1; 
	//gv->whoseTurn = PLAYER_LORD_GODALMING; 
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

/*void populateLocation(char player, char* location, int round) {
	switch (player) {
		case 'D':
			if (!strcmp(loc, "C?"))
				gv->dracula->moveHistory[round] = CITY_UNKNOWN;
			else if (!strcmp(loc, "S?"))
				gv->dracula->moveHistory[round] = SEA_UNKNOWN;
			else
			else for (int i = 0; i < 71)
			break;
		case 'G':

			break;
		case 'S':

			break;
		case 'H':

			break;
		case 'M':

			break;
	}
}*/

void appendTrapLoc(GameView gv, PlaceId loc) {
	for (int i = 0; i < TRAIL_SIZE - 1; i++) {
		gv->trapLocs[i] = gv->trapLocs[i+1];
	}
	gv->trapLocs[TRAIL_SIZE-1] = loc;
}

void reconstructGameState(GameView gv) {
	char* loc = malloc(sizeof(char)*3);
	PlaceId currentLoc;
	for (int i = 0; i < gv->pastPlaysLength; i += 8) {
		if (i % 5 == 0) {
			gv->roundNum++;
			gv->score--;
			gv->whoseTurn = PLAYER_LORD_GODALMING;
		}
		sprintf(loc, "%c%c", gv->pastPlays[i+1], gv->pastPlays[i+2])
		currentLoc = placeAbbrevToID(loc);
		if (gv->whoseTurn == PLAYER_DRACULA) {
			switch (currentLoc) {
				case TELEPORT:
					gv->dracula->moveHistory[gv->roundNum] = CASTLE_DRACULA;
					break;
				case HIDE:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-1];
					break;
				case DOUBLE_BACK_1:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-1];
					break;
				case DOUBLE_BACK_2:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-2];
					break;
				case DOUBLE_BACK_3:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-3];
					break;
				case DOUBLE_BACK_4:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-4];
					break;
				case DOUBLE_BACK_5:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-5];
					break;
				case DOUBLE_BACK_6:
					gv->dracula->moveHistory[gv->roundNum] = gv->dracula->moveHistory[gv->roundNum-6];
					break;
				default:
					gv->dracula->moveHistory[gv->roundNum] = currentLoc;
					break;
			}
			if (gv->dracula->moveHistory[gv->roundNum] == CASTLE_DRACULA) 
				gv->dracula->health += 10;
			if (pastPlays[i+3] == 'T') appendTrapLoc(gv, currentLoc);
			if (pastPlays[i+4] == 'V') gv->vampireLocation = currentLoc;
			if (pastPlays[i+5] == 'V') gv->score -= 13;
		} else {
			if (currentLoc == CURR_HUNTER->currLoc)
				CURR_HUNTER->health += 3;
			if (CURR_HUNTER->health > 9) 
				CURR_HUNTER->health = 9;
			CURR_HUNTER->moveHistory[gv->roundNum] = currentLoc;
			CURR_HUNTER->currLoc = currentLoc;
			if (pastPlays[i+3] == 'T') 
				CURR_HUNTER->health -= 2;
			if (pastPlays[i+4] == 'V')
				gv->vampireLocation = NOWHERE;
			if (pastPlays[i+4] == 'D') {
				CURR_HUNTER->health -= 4;
				gv->dracula->health -= 10;
			}
			if (CURR_HUNTER->health <= 0) {
				gv->score -= 6;
				CURR_HUNTER->health = 9;
			}	
		}
		gv->whoseTurn++;
	}
	if (gv->whoseTurn >= PLAYER_DRACULA) gv->whoseTurn = PLAYER_LORD_GODALMING;
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
	if (player != PLAYER_DRACULA) {
		// The player is a hunter.
		return gv->hunters[player]->health;
	} else {
		// The player is a dracula.
		return gv->dracula->health; 
	}
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (player != PLAYER_DRACULA) {
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
	PlaceId *ret = malloc(sizeof(ret));
	if (player != PLAYER_DRACULA) {
		ret = gv->hunters[player]->moveHistory;
	} else {
		ret = gv->dracula->moveHistory;
	}
	if (player >= gv->whoseTurn) {
		*numReturnedLocs = gv->roundNum-1;
	} else {
		*numReturnedLocs = gv->roundNum;
	}
	*canFree = false;
	return ret;
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

// I don't think this works because of double railway links. Madrid/Alicante/Barcelona/Saragossa
void addNextRailway(GameView gv, Queue railways, PlaceId from, int depth, int maxRailwayDepth){
	if (depth >= maxRailwayDepth) return; 
	ConnList curr = MapGetConnections(gv->gameMap, from);
	while(curr != NULL) {
		if (curr->type == RAIL) {
			QueueJoin(railways, curr->p); // change definition of item to placeID.
			addNextRailway(gv, railways, curr->p, depth+1, maxRailwayDepth);
		}
		curr = curr->next;
	}
}

bool linearScan(PlaceId *list, PlaceId itemToFind, int len) {
    for (int i =0; i < len; i++){
		if(list[i] == itemToFind){
            return true;
		}
    }
    return false;
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
		Queue railways = newQueue();
		addNextRailway(gv, railways, from, 0, maxRailwayDist);
		ConnList curr = connections;
		while(curr!= NULL) {
			if (curr->type != RAIL) {
				reachableLocations[*numReturnedLocs] = curr->p;
				*numReturnedLocs+=1;
			}
			curr = curr->next;
		}
		while(!QueueIsEmpty(railways)) {
			Item popped = QueueLeave(railways);
			if(linearScan(reachableLocations, popped, *numReturnedLocs) == false){
				reachableLocations[*numReturnedLocs] = popped;
				*numReturnedLocs+=1;
			}
		}
		dropQueue(railways);
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
	int len = 0; 
	PlaceId * allReachable = GvGetReachable(gv, player, round, from, &len);
	PlaceId * reachableFiltered = malloc(sizeof(enum placeId) * len);
	assert(reachableFiltered != NULL); 
	reachableFiltered[0] = from; 
	int retLength = 1; 
	for (int i =1; i < len; i++) {
		PlaceType nodeType = placeIdToType(allReachable[i]);
		if ((nodeType == ROAD && road == true) || (nodeType == BOAT && boat == true) || (nodeType == RAIL && rail == true)) {
			reachableFiltered[retLength] = allReachable[i];
			retLength++;
		}
	}
	*numReturnedLocs = retLength;
	return reachableFiltered;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
