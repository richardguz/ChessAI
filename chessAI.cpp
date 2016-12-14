#include <iostream>
#include <vector>
#include <array>

using namespace std;

void printGameboard(array<array<int, 8>, 8> gameBoard);

enum pieces {
	PAWN = "p",
	BISHOP = "b",
	KNIGHT = "n",
	ROOK = "r",
	KING = "k",
	QUEEN = "Q"
};

struct treeNode{
	treeNode* parent;
	int value;
	int gameBoard[8][8];
};

struct Coord{
	Coord(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};


vector<array<array<int, 8>, 8>> validKnightMoves(array<array<int, 8>, 8> gameBoard, Coord position) {
	vector<array<array<int, 8>, 8>> newGameBoards;
	int x = position.x;
	int y = position.y;
	int newx = x;
	int newy = y;
	for (int i = 0; i < 2; i++){
		for (int j = 0; j < 2; j++){
			int changeX = 1;
			int changeY = 2;
			array<array<int, 8>, 8> newGameBoard = gameBoard
			newx = 
		}
	}
	array<array<int, 8>, 8> newGameBoard= gameBoard;
	

	//newGameBoard[4][5] = 4;
	printGameboard(newGameBoard);
	cout << endl;
	printGameboard(gameBoard);
	cout << "Goog";

	return newGameBoards;
}

vector<Coord> findPiece(array<array<int, 8>, 8> gameBoard, int piece) {
	vector<Coord> pieces;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (gameBoard[i][j] == piece){
				Coord pieceCoord = Coord(i, j);
				pieces.push_back(pieceCoord);
			}
		}
	}

	return pieces;
}

void printGameboard(array<array<int, 8>, 8> gameBoard) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << gameBoard[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {
	array<array<int, 8>, 8> gameBoard = {{{0,0,0,0,0,0,0,0}, {0,1,0,0,0,0,0,0}, {0,0,1,0,0,0,0,0}, {0,0,0,0,0,0,1,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0}}};
	cout << "Hi" << endl;
	cout << BISHOP << endl;
	vector<Coord> coords = findPiece(gameBoard, PAWN);
	for (int i = 0; i < coords.size(); i++) {
		cout << "hi" << endl;
	}
	for (std::vector<Coord>::iterator it = coords.begin(); it != coords.end(); ++it){
		cout << it->x << it->y << endl;
	}
	validKnightMoves(gameBoard, coords[0]);
}