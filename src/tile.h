#pragma once
#include "raylib.h"

class Tile
{
	public:
		Color color;

		Tile(Color color)
		{
			this->color = color;
		}
};