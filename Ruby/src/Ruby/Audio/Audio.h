#pragma once

#include <string>
#include <cstddef>

#include <glm/glm.hpp>


namespace Ruby 
{

    typedef size_t AudioClip; 
    namespace Audio 
    {
        AudioClip createAndCache(const std::string& filepath);
        void play(AudioClip clip, float volume = -1.0f, float pan = 0.0f);
        void play3D(AudioClip clip, const glm::vec3& position, float volume = -1.0f);
        void updateListener(const glm::vec3& listenerPosition, const glm::vec3& listenerTarget);
        void stopAll();

    }

}
