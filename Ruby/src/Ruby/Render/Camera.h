#pragma once

#include <glm/glm.hpp>

namespace Ruby {

	// Orthographic Camera simply named Camera because it is default. Might add perspective cam later, though this is a 2D engine.
	class Camera
	{
	public:
		Camera(float left, float right, float bottom, float top);
		Camera(float aspectRatio, float scale = 1.0f);
		~Camera() = default;


		void setView(const glm::vec2& position, float rotationInDegrees);
		void setViewRadians(const glm::vec2& position, float rotationInRadians);
		void setProjection(float left, float right, float bottom, float top);
		void setProjection(float aspectRatio, float scale = 1.0f);
		void setPosition(float x, float y);
		void setPosition(const glm::vec2& position);
		void setRotation(float rotationInDegrees);
		void setRotationRadians(float rotationInRadians);

		inline const glm::mat4& getViewMatrix() const { return m_View; }
		inline const glm::mat4& getProjectionMatrix() const { return m_Proj; }
		inline const glm::mat4& getViewProjectionMatrix() const { return m_ViewProj; }
		inline const glm::vec2& getPosition() const { return m_Position; }
		inline float getRotation() const { return m_CurrentRotation; }



	private:
		glm::mat4 m_View;
		glm::mat4 m_Proj;
		glm::mat4 m_ViewProj;
		glm::vec2 m_Position = { 0.0f, 0.0f };
		float m_CurrentRotation = 0;
	};

}
