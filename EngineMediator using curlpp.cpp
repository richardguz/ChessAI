#include <cstdlib>
#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

#include <iostream>
#include <sstream>

#include "EngineMediator.h"
#include "json.hpp"

// for convenience
using json = nlohmann::json;
using namespace curlpp::options;
using std::string;
using std::cout;
using std::array;

EngineMediator::EngineMediator(string getUrl, string postUrl): getUrl(getUrl), postUrl(postUrl) {}

array<array<char, 8>, 8> EngineMediator::getGameBoard(){
	std::stringstream result;
	array<array<char, 8>, 8> gameboard;
	try
	{
		// That's all that is needed to do cleanup of used resources (RAII style).
		curlpp::Cleanup myCleanup;

		// Our request to be sent.
		curlpp::Easy myRequest;

		// Set the URL.
		myRequest.setOpt<Url>(getUrl);
		myRequest.setOpt(cURLpp::options::WriteStream(&result));
		myRequest.perform();
		
		json board = json::parse(result.str())["board"];

		for (int i = 0; i < 8; i++){
			array<char, 8> row;
			for (int j = 0; j < 8; j++){
				string s =  board[i][j];
				row[j] = s[0];
			}
			gameboard[i] = row;
		}
	}

	catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
	}
    
  return gameboard;
}

void EngineMediator::sendMove(string move){
	std::stringstream out; 
    std::stringstream ss; 
    ss << postUrl; 
	try {
		curlpp::Cleanup cleaner;
		curlpp::Easy request;
    
		request.setOpt(new curlpp::options::Url(postUrl)); 
		request.setOpt(new curlpp::options::Verbose(true)); 
    
		std::list<std::string> header; 
		header.push_back("Content-Type: application/octet-stream"); 
    
		request.setOpt(new curlpp::options::HttpHeader(header)); 
    
		request.setOpt(new curlpp::options::PostFields("abcd"));
		request.setOpt(new curlpp::options::PostFieldSize(5));
    
		request.perform(); 
	}
	catch ( curlpp::LogicError & e ) {
    	std::cout << e.what() << std::endl;
	}
	catch ( curlpp::RuntimeError & e ) {
		std::cout << e.what() << std::endl;
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