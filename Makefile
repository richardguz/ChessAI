all: EngineMediator.cpp EngineMediator.h
	g++ -std=c++11 -o EngineTest EngineMediator.cpp -L/usr/local/lib -lcurl -lcurlpp -I/usr/local/include
