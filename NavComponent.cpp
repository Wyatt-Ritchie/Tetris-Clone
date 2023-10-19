#include "NavComponent.h"
#include "Math.h"
#include "Actor.h"
NavComponent::NavComponent(Actor* owner) : MoveComponent(owner)
{}

void NavComponent::Update(float deltaTime)
{
	// 
	Vector2 diff = mOwner->GetPosition() - mNextPoint;

	if (diff.Length() <= 2.0f)
	{
		mNextPoint = GetNextPoint();
		TurnTo(mNextPoint);
	}

	// This moves the actor forward
	MoveComponent::Update(deltaTime);
}

void NavComponent::TurnTo(const Vector2& pos)
{
	// vector from me to pos
	Vector2 dir = pos - mOwner->GetPosition();

	// New angle is just atan2 of this dir
	// negate y because +y is down on screen

	float angle = Math::Atan2(-dir.y, dir.x);

	mOwner->SetRotation(angle);
}

Vector2 NavComponent::GetNextPoint()
{
	// This is not the correct implementation. 
	// This is simply a placeholder 
	return mOwner->GetPosition();
}