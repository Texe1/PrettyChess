#include "game.h"

_BOARD* createStdBoard() {
	_BOARD* board = (_BOARD*)malloc(sizeof(_BOARD) + 6 * sizeof(PIECE_TEMPLATE) + 14 * sizeof(MOVE_TEMPLATE));
	if (!board)
		return NULL;

	*board = (_BOARD){ 0 };

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

	// ROOK
	{
		board->game.pieceTypes[3].name = "ROOK";
		board->game.pieceTypes[3].abbreviation = "R";
		board->game.pieceTypes[3].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 8;

		board->game.pieceTypes[3].nMoves = 2;

		MOVE_TEMPLATE move = { 0 };
		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[3].moves[0] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[3].moves[1] = move;
	}

	// Bishop
	{
		board->game.pieceTypes[4].name = "BISHOP";
		board->game.pieceTypes[4].abbreviation = "B";
		board->game.pieceTypes[4].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 10;

		board->game.pieceTypes[4].nMoves = 1;

		MOVE_TEMPLATE move = { 0 };
		move.flipX = 1;
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[4].moves[0] = move;
	}

	// Pawn
	{
		board->game.pieceTypes[5].name = "PAWN";
		board->game.pieceTypes[5].abbreviation = "P";
		board->game.pieceTypes[5].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 11;

		board->game.pieceTypes[5].nMoves = 3;

		MOVE_TEMPLATE move = { 0 };
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		move.mustCap = 1;
		board->game.pieceTypes[5].moves[0] = move;


		move.flipY = 0;
		move.xDir = 0;
		move.minRep = 1;
		move.maxRep = 1;
		move.mustCap = 0;
		move.cantCap = 1;
		board->game.pieceTypes[5].moves[1] = move;
		
		move.minRep = 2;
		move.maxRep = 2;
		move.yDir = 1;
		move.init = 1;
		board->game.pieceTypes[5].moves[2] = move;
	}

	return board;
}