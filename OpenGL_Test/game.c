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
		board->game.pieceTypes[0] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[0].name = "KING";
		board->game.pieceTypes[0].abbreviation = 'K';
		board->game.pieceTypes[0].moves = (MOVE_TEMPLATE*)(board->game.pieceTypes + 6);
		board->game.pieceTypes[0].king = 1;

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
		board->game.pieceTypes[1] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[1].name = "QUEEN";
		board->game.pieceTypes[1].abbreviation = 'Q';
		board->game.pieceTypes[1].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 3;
		board->game.pieceTypes[1].king = 0;

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
		board->game.pieceTypes[2] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[2].name = "ROOK";
		board->game.pieceTypes[2].abbreviation = 'R';
		board->game.pieceTypes[2].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 6;
		board->game.pieceTypes[2].king = 0;

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
		board->game.pieceTypes[3] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[3].name = "KNIGHT";
		board->game.pieceTypes[3].abbreviation = 'N';
		board->game.pieceTypes[3].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 8;
		board->game.pieceTypes[3].king = 0;

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
		board->game.pieceTypes[4] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[4].name = "BISHOP";
		board->game.pieceTypes[4].abbreviation = 'B';
		board->game.pieceTypes[4].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 10;
		board->game.pieceTypes[4].king = 0;

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
		board->game.pieceTypes[5] = (PIECE_TEMPLATE){ 0 };
		board->game.pieceTypes[5].name = "PAWN";
		board->game.pieceTypes[5].abbreviation = 'P';
		board->game.pieceTypes[5].moves = ((MOVE_TEMPLATE*)(board->game.pieceTypes + 6)) + 11;
		board->game.pieceTypes[5].king = 0;

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

void startGame(_BOARD* board) {

	board->nPieces = 0;
	for (size_t i = 0; i < 64; i++)
	{
		board->squares[i] = 0;
		if (board->game.startPosition[i])
			board->nPieces++;
	}

	board->checkLines = NULL;
	board->nCheckLines = 0;
	board->bl_pieceCount = 0;
	board->wh_pieceCount = 0;
	board->moves = NULL;
	board->positions = NULL;
	board->nMoves = 0;
	board->nPositions = 0;
	board->turn = 0;


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

			if (p.col)
				board->bl_pieceCount++;
			else
				board->wh_pieceCount++;

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

	savePos(board);

	initCheckLines(board);
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

// TODO fix j++ bug where one (?, ?)->(7,6)-move (destination h7) is considered legal (most likely here?)
MOVE* funnyMovesStd(PIECE* piece, _BOARD* board, char checkCheck) {
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
		moves[0].y1 = piece->y + (piece->col ? -1 : 1);

		moves[1].valid = 0;

		return moves;

	}

	//castle
	if ((piece->ptemplate->abbreviation == 'K') //piece is a King 
		&& (piece->moved == 0)) {


		MOVE* moves = (MOVE*)malloc(sizeof(MOVE) * 3);
		if (!moves)
			return NULL;

		int j = 0;
		if (!(board->squares[piece->col * 56 + 1]) && !(board->squares[piece->col * 56 + 2]) && !(board->squares[piece->col * 56 + 3]) && (!board->pieces[piece->col * 24].moved) && (board->pieces[piece->col * 24].present)) { //big castles

			moves[j] = (MOVE){ 0 };
			moves[j].y0 = piece->col * 7;
			moves[j].y1 = piece->col * 7;
			moves[j].x0 = 4;
			moves[j].x1 = 2;
			moves[j].valid = 1;
			moves[j].funny = 1;

			if (checkCheck) {
				for (size_t i = 0; i < board->nCheckLines; i++)
				{
					CHECKLINE* cl = board->checkLines + i;

					if (cl->check && cl->move.y1 == piece->col * 7 && (cl->move.x0 == 4 || cl->move.x0 == 3 || cl->move.x0 == 2)) {
						moves[j].valid = 0;
					}

				}
			}

			if (moves[j].valid) {
				j++;
			}

		}
		if (!(board->squares[piece->col * 56 + 5]) && !(board->squares[piece->col * 56 + 6]) && (!board->pieces[piece->col * 24 + 7].moved) && (board->pieces[piece->col * 24 + 7].present)) { //small castles
			//TODO checkCheck

			moves[j] = (MOVE){ 0 };
			moves[j].y0 = piece->col * 7;
			moves[j].y1 = piece->col * 7;
			moves[j].x0 = 4;
			moves[j].x1 = 6;
			moves[j].valid = 1;
			moves[j].funny = 1;

			if (checkCheck) {

				for (size_t i = 0; i < board->nCheckLines; i++)
				{
					CHECKLINE* cl = board->checkLines + i;

					if (cl->check && cl->move.y1 == piece->col * 7 && (cl->move.x0 == 4 || cl->move.x0 == 5 || cl->move.x0 == 6)) {
						moves[j].valid = 0;
					}

				}
			}
			if (moves[j].valid) {
				j++;
			}
		}
		
		moves[j].valid = 0;
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
				if (piece->col)
					board->wh_pieceCount--;
				else
					board->bl_pieceCount--;
				board->nPositions = 0;
			}
		}
	}
	else if(piece->ptemplate->abbreviation == 'K') // castling
	{
		switch (move->x1)
		{
		case 2:{
			board->squares[3 + piece->col * 56] = board->squares[piece->col * 56];
			board->squares[piece->col * 56] = 0;

			board->pieces[24 * piece->col].x = 3;
			board->pieces[24 * piece->col].moved = 1;
			break;
		}
		case 6: {

			board->squares[5 + piece->col * 56] = board->squares[7+ piece->col * 56];
			board->squares[7 + piece->col * 56] = 0;

			board->pieces[24 * piece->col + 7].x = 5;
			board->pieces[24 * piece->col + 7].moved = 1;
			break;
		}
		default:
			break;
		}
	}
}

#define gotoxy(x,y) printf("\033[%d;%dH", (int)(y), (int)(x))
void print_board(_BOARD* board, int _y) {
	if (_y >= 0) {
		gotoxy(0, _y);
	}
	for (size_t y = 7; y < -1; y--)
	{
		for (size_t x = 0; x < 8; x++)
		{
			int i = y * 8 + x;
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
			if (i % 8 == 7)
				printf("\n");
		}
	}
}

int isDrawStd(_BOARD* board) {
	char draw;

	if ((board->bl_pieceCount < 3) && (board->wh_pieceCount < 3)){ //not enough material
		char bl_NotEnough = 0;
		char wh_NotEnough = 0;

		draw = 1;

		for (int i = 0; i < board->nPieces; i++){
			if (board->pieces[i].present && !board->pieces[i].ptemplate->king){
				if ((board->pieces[i].ptemplate->abbreviation == 'N') || (board->pieces[i].ptemplate->abbreviation == 'B')){
					if (board->pieces[i].col){
						if (wh_NotEnough) 
							return 1;
						bl_NotEnough = 1;
					}else{
						if (bl_NotEnough) 
							 return 1;
						wh_NotEnough = 1;
					}
				} else {
					draw = 0;
				}
			}
		}

		if(draw)
			return draw;
	}

	char rocc = 0;
	for (size_t i = 0; i < board->nPositions - 1 && board->nPositions; i++) //reoccuring position
	{
		char c = 1;
		for (unsigned char j = 0; j < 64; j++)
		{
			
			if (!(board->positions[64 * i + j] == board->positions[(board->nPositions - 1) * 64 + j])){
				c = 0;
				break;
			}
		}

		if (c)
			rocc++;

	}
	if (rocc > 1){
		return 2;
	}

	// mate TODO
	int nMoves = 0;
	MOVE* moves = NULL;
	MOVE_CONTAINER mc = getAllMoves(board);
	moves = mc.moves;
	if (mc.nMoves == 0) {

		for (size_t i = 0; i < board->nCheckLines; i++)
		{
			int index = board->checkLines[i].move.y1 * 8 + board->checkLines[i].move.x1;

			if (board->squares[index] && (board->pieces[board->squares[index] & 0b1111111].ptemplate->king)) {
				return board->turn ? 5 : 6; // checkmate
			}
		}

		return board->turn ? 3 : 4; // stalemate
	}


	//for (size_t i = 0; i < board->nPieces; i++)
	//{
	//	if (board->pieces[i].present && (board->pieces[i].col == board->turn)) {
	//		moves = getPossibleMoves(board->pieces + i, board, 1);
	//		if (moves && moves[0].valid) {
	//			nMoves++;
	//		}
	//		/*if (moves)
	//			free(moves);*/
	//	}
	//}

	//if (!nMoves) {
	//	for (size_t i = 0; i < board->nPieces; i++)
	//	{
	//		if (board->pieces[i].present && (board->pieces[i].col != board->turn)) {
	//			moves = getPossibleMoves(board->pieces + i, board, 1);
	//			for (int j = 0; moves && moves[j].valid; j++) {
	//				if (board->squares[8 * moves[j].y1 + moves[j].x1] && board->pieces[board->squares[8 * moves[j].y1 + moves[j].x1] & 0b1111111].ptemplate->king) {
	//					free(moves);
	//					return board->turn ? 5 : 6;
	//				}
	//			}
	//		}
	//	}
	//	if (moves)
	//		free(moves);
	//	return board->turn ? 3 : 4;
	//}

	return 0; //gamestate is not a draw
}

void savePos(_BOARD* board) {
	if (!board->nPositions) {
		if (board->positions)
			free(board->positions);

		board->positions = (unsigned char*)malloc(64);
		if (!board->positions)
			return;
	}
	else {
		unsigned char* realloced = (unsigned char*)realloc(board->positions, 64 * (board->nPositions + 1));
		if (!realloced)
			return;

		board->positions = realloced;
	}

	for (size_t i = 0; i < 64; i++)
	{
		board->positions[((unsigned long)64 * board->nPositions) + i] = board->squares[i] ? (1 << 7 | ((1 << 6) * board->pieces[board->squares[i] & 0b1111111].col) | (unsigned char)(board->pieces[board->squares[i] & 0b1111111].ptemplate - board->game.pieceTypes)) : 0;
	}

	board->nPositions++;
}

void freeBoard(_BOARD* board) {
	if (board) {
		if (board->moves) free(board->moves);
		if (board->pieces) free(board->pieces);
		if (board->positions) free(board->positions);
		if (board->checkLines) free(board->checkLines);
		free(board);
	}
}

void addCheckLine(_BOARD* board, CHECKLINE cl) {
	if (!board->nCheckLines) {
		if (board->checkLines)
			free(board->checkLines);

		board->checkLines = (CHECKLINE*)malloc(sizeof(CHECKLINE) * 10);
		if (!board->checkLines) {
			printf("There is some problem with the heap, please restart.\nIf this problem occurs repeatedly, restart your machine.");
			system("pause");
			return;
		}
	}
	else {
		if (board->nCheckLines % 10 == 0) {
			void* realloced = realloc(board->checkLines, (board->nCheckLines + 10) * sizeof(CHECKLINE));
			if (!realloced) {
				printf("There is some problem with the heap, please restart.\nIf this problem occurs repeatedly, restart your machine.");
				system("pause");
				return;
			}
			board->checkLines = realloced;
		}
	}

	board->checkLines[board->nCheckLines++] = cl;

}

void removeCheckLine(_BOARD* board, int index) {

	for (size_t i = index; i < board->nCheckLines - 1; i++)
	{
		board->checkLines[i] = board->checkLines[i + 1];
	}
	board->nCheckLines--;
}

void initCheckLines(_BOARD* board) {
	PIECE* king = board->pieces;

	// searching first king
	while (!king->ptemplate->king)
		king++;

	// creating checkLines that point directly to the king
	createCheckLineTarget(board, king->x, king->y, king->col);

	// creating checkLines that point to squares neighbouring the king
	MOVE_CONTAINER moves = getPossibleMoves(king, board, 0, 1);
	for (size_t i = 0; i < moves.nMoves; i++)
	{
		createCheckLineTarget(board, moves.moves[i].x1, moves.moves[i].y1, king->col);
	}

	free(moves.moves);


	// searching the second king
	king++;
	while (!king->ptemplate->king)
		king++;

	// rinse and repeat

	createCheckLineTarget(board, king->x, king->y, king->col);

	moves = getPossibleMoves(king, board, 0, 1);
	for (size_t i = 0; i < moves.nMoves; i++)
	{
		createCheckLineTarget(board, moves.moves[i].x1, moves.moves[i].y1, king->col);
	}

	free(moves.moves);
}