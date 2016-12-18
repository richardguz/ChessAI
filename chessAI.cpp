#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ChessAI.h"


using namespace std;

ChessAI::ChessAI(string endpointUrl) {
	egm = new EngineMediator(endpointUrl);
	egm->createGame();
	gameBoard = egm->getGameBoard();
	color = "white";//egm->getColor();

	gameBoard[0][3] = '\0';
	gameBoard[2][4] = 'k';
	gameBoard[3][4] = 'r';
	gameBoard[4][4] = 'Q';
	gameBoard[4][3] = 'K';
	gameBoard[7][3] = '\0';
	getPieces(gameBoard);
	printGameboard(gameBoard);
	cout << myPieces[0].pieceType << endl;
	vector<gameMove> gm = generateKingMoves(myPieces[0]);
	//cout << moveBadState(gm[8]) << endl;
	//gm = pruneBadMoves(gm);
	//pruneBadMoves(generateMoves(myPieces));

	// vector<gameMove> gm = generateKingMoves(myPieces[0]);
	//cout << gm.size() << endl;
	for (int i = 0; i < gm.size(); i++) {
		cout << gm[i].value << endl;
		makeMove(gm[i]);
	}
	// cout << myKing->x << " " << myKing->y << endl;
	//cout << inCheck(4, 4, this->gameBoard, "white") << endl;
}

vector<gameMove> ChessAI::generateMoves(vector<piece> pieces) {
	vector<gameMove> myMoves;
	for (int i = 0; i < pieces.size(); i++) {
		vector<gameMove> pieceMoves;
		piece p = pieces[i];
		switch(tolower(p.pieceType)) {
			case 'p':
				pieceMoves = generatePawnMoves(p);
				break;
			case 'b':
				pieceMoves = generateBishopMoves(p);
				break;
			case 'n':
				pieceMoves = generateKnightMoves(p);
				break;
			case 'r':
				pieceMoves = generateRookMoves(p);
				break;
			case 'q':
				pieceMoves = generateQueenMoves(p);
				break;
			// case 'k':
			// 	pieceMoves = generateKingMoves(p);
			// 	break;
		}
		myMoves.insert(myMoves.end(), pieceMoves.begin(), pieceMoves.end());
	}
	
	return myMoves;
}

vector<gameMove> ChessAI::pruneBadMoves(vector<gameMove> moves) {
	int i = 0;
	while (i < moves.size()) {
		if (moveBadState(moves[i])) {
			moves.erase(moves.begin() + i);
		}
		else {
			++i;
		}
	}
	return moves;
}


void ChessAI::getPieces(array<array<char, 8>, 8> board) {
	myPieces.clear();
	opponentPieces.clear();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (isupper(board[i][j])) {
				piece newPiece = piece(i, j, board[i][j]);
				if (color == "white") {
					if (newPiece.pieceType == 'K'){
						this->myKing = new piece(i, j, board[i][j]);
					}
					myPieces.push_back(newPiece);
				}
				else{
					if (newPiece.pieceType == 'K'){
						this->opponentKing = new piece(i, j, board[i][j]);
					}
					opponentPieces.push_back(newPiece);
				}
			}
			else if (islower(gameBoard[i][j])) {
				piece newPiece = piece(i, j, board[i][j]);
				if (color == "black"){
					if (newPiece.pieceType == 'k'){
						this->myKing = new piece(i, j, board[i][j]);
					}
					myPieces.push_back(newPiece);
				}
				else{
					if (newPiece.pieceType == 'k'){
						this->opponentKing = new piece(i, j, board[i][j]);
					}
					opponentPieces.push_back(newPiece);
				}
			}
		}
	}
}

bool ChessAI::pawnMoved(piece pawn) {
	char pieceType = pawn.pieceType;
	return !(isupper(pieceType) && pawn.x == 6 || islower(pieceType) && pawn.x == 1);
}

bool ChessAI::isBlocked(int x, int y, char pieceType) {
	return (isupper(pieceType) && isupper(this->gameBoard[x][y]) || islower(pieceType) && islower(this->gameBoard[x][y]));
}

bool ChessAI::outOfBounds(int x, int y) {
	return (x > 7 || x < 0 || y > 7 || y < 0);
}

bool ChessAI::inCheck(int x, int y, array<array<char, 8>, 8> board, string kingColor) {
	//color = color == "white" ? "black" : "white";
	bool kingInCheck = false;
	array<array<char, 8>, 8> saveGameboardState = this->gameBoard;
	this->gameBoard = board;

	board[x][y] = kingColor == "white" ? 'K' : 'k';
	vector<gameMove> opponentMoves = kingColor == color ? generateMoves(opponentPieces) : generateMoves(myPieces) ;
	for (int i = 0; i < opponentMoves.size(); i++) {
		if (tolower(opponentMoves[i].pieceType) == 'p') {
			if (opponentMoves[i].x2 == x && opponentMoves[i].y2 == y && opponentMoves[i].y1 != opponentMoves[i].y2) {
				kingInCheck = true;
				break;
			}
		}
		else if (opponentMoves[i].x2 == x && opponentMoves[i].y2 == y) {
			kingInCheck  = true;
			break;
		}
	}
	this->gameBoard = saveGameboardState;
	return kingInCheck;
}

bool ChessAI::kingsKissing(int x, int y, piece oppositeKing) {
	int xDiff = abs(x - oppositeKing.x);
	int yDiff = abs(y - oppositeKing.y);
	if (yDiff <= 1 && xDiff <= 1){
		return true;
	}
	return false;
}

bool ChessAI::moveBadState(gameMove m) {
	array<array<char, 8>, 8> tempGameboard = gameBoard;
	tempGameboard[m.x1][m.y1] = '\0';
	tempGameboard[m.x2][m.y2] = m.pieceType;
	getPieces(tempGameboard);
	string moveColor = isupper(m.pieceType) ? "white" : "black";
	int kingX = moveColor == color ? myKing->x : opponentKing->x;
	int kingY = moveColor == color ? myKing->y : opponentKing->y;
	if (inCheck(kingX, kingY, tempGameboard, "white"))
		return true;
	printGameboard(tempGameboard);
	return false;
}

bool ChessAI::isValidPawnMove(gameMove m) {
	int y = m.y1;
	int xDiff = m.x1 - m.x2;
	if (outOfBounds(m.x2, m.y2))
		return false;

	if (isupper(m.pieceType)) {
		for (int i = m.x2; i < m.x1; i++) {
			if (this->gameBoard[i][y] != '\0')
				return false;
		}
	}
	if (islower(m.pieceType)) {
		for (int i = m.x1 + 1; i <= m.x2; i++) {
			if (this->gameBoard[i][y] != '\0')
				return false;
		}
	}

	if (m.y1 != m.y2){
		if (!islower(gameBoard[m.x2][m.y2]) && isupper(m.pieceType))
			return false;
		if (!isupper(gameBoard[m.x2][m.y2]) && islower(m.pieceType))
			return false;
	}

	return true;
}

bool ChessAI::isValidKnightMove(gameMove m) {
	if (outOfBounds(m.x2, m.y2)){
		return false;
	}
	return !(isBlocked(m.x2, m.y2, m.pieceType));
}

double ChessAI::valueGained(char p) {
	p = tolower(p);
	switch(p) {
		case 'p': 
			return 1;
			break;
		case 'b':
			return 3;
			break;
		case 'n':
			return 3.5;
			break;
		case 'r':
			return 5;
			break;
		case 'q':
			return 9;
			break;
	}

	return 0;
}

vector<gameMove> ChessAI::generatePawnMoves(piece pawn) {
	vector<gameMove> moves;
	int oldX = pawn.x;
	int oldY = pawn.y;
	int newX = 0;
	int newY = 0;
	int offset = isupper(pawn.pieceType) ? - 1 : 1;
	if (!pawnMoved(pawn)) {
		newX = oldX + (offset * 2);
		newY = oldY;
		gameMove m = gameMove(oldX, oldY, newX, newY, pawn.pieceType, 0);
		if (isValidPawnMove(m)) {
			moves.push_back(m);
		}
	}

	newX = oldX + offset;
	newY = oldY;
	gameMove m = gameMove(oldX, oldY, newX, newY, pawn.pieceType, 0);
	if (isValidPawnMove(m)) {
		moves.push_back(m);
	}

	for (int i = 0; i < 2; i++) {
		newX = oldX + offset;
		newY = i == 1 ? oldY + 1 : oldY - 1;
		double value = valueGained(gameBoard[newX][newY]);
		gameMove mCapture = gameMove(oldX, oldY, newX, newY, pawn.pieceType, value);
		if (isValidPawnMove(mCapture)) {
			moves.push_back(mCapture);
		}
	}

	return moves;
}

vector<gameMove> ChessAI::generateKnightMoves(piece knight) {
	vector<gameMove> tempMoves;
	vector<gameMove> moves;
	int oldX = knight.x;
	int oldY = knight.y;

	for (int i = 0; i < 4; i++) {
			int offsetX = i % 2 ? 1 : -1;
			int offsetY = i > 1 ? 1 : -1;
			int newX1 = oldX + offsetX * 1;
			int newY1 = oldY + offsetY * 2;
			double value1 = valueGained(gameBoard[newX1][newY1]);
			tempMoves.push_back(gameMove(oldX, oldY, newX1, newY1, knight.pieceType, value1));

			int newX2 = oldX + offsetX * 2;
			int newY2 = oldY + offsetY * 1;
			double value2 = valueGained(gameBoard[newX2][newY2]);
			tempMoves.push_back(gameMove(oldX, oldY, newX2, newY2, knight.pieceType, value2));
	}

	for (int i = 0; i < 8; i++) {
		gameMove m = tempMoves[i];
		if (isValidKnightMove(m))
			moves.push_back(m);
	}

	return moves;
}

vector<gameMove> ChessAI::generateBishopMoves(piece bishop) {
	vector<gameMove> moves;
	int oldX = bishop.x;
	int oldY = bishop.y;
	

	for (int i = 0; i < 4; i++) {
		bool done = false;
		int offsetX = i % 2 ? 1 : -1;
		int offsetY = i > 1 ? 1 : -1;

		int newX = oldX + offsetX;
		int newY = oldY + offsetY;
		while (!isBlocked(newX, newY, bishop.pieceType) && !outOfBounds(newX, newY) && !done) {
			if (gameBoard[newX][newY] != '\0')
				done = true;
			double value = valueGained(gameBoard[newX][newY]);
			moves.push_back(gameMove(oldX, oldY, newX, newY, bishop.pieceType, value));
			newX += offsetX;
			newY += offsetY;
		}
	}
	return moves;
}

vector<gameMove> ChessAI::generateRookMoves(piece rook) {
	vector<gameMove> moves;
	int oldX = rook.x;
	int oldY = rook.y;

	for (int i = 0; i < 4; i++) {
		bool done = false;
		int offset = i % 2 ? 1 : -1;

		int newX = oldX;
		int newY = oldY;
		if (i < 2) {
			newX += offset;
		}
		else {
			newY += offset;
		}

		while (!isBlocked(newX, newY, rook.pieceType) && !outOfBounds(newX, newY) && !done) {
			if (gameBoard[newX][newY] != '\0')
				done = true;
			double value = valueGained(gameBoard[newX][newY]);
			moves.push_back(gameMove(oldX, oldY, newX, newY, rook.pieceType, value));
			if (i < 2) {
			newX += offset;
			}
			else {
				newY += offset;
			}
		}
	}
	return moves;
}

vector<gameMove> ChessAI::generateQueenMoves(piece queen) {
	vector<gameMove> bishopMoves = generateBishopMoves(queen);
	vector<gameMove> rookMoves = generateRookMoves(queen);
	bishopMoves.insert( bishopMoves.end(), rookMoves.begin(), rookMoves.end());

	return bishopMoves;
}

vector<gameMove> ChessAI::generateKingMoves(piece king) {
	vector<gameMove> moves;
	int oldX = king.x;
	int oldY = king.y;
	string kingColor = isupper(king.pieceType) ? "white" : "black";
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			int newX = oldX + i;
			int newY = oldY + j;
			piece oppositeKing = kingColor == color ? *opponentKing : *myKing;
			if (!isBlocked(newX, newY, king.pieceType) && !inCheck(newX, newY, gameBoard, kingColor) && !kingsKissing(newX, newY, oppositeKing)) {
				double value = valueGained(gameBoard[newX][newY]);
				moves.push_back(gameMove(oldX, oldY, newX, newY, king.pieceType, value));
			}
		}
	}

	return moves;
}



void ChessAI::makeMove(gameMove m) {
	char movedPiece = gameBoard[m.x1][m.y1];
	char removedPiece = gameBoard[m.x2][m.y2];
	gameBoard[m.x1][m.y1] = '\0';
	gameBoard[m.x2][m.y2] = m.pieceType;
	printGameboard(gameBoard);
	gameBoard[m.x1][m.y1] = movedPiece;
	gameBoard[m.x2][m.y2] = removedPiece;
}
// vector<array<array<int, 8>, 8>> validKnightMoves(array<array<int, 8>, 8> gameBoard, Coord position) {
// 	vector<array<array<int, 8>, 8>> newGameBoards;
// 	int x = position.x;
// 	int y = position.y;
// 	int newX = x;
// 	int newY = y;
// 	for (int i = 0; i < 2; i++){
// 		for (int j = 0; j < 2; j++){
// 			int changeX = 1;
// 			int changeY = 2;
// 			array<array<int, 8>, 8> newGameBoard = gameBoard
// 			newx = 
// 		}
// 	}
// 	array<array<int, 8>, 8> newGameBoard= gameBoard;
	

// 	//newGameBoard[4][5] = 4;
// 	printGameboard(newGameBoard);
// 	cout << endl;
// 	printGameboard(gameBoard);
// 	cout << "Goog";

// 	return newGameBoards;
// }

// vector<Coord> findPiece(array<array<int, 8>, 8> gameBoard, int piece) {
// 	vector<Coord> pieces;
// 	for (int i = 0; i < 8; i++) {
// 		for (int j = 0; j < 8; j++) {
// 			if (gameBoard[i][j] == piece){
// 				Coord pieceCoord = Coord(i, j);
// 				pieces.push_back(pieceCoord);
// 			}
// 		}
// 	}

// 	return pieces;
// }

std::array<std::array<char, 8>, 8> ChessAI::getGameBoard(){
	return gameBoard;
}


int main() {
	// string url;
 //  	cout << "Please Specify the url of the chess engine: ";
 // 	cin >> url;

 // 	int option = 0;
 // 	cout << "Please choose an option, press 1 to create a game or 2 to join an existing game: ";
 // 	cin >> option;

	ChessAI cai = ChessAI("http://localhost:3000/");
}