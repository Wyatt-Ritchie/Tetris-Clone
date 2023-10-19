#pragma once
#include "Actor.h"
#include "Block.h"

class ReverseLPiece : public Actor
{
public:
	ReverseLPiece(class Game* game);
	~ReverseLPiece();
	void RotateClockwise();
	void MovePiece(float distance);
	void ActorInput(const uint8_t* keyState);
	bool BlockVertCollision(class Block* block);
	bool BlockHorCollision(class Block* block);
	void UpdateActor(float deltaTime);
	void SetPlaced();
	std::vector<Block*> GetBlocks() const { return mBlocks; }


private:
	double mCooldown;
	double mRotCooldown;
	bool mRotUsed;
	bool moveUsed;
	bool mPlaced;
	int mOrientation;
	std::vector<Block*> mBlocks;
	Block* mCenterBlock;
	Block* mTopBlock;
	Block* mBottomBlock;
	Block* mFootBlock;
};

