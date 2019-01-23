#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

class RenderTexture;

class Texture
{
public:
	Texture() {}
	Texture(std::string path);
	glm::vec2 getSize();
	GLuint getId();

private:
	friend class RenderTexture;

	GLuint id;
	glm::vec2 size;

	Texture(int width, int height);
};
#endif