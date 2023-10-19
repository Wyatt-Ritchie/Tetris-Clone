#include "AudioSystem.h"
#include "SDL_log.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

AudioSystem::AudioSystem(Game* game) : mGame(game)
									  ,mSystem(nullptr)
									  ,mLowLevelSystem(nullptr)								  
{}

AudioSystem::~AudioSystem()
{
	ShutDown();
}

int AudioSystem::sNextID = 0;

bool AudioSystem::Initialize()
{
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // log errors
		FMOD_DEBUG_MODE_TTY // output to stdout
	);

	FMOD_RESULT result;

	result = FMOD::Studio::System::create(&mSystem);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	result = mSystem->initialize(
		512,						// max number of channels
		FMOD_STUDIO_INIT_NORMAL,	// use default settings
		FMOD_INIT_NORMAL,			// use default settings
		nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}
	mSystem->getCoreSystem(&mLowLevelSystem);

	// Load the master banks (strings first)
	LoadBank("Assets/Master Bank.string.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Update(float deltaTime)
{
	// find any stopped event instances
	std::vector<unsigned int> done;

	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		// Get state of the event
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);

		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			// release the event and add ID to done
			e->release();
			done.emplace_back(iter.first);
		}
	}

	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	if (mSystem)
	{
		mSystem->update();
	}
	
}

void AudioSystem::LoadBank(const std::string& name)
{
	// Prevent double-loading
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	// Try to load bank
	FMOD::Studio::Bank* bank = nullptr;

	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL, // Normal load
		&bank //save pointer to bank
	);

	const int maxPathLength = 512;	// same as the number of channels
	if (result == FMOD_OK)
	{
		// Add bank to the map
		mBanks.emplace(name, bank);

		// Load all non-streaming data
		bank->loadSampleData();

		// Get number of events in this bank
		int numEvents = 0;
		bank->getEventCount(&numEvents);

		// Get the number of buses in the bank
		int numBus = 0;
		bank->getBusCount(&numBus);

		if (numBus > 0)
		{
			// Get the list of buses
			std::vector<FMOD::Studio::Bus*> buses(numBus);
			bank->getBusList(buses.data(), numBus, &numBus);
			char busName[maxPathLength];
			for (int i = 0; i < numBus; i++)
			{
				FMOD::Studio::Bus* b = buses[i];
				// Get the path to the bus
				b->getPath(busName, maxPathLength, nullptr);
				// Add the bus to the map
				mBuses.emplace(busName, b);
			}
		}

		if (numEvents > 0)
		{
			// Get list of event descriptions in this bank
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				// Get the path of this event
				e->getPath(eventName, maxPathLength, nullptr);
				// Add the event to the map
				mEvents.emplace(eventName, e);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// First remove all the events from this bank
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);

	if (numEvents > 0)
	{
		// Get event description for this bank
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//Get list of events
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];

		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// get path of this event
			e->getPath(eventName, 512, nullptr);
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	// Unload sample data and bank
	bank->unloadSampleData();
	bank->unload();
	// Remove from banks map
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	mEvents.clear();
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int ID)
{
	auto iter = mEventInstances.find(ID);
	if (iter != mEventInstances.end())
	{
		return iter->second;
	}
	else
	{
		return nullptr;
	}
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	// Make sure event exists
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		//Create instance of the event
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		
		if (event)
		{
			// start the event instance
			event->start();
			//Get the next ID and add it to the map
			IncrementNextID();
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float busVol = 0.0f;
	auto iter = mBuses.find(name);

	if (iter != mBuses.end())
	{
		iter->second->getVolume(&busVol);
	}
	return busVol;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool busPause = false;
	auto iter = mBuses.find(name);

	if (iter != mBuses.end())
	{
		iter->second->getPaused(&busPause);
	}
	return busPause;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);

	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = mBuses.find(name);

	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}

void AudioSystem::ShutDown()
{
	if (mSystem)
	{
		mSystem->release();
	}
	
}