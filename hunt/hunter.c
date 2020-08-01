////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "Game.h"
#include "hunter.h"
#include "HunterView.h"

#define LORD_GODALMING_EARLY_GAME_PATH {EDINBURGH, MANCHESTER, LIVERPOOL, SWANSEA, LONDON}
#define DR_SEWARD_EARLY_GAME_PATH {TYRRHENIAN_SEA, ROME, FLORENCE, GENOA, MILAN}
#define VAN_HELSING_EARLY_GAME_PATH {ATLANTIC_OCEAN, BAY_OF_BISCAY, SANTANDER, SARAGOSSA, TOULOUSE}
#define MINA_HARKER_EARLY_GAME_PATH {BLACK_SEA, CONSTANTA, BUCHAREST, BELGRADE, SZEGED}

#define HUNTER_EARLY_GAME_PATHS {LORD_GODALMING_EARLY_GAME_PATH, DR_SEWARD_EARLY_GAME_PATH, VAN_HELSING_EARLY_GAME_PATH, MINA_HARKER_EARLY_GAME_PATH}

// Returns an array of locations which dracula can reach
PlaceId dfsBestDracLoc(HunterView hv, PlaceId lastKnownDraculaLocation, Round dracLocAge, PlaceId from);
PlaceId *dfs(PlaceId from, int depth, int *numLocs);
int evaluateLoc(HunterView hv, PlaceId location);

typedef enum _hunterMode { 	// Tbh Idk if we really need theses
	PATROL,
	HUNT,
	KILL,
} HunterMode;

void decideHunterMove(HunterView hv)
{
	// TODO: Replace this with something better!

	Round curRound = HvGetRound(hv);
	Player curPlayer = HvGetPlayer(hv);
	PlaceId bestMove;
	Message message;
	Round dracLocAge = 12;
	PlaceId lastDracLoc = HvGetLastKnownDraculaLocation(hv, &dracLocAge);

	if (dracLocAge > 11) {
		if (curPlayer < 5) {	// Early Game Hard Coded Paths
			PlaceId hunterPaths[NUM_PLAYERS-1][5] = HUNTER_EARLY_GAME_PATHS;
			bestMove = hunterPaths[curPlayer][curRound];
		} else {	// Rest
			bestMove = HvGetPlayerLocation(hv, curPlayer);
		}
	} else if (dracLocAge < 3) {
		// Dracula is close hunt him down
		// More aggressive Search
	} else { 	// dracLocAge is between 3 and 11
		PlaceId mostLikelyDracLoc = dfsBestDracLoc(hv, lastDracLoc, dracLocAge, HvGetPlayerLocation(hv, curPlayer));
		int numRetLocs = -1;
		// Here we can set a different target to have the 
		// hunters approach from different directions
		PlaceId *path = HvGetShortestPathTo(hv, curPlayer, mostLikelyDracLoc, &numRetLocs);
		bestMove = path[0];
	}

	registerBestPlay(placeIdToAbbrev(bestMove), message);
}

/*	DFS on reachable Places which are reachable
*	Best location guess on Dracula, is the node furthest from all the other hunters
*	PlaceId *dfsPossibleDracLocations(lastKnownDraculaLocation, dracLocAge)
*	Go through each location and get the shortest path length for each hunter,
*	The Place with the highest score is the predicted dracula location
*	Hunters make a move to that location
*/
PlaceId dfsBestDracLoc(HunterView hv, PlaceId lastKnownDraculaLocation, Round dracLocAge, PlaceId from) {
	
	int bestScore = -1; // Scores are always Greater than 0
	PlaceId targetLoc;
	int numLocs = -1;
	PlaceId *possibleLocs = dfs(lastKnownDraculaLocation, dracLocAge, &numLocs);
	for (int i = 0; i < numLocs; i++) {
		if (evaluateLoc(hv, possibleLocs[i]) > bestScore) targetLoc = possibleLocs[i];
	}
	return targetLoc;
}

PlaceId *dfs(PlaceId from, int depth, int *numLocs) {
	return NULL;
}

// Evaluates a location based on a current game state
// Currently just sums the distance to each hunter
int evaluateDracLoc(HunterView hv, PlaceId location) {

	PlaceId *path;
	int score = 0;
	int  pathLengthToDest = -1;

	for (int i = 0; i < NUM_PLAYERS-1; i++) {
		path = HvGetShortestPathTo(hv, i, location, &pathLengthToDest);
		if (path) path = NULL; 	// Compiler says path is unused without this
		score += pathLengthToDest;
	}
	return score;
}