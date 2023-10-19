#pragma once
#include "Actor.h"
#include "Block.h"
class Piece : public Actor
{
public:
	Piece(class Game* game);

	void ActorInput(const uint8_t* keyState) ;
	void MovePiece(float distance);
	
	std::vector<Block*> GetBlocks() const { return mBlocks; }
private:
	std::vector<Block*> mBlocks;
};

