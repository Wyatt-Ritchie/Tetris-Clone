#include "LPiece.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Math.h"
LPiece::LPiece(Game* game) : Actor(game)
							, mCenterBlock(nullptr)
							, mTopBlock(nullptr)
							, mBottomBlock(nullptr)
							, mFootBlock(nullptr)
							, mOrientation(0)
							, mBlocks()
							, mCooldown(cooldownFloat)
							, mRotCooldown(rotFloat)
							, mRotUsed(false)
							, mPlaced(false)
							, moveUsed(false)
{
	SetIsPiece(true);
	mCenterBlock = new Block(game);
	mBlocks.push_back(mCenterBlock);
	
	mBottomBlock = new Block(game);
	mBottomBlock->SetIsCenter(false);
	
	mBlocks.push_back(mBottomBlock);
	mTopBlock = new Block(game);
	mTopBlock->SetIsCenter(false);
	
	mBlocks.push_back(mTopBlock);
	mFootBlock = new Block(game);
	mFootBlock->SetIsCenter(false);
	
	mBlocks.push_back(mFootBlock);

	mCenterBlock->SetPosition(Vector2(513.0f, 49.0f));
	Vector2 centerPos = mCenterBlock->GetPosition();
	mBottomBlock->SetPosition(centerPos + Vector2(0.0f, 32.0f));
	mTopBlock->SetPosition(centerPos + Vector2(0.0f, -32.0f));
	mFootBlock->SetPosition(centerPos + Vector2(32.0f, 32.0f));

	mOrientation = 0;
	int n = 0;
	n = rand() % 5 + 1;
	for (auto& e : GetBlocks())
	{
		e->SetScale(0.5f);
		e->SetColour(n);
		e->SetSprite();
	}

	std::cout << "L piece created." << std::endl;
}


LPiece::~LPiece()
{
	
}
void LPiece::SetPlaced()
{
	for (auto& e : mBlocks)
	{
		e->SetState(EPlaced);
	}
}
void LPiece::UpdateActor(float deltaTime)
{
	// Checking to see if any of the blocks are touching the verticle boundary
	// or another block.

	for (auto& e : GetBlocks())
	{
		if (e->GetPosition().y >= 624.0f || BlockVertCollision(e))
		{
			SetState(EPlaced);
			mRotUsed = false;
			break;
		}
	}
	if (getState() == EPlaced && mPlaced == false)
	{
		SetPlaced();
		GetGame()->UpdateGrid(mBlocks);
		SetState(EDead);
		mPlaced = true;
	}

	// This is the cooldown calculation for rotating. Otherwise I find it rotates too quick
	if (mRotUsed == true)
	{
		if (mRotCooldown > 0.0f)
		{
			mRotCooldown -= deltaTime;

		}
		else {
			mRotCooldown = rotFloat;
			mRotUsed = false;
		}
	}
	// The horizontal movement cooldown
	if (mCooldown > 0.0f)
	{
		mCooldown -= deltaTime;
	}
	else
	{
		moveUsed = false;
		mCooldown = cooldownFloat;
	}

}

void LPiece::RotateClockwise()
{
	if (mRotUsed == false)
	{
		mRotUsed = true;
		mOrientation += 90;
		if (mOrientation > 360)
		{
			mOrientation = 90;
		}
		Vector2 centerPos = mCenterBlock->GetPosition();
		

		switch (mOrientation)
		{
		case 90:
			mTopBlock->SetPosition(centerPos + Vector2(32.0f, 0.0f));
			mBottomBlock->SetPosition(centerPos + Vector2(-32.0f, 0.0f));
			mFootBlock->SetPosition(centerPos + Vector2(-32.0f, 32.0f));
			break;
		case 180:
			mTopBlock->SetPosition(centerPos + Vector2(0.0f, 32.0f));
			mBottomBlock->SetPosition(centerPos + Vector2(0.0f, -32.0f));
			mFootBlock->SetPosition(centerPos + Vector2(-32.0f, -32.0f));
			break;
		case 270:
			mTopBlock->SetPosition(centerPos + Vector2(-32.0f, 0.0f));
			mBottomBlock->SetPosition(centerPos + Vector2(32.0f, 0.0f));
			mFootBlock->SetPosition(centerPos + Vector2(32.0f, -32.0f));
			break;
		case 360:
			mTopBlock->SetPosition(centerPos + Vector2(0.0f, -32.0f));
			mBottomBlock->SetPosition(centerPos + Vector2(0.0f, 32.0f));
			mFootBlock->SetPosition(centerPos + Vector2(32.0f, 32.0f));
			break;
		}
		for (auto& e : GetBlocks())
		{
			if (e->GetPosition().x > 656.0f)
			{
				float offset = e->GetPosition().x - 656.0f;
				MovePiece(-1 * (offset + 16));
			}
			if (e->GetPosition().x < 352.0f)
			{
				float offset = 336.0f - e->GetPosition().x;
				MovePiece(offset + 16);
			}
		}

	}
}

void LPiece::MovePiece(float distance)
{
	for (auto& e : GetBlocks())
	{
		e->SetPosition(e->GetPosition() + Vector2(distance, 0.0f));
	}
}

void LPiece::ActorInput(const uint8_t* keyState)
{
	if (getState() == EActive)
	{
		if (keyState[SDL_SCANCODE_W])
		{
			RotateClockwise();
		}
		if (keyState[SDL_SCANCODE_S])
		{
			for (auto& e : mBlocks)
			{
				e->SetPosition(e->GetPosition() + Vector2(0.0f, 32.0f));
			}
		}
		if (keyState[SDL_SCANCODE_D])
		{
			if (!moveUsed)
			{
				MovePiece(32.0f);
				moveUsed = true;
			}

			for (auto& e : mBlocks)
			{
				if (BlockHorCollision(e)) MovePiece(-32.0f);
				if (e->GetPosition().x > 657.0f)
				{
					float offset = e->GetPosition().x - 657.0f;
					MovePiece(-1 * (offset + 16));
				}
			}

		}
		if (keyState[SDL_SCANCODE_A])
		{
			if (!moveUsed)
			{
				MovePiece(-32.0f);
				moveUsed = true;
			}
			for (auto& e : mBlocks)
			{
				if (BlockHorCollision(e)) MovePiece(32.0f);
				if (e->GetPosition().x < 353.0f)
				{
					float offset = 337.0f - e->GetPosition().x;
					MovePiece(offset + 16);
				}
			}
		}
	}
}

bool LPiece::BlockVertCollision(Block* block)
{
	int row, col;
	Vector2 pos = block->GetPosition();
	row = (int)pos.y / 32;
	col = (int)(pos.x - 334) / 32;
	if (row > 20) return false;
	if (GetGame()->CheckGrid(row, col))
	{
		return true;
	}
	return false;
}

bool LPiece::BlockHorCollision(Block* block)
{
	if (GetGame()->GetPieces().empty()) return false;
	std::vector<Actor*> pieces = GetGame()->GetPieces();
	for (auto& p : pieces)
	{
		if (p->getState() != EPlaced && !p->GetIsPiece()) {}
		else if (Math::Abs(p->GetPosition().x - block->GetPosition().x) < 32 && block->GetPosition().y == p->GetPosition().y)
		{
			return true;
		}
	}
	return false;
}

