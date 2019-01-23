#include "Obstacle.h"

Obstacle::Obstacle(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale, int _objectType)
	: Entity(_texPath, _vaPath, _shader, _angle, _pos, _scale)
{
	m_objectType = _objectType;
}

Obstacle::~Obstacle()
{

}

void Obstacle::Update(float _deltaTime)
{
	SetPosition(GetPosition() + glm::vec3(0.0f, 0.0f, 0.8f));

	if (m_objectType == 0) //If the object is a small asteroid, set a smaller bounding box
	{
		SetaabbMax(GetPosition() + glm::vec3(3.0f, 3.0f, 3.0f));
		SetaabbMin(GetPosition() + glm::vec3(-3.0f, -3.0f, -3.0f));
	}
	if (m_objectType == 1) // If the object is a large asteroid, set a larger bounding box
	{
		SetaabbMax(GetPosition() + glm::vec3(5.0f, 5.0f, 5.0f));
		SetaabbMin(GetPosition() + glm::vec3(-5.0f, -5.0f, -5.0f));
	}

	UpdateModelMatrix(_deltaTime);
}

void Obstacle::SetAngularVelocity()
{
	std::random_device generator;
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f); // Used to create a random float between -1.0 and 1.0
	
	// Makes a random float between -4.0 and 4.0 and sets it to angular velocity
	float rand = distribution(generator) * 4.0f;
	m_angularVelocity.x = rand;
	rand = distribution(generator) * 4.0f;
	m_angularVelocity.y = rand;
	rand = distribution(generator) * 4.0f;
	m_angularVelocity.z = rand;

}

void Obstacle::UpdateModelMatrix(float _deltaTime)
{
	// Makes the angular velocity into a quaternion
	glm::quat angVelQuat = glm::quat(0.0f, m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z);

	m_rotationQuaternion += 0.5f*m_rotationQuaternion*angVelQuat*_deltaTime; // Equation to compute the quaternion
	m_rotationQuaternion = glm::normalize(m_rotationQuaternion);

	glm::mat4 model_rotation = glm::mat4_cast(m_rotationQuaternion); // Converts the quaterniom into a matrix

	m_model = glm::translate(glm::mat4(1.0f), GetPosition()); // applies translation to the model
	m_model = m_model * model_rotation; // Applies rotation
	m_model = glm::scale(m_model, GetScale()); // Applies scale
}