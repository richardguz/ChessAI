#ifndef ENGINE_MEDIATOR_H
#define ENGINE_MEDIATOR_H

#include <string>
#include <array>
#include "Utils.h"

class EngineMediator {

public:
	EngineMediator(std::string baseUrl);

	std::array<std::array<char, 8>, 8> getGameBoard();
	std::array<std::array<char, 8>, 8> parseGameBoard(std::string response);

	void joinGame(int gameId);
	void createGame();
	std::string sendMove(gameMove move);

	int getColor();
	bool myTurn();

private:
	std::string baseUrl;
	std::string getStateUrl;
	std::string postMoveUrl;
	std::string token;

	int color;
	int gameId;
	bool turn;
	std::array<std::array<char, 8>, 8> gameboard;
};

#endif
//ENGINE_MEDIATOR_H