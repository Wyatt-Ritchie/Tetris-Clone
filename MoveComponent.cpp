#include "MoveComponent.h"
#include "Math.h"
#include "Actor.h"
#include <iostream>
#define real_pow powf
MoveComponent::MoveComponent(Actor* actor) : Component(actor)
											,mAngularSpeed(0.0f)
											,mMass(1.0f)
											,mSumOfForces(Vector2(0.0f, 0.0f))
											,mAcceleration(Vector2(0.0f, 0.0f))
											,mVelocity(Vector2(0.0f, 0.0f))
											,mMaxVelocity(150.0f)
											,mDamper(0.999f)
											
{
	SetInvMass();
}

// Damper value must be <=1 and > 0
void MoveComponent::SetDamper(float damper)
{
	if (damper <= 1.0f && damper > 0.0f)
	{
		mDamper = damper;
	}
	else
	{
		std::cout << "Please set damper within the bounds of 0.0 to 1.0" << std::endl;
	}
}

void MoveComponent::ClearForces()
{
	mSumOfForces.x = 0.0f;
	mSumOfForces.y = 0.0f;
}

void MoveComponent::Integrator(float deltaTime)
{
	// calculate the position of the particle
	Vector2 pos = mOwner->GetPosition();
	pos += mVelocity * deltaTime;
	mOwner->SetPosition(pos);

	// Work out acceleration from the forces
	mAcceleration = mSumOfForces * mInvMass;

	// update velocity
	mVelocity += mAcceleration * deltaTime;

	// impose dampening/drag
	mVelocity *= real_pow(mDamper, deltaTime);

	// clear forces
	ClearForces();
}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	
	Integrator(deltaTime);
	

	// For asteroid game we can include some code for screen wrapping
	// for generic funcitions we do not include this.

	// Asteroids wrapping code
	Vector2 pos = mOwner->GetPosition();
	if (pos.x < -16)
	{
		mOwner->SetPosition(Vector2(1040, pos.y));
	}
	if (pos.x > 1040)
	{
		mOwner->SetPosition(Vector2(-16, pos.y));
	}
	if (pos.y < -16)
	{
		mOwner->SetPosition(Vector2(pos.x, 784));
	}
	if (pos.y > 784)
	{
		mOwner->SetPosition(Vector2(pos.x, -16));
	}
	
}