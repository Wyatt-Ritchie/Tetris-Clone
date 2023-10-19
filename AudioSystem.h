#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"

namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	}
}

class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();
	
	bool Initialize();
	void ShutDown();

	void Update(float deltaTime);

	// Function for loading the bank, and sample data
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	// Functions for controlling the buses
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);

	// Getters and setters
	void IncrementNextID() { sNextID++; }
	int GetNextID() const { return sNextID; }

protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int ID);
private:
	// game member variable
	class Game* mGame;
	// setting the ID for the next event instance
	static int sNextID;
	//FMOD 
	FMOD::Studio::System* mSystem;
	//FMOD low level 
	FMOD::System* mLowLevelSystem;

	// Banks
	// Map of loaded banks. The string is the filename of the bank
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	// Map of event name to event description. The string is the name assigned
	// by FMOD for the event. 
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	// Maps unsigned ints to event instances
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	// Map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
};

