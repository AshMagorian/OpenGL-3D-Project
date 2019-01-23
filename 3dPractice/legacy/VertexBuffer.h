#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class VertexBuffer
{
private:
	GLuint id;
	int components;
	std::vector<GLfloat> data;
	bool dirty;

public:
	VertexBuffer();

	void add(glm::vec2 value);
	void add(glm::vec3 value);
	void add(glm::vec4 value);

	int GetComponents();
	int GetDataSize();
	GLuint GetId();
};
