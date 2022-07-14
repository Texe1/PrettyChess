#include "Bot.h"

#include <string.h>
#include <stdio.h>

MOVE chooseComputerMove(_BOARD* board) {

	printf("choosing move...\n");
	MOVE m = miniMax(board, 6);

	return m;

}

MOVE test = { 0 };

MOVE_CONTAINER bestPath = { 0 };

int _miniMax(_BOARD* b, int alpha, int beta, int depth);

#define WHITE_WIN ~(1 << 31)
#define BLACK_WIN (1 << 31)

MOVE miniMax(_BOARD* b, int depth) {
	int alpha = BLACK_WIN;
	int beta = WHITE_WIN;

	MOVE_CONTAINER moves = getAllMoves(b);

	// sorting moves (captures first)
	{
		int nCaps = 0;
		for (size_t i = 0; i < moves.nMoves; i++)
		{
			if (moves.moves[i].cap) {
				if (i > nCaps) {
					MOVE m = moves.moves[i];
					moves.moves[i] = moves.moves[nCaps];
					moves.moves[nCaps] = m;
				}
				nCaps++;
			}
		}
	}

	_BOARD* bCopy;

	MOVE bestMove = { 0 };

	if (!b->turn) { // maximizing -> white
		int max = BLACK_WIN;


		for (size_t i = 0; i < moves.nMoves; i++)
		{
			//continues when the move is illegal
			if (!moves.moves[i].valid)
				continue;

			//moving in a copy of the board
			bCopy = copyBoard(b);
			_move(bCopy, moves.moves + i, 0);

			if (bCopy->evolve) {
				evolve(bCopy, 1);
			}

			//recursive call of the minimax function in order to go a layer deeper 
			int z = _miniMax(bCopy, alpha, beta, (depth - 1));

			//checking if the latest move is the best one so far
			if (z > max) {
				max = z;
				bestMove = moves.moves[i];
			}
			alpha = z > alpha ? z : alpha;

			// freeing boardCopy
			{
				free(bCopy->pieces);
				free(bCopy->checkLines);
				free(bCopy);
			}
		}


		return bestMove;

	}
	else { // minimizing -> black
		int min = WHITE_WIN;

		for (size_t i = 0; i < moves.nMoves; i++)
		{
			// move is invalid (don't calculate)
			if (!moves.moves[i].valid)
				continue;

			// copying board and making move
			bCopy = copyBoard(b);
			_move(bCopy, moves.moves + i, 0);

			// recursive call
			int val = _miniMax(bCopy, alpha, beta, depth - 1);

			printf("%d", val);

			// freeing board
			{
				free(bCopy->pieces);
				free(bCopy->checkLines);
				free(bCopy);
			}

			// best move so far
			if (val < min) {
				bestMove = moves.moves[i];
				min = val;
			}

			// better than any other path so far
			if (val < beta)
				beta = val;
		}

		return bestMove;
	}
}

int _miniMax(_BOARD* b, int alpha, int beta, int depth) {
	
	switch (b->end)
	{
	case 1 || 2 || 3 || 4:
		return 0;
	case 5:
		return WHITE_WIN-1;
	case 6:
		return BLACK_WIN+1;
	default:
		break;
	}

	if (depth == 0) {
		return eval(b);

	}
	MOVE_CONTAINER moves = getAllMoves(b);

	// sorting moves (captures first)
	{
		int nCaps = 0;
		for (size_t i = 0; i < moves.nMoves; i++)
		{
			if (moves.moves[i].cap) {
				if (i > nCaps) {
					MOVE m = moves.moves[i];
					moves.moves[i] = moves.moves[nCaps];
					moves.moves[nCaps] = m;
				}
				nCaps++;
			}
		}
	}

	_BOARD* bCopy;

	if (!b->turn) { // maximizing -> white
		int max = BLACK_WIN;
		

		for (size_t i = 0; i < moves.nMoves; i++)
		{
			//continues when the move is illegal
			if (!moves.moves[i].valid)
				continue;

			//moving in a copy of the board
			bCopy = copyBoard(b);
			_move(bCopy, moves.moves + i, 0);

			//recursive call of the minimax function in order to go a layer deeper 
			int z = _miniMax(bCopy, alpha, beta, (depth - 1));

			//checking if the latest move is the best one so far
			max = z > max ? z : max;
			alpha = z > alpha ? z : alpha;



			// freeing boardCopy
			{
				free(bCopy->pieces);
				free(bCopy->checkLines);
				free(bCopy);
			}

			//alpha-beta-pruning: stops the loop when the moves get too good
			if (beta <= alpha){
				break;
			}
		}


		return max;

	}
	else { // minimizing -> black
		int min = WHITE_WIN;

		for (size_t i = 0; i < moves.nMoves; i++)
		{
			// move is invalid (don't calculate)
			if (!moves.moves[i].valid)
				continue;

			// copying board and making move
			bCopy = copyBoard(b);
			_move(bCopy, moves.moves + i, 0);

			// recursive call
			int val = _miniMax(bCopy, alpha, beta, depth-1);

			// freeing board
			{
				free(bCopy->pieces);
				free(bCopy->checkLines);
				free(bCopy);
			}

			// move is too good
			if (val <= alpha) {
				return val;
			}

			// best move so far
			if (val < min)
				min = val;

			// better than any other path so far
			if (val < beta)
				beta = val;
		}

		return min;
	}
}

int eval(_BOARD* b) {

	int eval = 0;

	int end = b->game.isDraw(b);
	if ((end == 5) || (end == 6)) {
		return (1 << 31);
	}
	else if ((end == 3) || (end == 4))
		return 0;

	for (size_t i = 0; i < b->nPieces; i++)
	{
		if (!b->pieces[i].present) {
			continue;
		}

	//	int pieceEval = 0;

	//	switch (b->pieces[i].ptemplate->abbreviation)
	//	{
	//	case 'P':
	//		pieceEval = 100;
	//		//int xValue = (b->pieces[i].x <4) ? 3 - b->pieces[i].x : b->pieces[i].x - 4;
	//		//pieceEval -= 4 * (xValue); // Value decreaces with distance to the center
	//		//int colMultiplicator = b->pieces[i].col ? -1 : 1;
	//		//pieceEval *= (b->pieces[i].y * colMultiplicator);
	//		break;
	//	case 'N':
	//		pieceEval = 300;
	//		break;
	//	case 'B':
	//		pieceEval = 300;
	//		break;
	//	case 'R':
	//		pieceEval = 500;
	//		break;
	//	case 'Q':
	//		pieceEval = 900;
	//	default:
	//		break; 
	//	}

	//	if (b->pieces[i].col) 
	//		eval -= pieceEval;
	//	else
	//		eval += pieceEval;

		eval += b->pieces[i].eval;


	}

	return eval;
}

int standartEvaluate(PIECE* p) {
	int pieceEval = 0;

	switch (p->ptemplate->abbreviation)
	{
	case 'P':
		pieceEval = 100;
		int xValue = (p->x <4) ? 3 - p->x : p->x - 4;
		pieceEval -= 4 * (xValue); // Value decreaces with distance to the center
		int colMultiplicator = p->col ? -1 : 1;
		pieceEval *= (p->y * colMultiplicator);
		break;
	case 'N':
		pieceEval = 300;
		break;
	case 'B':
		pieceEval = 300;
		break;
	case 'R':
		pieceEval = 500;
		break;
	case 'Q':
		pieceEval = 900;
	default:
		break;
	}

	if (p->col)
		return -pieceEval;
	else
		return pieceEval;

}