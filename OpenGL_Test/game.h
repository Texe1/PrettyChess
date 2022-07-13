#pragma once
#include "piece.h"

#include <string.h>

struct _BOARD;

typedef struct GAME {
	int nPieceTypes;
	PIECE_TEMPLATE* pieceTypes;
	unsigned char startPosition[64];
	MOVE_CONTAINER (*funnyMoves)(PIECE*, struct _BOARD*, char, char);
	void (*doFunnyMove)(struct _BOARD*, MOVE*);
	int (*isDraw)(struct _BOARD*);
} GMAE, GAME;

struct CheckLine;

typedef struct _BOARD {
	GAME game;
	unsigned char squares[64]; // these contain the index of their piece (with bit 7 set)
	unsigned int nMoves;
	MOVE* moves;
	unsigned char nPieces;
	PIECE* pieces;
	unsigned char
		evolve		: 1, // if evolve is possible
		evolveIndex	: 6, // on which square it is happening
		turn		: 1; 
	unsigned char end;
	unsigned char wh_pieceCount;
	unsigned char bl_pieceCount;

	unsigned char blackComputer; // 0 for human (different difficulties?)
	unsigned char whiteComputer; // 0 for human

	unsigned long nPositions;
	unsigned char* positions;

	int nCheckLines;
	struct CheckLine* checkLines;
} _BOARD;

typedef struct CheckLine {
	MOVE_TEMPLATE* mTemplate;
	MOVE move;
	unsigned short
		direct  : 1,
		flipX	: 1,
		flipY	: 1,
		reps	: 3,
		col		: 1, // 1 for black King
		check	: 1, // if the line is free
		nBtw	: 3; // how many pieces block the Line

} CHECKLINE;

_BOARD* createStdBoard();

void startGame(_BOARD* board);

void setStartingPos(GAME* game, const char* pos);

MOVE_CONTAINER funnyMovesStd(PIECE* piece, _BOARD* board, char checkCheck, char raw);

void doFunnyMoveStd(struct _BOARD*, MOVE*);

int isDrawStd(struct _BOARD*);

void print_board(_BOARD* board, int y);

void savePos(_BOARD* board);

void freeBoard(_BOARD* board);

void addCheckLine(_BOARD* board, CHECKLINE cl);

void removeCheckLine(_BOARD* board, int index);

void initCheckLines(_BOARD* board);

_BOARD* copyBoard(_BOARD* board);

void setBoard(_BOARD* dest, _BOARD* src);