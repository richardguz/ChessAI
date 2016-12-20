#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <string>
#include <array>
#include <vector>

#include "Utils.h"
#include "EngineMediator.h"

class ChessAI {
public:
	ChessAI(std::string endpointUrl);
	std::array<std::array<char, 8>, 8> getGameBoard();
	std::vector<gameMove> generatePawnMoves(piece pawn);
	std::vector<gameMove> generateBishopMoves(piece bishop);
	std::vector<gameMove> generateKnightMoves(piece knight);
	std::vector<gameMove> generateRookMoves(piece rook);
	std::vector<gameMove> generateQueenMoves(piece queen);
	std::vector<gameMove> generateKingMoves(piece king);

	std::vector<gameMove> generateMoves(std::vector<piece> pieces);
	std::vector<gameMove> pruneBadMoves(std::vector<gameMove> moves);

	
	bool isValidPawnMove(gameMove m);
	bool isValidBishopMove(gameMove m);
	bool isValidKnightMove(gameMove m);
	bool pawnMoved(piece pawn);
	bool isBlocked(int x, int y, char pieceType);
	bool outOfBounds(int x, int y);
	bool inCheck(int x, int y, std::array<std::array<char, 8>, 8> board, int kingColor);
	bool moveBadState(gameMove m);
	bool kingsKissing(int x, int y, piece oppositeKing);
	treeNode* generateMoveTree(std::array<std::array<char, 8>, 8> board, treeNode* parent);


	double valueGained(char p);
	
	void getPieces(std::array<std::array<char, 8>, 8> board);
	void getOpponentPieces();
	std::array<std::array<char, 8>, 8> makeMove(gameMove m, std::array<std::array<char, 8>, 8> gameBoard);

	gameMove chooseMove();

private: 
	EngineMediator* egm;
	std::array<std::array<char, 8>, 8> gameBoard;
	int color;
	std::vector<piece> myPieces;
	std::vector<piece> opponentPieces;
	piece* myKing;
	piece* opponentKing;
	int recursionDepth;
};

#endif //CHESS_AI_H