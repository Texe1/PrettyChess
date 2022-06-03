#pragma once
#include <stdlib.h>

#define PIECE_TYPE(piece) (piece & 0b111)
#define IS_BLACK(piece) (piece & 1 << 7)
#define PIECE_TYPE(piece) (piece & 0b111)
#define IS_SELECTED(piece) (((piece) & 1 << 6) != 0)
#define EN_PASSANT(piece) ((piece) & (1 << 5))

typedef struct {
	char xDir;
	char yDir;
	unsigned char
		flipX	: 1,
		flipY	: 1,
		init	: 1,
		jump	: 1;

	unsigned char
		maxRep	: 3,
		minRep	: 3,
		mustCap	: 1,
		cantCap	: 1;


} MOVE_TEMPLATE;

typedef struct {
	unsigned short
		x0		: 3,
		y0		: 3,
		x1		: 3,
		y1		: 3,
		cap		: 1,
		valid	: 1;
} MOVE;

typedef struct {
	unsigned char nMoves;
	MOVE_TEMPLATE* moves;
	char* name;
	char abbreviation;
} PIECE_TEMPLATE;

typedef struct {
	PIECE_TEMPLATE* ptemplate;
	unsigned char
		x		: 3,
		y		: 3,
		col		: 1,
		moved	: 1;
} PIECE;

MOVE* getPossibleMoves(PIECE* p, void* pBoard);

inline int getMaxMoveCount(MOVE_TEMPLATE*);
