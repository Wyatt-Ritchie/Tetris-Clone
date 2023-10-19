#pragma once
#include "MoveComponent.h"
#include <cstdint>
class InputComponent : public MoveComponent
{
public:
	InputComponent(class Actor* owner);

	void ProcessInput(const std::uint8_t* keyState) override;

	// Getters/setter for private vars

	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	float GetMaxAngularSpeed() const { return mMaxAngularSpeed; }
	
	void SetForwardKey(int key) { mForwardKey = key; }
	int GetForwardKey() const { return mForwardKey; }

	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	int GetClockwiseKey() const { return mClockwiseKey; }

	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

private:
	// max speeds

	float mMaxAngularSpeed;

	int mForwardKey;

	// keys for angular movement

	int mClockwiseKey;
	
	int mCounterClockwiseKey;
};

