#pragma once
#include "Component.h"
#include <string>
#include <vector>
#include "SoundEvent.h"
class AudioComponent : public Component
{
public:
	AudioComponent(class Actor* owner, int updateOrder);
	~AudioComponent();

	void Update(float deltaTime) override;

	SoundEvent PlayEvent(const std::string& name);

	void StopAllEvents();
private:
	std::vector<SoundEvent> mEvents;

};

