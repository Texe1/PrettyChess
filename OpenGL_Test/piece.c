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

	MOVE* funnyMoves = board->game.funnyMoves(p, board, checkCheck);
	int nFunnyMoves = 0;

	if (funnyMoves) {
		MOVE m;
		while ((m = funnyMoves[nFunnyMoves++]).valid);
	}

	MOVE* moves = malloc((maxMoves + nFunnyMoves) * sizeof(MOVE));
	if (!moves)
		return (MOVE_CONTAINER) { 0 };

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
		int preY = (!p->col) ? mt->pre.dy : -mt->pre.dy;

		for (int k = 1; k <= mt->maxRep; k++)
		{

			if (mt->init && p->moved)
				continue;

			unsigned char xPlus = ((((int)p->x + (int)dx + mt->pre.dx) >= 0) && (((int)p->x + (int)dx + mt->pre.dx) < 8));
			unsigned char xMinus = ((((int)p->x - (int)dx + mt->pre.dx) >= 0) && (((int)p->x - (int)dx + mt->pre.dx) < 8));
			unsigned char yPlus = ((((int)p->y + (int)dy + preY) >= 0) && (((int)p->y + (int)dy + preY) < 8));
			unsigned char yMinus = ((((int)p->y - (int)dy + preY) >= 0) && (((int)p->y - (int)dy + preY) < 8));

			if (!flipX && !flipY && !flipXY && !std)
				break;

			if (std && xPlus && yPlus && !((dx + mt->pre.dx == 0) && (dy + preY == 0))) {
				m.x1 = p->x + dx + mt->pre.dx;
				m.y1 = p->y + dy + mt->pre.dy;

 				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (!raw) {
						if ((board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col) && !mt->cantCap && k >= mt->minRep) { // capturing
							m.cap = 1;
							moves[j++] = m;
							m.cap = 0;
						}
						std = 0;
					}
					else
						moves[j++] = m;
				}
				else if(!mt->mustCap && k >= mt->minRep) {
					moves[j++] = m;
				}
			}

			if (flipX && xMinus && yPlus && !((dx - mt->pre.dx == 0) && (dy + preY == 0))) { // flipped in X
				m.x1 = p->x - dx + mt->pre.dx;
				m.y1 = p->y + dy + mt->pre.dy;
				
				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (!raw) {
						if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k >= mt->minRep) {// capturing
							m.cap = 1;
							moves[j++] = m;
							m.cap = 0;
						}
						flipX = 0;
					}
					else
						moves[j++] = m;

				}
				else if (!mt->mustCap && k >= mt->minRep) {
					moves[j++] = m;
				}
			}

			if (flipY && yMinus && xPlus && !((dx + mt->pre.dx == 0) && (dy - preY == 0))) { // flipped in Y
				m.x1 = p->x + dx + mt->pre.dx;
				m.y1 = p->y - dy + mt->pre.dy;

				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (!raw) {
						if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k >= mt->minRep) {// capturing
							m.cap = 1;
							moves[j++] = m;
							m.cap = 0;
						}
						flipY = 0;
					}
					else
						moves[j++] = m;
				}
				else if (!mt->mustCap && k >= mt->minRep) {
					moves[j++] = m;
				}
			}

			if (flipXY && xMinus && yMinus && !((dx - mt->pre.dx == 0) && (dy - preY == 0))) { // flipped in X and Y
				m.x1 = p->x - dx + mt->pre.dx;
				m.y1 = p->y - dy + mt->pre.dy;

				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (!raw) {
						if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k >= mt->minRep) {
							m.cap = 1;
							moves[j++] = m;
							m.cap = 0;
						}
						flipXY = 0;
					}
					else
						moves[j++] = m;
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

	// New checkCheck
	if (checkCheck) {
		MOVE* move = moves;
		for (size_t k = 0; k < j; k++) {
			move = moves + k;

			if (move->x0 == move->x1 && move->y0 == move->y1) {
				move->valid = 0;
				continue;
			}

			for (i = 0; i < board->nCheckLines; i++)
			{

				CHECKLINE* cl = board->checkLines + i;

				// checkLine for other Color (don't bother)
				if (cl->col != p->col)
					continue;

				// the king wants to move
				if (p->ptemplate->king) {
					if (cl->check && (move->x1 == cl->move.x1 && move->y1 == cl->move.y1)) {
						move->valid = 0;
						break;
					}
				}
				else if (cl->direct && cl->check && !isInCheckLine(move->x1, move->y1, cl)
					&& (move->x1 != cl->move.x0 || move->y1 != cl->move.y0)) {
					move->valid = 0;
					break;
				}
				else if (!cl->check && cl->direct && isInCheckLine(move->x0, move->y0, cl) && !isInCheckLine(move->x1, move->y1, cl)
					&& !(move->x1 == cl->move.x0 && move->y1 == cl->move.y0)) { // would move out of checkLine, revealing the King
					if (cl->nBtw <= 1) {
						move->valid = 0;
						break;
					}
				}

				move->valid = 1;
			}
		}

		// shifting moves so that there are no invalid moves
		for (size_t k = 0; k < j; k++)
		{
			if (!moves[k].valid) {
				j--;
				for (size_t l = k; l < j; l++)
				{
					moves[l] = moves[l + 1];
				}
				k--;
			}
		}
	}

	MOVE_CONTAINER mc = { 0 };
	mc.moves = moves;
	mc.nMoves = j;
	if (j == 0) {
		if(moves)
			free(moves);
	}
	else
	{
		void* realloced = realloc(mc.moves, sizeof(MOVE) * j);
		if (realloced) {
			mc.moves = realloced;
			return mc;
		}
	}

	return (MOVE_CONTAINER) { 0 };

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

	if (board->squares[destIndex]) {
		m->cap = 1;
	}

	if (board->squares[index]) {

		// if it's the king, storing possible moves for later
		MOVE_CONTAINER movesBefore = { 0 };
		if (board->pieces[board->squares[index] & 0b1111111].ptemplate->king) {
			movesBefore = getPossibleMoves(board->pieces + (board->squares[index] & 0b1111111), b, 0, 1);
		}
		
		board->pieces[board->squares[index] & 0b1111111].x = m->x1;
		board->pieces[board->squares[index] & 0b1111111].y = m->y1;
		board->pieces[board->squares[index] & 0b1111111].moved = 1;
		if (m->cap && board->squares[destIndex] && !m->funny) {
			if (board->pieces[board->squares[destIndex] & 0b1111111].col)
				board->bl_pieceCount--;
			else
				board->wh_pieceCount--;

			board->pieces[board->squares[destIndex] & 0b1111111].present = 0;
			board->nPositions = 0;

			for (size_t i = 0; i < board->nCheckLines; i++)
			{
				if (board->checkLines[i].move.x0 == m->x1 && board->checkLines[i].move.y0 == m->y1) {
					removeCheckLine(board, i);
				}
			}

		}
		else if(m->cap || m->funny) {
			board->game.doFunnyMove(&board->pieces[board->squares[m->y0 * 8 +m->x0] & 0b1111111], board, m);
		}
		board->squares[destIndex] = board->squares[index];
		board->squares[index] = 0;

		PIECE* piece = board->pieces + (board->squares[destIndex] & 0b1111111);
		
		// evolving
		if (piece->ptemplate->evolveable) {
			if ((piece->ptemplate->evolveAtX == 1 << 3 || piece->ptemplate->evolveAtX == destIndex % 8)
				&& (piece->ptemplate->evolveAtY == 1 << 3 || piece->ptemplate->evolveAtY == (piece->col ? (7 - destIndex / 8) : (destIndex / 8)))) {
				board->evolve = 1;
				board->evolveIndex = destIndex;
			}
		}

		// saving move
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

		// updating checkLines
		{
			// getting king of opposite color
			PIECE* king = board->pieces;
			while (!(king)->ptemplate->king || ((king)->col == piece->col))
				king++;

			// moving away from CheckLine
			clearCheckLines(b, m->x0, m->y0);

			// captured pieces may have had checkLines
			if(m->cap)
				clearCheckLines(b, m->x1, m->y1);

			// manipulating other checkLines
			for (size_t i = 0; i < board->nCheckLines; i++)
			{
				// moving into existing checkLine (setting check bit to 0)
				if (isInCheckLine(m->x1, m->y1, board->checkLines + i) 
					&& !((m->x1 == board->checkLines[i].move.x1) && (m->y1 == board->checkLines[i].move.y1))
					) {
					board->checkLines[i].check = 0;
					if(!m->cap && !isInCheckLine(m->x0, m->y0, board->checkLines + i))
						board->checkLines[i].nBtw++;
				}

				// moving out of checkLine (setting check bit to 1)
				if (
					(m->x0 != board->checkLines[i].move.x1 || m->y0 != board->checkLines[i].move.y1) 
					&& isInCheckLine(m->x0, m->y0, board->checkLines + i) 
						&& (!isInCheckLine(m->x1, m->y1, board->checkLines + i) 
							|| ((m->x1 == board->checkLines[i].move.x1) && (m->y1 == board->checkLines[i].move.y1))
							)
					) {
					if (--(board->checkLines[i].nBtw) == 0) {
						board->checkLines[i].check = 1;
					}
					
				}
			}

			// moving to new CheckLine
			createCheckLine(b, (int)(piece - board->pieces), king->x, king->y);

			// looking for every square the king can move to (these CheckLines will have the direct bit set to 0)
			MOVE_CONTAINER move = getPossibleMoves(king, board, 0, 1);
			for (int i = 0; i < move.nMoves; i++)
			{
				createCheckLine(b, (int)(piece - board->pieces), move.moves[i].x1, move.moves[i].y1);
			}

			// king can move out of checkLines, into new ones
			if (piece->ptemplate->king) {
				MOVE_CONTAINER newMoves = getPossibleMoves(board->pieces + (board->squares[destIndex] & 0b1111111), b, 0, 1);

				if (newMoves.moves) {
					// removing all checkLines of own color
					for (int i = 0; i < board->nCheckLines; i++)
					{
						if (board->checkLines[i].col == piece->col) {
							removeCheckLine(board, i);
						}
					}

					// adding new checkLines
					createCheckLineTarget(board, m->x1, m->y1, piece->col);
					for (size_t i = 0; i < newMoves.nMoves; i++)
					{
						createCheckLineTarget(board, newMoves.moves[i].x1, newMoves.moves[i].y1, piece->col);
					}

					/*
					// changing direct bit
					for (size_t i = 0; i < board->nCheckLines; i++)
					{
						if (board->checkLines[i].direct && board->checkLines[i].col == piece->col) {
							board->checkLines[i].direct = 0;
						}
						else if (board->checkLines[i].move.x1 == piece->x && board->checkLines[i].move.y1 == piece->y) {

							board->checkLines[i].direct = 1;
						}
					}

					// removing redundant checkLines
					for (size_t i = 0; i < movesBefore.nMoves; i++)
					{
						// checking if old move is also in newMoves
						char isNew = 0;
						for (size_t j = 0; j < newMoves.nMoves; j++)
						{
							if (movesBefore.moves[i].x1 == newMoves.moves[j].x1 && movesBefore.moves[i].y1 == newMoves.moves[j].y1) {
								isNew = 1;
								break;
							}
						}
						if (!isNew && !(movesBefore.moves[i].x1 == piece->x && movesBefore.moves[i].y1 == piece->y)) {
							clearCheckLineTarget(b, movesBefore.moves[i].x1, movesBefore.moves[i].y1, piece->col);
						}
					}

					// adding necessessary checkLines
					for (size_t i = 0; i < newMoves.nMoves; i++)
					{
						// checking if old moves also appears in movesBefore (checkLines are already there)
						char isOld = 0;
						for (size_t j = 0; j < movesBefore.nMoves; j++)
						{
							if (movesBefore.moves[j].x1 == newMoves.moves[i].x1 && movesBefore.moves[j].y1 == newMoves.moves[i].y1) {
								isOld = 1;
								break;
							}
						}

						if (!isOld && (newMoves.moves[i].x1 != m->x0 || newMoves.moves[i].y1 != m->y0)) { // there are no preexisting checkLines here

							createCheckLineTarget(b, newMoves.moves[i].x1, newMoves.moves[i].y1, piece->col);
						}
					}
					*/

					free(newMoves.moves);
					newMoves = (MOVE_CONTAINER){ 0 };

					free(movesBefore.moves);
					movesBefore = (MOVE_CONTAINER){ 0 };
				}
			}
		}

		board->turn ^= 1;

		if(save) savePos(board);
		board->end = board->game.isDraw(board); // returns 0 when no draw


		printf("made move: %c%c->%c%c\n", (char)('a' + m->x0), (char)('1' + m->y0), (char)('a' + m->x1), (char)('1' + m->y1));
		print_board(b, -1);

		printf("CheckLines:\n");
		for (size_t i = 0; i < board->nCheckLines; i++)
		{
			printf("\t%c%c->%c%c; check: %d, direct: %d, color: %d, reps: %d, nBtw: %d\n", (char)('a' + board->checkLines[i].move.x0), (char)('1' + board->checkLines[i].move.y0), (char)('a' + board->checkLines[i].move.x1), (char)('1' + board->checkLines[i].move.y1), board->checkLines[i].check, board->checkLines[i].direct, board->checkLines[i].col, board->checkLines[i].reps, board->checkLines[i].nBtw);
		}
		printf("\n");


		return 0;
	}
	else return 1;
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
	return res;

}

int isInCheckLine(int x, int y, void* c)
{
	CHECKLINE* cl = (CHECKLINE*)c;

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

	// preparing MOVE struct for later tesMove() calls
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