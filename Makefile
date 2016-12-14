all:
	g++ -std=c++11 -o ChessAI chessAI.cpp EngineMediator.cpp Utils.cpp -L/usr/local/lib -lcurl -lcurlpp -I/usr/local/include

clean: 
	rm ChessAI