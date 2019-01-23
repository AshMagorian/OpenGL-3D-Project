#include "SkyPlane.h"

SkyPlane::SkyPlane(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale, int _positionNo)
	: Entity(_texPath, _vaPath, _shader,  _angle,  _pos, _scale)
{
	m_positionNo = _positionNo;
}
SkyPlane::~SkyPlane()
{

}

void SkyPlane::Update(Entity* _entity)
{
	if (m_positionNo == 0) // Front
	{
		SetPosition(_entity->GetPosition() + glm::vec3(0.0f, 0.0f, -150.0f));
	}
	if (m_positionNo == 1) // Left
	{
		SetPosition(_entity->GetPosition() + glm::vec3(-80.0f, 0.0f, -69.8f));
	}
	if (m_positionNo == 2) // Right
	{
		SetPosition(_entity->GetPosition() + glm::vec3(80.0f, 0.0f, -69.8f));
	}
	if (m_positionNo == 3) // Up
	{
		SetPosition(_entity->GetPosition() + glm::vec3(0.0f, 80.0f, -69.8f));
	}
	if (m_positionNo == 4) // Down
	{
		SetPosition(_entity->GetPosition() + glm::vec3(0.0f, -80.0f, -69.8f));
	}
	
	UpdateModelMatrix();
}