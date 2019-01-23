#include "bloom.h"

Bloom::Bloom(int _windowWidth, int _windowHeight)
{
	m_lightkeyRt = new RenderTexture(_windowWidth, _windowHeight);
	m_blurRt = new RenderTexture(_windowWidth, _windowHeight);
	m_blur2Rt = new RenderTexture(_windowWidth, _windowHeight);
	m_mergeRt = new RenderTexture(_windowWidth, _windowHeight);

	m_nullShaderProgram = new ShaderProgram("../shaders/null.vert", "../shaders/null.frag");
	m_lightKeyShaderProgram = new ShaderProgram("../shaders/lightkeypass.vert", "../shaders/lightkeypass.frag");
	m_blurShaderProgram = new ShaderProgram("../shaders/blur.vert", "../shaders/blur.frag");
	m_mergeShaderProgram = new ShaderProgram("../shaders/mergepass.vert", "../shaders/mergepass.frag");
}

Bloom::~Bloom()
{
	delete m_lightkeyRt;
	delete m_blur2Rt;
	delete m_blurRt;
	delete m_mergeRt;

	delete m_nullShaderProgram;
	delete m_lightKeyShaderProgram;
	delete m_blurShaderProgram;
	delete m_mergeShaderProgram;
}

void Bloom::BloomDraw(RenderTexture* _rt, int _windowWidth, int _windowHeight)
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_lightKeyShaderProgram->SetUniform("in_Texture", _rt);
	m_lightKeyShaderProgram->draw(m_lightkeyRt); // Draws a black and white image highlighting the light areas

	m_blurShaderProgram->SetUniform("in_Texture", m_lightkeyRt);
	m_blurShaderProgram->draw(m_blurRt); // Blurs the black and white image

	for (int i = 0; i < 4; i++) // Loops through blurring for a smoother finish
	{
		m_blurShaderProgram->SetUniform("in_Texture", m_blurRt);
		m_blurShaderProgram->draw(m_blur2Rt);

		m_blurShaderProgram->SetUniform("in_Texture", m_blur2Rt);
		m_blurShaderProgram->draw(m_blurRt);
	}

	m_mergeShaderProgram->SetUniform("in_TextureA", _rt);
	m_mergeShaderProgram->SetUniform("in_TextureB", m_blurRt);
	m_mergeShaderProgram->draw(m_mergeRt); // Merges the blurred texture with the original render texture

	m_nullShaderProgram->SetViewport(glm::vec4(0, 0, _windowWidth, _windowHeight));
	m_nullShaderProgram->SetUniform("in_Texture", m_mergeRt);
	m_nullShaderProgram->draw(); // Draws onto the screen

	_rt = m_mergeRt;
}