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

static bool hiddenInLast5(DraculaView dv);
static bool DoubleInLast5(DraculaView dv);
static PlaceId *ReplaceWithDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
);
static PlaceId *RemoveDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
);

struct draculaView {
	GameView gv;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[]) {
	DraculaView new = malloc(sizeof(DraculaView));

	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays,messages);
	return new;
}

void DvFree(DraculaView dv) {
	GvFree(dv->gv);
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv) {
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv) {
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player) {
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player) {
	return GvGetPlayerLocation(dv->gv, player);
}

PlaceId DvGetVampireLocation(DraculaView dv) {
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps) {
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves) {
	// Check if dracula has made a move
	if (GvGetPlayerLocation(dv->gv, PLAYER_DRACULA) == NOWHERE) {
		*numReturnedMoves = 0;
		return NULL;
	}

	PlaceId *moves = DvWhereCanIGoByType(dv, true, true, numReturnedMoves);

	bool doubleBackInTrail = DoubleInLast5(dv);

	int numHistMoves;
	bool canFree = false;
	int numMoves = 5;
	PlaceId *trailMoves = GvGetLastLocations(
		dv->gv, PLAYER_DRACULA, numMoves, &numHistMoves, &canFree
	);

	if (!doubleBackInTrail) { // Add Double backs as moves
		moves = ReplaceWithDoubleBack(moves, trailMoves, numHistMoves, numReturnedMoves);
	} else {
		RemoveDoubleBack(moves, trailMoves, numHistMoves, numReturnedMoves);
	}

	bool hiddenInTrail = hiddenInLast5(dv);

	if (placeIdToType(DvGetPlayerLocation(dv, PLAYER_DRACULA)) != SEA) { // Can't Hide at sea
		if (!hiddenInTrail) { // Add Hide as a move
			*numReturnedMoves += 1;
			moves = realloc(moves, sizeof(PlaceId) * (*numReturnedMoves));
			moves[*numReturnedMoves-1] = HIDE;
		}
	}

	return moves;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs) {
	return DvWhereCanIGoByType(dv, true, true, numReturnedLocs);
}

PlaceId *DvWhereCanIGoByType(
	DraculaView dv, bool road, bool boat, int *numReturnedLocs
) {
	return DvWhereCanTheyGoByType(
		dv, PLAYER_DRACULA, road, false, boat, numReturnedLocs
	);
}

PlaceId *DvWhereCanTheyGo(
	DraculaView dv, Player player, int *numReturnedLocs
) {
	if (player == PLAYER_DRACULA) {
		return DvWhereCanTheyGoByType(
			dv, PLAYER_DRACULA, true, false, true, numReturnedLocs
		);
	} else {
		return DvWhereCanTheyGoByType(
			dv, player, true, true, true, numReturnedLocs
		);
	}
}

PlaceId *DvWhereCanTheyGoByType(
	DraculaView dv, Player player,
    bool road, bool rail, bool boat,
    int *numReturnedLocs
) {
	// Check if player has made a move
	if (GvGetPlayerLocation(dv->gv, player) == NOWHERE) {
		*numReturnedLocs = 0;
		return NULL;
	}

	if (player == PLAYER_DRACULA) {
		PlaceId *places = GvGetReachableByType(
			dv->gv, player, DvGetRound(dv),
			DvGetPlayerLocation(dv, player),
			road, rail, boat,
			numReturnedLocs
		);
		
		// Account for trail restrictions
		bool doubleBackInTrail = DoubleInLast5(dv);

		int numHistMoves;
		bool canFree = false;
		int numMoves = 5;

		PlaceId *trailMoves = GvGetLastLocations(
			dv->gv, PLAYER_DRACULA, numMoves, &numHistMoves, &canFree
		);

		if (doubleBackInTrail) { // Remove Double Back locations
			places = RemoveDoubleBack(places, trailMoves, numHistMoves, numReturnedLocs);
			
			// If stil can hide, add current location back
			if (!hiddenInLast5(dv)) {
				*numReturnedLocs += 1;
				places = realloc(places, sizeof(PlaceId) * (*numReturnedLocs));
				places[*numReturnedLocs-1] = DvGetPlayerLocation(dv, PLAYER_DRACULA);
			}
		}

		if (canFree) free(trailMoves);
		return places;
	} else {
		return GvGetReachableByType(
			dv->gv, player, DvGetRound(dv),
			DvGetPlayerLocation(dv, player),
			road, rail, boat,
			numReturnedLocs
		);
	}
}

// Checks if dracula has used a Hide in the last 5 moves
static bool hiddenInLast5(DraculaView dv) {
	bool canFree = false;
	bool found = false;
	int numReturnedMoves;

	PlaceId *moveHist = GvGetLastMoves(
		dv->gv, PLAYER_DRACULA, 5, &numReturnedMoves, &canFree
	);

	for (int i = 0; i < numReturnedMoves; i++) {
		if (moveHist[i] == HIDE) {
			found = true;
			break;
		}
	}

	if (canFree) free(moveHist);
	return found;
}

// Checks if dracula has used a Double Back in the last 5 moves
static bool DoubleInLast5(DraculaView dv) {
	bool canfree = false;
	bool found = false;
	int numReturnedMoves;

	PlaceId *moveHist = GvGetLastMoves(
		dv->gv, PLAYER_DRACULA, 5, &numReturnedMoves, &canfree
	);
	
	for (int i = 0; i < numReturnedMoves; i++) {
		// If move is DOUBLE_BACK_n
		if (moveHist[i] >= DOUBLE_BACK_1 && moveHist[i] <= DOUBLE_BACK_5) {
			found = true;
			break;
		}
	}

	if (canfree) free(moveHist);
	return found;
}

// Replaces Items in locations with Double Backs; assumes Double Back isn't in trail
static PlaceId *ReplaceWithDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
) {
	// Go through places and see if they occur in Dracula's trail, if they do, replace
	// them with double_back_n
	for (int i = 0; i < numHistMoves; i++) {
		for (int j = 0; j < *numReturnedLocs; j++) {
			if (trailMoves[i] == locations[j]) {
				// Move to somewere 1 moves ago -- how many moves before it was
				locations[j] = DOUBLE_BACK_1 + i; // trailMoves[i]; 
			}
		}
	}

	return locations;
}

static PlaceId *RemoveDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
) {
	PlaceId remove = -3;

	// Go through places and see if they occur in Dracula's trail, if they do, remove them
	for (int i = 0; i < numHistMoves; i++) {
		for (int j = 0; j < *numReturnedLocs; j++) {
			if (trailMoves[i] == locations[j]) {
				// Move to somewere 5 moves ago - how many moves before it was
				locations[j] = remove;
			}
		}
	}

	// Rearrange the array

	int numShift = 0;
	for (int i = 0; i < *numReturnedLocs; i++) {
		if (locations[i] == remove) {
			numShift++;
		} else {
			locations[i-numShift] = locations[i];
		}
	}

	*numReturnedLocs -= numShift;
	locations = realloc(locations, sizeof(PlaceId) * (*numReturnedLocs));

	return locations;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
