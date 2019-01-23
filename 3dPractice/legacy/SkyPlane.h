#include "Entity.h"

class SkyPlane : public Entity
{
public:
	SkyPlane(std::string _texPath, std::string _vaPath, ShaderProgram* _shader, glm::vec3 _angle, glm::vec3 _pos, glm::vec3 _scale, int _positionNo);

	~SkyPlane();

	void Update(Entity* _entity);
private:
	int m_positionNo;
};