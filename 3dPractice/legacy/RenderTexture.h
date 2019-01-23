#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "Texture.h"

class RenderTexture : public Texture
{
	GLuint fbo;

public:
	RenderTexture();
	RenderTexture(int width, int height);

	GLuint getFbId();
	void clear();

};
#endif