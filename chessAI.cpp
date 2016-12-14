#include <iostream>
#include <vector>
#include <array>
#include <string>


#include "Utils.h"
#include "ChessAI.h"


using namespace std;

ChessAI::ChessAI(string endpointUrl) {
	egm = new EngineMediator(endpointUrl);
	egm->createGame();
}

// vector<array<array<int, 8>, 8>> validKnightMoves(array<array<int, 8>, 8> gameBoard, Coord position) {
// 	vector<array<array<int, 8>, 8>> newGameBoards;
// 	int x = position.x;
// 	int y = position.y;
// 	int newx = x;
// 	int newy = y;
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

int main() {
	ChessAI cai = ChessAI("http://localhost:3000/");
}