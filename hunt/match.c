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
#define ROUND_CHARS 40
#define START_SCORE 366

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

int main(void) {
	char pastPlays[ROUND_CHARS * START_SCORE] = "";

	for (int round = 0; round < START_SCORE; round++) {
		bool end = false;

		for (int player = 0; player < NUM_PLAYERS - 1; player++) {
			decideHunterMove(HvNew(pastPlays, (Message[]) {}));

			if (player == 0) strcat(pastPlays, "G");
			if (player == 1) strcat(pastPlays, "S");
			if (player == 2) strcat(pastPlays, "H");
			if (player == 3) strcat(pastPlays, "M");

			strcat(pastPlays, latestPlay);
			strcat(pastPlays, ".... ");

			if (HvGetHealth(HvNew(
				pastPlays, (Message[]) {}
			), PLAYER_DRACULA) <= 0) {
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

		if (DvGetHealth(DvNew(
			pastPlays, (Message[]) {}
		), PLAYER_DRACULA) <= 0) {
			break;
		}
	}

	printf("%s\n", pastPlays);

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
