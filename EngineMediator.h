#ifndef ENGINE_MEDIATOR_H
#define ENGINE_MEDIATOR_H

#include <string>
#include <array>

class EngineMediator {

public:
	EngineMediator(std::string baseUrl);

	std::array<std::array<char, 8>, 8> getGameBoard();
	std::array<std::array<char, 8>, 8> parseGameBoard(std::string response);
	void joingame(int gameId);
	void createGame();

	void sendMove(std::string move);

private:
	std::string baseUrl;
	std::string getStateUrl;
	std::string postMoveUrl;
	std::string color;
	std::string token;

	int gameId;
	bool turn;
	std::array<std::array<char, 8>, 8> gameboard;
};

#endif
//ENGINE_MEDIATOR_H