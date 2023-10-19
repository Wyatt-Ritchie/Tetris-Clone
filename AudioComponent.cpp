#include "AudioComponent.h"
#include "Actor.h"
#include "AudioSystem.h"

AudioComponent::AudioComponent(Actor* owner, int updateOrder) : Component(owner, updateOrder)
{
}

AudioComponent::~AudioComponent()
{
	StopAllEvents();
}

void AudioComponent::Update(float deltaTime)
{
	auto iter = mEvents.begin();
	while (iter != mEvents.end())
	{
		if (!iter->IsValid())
		{
			iter = mEvents.erase(iter);
		}
		else {
			++iter;
		}
	}
}

SoundEvent AudioComponent::PlayEvent(const std::string& name)
{
	SoundEvent e = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);
	mEvents.emplace_back(e);
	return e;
}

void AudioComponent::StopAllEvents()
{
	for (auto& e : mEvents)
	{
		e.Stop();
	}
	mEvents.clear();
}
