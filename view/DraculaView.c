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

#define REMOVED -3 

// Long parameters have been split up for more readability.
static bool hiddenInLast5(DraculaView dv, PlaceId *moveHist, int numHistMoves);
static bool doubleInLast5(DraculaView dv, PlaceId *moveHist, int numHistMoves);
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
	DraculaView new = malloc(sizeof(struct draculaView));

	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}

	new->gv = GvNew(pastPlays, messages);
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
	if (DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE) {
		*numReturnedMoves = 0;
		return NULL;
	}

	PlaceId *moves = DvWhereCanIGoByType(dv, true, true, numReturnedMoves);

	int numHistMoves;
	bool canFree = false;
	int numMoves = TRAIL_SIZE-1;

	PlaceId *trailMoves = GvGetLastMoves(
		dv->gv, PLAYER_DRACULA, numMoves, &numHistMoves, &canFree
	);

	bool doubleBackInTrail = doubleInLast5(dv, trailMoves, numHistMoves);
	
	if (!doubleBackInTrail) {	// Add Double backs as moves
		moves = ReplaceWithDoubleBack(moves, trailMoves, numHistMoves, numReturnedMoves);
	} else {
		moves = RemoveDoubleBack(moves, trailMoves, numHistMoves, numReturnedMoves);
	}

	bool hiddenInTrail = hiddenInLast5(dv, trailMoves, numHistMoves);

	if (placeIdToType(DvGetPlayerLocation(dv, PLAYER_DRACULA)) != SEA) { // Can't Hide at sea
		if (!hiddenInTrail) { // Add Hide as a move
			*numReturnedMoves += 1;
			moves = realloc(moves, sizeof(PlaceId) * (*numReturnedMoves));
			moves[*numReturnedMoves-1] = HIDE;
		}
	}
	if (canFree) free(trailMoves);
	if (*numReturnedMoves == 0) {
		free(moves);
		return NULL;
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
		
		// Account for trail Restrictions. 
		int numHistMoves;
		bool canFree = false;
		int numMoves = TRAIL_SIZE-1;

		PlaceId *trailMoves = GvGetLastMoves(
			dv->gv, PLAYER_DRACULA, numMoves, &numHistMoves, &canFree
		);

		bool doubleBackInTrail = doubleInLast5(dv, trailMoves, numHistMoves);

		if (doubleBackInTrail) { // Remove Double back locations
			places = RemoveDoubleBack(places, trailMoves, numHistMoves, numReturnedLocs);

			// If still can hide, add current location Back
			bool hiddenInTrail = hiddenInLast5(dv, trailMoves, numHistMoves);
			if (!hiddenInTrail) {
				PlaceId currLoc = DvGetPlayerLocation(dv, PLAYER_DRACULA);
				bool in = false;

				for (int i = 0; i < *numReturnedLocs; i++) {
					if (places[i] == currLoc) {
						in = true;
						break;
					}
				}

				if (!in) {
					*numReturnedLocs += 1;
					places = realloc(places, sizeof(PlaceId) * (*numReturnedLocs));
					places[*numReturnedLocs - 1] = currLoc;
				}
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


////////////////////////////////////////////////////////////////////////
// CUSTOM FUNCTIONS. 

/* These functons take in a trailMoves/moveHist parameter so that 
/  when the dracula AI can choose to modify ITS OWN view of the trail, (so if it decides to 
   considers future states of the game) the getReachable functions take this into account and 
   return proper results. */

// Checks if dracula has used a Hide in the last 5 moves
static bool hiddenInLast5 (DraculaView dv, PlaceId *moveHist, int numHistMoves) {
	bool found = false;

	for (int i = 0; i < numHistMoves; i++) {
		if (moveHist[i] == HIDE) {
			found = true;
			break;
		}
	}

	return found;
}

//Checks if dracula has used a doubleback in the last 5 moves
static bool doubleInLast5 (DraculaView dv, PlaceId *moveHist, int numHistMoves) {
	bool found = false;

	for (int i = 0; i < numHistMoves; i++) {
		//IF move is DOUBLE_BACK_1 or 2 or 3 or 4 or 5
		if (moveHist[i] >= DOUBLE_BACK_1 && moveHist[i] <= DOUBLE_BACK_5) {
			found = true;
			break;
		}
	}

	return found;
}

// Replaces Items in locations with Double Backs; assumes Double Back isn't in trail
static PlaceId *ReplaceWithDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
) {
	// Go through places and see if they occur in Dracula's trail, if they do, replace
	// them with double_back_n
	for (int i = numHistMoves - 1; i >= 0; i--) {
		for (int j = 0; j < *numReturnedLocs; j++) {
			if (trailMoves[i] == locations[j]) {
				// Move to somewere 1 moves ago -- how many moves before it was
				locations[j] = DOUBLE_BACK_1 + i; // trailMoves[i]; 
			}
		}
	}

	return locations;
}

// Removes double back locations in the set of possible locations. 
static PlaceId *RemoveDoubleBack(
	PlaceId *locations, PlaceId *trailMoves, int numHistMoves, int *numReturnedLocs
) {

	// Go through places and see if they occur in Dracula's trail, if they do, remove them
	for (int i = 0; i < numHistMoves; i++) {
		for (int j = 0; j < *numReturnedLocs; j++) {
			if (trailMoves[i] == locations[j]) {
				// Move to somewhere 5 moves ago - how many moves before it was
				locations[j] = REMOVED;
			}
		}
	}

	// Rearrange the array
	// numShift is the amount of elements removed so far (and how much we have to shift array by)
	int numShift = 0;
	for (int i = 0; i < *numReturnedLocs; i++) {
		if (locations[i] == REMOVED) {
			// Increase numShift (the amount of removed elements so far)
			numShift++;
		} else {
			locations[i-numShift] = locations[i];
		}
	}
	// Amount of elements in array = previousAmount - amountRemoved. 
	*numReturnedLocs -= numShift;
	if (*numReturnedLocs == 0) {
		free(locations);
		locations = NULL;
	}

	return locations;
}
