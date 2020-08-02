////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testUtils.h: test utilities
// You can change this as much as you want!
//
// 2020-07-10   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdlib.h>

#include "Game.h"
#include "Places.h"

#ifndef FOD__TEST_UTILS_H_
#define FOD__TEST_UTILS_H_

/**
 * Sorts an array of PlaceIds
 */
void sortPlaces(PlaceId *places, int numPlaces);

#endif // !defined (FOD__TEST_UTILS_H_)
