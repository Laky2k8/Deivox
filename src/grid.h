#pragma once

#include "raylib.h"
#include <vector>
#include <optional>
#include "tile.h"

class Grid
{
	private:
		int width;
		int height;
		int depth;
		
		// The grid itself (3D array of ints)
		std::vector<std::optional<Tile>> grid;

		inline int idx(int x, int y, int z) const
		{
       		return y * width + x + z * width * height;
		}

		bool inBounds(int x, int y, int z) const
		{
			return x>=0 && x<width && y>=0 && y<height && z>=0 && z<depth;
		}

	public:

		Grid(int w, int h, int d) : width(w), height(h), depth(d), grid(w * h * d, std::nullopt) {}

		int getWidth()
		{
			return this->width;
		}
		int getHeight()
		{
			return this->height;
		}
		int getDepth()
		{
			return this->depth;
		}

		void clear()
		{
			std::fill(grid.begin(), grid.end(), std::nullopt);
		}

		void setTile(int x, int y, int z, Tile &tile)
		{
			if(!inBounds(x, y, z)) return;

			grid[idx(x, y, z)] = tile;
		}

		void setEmpty(int x, int y, int z)
		{
			if(!inBounds(x, y, z)) return;

			grid[idx(x, y, z)] = std::nullopt;
		}

		void swap(Tile &a, Tile &b)
		{
			Tile temp = a;
			a = b;
			b = temp;
		}

		bool isEmpty(int x, int y, int z)
		{
			if(!inBounds(x,y,z)) return true;

			return grid[idx(x, y, z)] == std::nullopt;
		}
	
};