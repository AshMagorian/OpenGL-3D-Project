#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	components = 0;
	dirty = false;

	glGenBuffers(1, &id);

	if (!id)
	{
		throw std::exception();
	}
}

void VertexBuffer::add(glm::vec2 value)
{
	if (components != 2 && components != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	components = 2;
	dirty = true;
}

void VertexBuffer::add(glm::vec3 value)
{
	if (components != 3 && components != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	components = 3;
	dirty = true;
}

void VertexBuffer::add(glm::vec4 value)
{
	if (components != 4 && components != 0)
	{
		throw std::exception();
	}

	data.push_back(value.x);
	data.push_back(value.y);
	data.push_back(value.z);
	data.push_back(value.w);
	components = 4;
	dirty = true;
}

int VertexBuffer::GetComponents()
{
	return components;
}

int VertexBuffer::GetDataSize()
{
	return data.size();
}

GLuint VertexBuffer::GetId()
{
	if (dirty)
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		dirty = false;
	}

	return id;
}