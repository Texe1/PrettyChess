#include "game.h"

_BOARD* createStdBoard() {
	_BOARD* board = (_BOARD*)malloc(sizeof(_BOARD) + 6 * sizeof(PIECE_TEMPLATE) + 8 * sizeof(MOVE_TEMPLATE));
	if (!board)
		return NULL;

	board->game.nPieceTypes = 6;
	board->game.pieceTypes = (PIECE_TEMPLATE*)(board + 1);

	// King
	{
		board->game.pieceTypes[0].name = "KING";
		board->game.pieceTypes[0].abbreviation = 'K';
		board->game.pieceTypes[0].moves = (MOVE_TEMPLATE*)(board->game.pieceTypes + 6);

		board->game.pieceTypes[0].nMoves = 3;

		MOVE_TEMPLATE move = { 0 };
		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[0].moves[0] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[0].moves[1] = move;

		move.flipX = 1;
		move.yDir = 1;
		move.xDir = 1;
		board->game.pieceTypes[0].moves[2] = move;
	}

	// QUEEN
	{
		board->game.pieceTypes[1].name = "QUEEN";
		board->game.pieceTypes[1].abbreviation = 'Q';
		board->game.pieceTypes[1].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 3;

		board->game.pieceTypes[1].nMoves = 3;

		MOVE_TEMPLATE move = { 0 };
		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[1].moves[0] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[1].moves[1] = move;

		move.flipX = 1;
		move.yDir = 1;
		move.xDir = 1;
		board->game.pieceTypes[1].moves[2] = move;
	}

	// KNIGHT
	{
		board->game.pieceTypes[2].name = "KNIGHT";
		board->game.pieceTypes[2].abbreviation = 'N';
		board->game.pieceTypes[2].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 6;

		board->game.pieceTypes[2].nMoves = 3;

		MOVE_TEMPLATE move = { 0 };
		move.flipX = 1;
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 2;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[2].moves[0] = move;
		move.xDir = 2;
		move.yDir = 1;
		board->game.pieceTypes[2].moves[1] = move;
	}
	return board;
}