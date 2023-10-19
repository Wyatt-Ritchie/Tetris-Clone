#pragma once
#include "Actor.h"
#include "Block.h"

class TPiece : public Actor
{
public:
	TPiece(class Game* game);
	~TPiece();
	void RotateClockwise();
	void MovePiece(float distance);
	void ActorInput(const uint8_t* keyState);
	void UpdateActor(float deltaTime);
	bool BlockVertCollision(class Block* block);
	bool BlockHorCollision(class Block* block);
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
	Block* mLeftBlock;
	Block* mTopBlock;
	Block* mRightBlock;
};

