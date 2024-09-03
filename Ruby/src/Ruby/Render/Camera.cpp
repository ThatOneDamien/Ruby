#include "ruby_pch.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ruby 
{
    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
    {
        m_View = glm::mat4(1.0f);
        m_Proj = glm::ortho(left, right, bottom, top);
    }


    void OrthoCamera::setViewRadians(const glm::vec3& position, float rotationInRadians)
    {
        if(position == m_Position && rotationInRadians == m_Rotation) return;

        m_Position = position;
        m_Rotation = rotationInRadians;

        m_View = glm::translate(glm::mat4(1.0f), m_Position);
        m_View = glm::rotate(m_View, m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        m_View = glm::inverse(m_View);

        m_ViewProj.Cached = false;
    }

    void OrthoCamera::setProjection(float left, float right, float bottom, float top)
    {
        m_Proj = glm::ortho(left, right, bottom, top);
        m_ViewProj.Cached = false;
    }

    PerspCamera::PerspCamera(float fov, float aspectRatio, float znear, float zfar)
    {
        m_View = glm::lookAt(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
        m_Proj = glm::perspective(fov, aspectRatio, znear, zfar); 
    }

    void PerspCamera::setProjection(float fov, float aspectRatio, float znear, float zfar)
    {
        m_Proj = glm::perspective(fov, aspectRatio, znear, zfar);
        m_ViewProj.Cached = false;
    }

    void PerspCamera::setViewRadians(const glm::vec3& position, float yaw, float pitch)
    {
        if(position == m_Position && yaw == m_Yaw && pitch == m_Pitch) return;
        
        m_Position = position;
        
        if(yaw != m_Yaw || pitch != m_Pitch)
        {
            m_Yaw = yaw;
            m_Pitch = pitch;
            m_Front.x = cos(yaw) * cos(pitch);
            m_Front.y = sin(pitch);
            m_Front.z = sin(yaw) * cos(pitch);
            m_Front = glm::normalize(m_Front);
            m_Right = glm::normalize(glm::cross(m_Front, PerspCamera::WORLD_UP));
            m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
        }

        m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up); 
        m_ViewProj.Cached = false;
    }

    void PerspCamera::axisTranslate(const glm::vec3& translation)
    {
        glm::vec3 newPosition = m_Position;
        newPosition += translation.x * m_Right;
        newPosition += translation.y * PerspCamera::WORLD_UP;
        newPosition += translation.z * glm::normalize(glm::vec3{m_Front.x, 0.0f, m_Front.z});
        setViewRadians(newPosition, m_Yaw, m_Pitch);
    }

}
