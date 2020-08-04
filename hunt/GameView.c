////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
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
#include "Queue.h"

#define MAX_REACHABLE 200
#define CURR_HUNTER gv->hunters[gv->whoseTurn]
#define DRAC_LHIST gv->dracula->locHistory
#define DRAC_MHIST gv->dracula->moveHistory
#define NOT_VISITED -1
#define VISITED 1
#define ROUND_CHARACTER_LENGTH 40
#define TURN_CHARS 8
#define LOC_SIZE_STR 3
#define HUNTER_INFO_START 3
#define HUNTER_INFO_END 7
#define HISTORY_SIZE ( \
	sizeof(enum placeId) * (pastPlaysLength / ROUND_CHARACTER_LENGTH + 1) \
)

typedef struct _hunterData {
	int health; 
	PlaceId currLoc;
	PlaceId * moveHistory; 
} HunterData; 

typedef struct _draculaData {
	int health; 
	PlaceId currLoc; 
	Round lastRevealed; 
	PlaceId * moveHistory;
	PlaceId * locHistory;
} DraculaData; 

struct gameView {
	int score;
	Round roundNum; 
	Player whoseTurn; 
	PlaceId vampireLocation; 
	PlaceId trapLocs[TRAIL_SIZE];
	int numTraps; 
	HunterData *hunters[NUM_PLAYERS - 1];
	DraculaData *dracula; 
	Map gameMap; 
	char *pastPlays; 
	int pastPlaysLength; 
};

void addNextRailway(
	GameView gv, PlaceId from, int depth, int maxRailwayDepth,
	int *visited, int *numReturnedLocs, PlaceId *reachableLocations
);
void reconstructGameState(GameView gv);
void addTrapAndShift(GameView gv, PlaceId loc);
int findTrap(GameView gv, PlaceId trapToDelete);
void deleteTrapAndShift(GameView gv, PlaceId trapToDelete);

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

// Creates and returns a new Hunter
static HunterData *createNewHunter(int pastPlaysLength) {
	HunterData *h = malloc(sizeof(struct _hunterData)); 

	if (h == NULL) {
		fprintf(stderr, "Couldn't allocate HunterData!\n");
		exit(EXIT_FAILURE);
	}

	h->health = GAME_START_HUNTER_LIFE_POINTS;
	h->currLoc = NOWHERE; 
	h->moveHistory = malloc(HISTORY_SIZE); 

	return h;
}

// Creates and returns a new Dracula
static DraculaData *createNewDracula(int pastPlaysLength) {
	DraculaData *d = malloc(sizeof(struct _draculaData));

	if (d == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaData!\n");
		exit(EXIT_FAILURE);
	}

	d->health = GAME_START_BLOOD_POINTS;
	d->currLoc = NOWHERE;
	d->lastRevealed = 0; 
	d->moveHistory = malloc(HISTORY_SIZE);
	d->locHistory = malloc(HISTORY_SIZE);
	d->locHistory[0] = NOWHERE;

	return d; 
}

// Creates GameView and calls reconstructGameState()
GameView GvNew(char *pastPlays, Message messages[]) {
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
	gv->numTraps = 0; 

	for (int i = 0; i < TRAIL_SIZE; i++) {
		gv->trapLocs[i] = NOWHERE; 
	}

	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		gv->hunters[i] = createNewHunter(pastPlaysLength);
	}

	gv->dracula = createNewDracula(pastPlaysLength);
	gv->gameMap = MapNew();
	reconstructGameState(gv);
	return gv;
}

// Frees GameView memory
void GvFree(GameView gv) {
	free(DRAC_LHIST);
	free(DRAC_MHIST);
	free(gv->dracula); 
	
	for (int i = 0; i < NUM_PLAYERS-1; i++) {
		free(gv->hunters[i]->moveHistory);
		free(gv->hunters[i]);
	}

	MapFree(gv->gameMap);
	free(gv);
}

void GvExtendGameState(GameView gv, char *extension, int extLength) {
	gv->pastPlays = extension;
	gv->pastPlaysLength = extLength;
	reconstructGameState(gv);
}

// Custom function that deep copies the current state of the game
// For the AI to use if they wanted to evaluate future cases
// Without modifying the actual state of the game.
GameView copyGameState(GameView copyFrom, char *extension, int extLength) {
	//copyInto->trapLocs = malloc(sizeof(enum placeId) * TRAIL_SIZE);
	GameView copyInto = malloc(sizeof(struct gameView));
	*copyInto = *copyFrom; 
	for (int i = 0; i < TRAIL_SIZE; i++) {
		copyInto->trapLocs[i] = copyFrom->trapLocs[i];
	}
	//copyInto->trapLocs = copyFrom->trapLocs;
	//copyInto->hunters = malloc(sizeof(struct _hunterData));
	for (int i = 0; i < NUM_PLAYERS -1; i++) {
		copyInto->hunters[i] = malloc(sizeof(struct _hunterData));
		*(copyInto->hunters[i]) = *(copyFrom->hunters[i]);
		//printf("allocating %ld\n", sizeof(enum placeId));
		copyInto->hunters[i]->moveHistory = malloc(sizeof(enum placeId) * ((copyFrom->roundNum+1) * 4));
		//*(copyInto->hunters[i]->moveHistory) = *(copyFrom->hunters[i]->moveHistory);
		memcpy(copyInto->hunters[i]->moveHistory, copyFrom->hunters[i]->moveHistory, sizeof(enum placeId) * ((copyFrom->roundNum+1) * 4));
	}
	//copyInto->hunters = copyFrom->hunters;
	//copyInto->hunters->moveHistory = malloc(sizeof(enum placeId) * ((copyFrom->pastPlaysLength + extLength) / ROUND_CHARACTER_LENGTH + 1));
	//copyInto->hunters->moveHistory = copyFrom->hunters->moveHistory;
	copyInto->dracula = malloc(sizeof(struct _draculaData));
	*(copyInto->dracula) = *(copyFrom->dracula); 
	copyInto->dracula->locHistory = malloc(sizeof(enum placeId) * ((copyFrom->roundNum+1) * 4));
	memcpy(copyInto->dracula->locHistory, copyFrom->dracula->locHistory, sizeof(enum placeId) * ((copyFrom->roundNum+1) * 4));
	//*(copyInto->dracula->locHistory) = *(copyFrom->dracula->locHistory);
	copyInto->dracula->moveHistory = malloc(sizeof(enum placeId) * ((copyFrom->roundNum+1) * ROUND_CHARACTER_LENGTH));
	memcpy(copyInto->dracula->moveHistory, copyFrom->dracula->moveHistory, sizeof(enum placeId) * ((copyFrom->roundNum+1) * 4));
	//*(copyInto->dracula->moveHistory) = *(copyFrom->dracula->locHistory);
	copyInto->gameMap = MapNew();
	copyInto->pastPlays = extension; 
	copyInto->pastPlaysLength = extLength; 
	reconstructGameState(copyInto);
	return copyInto;
}

// Walks through the game, turn by turn, round by round and updates all values
void reconstructGameState(GameView gv) {
	char loc[LOC_SIZE_STR];
	PlaceId currentLoc;

	for (int i = 0; i < gv->pastPlaysLength; i += TURN_CHARS) {
		sprintf(loc, "%c%c", gv->pastPlays[i + 1], gv->pastPlays[i + 2]);
		currentLoc = placeAbbrevToId(loc);

		if (gv->whoseTurn == PLAYER_DRACULA) {
			if (currentLoc == TELEPORT) {
				DRAC_LHIST[gv->roundNum] = CASTLE_DRACULA;
			} else if (currentLoc >= HIDE && currentLoc <= DOUBLE_BACK_5) {
				int offset = currentLoc - DOUBLE_BACK_1 + 1; // DOUBLE_BACK_n => n
				if (currentLoc == HIDE) offset++; // same as DOUBLE_BACK_1

				DRAC_LHIST[gv->roundNum] = gv->dracula->currLoc = DRAC_LHIST[gv->roundNum - offset];
			} else {
				DRAC_LHIST[gv->roundNum] = currentLoc;
				gv->dracula->currLoc = currentLoc;

				if (currentLoc != CITY_UNKNOWN && currentLoc != NOWHERE) {
					gv->dracula->lastRevealed = gv->roundNum;
				}
			}

			DRAC_MHIST[gv->roundNum] = currentLoc;

			if (DRAC_LHIST[gv->roundNum] == CASTLE_DRACULA) {
				gv->dracula->health += LIFE_GAIN_CASTLE_DRACULA;
			}

			if (placeIdToType(DRAC_LHIST[gv->roundNum]) == SEA) {
				gv->dracula->health -= LIFE_LOSS_SEA;
			}

			if (gv->pastPlays[i + 5] == 'V') {
				 gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
				 gv->vampireLocation = NOWHERE;
			} else if (gv->pastPlays[i + 5] == 'M') gv->trapLocs[--gv->numTraps] = NOWHERE;
			if (gv->pastPlays[i + 3] == 'T') addTrapAndShift(gv, gv->dracula->currLoc);
			if (gv->pastPlays[i + 4] == 'V') gv->vampireLocation = currentLoc;

			gv->score--;
			gv->roundNum++;
			
		} else {
			if (CURR_HUNTER->health <= 0) {
				CURR_HUNTER->health = GAME_START_HUNTER_LIFE_POINTS;
			}

			if (CURR_HUNTER->health > GAME_START_HUNTER_LIFE_POINTS) {
				CURR_HUNTER->health = GAME_START_HUNTER_LIFE_POINTS;
			}

			CURR_HUNTER->moveHistory[gv->roundNum] = currentLoc;
			CURR_HUNTER->currLoc = currentLoc;

			for (
				int hunterAction = HUNTER_INFO_START;
				hunterAction < HUNTER_INFO_END;
				hunterAction++
			) {
				if (gv->pastPlays[i + hunterAction] == 'T') {
					CURR_HUNTER->health -= LIFE_LOSS_TRAP_ENCOUNTER;
					deleteTrapAndShift(gv, currentLoc);
				} else if (gv->pastPlays[i + hunterAction] == 'V') {
					gv->vampireLocation = NOWHERE;
				} else if (gv->pastPlays[i + hunterAction] == 'D') {
					CURR_HUNTER->health -= LIFE_LOSS_DRACULA_ENCOUNTER;
					gv->dracula->health -= LIFE_LOSS_HUNTER_ENCOUNTER;
				}
			}

			if (CURR_HUNTER->health <= 0) {
				gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
				CURR_HUNTER->health = 0;
				CURR_HUNTER->currLoc = HOSPITAL_PLACE;
			}

			if (currentLoc == CURR_HUNTER->currLoc) {
				CURR_HUNTER->health += LIFE_GAIN_REST;
			}
		}

		gv->whoseTurn++;
		gv->whoseTurn %= NUM_PLAYERS;
	}
}

////////////////////////////////////////////////////////////////////////
// Game State Information

// Adds a new Trap Location to gv->trapLocs
void addTrapAndShift(GameView gv, PlaceId loc) {
	for (int i = TRAIL_SIZE - 1; i > 0; i--) {
		gv->trapLocs[i] = gv->trapLocs[i-1];
	}

	gv->trapLocs[0] = loc;
	if (gv->numTraps < 6) gv->numTraps++;
}

// Finds the location of a trap in a particular city
int findTrap(GameView gv, PlaceId trapToDelete) {
	for (int i = 0; i < gv->numTraps; i++) {
		if (gv->trapLocs[i] == trapToDelete) {
			return i; 
		}
	}

	return -1; 
}

// Deletes a trap, and shifts resulting NOWHERE to end of gv->trapLocs
void deleteTrapAndShift(GameView gv, PlaceId trapToDelete) {
	int index = findTrap(gv, trapToDelete);
	if (index == -1) return;

	for (int i = index; i < gv->numTraps - 1; i++) {
		gv->trapLocs[i] = gv->trapLocs[i + 1]; // Shift
	}

	gv->trapLocs[gv->numTraps - 1] = NOWHERE;
	gv->numTraps--;
}

// Gets the round number
Round GvGetRound(GameView gv) {
	return gv->roundNum; 
}

// Gets the current player
Player GvGetPlayer(GameView gv) {
	return gv->whoseTurn;
}

// Gets the current score
int GvGetScore(GameView gv) {
	return gv->score;
}

// Gets the current health of any player
int GvGetHealth(GameView gv, Player player) {
	if (player == PLAYER_DRACULA) {
		return gv->dracula->health; // The player is a hunter
	} else {
		return gv->hunters[player]->health; // The player is a dracula
	}
}

// Gets the current location of any player
PlaceId GvGetPlayerLocation(GameView gv, Player player) {
	if (player == PLAYER_DRACULA) {
		return gv->dracula->currLoc;
	} else {
		return gv->hunters[player]->currLoc;
	}

	return NOWHERE;
}

// Gets location of vampire
PlaceId GvGetVampireLocation(GameView gv) {
	return gv->vampireLocation; 
}

// Gets array of Trap locations
PlaceId *GvGetTrapLocations(GameView gv, int *numTraps) {
	int size = sizeof(enum placeId) * gv->numTraps;

	PlaceId *ret = malloc(size);
	memcpy(ret, gv->trapLocs, size);

	//ret = gv->trapLocs;
	*numTraps = gv->numTraps;
	return ret;
}

////////////////////////////////////////////////////////////////////////
// Game History

// Gets a player's MoveHistory
PlaceId *GvGetMoveHistory(
	GameView gv, Player player, int *numReturnedMoves, bool *canFree
) {
	PlaceId* ret;

	if (player != PLAYER_DRACULA) {
		ret = gv->hunters[player]->moveHistory;
	} else {
		ret = DRAC_MHIST;
	}

	*numReturnedMoves = gv->roundNum + (
		player >= gv->whoseTurn ? 0 : 1
	);

	*canFree = false;
	return ret;
}

// Returns a player's last N moves.
// Returns entire moveHistory array if N is greater than size of list
PlaceId *GvGetLastMoves(
	GameView gv, Player player, int numMoves,
	int *numReturnedMoves, bool *canFree
) {
	int startFrom = 0; 

	if (numMoves > gv->roundNum) {
		startFrom = 0; 
		*numReturnedMoves = gv->roundNum + (gv->whoseTurn > player); 
	} else {
		startFrom = gv->roundNum - numMoves + (gv->whoseTurn > player); 
		*numReturnedMoves = numMoves; 
	}

	PlaceId *ret;
	*canFree = false;

	if (player == PLAYER_DRACULA) {
		ret = &(gv->dracula->moveHistory[startFrom]);
	} else {
		ret = &(gv->hunters[player]->moveHistory[startFrom]);
	}

	return ret; 
}

// Gets a player's location history
// For hunter's this is the same as moveHistory
// For Dracula, moves such as HI and D3 have to correspond to actual locations
PlaceId *GvGetLocationHistory(
	GameView gv, Player player,
	int *numReturnedLocs, bool *canFree
) {
	PlaceId* ret;

	if (player != PLAYER_DRACULA) {
		ret = gv->hunters[player]->moveHistory;
	} else {
		ret = DRAC_LHIST;
	}

	*numReturnedLocs = gv->roundNum + (
		player >= gv->whoseTurn ? 0 : 1
	);

	*canFree = false;
	return ret;
}

// Gets a player's last N locations
// Returns entire Move/LocHistory array if N is greater than size of array
PlaceId *GvGetLastLocations(
	GameView gv, Player player, int numLocs,
	int *numReturnedLocs, bool *canFree
) {
	int startFrom = 0; 

	if (numLocs > gv->roundNum) {
		startFrom = 0;
		// The current round + whether or not the player has had his turn
		*numReturnedLocs = gv->roundNum + (gv->whoseTurn > player);
	} else {
		// If the player has had a turn this round add 1 to gv->roundNum - numLocs
		startFrom = gv->roundNum - numLocs + (gv->whoseTurn > player);  
		*numReturnedLocs = numLocs; 
	}

	PlaceId * ret;
	*canFree = false;

	if (player == PLAYER_DRACULA) {
		ret = &(gv->dracula->locHistory[startFrom]);
	} else {
		ret = &(gv->hunters[player]->moveHistory[startFrom]);
	}

	return ret; 
}

////////////////////////////////////////////////////////////////////////
// Making a Move


void addNextRailway(
	GameView gv, PlaceId from, int depth, int maxRailwayDepth,
	int * visited, int *numReturnedLocs, PlaceId * reachableLocations
) {
	// If we have visited the node, return. Otherwise set this node to visited. 
	if (visited[from] == VISITED) {
		return;
	} else {
		visited[from] = VISITED;
		if (depth >= maxRailwayDepth) return; 
	}

	visited[from] = VISITED;
	ConnList curr = MapGetConnections(gv->gameMap, from);

	while (curr != NULL) {
		if (curr->type == RAIL && visited[curr->p] == NOT_VISITED) {
			// Change definition of item to placeID
			reachableLocations[*numReturnedLocs] = curr->p;
			*numReturnedLocs += 1;

			addNextRailway(gv, curr->p, depth + 1, maxRailwayDepth, visited, numReturnedLocs, reachableLocations);
		}

		curr = curr->next;
	}
}

// Gets a array of places than are reachable by a player
PlaceId *GvGetReachable(
	GameView gv, Player player, Round round,
	PlaceId from, int *numReturnedLocs
) {
	return GvGetReachableByType(
		gv, player, round, from,
		true, true, true,
		numReturnedLocs
	);
}

// Returns an array of places a player can visit, with only approved transportation types
// Takes into accounts restrictions in methods of transport between Hunters and Dracs
// When all types are true, this forms implementation of GvGetReachable
PlaceId *GvGetReachableByType(
	GameView gv, Player player, Round round, PlaceId from,
	bool road, bool rail, bool boat, int *numReturnedLocs
) {
	*numReturnedLocs = 0; 

	ConnList connections = MapGetConnections(gv->gameMap, from);
	PlaceId *reachableLocations = malloc(sizeof(enum placeId) * MAX_REACHABLE);
	assert(reachableLocations != NULL);

	reachableLocations[0] = from;
	*numReturnedLocs += 1; 

	if (player != PLAYER_DRACULA) {
		// Player is a hunter
		// Evaluate railways first
		int maxRailwayDist = (round + player) % 4;

		int visited[NUM_REAL_PLACES];
		for (int i = 0; i < NUM_REAL_PLACES; i++) {
			visited[i] = NOT_VISITED;
		}

		if (maxRailwayDist > 0 && rail) {
			addNextRailway(
				gv, from, 0, maxRailwayDist, visited,
				numReturnedLocs, reachableLocations
			);
		}

		ConnList curr = connections;
		while (curr != NULL) {
			if (visited[curr->p] == NOT_VISITED && curr->type != RAIL) {
				if ((boat && curr->type == BOAT) || (road && curr->type == ROAD)) {
					reachableLocations[*numReturnedLocs] = curr->p;
					*numReturnedLocs += 1;
				}
			}

			curr = curr->next;
		}
	} else {
		ConnList curr = connections;
		while (curr != NULL) {
			if (curr->p != HOSPITAL_PLACE && curr->type != RAIL) {
				if ((boat && curr->type == BOAT) || (road && curr->type == ROAD)) {
					reachableLocations[*numReturnedLocs] = curr->p;
					*numReturnedLocs += 1;
				}
			}
			curr = curr->next; 
		}
	}

	return reachableLocations;
}

// If a hunter calls this it will return CITY_UNKNOWN
PlaceId GvGetLastKnownDraculaLocation(GameView gv, int *round) {
	if (DRAC_LHIST[gv->dracula->lastRevealed] != NOWHERE) {
		*round = gv->dracula->lastRevealed;
	}

	return DRAC_LHIST[gv->dracula->lastRevealed];
}
