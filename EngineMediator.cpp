#include <iostream>
#include <string>

#include "EngineMediator.h"
#include "json.hpp"

// for convenience
using json = nlohmann::json;
using namespace std;

EngineMediator::EngineMediator(string baseUrl): baseUrl(baseUrl) {}

void EngineMediator::createGame() {
	string url = baseUrl + "games/new";
	string response = "";
	response = makeHTTPRequest(url.c_str(), "POST", "");

	if (response.empty()) {
		cout << "The server cannot be reached" << endl;
		return;
	}
	json game = json::parse(response);
	int id = game["id"];
	this->gameId = id;
	this->token = game["token"];
	this->turn = game["turn"];
	this->color = WHITE;
	this->gameboard = parseGameBoard(response);
	this->getStateUrl = baseUrl + "games/" + to_string(id) + "/state";
	this->postMoveUrl = baseUrl + "games/" + to_string(id) + "/move";
	cout << game << endl;
}

void EngineMediator::joinGame(int gameId) {
	string url = baseUrl + "games/" + to_string(gameId) + "/join";
	string response = "";
	response = makeHTTPRequest(url.c_str(), "POST", "");
	json game = json::parse(response);
	this->gameId = gameId;
	this->token = game["token"];
	this->turn = game["turn"];
	this->color = BLACK;
	this->gameboard = parseGameBoard(response);
	this->getStateUrl = baseUrl + "games/" + to_string(gameId) + "/state";
	this->postMoveUrl = baseUrl + "games/" + to_string(gameId) + "/move";
}

array<array<char, 8>, 8> EngineMediator::parseGameBoard(string response) {
	array<array<char, 8>, 8> gameboard;
	json board = json::parse(response)["board"];
	for (int i = 0; i < 8; i++){
		array<char, 8> row;
		for (int j = 0; j < 8; j++){
			string s =  board[i][j];
			row[j] = s[0];
		}
		gameboard[i] = row;
	}
	return gameboard;
}	

array<array<char, 8>, 8> EngineMediator::getGameBoard() {
	array<array<char, 8>, 8> gameboard;
	string response = makeHTTPRequest(getStateUrl.c_str(), "GET", "");
	gameboard = parseGameBoard(response);
	json game = json::parse(response);
	this->gameboard = gameboard;
	this->turn = game["turn"];
	return gameboard;
}

string EngineMediator::sendMove(gameMove move) {
	json postData;
	postData["token"] = this->token;
	postData["to"] = {move.x2, move.y2};
	postData["from"] = {move.x1, move.y1};
	string sPostData = postData.dump();
	//cout << postData << endl;
	string response = makeHTTPRequest(this->postMoveUrl.c_str(), "POST", sPostData.c_str());
	cout << response << endl;
	return response;
}

int EngineMediator::getColor(){
	return color;
}

bool EngineMediator::myTurn() {
	return turn;
}