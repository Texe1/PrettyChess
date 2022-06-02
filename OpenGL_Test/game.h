#pragma once
#include "piece.h"

typedef struct {
	int nPieceTypes;
	PIECE_TEMPLATE* pieceTypes;
	unsigned char startPosition[64];
} GAME;

typedef struct {
	GAME game;
	unsigned char squares[64]; // these contain the index of their piece (with bit 7 set)
	MOVE* moves;
	unsigned char nPieces;
	PIECE* pieces;
} _BOARD;

_BOARD* createStdBoard();