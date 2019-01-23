#include "Entity.h"

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h" 

#include <glm/gtc/quaternion.hpp>
#include <random>
#include <iostream>

class Obstacle : public Entity
{
public:
	Obstacle(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale, int _objectType);
	~Obstacle();

	void Update(float _deltaTime);
	void SetAngularVelocity();
	void UpdateModelMatrix(float _deltaTime);

private:

	int m_objectType;
	glm::vec3 m_angularVelocity;
	glm::quat m_rotationQuaternion;
};