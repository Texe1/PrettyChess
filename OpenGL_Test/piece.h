#pragma once
#include <stdlib.h>

#define PIECE_TYPE(piece) (piece & 0b111)
#define IS_BLACK(piece) (piece & 1 << 7)
#define PIECE_TYPE(piece) (piece & 0b111)
#define IS_SELECTED(piece) (((piece) & 1 << 6) != 0)
#define EN_PASSANT(piece) ((piece) & (1 << 5))

typedef struct MOVE_TEMPLATE {
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

	int preX;
	int preY;


} MOVE_TEMPLATE;

typedef struct MOVE {
	unsigned short
		x0		: 3,
		y0		: 3,
		x1		: 3,
		y1		: 3,
		cap		: 1,
		funny	: 1,
		valid	: 1;
} MOVE;

typedef struct MoveContainer {
	MOVE* moves;
	unsigned long long nMoves;
} MOVE_CONTAINER;

typedef struct PIECE_TEMPLATE {
	unsigned char nMoves;
	MOVE_TEMPLATE* moves;
	char* name;
	char abbreviation;
	unsigned char 
		king		: 1,
		multiKing	: 1, // for funny game modes with multiple King pieces :)
		evolveable	: 1,
		evolveAtX	: 4, // 1 << 3 for arbitrary
		evolveAtY	: 4; // 1 << 3 for arbitrary

	unsigned long evolveInto;
} PIECE_TEMPLATE;

typedef struct PIECE {
	PIECE_TEMPLATE* ptemplate;
	unsigned short
		x		: 3,
		y		: 3,
		col		: 1,
		moved	: 1,
		present : 1;
} PIECE;

MOVE_CONTAINER getPossibleMoves(PIECE* p, void* pBoard, char checkCheck, char raw);

inline int getMaxMoveCount(MOVE_TEMPLATE*);

int _move(void*, MOVE*, char save);

void evolve(void* board, int type);

int isInCheckLine(int x, int y, void* cl, char infRep);

MOVE_CONTAINER getAllMoves(void* b);

void createCheckLine(void* b, int pieceIndex, int _x, int _y);

void createCheckLineTarget(void* b, int _x, int _y, int col);

void clearCheckLines(void* b, int _x, int _y);

void clearCheckLineTarget(void* b, int _x, int _y, int col);