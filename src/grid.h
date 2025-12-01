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

	public:

		Grid(int w, int h, int d) : width(w), height(h), depth(d), grid(w * h * d, std::nullopt) {}

		bool inBounds(int x, int y, int z) const
		{
			return  (x >= 0 && x < this->width) && 
					(y >= 0 && y < this->height) && 
					(z >= 0 && z < this->depth);
		}

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

		Tile getTile(int x, int y, int z)
		{
			if(!inBounds(x, y, z)) return Tile(Color{255, 255, 255, 0});
			if(grid[idx(x, y, z)] == std::nullopt) return Tile(Color{255, 255, 255, 0});

			return grid[idx(x, y, z)].value();
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

		bool isEmpty(int x, int y, int z) const
		{
			if(!inBounds(x,y,z)) return true;

			return grid[idx(x, y, z)] == std::nullopt;
		}
	
};

// GridHit for block placing and breaking
struct GridHit
{
	Vector3 currHit;
	Vector3 prevHit;
};