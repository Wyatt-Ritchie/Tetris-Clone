#include "AIComponent.h"
#include "SDL_log.h"
#include "AIState.h"
AIComponent::AIComponent(Actor* owner) : Component(owner)
										,mCurrentState(nullptr)
{}

void AIComponent::ChangeState(const std::string& name)
{
	// Exit current state
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	// find the new state in the state map
	auto iter = mStateMap.find(name);
	if (iter != mStateMap.end())
	{
		mCurrentState = iter->second;
		mCurrentState->OnEnter();
	}
	else
	{
		SDL_Log("Could not find AIState %s in state map.", name);
		mCurrentState = nullptr;
	}
}

void AIComponent::RegisterState(AIState* state)
{
	mStateMap.emplace(state->GetName(), state);
}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}