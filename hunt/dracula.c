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
#include <stdio.h>
// structure to store a snapshot of a game state. 
// Anything above 8 won't make the 1.5 second time limit. 
#define MAX_DEPTH 7 
#define INFINITY 1e9
#define NEGATIVE_INFINITY -1e9

#define DISTANCE_WEIGHTING 12
#define HEALTH_LOSS_WEIGHTING -15
#define MAX_DIST_IGNORE 100 // At what distance to ignore DISTANCE_WEIGHTING. Set it rlly high 
#define CASTLE_DRACULA_WEIGHTING 15 // You get health points. So weight it higher.

static inline int max(int a, int b) {
	return a > b ? a : b; 
}

static inline int min(int a, int b) {
	return a < b ? a : b; 
}

int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString);
PlaceId minimaxHelper(DraculaView rootView, int currDepth);

bool insideArray(PlaceId * arr, PlaceId element, int len) {
	for (int i = 0; i < len; i++) {
		if (arr[i] == element) {
			return true;
		}
	}
	return false;
}

void decideDraculaMove(DraculaView dv)
{
	if (DvGetRound(dv) == 0) {
		int max_pid = -1; 
		int maxScore = -1;
		for (int pid = 0; pid < NUM_REAL_PLACES; pid++) {
			PlaceId filter[8] = {GALWAY, DUBLIN, SWANSEA, PLYMOUTH, LONDON, LIVERPOOL, MANCHESTER, EDINBURGH};
			int score = 0;
			if (placeIdToType(pid) != SEA && insideArray(filter, pid, 8)==false) {
				for (int player = 0; player < NUM_PLAYERS-1; player++) {
					score += calculateHunterDistFromDrac(dv, player, 0, pid, 
								DvGetPlayerLocation(dv, player), true, true, true); 
				}
				if (score > maxScore) {
					maxScore = score;
					max_pid = pid; 
				}
			}
			printf("score %d\n", score);
		}
		printf("%d\n", max_pid);
		registerBestPlay((char *)placeIdToAbbrev(max_pid), "");
	} else {
		registerBestPlay((char *)placeIdToAbbrev(minimaxHelper(dv, 0)), "");
	}
}

// Takes a game state s, and evaluates it based on how good is it.
int evalFunction(DraculaView currView) {
	int score = 0; 
	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		int dist = calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
					DvGetPlayerLocation(currView, PLAYER_DRACULA), DvGetPlayerLocation(currView, i), 
					true, false, false);
		if (dist > MAX_DIST_IGNORE) {
			dist = MAX_DIST_IGNORE;
		}
		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, PLAYER_DRACULA), DvGetPlayerLocation(currView, i), 
				false, true, false);
		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, PLAYER_DRACULA), DvGetPlayerLocation(currView, i), 
				false, false, true);
		score+= DISTANCE_WEIGHTING * (dist/3);
	}
	score+= (GAME_START_BLOOD_POINTS- DvGetHealth(currView, PLAYER_DRACULA)) * HEALTH_LOSS_WEIGHTING;
	return score;
}

PlaceId minimaxHelper(DraculaView rootView, int currDepth) {
	// is maxmising set to true for Drac.
	bool isMaximising = true; 
	int len = -1; 
	PlaceId * possibleMoves = DvGetValidMoves(rootView, &len); 
	int max = INT_MIN; 
	int index = -1; 
	if (possibleMoves == NULL) return TELEPORT;
	for (int i = 0; i < len; i++) {
		char extension[8]; 
		strcpy(extension, "D"); 
		strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
		strcat(extension, "....");
		DraculaView newState = extendGameState(rootView, extension, 7);
		printf("\t========NEXT when moving to %s ========\n", placeIdToName(possibleMoves[i]));
		int score = minimax(newState, currDepth+1, !isMaximising, NULL);
		if (possibleMoves[i] == CASTLE_DRACULA) {
			score+= CASTLE_DRACULA_WEIGHTING;
		} 
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
		char extension[40];
		PlaceId * possibleMoves = DvGetValidMoves(currView, &len);
		for (int i = 0; i < len; i++) {
			strcpy(extension, prevString); 
			strcat(extension, "D");
			strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
			strcat(extension, "...."); // Lets ignore traps and vampires. Game engine will deal with this.
			DraculaView newState = extendGameState(currView, extension, 40);
			int score = minimax(newState, currDepth+1, !isMaximising, NULL); 
			if (possibleMoves[i] == CASTLE_DRACULA) {
				score+= CASTLE_DRACULA_WEIGHTING/currDepth; // The more moves you take to get to Castle Dracula, the smaller the increase.
			}
			if (score > maxScore) {
				maxScore = score;
			}
			DvFree(newState);
		}
		if (possibleMoves == NULL) {
			strcpy(extension, prevString);
			strcat(extension, "DTP....");
			printf("oh yeah, we're teleporting NOW!\n");
			DraculaView newState = extendGameState(currView, extension, 40);
			maxScore = minimax(newState, currDepth+1, !isMaximising, NULL);
			DvFree(newState);
		} else free(possibleMoves);
		return maxScore; 
	} else {
		PlaceId minScores[NUM_PLAYERS-1] = {NOWHERE, NOWHERE, NOWHERE, NOWHERE};
		int len = -1; 
		char extension[32];
		for (int player = 0; player < NUM_PLAYERS - 1; player++){
			PlaceId *possiblePositions = DvWhereCanTheyGoByType(currView, player, true, true, true, &len);
			int global_min = INT_MAX; 
			for (int currPosIndex =0; currPosIndex < len; currPosIndex++) {
				int curr_min = calculateHunterDistFromDrac(currView, player, DvGetRound(currView), 
								possiblePositions[currPosIndex], DvGetPlayerLocation(currView, PLAYER_DRACULA), 
								true, true, true);
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
		}
		int ret = minimax(currView, currDepth+1, !isMaximising, extension);
		return ret; 
	}
}
