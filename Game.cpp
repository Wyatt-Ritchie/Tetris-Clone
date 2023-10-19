#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "SDL_image.h"
#include "Math.h"
#include "BGSpriteComponent.h"
#include "AudioSystem.h"
#include "InputComponent.h"
#include "TPiece.h"
#include "LPiece.h"
#include "FlatPiece.h"
#include "ReverseLPiece.h"
#include "SquarePiece.h"
#include "ZPiece.h"
#include "ReverseZPiece.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <set>
Game::Game() : mWindow(nullptr),
			   mRenderer(nullptr), 
			   mIsRunning(true),
			   mUpdatingActors(false),
			   mAudioSystem(nullptr),
			   ActivePiece(nullptr),
			   mSpawnTimer(0.1),
			   mDropTimer(0.8)
{
	mGrid = new char*[20];
	for (int i = 0; i < 20; i++)
	{
		mGrid[i] = new char[10];
	}

	for (int i = 0; i < 20; i++)
	{
		
		for (int j = 0; j < 10; j++)
		{
			mGrid[i][j] = '.';
		}
	}
	PrintGrid();
}

bool Game::Initialize()
{
	// int is 0 for success, non zero for fail
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		// reutrn false and a c-style string error message to the console
		SDL_Log("Unable to initialize the SDL: %s", SDL_GetError());
		return false;
	}

	// create a window
	mWindow = SDL_CreateWindow(
		"Tetris Clone", // Window Title
		100, // Top left x-coordinate of window
		100, // top left y-coordinate of window
		1024, // width of window
		768, // height of window
		0 // flags (0 for no flags set)
	);

	// check that window was created. Otherwise send error message
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// initialize the SDL renderer for mWindow
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create the renderer for
		-1, // usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// return false if rendered fails to initialize
	if (!mRenderer)
	{
		SDL_Log("Failed to initialize renderer: %s", SDL_GetError());
		return false;
	}

	int flags = IMG_INIT_PNG;
	int imgResult = IMG_Init(flags);

	// Initialize audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("No audio system to initialize.");
		mAudioSystem->ShutDown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}
	

	// Call the Load data function
	LoadData();

	mTicksCount = SDL_GetTicks();
	// set mIsRunning to true

	return true;
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		std::cout << "Escape pressed, begin shutdown" << std::endl;
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;
}

// implementation of the game loop function. 
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();

		UpdateGame();

		GenerateOutput();
	}

}

void Game::UpdateGame() 
{
	// ensure there is a 16ms elapsed time between frames
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// DeltaTime is thge difference in ticks from last
	// converted to seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// update ticks count

	// clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update the audio system
	mAudioSystem->Update(deltaTime);


	// update all actors

	mUpdatingActors = true;

	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}

	mUpdatingActors = false;

	// move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->getState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	// delete dead actors (remove them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	if (mSpawnTimer > 0.0f && ActivePiece == nullptr)
	{
		mSpawnTimer -= deltaTime;
	}
	else {
		mSpawnTimer = 0.1;
		if (ActivePiece == nullptr)
		{
			GenerateNextPiece();
		}
	}

	if (mDropTimer > 0 && ActivePiece != nullptr)
	{
		mDropTimer -= deltaTime;
	}
	else if(ActivePiece != nullptr)
	{
		for (auto& e : mActors)
		{
			if (e->getState() == Actor::EActive && !e->GetIsPiece())
			{
				Vector2 temp = e->GetPosition();
				temp += Vector2(0.0f, 32.0f);
				e->SetPosition(temp);
			}
		}
		mDropTimer = 0.8;
	}
}

void Game::GenerateOutput() 
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		0,
		255
	);

	SDL_RenderClear(mRenderer);

	// Creating background Box
	SDL_Rect box;
	box.x = 336;
	box.y = 0;
	box.w = 656 - 334;
	box.h = 642;
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 100, 255);
	SDL_RenderFillRect(mRenderer, &box);

	// horizontal
	for (int i = 0; i <= 20*32; i += 32)
	{
		SDL_SetRenderDrawColor(mRenderer, 80, 80, 80, 255);
		
		SDL_RenderDrawLine(mRenderer
							, 336
							, i
							, 656
							, i);
	}
	
	// Vertical
	for (int i = 336; i <= 656; i += 32)
	{
		SDL_SetRenderDrawColor(mRenderer, 80, 80, 80, 255);

		SDL_RenderDrawLine(mRenderer
			, i
			, 0
			, i
			, 640);
	}

	// creating background border box
	SDL_Rect bbox;
	bbox.x = 335;
	bbox.y = 0;
	bbox.w = 658 - 334;
	bbox.h = 642;
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(mRenderer, &bbox);

	// Add stuff to be rendered
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor)
{
	// if updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// find the insertion point in the sorted vector

	// (the first element with a higher draw order than me)

	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;iter != mSprites.end();++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	//insert element before position of the iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Game::PrintGrid()
{
	for (int i = 0; i < 20; i++)
	{
		std::cout << "| ";
		for (int j = 0; j < 10; j++)
		{
			std::cout << mGrid[i][j] << " ";
		}
		std::cout << "|" << std::endl;
	}
}

void Game::UpdateGrid(std::vector<Block*> blocks)
{
	for (auto& e : blocks)
	{
		int row, col;
		Vector2 pos = e->GetPosition();
		//mBlocks.push_back(e);
		row = (int)pos.y / 32;
		col = (int)(pos.x - 334) / 32;
		mGrid[row][col] = 'X';
	}
	ActivePiece = nullptr;
	PrintGrid();
}

void Game::GenerateNextPiece()
{
	CheckRows();
	ResetGrid();
	if (GetActivePiece() != nullptr) return;
	if (mGrid[0][5] != '.' || mGrid[0][4] != '.') return;
	int n;
	
	n = (rand() % 7) + 1;
	
	switch (n)
	{
	case 1:
		GenerateLPiece();
		break;
	case 2:
		GenerateRLPiece();
		break;
	case 3:
		GenerateFlatPiece();
		break;
	case 4:
		GenerateZPiece();
		break;
	case 5:
		GenerateRZPiece();
		break;
	case 6:
		GenerateTPiece();
		break;
	case 7:
		GenerateSquarePiece();
		break;
	default:
		std::cout << "Somhow default was used." << std::endl;
		break;
	}
	
}

void Game::GenerateLPiece()
{
	LPiece* l = new LPiece(this);
	SetActivePiece(l);
}

void Game::GenerateRLPiece()
{
	ReverseLPiece* rl = new ReverseLPiece(this);
	SetActivePiece(rl);
	
}

void Game::GenerateFlatPiece()
{
	FlatPiece* f = new FlatPiece(this);
	SetActivePiece(f);
	
}

void Game::GenerateZPiece()
{
	ZPiece* z = new ZPiece(this);
	SetActivePiece(z);
	
}

void Game::GenerateRZPiece()
{
	ReverseZPiece* rz = new ReverseZPiece(this);
	SetActivePiece(rz);
	
}

void Game::GenerateTPiece()
{
	TPiece* t = new TPiece(this);
	SetActivePiece(t);
}

void Game::GenerateSquarePiece()
{
	SquarePiece* s = new SquarePiece(this);
	SetActivePiece(s);
}

void Game::ResetGrid()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			mGrid[i][j] = '.';
		}
	}
	if (mActors.empty()) return;
	for (auto& e : mActors)
	{
		if (e->getState() == Actor::EPlaced && !e->GetIsPiece())
		{
			int row, col;
			Vector2 pos = e->GetPosition();
			row = (int)pos.y / 32;
			if (row > 19) row = 19;
			col = (int)(pos.x - 334) / 32;
			std::cout << "Row: " << row << " Col: " << col << std::endl;
			mGrid[row][col] = 'X';
		}
	}
	PrintGrid();
}

void Game::CheckRows()
{
	if (mActors.empty()) return;
	std::set<int> ColumnsToDrop;
	int colDeleted = 0;
	int fullRows = 0;
	for (int i = 0; i < 20; i++)
	{
		bool full = true;
		for (int j = 0; j < 10; j++)
		{
			if (mGrid[i][j] == '.')
			{
				full = false;
				break;
			}
		}
		
		if (full)
		{
			fullRows++;
			colDeleted = i;
			for (auto& b : mActors)
			{
				if ((int)(b->GetPosition().y / 32) == i)
				{
					
					Vector2 temp = b->GetPosition();

					if (mGrid[i][(int)(temp.x - 334) / 32] == 'X')
					{
						ColumnsToDrop.insert(((int)(temp.x - 334) / 32));
					}
					b->SetState(Actor::EDead);
				}
				
			}
			for (int j = 0; j < 10; j++)
			{
				mGrid[i][j] = '.';
				
			}
		}

	}
	if (!ColumnsToDrop.empty())
	{
		for (auto& b : mActors)
		{
			std::set<int>::iterator it;
			if (!b->GetIsPiece())
			{
				Vector2 temp = b->GetPosition();
				if (temp.y / 32 - 1 < colDeleted)
				{
					it = ColumnsToDrop.find((int)(temp.x - 334) / 32);
					if (it != ColumnsToDrop.end())
					{
						b->SetPosition(Vector2(temp.x, temp.y + (fullRows * 32)));
					}
				}
			}
			
		}
	}
}

SDL_Texture* Game::GetTexture(int c)
{
	
	if (c > 0 && c <=6)
	{
		return mTextures[c-1];
	}
	return mTextures[0];
	
	//auto iter = mTextures.find(fileName);

	//if (iter != mTextures.end())
	//{
	//	tex = iter->second;
	//}
	//else
	//{
	//	// Load from file
	//	SDL_Surface* surf = IMG_Load(fileName);
	//	if (!surf)
	//	{
	//		SDL_Log("Failed to load texture file %s", fileName);
	//		return nullptr;
	//	}

	//	

	//	// Create texture from surface
	//	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	//	SDL_FreeSurface(surf);
	//	if (!tex)
	//	{
	//		SDL_Log("Failed to convert surface to texture for %s", fileName);
	//		return nullptr;
	//	}

	//	mTextures.emplace(fileName, tex);
	//}
	
}

void Game::LoadData()
{
	// Set temp background 
	/*
	* Steps:
	* 1. create a temp Actor instance, and set position to center of screen
	* 2. Create a new BGSpriteComponent and set temp as the input
	* 3. Set Screen Size to desired size
	* 4. Get textures, then set textures.
	* 5. Optional if there are more than one background repeat and set scroll speeds if desired.
	*/
	
	// creating an actor 
	/*Actor* actor = new Actor(this);
	actor->SetPosition(Vector2(350.0f, 350.0f));
	actor->SetScale(0.5f);
	SpriteComponent* block = new SpriteComponent(actor, 10);
	block->setTexture(GetTexture("Assets/red_brick.png"));
	InputComponent* ic = new InputComponent(actor);
	ic->SetForwardKey(SDL_SCANCODE_S);*/
	
	SDL_Texture* tex = nullptr;

	SDL_Surface* surf = IMG_Load("Assets/blue_brick.png");
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", "Assets/blue_brick.png");
	}

	// Create texture from surface
	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Failed to convert surface to texture for %s", "Assets/blue_brick.png");
	}

	mTextures.push_back(tex);

	surf = IMG_Load("Assets/red_brick.png");
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", "Assets/red_brick.png");
	}

	// Create texture from surface
	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Failed to convert surface to texture for %s", "Assets/red_brick.png");
	}

	mTextures.push_back(tex);

	surf = IMG_Load("Assets/yellow_brick.png");
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", "Assets/yellow_brick.png");
	}

	// Create texture from surface
	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Failed to convert surface to texture for %s", "Assets/yellow_brick.png");
	}

	mTextures.push_back(tex);

	surf = IMG_Load("Assets/green_brick.png");
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", "Assets/green_brick.png");
	}

	// Create texture from surface
	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Failed to convert surface to texture for %s", "Assets/green_brick.png");
	}

	mTextures.push_back(tex);

	surf = IMG_Load("Assets/purple_brick.png");
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", "Assets/purple_brick.png");
	}

	// Create texture from surface
	tex = SDL_CreateTextureFromSurface(mRenderer, surf);
	SDL_FreeSurface(surf);
	if (!tex)
	{
		SDL_Log("Failed to convert surface to texture for %s", "Assets/purple_brick.png");
	}

	mTextures.push_back(tex);

	GenerateNextPiece();
	
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	std::cout << "Unload Data Called" << std::endl;
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	std::cout << "Actors Removed" << std::endl;
	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i);
	}

	mTextures.clear();
	std::cout << "Textures cleared" << std::endl;
	for (int i = 0; i < 20; i++)
	{
		delete[] mGrid[i];
	}
	delete[] mGrid;
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

bool Game::CheckGrid(int row, int col)
{

	if (mGrid[row + 1][col] == 'X')
	{
		return true;
	}

	return false;
}

void Game::ShutDown()
{
	std::cout << "Shutdown Called" << std::endl;
	UnloadData();
	std::cout << "Data Unloaded" << std::endl;
	IMG_Quit();
	std::cout << "Audio system shutdown called" << std::endl;
	mAudioSystem->ShutDown();
	// shut down renderer
	SDL_DestroyRenderer(mRenderer);
	std::cout << "Renderer Destroyed" << std::endl;
	// destroy the window
	SDL_DestroyWindow(mWindow);
	std::cout << "Window Destroyed" << std::endl;
	// close the sdl with quit
	SDL_Quit();
	std::cout << "SDL closed" << std::endl;
}

