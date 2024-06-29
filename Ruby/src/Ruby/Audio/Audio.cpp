#include "ruby_pch.h"
#include "Ruby/Main/Core.h"
#include "Audio.h"
#include <soloud.h>
#include <soloud_wav.h>

namespace Ruby 
{
    namespace Audio 
    {
        static SoLoud::Soloud* s_SoloudContext;
        static SoLoud::handle hand;

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

        void play(const AudioClip& clip, float pan)
        {
            hand = s_SoloudContext->play(*clip.getInternalSample(), -1.0f, pan);
        }

        void play3D(const AudioClip& clip, float x, float y)
        {
            hand = s_SoloudContext->play3d(*clip.getInternalSample(), x, y, 0.0f);
        }

        void updateListener(float x, float y) 
        {
            s_SoloudContext->set3dListenerPosition(x, y, 0.0f);
            s_SoloudContext->update3dAudio();
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