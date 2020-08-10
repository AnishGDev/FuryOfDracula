////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// A "Fury of Dracula" Dracula AI for UNSW'S COMP2521 
////////////////////////////////////////////////////////////////////////

#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>

#define MAX_DEPTH 5 // Look ahead n
#define INFINITY 1e9
#define NEGATIVE_INFINITY -1e9

#define DISTANCE_WEIGHTING 25
#define HEALTH_LOSS_WEIGHTING -15
#define MAX_DIST_IGNORE 3// At what distance to ignore DISTANCE_WEIGHTING.
#define CASTLE_DRACULA_WEIGHTING 5 
#define TELEPORT_WEIGHTING -25
#define SEA_WEIGHTING -5
#define SCORE_WEIGHTING 2

#define TIEBREAK -1
#define TELEPORT_THRESHOLD 4 // n units away
#define AT_HUNTER_POS -150;

static inline int max(int a, int b) {
	return a > b ? a : b; 
}

static inline int min(int a, int b) {
	return a < b ? a : b; 
}

int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString, int *counter);
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
			int score = 1;
			if (placeIdToType(pid) != SEA && pid != HOSPITAL_PLACE && !insideArray(filter, pid, 8)) {
				for (int player = 0; player < NUM_PLAYERS-1; player++) {
					score += calculateHunterDistFromDrac(dv, player, 1, DvGetPlayerLocation(dv, player), 
								pid, true, true, true); 
				}
				if (score/4 > maxScore) {
					score/=4;
					maxScore = score;
					max_pid = pid; 
				}
			}
		}
		registerBestPlay((char *)placeIdToAbbrev(max_pid), "");
	} else {
		registerBestPlay((char *)placeIdToAbbrev(minimaxHelper(dv, 0)), "");
	}
}

// Takes a game state s, and evaluates it based on how good is it.
int evalFunction(DraculaView currView) {
	int score = 0; 
	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		/*
		Taking combinatorics of distances.

		int dist = calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
					DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
					true, false, false);

		
		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				false, true, false);

		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				false, false, true);
		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				true, true, false);
		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				true, false, true);
		dist+= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				false, true, true);
		*/
		int plus = calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				true, true, true);
		int dist = 0; 
		if (plus == 0){ //&& placeIsLand(DvGetPlayerLocation(currView, PLAYER_DRACULA))) {
			if (placeIsLand(DvGetPlayerLocation(currView, PLAYER_DRACULA))) {
				score+=AT_HUNTER_POS
			} else {
				dist = 1;
			}
		} else {
			dist+=plus;
		}

		if (dist > (MAX_DIST_IGNORE)) {
			dist = MAX_DIST_IGNORE;
		}	
		score+= DISTANCE_WEIGHTING * (dist);
	}
	score+= (GAME_START_BLOOD_POINTS- DvGetHealth(currView, PLAYER_DRACULA)) * HEALTH_LOSS_WEIGHTING;
	return score;
}

// Root state. Do a DFS search from the root state. 
PlaceId minimaxHelper(DraculaView rootView, int currDepth) {
	// is maxmising set to true for Drac.
	bool isMaximising = true; 
	int len = -1; 
	PlaceId * possibleMoves = DvGetValidMoves(rootView, &len); 
	int max = INT_MIN; 
	int index = 0; 
	if (possibleMoves == NULL) return TELEPORT;
	int counter = 0; 
	for (int i = 0; i < len; i++) {
		char extension[8]; 
		strcpy(extension, "D"); 
		strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
		strcat(extension, "....");
		DraculaView newState = extendGameState(rootView, extension, 7);
		//printf("\t========NEXT when moving to %s ========\n", placeIdToName(possibleMoves[i]));
		int score = minimax(newState, currDepth+1, !isMaximising, NULL, &counter) + evalFunction(newState);
		if (possibleMoves[i] == CASTLE_DRACULA) {
			score+= CASTLE_DRACULA_WEIGHTING;
		} 
		printf("poggers2\n");
		printf("Evaluated %s with a score of %d \n",placeIdToName(possibleMoves[i]), score);
		if (score > max) {
			max = score;
			index = i; 
		}
		counter+=1; 
		DvFree(newState);
	}
	PlaceId ret = possibleMoves[index];
	free(possibleMoves);
	return ret; 
}

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

// Create new substates and send them to evaluation function. 
int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString, int *counter) {
	if (currDepth >= MAX_DEPTH) return evalFunction(currView)/(currDepth);

	PlaceId minScores[NUM_PLAYERS-1] = {NOWHERE, NOWHERE, NOWHERE, NOWHERE};
	int len = -1; 
	char extension[40];
	int maxScore = INT_MIN; 
	for (int player = 0; player < NUM_PLAYERS; player++){
		if (player != PLAYER_DRACULA) {
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
				*counter+=1; 
			}
			free(possiblePositions);
			extension[player * 8] = playerIdToName(player);
			extension[player * 8 + 1] = '\0';
			strcat(extension, placeIdToAbbrev(minScores[player]));
			if (minScores[player] == DvGetPlayerLocation(currView, PLAYER_DRACULA) && placeIsLand(DvGetPlayerLocation(currView, PLAYER_DRACULA))) {
				strcat(extension, "D... ");
			} else {
				strcat(extension, ".... ");
			}
		} else {
			int pos = -1;
			PlaceId * possibleMoves = DvGetValidMoves(currView, &pos);
			
			for (int i = 0; i < pos; i++) {
				extension[32] = '\0';
				strcat(extension, "D");
				strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
				strcat(extension, "...."); // Lets ignore traps and vampires. Game engine will deal with this.
				DraculaView newState = extendGameState(currView, extension, 40);
				if (DvGetHealth(newState, PLAYER_DRACULA) <= 0) return -10000/(currDepth+1);
				int score = minimax(newState, currDepth+1, !isMaximising, NULL, counter);
				if (currDepth != MAX_DEPTH-1) score+= evalFunction(newState)/(currDepth); 
				if (possibleMoves[i] == CASTLE_DRACULA) {
					score+= CASTLE_DRACULA_WEIGHTING/(currDepth+1); // The more moves you take to get to Castle Dracula, the smaller the increase.
				} 
				if (placeIdToType(possibleMoves[i]) == SEA) {
					score+= SEA_WEIGHTING/(currDepth+1); 
				}
				if (possibleMoves[i] >= HIDE && possibleMoves[i] <= DOUBLE_BACK_5) {
					score+=TIEBREAK;
				}
				if (score > maxScore) {
					maxScore = score; 
				}
				*counter+=1; 
				DvFree(newState);
			}

			if (possibleMoves == NULL) {
				extension[32] = '\0';
				int minDist = INT_MAX; 
				for (int i = 0; i < NUM_PLAYERS - 1; i++) {
					minDist = min(calculateHunterDistFromDrac(currView, i, DvGetRound(currView)+1, 
						DvGetPlayerLocation(currView, i), CASTLE_DRACULA, 
						true, true, true), minDist);
				}
				if (minDist < (TELEPORT_THRESHOLD)) {
					return -5000;
				}
				strcat(extension, "DTP....");
				DraculaView newState = extendGameState(currView, extension, 40);
				maxScore = minimax(newState, currDepth+1, !isMaximising, NULL, counter);
				maxScore += TELEPORT_WEIGHTING;
				DvFree(newState);
			} else free(possibleMoves);
		}
	}
	return maxScore;  
}
