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
#include <string.h>
#include <assert.h> // for testing purposes. Remove later.
#include <stdio.h> // Remove later.
// structure to store a snapshot of a game state. 
// Anything above 8 won't make the 1.5 second time limit. 
#define MAX_DEPTH 7 // might increase depending on how long it takes to evaluate.
#define INFINITY 1e9
#define NEGATIVE_INFINITY -1e9

#define DISTANCE_WEIGHTING 8
#define HEALTH_LOSS_WEIGHTING -20
#define MAX_DIST_IGNORE 100 // At what distance to ignore DISTANCE_WEIGHTING

static inline int max(int a, int b) {
	return a > b ? a : b; 
}

static inline int min(int a, int b) {
	return a < b ? a : b; 
}

int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString);
PlaceId minimaxHelper(DraculaView rootView, int currDepth);
void decideDraculaMove(DraculaView dv)
{
	//printf("\nSELECTED MOVE: %s\n", placeIdToName(minimaxHelper(dv, 0)));
	registerBestPlay((char *)placeIdToAbbrev(minimaxHelper(dv, 0)), "");
}

// Takes a game state s, and evaluates it based on how good is it.
int evalFunction(DraculaView currView) {
	int score = 0; 
	//printf("Evaluating at %s ", placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)));
	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		int dist = calculateHunterDistFromDrac(currView, i, DvGetRound(currView), DvGetPlayerLocation(currView, PLAYER_DRACULA), DvGetPlayerLocation(currView, i));
		if (dist > MAX_DIST_IGNORE) {
			dist = MAX_DIST_IGNORE;
		}
		score+= DISTANCE_WEIGHTING * dist;
	}
	//printf("HEALTH IS %d\n", DvGetHealth(currView, PLAYER_DRACULA));
	score+= (GAME_START_BLOOD_POINTS- DvGetHealth(currView, PLAYER_DRACULA)) * HEALTH_LOSS_WEIGHTING;
	//printf(" SCORE: %d\n",score);
	return score;
}
PlaceId minimaxHelper(DraculaView rootView, int currDepth) {
	// is maxmising set to true for Drac.
	bool isMaximising = true; 
	int len = -1; 
	PlaceId * possibleMoves = DvGetValidMoves(rootView, &len); 
	int max = INT_MIN; 
	int index = -1; 
	for (int i = 0; i < len; i++) {
		char extension[8]; 
		strcpy(extension, "D"); 
		strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
		strcat(extension, "....");
		DraculaView newState = extendGameState(rootView, extension, 7);
		printf("\t========NEXT when moving to %s ========\n", placeIdToName(possibleMoves[i]));
		int score = minimax(newState, currDepth+1, !isMaximising, NULL); 
		printf("Evaluated %s with a score of %d \n",placeIdToName(possibleMoves[i]), score);
		if (score > max) {
			max = score;
			index = i; 
		}
		DvFree(newState);
	}
	PlaceId ret = possibleMoves[index];
	free(possibleMoves);
	return ret; 
}
// Evaluate each substate and return a score.
// Need to support Hunter States as well. 
// Perhaps only consider distance for now? Hunter's goal is to minimize distance. 
char playerIdToName(int id) {
	if (id == 0) {
		return 'G'; 
	} else if (id == 1) {
		return 'S';
	} else if (id == 2) {
		return 'H';
	} else if (id == 3) {
		return 'M';
	} else {
		return '.';
	}
}

int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString) {
	if (currDepth >= MAX_DEPTH) return evalFunction(currView);
	if (isMaximising) {
		int maxScore = INT_MIN; 
		int len = -1;
		PlaceId * possibleMoves = DvGetValidMoves(currView, &len);
		if (possibleMoves == NULL) {
			//printf("OH SHIT WE HIT TELEPORT!!");
		}
		// Test for teleport as well ^^
		int index = -1; 
		for (int i = 0; i < len; i++) {
			char extension[40];
			strcpy(extension, prevString); 
			strcat(extension, "D");
			strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
			strcat(extension, "...."); // Lets ignore traps and vampires. Game engine will deal with this.
			//printf("Testing on %s@\nlol\n", extension);
			DraculaView newState = extendGameState(currView, extension, 40);
			//printf("\tmoving dracula to %s with DEPTH: %d\n", placeIdToName(possibleMoves[i]), currDepth);
			int score = minimax(newState, currDepth+1, !isMaximising, NULL); 
			if (score > maxScore) {
				maxScore = score;
				index = i; 
			}
			DvFree(newState);
		}
		printf("%s is good depth: %d\n", placeIdToName(possibleMoves[index]), currDepth);
		free(possibleMoves);
		//printf("====");
		return maxScore; 
	} else {
		//int minScore = 1e9; 
		PlaceId minScores[NUM_PLAYERS-1] = {NOWHERE, NOWHERE, NOWHERE, NOWHERE};
		int len = -1; 
		char extension[32];
		for (int player = 0; player < NUM_PLAYERS - 1; player++){
			PlaceId *possiblePositions = DvWhereCanTheyGoByType(currView, player, true, true, true, &len);
			int global_min = INT_MAX; 
			for (int currPosIndex =0; currPosIndex < len; currPosIndex++) {
				// MIGHT HAVE TO CHANGE ROUND IN CALCHUNTERFROMDIST
				int curr_min = //calculateDistance(dv, possiblePositions[currPosIndex], currState.whereIsPlayer[PLAYER_DRACULA]);
							calculateHunterDistFromDrac(currView, player, DvGetRound(currView), possiblePositions[currPosIndex], DvGetPlayerLocation(currView, PLAYER_DRACULA));
				if (curr_min < global_min) {
					global_min = curr_min;
					minScores[player] = possiblePositions[currPosIndex];
				}
			}
			free(possiblePositions);
			extension[player * 8] = playerIdToName(player);
			extension[player * 8 + 1] = '\0';
			strcat(extension, placeIdToAbbrev(minScores[player]));
			if (minScores[player] == DvGetPlayerLocation(currView, PLAYER_DRACULA)) {
				strcat(extension, "D... ");
			} else {
				strcat(extension, ".... ");
			}
			//score = min(score, global_min);
			//printf("\tMoving player %d to %s DEPTH: %d\n", player, placeIdToName(minScores[player]), currDepth);
			/*
			if(minScores[player] == newState.whereIsPlayer[PLAYER_DRACULA]) {
				//printf("Health LOSS!!\n"); 
				losses -= HEALTH_LOSS_WEIGHTING;
			}
			*/
		}
		int ret = minimax(currView, currDepth+1, !isMaximising, extension);
		return ret; 
	}
}
