#pragma once
#include <vector>
#include "Math.h"
#include "Game.h"
#include <cstdint>

constexpr double cooldownFloat = 0.07;
constexpr double rotFloat = 0.2;

class Actor
{
public:
	// track state of actor
	enum State
	{
		EActive,
		EPlaced,
		EPaused,
		EDead
	};

	//default constructor/destructor
	Actor(class Game* game);

	virtual ~Actor();

	// update function called from Game (not override)
	void Update(float deltaTime);

	// updates all the components attached to the actor
	void UpdateComponents(float deltaTime);

	// Any actor specific update code (override)
	virtual void UpdateActor(float deltaTime);

	// getters/setters
	Vector2 GetPosition() const { return mPosition; };

	void SetPosition(Vector2 position);
	void SetScale(float scale);
	void SetRotation(float rotation) { mRotation = rotation; }

	// Add/remove components
	void AddComponent(class Component* component);

	void RemoveComponent(class Component* component);

	Game* GetGame() const { return mGame; };

	std::vector<class Component*> GetComponents() { return mComponents; };

	State getState();
	void SetState(State state) { mState = state; }

	void SetIsPiece(bool piece) { mIsPiece = piece; }
	bool GetIsPiece() const { return mIsPiece; }

	float GetScale() const { return mScale; };
	float GetRotation() const { return mRotation; };
	// returns the forward pointing vector
	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }

	void ProcessInput(const uint8_t* keyState);

	virtual void ActorInput(const uint8_t* keyState);
	
private:
	// Actor's state
	State mState;
	bool mIsPiece;
	// Transform
	Vector2 mPosition; // center position of actor

	float mScale;	   // Uiform scale of actor

	float mRotation;   // Rotation angle in radians

	// Componenets held by this actor
	std::vector<class Component*> mComponents;

	class Game* mGame;
};

