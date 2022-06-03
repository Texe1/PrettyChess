#include "piece.h"
#include "game.h"

MOVE* getPossibleMoves(PIECE* p, void* pBoard) {
	_BOARD* board = (_BOARD*)pBoard;

	// calculating maximum possible moves and allocating heap space
	unsigned int maxMoves = 0;
	MOVE_TEMPLATE* mt = p->ptemplate->moves;
	int i = 0;
	while (i < p->ptemplate->nMoves) {
		mt = p->ptemplate->moves + i++;
		maxMoves += getMaxMoveCount(mt);
	}

	MOVE* funnyMoves = board->game.funnyMoves(p, board);
	int nFunnyMoves = 0;

	if (funnyMoves) {
		MOVE m;
		while ((m = funnyMoves[nFunnyMoves++]).valid);
	}

	MOVE* moves = malloc((maxMoves + nFunnyMoves + 1) * sizeof(MOVE));
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
		int dx = mt->xDir;
		int dy = !p->col ? mt->yDir : (-mt->yDir);

		char std = 1;
		char flipX = mt->flipX;
		char flipY = mt->flipY;
		char flipXY = flipX && flipY;

		for (int k = 1; k <= mt->maxRep; k++)
		{

			if (mt->init && p->moved)
				continue;

			unsigned char xPlus = ((((int)p->x + (int)dx) >= 0) && (((int)p->x + (int)dx) < 8));
			unsigned char xMinus = ((((int)p->x - (int)dx) >= 0) && (((int)p->x - (int)dx) < 8));
			unsigned char yPlus = ((((int)p->y + (int)dy) >= 0) && (((int)p->y + (int)dy) < 8));
			unsigned char yMinus = ((((int)p->y - (int)dy) >= 0) && (((int)p->y - (int)dy) < 8));

			if (!flipX && !flipY && !flipXY && !std)
				break;

			if (std && xPlus && yPlus) {
				m.x1 = p->x + dx;
				m.y1 = p->y + dy;

				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if ((board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col) && !mt->cantCap && k >= mt->minRep) { // capturing
						m.cap = 1;
						moves[j++] = m;
						m.cap = 0;
					}
					std = 0;
				}
				else if(!mt->mustCap && k >= mt->minRep) {
					moves[j++] = m;
				}
			}

			if (flipX && xMinus && yPlus) { // flipped in X
				m.x1 = p->x - dx;
				
				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k >= mt->minRep) {// capturing
						m.cap = 1;
						moves[j++] = m;
						m.cap = 0;
					}
					flipX = 0;
				}
				else if (!mt->mustCap && k >= mt->minRep) {
					moves[j++] = m;
				}
			}
			if (flipY && yMinus) { // flipped in Y
				if (xPlus) {
					
					m.x1 = p->x + dx;
					m.y1 = p->y - dy;

					if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
						if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k >= mt->minRep) {// capturing
							m.cap = 1;
							moves[j++] = m;
							m.cap = 0;
						}
						flipY = 0;
					}
					else if (!mt->mustCap && k >= mt->minRep) {
						moves[j++] = m;
					}
				}
			}
			if (flipXY && xMinus && yMinus) {
				m.x1 = p->x - dx;
				m.y1 = p->y - dy;
				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k < mt->minRep) {
						m.cap = 1;
						moves[j++] = m;
						m.cap = 0;
					}
					flipXY = 0;
				}
				else if (!mt->mustCap) {
					moves[j++] = m;
				}
			}

			dx += mt->xDir;
			dy += !p->col ? mt->yDir : (-mt->yDir);
		}
	}

	if (funnyMoves) {
		for (size_t k = 0; k < nFunnyMoves; k++)
		{
			moves[j++] = funnyMoves[k];
		}
		free(funnyMoves);
	}

	moves[j].valid = 0;

	return moves;

}

inline int getMaxMoveCount(MOVE_TEMPLATE* templ) {
	return templ->maxRep * (templ->flipX ? 2 : 1) * (templ->flipY ? 2 : 1);
}