#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>


class Game {
public:
	Game();

	~Game() {
		SDL_DestroyWindow(m_Window);
		SDL_DestroyRenderer(m_Renderer);
		SDL_Quit();
	}

	void handleEvents(void* p_World);
	void renderAll();
	void renderClear();

	bool is_running() { return running; }
	void getWindowSize(int* dstX, int* dstY);

private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Event m_Event{};

	uint16_t m_windowWidth, m_windowHeight;

	bool running;

};