#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>

class Game
{
public:
	// constructor
	Game();

	// initialize
	bool Initialize();

	// Run Loop
	void RunLoop();

	// add/remove actor functions
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	void PrintGrid();
	void UpdateGrid(std::vector<class Block*> blocks);

	void GenerateNextPiece();
	void GenerateLPiece();
	void GenerateRLPiece();
	void GenerateFlatPiece();
	void GenerateZPiece();
	void GenerateRZPiece();
	void GenerateTPiece();
	void GenerateSquarePiece();
	void ResetGrid();
	void SetActivePiece(class Actor* piece) { ActivePiece = piece; }
	class Actor* GetActivePiece() const { return ActivePiece; }
	// Check each row to see if it is full
	void CheckRows();
	

	SDL_Texture* GetTexture(int c);
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }

	std::vector<class Actor*> GetPieces() { return mActors; }
	bool CheckGrid(int row, int col);

	//shutdown game loop
	void ShutDown();
private:

	// helper functions for the game loop
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	void LoadData();

	void UnloadData();

	// Window created by SDL
	SDL_Window* mWindow;

	// game running bool
	bool mIsRunning;

	// SDL renderer
	SDL_Renderer* mRenderer;

	// FMOD audio system
	class AudioSystem* mAudioSystem;

	class Actor* ActivePiece;

	char** mGrid;

	double mSpawnTimer;
	double mDropTimer;

	// vectors containing active actors and pending actors
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class Block*> mBlocksToDrop;

	int mTicksCount;
	std::vector<SDL_Texture*> mTextures;
	
	bool mUpdatingActors;
};

