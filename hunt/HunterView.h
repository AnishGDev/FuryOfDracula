////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.h: the HunterView ADT
//
// You may augment this with as many of your own functions as you like,
// but do NOT remove or modify any of the existing function signatures,
// otherwise, your code will not be able to compile with our tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-11-30	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#ifndef FOD__HUNTER_VIEW_H_
#define FOD__HUNTER_VIEW_H_

#include <stdbool.h>

#include "Game.h"
#include "Places.h"
// add your own #includes here

typedef struct hunterView *HunterView;

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

/**
 * Creates a new view to summarise the current state of the game.
 *
 * @param pastPlays - A string of all the plays made in the game so far.
 *                    See the spec for details of the format. The string
 *                    can be assumed to be valid.
 *
 * @param messages - An  array containing a message for each play in the
 *                   game so far. It will have exactly the  same  number
 *                   of  elements as there are plays in `pastPlays`. The
 *                   message from the first play will be at index 0, and
 *                   so on. The contents of each message will be exactly
 *                   as provided by the player.
 *
 * The "Message" type is defined in Game.h.
 * You are free to ignore messages if you wish.
 */
HunterView HvNew(char *pastPlays, Message messages[]);

/**
 * Frees all memory allocated for `hv`.
 * After this has been called, `hv` should not be accessed.
 */
void HvFree(HunterView hv);

////////////////////////////////////////////////////////////////////////
// Game State Information

/**
 * Get the current round number
 */
Round HvGetRound(HunterView hv);

/**
 * Get the current player; i.e., whose turn is it?
 */
Player HvGetPlayer(HunterView hv);

/**
 * Gets the current game score; a positive integer between 0 and 366.
 */
int HvGetScore(HunterView hv);

/**
 * Gets the current health points for the given player; an value between
 * 0 and 9 for a hunter, or >= 0 for Dracula.
 */
int HvGetHealth(HunterView hv, Player player);

/**
 * Gets the current location of a given player.
 *
 * This function should return:
 * - NOWHERE if the given player has not had a turn yet.
 * - Otherwise, a PlaceId corresponding to a real location, if the given
 *   player  is  a hunter. Note that if the hunter was teleported to the
 *   Hospital of St Joseph and St Mary, ST_JOSEPH_AND_ST_MARY should  be
 *   returned.
 * - Otherwise, a PlaceId corresponding to a real location, if the given
 *   player is Dracula and his current location has been revealed in the
 *   play string. Note that Dracula's location could be revealed even if
 *   his  latest move was a HIDE or DOUBLE_BACK, if these moves refer to
 *   a previous LOCATION move (see the spec for details). Also note that
 *   if Dracula's latest move was TELEPORT, this means he is  in  Castle
 *   Dracula.
 * - Otherwise, CITY_UNKNOWN if Dracula is in a city, and SEA_UNKNOWN if
 *   Dracula is at sea.
 * 
 * This  function should never return HIDE or DOUBLE_BACK - if Dracula's
 * latest move was a HIDE or DOUBLE_BACK, it should still be able to  be
 * resolved to a city or sea.
 */
PlaceId HvGetPlayerLocation(HunterView hv, Player player);

/**
 * Gets the location of the sleeping immature vampire.
 *
 * This function should return:
 * - NOWHERE  if the immature vampire does not exist (i.e., if it hasn't
 *   been spawned, or if it has already matured or been vanquished).
 * - A PlaceId corresponding to a real location, if the location of  the
 *   immature  vampire  has been revealed in the play string. This means
 *   Dracula's location during the round in which he placed an  immature
 *   vampire was revealed.
 * - Otherwise,  CITY_UNKNOWN.  Note that this function can never return
 *   SEA_UNKNOWN, since Dracula never places encounters at sea.
 */
PlaceId HvGetVampireLocation(HunterView hv);

////////////////////////////////////////////////////////////////////////
// Utility Functions

/**
 * Gets  Dracula's  last  known  real  location  as revealed in the play
 * string and sets *round to the number of the  latest  round  in  which
 * Dracula moved there.
 *
 * If Dracula's location has never been reveled in the play string, this
 * function should return NOWHERE and leave *round as it is.
 */
PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round);

/**
 * Gets  the  shortest  path from the given hunter's current location to
 * the given location, taking into account all connection types and  the
 * fact that hunters can only move once per round.
 *
 * This function should store the path in a dynamically allocated array,
 * and set *pathLength to the length of the path. If there are  multiple
 * shortest  paths, any of them can be returned. The path should exclude
 * the hunter's current location.
 *
 * EXAMPLE: If  the  hunter  is currently at Cadiz (CA) and the shortest
 *          path to Barcelona (BA) was Cadiz -> Madrid -> Barcelona, the
 *          returned array should contain just [Madrid,  Barcelona]  and
 *          *pathLength should be set to 2.
 *
 * NOTE: Since  this function will require a traversal of the map, which
 *       is a lot of work for just a single path, you may  want to store
 *       the  result of the traversal in your HunterView data structure,
 *       so that future calls to this function with the same player will
 *       be less expensive.
 */
PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength);

////////////////////////////////////////////////////////////////////////
// Making a Move

/**
 * Gets  all  the  locations  that the current hunter player can move to
 * this turn.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of locations  returned.
 * The  array  can  be in any order but must contain unique entries. The
 * returned array should include the player's current location.
 *
 * If the current player hasn't made a move yet, set *numReturnedLocs to
 * 0 and return NULL.
 */
PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs);

/**
 * Similar  to  HvWhereCanIGo, but the caller can restrict the transport
 * types to be considered. For example, if road and rail are  true,  but
 * boat is false, boat connections will be ignored.
 */
PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs);

/**
 * Gets  all  the  locations  that the given player can move to in their
 * next move (for the current player that is this turn).
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of locations  returned.
 * The  array  can  be in any order but must contain unique entries. The
 * returned array should include the player's current location.
 *
 * If  the given player is a hunter, the function must take into account
 * the round number of the player's next move for  determining  how  far
 * the player can travel by rail.
 *
 * If  the  given player is Dracula, the function must take into account
 * the fact that Dracula may not travel by rail or move to the  Hospital
 * of  St  Joseph  and  St  Mary. But it shouldn't take into account the
 * trail restriction. If Dracula's current location is not revealed, the
 * function should set *numReturnedLocs to 0 and return NULL.
 *
 * If the given player hasn't made a move yet, set *numReturnedLocs to 0
 * and return NULL.
 */
PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs);

/**
 * Similar to HvWhereCanTheyGo but the caller can restrict the transport
 * types  to  be considered. For example, if road and rail are true, but
 * boat is false, boat connections will be ignored.
 */
PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs);

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO

#endif // !defined (FOD__HUNTER_VIEW_H_)
