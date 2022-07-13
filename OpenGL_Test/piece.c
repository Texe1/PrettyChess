#include "piece.h"

#include "game.h"
#include <Windows.h>
#include <stdio.h>

MOVE_CONTAINER getPossibleMoves(PIECE* p, void* pBoard, char checkCheck, char raw) {
	_BOARD* board = (_BOARD*)pBoard;

	// calculating maximum possible moves and allocating heap space
	unsigned int maxMoves = 0;
	MOVE_TEMPLATE* mt = p->ptemplate->moves;
	int i = 0;
	while (i < p->ptemplate->nMoves) {
		mt = p->ptemplate->moves + i++;
		maxMoves += getMaxMoveCount(mt);
	}

	MOVE_CONTAINER funnyMoves = board->game.funnyMoves(p, board, checkCheck, raw);

	MOVE_CONTAINER moves = { 0 };

	moves.moves = malloc((maxMoves + funnyMoves.nMoves) * sizeof(MOVE));
	if (!moves.moves)
		return (MOVE_CONTAINER) { 0 };

	// getting all possible moves
	i = 0;
	unsigned int j = 0;
	MOVE m = (MOVE){
					.x0 = p->x,
					.y0 = p->y,
					.valid = 0,
					.cap = 0,
					.funny = 0
	};

	while (i < p->ptemplate->nMoves) {
		mt = p->ptemplate->moves + i++;

		if (p->moved && mt->init)
			continue;

		int xDir = mt->xDir;
		int yDir = (p->col) ? -mt->yDir : mt->yDir;

		char flips = 0b1 | (mt->flipX ? 2 : 0) | (mt->flipY ? 4 : 0) | ((mt->flipX && mt->flipY) ? 8 : 0);

		int dx = xDir;
		int dy = yDir;

		int x = p->x + mt->pre.dx;
		int y = p->y + (p->col ? -mt->pre.dy : mt->pre.dy);

		for (size_t j = 1; j <= mt->maxRep; j++)
		{
			if (flips == 0)
				break;

			for (size_t l = 0; l < 4; l++)
			{
				if (!(flips & (1 << l)))
					continue;

				int x1 = x + ((l & 1) ? -dx : dx);
				int y1 = y + ((l & 2) ? -dy : dy);

				if ((x1 > 7) || (x1 < 0) || (y1 > 7) || (y1 < 0)) {
					flips &= ~(1 << l);
					continue;
				}

				m.cap = 0;
				m.valid = 1;
				m.x0 = x;
				m.y0 = y;
				m.x1 = x1;
				m.y1 = y1;

				int squareIndex = m.y1 * 8 + m.x1;

				if (!raw) {
					if (board->squares[squareIndex]) {
						flips &= ~(1 << l);
						m.cap = 1;
						if (board->pieces[board->squares[squareIndex] & 0b111111].col == p->col || mt->cantCap) {
							m.valid = 0;
							continue;
						}
					}
					else
					{
						if (mt->mustCap) {
							m.valid = 0;
							continue;
						}
					}
				}

				// newest checkCheck
				if(!raw && checkCheck) {
					for (size_t k = 0; k < board->nCheckLines; k++)
					{
						CHECKLINE* cl = board->checkLines + k;

						// can't move into checkLine
						if (p->ptemplate->king) {
							if ((cl->move.x1 == m.x1) && (cl->move.y1 == m.y1) && (cl->nBtw == 0)){
								m.valid = 0;
								break;
							}
						}
						else {
							if (cl->col == p->col	// own king is threatened
								&& cl->nBtw == 0 && cl->direct // direct hit
								&& ((cl->move.x0 != m.x1) || (cl->move.y0 != m.y1)) // not capturing threatening piece
								&& (!isInCheckLine(m.x1, m.y1, cl))) { // destination not in checkLine
								m.valid = 0;
								break;
							}

							if ((cl->col == p->col) && isInCheckLine(p->x, p->y, cl) && !isInCheckLine(m.x1, m.y1, cl) && (cl->nBtw <= 1) && cl->direct // moving out of checkLine as only Piece
								&& ((cl->move.x0 != m.x1) || (cl->move.y0 != m.y1))) { // not capturing threatening piece
								m.valid = 0;
								break;
							}
						}
					}
				}

				if (m.valid) {
					moves.moves[moves.nMoves] = m;
					moves.nMoves++;
				}

			}

			dx += xDir;
			dy += yDir;
		}

	}

	// adding funnyMoves (TODO!)
	if (funnyMoves.moves) {
		for (size_t k = 0; k < funnyMoves.nMoves; k++)
		{
			moves.moves[moves.nMoves] = funnyMoves.moves[k];
			moves.nMoves++;
		}
		free(funnyMoves.moves);
	}

	// shifting moves so that there are no invalid moves
	/*for (size_t k = 0; k < moves.nMoves; k++)
	{
		if (!moves.moves[k].valid) {
			moves.nMoves--;
			for (size_t l = k; l < moves.nMoves; l++)
			{
				moves.moves[l] = moves.moves[l + 1];
			}
			k--;
		}
	}*/
	if (moves.nMoves == 0) {
		if(moves.moves)
			free(moves.moves);

		return (MOVE_CONTAINER) { 0 };
	}
	else
	{
		void* realloced = realloc(moves.moves, sizeof(MOVE) * moves.nMoves);
		if (realloced) {
			moves.moves = realloced;
			return moves;
		}
	}

}

MOVE_TEMPLATE* testMove(MOVE* move, PIECE_TEMPLATE* templ, char col) {
	for (size_t i = 0; i < templ->nMoves; i++)
	{
		MOVE_TEMPLATE* mTempl = templ->moves + i;
		if (testMoveOnTempl(move, mTempl, col))
			return mTempl;
	}

	move->valid = 0;
	return NULL;
}

int testMoveOnTempl(MOVE* move, MOVE_TEMPLATE* templ, char col) {
	if (move->cap && templ->cantCap)
		return 0;

	int dx = (int)move->x1 - (int)move->x0 - (int)templ->pre.dx;
	int dy = (int)move->y1 - (int)move->y0 - (int)templ->pre.dy;

	if (col) dy *= -1;

	if (dx == 0 && dy == 0)
		return 0;

	if (move->x1 >= 6) {
		int xdfjk = 0;
	}

	if (((dx == 0) != (templ->xDir == 0)) || ((dy == 0) != (templ->yDir == 0))
		|| (!templ->flipX && ((dx > 0) != (templ->xDir > 0))) 
		|| (!templ->flipY && ((dy > 0) != (templ->yDir > 0)))
		)
		return 0;

	dx = abs(dx);
	dy = abs(dy);


	int xDir = abs(templ->xDir);
	int yDir = abs(templ->yDir);

	if (((dx != 0) && (dx % xDir)) || ((dy != 0) && (dy % yDir)))
		return 0;

	int reps = (dx != 0) ? (dx / xDir) : (dy / yDir);
	if ((reps > templ->maxRep) || (reps < templ->minRep))
		return 0;

	if (dx != 0 && dy != 0 && ((dy / yDir) != reps))
		return 0;

	return 1;

}

inline int getMaxMoveCount(MOVE_TEMPLATE* templ) {
	return templ->maxRep * (templ->flipX ? 2 : 1) * (templ->flipY ? 2 : 1);
}

int _move(void* b, MOVE* m, char save) {

	_BOARD* board = (_BOARD*)b;

	int index = m->y0 * 8 + m->x0;
	int destIndex = m->y1 * 8 + m->x1;

	
	if (m->funny) {
		board->game.doFunnyMove(b, m);
	}
	else {
		if (!board->squares[m->y0 * 8 + m->x0])
			return 0;
		
		PIECE* piece = board->pieces + (board->squares[index] & 0b1111111);

		MOVE_CONTAINER movesBefore = { 0 };

		if (piece->ptemplate->king) {
			movesBefore = getPossibleMoves(piece, b, 0, 1);
		}
		
		// changing coords
		piece->x = m->x1;
		piece->y = m->y1;
		piece->moved = 1;

		// checking for capped piece
		if (m->cap) {
			PIECE* cappedPiece = board->pieces + (board->squares[m->y1 * 8 + m->x1] & 0b1111111);
			cappedPiece->present = 0;
			if (cappedPiece->col)
				board->bl_pieceCount--;
			else
				board->wh_pieceCount--;
		}

		board->squares[m->y1 * 8 + m->x1] =  (1 << 7) | (int)(piece - board->pieces);
		board->squares[m->y0 * 8 + m->x0] = 0;

		//evolving
		{
			PIECE_TEMPLATE* templ = piece->ptemplate;

			if (templ->evolveable
				&& ((templ->evolveAtX == (1 << 3)) || (templ->evolveAtX == piece->x))
				&& ((templ->evolveAtY == (1 << 3)) || (templ->evolveAtY == (piece->col ? (7 - piece->y) : piece->y)))) {
				board->evolve = 1;
				board->evolveIndex = piece->y * 8 + piece->x;
			}
		}

		// checkLines
		{
			for (size_t i = 0; i < board->nCheckLines; i++)
			{
				CHECKLINE* cl = board->checkLines + i;
				if (isInCheckLine(m->x0, m->y0, cl) && !((m->x0 == cl->move.x1) && (m->y0 == cl->move.y1))) {
					board->checkLines[i].nBtw--;
				}
				if (!m->cap && isInCheckLine(m->x1, m->y1, cl) && !((m->x1 == cl->move.x1) && (m->y1 == cl->move.y1))) {
					board->checkLines[i].nBtw++;
				}
			}

			clearCheckLines(b, m->x0, m->y0);
			if (m->cap) {
				clearCheckLines(b, m->x1, m->y1);
			}

			
			PIECE* king = board->pieces;
			while (!king->ptemplate->king || king->col == piece->col)
				king++;
			MOVE_CONTAINER kingMoves = getPossibleMoves(king, b, 0, 1);

			createCheckLine(b, (int)(piece - board->pieces), king->x, king->y);
			for (size_t i = 0; i < kingMoves.nMoves; i++)
			{
				createCheckLine(b, (int)(piece - board->pieces), kingMoves.moves[i].x1, kingMoves.moves[i].y1);
			}


			if (piece->ptemplate->king) {
				for (size_t i = 0; i < board->nCheckLines; i++)
				{
					if (board->checkLines[i].col == piece->col) {
						removeCheckLine(b, i);
					}
				}

				MOVE_CONTAINER moves = getPossibleMoves(piece, b, 0, 1);
				createCheckLineTarget(b, piece->x, piece->y, piece->col);
				for (size_t i = 0; i < moves.nMoves; i++)
				{
					createCheckLineTarget(b, moves.moves[i].x1, moves.moves[i].y1, piece->col);
				}

			}
		}
	}

	board->turn ^= 1;
	board->end = board->game.isDraw(board);
	if (save) {
		// saving position
		savePos(board);

		// saving move
		{
			if (board->nMoves % 10 == 0) {
				if (board->nMoves == 0) {
					board->moves = (MOVE*)malloc(10 * sizeof(MOVE));
					if (!board->moves) {
						printf("FATAL ERROR: HAVING PROBLEMS WITH HEAP ALLOCATION");
						exit(1);
					}
				}
				else
				{
					MOVE* realloced = (MOVE*)realloc(board->moves, sizeof(MOVE) * (board->nMoves + 10));
					if (!realloced) {
						printf("FATAL ERROR: HAVING PROBLEMS WITH HEAP ALLOCATION");
						exit(1);
					}
					board->moves = realloced;
				}
			}
			board->moves[board->nMoves++] = *m;
		}
	}
}

MOVE_CONTAINER getAllMoves(void* b) {
	_BOARD* board = (_BOARD*)b;

	MOVE* moves = NULL;
	int nMoves = 0;

	// TODO
	for (size_t i = 0; i < board->nPieces; i++)
	{
		if (!board->pieces[i].present || board->pieces[i].col != board->turn)
			continue;


		MOVE_CONTAINER m = getPossibleMoves(board->pieces + i, b, 1, 0);

		if (!m.moves) {
			continue;
		}

		if (!moves) {
			moves = malloc((nMoves + m.nMoves) * sizeof(MOVE));
			if (!moves)
				return (MOVE_CONTAINER) { 0 };
		}
		else {
			void* realloced = realloc(moves, (nMoves + m.nMoves) * sizeof(MOVE));
			if (!realloced)
				return (MOVE_CONTAINER) { 0 };

			moves = realloced;
		}

		for (size_t k = 0; k < m.nMoves; k++)
		{
			moves[nMoves + k] = m.moves[k];
		}

		nMoves += m.nMoves;

		if(m.moves)
			free(m.moves);
	}

	if (nMoves == 0) {
		free(moves);
		return (MOVE_CONTAINER) { 0 };
	}


	MOVE_CONTAINER res = { 0 };
	res.moves = moves;
	res.nMoves = nMoves;
	// shifting moves so that there are no invalid moves
	for (size_t k = 0; k < res.nMoves; k++)
	{
		if (!res.moves[k].valid) {
			res.nMoves--;
			for (size_t l = k; l < res.nMoves; l++)
			{
				res.moves[l] = res.moves[l + 1];
			}
			k--;
		}
	}
	return res;

}

int isInCheckLine(int x, int y, void* c)
{
	CHECKLINE* cl = (CHECKLINE*)c;

	if (c == NULL || cl->mTemplate == NULL)
		return 0;

	int dx = x - cl->move.x0 - cl->mTemplate->pre.dx;
	int dy = y - cl->move.y0 - cl->mTemplate->pre.dy;

	if (cl->flipX)
		dx *= -1;
	if (cl->flipY)
		dy *= -1;

	int clDx = cl->mTemplate->xDir;
	int clDy = cl->mTemplate->yDir;

	if (((clDx == 0) != (dx == 0)) || ((clDy == 0) != (dy == 0)))
		return 0;

	if ((clDx && (dx % clDx)) || (clDy && (dy % clDy)))
		return 0;

	int reps = clDx ? (dx / clDx) : (dy / clDy);

	if (reps > cl->reps || reps < cl->mTemplate->minRep || (clDy != 0 && (reps != dy / clDy)))
		return 0;

	return 1;

}

void createCheckLine(void* b, int pieceIndex, int _x, int _y) {
	_BOARD* board = (_BOARD*)b;
	PIECE* piece = board->pieces + pieceIndex;

	// creating a move struct for the testMove() function
	MOVE m = { 0 };
	m.x0 = piece->x;
	m.y0 = piece->y;
	m.x1 = _x;
	m.y1 = _y;

	CHECKLINE cl = {
		.col = piece->col ? 0 : 1,
		.move = (MOVE) {
			.x0 = piece->x,
			.y0 = piece->y,
			.x1 = _x,
			.y1 = _y
		},
		.nBtw = 0
	};

	MOVE_TEMPLATE* mt;

	for (size_t i = 0; i < piece->ptemplate->nMoves; i++)
	{
		mt = piece->ptemplate->moves + i;

		if (mt->cantCap || (mt->init && piece->moved)) // this move can not be used (anymore) to capture the king
			continue;

		if (!testMoveOnTempl(&m, mt, piece->col))// not an appropriate MOVE_TEMPLATE
			continue;

		cl.mTemplate = mt;
		if (board->squares[_y * 8 + _x] && board->pieces[board->squares[_y * 8 + _x] & 0b1111111].ptemplate->king) {
			cl.direct = 1;
		}
		else {
			cl.direct = 0;
		}
		int dx = ((int)m.x1 - (int)m.x0 - (int)mt->pre.dx);
		int dy = ((int)m.y1 - (int)m.y0 - (int)mt->pre.dy);
		if (piece->col)
			dy *= -1;

		cl.flipX = mt->flipX && ((mt->xDir > 0) != (dx > 0));
		cl.flipY = mt->flipY && ((mt->yDir > 0) != (dy > 0));

		if (piece->col) {
			dy *= -1;
			cl.flipY = !cl.flipY;
		}

		cl.reps = (dx != 0) ? abs(dx / mt->xDir) : abs(dy / mt->yDir);

		int x = piece->x + mt->pre.dx;
		int y = piece->y + mt->pre.dy;

		cl.check = 1;

		if (dx) dx /= abs(dx);
		if (dy) dy /= abs(dy);

		// not counting start or end (0 repetitions or cl.reps repetitions)
		for (size_t j = 1; j < cl.reps; j++)
		{
			x += dx;
			y += dy;

			if (board->squares[y * 8 + x] && !(board->pieces[board->squares[y*8 + x] & 0b1111111].ptemplate->king)) {
				cl.check = 0;
				cl.nBtw++;
				//break; // TODO var for number of blocking pieces
			}
		}

		addCheckLine(board, cl);
	}
}

void createCheckLineTarget(void* b, int _x, int _y, int col) {
	_BOARD* board = (_BOARD*)b;

	// preparing MOVE struct for later testMove() calls
	MOVE m = { 0 };
	m.x1 = _x;
	m.y1 = _y;

	// looping through all pieces to find ones that attack (_x, _y)
	for (size_t i = 0; i < board->nPieces; i++)
	{
		PIECE* piece = board->pieces + i;

		if (!piece->present || piece->col == col) // this piece is not present or has the wrong color
			continue;

		// filling position of piece as starting position of m
		m.x0 = piece->x;
		m.y0 = piece->y;

		MOVE_TEMPLATE* mt = testMove(&m, piece->ptemplate, board->pieces[i].col);

		if (!mt) // this piece doesn't attack (_x, _y)
			continue;

		if ((mt->init && piece->moved) || mt->cantCap) // the opponents King can't be captured (anymore)
			continue;

		// filling in a CHECKLINE struct
		CHECKLINE cl = { 0 };
		cl.check = 1;
		cl.direct = (board->squares[_y * 8 + _x] && board->pieces[board->squares[_y * 8 + _x] & 0b1111111].ptemplate->king) ? 1 : 0;
		cl.move = m;
		cl.col = col;
		cl.mTemplate = mt;
		int dx = (int)m.x1 - (int)m.x0 - mt->pre.dx;
		int dy = (int)m.y1 - (int)m.y0 - mt->pre.dy;
		cl.reps = (dx && mt->xDir) ? (abs(dx) / abs(mt->xDir)) : (abs(dy) / abs(mt->yDir));
		cl.flipX = ((m.x1 < m.x0&& mt->xDir > 0) || (m.x1 > m.x0 && mt->xDir < 0)) ? 1 : 0;
		cl.flipY = ((m.y1 < m.y0&& mt->yDir > 0) || (m.y1 > m.y0 && mt->yDir < 0)) ? 1 : 0;
		cl.nBtw = 0;

		int x = piece->x + mt->pre.dx, y = piece->y + mt->pre.dy;
		for (size_t i = 0; i < cl.reps - 1; i++)
		{
			x += cl.flipX ? -mt->xDir : mt->xDir;
			y += cl.flipY ? -mt->yDir : mt->yDir;

			if (board->squares[y * 8 + x] && !board->pieces[board->squares[y * 8 + x] & 0b1111111].ptemplate->king) {
				cl.check = 0;
				cl.nBtw++;
			}
		}

		addCheckLine(board, cl);
	}
}

// clears checkLines coming from (_x, _y)
void clearCheckLines(void* b, int _x, int _y) {
	_BOARD* board = (_BOARD*)b;


	for (size_t i = 0; i < board->nCheckLines; i++)
	{
		
		if ((board->checkLines[i].move.x0 == _x) && (board->checkLines[i].move.y0 == _y)) {
			removeCheckLine(board, i);
			i--;
		}
	}
}

// clears checkLines attacking (_x, _y)
void clearCheckLineTarget(void* b, int _x, int _y, int col) {
	_BOARD* board = (_BOARD*)b;

	for (size_t i = 0; i < board->nCheckLines; i++)
	{
		if (board->checkLines[i].move.x1 == _x && board->checkLines[i].move.y1 == _y && (col < 0 || (col == board->checkLines[i].col))) {
			removeCheckLine(board, i);
			i--;
		}
	}
}

void evolve(void* b, int type) {
	_BOARD* board = (_BOARD*)b;
	if (!board->squares[board->evolveIndex])
		return;

	PIECE* piece = board->pieces + (board->squares[board->evolveIndex] & 0b1111111);

	if ((board->evolve) && (piece->ptemplate->evolveable) && (piece->ptemplate->evolveInto & (1 << type))) {
		piece->ptemplate = board->game.pieceTypes + type;
		printf("evolved to %s", piece->ptemplate->name);
		board->evolve = 0;
		clearCheckLines(b, piece->x, piece->y);

		PIECE* king = board->pieces;
		while (!king->ptemplate->king || king->col == piece->col)
			king++;

		createCheckLine(b, (int)(piece - board->pieces), king->x, king->y);

		MOVE_CONTAINER kingMoves = getPossibleMoves(king, b, 0, 1);
		for (size_t i = 0; i < kingMoves.nMoves; i++)
		{
			createCheckLine(b, (int)(piece - board->pieces), kingMoves.moves[i].x1, kingMoves.moves[i].y1);
		}
	}
}