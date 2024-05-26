#pragma once

// FORWARD DECLARATION
namespace SoLoud 
{
	class Wav;
}

namespace Ruby 
{
	class AudioClip
	{
	public:
		AudioClip(const char* filename, const std::string& name);
		AudioClip(const char* filename);
		~AudioClip();

		inline SoLoud::Wav* getInternalSample() const { return m_Sample; }

	private:
		SoLoud::Wav* m_Sample;
		std::string m_Name;

	};

}