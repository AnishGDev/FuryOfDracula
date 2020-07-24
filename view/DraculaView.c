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
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	if (numTraps == NULL) { 	//CAN THIS HAPPEN????
		numTraps = malloc(sizeof(int));
	}
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	return NULL;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	PlaceId *places = GvGetReachable(dv->gv, PLAYER_DRACULA,
		DvGetRound(dv->gv), DvGetPlayerLocation(dv, PLAYER_DRACULA),
		numReturnedLocs); 	//dont have to check for trains and hospital
							// since GvGetReachable takes that into account
							//Should also give teleport (TELEPORT) move
	
	//Remove Items which Dracula cannot visit 

	//Check move history to see if these have occured
	bool hiddenInTrail = hiddenInLast5(dv);
	bool doubleBackInTrail = DoubleInLast5(dv);

	if (hiddenInTrail) { //Remove the hide move for dracula
		for (int i = 0; i < *numReturnedLocs; i++) {
			if (places[i] == HIDE) {	//found index of Hide
				for (int j = i; j < *numReturnedLocs; j++) {
					places[j] = places[j+1];
				}
				*numReturnedLocs--;
				break;
			}
		}
	}

	if (doubleBackInTrail) {
		for (int i = 0; i < *numReturnedLocs; i++) {
			if (places[i] >= DOUBLE_BACK_1 && places[i] <= DOUBLE_BACK_5) {	//found index of a doubleBack
				for (int j = i; j < *numReturnedLocs; j++) {
					places[j] = places[j+1];
				}
				*numReturnedLocs--;
				break;
			}
		}
	}

	return places;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	PlaceId *places = GvGetReachableByType(dv->gv, PLAYER_DRACULA,
		DvGetRound(dv->gv), DvGetPlayerLocation(dv->gv, PLAYER_DRACULA),
		road, false, boat, numReturnedLocs);
	//Remove items which break dracula's rules
	//Add special move items for dracula
	return NULL;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	//Should be the same as HvWhereCanIGo
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	//Should be the same as HvWhereCanIGoByType
	*numReturnedLocs = 0;
	return NULL;
}

//Checks if dracula has used a Hide in the last 5 moves
static bool hiddenInLast5 (DraculaView dv) {
	int numReturnedMoves;
	PlaceId *moveHist = GvGetMoveHistory (dv->gv, PLAYER_DRACULA, 
		&numReturnedMoves, false);

	int startingIndex = (numReturnedMoves > 5) ? numReturnedMoves - 5:0;

	for (int i = startingIndex; i < numReturnedMoves; i++) {
		if (moveHist[i] == HIDE) return true;
	}
	return false;
}

//Checks if dracula has used a doubleback in the last 5 moves
static bool DoubleInLast5 (DraculaView dv) {
	int numReturnedMoves;
	PlaceId *moveHist = GvGetMoveHistory (dv->gv, PLAYER_DRACULA, 
		&numReturnedMoves, false);

	int startingIndex = (numReturnedMoves > 5) ? numReturnedMoves - 5:0;

	for (int i = startingIndex; i < numReturnedMoves; i++) {
		//IF move is DOUBLE_BACK_1 or 2 or 3 or 4 or 5
		if (moveHist[i] >= DOUBLE_BACK_1 && moveHist[i] <= DOUBLE_BACK_5) return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
