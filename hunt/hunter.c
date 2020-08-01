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

#define EARLY_GAME_ROUNDS 5
#define RESEARCH_THRESHOLD 12
#define PATROL_THRESHOLD 3
#define HUNT_THRESHOLD 0

#define LORD_GODALMING_EARLY_GAME_PATH { \
	EDINBURGH, MANCHESTER, LIVERPOOL, SWANSEA, LONDON \
}

#define DR_SEWARD_EARLY_GAME_PATH { \
	TYRRHENIAN_SEA, ROME, FLORENCE, GENOA, MILAN \
}

#define VAN_HELSING_EARLY_GAME_PATH { \
	ATLANTIC_OCEAN, BAY_OF_BISCAY, SANTANDER, SARAGOSSA, TOULOUSE \
}

#define MINA_HARKER_EARLY_GAME_PATH { \
	BLACK_SEA, CONSTANTA, BUCHAREST, BELGRADE, SZEGED \
}

PlaceId earlyMode(HunterView hv, Message *message);
PlaceId researchMode(HunterView hv, Message *message);
PlaceId patrolMode(HunterView hv, Message *message);
PlaceId huntMode(HunterView hv, Message *message);

PlaceId bestDracLoc(HunterView hv, PlaceId from);
PlaceId *dfs(PlaceId from, int depth, int *numLocs);
int evaluateDracLoc(HunterView hv, PlaceId loc);

// Is a global variable like this ok?
PlaceId hunterPaths[NUM_PLAYERS - 1][EARLY_GAME_ROUNDS] = {
	LORD_GODALMING_EARLY_GAME_PATH,
	DR_SEWARD_EARLY_GAME_PATH,
	VAN_HELSING_EARLY_GAME_PATH,
	MINA_HARKER_EARLY_GAME_PATH
};

void decideHunterMove(HunterView hv) {
	PlaceId move = NOWHERE;
	Message message = "";

	Round dracLocAge = RESEARCH_THRESHOLD;
	HvGetLastKnownDraculaLocation(hv, &dracLocAge);

	if (dracLocAge >= RESEARCH_THRESHOLD) {
		if (HvGetRound(hv) < EARLY_GAME_ROUNDS) {
			move = earlyMode(hv, &message);
		} else {
			move = researchMode(hv, &message);
		}
	} else if (dracLocAge >= PATROL_THRESHOLD) {
		move = patrolMode(hv, &message);
	} else if (dracLocAge >= HUNT_THRESHOLD) {
		move = huntMode(hv, &message);
	}

	if (move == NOWHERE) { // Indicates no movement
		move = HvGetPlayerLocation(hv, HvGetPlayer(hv));
	}

	registerBestPlay((char *) placeIdToAbbrev(move), message);
}

// Hard-coded paths used in the early-game
PlaceId earlyMode(HunterView hv, Message *message) {
	return hunterPaths[HvGetPlayer(hv)][HvGetRound(hv)];
}

// Research used when our info is stale
PlaceId researchMode(HunterView hv, Message *message) {
	// Indicates no movement
	return NOWHERE;
}

// Spread out and cover a large area to find the trail
PlaceId patrolMode(HunterView hv, Message *message) {
	Player me = HvGetPlayer(hv);
	PlaceId target = bestDracLoc(hv, HvGetPlayerLocation(hv, me));

	// TODO: set different target to have the hunters approach from
	// different directions
	int n;
	PlaceId *path = HvGetShortestPathTo(hv, me, target, &n);
	
	return path[0];
}

// Try to force an encounter, used when info is accurate
PlaceId huntMode(HunterView hv, Message *message) {
	return NOWHERE;
}

// Approximates the singularly most likely place where Dracula would
// currently be
PlaceId bestDracLoc(HunterView hv, PlaceId from) {
	PlaceId bestLoc;
	int bestScore = -1; // Scores always > 0

	Round dracLocAge = 0;
	PlaceId lastDracLoc = HvGetLastKnownDraculaLocation(hv, &dracLocAge);

	int numLocs = -1;
	PlaceId *possibleLocs = dfs(lastDracLoc, dracLocAge, &numLocs);

	for (int i = 0; i < numLocs; i++) {
		if (evaluateDracLoc(hv, possibleLocs[i]) > bestScore) {
			bestLoc = possibleLocs[i];
		}
	}

	return bestLoc;
}

PlaceId *dfs(PlaceId from, int depth, int *numLocs) {
	return NULL;
}

// Evaluate a location for Dracula within a game state; currently just
// sums the distance to each hunter
int evaluateDracLoc(HunterView hv, PlaceId loc) {
	int score = 0;

	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		int pathLength = -1;
		HvGetShortestPathTo(hv, i, loc, &pathLength);

		score += pathLength;
	}

	return score;
}