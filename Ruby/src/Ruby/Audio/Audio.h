#pragma once

#include "AudioClip.h"

namespace Ruby {

	namespace Audio {

		void init();
		void deInit();
		void play(const AudioClip& clip);
		void stopAll();
		bool isInitialized();

	}

}