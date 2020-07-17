////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testMap.c: a simple program that checks the Map ADT
// You can change this as much as you want!
// You do not need to submit this file.
//
// 2020-07-10	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"

#define MAX_LINE 1024

int main(void)
{
	Map m = MapNew();
	// MapShow(m);
	
	char buffer[MAX_LINE];
	
	printf("\nType a location name to see its connections: ");
	while (fgets(buffer, MAX_LINE, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0'; // remove newline character
		
		PlaceId place = placeNameToId(buffer);
		if (place == NOWHERE) {
			printf("Unknown location '%s'\n", buffer);
		} else {
			for (ConnList c = MapGetConnections(m, place); c != NULL; c = c->next) {
				const char *dest = placeIdToName(c->p);
				const char *transportType = transportTypeToString(c->type);
				printf("%s connects to %s by %s\n", buffer, dest, transportType);
			}
		}
		
		printf("\nType a location name to see its connections: ");
	}
}
