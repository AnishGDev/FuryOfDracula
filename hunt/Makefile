########################################################################
# COMP2521 20T2 // the Fury of Dracula // the Hunt
# hunt/Makefile: build your Dracula and Hunter AIs
#
# 2018-12-31	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
# 2020-07-10	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>

CC = gcc
CFLAGS = -Wall -Werror

# do not change the following line
BINS = dracula hunter

# add any other *.o files that your solution requires
# (and add their dependencies below after DraculaView.o)
# if you're not using Map.o or Places.o, you can remove them
OBJS = GameView.o Map.o Places.o

# add whatever system libraries you need here (e.g. -lm)
LIBS =

all: $(BINS)

dracula: playerDracula.o dracula.o DraculaView.o Queue.o $(OBJS) $(LIBS)
hunter: playerHunter.o hunter.o HunterView.o Queue.o $(OBJS) $(LIBS)

playerDracula.o: player.c dracula.h Game.h DraculaView.h GameView.h Places.h Queue.h
	$(CC) $(CFLAGS) -DI_AM_DRACULA -c $< -o $@
playerHunter.o: player.c hunter.h Game.h HunterView.h GameView.h Places.h Queue.h
	$(CC) $(CFLAGS) -c $< -o $@

Queue.o: Queue.c Queue.h
dracula.o: dracula.c dracula.h DraculaView.h GameView.h Places.h Game.h
hunter.o: hunter.c hunter.h HunterView.h GameView.h Places.h Game.h
GameView.o:	GameView.c GameView.h Places.h Game.h
DraculaView.o: DraculaView.c DraculaView.h GameView.h Places.h Game.h
HunterView.o: HunterView.c GameView.h Places.h HunterView.h Game.h
Places.o: Places.c Places.h

# if you use other ADTs, add dependencies for them here

.PHONY: clean
clean:
	-rm -f $(BINS) *.o core
