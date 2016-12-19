#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <string>
#include <iostream> 


#define BLACK 0
#define WHITE 1
// enum pieces {
// 	PAWN = "p",
// 	BISHOP = "b",
// 	KNIGHT = "n",
// 	ROOK = "r",
// 	KING = "k",
// 	QUEEN = "Q"
// };

struct Coord{
	Coord(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};

struct piece{
	piece(int x, int y, char pieceType) : x(x), y(y), pieceType(pieceType) {}
	int x;
	int y;
	char pieceType;
	int color;
};

struct gameMove{
	//gameMove();
	gameMove(int x1, int y1, int x2, int y2, char pieceType, double value)
	: x1(x1), y1(y1), x2(x2), y2(y2), pieceType(pieceType), value(value) {}
	int x1;
	int y1;
	int x2;
	int y2;
	char pieceType;
	double value;
};

struct treeNode{
	treeNode* parent;
	int value;
	gameMove gm;
	std::array<std::array<char, 8>, 8> gameBoard;
};

void printGameboard(std::array<std::array<char, 8>, 8> gameBoard);
size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);
std::string makeHTTPRequest(const char* requestUrl, std::string type, const char* fields);

#endif //UTILS_H