////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.h: the interface to your "Fury of Dracula" hunter AI
//
// 2012-01-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
// // // // // // // // DO NOT CHANGE THIS FILE! // // // // // // // //

#ifndef FOD__HUNTER_H_
#define FOD__HUNTER_H_

#include "HunterView.h"

/**
 * For  a  given view of the game, decide what the best next move is for
 * the current hunter by calling `registerBestPlay`.
 */
void decideHunterMove(HunterView state);

#endif // !defined(FOD__HUNTER_H_)
