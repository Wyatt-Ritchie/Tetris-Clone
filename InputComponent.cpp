#include "InputComponent.h"
#include "Math.h"
#include <iostream>
#include "Actor.h"
InputComponent::InputComponent(Actor* actor) : MoveComponent(actor)
											  ,mMaxAngularSpeed(Math::Pi)
											  
											  
{
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// calculate the forward speed of movecomponent

	if (keyState[mForwardKey])
	{
		Vector2 pos = mOwner->GetPosition();
		pos -= Vector2(0.0f, -10.0f);
		mOwner->SetPosition(pos);
	}

	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		
	}
	if (keyState[mCounterClockwiseKey])
	{
		
	}

	
}