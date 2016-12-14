#ifndef ENGINE_MEDIATOR_H
#define ENGINE_MEDIATOR_H

#include <string>
#include <array>

class EngineMediator {

public:
	EngineMediator(std::baseUrl);

	std::array<std::array<char, 8>, 8> getGameBoard();
	void joingame(int gameId);
	void createGame()

	void sendMove(std::string move);

private:
	std::string url;
	std::string gameId;
	std::string getStateUrl;
	std::string postUrl;
	std::string color;
	std::string token;
	std::array<std::array<char, 8>, 8> gameboard;
};

#endif
//ENGINE_MEDIATOR_H