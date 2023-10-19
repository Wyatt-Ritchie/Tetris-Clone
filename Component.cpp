#include "Component.h"
#include "Actor.h"
Component::Component(class Actor* owner, int updateOrder) : mOwner(owner)
															,mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this);
}


Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}