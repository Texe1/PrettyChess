#pragma once
#include "game.h"
#include "piece.h"

typedef struct cheapBoard {
	GAME game;
	char squares[64];
	char turn;
} CHEAP_BOARD;

MOVE chooseComputerMove(_BOARD*);

MOVE miniMax(_BOARD* b, int depth);

int eval(_BOARD* b);
