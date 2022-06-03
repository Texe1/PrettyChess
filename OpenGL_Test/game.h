#pragma once
#include "piece.h"

struct _BOARD;

typedef struct GAME {
	int nPieceTypes;
	PIECE_TEMPLATE* pieceTypes;
	unsigned char startPosition[64];
	MOVE* (*funnyMoves)(PIECE*, struct _BOARD*);
} GAME;

typedef struct _BOARD {
	GAME game;
	unsigned char squares[64]; // these contain the index of their piece (with bit 7 set)
	unsigned int nMoves;
	MOVE* moves;
	unsigned char nPieces;
	PIECE* pieces;
} _BOARD;

_BOARD* createStdBoard();

void startGame(_BOARD* board);

void setStartingPos(GAME* game, const char* pos);

MOVE* funnyMovesStd(PIECE* piece, _BOARD* board);