#include "schroedinger.h"

_BOARD* createSchroedingerBoard() {
	_BOARD* board = (_BOARD*)malloc(sizeof(_BOARD) + 6 * sizeof(PIECE_TEMPLATE) + 28 * sizeof(MOVE_TEMPLATE) + 32 * sizeof(PIECE));
	if (!board)
		return NULL;

	*board = (_BOARD){ 0 };

	board->game.nPieceTypes = 6;
	board->game.pieceTypes = (PIECE_TEMPLATE*)(board + 1);

	// King		(0)
	{
		board->game.pieceTypes[0] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[0].name = "KING";
		board->game.pieceTypes[0].abbreviation = 'K';
		board->game.pieceTypes[0].moves = (MOVE_TEMPLATE*)(board->game.pieceTypes + 6);
		board->game.pieceTypes[0].king = 1;

		board->game.pieceTypes[0].nMoves = 6;

		MOVE_TEMPLATE move = { 0 };
		move.preX = 1;

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

		move = (MOVE_TEMPLATE){ 0 };
		move.preX = -1;

		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[0].moves[3] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[0].moves[4] = move;

		move.flipX = 1;
		move.yDir = 1;
		move.xDir = 1;
		board->game.pieceTypes[0].moves[5] = move;
	}

	// Queen	(1)
	{
		board->game.pieceTypes[1] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[1].name = "QUEEN";
		board->game.pieceTypes[1].abbreviation = 'Q';
		board->game.pieceTypes[1].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 6;
		board->game.pieceTypes[1].king = 0;

		board->game.pieceTypes[1].nMoves = 6;

		MOVE_TEMPLATE move = { 0 };
		move.preX = 1;

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

		move = (MOVE_TEMPLATE){ 0 };
		move.preX = -1;
		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[1].moves[3] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[1].moves[4] = move;

		move.flipX = 1;
		move.yDir = 1;
		move.xDir = 1;
		board->game.pieceTypes[1].moves[5] = move;
	}

	// Rook		(2)
	{
		board->game.pieceTypes[2] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[2].name = "ROOK";
		board->game.pieceTypes[2].abbreviation = 'R';
		board->game.pieceTypes[2].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 12;
		board->game.pieceTypes[2].king = 0;

		board->game.pieceTypes[2].nMoves = 4;

		MOVE_TEMPLATE move = { 0 };
		move.preX = 1;

		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[2].moves[0] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[2].moves[1] = move;

		move = (MOVE_TEMPLATE){0};
		move.preX = -1;

		move.flipX = 1;
		move.xDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[2].moves[2] = move;

		move.flipY = 1;
		move.flipX = 0;
		move.yDir = 1;
		move.xDir = 0;
		board->game.pieceTypes[2].moves[3] = move;
	}

	// Knight	(3)
	{
		board->game.pieceTypes[3] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[3].name = "KNIGHT";
		board->game.pieceTypes[3].abbreviation = 'N';
		board->game.pieceTypes[3].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 16;
		board->game.pieceTypes[3].king = 0;

		board->game.pieceTypes[3].nMoves = 4;

		MOVE_TEMPLATE move = { 0 };
		move.preX = 1;

		move.flipX = 1;
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 2;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[3].moves[0] = move;
		move.xDir = 2;
		move.yDir = 1;
		board->game.pieceTypes[3].moves[1] = move;

		move = (MOVE_TEMPLATE){0};
		move.preX = -1;

		move.flipX = 1;
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 2;
		move.minRep = 1;
		move.maxRep = 1;
		board->game.pieceTypes[3].moves[2] = move;
		move.xDir = 2;
		move.yDir = 1;
		board->game.pieceTypes[3].moves[3] = move;
	}

	// Bishop	(4)
	{
		board->game.pieceTypes[4] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[4].name = "BISHOP";
		board->game.pieceTypes[4].abbreviation = 'B';
		board->game.pieceTypes[4].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 20;
		board->game.pieceTypes[4].king = 0;

		board->game.pieceTypes[4].nMoves = 2;

		MOVE_TEMPLATE move = { 0 };
		move.preX = 1;

		move.flipX = 1;
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[4].moves[0] = move;


		move = (MOVE_TEMPLATE){0};
		move.preX = -1;
		move.flipX = 1;
		move.flipY = 1;
		move.xDir = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 7;
		board->game.pieceTypes[4].moves[1] = move;
	}

	// Pawn		(5)
	{
		board->game.pieceTypes[5] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[5].name = "PAWN";
		board->game.pieceTypes[5].abbreviation = 'P';
		board->game.pieceTypes[5].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 22;
		board->game.pieceTypes[5].king = 0;

		board->game.pieceTypes[5].nMoves = 6;

		MOVE_TEMPLATE move = { 0 };
		move.preX = 1;

		move.flipX = 1;
		move.xDir = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		move.mustCap = 1;
		board->game.pieceTypes[5].moves[0] = move;


		move.flipX = 0;
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


		move = (MOVE_TEMPLATE){0};
		move.preX = -1;

		move.flipX = 1;
		move.xDir = 1;
		move.yDir = 1;
		move.minRep = 1;
		move.maxRep = 1;
		move.mustCap = 1;
		board->game.pieceTypes[5].moves[3] = move;


		move.flipX = 0;
		move.xDir = 0;
		move.minRep = 1;
		move.maxRep = 1;
		move.mustCap = 0;
		move.cantCap = 1;
		board->game.pieceTypes[5].moves[4] = move;

		move.minRep = 2;
		move.maxRep = 2;
		move.yDir = 1;
		move.init = 1;
		board->game.pieceTypes[5].moves[5] = move;

		board->game.pieceTypes[5].evolveable = 1;
		board->game.pieceTypes[5].evolveAtX = 1 << 3;
		board->game.pieceTypes[5].evolveAtY = 7;
		board->game.pieceTypes[5].evolveInto = 0b11110;
	}

	setStartingPos(&board->game, "RNBQKBNRPPPPPPPP8888pppppppprnbqkbnr");
	//setStartingPos(&board->game, "RQK57k888888");

	board->game.funnyMoves = funnyMovesStd;
	board->game.doFunnyMove = doFunnyMoveStd;
	board->game.isDraw = isDrawStd;
	board->nMoves = 0;

	startGame(board);

	return board;

}