#include "MotionBlur.h"

MotionBlur::MotionBlur(int _windowWidth, int _windowHeight)
{
	
	m_mergeRt = new RenderTexture(_windowWidth, _windowHeight);

	m_motionBlurShaderProgram = new ShaderProgram("../shaders/motionblur.vert", "../shaders/motionblur.frag");
	m_mergeShaderProgram = new ShaderProgram("../shaders/mergepass.vert", "../shaders/mergepass.frag");
	m_nullShaderProgram = new ShaderProgram("../shaders/null.vert", "../shaders/null.frag");
}

MotionBlur::~MotionBlur()
{
	delete m_mergeRt;
	delete m_mergeShaderProgram;
	delete m_motionBlurShaderProgram;
}

void MotionBlur::MotionBlurDraw(RenderTexture* _rt, RenderTexture* _previousRt, RenderTexture* _previousRt2, int _windowWidth, int _windowHeight)
{

	m_motionBlurRt = *_previousRt;
	m_motionBlurRt2 = *_previousRt2;

	m_motionBlurShaderProgram->SetUniform("in_Texture", _rt);
	m_motionBlurShaderProgram->draw(&m_motionBlurRt);

	m_mergeShaderProgram->SetUniform("in_TextureA", _rt);
	m_mergeShaderProgram->SetUniform("in_TextureB", &m_motionBlurRt);

	m_mergeShaderProgram->draw(m_mergeRt);

	m_mergeShaderProgram->SetUniform("in_TextureA", m_mergeRt);
	m_mergeShaderProgram->SetUniform("in_TextureB", &m_motionBlurRt2);

	m_mergeShaderProgram->draw(m_mergeRt);

	m_nullShaderProgram->SetViewport(glm::vec4(0, 0, _windowWidth, _windowHeight));
	m_nullShaderProgram->SetUniform("in_Texture", m_mergeRt);
	m_nullShaderProgram->draw(); // Draws onto the screen

}