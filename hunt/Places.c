////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... The Fury of Dracula
// Places.c: places and place utilities
//
// 2012-01-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-11-25	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
// // // // // // // // DO NOT CHANGE THIS FILE! // // // // // // // //

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Places.h"

/**
 * Places should appear in alphabetic order.
 * Each entry should satisfy `(PLACES[i].id == i)`.
 * First real place must be at index `MIN_REAL_PLACE`.
 * Last real place must be at index `MAX_REAL_PLACE`.
 */
Place PLACES[] =
{
	{ADRIATIC_SEA,          "Adriatic Sea",          "AS", SEA },
	{ALICANTE,              "Alicante",              "AL", LAND},
	{AMSTERDAM,             "Amsterdam",             "AM", LAND},
	{ATHENS,                "Athens",                "AT", LAND},
	{ATLANTIC_OCEAN,        "Atlantic Ocean",        "AO", SEA },
	{BARCELONA,             "Barcelona",             "BA", LAND},
	{BARI,                  "Bari",                  "BI", LAND},
	{BAY_OF_BISCAY,         "Bay of Biscay",         "BB", SEA },
	{BELGRADE,              "Belgrade",              "BE", LAND},
	{BERLIN,                "Berlin",                "BR", LAND},
	{BLACK_SEA,             "Black Sea",             "BS", SEA },
	{BORDEAUX,              "Bordeaux",              "BO", LAND},
	{BRUSSELS,              "Brussels",              "BU", LAND},
	{BUCHAREST,             "Bucharest",             "BC", LAND},
	{BUDAPEST,              "Budapest",              "BD", LAND},
	{CADIZ,                 "Cadiz",                 "CA", LAND},
	{CAGLIARI,              "Cagliari",              "CG", LAND},
	{CASTLE_DRACULA,        "Castle Dracula",        "CD", LAND},
	{CLERMONT_FERRAND,      "Clermont-Ferrand",      "CF", LAND},
	{COLOGNE,               "Cologne",               "CO", LAND},
	{CONSTANTA,             "Constanta",             "CN", LAND},
	{DUBLIN,                "Dublin",                "DU", LAND},
	{EDINBURGH,             "Edinburgh",             "ED", LAND},
	{ENGLISH_CHANNEL,       "English Channel",       "EC", SEA },
	{FLORENCE,              "Florence",              "FL", LAND},
	{FRANKFURT,             "Frankfurt",             "FR", LAND},
	{GALATZ,                "Galatz",                "GA", LAND},
	{GALWAY,                "Galway",                "GW", LAND},
	{GENEVA,                "Geneva",                "GE", LAND},
	{GENOA,                 "Genoa",                 "GO", LAND},
	{GRANADA,               "Granada",               "GR", LAND},
	{HAMBURG,               "Hamburg",               "HA", LAND},
	{IONIAN_SEA,            "Ionian Sea",            "IO", SEA },
	{IRISH_SEA,             "Irish Sea",             "IR", SEA },
	{KLAUSENBURG,           "Klausenburg",           "KL", LAND},
	{LE_HAVRE,              "Le Havre",              "LE", LAND},
	{LEIPZIG,               "Leipzig",               "LI", LAND},
	{LISBON,                "Lisbon",                "LS", LAND},
	{LIVERPOOL,             "Liverpool",             "LV", LAND},
	{LONDON,                "London",                "LO", LAND},
	{MADRID,                "Madrid",                "MA", LAND},
	{MANCHESTER,            "Manchester",            "MN", LAND},
	{MARSEILLES,            "Marseilles",            "MR", LAND},
	{MEDITERRANEAN_SEA,     "Mediterranean Sea",     "MS", SEA },
	{MILAN,                 "Milan",                 "MI", LAND},
	{MUNICH,                "Munich",                "MU", LAND},
	{NANTES,                "Nantes",                "NA", LAND},
	{NAPLES,                "Naples",                "NP", LAND},
	{NORTH_SEA,             "North Sea",             "NS", SEA },
	{NUREMBURG,             "Nuremburg",             "NU", LAND},
	{PARIS,                 "Paris",                 "PA", LAND},
	{PLYMOUTH,              "Plymouth",              "PL", LAND},
	{PRAGUE,                "Prague",                "PR", LAND},
	{ROME,                  "Rome",                  "RO", LAND},
	{SALONICA,              "Salonica",              "SA", LAND},
	{SANTANDER,             "Santander",             "SN", LAND},
	{SARAGOSSA,             "Saragossa",             "SR", LAND},
	{SARAJEVO,              "Sarajevo",              "SJ", LAND},
	{SOFIA,                 "Sofia",                 "SO", LAND},
	{ST_JOSEPH_AND_ST_MARY, "St Joseph and St Mary", "JM", LAND},
	{STRASBOURG,            "Strasbourg",            "ST", LAND},
	{SWANSEA,               "Swansea",               "SW", LAND},
	{SZEGED,                "Szeged",                "SZ", LAND},
	{TOULOUSE,              "Toulouse",              "TO", LAND},
	{TYRRHENIAN_SEA,        "Tyrrhenian Sea",        "TS", SEA },
	{VALONA,                "Valona",                "VA", LAND},
	{VARNA,                 "Varna",                 "VR", LAND},
	{VENICE,                "Venice",                "VE", LAND},
	{VIENNA,                "Vienna",                "VI", LAND},
	{ZAGREB,                "Zagreb",                "ZA", LAND},
	{ZURICH,                "Zurich",                "ZU", LAND},
};

/**
 * Connections have no particular order required.
 * For convenience, they're grouped into road, rail, and boat
 * connections, and sorted alphabetically within those groups.
 */
Connection CONNECTIONS[] = {
	//### ROAD Connections ###
	{ALICANTE,              GRANADA,               ROAD},
	{ALICANTE,              MADRID,                ROAD},
	{ALICANTE,              SARAGOSSA,             ROAD},
	{AMSTERDAM,             BRUSSELS,              ROAD},
	{AMSTERDAM,             COLOGNE,               ROAD},
	{ATHENS,                VALONA,                ROAD},
	{BARCELONA,             SARAGOSSA,             ROAD},
	{BARCELONA,             TOULOUSE,              ROAD},
	{BARI,                  NAPLES,                ROAD},
	{BARI,                  ROME,                  ROAD},
	{BELGRADE,              BUCHAREST,             ROAD},
	{BELGRADE,              KLAUSENBURG,           ROAD},
	{BELGRADE,              SARAJEVO,              ROAD},
	{BELGRADE,              SOFIA,                 ROAD},
	{BELGRADE,              ST_JOSEPH_AND_ST_MARY, ROAD},
	{BELGRADE,              SZEGED,                ROAD},
	{BERLIN,                HAMBURG,               ROAD},
	{BERLIN,                LEIPZIG,               ROAD},
	{BERLIN,                PRAGUE,                ROAD},
	{BORDEAUX,              CLERMONT_FERRAND,      ROAD},
	{BORDEAUX,              NANTES,                ROAD},
	{BORDEAUX,              SARAGOSSA,             ROAD},
	{BORDEAUX,              TOULOUSE,              ROAD},
	{BRUSSELS,              COLOGNE,               ROAD},
	{BRUSSELS,              LE_HAVRE,              ROAD},
	{BRUSSELS,              PARIS,                 ROAD},
	{BRUSSELS,              STRASBOURG,            ROAD},
	{BUCHAREST,             CONSTANTA,             ROAD},
	{BUCHAREST,             GALATZ,                ROAD},
	{BUCHAREST,             KLAUSENBURG,           ROAD},
	{BUCHAREST,             SOFIA,                 ROAD},
	{BUDAPEST,              KLAUSENBURG,           ROAD},
	{BUDAPEST,              SZEGED,                ROAD},
	{BUDAPEST,              VIENNA,                ROAD},
	{BUDAPEST,              ZAGREB,                ROAD},
	{CADIZ,                 GRANADA,               ROAD},
	{CADIZ,                 LISBON,                ROAD},
	{CADIZ,                 MADRID,                ROAD},
	{CASTLE_DRACULA,        GALATZ,                ROAD},
	{CASTLE_DRACULA,        KLAUSENBURG,           ROAD},
	{CLERMONT_FERRAND,      GENEVA,                ROAD},
	{CLERMONT_FERRAND,      MARSEILLES,            ROAD},
	{CLERMONT_FERRAND,      NANTES,                ROAD},
	{CLERMONT_FERRAND,      PARIS,                 ROAD},
	{CLERMONT_FERRAND,      TOULOUSE,              ROAD},
	{COLOGNE,               FRANKFURT,             ROAD},
	{COLOGNE,               HAMBURG,               ROAD},
	{COLOGNE,               LEIPZIG,               ROAD},
	{COLOGNE,               STRASBOURG,            ROAD},
	{CONSTANTA,             GALATZ,                ROAD},
	{CONSTANTA,             VARNA,                 ROAD},
	{DUBLIN,                GALWAY,                ROAD},
	{EDINBURGH,             MANCHESTER,            ROAD},
	{FLORENCE,              GENOA,                 ROAD},
	{FLORENCE,              ROME,                  ROAD},
	{FLORENCE,              VENICE,                ROAD},
	{FRANKFURT,             LEIPZIG,               ROAD},
	{FRANKFURT,             NUREMBURG,             ROAD},
	{FRANKFURT,             STRASBOURG,            ROAD},
	{GALATZ,                KLAUSENBURG,           ROAD},
	{GENEVA,                MARSEILLES,            ROAD},
	{GENEVA,                PARIS,                 ROAD},
	{GENEVA,                STRASBOURG,            ROAD},
	{GENEVA,                ZURICH,                ROAD},
	{GENOA,                 MARSEILLES,            ROAD},
	{GENOA,                 MILAN,                 ROAD},
	{GENOA,                 VENICE,                ROAD},
	{GRANADA,               MADRID,                ROAD},
	{HAMBURG,               LEIPZIG,               ROAD},
	{KLAUSENBURG,           SZEGED,                ROAD},
	{LEIPZIG,               NUREMBURG,             ROAD},
	{LE_HAVRE,              NANTES,                ROAD},
	{LE_HAVRE,              PARIS,                 ROAD},
	{LISBON,                MADRID,                ROAD},
	{LISBON,                SANTANDER,             ROAD},
	{LIVERPOOL,             MANCHESTER,            ROAD},
	{LIVERPOOL,             SWANSEA,               ROAD},
	{LONDON,                MANCHESTER,            ROAD},
	{LONDON,                PLYMOUTH,              ROAD},
	{LONDON,                SWANSEA,               ROAD},
	{MADRID,                SANTANDER,             ROAD},
	{MADRID,                SARAGOSSA,             ROAD},
	{MARSEILLES,            MILAN,                 ROAD},
	{MARSEILLES,            TOULOUSE,              ROAD},
	{MARSEILLES,            ZURICH,                ROAD},
	{MILAN,                 MUNICH,                ROAD},
	{MILAN,                 VENICE,                ROAD},
	{MILAN,                 ZURICH,                ROAD},
	{MUNICH,                NUREMBURG,             ROAD},
	{MUNICH,                STRASBOURG,            ROAD},
	{MUNICH,                VENICE,                ROAD},
	{MUNICH,                VIENNA,                ROAD},
	{MUNICH,                ZAGREB,                ROAD},
	{MUNICH,                ZURICH,                ROAD},
	{NANTES,                PARIS,                 ROAD},
	{NAPLES,                ROME,                  ROAD},
	{NUREMBURG,             PRAGUE,                ROAD},
	{NUREMBURG,             STRASBOURG,            ROAD},
	{PARIS,                 STRASBOURG,            ROAD},
	{PRAGUE,                VIENNA,                ROAD},
	{SALONICA,              SOFIA,                 ROAD},
	{SALONICA,              VALONA,                ROAD},
	{SANTANDER,             SARAGOSSA,             ROAD},
	{SARAGOSSA,             TOULOUSE,              ROAD},
	{SARAJEVO,              SOFIA,                 ROAD},
	{SARAJEVO,              ST_JOSEPH_AND_ST_MARY, ROAD},
	{SARAJEVO,              VALONA,                ROAD},
	{SARAJEVO,              ZAGREB,                ROAD},
	{SOFIA,                 VALONA,                ROAD},
	{SOFIA,                 VARNA,                 ROAD},
	{STRASBOURG,            ZURICH,                ROAD},
	{ST_JOSEPH_AND_ST_MARY, SZEGED,                ROAD},
	{ST_JOSEPH_AND_ST_MARY, ZAGREB,                ROAD},
	{SZEGED,                ZAGREB,                ROAD},
	{VIENNA,                ZAGREB,                ROAD},

	//### RAIL Connections ###
	{ALICANTE,   BARCELONA,  RAIL},
	{ALICANTE,   MADRID,     RAIL},
	{BARCELONA,  SARAGOSSA,  RAIL},
	{BARI,       NAPLES,     RAIL},
	{BELGRADE,   SOFIA,      RAIL},
	{BELGRADE,   SZEGED,     RAIL},
	{BERLIN,     HAMBURG,    RAIL},
	{BERLIN,     LEIPZIG,    RAIL},
	{BERLIN,     PRAGUE,     RAIL},
	{BORDEAUX,   PARIS,      RAIL},
	{BORDEAUX,   SARAGOSSA,  RAIL},
	{BRUSSELS,   COLOGNE,    RAIL},
	{BRUSSELS,   PARIS,      RAIL},
	{BUCHAREST,  CONSTANTA,  RAIL},
	{BUCHAREST,  GALATZ,     RAIL},
	{BUCHAREST,  SZEGED,     RAIL},
	{BUDAPEST,   SZEGED,     RAIL},
	{BUDAPEST,   VIENNA,     RAIL},
	{COLOGNE,    FRANKFURT,  RAIL},
	{EDINBURGH,  MANCHESTER, RAIL},
	{FLORENCE,   MILAN,      RAIL},
	{FLORENCE,   ROME,       RAIL},
	{FRANKFURT,  LEIPZIG,    RAIL},
	{FRANKFURT,  STRASBOURG, RAIL},
	{GENEVA,     MILAN,      RAIL},
	{GENOA,      MILAN,      RAIL},
	{LEIPZIG,    NUREMBURG,  RAIL},
	{LE_HAVRE,   PARIS,      RAIL},
	{LISBON,     MADRID,     RAIL},
	{LIVERPOOL,  MANCHESTER, RAIL},
	{LONDON,     MANCHESTER, RAIL},
	{LONDON,     SWANSEA,    RAIL},
	{MADRID,     SANTANDER,  RAIL},
	{MADRID,     SARAGOSSA,  RAIL},
	{MARSEILLES, PARIS,      RAIL},
	{MILAN,      ZURICH,     RAIL},
	{MUNICH,     NUREMBURG,  RAIL},
	{NAPLES,     ROME,       RAIL},
	{PRAGUE,     VIENNA,     RAIL},
	{SALONICA,   SOFIA,      RAIL},
	{SOFIA,      VARNA,      RAIL},
	{STRASBOURG, ZURICH,     RAIL},
	{VENICE,     VIENNA,     RAIL},

	//### BOAT Connections ###
	{ADRIATIC_SEA,      BARI,              BOAT},
	{ADRIATIC_SEA,      IONIAN_SEA,        BOAT},
	{ADRIATIC_SEA,      VENICE,            BOAT},
	{ALICANTE,          MEDITERRANEAN_SEA, BOAT},
	{AMSTERDAM,         NORTH_SEA,         BOAT},
	{ATHENS,            IONIAN_SEA,        BOAT},
	{ATLANTIC_OCEAN,    BAY_OF_BISCAY,     BOAT},
	{ATLANTIC_OCEAN,    CADIZ,             BOAT},
	{ATLANTIC_OCEAN,    ENGLISH_CHANNEL,   BOAT},
	{ATLANTIC_OCEAN,    GALWAY,            BOAT},
	{ATLANTIC_OCEAN,    IRISH_SEA,         BOAT},
	{ATLANTIC_OCEAN,    LISBON,            BOAT},
	{ATLANTIC_OCEAN,    MEDITERRANEAN_SEA, BOAT},
	{ATLANTIC_OCEAN,    NORTH_SEA,         BOAT},
	{BARCELONA,         MEDITERRANEAN_SEA, BOAT},
	{BAY_OF_BISCAY,     BORDEAUX,          BOAT},
	{BAY_OF_BISCAY,     NANTES,            BOAT},
	{BAY_OF_BISCAY,     SANTANDER,         BOAT},
	{BLACK_SEA,         CONSTANTA,         BOAT},
	{BLACK_SEA,         IONIAN_SEA,        BOAT},
	{BLACK_SEA,         VARNA,             BOAT},
	{CAGLIARI,          MEDITERRANEAN_SEA, BOAT},
	{CAGLIARI,          TYRRHENIAN_SEA,    BOAT},
	{DUBLIN,            IRISH_SEA,         BOAT},
	{EDINBURGH,         NORTH_SEA,         BOAT},
	{ENGLISH_CHANNEL,   LE_HAVRE,          BOAT},
	{ENGLISH_CHANNEL,   LONDON,            BOAT},
	{ENGLISH_CHANNEL,   NORTH_SEA,         BOAT},
	{ENGLISH_CHANNEL,   PLYMOUTH,          BOAT},
	{GENOA,             TYRRHENIAN_SEA,    BOAT},
	{HAMBURG,           NORTH_SEA,         BOAT},
	{IONIAN_SEA,        SALONICA,          BOAT},
	{IONIAN_SEA,        TYRRHENIAN_SEA,    BOAT},
	{IONIAN_SEA,        VALONA,            BOAT},
	{IRISH_SEA,         LIVERPOOL,         BOAT},
	{IRISH_SEA,         SWANSEA,           BOAT},
	{MARSEILLES,        MEDITERRANEAN_SEA, BOAT},
	{MEDITERRANEAN_SEA, TYRRHENIAN_SEA,    BOAT},
	{NAPLES,            TYRRHENIAN_SEA,    BOAT},
	{ROME,              TYRRHENIAN_SEA,    BOAT},

	// Sentinel
	{-1, -1, ANY}
};

////////////////////////////////////////////////////////////////////////

PlaceId placeNameToId(char *name)
{
	// binary search
	PlaceId lo = MIN_REAL_PLACE;
	PlaceId hi = MAX_REAL_PLACE;
	while (lo <= hi) {
		PlaceId mid = (hi + lo) / 2;
		int ord = strcmp(name, PLACES[mid].name);
		if (ord == 0) return PLACES[mid].id;
		if (ord <  0) hi = mid - 1;
		if (ord >  0) lo = mid + 1;
	}

	return NOWHERE;
}

PlaceId placeAbbrevToId(char *abbrev)
{
	for (PlaceId i = MIN_REAL_PLACE; i <= MAX_REAL_PLACE; i++) {
		if (strcmp(PLACES[i].abbrev, abbrev) == 0) {
			return PLACES[i].id;
		}
	}
	
    if (strcmp("C?", abbrev) == 0) {
        return CITY_UNKNOWN;
    } else if (strcmp("S?", abbrev) == 0) {
        return SEA_UNKNOWN;
    } else if (strcmp("HI", abbrev) == 0) {
        return HIDE;
    } else if (strcmp("D1", abbrev) == 0) {
        return DOUBLE_BACK_1;
    } else if (strcmp("D2", abbrev) == 0) {
        return DOUBLE_BACK_2;
    } else if (strcmp("D3", abbrev) == 0) {
        return DOUBLE_BACK_3;
    } else if (strcmp("D4", abbrev) == 0) {
        return DOUBLE_BACK_4;
    } else if (strcmp("D5", abbrev) == 0) {
        return DOUBLE_BACK_5;
    } else if (strcmp("TP", abbrev) == 0) {
        return TELEPORT;
    }
    
    return NOWHERE;
}

////////////////////////////////////////////////////////////////////////

PlaceType placeIdToType(PlaceId pid)
{
	if (placeIsReal(pid)) {
		return PLACES[pid].type;
	} else if (pid == CITY_UNKNOWN) {
		return  LAND;
	} else if (pid == SEA_UNKNOWN) {
		return SEA;
	} else {
		return UNKNOWN;
	}
}


const char *placeIdToName(PlaceId pid)
{
	if (placeIsReal(pid)) {
		return PLACES[pid].name;
	}
	
	switch (pid) {
		case CITY_UNKNOWN:  return "Unknown City";
		case SEA_UNKNOWN:   return "Unknown Sea";
		case HIDE:          return "Hide";
		case DOUBLE_BACK_1: return "Double Back 1";
		case DOUBLE_BACK_2: return "Double Back 2";
		case DOUBLE_BACK_3: return "Double Back 3";
		case DOUBLE_BACK_4: return "Double Back 4";
		case DOUBLE_BACK_5: return "Double Back 5";
		case TELEPORT:      return "Teleport";
		default:            return "Unknown Place";
	}
}

const char *placeIdToAbbrev(PlaceId pid)
{
	if (placeIsReal(pid)) {
		return PLACES[pid].abbrev;
	}
	
	switch (pid) {
		case CITY_UNKNOWN:  return "C?";
		case SEA_UNKNOWN:   return "S?";
		case HIDE:          return "HI";
		case DOUBLE_BACK_1: return "D1";
		case DOUBLE_BACK_2: return "D2";
		case DOUBLE_BACK_3: return "D3";
		case DOUBLE_BACK_4: return "D4";
		case DOUBLE_BACK_5: return "D5";
		case TELEPORT:      return "TP";
		default:            return "??";
	}
}

////////////////////////////////////////////////////////////////////////

const char *transportTypeToString(TransportType t)
{
	switch (t) {
		case ROAD: return "road";
		case RAIL: return "rail";
		case BOAT: return "boat";
		case NONE:
		case ANY:
		default:   return "????";
	}
}
