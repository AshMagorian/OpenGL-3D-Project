#include "Entity.h"

#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h" 
#include "input.h"

class Ship : public Entity
{
public:
	Ship(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale);
	~Ship();

	void Update(Input* _input, float _deltaTime);

private:

	glm::vec3 m_velocity;

};