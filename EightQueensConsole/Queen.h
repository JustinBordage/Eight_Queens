#pragma once
#include "Vector2Int.h"

class Queen
{
public:
	//int ID;
	Vector2Int position;

	Queen(int _x, int _y) : position(Vector2Int(_x, _y)) {};
};