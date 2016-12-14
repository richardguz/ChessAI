#ifndef UTILS_H
#define UTILS_H

#include <array>
#include <string>
#include <iostream> 

// enum pieces {
// 	PAWN = "p",
// 	BISHOP = "b",
// 	KNIGHT = "n",
// 	ROOK = "r",
// 	KING = "k",
// 	QUEEN = "Q"
// };

struct treeNode{
	treeNode* parent;
	int value;
	int gameBoard[8][8];
};

struct Coord{
	Coord(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};

void printGameboard(std::array<std::array<int, 8>, 8> gameBoard);
size_t write_to_string(void *ptr, size_t size, size_t count, void *stream);
std::string makeHTTPRequest(const char* requestUrl, std::string type, const char* fields);

#endif //UTILS_H