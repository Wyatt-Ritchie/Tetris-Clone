#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

// This is a class pspecifically for handling background images
class BGSpriteComponent : public SpriteComponent
{
public:
	// set a low draw order
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);

	//update/draw overriden from parent
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;

	// Set textires for background
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);

	// get/set the screen size and the scroll speed
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float scrollSpeed) { mScrollSpeed = scrollSpeed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	// Structure to encapsulate each BG image and its offset
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	// member variables
	Vector2 mScreenSize;
	std::vector<BGTexture> mBGTextures;
	float mScrollSpeed;
};

