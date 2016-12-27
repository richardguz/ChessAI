#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <climits>
#include <chrono>
#include <thread> 
#include "ChessAI.h"


using namespace std;

ChessAI::ChessAI(string endpointUrl) {
	egm = new EngineMediator(endpointUrl);
	egm->createGame();
	gameBoard = egm->getGameBoard();
	color = egm->getColor();
	
	recursionDepth = 0;
	turnNumber = 0;

	// gameBoard[0] = {'r','\0','\0','k','\0','\0','\0','\0'};
	// gameBoard[1] = {'p','\0','\0','\0','\0','p','\0','Q'};
	// gameBoard[2] = {'\0','b','\0','\0','\0','r','\0','\0'};
	// gameBoard[3] = {'\0','\0','\0','\0','\0','\0','\0','b'};
	// gameBoard[4] = {'\0','\0','\0','p','\0','\0','\0','P'};
	// gameBoard[5] = {'\0','p','\0','P','\0','P','\0','\0'};
	// gameBoard[6] = {'\0','\0','q','\0','B','\0','\0','\0'};
	// gameBoard[7] = {'R','N','\0','K','\0','\0','N','R'};
	
	
	// getPieces(gameBoard);
	
	
	
	
	// gameMove emptyMove;
	// vector<treeNode*> children;
	// treeNode* root = new treeNode(children, emptyMove, 0, gameBoard); 
	// generateMoveTree(gameBoard, root);
	// cout << root->children[0]->children.size() << endl;

	// while (true){
	// 	printGameboard(root->gameBoard);
	// 	cout << root->children.size() << endl;
	// 	if (root->children.size() != 0){
	// 		random_device rd; // obtain a random number from hardware
	//     	mt19937 eng(rd()); // seed the generator
	//     	uniform_int_distribution<> distr(0, root->children.size()); // define the range
	// 		root = root->children[distr(eng)];
	// 	}
	// 	else {
	// 		cout << "BREAKING BAD" << endl;
	// 		break;
	// 	}
	// }
	//printGameboard(root->children[0]->children[0]->children[0]->gameBoard);
	// gameBoard[0][3] = '\0';
	// gameBoard[2][4] = 'k';
	// gameBoard[3][4] = 'r';
	// gameBoard[4][4] = 'Q';
	// gameBoard[4][3] = 'K';
	// gameBoard[7][3] = '\0';
	// getPieces(gameBoard);
	// printGameboard(gameBoard);
	// cout << myPieces[7].pieceType << endl;
	// vector<gameMove> gm = generateKingMoves(myPieces[7], gameBoard);
	// gm = pruneBadMoves(gm);
	// for (int i = 0; i < gm.size(); i++) {
	// 	cout << gm[i].value << endl;
	// 	makeMove(gm[i], gameBoard);
	// }
}

vector<gameMove> ChessAI::generateEarlyMoves(vector<piece> pieces, array<array<char, 8>, 8> board){
	vector<gameMove> myMoves;
	for (int i = 0; i < pieces.size(); i++) {
		vector<gameMove> pieceMoves;
		piece p = pieces[i];
		switch(tolower(p.pieceType)) {
			case 'p':
				if (p.y > 2 && p.y < 5){
					pieceMoves = generatePawnMoves(p, board);
				}
				break;
			case 'b':
				pieceMoves = generateBishopMoves(p, board);
				break;
			case 'n':
				pieceMoves = generateKnightMoves(p, board);
				break;
		}
		myMoves.insert(myMoves.end(), pieceMoves.begin(), pieceMoves.end());
	}
	
	return myMoves;
}

vector<gameMove> ChessAI::generateMoves(vector<piece> pieces, array<array<char, 8>, 8> board) {
	vector<gameMove> myMoves;
	for (int i = 0; i < pieces.size(); i++) {
		vector<gameMove> pieceMoves;
		piece p = pieces[i];
		switch(tolower(p.pieceType)) {
			case 'p':
				pieceMoves = generatePawnMoves(p, board);
				break;
			case 'b':
				pieceMoves = generateBishopMoves(p, board);
				break;
			case 'n':
				pieceMoves = generateKnightMoves(p, board);
				break;
			case 'r':
				pieceMoves = generateRookMoves(p, board);
				break;
			case 'q':
				pieceMoves = generateQueenMoves(p, board);
				break;
			case 'k':
				pieceMoves = generateKingMoves(p, board);
				break;
		}
		myMoves.insert(myMoves.end(), pieceMoves.begin(), pieceMoves.end());
	}
	
	return myMoves;
}


gameMove ChessAI::chooseMove() {
	vector<gameMove> gm = generateMoves(myPieces, gameBoard);
	int maxValue = 0;
	int index = -1;

    gameBoard = egm->getGameBoard();
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (egm->myTurn()){
    	cout << "Its My Turn!" << endl;
		getPieces(gameBoard);
		

		gameMove emptyMove;
		vector<treeNode*> children;
		//treeNode* root = new treeNode(children, emptyMove, 0, gameBoard); 
		//int bestValue = (int) generateMoveTree(gameBoard, root);
		gameMove gm = MinMax(emptyMove, gameBoard, 4);
		cout << gm.value << endl;
		egm->sendMove(gm);	
		turnNumber+= 1;
		printGameboard(makeMove(gm, gameBoard));
		return gm;
	}
}

gameMove ChessAI::MinMax(gameMove move, array<array<char, 8>, 8> board, short int depth_limit) {
	return MaxMove(move, board, depth_limit, 0);
}

gameMove ChessAI::MaxMove(gameMove lastMove, array<array<char, 8>, 8> board, short int depth_limit, short int depth) {
	vector<gameMove> moves;
	gameMove best_real_move;
	gameMove best_move;
	gameMove move;
	bool firstMove = true;

	getPieces(board);
	if (depth >= depth_limit) {//if depth limit is reached
		lastMove.value = stateValue;
		return lastMove;
	} else {
		vector<piece> pieces = depth % 2 == 0 ? myPieces : opponentPieces;
		if (turnNumber < 3 && depth == 0) {
			moves = generateEarlyMoves(pieces, board);
		}
		else {
			moves = generateMoves(pieces, board);
		}
		for (int i = 0; i < moves.size(); i++) {
			array<array<char, 8>, 8> newGameBoard = makeMove(moves[i], board);
			move = MinMove(moves[i], newGameBoard, depth_limit, depth+1);
			if (firstMove || move.value
					> best_move.value) {
				if (!moveBadState(moves[i], board)){
					firstMove = false;
					best_move = move;
					best_real_move = moves[i];
					best_real_move.value = move.value;
				}
			}
		}
		return best_real_move;
	}
}

gameMove ChessAI::MinMove(gameMove lastMove, array<array<char, 8>, 8> board, short int depth_limit, short int depth) {
	vector<gameMove> moves;
	gameMove best_real_move;
	gameMove best_move;
	gameMove move;
	bool firstMove = true;

	getPieces(board);
	if (depth >= depth_limit) {//if depth limit is reached
		lastMove.value = stateValue;
		return lastMove;
	} else {
		vector<piece> pieces = depth % 2 == 0 ? myPieces : opponentPieces;
		moves = generateMoves(pieces, board);
		for (int i = 0; i < moves.size(); i++) {
			array<array<char, 8>, 8> newGameBoard = makeMove(moves[i], board);
			move = MaxMove(moves[i], newGameBoard, depth_limit, depth+1);
			if (firstMove || move.value
					< best_move.value) {
				if (!moveBadState(moves[i], board)) {
					firstMove = false;
					best_move = move;
					best_real_move = moves[i];
					best_real_move.value = move.value;
				}
			}
		}
		return best_real_move;
	}
}

double ChessAI::generateMoveTree(array<array<char, 8>, 8> board, treeNode* parent){
	getPieces(board);
	bool returnIndex = false;
	double bestValue = INT_MAX;
	int bestValueIndex = 0;
	int randIndex = 0;
	int offset = 0;

	if (recursionDepth == 0) {
		returnIndex = true;
	}
	int preserveRecursionDepth = recursionDepth;
	if (recursionDepth < 4){
		int turnColor = recursionDepth % 2 == 0 ? WHITE : BLACK;
		bool opponentMove = turnColor != color;
		vector<piece> pieces = recursionDepth % 2 == 0 ? myPieces : opponentPieces;
		recursionDepth++;

		vector<gameMove> gm;
		if (turnNumber < 3) {
			gm = generateEarlyMoves(pieces, board);
		}
		else {
			gm = generateMoves(pieces, board);
		}
		if (recursionDepth <= 2) {
			if (inCheck(myKing.x, myKing.y, board, turnColor)){
				offset == -2;
				gm = pruneBadMoves(gm);
			}
		}
		
		random_device rd; // obtain a random number from hardware
		mt19937 eng(rd()); // seed the generator
		uniform_int_distribution<> distr(0, gm.size()); // define the range
		randIndex = distr(eng);

		for (int i = 0; i < gm.size(); i++) {
			array<array<char, 8>, 8> newGameBoard = makeMove(gm[i], board);
			
			vector<treeNode*> children;
			treeNode* child = new treeNode(children, gm[i], gm[i].value, newGameBoard);
			parent->children.push_back(child);
			double childValue = generateMoveTree(newGameBoard, child);
			child->value = childValue;
			if (opponentMove && bestValue > childValue || bestValue == INT_MAX) {
				if (!moveBadState(gm[i], newGameBoard)){
					bestValue = childValue;
					bestValueIndex = i;
				}
			}
			if (!opponentMove && bestValue < childValue || bestValue == INT_MAX) {
				if (!moveBadState(gm[i], newGameBoard)){
					bestValue = childValue;
					bestValueIndex = i;
				}
			}
			if (recursionDepth > 1) {
				delete child;
			}

		}
	}
	else {
		recursionDepth = preserveRecursionDepth;
		return stateValue;
	}
	recursionDepth = preserveRecursionDepth;
	
	if (returnIndex) {
		for (int i = 0; i < parent->children.size(); i++) {
			if (parent->children[i]->value > bestValue) {
				bestValue = parent->children[i]->value;
				bestValueIndex = i;
			}
		}
		cout << bestValue << endl;
		if (bestValue == 0)
			return randIndex;
		return bestValueIndex;
	}
	
	return bestValue + offset;
}

vector<gameMove> ChessAI::pruneBadMoves(vector<gameMove> moves) {
	int i = 0;
	while (i < moves.size()) {
		if (moveBadState(moves[i], gameBoard)) {
			moves.erase(moves.begin() + i);
		}
		else {
			++i;
		}
	}
	return moves;
}


void ChessAI::getPieces(array<array<char, 8>, 8> board) {
	stateValue = 0;
	myPieces.clear();
	opponentPieces.clear();
	// if (this->myKing != NULL) {
	// 	delete this->myKing;
	// }
	// if (this->opponentKing != NULL){
	// 	delete this->opponentKing;
	// }

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char p = board[i][j];
			if (isupper(p)) {
				piece newPiece = piece(i, j, p);
				if (color == WHITE) {
					if (newPiece.pieceType == 'K'){
						this->myKing = newPiece;
					}
					myPieces.push_back(newPiece);
					stateValue += valueGained(p);
				}
				else{
					if (newPiece.pieceType == 'K'){
						this->opponentKing = newPiece;
					}
					opponentPieces.push_back(newPiece);
					stateValue -= valueGained(p);
				}
			}
			else if (islower(p)) {
				piece newPiece = piece(i, j, p);
				if (color == BLACK){
					if (newPiece.pieceType == 'k'){
						this->myKing = newPiece;
					}
					myPieces.push_back(newPiece);
					stateValue += valueGained(p);
				}
				else{
					if (newPiece.pieceType == 'k'){
						this->opponentKing = newPiece;
					}
					stateValue -= valueGained(p);
					opponentPieces.push_back(newPiece);
				}
			}
		}
	}
	// if (recursionDepth < 2) {
	// 	if (inCheck(myKing.x, myKing.y, board, color)) {
	// 	stateValue -= 2;
	// 	}
	// }
}

bool ChessAI::pawnMoved(piece pawn) {
	char pieceType = pawn.pieceType;
	return !(isupper(pieceType) && pawn.x == 6 || islower(pieceType) && pawn.x == 1);
}

bool ChessAI::isBlocked(int x, int y, char pieceType, array<array<char, 8>, 8> board) {
	return (isupper(pieceType) && isupper(board[x][y]) || islower(pieceType) && islower(board[x][y]));
}

bool ChessAI::outOfBounds(int x, int y) {
	return (x > 7 || x < 0 || y > 7 || y < 0);
}

bool ChessAI::inCheck(int x, int y, array<array<char, 8>, 8> board, int kingColor) {
	bool kingInCheck = false;
	array<array<char, 8>, 8> saveGameboardState = this->gameBoard;
	this->gameBoard = board;

	board[x][y] = kingColor == WHITE ? 'K' : 'k';
	vector<gameMove> opponentMoves = kingColor == color ? generateMoves(opponentPieces, board) : generateMoves(myPieces, board) ;
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

bool ChessAI::moveBadState(gameMove m, array<array<char, 8>, 8> board) {
	array<array<char, 8>, 8> tempGameboard = board;
	tempGameboard[m.x1][m.y1] = '\0';
	tempGameboard[m.x2][m.y2] = m.pieceType;
	getPieces(tempGameboard);
	int moveColor = isupper(m.pieceType) ? WHITE : BLACK;
	int kingX = moveColor == color ? myKing.x : opponentKing.x;
	int kingY = moveColor == color ? myKing.y : opponentKing.y;
	if (inCheck(kingX, kingY, tempGameboard, moveColor))
		return true;
	//printGameboard(tempGameboard);
	return false;
}

bool ChessAI::isValidPawnMove(gameMove m, array<array<char, 8>, 8> board) {
	int y = m.y1;
	int xDiff = m.x1 - m.x2;
	if (outOfBounds(m.x2, m.y2))
		return false;

	if (isupper(m.pieceType)) {
		for (int i = m.x2; i < m.x1; i++) {
			if (board[i][y] != '\0')
				return false;
		}
	}
	if (islower(m.pieceType)) {
		for (int i = m.x1 + 1; i <= m.x2; i++) {
			if (board[i][y] != '\0')
				return false;
		}
	}

	if (m.y1 != m.y2){
		if (!islower(board[m.x2][m.y2]) && isupper(m.pieceType))
			return false;
		if (!isupper(board[m.x2][m.y2]) && islower(m.pieceType))
			return false;
	}

	return true;
}

bool ChessAI::isValidKnightMove(gameMove m, array<array<char, 8>, 8> board) {
	if (outOfBounds(m.x2, m.y2)){
		return false;
	}
	return !(isBlocked(m.x2, m.y2, m.pieceType, board));
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

vector<gameMove> ChessAI::generatePawnMoves(piece pawn, array<array<char, 8>, 8> board) {
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
		if (isValidPawnMove(m, board)) {
			moves.push_back(m);
		}
	}

	newX = oldX + offset;
	newY = oldY;
	gameMove m = gameMove(oldX, oldY, newX, newY, pawn.pieceType, 0);
	if (isValidPawnMove(m, board)) {
		moves.push_back(m);
	}

	for (int i = 0; i < 2; i++) {
		newX = oldX + offset;
		newY = i == 1 ? oldY + 1 : oldY - 1;
		double value = valueGained(board[newX][newY]);
		gameMove mCapture = gameMove(oldX, oldY, newX, newY, pawn.pieceType, value);
		if (isValidPawnMove(mCapture, board)) {
			moves.push_back(mCapture);
		}
	}

	return moves;
}

vector<gameMove> ChessAI::generateKnightMoves(piece knight, array<array<char, 8>, 8> board) {
	vector<gameMove> tempMoves;
	vector<gameMove> moves;
	int oldX = knight.x;
	int oldY = knight.y;

	for (int i = 0; i < 4; i++) {
			int offsetX = i % 2 ? 1 : -1;
			int offsetY = i > 1 ? 1 : -1;
			int newX1 = oldX + offsetX * 1;
			int newY1 = oldY + offsetY * 2;
			double value1 = valueGained(board[newX1][newY1]);
			tempMoves.push_back(gameMove(oldX, oldY, newX1, newY1, knight.pieceType, value1));

			int newX2 = oldX + offsetX * 2;
			int newY2 = oldY + offsetY * 1;
			double value2 = valueGained(board[newX2][newY2]);
			tempMoves.push_back(gameMove(oldX, oldY, newX2, newY2, knight.pieceType, value2));
	}

	for (int i = 0; i < 8; i++) {
		gameMove m = tempMoves[i];
		if (isValidKnightMove(m, board))
			moves.push_back(m);
	}

	return moves;
}

vector<gameMove> ChessAI::generateBishopMoves(piece bishop, array<array<char, 8>, 8> board) {
	vector<gameMove> moves;
	int oldX = bishop.x;
	int oldY = bishop.y;
	

	for (int i = 0; i < 4; i++) {
		bool done = false;
		int offsetX = i % 2 ? 1 : -1;
		int offsetY = i > 1 ? 1 : -1;

		int newX = oldX + offsetX;
		int newY = oldY + offsetY;
		while (!isBlocked(newX, newY, bishop.pieceType, board) && !outOfBounds(newX, newY) && !done) {
			if (board[newX][newY] != '\0')
				done = true;
			double value = valueGained(board[newX][newY]);
			moves.push_back(gameMove(oldX, oldY, newX, newY, bishop.pieceType, value));
			newX += offsetX;
			newY += offsetY;
		}
	}
	return moves;
}

vector<gameMove> ChessAI::generateRookMoves(piece rook, array<array<char, 8>, 8> board) {
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

		while (!isBlocked(newX, newY, rook.pieceType, board) && !outOfBounds(newX, newY) && !done) {
			if (board[newX][newY] != '\0')
				done = true;
			double value = valueGained(board[newX][newY]);
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

vector<gameMove> ChessAI::generateQueenMoves(piece queen, array<array<char, 8>, 8> board) {
	vector<gameMove> bishopMoves = generateBishopMoves(queen, board);
	vector<gameMove> rookMoves = generateRookMoves(queen, board);
	bishopMoves.insert( bishopMoves.end(), rookMoves.begin(), rookMoves.end());

	return bishopMoves;
}

vector<gameMove> ChessAI::generateKingMoves(piece king, array<array<char, 8>, 8> board) {
	vector<gameMove> moves;
	int oldX = king.x;
	int oldY = king.y;
	int kingColor = isupper(king.pieceType) ? WHITE : BLACK;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;
			int newX = oldX + i;
			int newY = oldY + j;
			piece oppositeKing = kingColor == color ? opponentKing : myKing;
			if (!isBlocked(newX, newY, king.pieceType, board) && !outOfBounds(newX, newY) && !kingsKissing(newX, newY, oppositeKing)) {
				double value = valueGained(gameBoard[newX][newY]);
				moves.push_back(gameMove(oldX, oldY, newX, newY, king.pieceType, value));
			}
		}
	}

	return moves;
}



array<array<char, 8>, 8> ChessAI::makeMove(gameMove m, array<array<char, 8>, 8> board) {
	array<array<char, 8>, 8> newGameBoard = board;
	newGameBoard[m.x1][m.y1] = '\0';
	newGameBoard[m.x2][m.y2] = m.pieceType;
	return newGameBoard;
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
	gameMove gm = cai.chooseMove();
	while (true) {
			cai.chooseMove();
	}
}