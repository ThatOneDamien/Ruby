#include "ruby_pch.h"
#include "Ruby/Main/Core.h"
#include "Audio.h"
#include <soloud.h>
#include <soloud_wav.h>

namespace Ruby {

	namespace Audio {

		static SoLoud::Soloud* s_SoloudContext;

		void init()
		{
			s_SoloudContext = new SoLoud::Soloud();
			s_SoloudContext->init();
		}

		void deInit()
		{
			s_SoloudContext->deinit();
			delete s_SoloudContext;
			s_SoloudContext = nullptr;
		}

		void play(const AudioClip& clip)
		{
			s_SoloudContext->play(*clip.getInternalSample());
		}

		void stopAll()
		{
			s_SoloudContext->stopAll();
		}

		bool isInitialized()
		{
			return s_SoloudContext;
		}

	}

}