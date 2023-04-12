#pragma once
#include <iostream>
#include <deque>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


#include "GameObject.h"

class Game {
public:
	Game();

	~Game() {
		SDL_DestroyWindow(m_Window);
		SDL_DestroyRenderer(m_Renderer);
		IMG_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	void handleEvents(void* p_World);
	void renderPresent();
	void renderAll();
	void renderClear();

	bool isRunning() { return running; }
	void getWindowSize(int* dstX, int* dstY);

	void linkObject(GameObject*);
	void unlinkObject(GameObject*);

private:
	std::deque<GameObject*> m_Objects;

	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Event m_Event{};

	uint16_t m_windowWidth, m_windowHeight;

	bool running;

};