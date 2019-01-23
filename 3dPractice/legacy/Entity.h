#ifndef ENTITY_H
#define ENTITY_H

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Input.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>


class Entity
{
public:
	Entity(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale);
	~Entity();

	virtual void Update();
	virtual void Update(float _deltaTime) {}
	virtual void Update(Entity* _entity) {}
	virtual void Update(Input* _input, float _deltaTime) {}

	virtual void SetAngularVelocity() {}

	void Draw(RenderTexture* _rt);

	void UpdateModelMatrix();

	glm::vec3 GetPosition() { return m_position; }
	void SetPosition(glm::vec3 _pos) { m_position = _pos; }

	glm::vec3 GetAngle() { return m_rotationAngle; }

	glm::vec3 GetScale() { return m_scale; }

	glm::vec3 GetaabbMax() { return m_aabbMax; }
	void SetaabbMax(glm::vec3 _value) { m_aabbMax = _value; }

	glm::vec3 GetaabbMin() { return m_aabbMin; }
	void SetaabbMin(glm::vec3 _value) { m_aabbMin = _value; }

	void SetCollision(bool _collision) { m_collision = _collision; }

	void SetEmissive(glm::vec3 _emissive);

private:
	Texture* m_texture;
	VertexArray* m_va;
	ShaderProgram* m_shader;

	bool m_collision;

	glm::vec3 m_position;
	glm::vec3 m_scale;

	glm::vec3 m_aabbMax;
	glm::vec3 m_aabbMin;

protected:

	glm::vec3 m_rotationAngle;

	glm::mat4 m_model;
};
#endif