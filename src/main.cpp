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

bool RaycastGrid(const Ray &ray, const Grid )

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

	Grid voxelGrid = Grid(16, 16, 16);
	Vector3 hitPoint = { 0 };
	bool anyHit = false;
	float closestHit = FLT_MAX;
	int hitX = -1, hitY = -1, hitZ = -1;


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

			closestHit = FLT_MAX;
			hitX = -1, hitY = -1, hitZ = -1;
			hitPoint = { 0 };
			anyHit = false;

			for(int x = 0; x < voxelGrid.getWidth(); x++)
			{
				for(int y = 0; y < voxelGrid.getHeight(); y++)
				{
					for(int z = 0; z < voxelGrid.getDepth(); z++)
					{

						if (voxelGrid.isEmpty(x,y,z) && y > 1) continue;

						BoundingBox box;
						box.min = (Vector3){ x - 0.5f, y - 0.5f, z - 0.5f };
						box.max = (Vector3){ x + 0.5f, y + 0.5f, z + 0.5f };

						RayCollision rc = GetRayCollisionBox(ray, box);

						if(rc.hit && rc.distance < closestHit)
						{
							closestHit = rc.distance;
							anyHit = true;
							hitX = x; hitY = y; hitZ = z;

							hitPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, rc.distance));

							int gx = (int)roundf(hitPoint.x);
							int gy = (int)roundf(hitPoint.y);
							int gz = (int)roundf(hitPoint.z);


							if(voxelGrid.isEmpty(gx, gy, gz))
							{
								Tile tile = Tile();
								voxelGrid.setTile(gx, gy, gz, tile);
							}
						}
					}
				}
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
		
		if(anyHit)
		{
			DrawText(("Ray 3D pos:\n x:" + to_string(hitPoint.x) + " y:" + to_string(hitPoint.y) + " z:" + to_string(hitPoint.z)).c_str(), 20, 50, 20, DARKGRAY);
		}

		EndDrawing();


	}


	CloseWindow();
	return 0;
}

