#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
class AnimateSpriteComponent : public SpriteComponent
{
	// structure for representing a single animation
	struct Animation
	{
		std::string mAnimName;
		int mStartPos, mEndPos;
		bool mLoop;
	};

public:
	AnimateSpriteComponent(class Actor* owner, int drawOrder = 100);

	void Update(float deltaTime) override;

	void SetCurrentAnimation(std::string animationName);

	Animation *GetCurrentAnimation() const { return mCurrentAnimation; }

	void SetAnimationTextures(const std::vector<SDL_Texture*>& textures
								,std::string animName
								,bool looping);

	float GetAnimFPS() const { return mAnimFPS; };
	void SetAnimFPS(float fps) { mAnimFPS = fps; };

private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;

	// all the animations
	std::vector<Animation*> mAnimations;

	Animation *mCurrentAnimation;

	const Uint8* state;

	// Current fram displayed
	float mCurrentFrame;

	// Animation FPS
	float mAnimFPS;
};

