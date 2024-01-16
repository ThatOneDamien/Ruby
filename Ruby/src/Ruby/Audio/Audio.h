#pragma once

#include "AudioClip.h"

namespace Ruby {

	namespace Audio {

		void init();
		void deInit();
		void play(const AudioClip& clip, float pan = 0.0f);
		void play3D(const AudioClip& clip, float x, float y);
		void updateListener(float x, float y);
		void stopAll();
		bool isInitialized();

	}

}