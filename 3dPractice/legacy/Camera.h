#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Input.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Update(Input* _input, glm::mat4* _model, glm::vec3 _targetPosition);

	float GetCamAngleX() { return m_camAngleX; }
	float GetCamAngleY() { return m_camAngleY; }

private:

	float m_camAngleX;
	float m_camAngleY;

	glm::vec3 m_camPosition;
	glm::mat4 tempMat;

};