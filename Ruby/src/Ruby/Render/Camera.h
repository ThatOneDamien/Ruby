#pragma once

#include "Ruby/Main/App.h"
#include "Ruby/Utility/CachedVar.h"

#include <glm/glm.hpp>

namespace Ruby 
{
    class OrthoCamera
    {
    public:
        OrthoCamera(float left, float right, float bottom, float top);
        inline OrthoCamera(float aspectRatio, float scale = 1.0f)
            : OrthoCamera(-aspectRatio * scale, aspectRatio * scale, -scale, scale)
        {}

        OrthoCamera() = delete;
        OrthoCamera(const OrthoCamera&) = default;
        ~OrthoCamera() = default;

        void setViewRadians(const glm::vec3& position, float rotationInRadians);
        inline void setView(const glm::vec3& position, float rotationInDegrees) 
        { 
            setViewRadians(position, glm::radians(rotationInDegrees));
        }


        void setProjection(float left, float right, float bottom, float top);
        inline void setProjection(float aspectRatio, float scale = 1.0f)
        {
            setProjection(-aspectRatio * scale, aspectRatio * scale, -scale, scale);
        }

        inline void setPosition(const glm::vec3& position) { setViewRadians(position, m_Rotation); }
        inline void setPosition(float x, float y, float z) { setPosition({x, y, z}); }


        inline void setRotationRadians(float rotationInRadians) { setViewRadians(m_Position, rotationInRadians); }
        inline void setRotation(float rotationInDegrees) { setRotationRadians(glm::radians(rotationInDegrees)); }

        inline float getRotation() const                        { return glm::degrees(m_Rotation); }
        inline const glm::vec3& getPosition() const             { return m_Position; }
        inline const glm::mat4& getViewMatrix() const           { return m_View; }
        inline const glm::mat4& getProjectionMatrix() const     { return m_Proj; }

        inline const glm::mat4& getViewProjectionMatrix() const 
        {
            if(m_ViewProj.Cached)
            {
                m_ViewProj.Value = m_Proj * m_View;
                m_ViewProj.Cached = true;
            }
            return m_ViewProj.Value; 
        }



    private:
        float     m_Rotation = 0.0f;
        glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
        glm::mat4 m_View;
        glm::mat4 m_Proj;
        mutable CachedVar<glm::mat4> m_ViewProj;
    };


    class PerspCamera
    {
    public:
        PerspCamera(float fov, float aspectRatio, float znear = 0.1f, float zfar = 100.0f);

        PerspCamera() = delete;
        PerspCamera(const PerspCamera&) = default;
        ~PerspCamera() = default;

        void setProjection(float fov, float aspectRatio, float znear = 0.1f, float zfar = 100.0f);
        void setViewRadians(const glm::vec3& position, float yaw, float pitch);
        inline void setViewDegrees(const glm::vec3& position, float yaw, float pitch)
        {
            setViewRadians(position, glm::radians(yaw), glm::radians(pitch));
        }

        void axisTranslate(const glm::vec3& translation);

        inline void setPosition(const glm::vec3& position) { setViewRadians(position, m_Yaw, m_Pitch); }
        inline void setPosition(float x, float y, float z) { setViewRadians({x, y, z}, m_Yaw, m_Pitch); }

        inline float getYaw() const { return glm::degrees(m_Yaw); }
        inline float getPitch() const { return glm::degrees(m_Pitch); }
        inline const glm::vec3& getTarget() const { return m_Front; }
        inline const glm::vec3& getPosition() const             { return m_Position; }
        inline const glm::mat4& getViewMatrix() const           { return m_View; }
        inline const glm::mat4& getProjectionMatrix() const     { return m_Proj; }
        inline const glm::mat4& getViewProjectionMatrix() const 
        {
            if(!m_ViewProj.Cached)
            {
                m_ViewProj.Value = m_Proj * m_View; 
                m_ViewProj.Cached = true;
            }
            return m_ViewProj.Value; 
        }


    private:
        static constexpr glm::vec3 WORLD_UP = { 0.0f, 1.0f, 0.0f };

        float m_Yaw = glm::radians(-90.0f);
        float m_Pitch = 0.0f;
        glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 m_Front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 m_Up{ 0.0f, 1.0f, 0.0f };
        glm::vec3 m_Right{ 1.0f, 0.0f, 0.0f };
        glm::mat4 m_View;
        glm::mat4 m_Proj;
        mutable CachedVar<glm::mat4> m_ViewProj;
    };

}
