////////////////////////////////////////////////////////////////////////
// Runs a player's game turn ...
//
// Can  produce  either a hunter player or a Dracula player depending on
// the setting of the I_AM_DRACULA #define
//
// This  is  a  dummy  version of the real player.c used when you submit
// your AIs. It is provided so that you can test whether  your  code  is
// likely to compile ...
//
// Note that this is used to drive both hunter and Dracula AIs. It first
// creates an appropriate view, and then invokes the relevant decideMove
// function,  which  should  use the registerBestPlay() function to send
// the move back.
//
// The real player.c applies a timeout, and will halt your  AI  after  a
// fixed  amount of time if it doesn 't finish first. The last move that
// your AI registers (using the registerBestPlay() function) will be the
// one used by the game engine. This version of player.c won't stop your
// decideMove function if it goes into an infinite loop. Sort  that  out
// before you submit.
//
// Based on the program by David Collien, written in 2012
//
// 2017-12-04	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.3	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "dracula.h"
#include "DraculaView.h"
#include "hunter.h"
#include "HunterView.h"

// Moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3
#define LOC_SIZE_STR 3
#define TURN_CHARS 8
#define ROUND_CHARS 40
#define START_SCORE 366

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

Player getPlayer(char *s, int i) {
	if (i < 0) return -2;
	if (s[i] == 'G') return 0;
	if (s[i] == 'S') return 1;
	if (s[i] == 'H') return 2;
	if (s[i] == 'M') return 3;
	if (s[i] == 'D') return 4;
	return -1;
}

PlaceId getLoc(char *s, int i) {
	char locString[LOC_SIZE_STR];
	sprintf(locString, "%c%c", s[i + 1], s[i + 2]);
	return placeAbbrevToId(locString);
}

bool hasRested(char *s, int i) {
	return (
		i >= TURN_CHARS * NUM_PLAYERS
		&& getLoc(s, i) == getLoc(s, i - TURN_CHARS * NUM_PLAYERS)
	);
}

bool hasResearched(char *s, int i) {
	for (int j = i; j >= i - TURN_CHARS * NUM_PLAYERS; j -= TURN_CHARS) {
		if (getPlayer(s, j) != PLAYER_DRACULA && !hasRested(s, j)) {
			return false;
		}
	}
	return true;
}

void trailAdd(PlaceId *trail, PlaceId loc) {
	for (int i = TRAIL_SIZE - 1; i > 0; i--) {
		trail[i] = trail[i - 1];
	}

	trail[0] = loc;
}

bool isRevealed(char *s, int i) {
	return false; // TODO
}

char *redactedPastPlays(char *pastPlays) {
	char *new = strdup(pastPlays);
	PlaceId trail[TRAIL_SIZE] = {[0 ... TRAIL_SIZE - 1] = NOWHERE};
	
	for (int i = 0; i < strlen(new); i += TURN_CHARS) {
		Player player = getPlayer(new, i);
		PlaceId loc = getLoc(new, i);

		if (player == PLAYER_DRACULA) {
			trailAdd(trail, getLoc(new, i));

			if (!isRevealed(new, i)) {
				new[i + 1] = placeIsLand(loc) ? 'C' : 'S';
				new[i + 2] = '?';
			}
		}
		// if (hasResearched(new, i)) {
		// 	printf("Research at %d\n", i);
		// }
	}

	return new;
}

int main(void) {
	char pastPlays[ROUND_CHARS * START_SCORE] = "";

	for (int round = 0; round < 20; round++) {
		bool end = false;

		for (int player = 0; player < NUM_PLAYERS - 1; player++) {
			printf("%s\n", redactedPastPlays(pastPlays));

			decideHunterMove(HvNew(pastPlays, (Message[]) {}));

			if (player == 0) strcat(pastPlays, "G");
			if (player == 1) strcat(pastPlays, "S");
			if (player == 2) strcat(pastPlays, "H");
			if (player == 3) strcat(pastPlays, "M");

			strcat(pastPlays, latestPlay);
			strcat(pastPlays, ".... ");

			HunterView hv = HvNew(pastPlays, (Message[]) {});
			if (HvGetHealth(hv, PLAYER_DRACULA) <= 0) {
				end = true;
			}
		}

		if (end) break;

		DraculaView dv = DvNew(pastPlays, (Message[]) {});
		decideDraculaMove(dv);

		strcat(pastPlays, "D");
		strcat(pastPlays, latestPlay);
		if (round % 13 == 0) {
			strcat(pastPlays, ".V");
		} else {
			strcat(pastPlays, "T.");
		}
		strcat(pastPlays, ".. ");

		dv = DvNew(pastPlays, (Message[]) {});
		if (DvGetHealth(dv, PLAYER_DRACULA) <= 0) {
			break;
		}

		printf(
			"=== Hunters: %d === Dracula: %d ===\n",
			DvGetScore(dv),
			DvGetHealth(dv, PLAYER_DRACULA)
		);
		// printf("%s\n", pastPlays);
	}

	DraculaView dv = DvNew(pastPlays, (Message[]) {});
	int hunters = DvGetScore(dv);
	int dracula = DvGetHealth(dv, PLAYER_DRACULA);
	if (hunters > 0 && dracula <= 0) {
		printf("The hunters won!\n");
	} else if (hunters <= 0 && dracula > 0) {
		printf("Dracula won!");
	} else {
		printf("Tie?\n");
	}

	return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay(char *play, Message message) {
	strncpy(latestPlay, play, MOVE_SIZE - 1);
	latestPlay[MOVE_SIZE - 1] = '\0';

	strncpy(latestMessage, message, MESSAGE_SIZE - 1);
	latestMessage[MESSAGE_SIZE - 1] = '\0';
}
