#include "Ship.h"

Ship::Ship(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale)
	: Entity(_texPath, _vaPath, _shader, _angle, _pos, _scale)
{

}

Ship::~Ship()
{

}

void Ship::Update(Input* _input, float _deltaTime)
{
	if (_input->GetWPress() == true) // Moving upwards
	{
		m_velocity.y += 0.05f;

	}
	else 
	{
		if (m_velocity.y > 0.01f)
		{
			m_velocity.y -= 0.05f;
		}
	}

	if (_input->GetSPress() == true) // Moving down
	{
		m_velocity.y -= 0.05f;

	}
	else 
	{
		if (m_velocity.y < -0.01f)
		{
			m_velocity.y += 0.05f;
		}
	}

	if (_input->GetAPress() == true) // Moving left
	{
		m_velocity.x -= 0.05f;
	}
	else 
	{
		if (m_velocity.x < -0.01f)
		{
			m_velocity.x += 0.05f;
		}
	}

	if (_input->GetDPress() == true) // Moving right
	{
		m_velocity.x += 0.05f;
	}
	else
	{
		if (m_velocity.x > 0.01f)
		{
			m_velocity.x -= 0.05f;
		}
	}

	if (m_velocity.y > 1.0f) { m_velocity.y = 1.0f; }
	if (m_velocity.y < -1.0f) { m_velocity.y = -1.0f; }
	if (m_velocity.x > 1.0f) { m_velocity.x = 1.0f; }
	if (m_velocity.x < -1.0f) { m_velocity.x = -1.0f; } // Caps velocity

	SetPosition(GetPosition() + m_velocity); // Updates the position

	m_rotationAngle.x = m_velocity.y * 50.0f; // Rotation is dependant on the velocity in each direction
	m_rotationAngle.z = m_velocity.x * 50.0f;
	
	SetaabbMax(GetPosition() + glm::vec3(5.0f, 1.5f, 5.0f));
	SetaabbMin(GetPosition() + glm::vec3(-5.0f, -2.5f, -5.0f)); // Updates the bonding box positions

	UpdateModelMatrix();
}

