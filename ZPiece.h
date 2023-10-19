#pragma once
#include "Actor.h"
#include "Block.h"

class ZPiece : public Actor
{
public:
	ZPiece(class Game* game);
	~ZPiece();
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
	Block* mTopLeftBlock;
	Block* mRightBlock;
};

