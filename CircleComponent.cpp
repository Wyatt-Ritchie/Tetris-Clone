#include "CircleComponent.h"
#include "Actor.h"
#include "Math.h"
#include <iostream>
CircleComponent::CircleComponent(Actor* owner) : Component(owner) 
												,mRadius(0.0f)
{}

const Vector2& CircleComponent::GetCenter() const
{
	Vector2 pos = mOwner->GetPosition();
	//std::cout << "Get Center call\n Position x: " << pos.x << " Position y: " << pos.y << std::endl;
	return pos;
}

float CircleComponent::GetRadius() const
{
	return mOwner->GetScale() * mRadius;
}
bool Intersect(const CircleComponent& circA, const CircleComponent& circB)
{
	Vector2 aCenter = circA.GetCenter();
	Vector2 bCenter = circB.GetCenter();
	
	Vector2 diff = Vector2(aCenter.x - bCenter.x, aCenter.y - bCenter.y);
	
	float distSq = diff.LengthSq();
	
	// calcualte the sum of radii squared
	float radiiSq = circA.GetRadius() + circB.GetRadius();

	radiiSq *= radiiSq;
	return distSq <= radiiSq;
}