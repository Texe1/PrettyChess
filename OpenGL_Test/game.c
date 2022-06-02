#include "game.h"

BOARD* createStdBoard() {
	BOARD* board = (BOARD*)malloc(sizeof(BOARD) + 6 * sizeof(PIECE_TEMPLATE) + 2 * sizeof(MOVE_TEMPLATE));
	if (!board)
		return NULL;

	board->game.nPieceTypes = 6;
	board->game.pieceTypes = (PIECE_TEMPLATE*)(board + 1);
	if (!board->game.pieceTypes) {
		free(board);
		return NULL;
	}

	// King
	{
		board->game.pieceTypes[0].name = "KING";
		board->game.pieceTypes[0].abbreviation = 'K';
		board->game.pieceTypes[0].moves = (MOVE_TEMPLATE*)(board->game.pieceTypes + 6);

		board->game.pieceTypes[0].nMoves = 2;

		MOVE_TEMPLATE move = { 0 };
		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[0].moves[0] = move;
		move = (MOVE_TEMPLATE){0};
		move.flipY = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[0].moves[1] = move;
	}



	return board;
}