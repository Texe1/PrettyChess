#pragma once
#include "piece.h"

struct _BOARD;

typedef struct GAME {
	int nPieceTypes;
	PIECE_TEMPLATE* pieceTypes;
	unsigned char startPosition[64];
	MOVE* (*funnyMoves)(PIECE*, struct _BOARD*);
	void (*doFunnyMove)(PIECE*, struct _BOARD*, MOVE*);
	int (*isDraw)(struct _BOARD*);
} GAME;

typedef struct _BOARD {
	GAME game;
	unsigned char squares[64]; // these contain the index of their piece (with bit 7 set)
	unsigned int nMoves;
	MOVE* moves;
	unsigned char nPieces;
	PIECE* pieces;
	unsigned char
		evolve		: 1, // if evolve is possible
		evolveIndex	: 6; // on which square it is happening
	unsigned char end;
	unsigned char wh_pieceCount;
	unsigned char bl_pieceCount;

	unsigned long nPositions;
	unsigned char* positions;
} _BOARD;

_BOARD* createStdBoard();

void startGame(_BOARD* board);

void setStartingPos(GAME* game, const char* pos);

MOVE* funnyMovesStd(PIECE* piece, _BOARD* board);

void doFunnyMoveStd(PIECE*, struct _BOARD*, MOVE*);

int isDrawStd(struct _BOARD*);

void print_board(_BOARD* board, int y);

void savePos(_BOARD* board);

void freeBoard(_BOARD* board);