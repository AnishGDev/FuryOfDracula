////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.h: the Game View ADT
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

#ifndef FOD__GAME_VIEW_H_
#define FOD__GAME_VIEW_H_

#include <stdbool.h>

#include "Game.h"
#include "Places.h"
// add your own #includes here

typedef struct gameView *GameView;

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
GameView GvNew(char *pastPlays, Message messages[]);

/**
 * Frees all memory allocated for `gv`.
 * After this has been called, `gv` should not be accessed.
 */
void GvFree(GameView gv);

////////////////////////////////////////////////////////////////////////
// Game State Information

/**
 * Gets the current round number
 */
Round GvGetRound(GameView gv);

/**
 * Gets the current player; i.e., whose turn is it?
 */
Player GvGetPlayer(GameView gv);

/**
 * Gets the current game score; a positive integer between 0 and 366.
 */
int GvGetScore(GameView gv);

/**
 * Gets the current health points for the given player; an value between
 * 0 and 9 for a hunter, or >= 0 for Dracula.
 */
int GvGetHealth(GameView gv, Player player);

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
PlaceId GvGetPlayerLocation(GameView gv, Player player);

/**
 * Gets the location of the sleeping immature vampire.
 *
 * This function should return:
 * - NOWHERE  if the immature vampire does not exist (i.e., if it hasn't
 *   been spawned, or if it has already matured or been vanquished).
 * - A  PlaceId corresponding to a real location, if the location of the
 *   immature vampire has been revealed in the play string.  This  means
 *   Dracula's  location during the round in which he placed an immature
 *   vampire was revealed.
 * - Otherwise,  CITY_UNKNOWN.  Note that this function can never return
 *   SEA_UNKNOWN, since Dracula never places encounters at sea.
 */
PlaceId GvGetVampireLocation(GameView gv);

/**
 * Gets the locations of all active traps.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numTraps to the number of  traps.  The  array  should
 * contain  multiple  copies  of the same location if there are multiple
 * traps in that location. The locations can be in any order.
 *
 * NOTE: This  function  may  do  whatever  you  feel is suitable if the
 *       current player is a hunter, as it is  impossible  to  get  100%
 *       right  if  you do not know where Dracula has been. We will only
 *       be testing this function when Dracula is the current player.
 */
PlaceId *GvGetTrapLocations(GameView gv, int *numTraps);

////////////////////////////////////////////////////////////////////////
// Game History

/**
 * Gets  the complete move history, in chronological order, of the given
 * player as shown in the play string.
 *
 * This  function  should  return  the  moves in a dynamically allocated
 * array, and set *numReturnedMoves to the number of moves returned.
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 */
PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree);

/**
 * Gets  the given player's last `numMoves` moves in chronological order
 * as shown in the play string.
 *
 * This  function  should  return  the  moves in a dynamically allocated
 * array, and set *numReturnedMoves to the number of moves returned  (in
 * case  `numMoves`  is  greater than the number of moves the player has
 * actually made).
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 *
 * NOTE: This  function is very similar to GvGetMoveHistory, except that
 *       it gets only the last `numMoves` moves rather than the complete
 *       move history.
 */
PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree);

/**
 * Gets  the  complete  location history, in chronological order, of the
 * given player.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of locations returned.
 *
 * If  the given player is a hunter, this function should behave exactly
 * the same as GvGetMoveHistory. If the given  player  is  Dracula,  the
 * PlaceIds  returned  should  either  correspond  to real places, or be
 * CITY_UNKNOWN or SEA_UNKNOWN. See the comment for  GvGetPlayerLocation
 * for more details.
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 */
PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree);

/**
 * Gets  the  given   player's last `numLocs` locations in chronological
 * order.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of  locations  returned
 * (in  case  `numLocs` is greater than the number of  moves  the player
 * has actually made).
 *
 * This  function can decide whether the caller is allowed to  modify or
 * free the returned array. (If the array is part of the  GameView  data
 * structure,  you may not want the caller to modify or free it.) If the
 * returned array can be modified/freed, set *canFree to true  to  avoid
 * memory leaks. Otherwise, set it to false.
 *
 * NOTE: This function is very similar to  GvGetLocationHistory,  except
 *       that  it gets only the last `numLocs` locations rather than the
 *       complete location history.
 */
PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree);

////////////////////////////////////////////////////////////////////////
// Making a Move

/**
 * Gets all the locations that can be reached by the given player in the
 * given round, assuming that they are at the given location.
 *
 * This  function should return the locations in a dynamically allocated
 * array, and set *numReturnedLocs to the number of returned  locations.
 * The  array  can  be in any order but must contain unique entries. The
 * array should include the given location.
 *
 * If  the  given  player is a hunter, the function should use the given
 * round number for determining how far the player can travel by rail.
 *
 * If  the  given player is Dracula, the function must take into account
 * the fact that Dracula may not travel by rail or move to the  Hospital
 * of  St  Joseph  and  St  Mary. But it shouldn't take into account the
 * trail restriction.
 *
 * This  function  can  assume that the given player has already made at
 * least one move.
 */
PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs);

/**
 * Similar  to GvGetReachable, but the caller can restrict the transport
 * types to be considered. For example, if road and rail are  true,  but
 * boat is false, boat connections will be ignored.
 */
PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs);

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO

#endif // !defined (FOD__GAME_VIEW_H_)
