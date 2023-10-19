#pragma once
#include "MoveComponent.h"
class NavComponent : public MoveComponent
{
public:
	NavComponent(class Actor* owner);

	// Update tests whether the actor reaches next point
	void Update(float deltaTime) override;

	// this function gets the next point in the list of points
	Vector2 GetNextPoint();

	void TurnTo(const Vector2& pos);
private:
	Vector2 mNextPoint;
};

