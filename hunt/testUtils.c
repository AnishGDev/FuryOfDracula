////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testUtils.c: test utilities
// You can change this as much as you want!
//
// 2020-07-10   v1.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "testUtils.h"

static int placeIdCmp(const void *ptr1, const void *ptr2) {
	PlaceId p1 = *(PlaceId *)ptr1;
	PlaceId p2 = *(PlaceId *)ptr2;
	return p1 - p2;
}

void sortPlaces(PlaceId *places, int numPlaces) {
	qsort(places, (size_t)numPlaces, sizeof(PlaceId), placeIdCmp);
}
