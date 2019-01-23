#include "Input.h"

#include <iostream>

Input::Input()
{
	m_upPress = false;
	m_downPress = false;
	m_leftPress = false;
	m_rightPress = false;
	m_deltaMouseX = 0;
	m_deltaMouseY = 0;
	m_oldMouseX = 0;
	m_oldMouseY = 0;
	m_mouseX = 320;
	m_mouseY = 240;

	event = { 0 };
}

Input::~Input()
{

}

void Input::Update(bool* _quit)
{
	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
	m_deltaMouseX = 0;
	m_deltaMouseY = 0;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			*_quit = true;
			break;
		case SDL_KEYDOWN:														// If Key pressed down
			if (event.key.keysym.sym == SDLK_LEFT) { m_leftPress = true; }
			if (event.key.keysym.sym == SDLK_RIGHT) { m_rightPress = true; }
			if (event.key.keysym.sym == SDLK_UP) { m_upPress = true; }
			if (event.key.keysym.sym == SDLK_DOWN) { m_downPress = true; }
			if (event.key.keysym.sym == SDLK_w) { m_wPress = true; }
			if (event.key.keysym.sym == SDLK_a) { m_aPress = true; }
			if (event.key.keysym.sym == SDLK_s) { m_sPress = true; }
			if (event.key.keysym.sym == SDLK_d) { m_dPress = true; }
			if (event.key.keysym.sym == SDLK_ESCAPE) { *_quit = true; }
			break;
		case SDL_KEYUP:															// If key is released
			if (event.key.keysym.sym == SDLK_LEFT) { m_leftPress = false; }
			if (event.key.keysym.sym == SDLK_RIGHT) { m_rightPress = false; }
			if (event.key.keysym.sym == SDLK_UP) { m_upPress = false; }
			if (event.key.keysym.sym == SDLK_DOWN) { m_downPress = false; }
			if (event.key.keysym.sym == SDLK_w) { m_wPress = false; }
			if (event.key.keysym.sym == SDLK_a) { m_aPress = false; }
			if (event.key.keysym.sym == SDLK_s) { m_sPress = false; }
			if (event.key.keysym.sym == SDLK_d) { m_dPress = false; }
			break;
		default:
			break;

		}
		if (event.type == SDL_MOUSEMOTION) // Reads the mouse
		{
			m_mouseX = event.motion.x;
			m_mouseY = event.motion.y;
			m_deltaMouseX = 320 - m_mouseX;  // deltaX = centre of screen - mouseX
			m_deltaMouseY = 240 - m_mouseY;	 // deltaY = centre of screen - mouseY
		}
	}
}

int Input::GetDeltaMouseX()
{
	return m_deltaMouseX;
}

int Input::GetDeltaMouseY()
{
	return m_deltaMouseY;
}