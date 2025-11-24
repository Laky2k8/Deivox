#include "raylib.h"
#include "raymath.h"

#include <string>
#include <iostream>
#include <cmath>
#include <cfloat>
#include "grid.h"

#define TITLE "Deivox"
#define VERSION_NUM "0.0.1"

using namespace std;

std::string getTitle(float fps = -1)
{
	if (fps >= 0)
	{
		return (string(TITLE) + " " + string(VERSION_NUM) + " - " + to_string(GetFPS()) + " FPS");
	}
	else
	{
		return (string(TITLE) + " " + string(VERSION_NUM));
	}
}

bool RaycastGrid(const Ray &ray, const Grid &grid, GridHit &hit, float maxDistance = 100.0f);

int main() 
{
	const int screenWidth = 1280;
	const int screenHeight = 720;
	InitWindow(screenWidth, screenHeight, getTitle().c_str());
	SetTargetFPS(165);

	Camera camera = { 0 };
	camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	Vector3 cubePosition = { 0.0f, 1.0f, 0.0f };
	Vector3 cubeSize = { 2.0f, 2.0f, 2.0f };

	Ray ray = { 0 };
	GridHit hit;

	Grid voxelGrid = Grid(16, 16, 16);


	Tile groundTile; // default tile (fill it the way your Tile constructor expects)
	for (int x = 0; x < voxelGrid.getWidth(); ++x)
		for (int z = 0; z < voxelGrid.getDepth(); ++z)
			voxelGrid.setTile(x, 0, z, groundTile);


	while (!WindowShouldClose())
	{
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			/*if(!collision.hit)
			{
				ray = GetScreenToWorldRay(GetMousePosition(), camera);

				collision = GetRayCollisionBox(ray,
							(BoundingBox){(Vector3){ cubePosition.x - cubeSize.x/2, cubePosition.y - cubeSize.y/2, cubePosition.z - cubeSize.z/2 },
							(Vector3){ cubePosition.x + cubeSize.x/2, cubePosition.y + cubeSize.y/2, cubePosition.z + cubeSize.z/2 }});
			}
			else
			{
				collision.hit = false;
			}*/

			ray = GetScreenToWorldRay(GetMousePosition(), camera);
			std::cout << "Ray Origin: (" << ray.position.x << ", " << ray.position.y << ", " << ray.position.z << ")\n";

			if(RaycastGrid(ray, voxelGrid, hit, 100.0f))
			{
				std::cout << "Hit at: (" << hit.currHit.x << ", " << hit.currHit.y << ", " << hit.currHit.z << ")\n";

				Tile tile;

				voxelGrid.setTile(hit.prevHit.x, hit.prevHit.y, hit.prevHit.z, tile);
				// hit.currHit is the first OCCUPIED voxel
				// hit.prevHit is the actual empty tile we want to modify
			}

		}


		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);

		/*if (collision.hit)
		{
			DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, RED);
			DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, MAROON);

			DrawCubeWires(cubePosition, cubeSize.x + 0.2f, cubeSize.y + 0.2f, cubeSize.z + 0.2f, GREEN);
		}
		else
		{
			DrawCube(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, GRAY);
			DrawCubeWires(cubePosition, cubeSize.x, cubeSize.y, cubeSize.z, DARKGRAY);
		}*/

		for(int x = 0; x < voxelGrid.getWidth(); x++)
		{
			for(int y = 0; y < voxelGrid.getHeight(); y++)
			{
				for(int z = 0; z < voxelGrid.getDepth(); z++)
				{
					if(!voxelGrid.isEmpty(x, y, z))
					{
						DrawCube((Vector3){x, y, z}, 1.0f, 1.0f, 1.0f, BLUE);
						DrawCubeWires((Vector3){x, y, z}, 1.0f, 1.0f, 1.0f, DARKBLUE);
					}
				}
			}
		}



		DrawRay(ray, MAROON);
		DrawGrid(10, 1.0f);


		EndMode3D();


		DrawText(getTitle(GetFPS()).c_str(), 20, 20, 20, BLACK);

		EndDrawing();


	}


	CloseWindow();
	return 0;
}


bool RaycastGrid(const Ray &ray, const Grid &grid, GridHit &hit, float maxDistance)
{
	Vector3 dir = Vector3Normalize(ray.direction);

	// starting voxel
	Vector3 start = ray.position;

	int x = (int)floorf(start.x);
	int y = (int)floorf(start.y);
	int z = (int)floorf(start.z);

	// if starting outside grid: step until we enter or go further than the max dist
	int stepX = (dir.x > 0) ? 1 : -1;
	int stepY = (dir.y > 0) ? 1 : -1;
	int stepZ = (dir.z > 0) ? 1 : -1;

	float tMaxX, tMaxY, tMaxZ;
	float tDeltaX, tDeltaY, tDeltaZ;

	if(dir.x == 0.0f)
	{
		tMaxX = FLT_MAX;
		tDeltaX = FLT_MAX;
	}
	else
	{
		float vx = (float)floorf(start.x);
		float distToBoundaryX = (dir.x > 0) ? (vx + 1.0f - start.x) : (start.x - vx);
		tMaxX = distToBoundaryX / fabsf(dir.x);
		tDeltaX = 1.0f / fabsf(dir.x);
	}

	if(dir.y == 0.0f)
	{
		tMaxY = FLT_MAX;
		tDeltaY = FLT_MAX;
	}
	else
	{
		float vy = (float)floorf(start.y);
		float distToBoundaryY = (dir.y > 0) ? (vy + 1.0f - start.y) : (start.y - vy);
		tMaxY = distToBoundaryY / fabsf(dir.y);
		tDeltaY = 1.0f / fabsf(dir.y);
	}
	
	if(dir.z == 0.0f)
	{
		tMaxZ = FLT_MAX;
		tDeltaZ = FLT_MAX;
	}
	else
	{
		float vz = (float)floorf(start.z);
		float distToBoundaryZ = (dir.z > 0) ? (vz + 1.0f - start.z) : (start.z - vz);
		tMaxZ = distToBoundaryZ / fabsf(dir.z);
		tDeltaZ = 1.0f / fabsf(dir.z);
	}

	int prevX = x, prevY = y, prevZ = z;
	float t = 0.0f;

	while(t <= maxDistance)
	{
		if(grid.inBounds(x, y, z))
		{
			if(!grid.isEmpty(x, y, z))
			{
				hit.currHit.x = x;
				hit.currHit.y = y;
				hit.currHit.z = z;

				hit.prevHit.x = prevX;
				hit.prevHit.y = prevY;
				hit.prevHit.z = prevZ;

				return true;
			}
		}

		prevX = x; prevY = y; prevZ = z;

		if(tMaxX < tMaxY)
		{
			if(tMaxX < tMaxZ)
			{
				x += stepX;
				t = tMaxX;
				tMaxX += tDeltaX;
			}
			else
			{
				z += stepZ;
				t = tMaxZ;
				tMaxZ += tDeltaZ;
			}
		}
		else
		{
			if(tMaxY < tMaxZ)
			{
				y += stepY;
				t = tMaxY;
				tMaxY += tDeltaY;
			}
			else
			{
				z += stepZ;
				t = tMaxZ;
				tMaxZ += tDeltaZ;
			}
		}
	}

	return false;

}

