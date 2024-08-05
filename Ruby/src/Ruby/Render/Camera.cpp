#include "ruby_pch.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ruby 
{
    Camera::Camera(float left, float right, float bottom, float top)
        : m_View(1.0f), m_Proj(glm::ortho(left, right, bottom, top))
    {
        m_ViewProj = m_Proj;
    }

    Camera::Camera(float aspectRatio, float scale)
        : m_View(1.0f)
    {
        if (aspectRatio < 0.0f)
        {
            aspectRatio = Ruby::App::getInstance().getWindow().getAspectRatio();
        }

        m_Proj = glm::ortho(-aspectRatio * scale, aspectRatio * scale, -scale, scale);

        m_ViewProj = m_Proj;
    }

    void Camera::setView(const glm::vec3& position, float rotationInDegrees)
    {
        m_Position = position;
        m_CurrentRotation = rotationInDegrees;
        m_View = glm::inverse(glm::rotate(glm::translate(glm::mat4(1.0f), m_Position), glm::radians(m_CurrentRotation), glm::vec3(0.0f, 0.0f, 1.0f)));
        m_ViewProj = m_Proj * m_View;
    }

    void Camera::setViewRadians(const glm::vec3& position, float rotationInRadians)
    {
        m_Position = position;
        m_CurrentRotation = glm::degrees(rotationInRadians);
        m_View = glm::inverse(glm::rotate(glm::translate(glm::mat4(1.0f), m_Position), rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f)));
        m_ViewProj = m_Proj * m_View;
    }

    void Camera::setProjection(float left, float right, float bottom, float top)
    {
        m_Proj = glm::ortho(left, right, bottom, top);
        m_ViewProj = m_Proj * m_View;
    }
    
    void Camera::setProjection(float aspectRatio, float scale)
    {
        m_Proj = glm::ortho(-aspectRatio * scale, aspectRatio * scale, -scale, scale);
        m_ViewProj = m_Proj * m_View;
    }

    void Camera::setPosition(float x, float y)
    {
        glm::vec3 position = { x, y, 0.0f };
        if (position == m_Position) return;

        m_Position = position;
        m_View = glm::inverse(glm::rotate(glm::translate(glm::mat4(1.0f), m_Position), glm::radians(m_CurrentRotation), glm::vec3(0.0f, 0.0f, 1.0f)));
        m_ViewProj = m_Proj * m_View;
    }

    void Camera::setPosition(const glm::vec3& position)
    {
        if (position == m_Position) return;

        m_Position = position;
        m_View = glm::inverse(glm::rotate(glm::translate(glm::mat4(1.0f), m_Position), glm::radians(m_CurrentRotation), glm::vec3(0.0f, 0.0f, 1.0f)));
        m_ViewProj = m_Proj * m_View;
    }

    void Camera::setRotation(float rotationInDegrees)
    {
        if (m_CurrentRotation == rotationInDegrees) return;

        m_CurrentRotation = rotationInDegrees;
        m_View = glm::inverse(glm::rotate(glm::translate(glm::mat4(1.0f), m_Position), glm::radians(m_CurrentRotation), glm::vec3(0.0f, 0.0f, 1.0f)));
        m_ViewProj = m_Proj * m_View;
    }

    void Camera::setRotationRadians(float rotationInRadians)
    {
        m_CurrentRotation = glm::degrees(rotationInRadians);
        m_View = glm::inverse(glm::rotate(glm::translate(glm::mat4(1.0f), m_Position), rotationInRadians, glm::vec3(0.0f, 0.0f, 1.0f)));
        m_ViewProj = m_Proj * m_View;
    }

}
