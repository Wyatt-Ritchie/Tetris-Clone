#include "TileMapComponent.h"
#include <fstream>
#include <sstream>
#include "Actor.h"
#include <iostream>
TileMapComponent::TileMapComponent(Actor* actor, int DrawOrder) : SpriteComponent(actor, DrawOrder)
																 ,mTileSet(nullptr)
{
	std::vector<std::vector<std::vector<int>>> mTileMaps;

	// Order of the tilemaps is important!
	// The layers are rendered back to front.
}


void TileMapComponent::LoadData(std::string fileName)
{
	// start by loading the csv file
	std::ifstream file;
	file.open(fileName);
	std::string tline, tval;
	std::string row;
	// read the csv file
	// and store int** in mTileMaps

	std::vector<std::vector<int>> arr;
	while (std::getline(file, tline))
	{
		std::vector<int> v;
		std::stringstream s(tline);
		while (std::getline(s, tval, ','))
		{
			v.push_back(std::stoi(tval));
		}
		arr.push_back(v);
	}

	mTileMaps.push_back(arr);
	//close the file
	file.close();
}

void TileMapComponent::LoadTileSet(const char* fileName)
{
	mTileSet = this->mOwner->GetGame()->GetTexture(0);
	if (mTileSet != nullptr)
	{
		std::cout << "Tileset loaded" << std::endl;
	}
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	int row, col, n;
	
	SDL_Rect rDest;
	SDL_Rect rSrc;
	// Scale the width and height of the tile on the screen
	rDest.w = 32;
	rDest.h = 32;
	// The width and height of the tile on the tile set
	rSrc.w = 32;
	rSrc.h = 32;

	// This can probably be optimized out of using a tripple loop. Fine for now
	if (mTileSet)
	{
		for (int i=0; i<mTileMaps.size(); i++)
		{
			for (int j=0; j<mTileMaps[i].size(); j++)
			{
				for (int k=0; k<mTileMaps[i][j].size(); k++)
				{
					n = mTileMaps[i][j][k];

					// calculating the position on the tileset from
					// the int provided in the map. This 
					// particular tileset is 8 tiles wide
					col = n / 8;
					row = n % 8;
					if (mTileMaps[i][j][k] == -1)
					{
						// Draw
						rSrc.x = 0;
						rSrc.y = 0;
						SDL_RenderCopyEx(renderer,
							mTileSet,
							&rSrc,
							&rDest,
							-Math::ToDegrees(0.0f),
							nullptr,
							SDL_FLIP_NONE);
					}
					else
					{
						// Center position of the tile on the screen (32px, based on scale)
						rDest.x = k*32;
						rDest.y = j*32;

						// The tile to be drawn (each tile is 32x32 pixel)
						rSrc.x = row * 32;
						rSrc.y = col * 32;
					}
					// Draw
					SDL_RenderCopyEx(renderer,
						mTileSet,
						&rSrc,
						&rDest,
						-Math::ToDegrees(0.0f),
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}

	}
}
