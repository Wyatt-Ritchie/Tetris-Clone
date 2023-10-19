#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>
class AIComponent : public Component
{

public:

	AIComponent(class Actor* owner);

	void Update(float deltaTime) override;

	void ChangeState(const std::string& name);

	// add a new state to the state map
	void RegisterState(class AIState* state);

private:

	// Map containing the name of the state and the AIState instance

	std::unordered_map<std::string, class AIState*> mStateMap;

	// Current state of the Actor
	class AIState* mCurrentState;

};

