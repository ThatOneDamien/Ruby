#include "ruby_pch.h"
#include "AudioClip.h"
#include <soloud.h>
#include <soloud_wav.h>

namespace Ruby {

	namespace Audio
	{
		bool isInitialized();
	}

	AudioClip::AudioClip(const char* filename, const std::string& name)
		: m_Name(name)
	{
		m_Sample = new SoLoud::Wav();
		m_Sample->load(filename);
	}

	AudioClip::AudioClip(const char* filename)
		: AudioClip::AudioClip(filename, filename)
	{}

	AudioClip::~AudioClip() 
	{
		if (Audio::isInitialized())
			delete m_Sample;
	}

}