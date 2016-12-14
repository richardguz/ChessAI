#ifndef CHESS_AI_H
#define CHESS_AI_H

#include <string>
#include <array>

#include "EngineMediator.h"

class ChessAI {
public:
	ChessAI(std::string endpointUrl);
	void printGameboard(std::array<std::array<int, 8>, 8> gameBoard);

private: 
	EngineMediator* egm;
	
};

#endif //CHESS_AI_H