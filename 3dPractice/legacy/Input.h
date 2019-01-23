#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

class Input
{
public:
	Input();
	~Input();

	void Update(bool* _quit);

	int GetDeltaMouseX();
	int GetDeltaMouseY();

	bool GetUpPress() { return m_upPress; }
	bool GetDownPress() { return m_downPress; }
	bool GetLeftPress() { return m_leftPress; }
	bool GetRightPress() { return m_rightPress; }

	bool GetWPress() { return m_wPress; }
	bool GetAPress() { return m_aPress; }
	bool GetSPress() { return m_sPress; }
	bool GetDPress() { return m_dPress; }

private:
	SDL_Event event;

	int m_deltaMouseX;
	int m_deltaMouseY;
	int m_oldMouseX;
	int m_oldMouseY;
	int m_mouseX;
	int m_mouseY;

	bool m_upPress;
	bool m_downPress;
	bool m_leftPress;
	bool m_rightPress;

	bool m_wPress;
	bool m_aPress;
	bool m_sPress;
	bool m_dPress;
};
#endif