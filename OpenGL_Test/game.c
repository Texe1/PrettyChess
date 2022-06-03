#include "game.h"
#include <stdio.h>

_BOARD* createStdBoard() {
	_BOARD* board = (_BOARD*)malloc(sizeof(_BOARD) + 6 * sizeof(PIECE_TEMPLATE) + 14 * sizeof(MOVE_TEMPLATE) + 32 * sizeof(PIECE));
	if (!board)
		return NULL;

	*board = (_BOARD){ 0 };

	board->game.nPieceTypes = 6;
	board->game.pieceTypes = (PIECE_TEMPLATE*)(board + 1);

	// King		(0)
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

	// Queen	(1)
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

	// Rook		(2)
	{
		board->game.pieceTypes[2].name = "ROOK";
		board->game.pieceTypes[2].abbreviation = 'R';
		board->game.pieceTypes[2].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 6;

		board->game.pieceTypes[2].nMoves = 2;

		MOVE_TEMPLATE move = { 0 };
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
	}

	// Knight	(3)
	{
		board->game.pieceTypes[3].name = "KNIGHT";
		board->game.pieceTypes[3].abbreviation = 'N';
		board->game.pieceTypes[3].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 8;

		board->game.pieceTypes[3].nMoves = 2;

		MOVE_TEMPLATE move = { 0 };
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
	}

	// Bishop	(4)
	{
		board->game.pieceTypes[4].name = "BISHOP";
		board->game.pieceTypes[4].abbreviation = 'B';
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

	// Pawn		(5)
	{
		board->game.pieceTypes[5].name = "PAWN";
		board->game.pieceTypes[5].abbreviation = 'P';
		board->game.pieceTypes[5].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 11;

		board->game.pieceTypes[5].nMoves = 3;

		MOVE_TEMPLATE move = { 0 };
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
	}

	setStartingPos(&board->game, "RNBQKBNRPPPPPPPP8888pppppppprnbqkbnr");

	board->game.funnyMoves = funnyMovesStd;
	board->game.doFunnyMove = doFunnyMoveStd;
	board->nMoves = 0;

	startGame(board);
	
	return board;
}

void startGame(_BOARD* board) {
	for (size_t i = 0; i < 64; i++)
	{
		if (board->game.startPosition[i])
			board->nPieces++;
	}

	board->pieces = (PIECE*)malloc(sizeof(PIECE) * board->nPieces);

	int j = 0;

	for (size_t i = 0; i < 64; i++)
	{
		if (board->game.startPosition[i]) {
			int index = board->game.startPosition[i] & 0b111111;
			int x = i % 8;
			int y = i / 8;

			PIECE p = { 0 };
			p.col = (board->game.startPosition[i] & 0x40) != 0;
			p.ptemplate = &board->game.pieceTypes[index];
			p.x = x;
			p.y = y;
			p.present = 1;

			board->pieces[j] = p;
			board->squares[i] = j++ | (1 << 7);

			if (j >= board->nPieces)
				break;
		}
	}
	
}

void setStartingPos(GAME* game, const char* pos) {// sets the starting position of a game

	unsigned char lookup[256] = { 0 };
	for (size_t i = 0; i < game->nPieceTypes; i++)
	{
		lookup[(unsigned char)(game->pieceTypes[i].abbreviation) & ~(1 << 5)] = i + 1;
	}

	int j = 0;
	for (size_t i = 0; pos[i] != 0; i++)
	{
		if (pos[i] >= '0' && pos[i] <= '8') {
			j += pos[i] - '0';
				continue;
		}

		if (!lookup[pos[i]] && !lookup[pos[i] & ~(1 << 5)]) {
			j++;
			continue;
		}

		if (pos[i] >= 'a')
			game->startPosition[j++] = (lookup[pos[i] & ~(1 << 5)] - 1) | (0b11000000); // col = 1
		else
			game->startPosition[j++] = (lookup[pos[i]] - 1) | (0b10000000); // col = 0;
	}
}

MOVE* funnyMovesStd(PIECE* piece, _BOARD* board) {
	MOVE* lastMove = &board->moves[board->nMoves-1];

	if ((board->nMoves > 0 && piece->ptemplate->abbreviation == 'P') // piece is a PAWN
		&& (lastMove->y1 == piece->y) && (abs(lastMove->y1 - lastMove->y0) == 2) && (abs(lastMove->x1 - piece->x) == 1)  // last move was 2 forward next to piece
		&& (board->pieces[board->squares[lastMove->y1 * 8 + lastMove->x1] & 0b111111].ptemplate->abbreviation == 'P') /*last move was a Pawn*/) {
		MOVE* moves = (MOVE*)malloc(sizeof(MOVE) * 2);
		if (!moves)
			return NULL;

		moves[0].cap = 1;
		moves[0].valid = 1;
		moves[0].x0 = piece->x;
		moves[0].y0 = piece->y;
		moves[0].x1 = lastMove->x1;
		moves[0].y1 = piece->x + (piece->col ? 1 : -1);

		moves[1].valid = 0;

		return moves;
	}

	return NULL;
}

void doFunnyMoveStd(PIECE* piece, struct _BOARD* board, MOVE* move) {
	if (piece->ptemplate->abbreviation == 'P') {
		if (!board->squares[move->y1 * 8 + move->x1]) { // probably en passant
			if (abs(move->x1 - move->x0) == 1 && abs(move->y0 - move->y1) == 1 && (board->squares[move->y0 * 8 + move->x1] && board->pieces[board->squares[move->y0 * 8 + move->x1] & 0b1111111].ptemplate->abbreviation == 'P')) { // definetely En passant
				board->pieces[board->squares[move->y0 * 8 + move->x1] & 0b1111111].present = 0;
				board->squares[move->y0 * 8 + move->x1] = 0;
				
			}
		}
	}
}

#define gotoxy(x,y) printf("\033[%d;%dH", (int)(y), (int)(x))
void print_board(_BOARD* board, int y) {
	gotoxy(0, y);
	for (int i = 63; i >= 0; i--)
	{

		if (board->squares[i]) {
			if (board->pieces[board->squares[i] & 0b1111111].col)
				printf("b");
			else
				printf("w");
			printf("%c", board->pieces[board->squares[i] & 0b1111111].ptemplate->abbreviation);
		}
		else {
			printf("  ");
		}
		if (i % 8 == 0)
			printf("\n");
	}
}