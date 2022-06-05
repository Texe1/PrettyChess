#include "piece.h"

#include "game.h"
#include <Windows.h>
#include <stdio.h>

MOVE* getPossibleMoves(PIECE* p, void* pBoard, char checkCheck) {
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
				m.y1 = p->y + dy;
				
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

			if (flipY && yMinus && xPlus) { // flipped in Y
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

			if (flipXY && xMinus && yMinus) { // flipped in X and Y
				m.x1 = p->x - dx;
				m.y1 = p->y - dy;

				if (board->squares[m.y1 * 8 + m.x1] && !mt->jump) {
					if (board->pieces[board->squares[m.y1 * 8 + m.x1] & 0b111111].col != p->col && !mt->cantCap && k >= mt->minRep) {
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

	if (checkCheck) {
		_BOARD* duplicate = (_BOARD*)malloc(sizeof(_BOARD));
		if (!duplicate) {
			moves[j].valid = 0; // continuing without checkCheck
			return moves;
		}

		*duplicate = *board;
		duplicate->moves = (MOVE*)malloc((board->nMoves + 10) * sizeof(MOVE));
		if (!duplicate->moves) {
			free(duplicate);
			moves[j].valid = 0; // continuing without checkCheck
			return moves;
		}

		duplicate->pieces = (PIECE*)malloc(board->nPieces * sizeof(PIECE));
		if (!duplicate->pieces) {
			free(duplicate->moves);
			free(duplicate);
			moves[j].valid = 0; // continuing without checkCheck
			return moves;
		}

		for (size_t i = 0; i < board->nMoves; i++)
		{
			duplicate->moves[i] = board->moves[i];
		}

		for (size_t i = 0; i < board->nPieces; i++)
		{
			duplicate->pieces[i] = board->pieces[i];
		}

		duplicate->nPositions = 0;
		duplicate->positions = NULL;

		MOVE* m = moves;
		// filtering moves
		for (size_t i = 0; i < j; i++)
		{
 			_move(duplicate, m, 0);
			// checkCheck is happening here


			PIECE* piece = duplicate->pieces;
			// looping through all pieces
			for (size_t i = 0; i < duplicate->nPieces; i++)
			{
				if ((piece->col == p->col) || !piece->present) { // own or not present pieces can't capture the King
					piece++;
					continue;
				}

				// getting possible moves for this piece
				MOVE* theoreticalMoves = getPossibleMoves(piece, duplicate, 0);

				for (MOVE* move = theoreticalMoves; move->valid; move++)
				{
					PIECE* capturedPiece = duplicate->pieces + (duplicate->squares[move->y1 * 8 + move->x1] & 0b1111111);
					if (move->cap && capturedPiece->ptemplate->king && capturedPiece->col == p->col) {
						m->valid = 0;
						break;
					}
				}

				if (!m->valid)
					break;

				piece++;
			}

			// reversing to actual board
			if (!m->cap) { // there is no need to copy all pieces again, we only need to set back the moved piece (also no funnyMoves) TODO castling (funnyMove bit?)
				PIECE* piece = &duplicate->pieces[duplicate->squares[m->y1 * 8 + m->x1] & 0b1111111];
				duplicate->squares[m->y0 * 8 + m->x0] = duplicate->squares[m->y1 * 8 + m->x1];
				duplicate->squares[m->y1 * 8 + m->x1] = 0;
			}
			else { // recpoying pieces and decrementing nMoves because I won't deal with En passant
				duplicate->nMoves--;
				for (size_t j = 0; j < duplicate->nPieces; j++)
				{
					duplicate->pieces[j] = board->pieces[j];
				}
				for (size_t j = 0; j < 64; j++)
				{
					duplicate->squares[j] = board->squares[j];
				}
			}
			m++;
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

		free(duplicate->moves);
		free(duplicate->pieces);
		if(duplicate->positions)
			free(duplicate->positions);
		free(duplicate);
	}

	moves[j].valid = 0;
	return moves;

}

void testMove(MOVE* move, PIECE_TEMPLATE templ) {
	MOVE m = *move;


	while ((m.y1 - m.y0) % (m.x1 - m.x0) == 0) {
		m.y1 = ((m.y1 - m.y0) / (m.x1 - m.x0)) + m.y0;
	}

	while ((m.x1 - m.x0) % (m.y1 - m.y0) == 0) {
		m.x1 = ((m.x1 - m.x0) / (m.y1 - m.y0)) + m.x0;
	}

	for (size_t i = 0; i < templ.nMoves; i++)
	{
		MOVE_TEMPLATE* mTempl = templ.moves + i;
		if ((((int)m.x1 - (int)m.x0) == mTempl->xDir || (mTempl->flipX && (((int)m.x1 - (int)m.x0) == mTempl->xDir)))
			&& (((int)m.y1 - (int)m.y0) == mTempl->yDir || (mTempl->flipY && (((int)m.y1 - (int)m.y0) == mTempl->yDir)))) {
			move->valid = 1;
			return;
		}
	}

	move->valid = 0;
}

inline int getMaxMoveCount(MOVE_TEMPLATE* templ) {
	return templ->maxRep * (templ->flipX ? 2 : 1) * (templ->flipY ? 2 : 1);
}

int _move(void* b, MOVE* m, char save) {

	_BOARD* board = (_BOARD*)b;

	int index = m->y0 * 8 + m->x0;
	if (board->squares[index]) {
		print_board(board, 0);

		int destIndex = m->y1 * 8 + m->x1;
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

		}
		else if(m->cap || m->funny) {
			board->game.doFunnyMove(&board->pieces[board->squares[m->y0 * 8 +m->x0] & 0b1111111], board, m);
		}
		board->squares[destIndex] = board->squares[index];
		board->squares[index] = 0;

		PIECE* piece = &board->pieces[board->squares[destIndex] & 0b1111111];

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

		if(save) savePos(board);
		if(save)board->end = board->game.isDraw(board); // returns 0 when no draw

		printf("Made move: (%d, %d) -> (%d,%d), valid:%d\n", m->x0, m->y0, m->x1, m->y1, m->valid);

		print_board(board, 10);


		return 0;
	}
	else return 1;
}

void evolve(void* b, int type) {
	_BOARD* board = (_BOARD*)b;
	if ((board->evolve) && (board->pieces[board->squares[board->evolveIndex] & 0b1111111].ptemplate->evolveable) && (board->pieces[board->squares[board->evolveIndex] & 0b1111111].ptemplate->evolveInto & (1 << type))) {
		board->pieces[board->squares[board->evolveIndex] & 0b1111111].ptemplate = &board->game.pieceTypes[type];
		board->evolve = 0;
	}
}