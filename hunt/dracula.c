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
#include <assert.h> // for testing purposes. Remove later.
// structure to store a snapshot of a game state. 
#define MAX_DEPTH 3 // might increase depending on how long it takes to evaluate.

typedef struct _state {
	PlaceId id; // to keep track of which state. 
	// used in evalFunction. 
	int health;
	int score;
	int distFromPlayers[NUM_PLAYERS]; // 5TH one could be Castle Drac with negative weight. 
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
	registerBestPlay("CD", "Mwahahahaha");
}

// Takes a game state s, and evaluates it based on how good is it.
int evalFunction(State s) {
	return 0; 
}
// If Dracula was at place p, what would his distance to other players be?
void evalDistFromPlayers(PlaceId p, State s) {

}
// Takes in a possible positions and evaluates their state.
// Returns a list. 
State * evaluatePositionState(PlaceId *positions) {
	return NULL;
}
// Returns best state. 
State minimaxHelper(DraculaView dv, State rootState, int currDepth, bool isMaximising) {
	// For the rootState, call the recursive function on its children
	// And evaluate the max. 
	int len = -1; 
	PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
	State * s = evaluatePositionState(possiblePositions);
	int maxScore = -1e9; // Very smoll number. 
	int index = -1; 
	for (int i =0; i < len; i++) {
		int score = minimax(dv, s[i], currDepth+1, !isMaximising); // switch isMaximising.
		if (score > maxScore) {
			maxScore = score;
			index = i; 
		}
	}
	return s[index];
}
// Evaluate each substate and return a score.
// Need to support Hunter States as well. 
// Perhaps only consider distance for now? Hunter's goal is to minimize distance. 
int minimax(DraculaView dv, State currState, int currDepth, bool isMaximising) {
	if (currDepth >= MAX_DEPTH) return evalFunction(currState);
	int len = -1; 
	PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
	State * s = evaluatePositionState(possiblePositions);
	if (isMaximising) {
		long long maxScore = -1e9; // very low number. 
		for (int i = 0; i < len; i++) {
			maxScore = max(maxScore, minimax(dv, s[i], currDepth+1, !isMaximising)); // switch isMaximising.
		}
		assert(maxScore != -1e9);
		return maxScore;
	} else {
		long long  minScore = 1e9; // very high number. 
		for (int i =0; i < len; i++) {
			minScore = min(minScore, minimax(dv, s[i], currDepth+1, !isMaximising));
		}
		assert(minScore != 1e9);
		return minScore; 
	}
}