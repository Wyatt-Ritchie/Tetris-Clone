#pragma once

class AIState
{
public:

	AIState(class AIComponent* owner) : mOwner(owner) {}

	// State Specific behavior 
	virtual void Update(float deltaTime) = 0;

	virtual void OnEnter() = 0;

	virtual void OnExit() = 0;

	// Getter for string name of state
	virtual const char* GetName() const = 0;

protected:

	class AIComponent* mOwner;
};

