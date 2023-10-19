#pragma once
#include "Actor.h"
#include <map>
class Block : public Actor
{
public:
	Block(class Game* game);
	~Block();
	// Function for getting and setting the correct texture
	// for the block from the colour
	void SetSprite();

	// Getters
	bool GetIsCenter() const { return mIsCenter; }
	int GetColour() const { return mColour; }
	void SetRowAndCol(int row, int col) { row = row; col = col; }

	// Setters
	void SetIsCenter(bool isCenter) { mIsCenter = isCenter; }
	void SetColour(int col) { mColour = col; }

private:
	int mColour;
	bool mIsCenter;
};

