#pragma once
#include "piece.h"

typedef struct {
	int nPieceTypes;
	PIECE_TEMPLATE* pieceTypes;
	unsigned char startPosition[64];
} GAME;

typedef struct {
	GAME game;
	unsigned char squares[64];
	MOVE* moves;
} BOARD;

BOARD* createStdBoard();