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
	// std::vector<move> generateKingMoves(piece king);
	bool isValidPawnMove(gameMove m);
	bool isValidBishopMove(gameMove m);
	bool isValidKnightMove(gameMove m);


	double valueGained(char p);
	bool pawnMoved(piece pawn);
	bool isBlocked(int x, int y);
	bool outOfBounds(int x, int y);
	void checkPieces();
	void makeMove(gameMove m);

private: 
	EngineMediator* egm;
	std::array<std::array<char, 8>, 8> gameBoard;
	std::string color;
	std::vector<piece> myPieces;
};

#endif //CHESS_AI_H