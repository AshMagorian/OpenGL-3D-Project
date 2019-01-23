#include "Entity.h"
#include <iostream>

Entity::Entity(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale)
{
	m_texture = new Texture(_texPath);
	m_va = new VertexArray(_vaPath);
	m_shader = _shader;
	m_rotationAngle = _angle;
	m_position = _pos;
	m_scale = _scale;

	m_model = glm::mat4(1.0f);

	m_model = glm::rotate(m_model, glm::radians(m_rotationAngle.x), glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotationAngle.y), glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotationAngle.z), glm::vec3(0, 0, 1));
	m_model = glm::translate(m_model, m_position);

	m_aabbMax = glm::vec3(10.0f, 10.0f, 10.0f);
	m_aabbMin = glm::vec3(-10.0f, -10.0f, -10.0f);
}

Entity::~Entity()
{

}

void Entity::Update()
{

}


void Entity::UpdateModelMatrix()
{
	m_model = glm::mat4(1.0f);
	
	m_model = glm::translate(m_model, m_position);
	m_model = glm::rotate(m_model, glm::radians(m_rotationAngle.x), glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotationAngle.y), glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, glm::radians(m_rotationAngle.z), glm::vec3(0, 0, 1));
	m_model = glm::scale(m_model, m_scale);
}

void Entity::SetEmissive(glm::vec3 _emissive)
{
	m_shader->SetUniform("in_Emissive", _emissive);
}


void Entity::Draw(RenderTexture* _rt)
{
	m_shader->SetUniform("in_Model", m_model);
	m_shader->SetUniform("in_Texture", m_texture);
	m_shader->draw(_rt, m_va);
}