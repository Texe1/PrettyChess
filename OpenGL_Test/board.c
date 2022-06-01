#include "Board.h"


#define gotoxy(x,y) printf("\033[%d;%dH", (int)(y), (int)(x))
void printBoard(BOARD* board) {
	gotoxy(0, 0);
	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{

			char piece = board->squares[y * 8 + x].piece;

			if (board->selected && (y * 8 + x) == (board->selected & 0b111111)) printf("\033[48;2;0;0;255m");
			else if (board->squares[y * 8 + x].color) {
				if (piece) printf("\033[48;2;255;0;0m");
				else if (!piece && (PIECE_TYPE(board->squares[(board->selected) & 0b111111].piece) == PAWN) && x != (board->selected % 8)) printf("\033[48;2;255;0;0m");
				else printf("\033[48;2;0;255;0m");
			}
			else if (board->lastMove && ((board->lastMove & 0b111111) == (y * 8 + x))) printf("\033[48;2;100;100;0m");
			else if ((x + y) & 1)
				printf("\033[48;2;209;139;071m");
			else
				printf("\033[48;2;255;206;158m");
			if (piece & (1 << 7))
				printf("\033[30m");
			else
				printf("\033[97m");

			char type = PIECE_TYPE(piece);

			switch (type)
			{
			case PAWN:
				printf("P ");
				break;
			case BISHOP:
				printf("B ");
				break;
			case KNIGHT:
				printf("N ");
				break;
			case ROOK:
				printf("R ");
				break;
			case QUEEN:
				printf("Q ");
				break;
			case KING:
				printf("K ");
				break;
			default:
				printf("  ");
				break;
			}
		}
		if (y == 4) { // printing score
			printf("\033[0m");
			if (board->score == -128) printf("+infinity");
			else if (board->score == 127) printf("-infinity");
			else if (board->score > 0) printf("+%d ", (int)board->score);
			else if (board->score == 0) printf("\xf1%d", 0);
			else printf("%d ", (int)board->score);
		}
		printf("\n");
	}

	printf("\033[0m");
	if (board->evolve) {
		printf("\n    \033[30m\033[48;2;209;139;071mQ \033[48;2;255;206;158mR \033[48;2;209;139;071mB \033[48;2;255;206;158mN ");
	}
	else
	{
		printf("\n            ");
	}
}


void fillBoard(BOARD* board) {

	board->squares[0x00].piece = ROOK | BLACK;
	board->squares[0x01].piece = KNIGHT | BLACK;
	board->squares[0x02].piece = BISHOP | BLACK;
	board->squares[0x03].piece = QUEEN | BLACK;
	board->squares[0x04].piece = KING | BLACK;
	board->squares[0x05].piece = BISHOP | BLACK;
	board->squares[0x06].piece = KNIGHT | BLACK;
	board->squares[0x07].piece = ROOK | BLACK;
	board->squares[0x08].piece = PAWN | BLACK;
	board->squares[0x09].piece = PAWN | BLACK;
	board->squares[0x0a].piece = PAWN | BLACK;
	board->squares[0x0b].piece = PAWN | BLACK;
	board->squares[0x0c].piece = PAWN | BLACK;
	board->squares[0x0d].piece = PAWN | BLACK;
	board->squares[0x0e].piece = PAWN | BLACK;
	board->squares[0x0f].piece = PAWN | BLACK;


	board->squares[0x30].piece = PAWN;
	board->squares[0x31].piece = PAWN;
	board->squares[0x32].piece = PAWN;
	board->squares[0x33].piece = PAWN;
	board->squares[0x34].piece = PAWN;
	board->squares[0x35].piece = PAWN;
	board->squares[0x36].piece = PAWN;
	board->squares[0x37].piece = PAWN;
	board->squares[0x38].piece = ROOK;
	board->squares[0x39].piece = KNIGHT;
	board->squares[0x3a].piece = BISHOP;
	board->squares[0x3b].piece = QUEEN;
	board->squares[0x3c].piece = KING;
	board->squares[0x3d].piece = BISHOP;
	board->squares[0x3e].piece = KNIGHT;
	board->squares[0x3f].piece = ROOK;

	board->b_King = 1;
	board->b_Queens = 1;
	board->b_Knights = 2;
	board->b_Bishops = 2;
	board->b_Rooks = 2;
	board->b_Pawns = 8;
	board->w_King = 1;
	board->w_Queens = 1;
	board->w_Knights = 2;
	board->w_Bishops = 2;
	board->w_Rooks = 2;
	board->w_Pawns = 8;

	board->turn = 0;
	board->evolve = 0;

	board->blackKing = 4;
	board->whiteKing = 60;
}

int selectInBoard(BOARD* board, int x, int y) {

	if (((board->turn == 0) == IS_BLACK(board->squares[y * 8 + x].piece)) || !(board->squares[y * 8 + x].piece))
		//board->selected = 0;
		return 1;

	if (x > 7 || y > 7) return 1;

	board->squares[8 * y + x].piece |= (1 << 6);
	board->selected = 8 * y + x | 0b1000000;


	// checking more specific to get more efficient
	switch (PIECE_TYPE(board->squares[8 * y + x].piece))
	{
	case PAWN:
		if (IS_BLACK(board->squares[8 * y + x].piece)) {
			if ((testMove(board, x, y, x, y + 1) == 0))		// moving 1
				board->squares[8 * y + x + 8].color = 1;
			if ((testMove(board, x, y, x - 1, y + 1) == 0))	// capture left
				board->squares[8 * y + x + 7].color = 1;
			if ((testMove(board, x, y, x + 1, y + 1) == 0))	// capture right
				board->squares[8 * y + x + 9].color = 1;
			if ((testMove(board, x, y, x, y + 2) == 0))		// moving 2
				board->squares[8 * y + x + 16].color = 1;
		}
		else {
			if ((testMove(board, x, y, x, y - 1) == 0)) {
				// moving 1
				board->squares[8 * y + x - 8].color = 1;
			}
			if ((testMove(board, x, y, x - 1, y - 1) == 0)) {
				// capture left
				board->squares[8 * y + x - 9].color = 1;
			}
			if ((testMove(board, x, y, x + 1, y - 1) == 0)) {
				// capture right
				board->squares[8 * y + x - 7].color = 1;
			}
			if ((testMove(board, x, y, x, y - 2) == 0)) {
				// moving 2
				board->squares[8 * y + x - 16].color = 1;
			}
		}
		break;
	case KING:
		if ((testMove(board, x, y, x - 1, y - 1) == 0))
			board->squares[8 * y + x - 9].color = 1;
		if ((testMove(board, x, y, x, y - 1) == 0))
			board->squares[8 * y + x - 8].color = 1;
		if ((testMove(board, x, y, x + 1, y - 1) == 0))
			board->squares[8 * y + x - 7].color = 1;
		if ((testMove(board, x, y, x - 1, y) == 0))
			board->squares[8 * y + x - 1].color = 1;
		if ((testMove(board, x, y, x + 1, y) == 0))
			board->squares[8 * y + x + 1].color = 1;
		if ((testMove(board, x, y, x - 1, y + 1) == 0))
			board->squares[8 * y + x + 7].color = 1;
		if ((testMove(board, x, y, x, y + 1) == 0))
			board->squares[8 * y + x + 8].color = 1;
		if ((testMove(board, x, y, x + 1, y + 1) == 0))
			board->squares[8 * y + x + 9].color = 1;
		break;
	default:
		for (size_t y0 = 0; y0 < 8; y0++)
		{
			for (size_t x0 = 0; x0 < 8; x0++)
			{
				if ((testMove(board, x, y, x0, y0) == 0)) {
					// legal move
					board->squares[8 * y0 + x0].color = 1;
				}
				else {
					// illegal move
					board->squares[8 * y0 + x0].color = 0;
				}
			}
		}
		break;
	}

	return 0;
}

void deselectInBoard(BOARD* board) {
	for (size_t i = 0; i < 64; i++)
	{
		board->squares[i].piece &= ~(1 << 6);
		board->squares[i].color = 0;
	}
	board->selected = 0;
}


int testMove(BOARD* board, int x0, int y0, int x1, int y1) {

	int dx = x1 - x0;
	int dy = y1 - y0;


	if ((IS_BLACK(board->squares[y0 * 8 + x0].piece) && (IS_BLACK(board->squares[y1 * 8 + x1].piece) && (board->squares[y1 * 8 + x1].piece)))
		|| (!IS_BLACK(board->squares[y0 * 8 + x0].piece) && (!IS_BLACK(board->squares[y1 * 8 + x1].piece) && (board->squares[y1 * 8 + x1].piece)))
		|| x0 < 0 || x0 > 7 || y0 < 0 || y0 > 7
		|| x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7
		|| (dx == 0 && dy == 0)) {

		return 1;
	}

	/*   // test, wheater a move is illegal because of check
	int testMove(BOARD* board, int x0, int y0, int x1, int y1, boolean checkCheck) {
	//x4, y4 = enemy kings position(board->selected)
	if (!checkCheck){
		for( int x3 = 0 ; x3 < 8 ; x3 ++ ){
			for( int y3 = 0 ; y3 < 8 ; y3 ++){
				if (testMove(board, x3, y3, x4, y4, true){
					return 0;
				}
			}
		}
	}    //TODO differentiate between draw and checkmate when no legal moves are available ->boolean/bit check?
	*/

	switch (board->squares[y0 * 8 + x0].piece & 0b111) {
	case PAWN:
		if (IS_BLACK(board->squares[y0 * 8 + x0].piece)) {	//black piece	moving 2												|					moving		1										|				taking																							|				en passant
			return !(((y0 == 1 && y1 == 3) && (dx == 0) && !(board->squares[16 + x0].piece) && (!(board->squares[8 * y1 + x1].piece))) || ((dy == 1) && (((dx == 0) && (!(board->squares[y1 * 8 + x1].piece))) || (abs(dx) == 1) && ((!(IS_BLACK(board->squares[y1 * 8 + x1].piece)) && (board->squares[y1 * 8 + x1].piece)) || (((!IS_BLACK(board->squares[y0 * 8 + x1].piece))) && (EN_PASSANT(board->squares[y0 * 8 + x1].piece)))))));

		}
		else {													//white piece	moving 2												|					moving		1										|				taking																							|				en passant 
			return !(((y0 == 6 && y1 == 4) && (dx == 0) && !(board->squares[40 + x0].piece) && (!(board->squares[8 * y1 + x1].piece))) || ((dy == -1) && (((dx == 0) && (!(board->squares[y1 * 8 + x1].piece))) || (abs(dx) == 1) && ((IS_BLACK(board->squares[y1 * 8 + x1].piece)) && (board->squares[y1 * 8 + x1].piece)) || (((IS_BLACK(board->squares[y0 * 8 + x1].piece))) && (EN_PASSANT(board->squares[y0 * 8 + x1].piece))))));
		}
		break;
	case BISHOP:
		if (dx != dy && dx != -dy) {
			return 1;
		}
		int x = x0;
		int y = y0;

		if (dx > 0) x++;
		else x--;
		if (dy > 0) y++;
		else y--;

		while (x != x1 && (x >= 0 && x < 8 && y >= 0 && y < 8)) {
			if (board->squares[y * 8 + x].piece) {
				return 1;
			}
			if (dx > 0) x++;
			else x--;
			if (dy > 0) y++;
			else y--;

		}
		return 0;
	case KNIGHT:
		return !(((abs(dx) == 2) && (abs(dy) == 1)) || ((abs(dy) == 2) && (abs(dx) == 1)));
	case ROOK:
		if (dx == 0) {
			int y = y0;
			if (dy > 0)y++;
			else y--;
			while (y != y1)
			{
				if (board->squares[y * 8 + x0].piece) {
					return 1;
				}

				if (dy > 0)y++;
				else y--;
			}
			return 0;
		}
		else if (dy == 0) {
			int x = x0;
			if (dx > 0)x++;
			else x--;
			while (x != x1)
			{
				if (board->squares[y0 * 8 + x].piece) {
					return 1;
				}

				if (dx > 0)x++;
				else x--;
			}
			return 0;
		}
		break;
	case QUEEN:
		// Rook moves
		if (dx == 0) {
			int y = y0;
			if (dy > 0)y++;
			else y--;
			while (y != y1)
			{
				if (board->squares[y * 8 + x0].piece) {
					return 1;
				}

				if (dy > 0)y++;
				else y--;
			}
			return 0;
		}
		else if (dy == 0) {
			int x = x0;
			if (dx > 0)x++;
			else x--;
			while (x != x1)
			{
				if (board->squares[y0 * 8 + x].piece) {
					return 1;
				}

				if (dx > 0)x++;
				else x--;
			}
			return 0;
		}
		else { // Bishop moves
			if (dx != dy && dx != -dy) {
				return 1;
			}
			int x = x0;
			int y = y0;
			if (dx > 0) x++;
			else x--;
			if (dy > 0) y++;
			else y--;
			while (x != x1 && (x >= 0 && x < 8 && y >= 0 && y < 8)) {
				if (board->squares[y * 8 + x].piece) {
					return 1;
				}
				if (dx > 0) x++;
				else x--;
				if (dy > 0) y++;
				else y--;
			}
		}
		return 0;

	case KING: //without check(mate) or draw
		if ((abs(dx) <= 1) && (abs(dy) <= 1)) {
			for (size_t i = 0; i < board->nCheckLines; i++)
			{
				if (isInCheckLine(board->CheckLines + i, y1 * 8 + x1))
					return 1;
			}
			return 0;
		}
		return 1;
	default:
		return 1;
	}

	return 1;
}

int move(BOARD* board, int x0, int y0, int x1, int y1) {
	if (((board->turn == 0) == IS_BLACK(board->squares[y0 * 8 + x0].piece)) || !(board->squares[y0 * 8 + x0].piece)) {
		return 1;
	}
	if (testMove(board, x0, y0, x1, y1))
		return 1;

	if (PIECE_TYPE(board->squares[board->lastMove & 0b111111].piece) == PAWN) {// setting en passant bit to zero
		board->squares[board->lastMove & 0b111111].piece &= ~(1 << 5);
	}

	if ((x0 != x1) && (!board->squares[y1 * 8 + x1].piece) && (PIECE_TYPE(board->squares[y0 * 8 + x0].piece) == PAWN)) {
		board->squares[y0 * 8 + x1].piece = 0;
		if (IS_BLACK(board->squares[y0 * 8 + x0].piece)) {
			board->score--;
			board->w_Pawns--;
		}
		else
		{
			board->score++;
			board->b_Pawns--;
		}
	}
	if ((PIECE_TYPE(board->squares[y0 * 8 + x0].piece) == PAWN) && (abs(y1 - y0) == 2)) {// setting en passant bit
		board->squares[y0 * 8 + x0].piece |= (1 << 5);
	}

	// capturing
	if (board->squares[y1 * 8 + x1].piece) {
		char black = IS_BLACK(board->squares[y1 * 8 + x1].piece) ? 1 : 0;

		if (board->score != 127 && board->score != -128) {
			switch (PIECE_TYPE(board->squares[y1 * 8 + x1].piece)) {
			case KING:
				board->score = IS_BLACK(board->squares[y1 * 8 + x1].piece) ? 0b10000000 : 0b01111111;
				printf("you broke the Game!\xf4");
				break;
			case QUEEN:
				if (black) {
					board->b_Queens--;
					board->score += 9;
				}
				else {
					board->w_Queens--;
					board->score -= 9;
				}
				break;
			case BISHOP:
				if (black) {
					board->b_Bishops--;
					board->score += 3;
				}
				else {
					board->w_Bishops--;
					board->score -= 3;
				}
				break;
			case KNIGHT:
				if (black) {
					board->b_Knights--;
					board->score += 3;
				}
				else {
					board->w_Knights--;
					board->score -= 3;
				}
				break;
			case ROOK:
				if (black) {
					board->b_Rooks--;
					board->score += 5;
				}
				else {
					board->w_Rooks--;
					board->score -= 5;
				}
				break;
			case PAWN:
				if (black) {
					board->b_Pawns--;
					board->score += 1;
				}
				else {
					board->w_Pawns--;
					board->score -= 1;
				}
				break;

			}
		}
	}

	// actual move
	board->squares[y1 * 8 + x1] = board->squares[y0 * 8 + x0];
	board->squares[y0 * 8 + x0].piece = 0;

	if ((y1 == 0 || y1 == 7) && (PIECE_TYPE(board->squares[y1 * 8 + x1].piece) == PAWN)) {
		board->evolve = 1;
	}

	// 0 1 . 3 4 5 6
	// 0 1 3 4 5 6 .

	// checkLines
	//  testing, if checkLine

	CheckLine cls[5] = { 0 };
	char nAdd = 0;

	int i = y1 * 8 + x1;
	switch (PIECE_TYPE(board->squares[i].piece))
	{
	case PAWN:
		if (IS_BLACK(board->squares[i].piece)) {
			if (i + 18 == board->whiteKing
				|| i + 17 == board->whiteKing
				|| i + 16 == board->whiteKing
				|| i + 10 == board->whiteKing
				|| i + 9 == board->whiteKing
				|| i + 8 == board->whiteKing
				|| i + 1 == board->whiteKing
				|| i - 1 == board->whiteKing) {
				cls[0].dir = 7;
				cls[0].len = 1;
				cls[0].org = i;
				cls[0].direct = (i + 9 == board->whiteKing);
				++nAdd;
			}
			/*if (i - 18 == board->whiteKing
				|| i - 17 == board->whiteKing
				|| i - 16 == board->whiteKing
				|| i + 10 == board->whiteKing
				|| i + 9 == board->whiteKing
				|| i + 8 == board->whiteKing
				|| i + 1 == board->whiteKing
				|| i - 1 == board->whiteKing) {
				if (cls[0].org || cls[0].dir) {
					cls[1].dir = 7;
					cls[1].len = 1;
					cls[1].org = i;
					cls[1].direct = (i + 9 == board->whiteKing);
				} else {
					cls[0].dir = 7;
					cls[0].len = 1;
					cls[0].org = i;
					cls[0].direct = (i + 9 == board->whiteKing);
				}
			}*/
		}
	default:
		break;
	}
	//  testing, if piece was org
	char remove[5];
	char nRemove = 0;

	for (size_t j = 0; j < board->nCheckLines; j++)
	{
		if (board->CheckLines[j].org == y0 * 8 + x0) {
			remove[nRemove++] = j;
			if (nRemove == 5) break;
		}
	}

	addrmCheckLines(board, nRemove, remove, nAdd, cls);


	board->turn ^= 1;
	board->lastMove = (y1 * 8 + x1) | 0b1000000;

	return 0;
}

int evolve(BOARD* board, int n) {

	board->squares[(board->lastMove) & 0b111111].piece &= ~0b111;

	switch (n) {
	case 0:
		board->squares[(board->lastMove) & 0b111111].piece |= QUEEN;

		if (board->turn) {
			board->w_Queens++;
			board->score += 8;
		}
		else {
			board->b_Queens++;
			board->score -= 8;
		}
		break;

	case 1:
		board->squares[(board->lastMove) & 0b111111].piece |= ROOK;
		if (board->turn) {
			board->w_Rooks++;
			board->score += 4;
		}
		else {
			board->b_Rooks++;
			board->score -= 4;
		}
		break;

	case 2:
		board->squares[(board->lastMove) & 0b111111].piece |= BISHOP;

		if (board->turn) {
			board->w_Bishops++;
			board->score += 2;
		}
		else {
			board->b_Bishops++;
			board->score -= 2;
		}
		break;

	case 3:
		board->squares[(board->lastMove) & 0b111111].piece |= KNIGHT;

		if (board->turn) {
			board->w_Knights++;
			board->score += 2;
		}
		else {
			board->b_Knights++;
			board->score -= 2;
		}
		break;

	default:
		board->squares[(board->lastMove) & 0b111111].piece |= PAWN;
		return 1;
	}

	board->evolve = 0;

	return 0;

}

int isInCheckLine(CheckLine* cl, int n) { // tests, if square [n] is in CheckLine [cl]

	int j = cl->org;
	int i = 0;
	if ((n < 0) || (n > 63))
		return 1;

	switch (cl->dir)
	{
	case 0:
		return !(n < j && (n - j) % 9 == 0 && abs(n - j) / 9 <= cl->len && abs(n - j) / 9 <= n % 8);
	case 1:
		return !((n % 8 == j % 8) && (n < j) && (abs(n - j) <= (cl->len * 8)));
	case 2:
		return !(n < j && (n - j) % 7 == 0 && abs(n - j) / 7 <= cl->len && abs(n - j) / 7 <= 8 - (n % 8));
	case 3:
		return !(n / 8 == j / 8 && j > n && abs(j - n) <= cl->len);
	case 4:
		return !(n / 8 == j / 8 && j < n&& abs(j - n) <= cl->len);
	case 5:
		return !(n > j && (n - j) % 7 == 0 && (n - j) / 7 <= cl->len && (n - j) / 7 <= n % 8);
	case 6:
		return !((n % 8 == j % 8) && (n > j) && (abs(n - j) <= (cl->len * 8)));
	case 7:
		return !(n > j && (n - j) % 9 == 0 && ((cl->len == 0) || ((n - j) / 9 <= cl->len)) && (n - j) / 9 <= 8 - (n % 8));
	default:
		return 2;
		break;
	}

	return 1;
}

int addrmCheckLines(BOARD* board, char nRemove, char* remove, char nAdd, CHECKLINE* add) {
	return 1;

	if (nAdd == 0 && nRemove == 0)
		return 0;
	if (nRemove <= nAdd) {
		if (board->CheckLines == NULL) {
			board->CheckLines = malloc(sizeof(CHECKLINE) * (board->nCheckLines + nAdd - nRemove));
			if (board->CheckLines == NULL) {
				printf("malloc doesn't work in addrmCheckLines()\n");
				exit(1);
			}
		}
		else {
			board->CheckLines = realloc(board->CheckLines, sizeof(CHECKLINE) * (board->nCheckLines + nAdd - nRemove));
			if (board->CheckLines == NULL) {
				printf("realloc doesn't work in addrmCheckLines()\n");
				exit(1);
			}
		}

		for (size_t i = 0; i < nAdd; i++)
		{
			if (i < nRemove) {
				board->CheckLines[remove[i]] = add[i];
			}
			else {
				board->CheckLines[board->nCheckLines++] = add[i];
			}
		}
	}
	else {
		for (size_t i = 0; i < nRemove; i++)
		{
			if (i < nAdd) {
				board->CheckLines[remove[i]] = add[i];
			}
			else {
				for (size_t j = remove[i]; j < board->nCheckLines - 1; j++)
				{
					board->CheckLines[j] = board->CheckLines[j + 1];
				}
			}
		}
		board->CheckLines = realloc(board->CheckLines, sizeof(CHECKLINE) * (board->nCheckLines + nAdd - nRemove));
		if (board->CheckLines == NULL) {
			printf("alloc doesn't work in addrmCheckLines()\n");
			exit(1);
		}
	}
}




