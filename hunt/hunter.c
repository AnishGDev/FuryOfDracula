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

#define LORD_GODALMING_EARLY_GAME_PATH {EDINBURGH, EDINBURGH, EDINBURGH, EDINBURGH, EDINBURGH}
#define DR_SEWARD_EARLY_GAME_PATH {CADIZ, CADIZ, CADIZ, CADIZ, CADIZ}
#define VAN_HELSING_EARLY_GAME_PATH {TYRRHENIAN_SEA, TYRRHENIAN_SEA, TYRRHENIAN_SEA, TYRRHENIAN_SEA}
#define MINA_HARKER_EARLY_GAME_PATH {BLACK_SEA, BLACK_SEA, BLACK_SEA, BLACK_SEA, BLACK_SEA}

#define HUNTER_EARLY_GAME_PATHS {LORD_GODALMING_EARLY_GAME_PATH, DR_SEWARD_EARLY_GAME_PATH, VAN_HELSING_EARLY_GAME_PATH, MINA_HARKER_EARLY_GAME_PATH}

typedef enum _hunterMode {
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

		/*	DFS on reachable Places which are reachable
		 *	Best location guess on Dracula, is the node furthest from all the other hunters
		 *	PlaceId *dfsPossibleDracLocations(lastKnownDraculaLocation, dracLocAge)
		 *	Go through each location and get the shortest path length for each hunter,
		 *	The Place with the highest score is the predicted dracula location
		 *	Hunters make a move to that location
		 */
	}

	registerBestPlay(placeIdToAbbrev(bestMove), message);
}
