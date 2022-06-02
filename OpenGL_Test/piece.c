#include "piece.h"

MOVE* getPossibleMoves(PIECE* p) {
	// calculating maximum possible moves and allocating heap space
	unsigned int maxMoves = 0;
	MOVE_TEMPLATE* mt = p->ptemplate->moves;
	int i = 0;
	while ((mt = p->ptemplate->moves + i)->valid) {
		maxMoves += getMaxMoveCount(&mt);
		i++;
	}

	MOVE* moves = malloc((maxMoves + 1) * sizeof(MOVE));
	if (!moves)
		return 0;

	// getting all possible moves
	i = 0;
	unsigned int j = 0;
	MOVE m = { 0 };
	m.x0 = p->x;
	m.y0 = p->y;
	while ((mt = p->ptemplate->moves + i++)->valid) {
		int dx = mt->minRep * mt->xDir;
		int dy = mt->minRep * mt->yDir;

		for (int i = mt->minRep; i <= mt->maxRep; i++)
		{
			m.x1 = p->x + dx;
			m.y1 = p->y + dy;

			moves[j++] = m;

			if (mt->flipX) {
				m.x1 = p->x - dx;
				moves[j++] = m;
			}
			if (mt->flipY) {
				m.x1 = p->x + dx;
				m.y1 = p->y - dy;
				moves[j++] = m;

				if (mt->flipX) {
					m.x1 = p->x - dx;
					moves[j++] = m;
				}
			}

			dx += mt->xDir;
			dy += mt->yDir;
		}
	}

	moves[j].valid = 0;

}

inline int getMaxMoveCount(MOVE_TEMPLATE* templ) {
	return templ->maxRep * (templ->flipX ? 2 : 1) * (templ->flipY ? 2 : 1);
}