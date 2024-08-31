#pragma once

#include "Ruby/Main/App.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
        inline void setViewDegrees(const glm::vec3& position, float rotationInDegrees) 
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
        inline void setRotationDegrees(float rotationInDegrees) { setRotationRadians(glm::radians(rotationInDegrees)); }

        inline const glm::mat4& getViewMatrix() const           { return m_View; }
        inline const glm::mat4& getProjectionMatrix() const     { return m_Proj; }
        inline const glm::vec3& getPosition() const             { return m_Position; }
        inline float getRotation() const                        { return m_Rotation; }

        inline const glm::mat4& getViewProjectionMatrix() const 
        {
            if(m_CachedViewProj)
            {
                m_ViewProj = m_Proj * m_View;
                m_CachedViewProj = true;
            }
            return m_ViewProj; 
        }



    private:
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float     m_Rotation = 0.0f;
        glm::mat4 m_View;
        glm::mat4 m_Proj;
        mutable glm::mat4 m_ViewProj;
        mutable bool      m_CachedViewProj = false;
    };


    class PerspCamera
    {
    public:
        PerspCamera(float fov, float aspectRatio, float znear, float zfar);

        PerspCamera() = delete;
        PerspCamera(const PerspCamera&) = default;
        ~PerspCamera() = default;

        inline const glm::mat4& getViewMatrix() const           { return m_View; }
        inline const glm::mat4& getProjectionMatrix() const     { return m_Proj; }
        inline const glm::mat4& getViewProjectionMatrix() const { return m_ViewProj; }
        inline const glm::vec3& getPosition() const             { return m_Position; }



    private:
        glm::mat4 m_View;
        glm::mat4 m_Proj;
        glm::mat4 m_ViewProj;
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
        float m_Yaw, m_Pitch;
    };

}
