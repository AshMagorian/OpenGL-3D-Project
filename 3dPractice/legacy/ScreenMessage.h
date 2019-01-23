#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class ScreenMessage
{
public:
	ScreenMessage();
	~ScreenMessage();

	void Draw(RenderTexture* _rt);


private:

	Texture* m_texture;
	VertexArray* m_va;
	ShaderProgram* m_shader;

};