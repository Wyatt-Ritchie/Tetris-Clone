#pragma once
#include <string>
class SoundEvent
{
public:
	SoundEvent();

	// Returns true if associated FMOD event instance is valid
	bool IsValid();
	
	// Restart event from beginning
	void Restart();

	// Stop the event
	void Stop(bool allowFadeOut = true);

	// Setters
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	// Getters
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
protected:
	// Make this constructor proteced and AudioSystem a friend
	// so that only AudioSystem can access the constructor
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int ID);
private:
	class AudioSystem* mSystem;
	unsigned int mID;

};

