#include <curl/curl.h>
#include <iostream>
#include <string>

#include "EngineMediator.h"
#include "json.hpp"

// for convenience
using json = nlohmann::json;
using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
  ((string*)stream)->append((char*)ptr, 0, size*count);
  return size*count;
}

string makeHTTPRequest(const char* requestUrl, string type, const char* fields){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	string response;

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, requestUrl);
		if (type == "POST") {
			curl_easy_setopt(curl, CURLOPT_POST, 1);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
		}

		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return response;
}

EngineMediator::EngineMediator(string baseUrl): baseUrl(baseUrl) {}

void EngineMediator::createGame(){
	string url = baseUrl + "games/new";
	string response = "";
	response = makeHTTPRequest(url.c_str(), "POST", "");

	json game = json::parse(response);
	int id = game["id"];
	this->gameId = id;
	this->token = game["token"];
	this->turn = game["turn"];
	this->gameboard = parseGameBoard(response);
	this->getStateUrl = baseUrl + "games/" + to_string(id) + "/state";
	this->postMoveUrl = baseUrl + "games/" + to_string(id) + "/move";
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
	this->gameboard = gameboard;
	return gameboard;

}

void EngineMediator::sendMove(string move) {
	json postData;
	postData["token"] = this->token;
	postData["to"] = {5, 1};
	postData["from"] = {6, 1};
	string sPostData = postData.dump();
	string response = makeHTTPRequest(this->postMoveUrl.c_str(), "POST", sPostData.c_str());
}

void printGameboard(array<array<char, 8>, 8> gameBoard) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << gameBoard[i][j] << " ";
		}
		cout << std::endl;
	}
}

int main(int, char **)
{
	EngineMediator egm = EngineMediator("http://localhost:3000/");
	egm.createGame();
	array<array<char, 8>, 8> gameboard = egm.getGameBoard();
	printGameboard(gameboard);
	egm.sendMove("f");
}