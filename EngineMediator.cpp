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

EngineMediator::EngineMediator(string getUrl, string postUrl): getUrl(getUrl), postUrl(postUrl) {}

array<array<char, 8>, 8> EngineMediator::getGameBoard() {
	array<array<char, 8>, 8> gameboard;
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/games/1/state");

		string response;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		json board = json::parse(response)["board"];
		for (int i = 0; i < 8; i++){
			array<char, 8> row;
			for (int j = 0; j < 8; j++){
				string s =  board[i][j];
				row[j] = s[0];
			}
			gameboard[i] = row;
		}
		cout << response << endl;
	}
	return gameboard;

}

void EngineMediator::sendMove(string move) {
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3000/games/new");
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{e2e4}");

		string response;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
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
	EngineMediator egm = EngineMediator("http://localhost:3000/games/1/state", "http://localhost:3000/games/1/");
	array<array<char, 8>, 8> gameboard = egm.getGameBoard();
	egm.sendMove("e4e5");
	printGameboard(gameboard);
}