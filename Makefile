CC=g++
CFLAGS= -std=c++11
LIBS= -L/usr/local/lib -lcurl -lcurlpp -I/usr/local/include



all:
	$(CC) $(CFLAGS) -o ChessAI chessAI.cpp EngineMediator.cpp Utils.cpp $(LIBS)

clean: 
	rm ChessAI