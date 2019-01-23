#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>

class VertexArray;
class Texture;
class RenderTexture;

struct Sampler
{
	GLint id;
	Texture *texture;
};

class ShaderProgram
{
private:
	GLuint id;
	std::vector<Sampler> samplers;
	VertexArray *simpleShape;
	glm::vec4 viewport;

public:
	ShaderProgram(std::string vert, std::string frag);

	void draw();
	void draw(RenderTexture *renderTexture);
	void draw(VertexArray *vertexArray);
	void draw(RenderTexture *renderTexture, VertexArray *vertexArray);
	void SetUniform(std::string uniform, glm::vec3 value);
	void SetUniform(std::string uniform, glm::vec4 value);
	void SetUniform(std::string uniform, float value);
	void SetUniform(std::string uniform, int value);
	void SetUniform(std::string uniform, glm::mat4 value);
	void SetUniform(std::string uniform, Texture *texture);

	GLuint getId();
	void SetViewport(glm::vec4 viewport);
};
#endif