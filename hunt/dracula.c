////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
#include <limits.h>
#include <assert.h> // for testing purposes. Remove later.
// structure to store a snapshot of a game state. 
#define MAX_DEPTH 3 // might increase depending on how long it takes to evaluate.
#define INFINITY 1e9
#define NEGATIVE_INFINITY -1e9


typedef struct _state {
	PlaceId whereIsPlayer[5]; // to keep track of which placeID. 5th one is dracula. 
	// used in evalFunction. 
	int health;
	int score;
	PlaceId * trail;// Store current trail info. 
	int distFromPlayers[NUM_PLAYERS];
	// store CD later.
} State; 

static inline int max(int a, int b) {
	return a > b ? a : b; 
}

static inline int min(int a, int b) {
	return a < b ? a : b; 
}

int minimax(DraculaView dv, State currState, int currDepth, bool isMaximising);

void decideDraculaMove(DraculaView dv)
{
	// TODO: Replace this with something better!
	// Set up initial root state
	int currScore = DvGetScore(dv);
	if (currScore == GAME_START_SCORE) {
		// Only first round. 
		registerBestPlay("CD", "Mwahahahaha");
	} else {
		State * s = malloc(sizeof(struct _state)); 
		s->whereIsPlayer[PLAYER_DRACULA] = DvGetPlayerLocation(dv, PLAYER_DRACULA);
		for (int i =0; i < NUM_PLAYERS - 1; i++) {
			s->whereIsPlayer[i] = DvGetPlayerLocation(dv, i); 
			s->distFromPlayers[i] = calculateDistance(dv, s->whereIsPlayer[PLAYER_DRACULA], s->whereIsPlayer[i]); //calculateDistance(dv, from, to); 
		}
		// 5th element will represent distance to castle dracula.
		s->distFromPlayers[PLAYER_DRACULA] = calculateDistance(dv, s->whereIsPlayer[PLAYER_DRACULA], CASTLE_DRACULA); 
		s->health = DvGetHealth(dv, PLAYER_DRACULA);
		s->score = currScore;
		int numReturned = 0;
		bool canFree = false; 
		s->trail = returnCurrentTrail(dv, &numReturned, &canFree); 
		registerBestPlay("CD", "Mwahahahaha");
	}
}

State updateTrailInfo(State *s) {
	return *s;
}

// Takes a game state s, and evaluates it based on how good is it.
int evalFunction(State s) {
	return 0; 
}
// If Dracula was at place p, what would his distance to other players be?
void evalDistFromPlayers(DraculaView dv, PlaceId p, State *s) {
	for (int i =0; i < NUM_PLAYERS; i++) {
		s->distFromPlayers[i] = calculateDistance(dv, p, PLAYER_DRACULA);
	}
}
// Takes in a possible positions and evaluates their state.
// Returns a list. 
State * evaluatePositionState(PlaceId *positions, Player currPlayer,State *rootState, int numPositions) {
	// From the root state, generate possible states.
	State *s = malloc(sizeof(struct _state) * numPositions); 
	for (int i =0; i < numPositions; i++) {
		s[i] = *rootState; 
		s[i].whereIsPlayer[currPlayer] = positions[i];
	}
	return s;
}

void appendToTrail(PlaceId * trail) {

}
// Returns best move. 
PlaceId minimaxHelper(DraculaView dv, State *rootState, int currDepth, bool isMaximising) {
	// For the rootState, call the recursive function on its children
	// And evaluate the max. 
	int len = -1; 
	PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
	//State * s = evaluatePositionState(possiblePositions, PLAYER_DRACULA, rootState, len);
	int maxScore = INT_MIN; // Very smoll number. 
	PlaceId retValue; 
	for (int i =0; i < len; i++) {
		State s = *rootState;
		
		s.whereIsPlayer[PLAYER_DRACULA] = possiblePositions[i];
		evalDistFromPlayers(dv, possiblePositions[i], &s); 
		//maxScore = max(maxScore, minimax(dv, s, currDepth+1, !isMaximising)); // switch isMaximising.
		int score = minimax(dv, s, currDepth+1, !isMaximising); // switch isMaximising.
		if (score > maxScore) {
			maxScore = score;
			retValue = possiblePositions[i]; 
		}
	}
	return retValue;
}

// Evaluate each substate and return a score.
// Need to support Hunter States as well. 
// Perhaps only consider distance for now? Hunter's goal is to minimize distance. 
int minimax(DraculaView dv, State currState, int currDepth, bool isMaximising) {
	if (currDepth >= MAX_DEPTH) return evalFunction(currState);
	int len = -1; 
	//PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
	if (isMaximising) {
		int maxScore = INT_MIN; // very low number. 
		PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
		//State * s = evaluatePositionState(possiblePositions, PLAYER_DRACULA, &currState, len);
		for (int i = 0; i < len; i++) {
			State s = currState; 
			s.whereIsPlayer[PLAYER_DRACULA] = possiblePositions[i];
			evalDistFromPlayers(dv, possiblePositions[i], &s); 
			maxScore = max(maxScore, minimax(dv, s, currDepth+1, !isMaximising)); // switch isMaximising.
		}
		assert(maxScore != INT_MIN);
		return maxScore;
	} else {
		//int minScore = 1e9; // very high number.
		PlaceId minScores[NUM_PLAYERS-1] = {NOWHERE, NOWHERE, NOWHERE, NOWHERE}; 
		State newState = currState;
		int score = INT_MAX; 
		for (int player =0; player < NUM_PLAYERS - 1; player++) {
			PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
			int global_min = INT_MAX; 
			for (int currPosIndex =0; currPosIndex < len; currPosIndex++) {
				int curr_min = calculateDistance(dv, possiblePositions[currPosIndex], currState.whereIsPlayer[PLAYER_DRACULA]);
				/*
				State s = currState; 
				s.whereIsPlayer[i] = po
				minScore = min(minScore, minimax(dv, s[i], currDepth+1, !isMaximising));
				*/
				if (curr_min < global_min) {
					global_min = curr_min;
					minScores[player] = possiblePositions[currPosIndex];
				}
			}
			score = min(score, global_min);
			newState.whereIsPlayer[player] = minScores[player];
		}
		minimax(dv, newState, currDepth+1, !isMaximising);
		//assert(minScore != 1e9);
		// Currently just returns the smallest distance between them
		return score; 
	}
}