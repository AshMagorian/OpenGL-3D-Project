#include "Camera.h"

#include <iostream>


Camera::Camera()
{
	m_camAngleX = 0.0f;
	m_camAngleY = 0.0f;
	m_camPosition = glm::vec3(0.0f);
	tempMat = glm::mat4(1.0f);
}

Camera::~Camera()
{

}

void Camera::Update(Input* _input, glm::mat4* _model, glm::vec3 _targetPosition)
{
	tempMat = glm::mat4(1.0f);
	float m_distanceFromObject = 20.0f; // Distance between the camera and the object

	m_camPosition = glm::vec3(_targetPosition.x, _targetPosition.y + 5.0f, _targetPosition.z); // Moves the camera to the position of the ship with an offset of 5 in the y axis

	m_camAngleX += _input->GetDeltaMouseX() / 4.0f;
	m_camAngleY += _input->GetDeltaMouseY() / 4.0f; // Reads the mouse input and converts it into an angle
	if (m_camAngleY > 45) { m_camAngleY = 45; } 
	if (m_camAngleY < -45) { m_camAngleY = -45; }
	if (m_camAngleX > 45) { m_camAngleX = 45; }
	if (m_camAngleX < -45) { m_camAngleX = -45; } // Caps the maximum angle

	tempMat = glm::rotate(tempMat, glm::radians(m_camAngleX), glm::vec3(0, 1, 0)); // Rotates the matrix in the y axis
	tempMat = glm::rotate(tempMat, glm::radians(m_camAngleY), glm::vec3(1, 0, 0)); // Rotates the matrix in the x axis
	tempMat = glm::translate(tempMat, glm::vec3(0, 0, 1));

	
	glm::vec3 fwd = tempMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	if (fwd != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		fwd = glm::normalize(fwd) * m_distanceFromObject; // Gets a unit vector of the direction and then moves back by the camera distance
		m_camPosition += fwd;
	}
	
	*_model = glm::translate(*_model, m_camPosition);
	*_model = glm::rotate(*_model, glm::radians(m_camAngleX), glm::vec3(0, 1, 0));
	*_model = glm::rotate(*_model, glm::radians(m_camAngleY), glm::vec3(1, 0, 0));

	//std::cout << "x position = " << m_camPosition.x << "\t y position = " << m_camPosition.y << "\tz position = " << m_camPosition.z << std::endl;
}