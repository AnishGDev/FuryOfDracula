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
#include <stdio.h> // Remove later.
// structure to store a snapshot of a game state. 
// Anything above 8 won't make the 1.5 second time limit. 
#define MAX_DEPTH 8 // might increase depending on how long it takes to evaluate.
#define INFINITY 1e9
#define NEGATIVE_INFINITY -1e9

#define DISTANCE_WEIGHTING 10
#define HEALTH_LOSS_WEIGHTING -20
#define MAX_DIST_IGNORE 4 // At what distance to ignore DISTANCE_WEIGHTING
typedef struct _state {
	PlaceId whereIsPlayer[5]; // to keep track of which placeID. 5th one is dracula. 
	// used in evalFunction. 
	int health;
	int score;
	PlaceId * trail;// Store current trail info. 
	int distFromPlayers[NUM_PLAYERS];
	// store CD later.
	Round currRound;
} State; 

static inline int max(int a, int b) {
	return a > b ? a : b; 
}

static inline int min(int a, int b) {
	return a < b ? a : b; 
}

int minimax(DraculaView dv, State currState, int currDepth, bool isMaximising);
PlaceId minimaxHelper(DraculaView dv, State *rootState, int currDepth, bool isMaximising);
void decideDraculaMove(DraculaView dv)
{
	// TODO: Replace this with something better!
	// Set up initial root state
	int currScore = DvGetScore(dv);
	//currScore+=1;
	if (currScore == GAME_START_SCORE) {
		// Only first round. 
		registerBestPlay("CD", "Mwahahahaha");
	} else {
		State * s = malloc(sizeof(struct _state)); 
		s->whereIsPlayer[PLAYER_DRACULA] = DvGetPlayerLocation(dv, PLAYER_DRACULA);
		s->currRound = DvGetRound(dv);
		for (int i =0; i < NUM_PLAYERS - 1; i++) {
			s->whereIsPlayer[i] = DvGetPlayerLocation(dv, i); 
			s->distFromPlayers[i] = //calculateDistance(dv, s->whereIsPlayer[PLAYER_DRACULA], s->whereIsPlayer[i]); //calculateDistance(dv, from, to); 
									calculateHunterDistFromDrac(dv, i, s->currRound, s->whereIsPlayer[PLAYER_DRACULA], s->whereIsPlayer[i]);
		}					
		// 5th element will represent distance to castle dracula.
		//s->distFromPlayers[PLAYER_DRACULA] = calculateDistance(dv, s->whereIsPlayer[PLAYER_DRACULA], CASTLE_DRACULA); 
		s->health = DvGetHealth(dv, PLAYER_DRACULA);
		s->score = currScore;
		int numReturned = 0;
		bool canFree = false; 
		s->trail = returnCurrentTrail(dv, &numReturned, &canFree); 
		//printf("lol %d\n",minimaxHelper(dv, s, 0, true));
		printf("%s\n",placeIdToName(minimaxHelper(dv, s, 0, true)));
		registerBestPlay("CD", "Mwahahahaha");
	}
}

State updateTrailInfo(State *s) {
	return *s;
}

// Takes a game state s, and evaluates it based on how good is it.
int evalFunction(State s) {
	int score = 0; 
	for (int i =0; i < NUM_PLAYERS; i++) {
		//printf("Player %d is located at %s\n", i, placeIdToName(s.whereIsPlayer[i]));
	}

	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		//printf("Adding %d \n", s.distFromPlayers[i] * DISTANCE_WEIGHTING);
		if(s.distFromPlayers[i] > MAX_DIST_IGNORE) {
			score+= DISTANCE_WEIGHTING * MAX_DIST_IGNORE;
		} else {
			score+= s.distFromPlayers[i] * DISTANCE_WEIGHTING;
		}
	}
	//printf("AI rates this position %d\n", score);
	return score;
}
// If Dracula was at place p, what would his distance to other players be?
void evalDistFromPlayers(DraculaView dv, PlaceId p, State *s) {
	for (int i =0; i < NUM_PLAYERS - 1; i++) {
		//s->whereIsPlayer[i] = DvGetPlayerLocation(dv, i); 
		s->distFromPlayers[i] = //calculateDistance(dv, s->whereIsPlayer[PLAYER_DRACULA], s->whereIsPlayer[i]); //calculateDistance(dv, from, to); 
								calculateHunterDistFromDrac(dv, i, s->currRound, s->whereIsPlayer[PLAYER_DRACULA], s->whereIsPlayer[i]);
		//printf("We are %d away from player %d who is at %s and we are at %s\n", s->distFromPlayers[i], i, placeIdToName(s->whereIsPlayer[i]), placeIdToName(s->whereIsPlayer[PLAYER_DRACULA]));
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
	printf("len is %d\n", len);
	//State * s = evaluatePositionState(possiblePositions, PLAYER_DRACULA, rootState, len);
	int maxScore = INT_MIN; // Very smoll number. 
	PlaceId retValue = 0; 
	for (int i =0; i < len; i++) {
		State s = *rootState;
		// Update round.
		s.currRound+=1; 
		s.whereIsPlayer[PLAYER_DRACULA] = possiblePositions[i];
		setTheoreticalState(dv, s.trail, s.currRound, s.whereIsPlayer);
		evalDistFromPlayers(dv, possiblePositions[i], &s); 
		//maxScore = max(maxScore, minimax(dv, s, currDepth+1, !isMaximising)); // switch isMaximising.
		//printf("========NEXT when moving to %s ========\n", placeIdToName(possiblePositions[i]));
		int score = minimax(dv, s, currDepth+1, !isMaximising); // switch isMaximising.
		printf("Loc: %s\n with a score of %d\n========DONE========\n", placeIdToName(possiblePositions[i]), score);
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
	//int index = -1; // Remove later.
	if (isMaximising) {
		//printf(">>>> INSIDE DRACULA STATE <<<<\n");
		int maxScore = INT_MIN; // very low number. 
		PlaceId *possiblePositions = DvWhereCanIGo(dv, &len);
		//State * s = evaluatePositionState(possiblePositions, PLAYER_DRACULA, &currState, len);
		for (int i = 0; i < len; i++) {
			//printf("\t ==== Let us move to %s from %s====\n", placeIdToName(possiblePositions[i]), placeIdToName(currState.whereIsPlayer[PLAYER_DRACULA]));
			State s = currState; 
			//printf("Evaluating %s in Dracula\n", placeIdToName(possiblePositions[i]));
			s.currRound+=1;
			s.whereIsPlayer[PLAYER_DRACULA] = possiblePositions[i];
			setTheoreticalState(dv, currState.trail, currState.currRound, currState.whereIsPlayer);
			evalDistFromPlayers(dv, possiblePositions[i], &s); 
			int score = minimax(dv, s, currDepth+1, !isMaximising); 
			if (score > maxScore) {
				maxScore = score; 
				//index = i;
			}
			//maxScore = max(maxScore, minimax(dv, s, currDepth+1, !isMaximising)); // switch isMaximising.
		}
		//printf("Suggesting move to %s\n", placeIdToName(possiblePositions[index]));
		assert(maxScore != INT_MIN);
		//printf("Returning %d for Dracula\n", maxScore);
		//printf(">>>> END OF DRACULA STATE <<<<\n");
		return maxScore;
	} else {
		//printf(">>>> INSIDE HUNTER STATE <<<<\n");
		//int minScore = 1e9; // very high number.
		PlaceId minScores[NUM_PLAYERS-1] = {NOWHERE, NOWHERE, NOWHERE, NOWHERE}; 
		State newState = currState;
		//int score = INT_MAX;
		int losses = 0; 
		setTheoreticalState(dv, currState.trail, currState.currRound, currState.whereIsPlayer);
		for (int player =0; player < NUM_PLAYERS - 1; player++) {
			PlaceId *possiblePositions = DvWhereCanTheyGoByType(dv, player, true, true, true, &len);
			int global_min = INT_MAX; 
			for (int currPosIndex =0; currPosIndex < len; currPosIndex++) {
				//printf("Evaluating %s in HunterView for player %d\n", placeIdToName(possiblePositions[currPosIndex]), player);
				int curr_min = //calculateDistance(dv, possiblePositions[currPosIndex], currState.whereIsPlayer[PLAYER_DRACULA]);
				calculateHunterDistFromDrac(dv, player, newState.currRound+1,possiblePositions[currPosIndex], currState.whereIsPlayer[PLAYER_DRACULA]);
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
			//score = min(score, global_min);
			newState.whereIsPlayer[player] = minScores[player];
			//printf("\tMoving player %d to %s\n", player, placeIdToName(minScores[player]));
			if(minScores[player] == newState.whereIsPlayer[PLAYER_DRACULA]) {
				//printf("Health LOSS!!\n"); 
				losses -= HEALTH_LOSS_WEIGHTING;
			}
		}
		//printf("Minusing losses %d\n", losses);
		int score = minimax(dv, newState, currDepth+1, !isMaximising) - losses;
		//assert(minScore != 1e9);
		// Currently just returns the smallest distance between them
		//printf("Returning %d for hunters\n", score);
		//printf(">>>> END OF HUNTER STATE <<<<\n");
		return score; 
	}
}