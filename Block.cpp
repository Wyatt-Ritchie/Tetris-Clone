#include "Block.h"
#include "SpriteComponent.h"
#include <iostream>

Block::Block(Game* game) : Actor(game)
						  ,mColour(0)
						  ,mIsCenter(true)
{
	SetIsPiece(false);
}

Block::~Block()
{
}

void Block::SetSprite()
{
	if (mColour != 0)
	{
		SpriteComponent* blockSprite = new SpriteComponent(this, 10);
		blockSprite->setTexture(GetGame()->GetTexture(mColour));
	}
}
