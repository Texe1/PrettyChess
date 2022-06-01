#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define PAWN 1
#define BISHOP 2
#define KNIGHT 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define BLACK		0b10000000
#define SELECTED	0b01000000

#define IS_BLACK(piece) (((piece) & BLACK) != 0)
#define IS_SELECTED(piece) (((piece) & SELECTED) != 0)
#define PIECE_TYPE(piece) ((piece) & 0b111)
#define EN_PASSANT(piece) ((piece) & (1 << 5))



typedef struct Square {
	char piece; // black :1, selected :1, extra(pawn: en passant :1) :2, moved :1, type :3
	char color;
} SQUARE, Square;

struct CheckCheckLine;

typedef struct Board {
	SQUARE squares[64];
	char selected;
	char lastMove;

	unsigned long
		b_King : 1,
		b_Queens : 4,
		b_Knights : 4,
		b_Bishops : 4,
		b_Rooks : 4,
		b_Pawns : 4,

		w_King : 1,
		w_Queens : 4,
		w_Knights : 4,
		w_Bishops : 4,
		w_Rooks : 4,
		w_Pawns : 4,

		turn : 1,
		evolve : 1;

	char score;

	char blackKing;
	char whiteKing;


	char nCheckLines;
	struct CheckCheckLine* CheckLines;
} BOARD, Board;

typedef struct CheckCheckLine {
	unsigned long
		dir : 4, // direction
		len : 3, // length of the line (ignored for dir >= 8)
		clear : 1, // no pieces in between
		col : 1, // color of the threatened King
		direct : 4, // if it hits the king or neighbouring squares (and if, which)
		org : 6; // origin (source of all evil)
} CheckCheckLine, CHECKCHECKLINE, CHECKLINE, CheckLine, CheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckCheckLine, CCCCCCCCCCCCCCCCCCCCCCCCCCCLine;

void printBoard(BOARD* board);
void fillBoard(BOARD* board);
int selectInBoard(BOARD* board, int x, int y);
void deselectInBoard(BOARD* board);
int testMove(BOARD* board, int x0, int y0, int x1, int y1);
int move(BOARD* board, int x0, int y0, int x1, int y1);
int evolve(BOARD* board, int n);
int isInCheckLine(CheckLine* cl, int n);
int addrmCheckLines(BOARD* board, char nRemove, char* remove, char nAdd, CHECKLINE* add);