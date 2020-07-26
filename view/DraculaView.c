////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
// add your own #includes here

static bool hiddenInLast5 (DraculaView dv);
static bool DoubleInLast5 (DraculaView dv);

// TODO: ADD YOUR OWN STRUCTS HERE

struct draculaView {
	GameView gv;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
	DraculaView new = malloc(sizeof(DraculaView));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}
	new->gv = GvNew(pastPlays,messages);
	return new;
}

void DvFree(DraculaView dv)
{
	GvFree(dv->gv);
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv)
{
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player)
{
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	return GvGetPlayerLocation(dv->gv, player);
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	if (numTraps == NULL) { 	//CAN THIS HAPPEN????
		numTraps = malloc(sizeof(int));
	}
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	//Checking if dracula has made a move
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE) {
		*numReturnedMoves = 0;
		return NULL;
	}
	*numReturnedMoves = 0;
	PlaceId *places = GvGetReachable(dv->gv, PLAYER_DRACULA,
		DvGetRound(dv), DvGetPlayerLocation(dv, PLAYER_DRACULA)
		,numReturnedMoves); 	//dont have to check for trains and hospital
							// since GvGetReachable takes that into account
	
	//Remove Items which Dracula cannot visit
	//Checking if only move is teleport
	if (*numReturnedMoves == 0) return NULL;

	//Check move history to see if these have occured
	bool hiddenInTrail = hiddenInLast5(dv);
	bool doubleBackInTrail = DoubleInLast5(dv);

	if (!hiddenInTrail) {	//Add Hide as a move
		*numReturnedMoves += 1;
		places = realloc(places, sizeof(PlaceId) * (*numReturnedMoves));
		places[*numReturnedMoves-1] = HIDE;
	}

	int numHistMoves;
	bool canFree = false;
	int numMoves = 5;
	PlaceId *trailMoves = GvGetLastLocations(dv->gv, PLAYER_DRACULA, 
		numMoves, &numHistMoves, &canFree);

	if (!doubleBackInTrail) {	//Add Double backs as moves
		for (int i = 0; i < numHistMoves; i++) {	//Go through places and see if they occur in
														//Dracula's trail, if they do, replace them with double_back_n
			 for (int j = 0; j < *numReturnedMoves; j++) {
				 if (trailMoves[i] == places[j]) {
					 places[j] = DOUBLE_BACK_5 - i; //Move to somewere 5 moves ago - how many moves before it was
				 }
			 }
		}
	} else {	//Double Back in trail remove locations in last 5 moves
		PlaceId remove = -3;
		for (int i = 0; i < numHistMoves; i++) {	//Go through places and see if they occur in
													//Dracula's trail, if they do, remove them
			for (int j = 0; j < *numReturnedMoves; j++) {
				if (trailMoves[i] == places[j]) {
					places[j] = remove; //Move to somewere 5 moves ago - how many moves before it was
				}
			}
		}
		//Rearrange the array
		int numShift = 0;
		for (int i = 0; i < *numReturnedMoves - numShift; i++) {
			if (places[i] == remove) {
				numShift++;
				places[i] = places[i+numShift];
			}
		}
		places = realloc(places, sizeof(PlaceId) * (*numReturnedMoves - numShift));
	}

	//Adding Teleport as a move
	*numReturnedMoves += 1;
	places = realloc(places, sizeof(PlaceId) * (*numReturnedMoves));
	places[*numReturnedMoves-1] = TELEPORT;

	if (canFree) free(trailMoves);
	return places;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	//Checking if dracula has made a move
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}

	*numReturnedLocs = 0;
	PlaceId *places = GvGetReachable(dv->gv, PLAYER_DRACULA,
		DvGetRound(dv), DvGetPlayerLocation(dv, PLAYER_DRACULA),
		numReturnedLocs); 	//dont have to check for trains and hospital
							//since GvGetReachable takes that into account
	
	//Checking if only move is teleport
	if (*numReturnedLocs == 0) return NULL;
	//Remove Items which Dracula cannot visit

	//Check move history to see if can doubleback
	bool doubleBackInTrail = DoubleInLast5(dv);

	int numHistMoves;
	bool canFree = false;
	int numMoves = 5;
	PlaceId *trailMoves = GvGetLastLocations(dv->gv, PLAYER_DRACULA, 
		numMoves, &numHistMoves, &canFree);

	if (!doubleBackInTrail) {	//Add Double backs as moves
		for (int i = 0; i < numHistMoves; i++) {	//Go through places and see if they occur in
													//Dracula's trail, if they do, replace them with double_back_n
			 for (int j = 0; j < *numReturnedLocs; j++) {
				 if (trailMoves[i] == places[j]) {
					 places[j] = DOUBLE_BACK_5 - i; //Move to somewere 5 moves ago - how many moves before it was
				 }
			 }
		}
	} else {	//Double Back in trail remove locations visited in last 5 moves
		PlaceId remove = -3;
		for (int i = 0; i < numHistMoves; i++) {	//Go through places and see if they occur in
													//Dracula's trail, if they do, remove them
			for (int j = 0; j < *numReturnedLocs; j++) {
				if (trailMoves[i] == places[j]) {
					places[j] = remove; //Move to somewere 5 moves ago - how many moves before it was
				}
			}
		}
		//Rearrange the array
		int numShift = 0;
		for (int i = 0; i < *numReturnedLocs - numShift; i++) {
			if (places[i] == remove) {
				numShift++;
				places[i] = places[i+numShift];
			}
		}
		places = realloc(places, sizeof(PlaceId) * (*numReturnedLocs - numShift));
	}

	if (canFree) free(trailMoves);
	return places;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	//Checking if dracula has made a move
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}

	PlaceId *places = GvGetReachableByType(dv->gv, PLAYER_DRACULA,
		DvGetRound(dv), DvGetPlayerLocation(dv, PLAYER_DRACULA),
		road, false, boat, numReturnedLocs);	//dont have to check for trains and hospital
												// since GvGetReachable takes that into account

	//Checking if only move is teleport
	if (*numReturnedLocs == 0) {
		*numReturnedLocs = 0;
		return NULL;
	}

	//Remove Items which Dracula cannot visit 

	//Check move history to see if doubleback has occured
	bool doubleBackInTrail = DoubleInLast5(dv);

	if (doubleBackInTrail) {
		for (int i = 0; i < *numReturnedLocs; i++) {
			if (places[i] >= DOUBLE_BACK_1 && places[i] <= DOUBLE_BACK_5) {	//found index of a doubleBack
				for (int j = i; j < *numReturnedLocs; j++) {	//Since double back can only occur once, shift the array
					places[j] = places[j+1];
				}
				*numReturnedLocs -= 1;
				break;
			}
		}
	}
	return places;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	if (GvGetPlayerLocation(dv->gv, player) == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}

	*numReturnedLocs = 0;
	if (player == PLAYER_DRACULA) {
		return DvWhereCanIGo(dv, numReturnedLocs); 
	} else {
		return GvGetReachable(dv->gv, player, DvGetRound(dv), DvGetPlayerLocation(dv, player),
			numReturnedLocs);
	}
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	if (GvGetPlayerLocation(dv->gv, player) == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}
	if (player == PLAYER_DRACULA) {
		return DvWhereCanIGoByType(dv, road, boat, numReturnedLocs);
	} else {
		return GvGetReachableByType(dv->gv, player, DvGetRound(dv),
									DvGetPlayerLocation(dv, player), road,
									rail, boat, numReturnedLocs);
	}
}

//Checks if dracula has used a Hide in the last 5 moves
static bool hiddenInLast5 (DraculaView dv) {
	bool canFree = false;
	bool found = false;
	int numReturnedMoves;

	PlaceId *moveHist = GvGetLastMoves (dv->gv, PLAYER_DRACULA, 
		5, &numReturnedMoves, &canFree);

	for (int i = 0; i < numReturnedMoves; i++) {
		if (moveHist[i] == HIDE) {
			found = true;
			break;
		}
	}
	if (canFree) free(moveHist);
	return found;
}

//Checks if dracula has used a doubleback in the last 5 moves
static bool DoubleInLast5 (DraculaView dv) {

	bool canfree = false;
	bool found = false;
	int numReturnedMoves;
	PlaceId *moveHist = GvGetLastMoves (dv->gv, PLAYER_DRACULA, 
		5, &numReturnedMoves, &canfree);

	for (int i = 0; i < numReturnedMoves; i++) {
		//IF move is DOUBLE_BACK_1 or 2 or 3 or 4 or 5
		if (moveHist[i] >= DOUBLE_BACK_1 && moveHist[i] <= DOUBLE_BACK_5) {
			found = true;
			break;
		}
	}

	if (canfree) free(moveHist);
	return found;
}
////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
