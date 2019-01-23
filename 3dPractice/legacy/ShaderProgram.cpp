#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "RenderTexture.h"

#include <glm/ext.hpp>
#include <fstream>
#include <iostream>
#include <vector>


ShaderProgram::ShaderProgram(std::string vert, std::string frag)
{
	std::string vertShader;
	std::string fragShader;

	std::ifstream file(vert);
	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			vertShader += line + "\n";
		}
	}
	file.close();

	file.open(frag);

	if (!file.is_open())
	{
		throw std::exception();
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			fragShader += line + "\n";
		}
	}
	file.close();

	const char *vertex = vertShader.c_str();
	const char *fragment = fragShader.c_str();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorlog[0]);
		std::cout << &errorlog.at(0) << std::endl;
		throw std::exception();
	}

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	glCompileShader(fragmentShaderId);
	success = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorlog[0]);
		std::cout << &errorlog.at(0) << std::endl;
		throw std::exception();
	}

	id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);

	glBindAttribLocation(id, 0, "in_Position");
	glBindAttribLocation(id, 1, "in_Color");
	glBindAttribLocation(id, 2, "in_TexCoord");
	glBindAttribLocation(id, 3, "in_Normal");
	// Perform the link and check for faliure
	glLinkProgram(id);
	success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw std::exception();
	}

	glDetachShader(id, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(id, fragmentShaderId);
	glDeleteShader(fragmentShaderId);


	VertexBuffer *positions = new VertexBuffer();
	positions->add(glm::vec2(-1.0f, 1.0f));
	positions->add(glm::vec2(-1.0f, -1.0f));
	positions->add(glm::vec2(1.0f, -1.0f));
	positions->add(glm::vec2(1.0f, -1.0f));
	positions->add(glm::vec2(1.0f, 1.0f));
	positions->add(glm::vec2(-1.0f, 1.0f));

	VertexBuffer *texCoords = new VertexBuffer();
	texCoords->add(glm::vec2(0.0f, 0.0f));
	texCoords->add(glm::vec2(0.0f, -1.0f));
	texCoords->add(glm::vec2(1.0f, -1.0f));
	texCoords->add(glm::vec2(1.0f, -1.0f));
	texCoords->add(glm::vec2(1.0f, 0.0f));
	texCoords->add(glm::vec2(0.0f, 0.0f));

	simpleShape = new VertexArray();
	simpleShape->SetBuffer("in_Position", positions);
	simpleShape->SetBuffer("in_TexCoord", texCoords);
}

void ShaderProgram::draw()
{
	draw(simpleShape);
}

void ShaderProgram::draw(VertexArray *vertexArray)
{
	// Instruct OpenGL to use our shader program and our VAO
	glUseProgram(id);
	glBindVertexArray(vertexArray->GetId());

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		if (samplers.at(i).texture)
		{
			glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	glDrawArrays(GL_TRIANGLES, 0, vertexArray->GetVertexCount());

	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Reset the state
	glBindVertexArray(0);
	glUseProgram(0);
}

void ShaderProgram::draw(RenderTexture *renderTexture, VertexArray *vertexArray)
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->getFbId());
	glm::vec4 lastViewport = viewport;
	viewport = glm::vec4(0, 0, renderTexture->getSize().x, renderTexture->getSize().y);
	draw(vertexArray);
	viewport = lastViewport;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShaderProgram::draw(RenderTexture *renderTexture)
{
	draw(renderTexture, simpleShape);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec3 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform3f(uniformId, value.x, value.y, value.z);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform4f(uniformId, value.x, value.y, value.z, value.w);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, float value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1f(uniformId, value);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, int value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform1i(uniformId, value);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::mat4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, Texture *texture)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	for (size_t i = 0; i < samplers.size(); i++)
	{
		if (samplers.at(i).id == uniformId)
		{
			samplers.at(i).texture = texture;

			glUseProgram(id);
			glUniform1i(uniformId, i);
			glUseProgram(0);
			return;
		}
	}
	Sampler s;
	s.id = uniformId;
	s.texture = texture;
	samplers.push_back(s);

	glUseProgram(id);
	glUniform1i(uniformId, samplers.size() - 1);
	glUseProgram(0);
}

void ShaderProgram::SetViewport(glm::vec4 viewport)
{
	this->viewport = viewport;
}

GLuint ShaderProgram::getId()
{
	return id;
}