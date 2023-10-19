#pragma once
#include "Component.h"
#include "SDL.h"
class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);
	virtual void setTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

protected:
	// Texture to draw
	SDL_Texture* mTexture;
	// Draw order used for painters algo
	int mDrawOrder;
	// dimensions of texture
	int mTexWidth;
	int mTexHeight;
};