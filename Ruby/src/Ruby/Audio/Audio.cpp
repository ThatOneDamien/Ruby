#include "ruby_pch.h"

#include "Audio.h"
#include "Ruby/Main/Core.h"

#include <unordered_map>
#include <soloud.h>
#include <soloud_wav.h>

namespace Ruby 
{
    namespace Audio 
    {
        static SoLoud::Soloud s_SoloudContext;
        static SoLoud::handle hand;
        static bool s_Initialized = false;
        static size_t s_Counter = 0;

        static std::unordered_map<std::string, AudioClip> s_FilenameToIndex;
        static std::unordered_map<AudioClip, SoLoud::Wav> s_ClipToInternal;

        void init()
        {
            if(s_Initialized)
            {
                RB_WARN("Attempted to initialize Audio Engine more than once. Ignoring...");
                return;
            }
            s_SoloudContext.init();

            s_Initialized = true;
        }

        void deInit()
        {
            if(!s_Initialized)
            {
                RB_WARN("Attempted to deinitialize Audio Engine before it was initialized. Ignoring...");
                return;
            }

            s_FilenameToIndex.clear();
            s_ClipToInternal.clear();
            s_SoloudContext.deinit();
            s_Initialized = false;
        }

        AudioClip createAndCache(const std::string& filepath)
        {
            if(s_FilenameToIndex.count(filepath))
                return s_FilenameToIndex[filepath];

            s_FilenameToIndex.emplace(filepath, s_Counter);
            s_ClipToInternal.emplace(s_Counter, SoLoud::Wav{});
            s_ClipToInternal[s_Counter].load(filepath.c_str());
            ++s_Counter;
            return s_Counter - 1;
        }

        void play(AudioClip clip, float volume, float pan)
        {
            hand = s_SoloudContext.play(s_ClipToInternal[clip], volume, pan);
        }

        void play3D(AudioClip clip, const glm::vec3& position, float volume)
        {
            s_ClipToInternal[clip].set3dAttenuation(SoLoud::AudioSource::INVERSE_DISTANCE, 0.1f);
            hand = s_SoloudContext.play3d(s_ClipToInternal[clip], position.x, position.y, position.z, 0.0f, 0.0f, 0.0f, volume);
        }

        void updateListener(const glm::vec3& listenerPosition, const glm::vec3& listenerTarget) 
        {
            s_SoloudContext.set3dListenerPosition(listenerPosition.x, listenerPosition.y, listenerPosition.z);
            s_SoloudContext.set3dListenerAt(listenerTarget.x, listenerTarget.y, listenerTarget.z);
            s_SoloudContext.update3dAudio();
        }

        void stopAll()
        {
            s_SoloudContext.stopAll();
        }

    }

}
