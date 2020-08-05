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

#include <stdio.h>

#include "Game.h"
#include "hunter.h"
#include "HunterView.h"

#define RESEARCH_THRESHOLD 12
#define PATROL_THRESHOLD 4
#define HUNT_THRESHOLD 0

#define EARLY_GAME_ROUNDS 5
#define TURN_CHARS 8

#define LORD_GODALMING_EARLY_GAME_PATH { \
	EDINBURGH, MANCHESTER, LONDON, SWANSEA, LIVERPOOL \
}

#define DR_SEWARD_EARLY_GAME_PATH { \
	MADRID, BORDEAUX, NANTES, LE_HAVRE, PARIS \
}

#define VAN_HELSING_EARLY_GAME_PATH { \
	ROME, MILAN, ZURICH, STRASBOURG, LEIPZIG \
}

#define MINA_HARKER_EARLY_GAME_PATH { \
	GALATZ, CASTLE_DRACULA, KLAUSENBURG, BELGRADE, BUDAPEST \
}

typedef struct _EvaluatedLoc {
	PlaceId place;
	int score;
} EvaluatedLoc;

PlaceId earlyMode(HunterView hv, Message *message);
PlaceId researchMode(HunterView hv, Message *message);
PlaceId patrolMode(HunterView hv, Message *message);
PlaceId huntMode(HunterView hv, Message *message);

PlaceId bestDracLoc(HunterView hv);
EvaluatedLoc runBestDracLoc(
	HunterView *history, PlaceId from, Round fromRound, Round toRound
);
int evaluateDracLoc(HunterView hv, PlaceId loc);
HunterView *buildHistory(HunterView hv, int *length);
void freeHistory(HunterView *history, int length);

PlaceId *removeLocsByType (PlaceId *places, PlaceType placeType, int *numLocs);

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
	//printf("DracLoc:%s\n", placeIdToName(HvGetLastKnownDraculaLocation(hv, &dracLocAge)));
	
	if (HvGetLastKnownDraculaLocation(hv, &dracLocAge) != NOWHERE) {
		dracLocAge = HvGetRound(hv) - dracLocAge;
	}

	//printf("Round: %d\n", HvGetRound(hv));
	//printf("DracLocAge %d\n", dracLocAge);
	//printf("CurLoc %s\n", placeIdToName(HvGetPlayerLocation(hv, HvGetPlayer(hv))));
	//printf("CurHealth %d\n", HvGetHealth(hv, HvGetPlayer(hv)));

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
	// No movement
	return NOWHERE;
}

// Spread out and cover a large area to find the trail
PlaceId patrolMode(HunterView hv, Message *message) {
	// TODO: determine locations dracula could have gone
	int dracLocAge = 0;
	PlaceId lastDracLoc = HvGetLastKnownDraculaLocation(hv, &dracLocAge);
	dracLocAge = HvGetRound(hv) - dracLocAge;

	PlaceType dracLocType = placeIdToType(HvGetPlayerLocation(hv, PLAYER_DRACULA));
	//printf("LastDLoc: %s\n", placeIdToName(lastDracLoc));
	int numLocs = -1;
	PlaceId *patrolLocs = locationsNNodesAway(hv, lastDracLoc, dracLocAge, &numLocs);

	patrolLocs = removeLocsByType(patrolLocs, dracLocType, &numLocs);
	//printf("NLocs: %d\n", numLocs);
	//for (int i = 0; i < numLocs; i++) printf("Patrol: %s %d\n", placeIdToName(patrolLocs[i]), placeIdToType(patrolLocs[i]));


	// TODO: Discard Nodes whose path would've resulted in 
	// a hunter revealing a better dracLocAge

	// TODO: determine the best way to split the nodes
	// amoung the hunters
	Player whosSearching[NUM_REAL_PLACES];
	int bestLen = 100;
	int lenToDest;

	for (int i = 0; i < numLocs; i++) {
		bestLen = 100;
		for (int j = 0; j < NUM_PLAYERS - 1; j++) {
			HvGetShortestPathTo(hv, j, patrolLocs[i], &lenToDest);
			if (lenToDest < bestLen) {
				bestLen = lenToDest;
				whosSearching[i] = j;
			}
		}
	}

	Player me = HvGetPlayer(hv);
	PlaceId *path;
	int numPathLocs;
	PlaceId bestMove = NOWHERE;

	for (int i = 0; i < numLocs; i++) {
		if (whosSearching[i] == me) {
			path = HvGetShortestPathTo(hv, me, patrolLocs[i], &numPathLocs);
			if (numLocs > 0) {
				bestMove = path[0];
				break;
			}
		}
	}
	// If the hunter isnt closest to any node move closer to last Drac Loc
	if (bestMove == NOWHERE) {
		path = HvGetShortestPathTo(hv, me, lastDracLoc, &numPathLocs);
		bestMove = path[0];
	}
	free(patrolLocs);
	free(path);
	return bestMove;
}

// Array of place names for debugging; TODO: remove
char *d[] = {"ADRIATIC_SEA", "ALICANTE", "AMSTERDAM", "ATHENS", "ATLANTIC_OCEAN", "BARCELONA", "BARI", "BAY_OF_BISCAY", "BELGRADE", "BERLIN", "BLACK_SEA", "BORDEAUX", "BRUSSELS", "BUCHAREST", "BUDAPEST", "CADIZ", "CAGLIARI", "CASTLE_DRACULA", "CLERMONT_FERRAND", "COLOGNE", "CONSTANTA", "DUBLIN", "EDINBURGH", "ENGLISH_CHANNEL", "FLORENCE", "FRANKFURT", "GALATZ", "GALWAY", "GENEVA", "GENOA", "GRANADA", "HAMBURG", "IONIAN_SEA", "IRISH_SEA", "KLAUSENBURG", "LE_HAVRE", "LEIPZIG", "LISBON", "LIVERPOOL", "LONDON", "MADRID", "MANCHESTER", "MARSEILLES", "MEDITERRANEAN_SEA", "MILAN", "MUNICH", "NANTES", "NAPLES", "NORTH_SEA", "NUREMBURG", "PARIS", "PLYMOUTH", "PRAGUE", "ROME", "SALONICA", "SANTANDER", "SARAGOSSA", "SARAJEVO", "SOFIA", "ST_JOSEPH_AND_ST_MARY", "STRASBOURG", "SWANSEA", "SZEGED", "TOULOUSE", "TYRRHENIAN_SEA", "VALONA", "VARNA", "VENICE", "VIENNA", "ZAGREB", "ZURICH"};

// Try to force an encounter, used when info is accurate
PlaceId huntMode(HunterView hv, Message *message) {
	// TODO: have the hunters approach from different directions
	int n;
	PlaceId *path = HvGetShortestPathTo(
		hv, HvGetPlayer(hv), bestDracLoc(hv), &n
	);

	PlaceId move = NOWHERE; // No movement
	if (n > 0) {
		move = path[0];
	}

	free(path);
	return move;
}

// Approximates the singularly most likely place where Dracula would
// currently be
PlaceId bestDracLoc(HunterView hv) {
	int historySize = 0;
	HunterView *history = buildHistory(hv, &historySize);

	Round toRound = HvGetRound(hv) - 1;
	Round fromRound = toRound; // In case something goes wrong
	PlaceId lastDracLoc = HvGetLastKnownDraculaLocation(hv, &fromRound);

	EvaluatedLoc result = runBestDracLoc(
		history, lastDracLoc, fromRound, toRound
	);

	freeHistory(history, historySize);

	return result.place;
}

EvaluatedLoc runBestDracLoc(
	HunterView *history, PlaceId from, Round fromRound, Round toRound
) {
	HunterView hv = history[fromRound];

	if (fromRound == toRound) {
		return (EvaluatedLoc) {
			.place = from,
			.score = evaluateDracLoc(hv, from)
		};
	}

	fromRound++;

	int n = 0;
	PlaceId* reachable = HvGetReachable(
		history[fromRound], PLAYER_DRACULA, fromRound, from, &n
	);

	EvaluatedLoc best = {
		.place = NOWHERE,
		.score = 0
	};

	for (int i = 0; i < n; i++) {
		if (placeIsLand(reachable[i])) {
			EvaluatedLoc this = runBestDracLoc(
				history, reachable[i], fromRound, toRound
			);

			if (this.score >= best.score) {
				best = this;
			}
		}
	}

	free(reachable);

	return best;
}

// Evaluate a location for Dracula within a game state; currently just
// sums the distance to each hunter
int evaluateDracLoc(HunterView hv, PlaceId loc) {
	int score = 0;

	for (int i = 0; i < NUM_PLAYERS - 1; i++) {
		int distance = 0;
		free(HvGetShortestPathTo(hv, i, loc, &distance));
		if (distance == 1) return 0;
		score += distance;
	}

	return score;
}

HunterView *buildHistory(HunterView hv, int *length) {
	Round round = HvGetRound(hv);
	// + 1 for the current state
	HunterView *history = malloc(sizeof(*history) * (round + 1));

	for (int i = 0; i < round; i++) {
		history[i] = HvWaybackMachine(hv, i);
	}
	history[round] = hv;

	*length = round;
	return history;
}

void freeHistory(HunterView *history, int length) {
	for (int i = 0; i < length; i++) {
		HvFree(history[i]);
	}

	free(history);
}

// If a place isnt of the same type it will be removed
PlaceId *removeLocsByType (PlaceId *places, PlaceType placeType, int *numLocs) {
	
	int numShift = 0;
	
	for (int i = 0; i < *numLocs; i++) {
		if (placeIdToType(places[i]) != placeType) {
			numShift++;
		} else {
			places[i - numShift] = places[i];
		}
	}
	
	*numLocs -= numShift;
	return places;
}
