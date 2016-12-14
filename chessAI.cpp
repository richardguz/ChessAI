#include <iostream>
#include "ChessAI.h"


using namespace std;

ChessAI::ChessAI(string endpointUrl) {
	egm = new EngineMediator(endpointUrl);
	egm->createGame();
	gameBoard = egm->getGameBoard();
	color = egm->getColor();

	gameBoard[7][0] = '\0';
	gameBoard[5][5] = 'R';
	checkPieces();
	vector<gameMove> gm = generateRookMoves(myPieces[0]);
	cout << gm.size() << endl;
	cout << gm[0].x2 << " " << gm[0].y2 << endl;
	for (int i = 0; i < gm.size(); i++) {
		makeMove(gm[i]);
	}
}

void ChessAI::checkPieces() {
	myPieces.clear();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (color == "white" && isupper(gameBoard[i][j])) {
				piece newPiece = piece(i, j, gameBoard[i][j]);
				myPieces.push_back(newPiece);
			}
			else if (color == "black" && islower(gameBoard[i][j])) {
				piece newPiece = piece(i, j, gameBoard[i][j]);
				myPieces.push_back(newPiece);
			}
		}
	}
}

bool ChessAI::pawnMoved(piece pawn) {
	return !(this->color == "white" && pawn.x == 6 || this->color == "black" && pawn.x == 1);
}

bool ChessAI::isBlocked(int x, int y) {
	return (this->color == "white" && isupper(this->gameBoard[x][y]) || this->color == "black" && islower(this->gameBoard[x][y]));
}

bool ChessAI::outOfBounds(int x, int y) {
	return (x > 7 || x < 0 || y > 7 || y < 0);
}

bool ChessAI::isValidPawnMove(gameMove m) {
	int y = m.y1;
	int xDiff = m.x1 - m.x2;
	if (outOfBounds(m.x2, m.y2))
		return false;

	if (this->color == "white") {
		for (int i = m.x2; i < m.x1; i++) {
			cout << i << endl;
			if (isupper(this->gameBoard[i][y]))
				return false;
		}
	}
	if (this->color == "black") {
		for (int i = m.x1 + 1; i <= m.x2; i++) {
			if (isupper(this->gameBoard[i][y]))
				return false;
		}
	}

	return true;
}

bool ChessAI::isValidKnightMove(gameMove m) {
	if (outOfBounds(m.x2, m.y2)){
		return false;
	}
	return !(isBlocked(m.x2, m.y2));
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
	//gameMove m;
	vector<gameMove> moves;
	int oldX = pawn.x;
	int oldY = pawn.y;
	int newX = 0;
	int newY = 0;
	if (!pawnMoved(pawn)) {
		newX = oldX - 2;
		newY = oldY;
		double value = valueGained(gameBoard[newX][newY]);
		gameMove m = gameMove(oldX, oldY, newX, newY, pawn.pieceType, value);
		if (isValidPawnMove(m)) {
			moves.push_back(m);
		}
	}

	newX = oldX - 1;
	newY = oldY;
	cout << newX << newY << endl;
	double value = valueGained(gameBoard[newX][newY]);
	gameMove m = gameMove(oldX, oldY, newX, newY, pawn.pieceType, value);
	if (isValidPawnMove(m)) {
		moves.push_back(m);
	}
	return moves;
}

vector<gameMove> ChessAI::generateKnightMoves(piece knight) {
	vector<gameMove> tempMoves;
	vector<gameMove> moves;
	int oldX = knight.x;
	int oldY = knight.y;

	cout << oldX << " " << oldY << endl;
	for (int i = 0; i < 4; i++) {
			int offsetX = i % 2 ? 1 : -1;
			int offsetY = i > 1 ? 1 : -1;
			cout << offsetX << " " << offsetY << endl;
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
		int offsetX = i % 2 ? 1 : -1;
		int offsetY = i > 1 ? 1 : -1;

		int newX = oldX + offsetX;
		int newY = oldY + offsetY;
		while (!isBlocked(newX, newY) && !outOfBounds(newX, newY)) {
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
		int offset = i % 2 ? 1 : -1;

		int newX = oldX;
		int newY = oldY;
		if (i < 2) {
			newX += offset;
		}
		else {
			newY += offset;
		}

		while (!isBlocked(newX, newY) && !outOfBounds(newX, newY)) {
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

void ChessAI::makeMove(gameMove m) {
	gameBoard[m.x1][m.y1] = '\0';
	gameBoard[m.x2][m.y2] = m.pieceType;
	printGameboard(gameBoard);
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
	ChessAI cai = ChessAI("http://localhost:3000/");
}