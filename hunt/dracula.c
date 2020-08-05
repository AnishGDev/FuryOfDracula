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
#define MAX_DEPTH 5 // Look ahead n
#define INFINITY 1e9
#define NEGATIVE_INFINITY -1e9

#define DISTANCE_WEIGHTING 7
#define HEALTH_LOSS_WEIGHTING -15
#define MAX_DIST_IGNORE DISTANCE_WEIGHTING * 10 // At what distance to ignore DISTANCE_WEIGHTING. Set it rlly high (100 * 7) 7 combinations
#define CASTLE_DRACULA_WEIGHTING 5 // You get health points. So weight it higher.
#define TELEPORT_WEIGHTING -5
#define SEA_WEIGHTING -9
#define SCORE_WEIGHTING 2

#define TELEPORT_THRESHOLD 2 // n units away

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
			if (placeIdToType(pid) != SEA && pid != HOSPITAL_PLACE && insideArray(filter, pid, 8)==false) {
				for (int player = 0; player < NUM_PLAYERS-1; player++) {
					//printf("Distance from %s to %s is %d\n", placeIdToName(DvGetPlayerLocation(dv, player)),placeIdToName(pid), calculateHunterDistFromDrac(dv, player, 0, DvGetPlayerLocation(dv, player), pid, true, true, true));
					score += calculateHunterDistFromDrac(dv, player, 0, DvGetPlayerLocation(dv, player), 
								pid, true, true, true); 
				}
				if (score > maxScore) {
					maxScore = score;
					max_pid = pid; 
				}
			}
			//printf("%s: %d\n",placeIdToName(pid), score);
		}
		//printf("%d\n", max_pid);
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
		int dist= calculateHunterDistFromDrac(currView, i, DvGetRound(currView), 
				DvGetPlayerLocation(currView, i), DvGetPlayerLocation(currView, PLAYER_DRACULA), 
				true, true, true);

		if (dist > (MAX_DIST_IGNORE)) {
			//printf("%d was bigger", dist);
			dist = MAX_DIST_IGNORE;
		}	
		score+= DISTANCE_WEIGHTING * (dist);
	}
	score+= (GAME_START_SCORE - DvGetScore(currView)) * SCORE_WEIGHTING;
	score+= (GAME_START_BLOOD_POINTS- DvGetHealth(currView, PLAYER_DRACULA)) * HEALTH_LOSS_WEIGHTING;
	return score;
}

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
		int score = minimax(newState, currDepth+1, !isMaximising, NULL, &counter);
		if (possibleMoves[i] == CASTLE_DRACULA) {
			score+= CASTLE_DRACULA_WEIGHTING;
		} 
		printf("Evaluated %s with a score of %d \n",placeIdToName(possibleMoves[i]), score);
		if (score > max) {
			max = score;
			index = i; 
		}
		counter+=1; 
		DvFree(newState);
	}
	//printf("We did %d calculations \n", counter);
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

		/*
		printf("CURR DEPTH : %d \nPossible moves from %s : ",currDepth, placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)));
		for (int i = 0; i < len; i++) {
			printf("%s ", placeIdToName(possibleMoves[i]));
		}
		printf("\n");
		*/

/*
int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString, int *counter) {
	if (currDepth >= MAX_DEPTH) return evalFunction(currView);
	if (isMaximising) {
		int maxScore = INT_MIN; 
		int len = -1;
		char extension[40];
		//int index = -1; 
		PlaceId * possibleMoves = DvGetValidMoves(currView, &len);

		for (int i = 0; i < len; i++) {
			strcpy(extension, prevString); 
			strcat(extension, "D");
			strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
			strcat(extension, "...."); // Lets ignore traps and vampires. Game engine will deal with this.
			DraculaView newState = extendGameState(currView, extension, 40);
			int score = minimax(newState, currDepth+1, !isMaximising, NULL, counter); 
			if (possibleMoves[i] == CASTLE_DRACULA) {
				score+= CASTLE_DRACULA_WEIGHTING/currDepth; // The more moves you take to get to Castle Dracula, the smaller the increase.
			} 
			if (placeIdToType(possibleMoves[i])) {
				//printf("Its the sea!\n");
				score+= SEA_WEIGHTING; 
			}
			if (score > maxScore) {
				maxScore = score;
				//index = i; 
			}
			*counter+=1; 
			DvFree(newState);
		}
		//printf("Returning %d for %s from %s currDepth: %d\n", maxScore, placeIdToName(possibleMoves[index]), placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)), currDepth);
		if (possibleMoves == NULL) {
			strcpy(extension, prevString);
			strcat(extension, "DTP....");
			DraculaView newState = extendGameState(currView, extension, 40);
			maxScore = minimax(newState, currDepth+1, !isMaximising, NULL, counter);
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
				*counter+=1; 
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
		int ret = minimax(currView, currDepth+1, !isMaximising, extension, counter);
		return ret; 
	}
}
*/

int minimax(DraculaView currView, int currDepth, bool isMaximising, char * prevString, int *counter) {
	if (currDepth >= MAX_DEPTH) return evalFunction(currView);

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
			if (minScores[player] == DvGetPlayerLocation(currView, PLAYER_DRACULA)) {
				strcat(extension, "D... ");
			} else {
				strcat(extension, ".... ");
			}
		} else {
			//int maxScore = INT_MIN; 
			int pos = -1;
			//char extension[40];
			//int index = -1; 
			PlaceId * possibleMoves = DvGetValidMoves(currView, &pos);
				
			//printf("CURR DEPTH : %d \nPossible moves from %s : ",currDepth, placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)));
			
			for (int i = 0; i < pos; i++) {
				//strcpy(extension, prevString); 
				extension[32] = '\0';
				strcat(extension, "D");
				strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
				strcat(extension, "...."); // Lets ignore traps and vampires. Game engine will deal with this.
				DraculaView newState = extendGameState(currView, extension, 40);
				//printf("Health is %d\n", DvGetHealth(newState, PLAYER_DRACULA));
				if (DvGetHealth(newState, PLAYER_DRACULA) <= 0) return -10000;
				int score = minimax(newState, currDepth+1, !isMaximising, NULL, counter); 
				if (possibleMoves[i] == CASTLE_DRACULA) {
					score+= CASTLE_DRACULA_WEIGHTING/(currDepth+1); // The more moves you take to get to Castle Dracula, the smaller the increase.
				} 
				if (placeIdToType(possibleMoves[i]) == SEA) {
					//printf("Its the sea!\n");
					score+= SEA_WEIGHTING/(currDepth+1); 
				}
				if (score > maxScore) {
					maxScore = score;
					//index = i; 
				}
				*counter+=1; 
				DvFree(newState);
			}
			//printf("Returning %d for %s from %s currDepth: %d\n", maxScore, placeIdToName(possibleMoves[index]), placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)), currDepth);
			if (possibleMoves == NULL) {
				//strcpy(extension, prevString);
				extension[32] = '\0';
				int minDist = INT_MAX; 
				for (int i = 0; i < NUM_PLAYERS - 1; i++) {
					printf("Dist is %d\n", calculateHunterDistFromDrac(currView, i, DvGetRound(currView)+1, 
						DvGetPlayerLocation(currView, i), CASTLE_DRACULA, true, true, true));
					minDist = min(calculateHunterDistFromDrac(currView, i, DvGetRound(currView)+1, 
						DvGetPlayerLocation(currView, i), CASTLE_DRACULA, 
						true, true, true), minDist);
				}
				printf("OKAY Teleport threshold is %d versus %d\n", minDist, TELEPORT_THRESHOLD);
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
	/*
	int maxScore = INT_MIN; 
	int pos = -1;
	//char extension[40];
	//int index = -1; 
	PlaceId * possibleMoves = DvGetValidMoves(currView, &pos);
		
	//printf("CURR DEPTH : %d \nPossible moves from %s : ",currDepth, placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)));
	
	for (int i = 0; i < pos; i++) {
		//strcpy(extension, prevString); 
		extension[32] = '\0';
		strcat(extension, "D");
		strcat(extension, placeIdToAbbrev(possibleMoves[i])); 
		strcat(extension, "...."); // Lets ignore traps and vampires. Game engine will deal with this.
		DraculaView newState = extendGameState(currView, extension, 40);
		int score = minimax(newState, currDepth+1, !isMaximising, NULL, counter); 
		if (possibleMoves[i] == CASTLE_DRACULA) {
			score+= CASTLE_DRACULA_WEIGHTING/currDepth; // The more moves you take to get to Castle Dracula, the smaller the increase.
		} 
		if (placeIdToType(possibleMoves[i]) == SEA) {
			//printf("Its the sea!\n");
			score+= SEA_WEIGHTING; 
		}
		if (score > maxScore) {
			maxScore = score;
			//index = i; 
		}
		*counter+=1; 
		DvFree(newState);
	}
	//printf("Returning %d for %s from %s currDepth: %d\n", maxScore, placeIdToName(possibleMoves[index]), placeIdToName(DvGetPlayerLocation(currView, PLAYER_DRACULA)), currDepth);
	if (possibleMoves == NULL) {
		//strcpy(extension, prevString);
		extension[32] = '\0';
		strcat(extension, "DTP....");
		DraculaView newState = extendGameState(currView, extension, 40);
		maxScore = minimax(newState, currDepth+1, !isMaximising, NULL, counter);
		DvFree(newState);
	} else free(possibleMoves);
	return maxScore;  
	*/
}
