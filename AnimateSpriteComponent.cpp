#include "AnimateSpriteComponent.h"
#include <iostream>

AnimateSpriteComponent::AnimateSpriteComponent(Actor* actor, int drawOrder) : SpriteComponent(actor, drawOrder)
																			  ,mCurrentFrame(0.0f)
																			  ,mAnimFPS(24.0f)
																			  ,mCurrentAnimation(nullptr)
																			  ,state(SDL_GetKeyboardState(NULL))
{
	std::vector<SDL_Texture*> mAnimTextures;
	std::vector<Animation*> mAnimations;
}

void AnimateSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// if the animation is non looping and has reached then last frame, do nothing
		if (mCurrentFrame >= mCurrentAnimation->mEndPos && !mCurrentAnimation->mLoop)
		{
			return;
		}
		else {
			// update current frame based on frame rate
			// and delta time
			mCurrentFrame += mAnimFPS * deltaTime;
		}
		
		// if the animation is looping and the last frame is reached, return to the first
		// frame of the animation
		while (mCurrentFrame >= mCurrentAnimation->mEndPos && mCurrentAnimation->mLoop)
		{
			mCurrentFrame -= (mCurrentAnimation->mEndPos - mCurrentAnimation->mStartPos);
		}

		// set the current texture
		setTexture(mAnimTextures[static_cast<int>(mCurrentFrame)]);
	}
}

void AnimateSpriteComponent::SetAnimationTextures(const std::vector<SDL_Texture*>& textures
													,std::string animName
													,bool looping)
{
	// creating a vector of animations to draw from
	Animation *anim = new Animation;
	anim->mAnimName = animName;
	anim->mLoop = looping;
	anim->mStartPos = mAnimTextures.size();
	anim->mEndPos = mAnimTextures.size() + textures.size() - 1;
	mAnimations.push_back(anim);
	std::cout << "Added " << animName << " to the animation vector" << std::endl;

	for (int i = 0; i < textures.size(); i++)
	{
		mAnimTextures.push_back(textures[i]);
	}
}
void AnimateSpriteComponent::SetCurrentAnimation(std::string animationName)
{	
	std::cout << "Setting current animation to " << animationName << std::endl;
	auto iter = mAnimations.begin();

	for (; iter != mAnimations.end(); ++iter)
	{
		if ((*iter)->mAnimName == animationName)
		{
			mCurrentAnimation = (*iter);
			mCurrentFrame = (float)mCurrentAnimation->mStartPos;
			break;
		}
	}
}