#include "ShaderProgram.h"
#include "RenderTexture.h"

class Bloom
{
public:

	Bloom(int _windowWidth, int _windowHeight);
	~Bloom();

	void BloomDraw(RenderTexture* _rt, int _windowWidth, int _windowHeight);

private:

	RenderTexture *m_lightkeyRt;
	RenderTexture *m_blurRt;
	RenderTexture *m_blur2Rt;
	RenderTexture *m_mergeRt;

	ShaderProgram *m_nullShaderProgram;
	ShaderProgram *m_lightKeyShaderProgram;
	ShaderProgram *m_blurShaderProgram;
	ShaderProgram *m_mergeShaderProgram;
};