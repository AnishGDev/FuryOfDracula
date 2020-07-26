////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// Places.h: places and place utilities
//
// 2012-01-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-11-25	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
// // // // // // // // DO NOT CHANGE THIS FILE! // // // // // // // //

#include <stdbool.h>
#include <stdlib.h>

#ifndef FOD__PLACES_H_
#define FOD__PLACES_H_

// Types of places
typedef enum placeType {
	UNKNOWN,
	LAND,
	SEA,
} PlaceType;

// Types of connections between places
typedef enum transportType {
	NONE,
	ROAD,
	RAIL,
	BOAT,
	ANY,
} TransportType;

#define MIN_TRANSPORT (TransportType) ROAD
#define MAX_TRANSPORT (TransportType) BOAT

// Places
// Real places should come first
typedef enum placeId {
	// real places
	ADRIATIC_SEA,
	ALICANTE,
	AMSTERDAM,
	ATHENS,
	ATLANTIC_OCEAN,
	BARCELONA,
	BARI,
	BAY_OF_BISCAY,
	BELGRADE,
	BERLIN,
	BLACK_SEA,
	BORDEAUX,
	BRUSSELS,
	BUCHAREST,
	BUDAPEST,
	CADIZ,
	CAGLIARI,
	CASTLE_DRACULA,
	CLERMONT_FERRAND,
	COLOGNE,
	CONSTANTA,
	DUBLIN,
	EDINBURGH,
	ENGLISH_CHANNEL,
	FLORENCE,
	FRANKFURT,
	GALATZ,
	GALWAY,
	GENEVA,
	GENOA,
	GRANADA,
	HAMBURG,
	IONIAN_SEA,
	IRISH_SEA,
	KLAUSENBURG,
	LE_HAVRE,
	LEIPZIG,
	LISBON,
	LIVERPOOL,
	LONDON,
	MADRID,
	MANCHESTER,
	MARSEILLES,
	MEDITERRANEAN_SEA,
	MILAN,
	MUNICH,
	NANTES,
	NAPLES,
	NORTH_SEA,
	NUREMBURG,
	PARIS,
	PLYMOUTH,
	PRAGUE,
	ROME,
	SALONICA,
	SANTANDER,
	SARAGOSSA,
	SARAJEVO,
	SOFIA,
	ST_JOSEPH_AND_ST_MARY,
	STRASBOURG,
	SWANSEA,
	SZEGED,
	TOULOUSE,
	TYRRHENIAN_SEA,
	VALONA,
	VARNA,
	VENICE,
	VIENNA,
	ZAGREB,
	ZURICH,

	// other places
	CITY_UNKNOWN = 100,
	SEA_UNKNOWN = 101,
	HIDE = 102,
	DOUBLE_BACK_1 = 103,
	DOUBLE_BACK_2 = 104,
	DOUBLE_BACK_3 = 105,
	DOUBLE_BACK_4 = 106,
	DOUBLE_BACK_5 = 107,
	TELEPORT = 108,
	UNKNOWN_PLACE = -1,
	NOWHERE = -2,
} PlaceId;

#define MIN_REAL_PLACE	(PlaceId) ADRIATIC_SEA // Should be 0
#define HOSPITAL_PLACE	(PlaceId) ST_JOSEPH_AND_ST_MARY
#define MAX_REAL_PLACE	(PlaceId) ZURICH
#define NUM_REAL_PLACES	((int) ZURICH + 1)

typedef struct place {
	PlaceId    id;
	char      *name;
	char      *abbrev;
	PlaceType  type;
} Place;

extern Place PLACES[];

typedef struct connection {
	PlaceId       v;
	PlaceId       w;
	TransportType t;
} Connection;

extern Connection CONNECTIONS[];

////////////////////////////////////////////////////////////////////////

/** given a place name, return its ID number */
PlaceId placeNameToId(char *name);

/** given a place abbreviation, return its ID number */
PlaceId placeAbbrevToId(char *abbrev);

////////////////////////////////////////////////////////////////////////

/** given a place number, return its type */
PlaceType placeIdToType(PlaceId pid);

/** given a place number, return its name */
const char *placeIdToName(PlaceId pid);

/** given a place number, return its two-char code */
const char *placeIdToAbbrev(PlaceId pid);

////////////////////////////////////////////////////////////////////////

/** given a type of transport, return a string representation. */
const char *transportTypeToString(TransportType t);

/** is the given mode of transport valid? */
static inline bool transportTypeIsValid(TransportType t)
{
	return t >= MIN_TRANSPORT && t <= MAX_TRANSPORT;
}

////////////////////////////////////////////////////////////////////////

/** given a place, is it land? */
static inline bool placeIsLand(PlaceId pid)
{
	return placeIdToType(pid) == LAND;
}

/** given a place, is it a sea? */
static inline bool placeIsSea(PlaceId pid)
{
	return placeIdToType(pid) == SEA;
}

/** given a place, is it a real place? */
static inline bool placeIsReal(PlaceId pid)
{
	return pid >= MIN_REAL_PLACE && pid <= MAX_REAL_PLACE;
}

////////////////////////////////////////////////////////////////////////

#endif // !defined (FOD__PLACES_H_)
