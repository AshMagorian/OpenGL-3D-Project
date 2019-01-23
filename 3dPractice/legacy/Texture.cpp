#include "Texture.h"

#include "stb_image.h"


Texture::Texture(int width, int height)
{
	size.x = width;
	size.y = height;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(std::string path)
{
	int w = 0;
	int h = 0;
	int channels = 0;
	unsigned char *data = stbi_load(path.c_str(), &w, &h, &channels, 4);
	if (!data)
	{
		throw std::exception();
	}

	size.x = w;
	size.y = h;

	// Create and bind a texture.
	//GLuint textureId = 0;
	glGenTextures(1, &id);
	if (!id)
	{
		throw std::exception();
	}
	glBindTexture(GL_TEXTURE_2D, id);
	// Upload the image data to the bound texture unit in the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);
	// Free the loaded data because we now have a copy on the GPU
	free(data);
	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);
	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::vec2 Texture::getSize()
{
	return size;
}

GLuint Texture::getId()
{
	return id;
}
