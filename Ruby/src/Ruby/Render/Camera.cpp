#include "ruby_pch.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ruby 
{
    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
    {
        m_View = glm::mat4(1.0f);
        m_Proj = glm::ortho(left, right, bottom, top);
        m_ViewProj = m_Proj;
    }


    void OrthoCamera::setViewRadians(const glm::vec3& position, float rotationInRadians)
    {
        if(position == m_Position && rotationInRadians == m_Rotation) return;

        m_Position = position;
        m_Rotation = rotationInRadians;

        m_View = glm::translate(glm::mat4(1.0f), m_Position);
        m_View = glm::rotate(m_View, m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        m_View = glm::inverse(m_View);
    }

    void OrthoCamera::setProjection(float left, float right, float bottom, float top)
    {
        m_Proj = glm::ortho(left, right, bottom, top);
    }

    PerspCamera::PerspCamera(float fov, float aspectRatio, float znear, float zfar)
    {
        m_Yaw = 0.0f;
        m_Pitch = 0.0f;
        m_View = glm::lookAt(glm::vec3{0.0f, 0.0f, -3.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
        m_Proj = glm::perspective(fov, aspectRatio, znear, zfar); 
    }

}
