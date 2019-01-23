#include "ShaderProgram.h"
#include "RenderTexture.h"

class MotionBlur
{
	public:
		MotionBlur(int _windowWidth, int _windowHeight);
		~MotionBlur();

		void MotionBlurDraw(RenderTexture* _rt, RenderTexture* _previousRt, RenderTexture* _previousRt2, int _windowWidth, int _windowHeight);
		 
private:

	RenderTexture m_motionBlurRt;
	RenderTexture *m_mergeRt;
	RenderTexture m_motionBlurRt2;

	ShaderProgram *m_motionBlurShaderProgram;
	ShaderProgram *m_mergeShaderProgram;
	ShaderProgram* m_nullShaderProgram;
};