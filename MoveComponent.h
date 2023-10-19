#pragma once
#include "Component.h"
#include "Math.h"
class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* actor);
	

	void Update(float deltaTime) override;

	// Getters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetInvMass() const { return mInvMass; }
	float GetMass() const { return mMass; }
	float GetDamper() const { return mDamper; }
	float GetMaxVelocity() const { return mMaxVelocity; }
	Vector2 GetVelocity() const { return mVelocity; }
	Vector2 GetAcceleration() const { return mAcceleration; }

	// Setters
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }	
	void SetInvMass() { mInvMass = 1 / mMass; }
	void SetMass(float mass) { mMass = mass; }	
	void SetDamper(float damp);
	void SetAcceleration(Vector2 acc) { mAcceleration = acc; }
	void SetVelocity(Vector2 vel) { mVelocity = vel; }
	void SetMaxVelocity(float max) { mMaxVelocity = max; }
	
	// Add a force to the Sum of Forces
	void AddForce(Vector2 force) { mSumOfForces += force; }
	// Clear the Sum of Forces
	void ClearForces();
	// Calculates the position, velocity, and acceleration of the object
	// clears the sum of forces at then end
	void Integrator(float deltaTime);

private:
	// constrols the rotation (radians per sec)
	float mAngularSpeed;
	// constrols the forward speed (units per sec)
	float mMaxVelocity;
	// Madss of the object
	float mMass;
	// The inverse of the mass for calculating Force
	float mInvMass;
	// The damper/drag that is used to avoid hardware calculation errors
	float mDamper;
	// Velocity of the object
	Vector2 mVelocity;
	// Acceleration of the object
	Vector2 mAcceleration;
	// Sum of foces acting on the object
	Vector2 mSumOfForces;
};

