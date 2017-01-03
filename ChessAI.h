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
	std::vector<gameMove> generatePawnMoves(piece pawn, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> generateBishopMoves(piece bishop, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> generateKnightMoves(piece knight, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> generateRookMoves(piece rook, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> generateQueenMoves(piece queen, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> generateKingMoves(piece king, std::array<std::array<char, 8>, 8> board);

	std::vector<gameMove> generateMoves(std::vector<piece> pieces, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> generateEarlyMoves(std::vector<piece> pieces, std::array<std::array<char, 8>, 8> board);
	std::vector<gameMove> pruneBadMoves(std::vector<gameMove> moves);

	
	bool isValidPawnMove(gameMove m, std::array<std::array<char, 8>, 8> board);
	bool isValidBishopMove(gameMove m, std::array<std::array<char, 8>, 8> board);
	bool isValidKnightMove(gameMove m, std::array<std::array<char, 8>, 8> board);
	bool pawnMoved(piece pawn);
	bool isBlocked(int x, int y, char pieceType, std::array<std::array<char, 8>, 8> board);
	bool outOfBounds(int x, int y);
	bool inCheck(int x, int y, std::array<std::array<char, 8>, 8> board, int kingColor);
	bool moveBadState(gameMove m, std::array<std::array<char, 8>, 8> board);
	bool kingsKissing(int x, int y, piece oppositeKing);
	double generateMoveTree(std::array<std::array<char, 8>, 8> board, treeNode* parent);



	double valueGained(char p);
	
	void pollForTurn();
	void getPieces(std::array<std::array<char, 8>, 8> board);
	void getOpponentPieces();
	std::array<std::array<char, 8>, 8> makeMove(gameMove m, std::array<std::array<char, 8>, 8> board);

	gameMove chooseMove();


	gameMove MinMax(gameMove move, std::array<std::array<char, 8>, 8> board, short int depth_limit);
	gameMove MaxMove(gameMove move, std::array<std::array<char, 8>, 8> board, short int depth_limit, short int depth, int alpha, int beta);
	gameMove MinMove(gameMove move, std::array<std::array<char, 8>, 8> board, short int depth_limit, short int depth, int alpha, int beta);

private: 
	EngineMediator* egm;
	std::array<std::array<char, 8>, 8> gameBoard;
	int color;
	std::vector<piece> myPieces;
	std::vector<piece> opponentPieces;
	piece myKing;
	piece opponentKing;
	int recursionDepth;
	int stateValue;
	int turnNumber;
};

#endif //CHESS_AI_H