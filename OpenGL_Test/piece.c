#include "piece.h"

MOVE* getPossibleMoves(PIECE* p) {
	// calculating maximum possible moves and allocating heap space
	unsigned int maxMoves = 0;
	MOVE_TEMPLATE* mt = p->ptemplate->moves;
	int i = 0;
	while (i < p->ptemplate->nMoves) {
		mt = p->ptemplate->moves + i++;
		maxMoves += getMaxMoveCount(mt);
	}

	MOVE* moves = malloc((maxMoves + 1) * sizeof(MOVE));
	if (!moves)
		return 0;

	// getting all possible moves
	i = 0;
	unsigned int j = 0;
	MOVE m = { 0 };
	m.valid = 1;
	m.x0 = p->x;
	m.y0 = p->y;
	while (i < p->ptemplate->nMoves) {
		mt = p->ptemplate->moves + i++;
		int dx = mt->minRep * mt->xDir;
		int dy = mt->minRep * mt->yDir;

		for (int k = mt->minRep; k <= mt->maxRep; k++)
		{

			unsigned char xPlus = ((((int)p->x + (int)dx) >= 0) && (((int)p->x + (int)dx) < 8));
			unsigned char xMinus = ((((int)p->x - (int)dx) >= 0) && (((int)p->x - (int)dx) < 8));
			unsigned char yPlus = ((((int)p->y + (int)dy) >= 0) && (((int)p->y + (int)dy) < 8));
			unsigned char yMinus = ((((int)p->y - (int)dy) >= 0) && (((int)p->y - (int)dy) < 8));

			if (xPlus && yPlus) {
				m.x1 = p->x + dx;
				m.y1 = p->y + dy;
				moves[j++] = m;
			}

			if (mt->flipX && xMinus && yPlus) { // flipped in X
				m.x1 = p->x - dx;
				moves[j++] = m;
			}
			if (mt->flipY && yMinus) { // flipped in Y
				if (xPlus) {
					
					m.x1 = p->x + dx;
					m.y1 = p->y - dy;
					moves[j++] = m;
				}

				if (mt->flipX && xMinus) { // filpped in X and Y
					m.x1 = p->x - dx;
					moves[j++] = m;
				}
			}

			dx += mt->xDir;
			dy += mt->yDir;
		}
	}

	moves[j].valid = 0;

	return moves;

}

inline int getMaxMoveCount(MOVE_TEMPLATE* templ) {
	return templ->maxRep * (templ->flipX ? 2 : 1) * (templ->flipY ? 2 : 1);
}