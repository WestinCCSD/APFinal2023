#include "Game.h"
#include "Renderer.h"
#include "Debug.h"
#include "Camera.h"
#include "World.h"
#include "UI.h"

Game::Game() 
{
	SDLCall(SDL_Init(SDL_INIT_EVERYTHING));
	EXCPTCall(IMG_Init(IMG_INIT_PNG), IMG_INIT_PNG);
	SDLCall(TTF_Init());
	EXCPTCall(Mix_Init(MIX_INIT_MP3), MIX_INIT_MP3);
	SDLCall(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096));

	{
		SDL_DisplayMode displaymode;
		SDLCall(SDL_GetDesktopDisplayMode(0, &displaymode));
		m_windowWidth = displaymode.w;
		m_windowHeight = displaymode.h;
		std::cout << "Adapting window for display with resolution: " << m_windowWidth << "x" << m_windowHeight << " @" << displaymode.refresh_rate << "hz \n";
	}

	m_Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight, SDL_WINDOW_BORDERLESS);
	SDLVERIFY(m_Window);
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDLVERIFY(m_Renderer);
	SDL_SetRenderDrawColor(m_Renderer, 25, 95, 240, 255);

	

	Renderer::setRenderer(m_Renderer);

	running = true;
}

// basic event handler. Might add input support later or smth idk
void Game::handleEvents(void* p_World)
{
	World* world = static_cast<World*>(p_World);

	while (SDL_PollEvent(&m_Event))
	{
		if (m_Event.type == SDL_QUIT) 
		{
			running = false;
		}
		if (m_Event.type == SDL_MOUSEBUTTONDOWN)
		{
			int x=0, y=0;
			SDL_GetMouseState(&x, &y);
			// do click checks or smth idk
			world->onClick(x, y);
		}
		if (m_Event.type == SDL_KEYDOWN)
		{
			auto key = m_Event.key.keysym.sym;
			if (key == SDLK_SPACE)
			{
				world->Pause();
			}
		}
	}

	for (auto& object : m_Objects)
	{
		object->Handle(0.f);
	}

	auto key = SDL_GetKeyboardState(NULL);

	if (key[SDL_SCANCODE_W]) 
	{
		Camera::Move(0, -3);
	}
	if (key[SDL_SCANCODE_S])
	{
		Camera::Move(0, 3);
	}
	if (key[SDL_SCANCODE_A])
	{
		Camera::Move(-3, 0);
	}
	if (key[SDL_SCANCODE_D])
	{
		Camera::Move(3, 0);
	}

}

void Game::renderClear() {
	SDLCall(SDL_RenderClear(m_Renderer));
}

void Game::getWindowSize(int* dstX, int* dstY)
{
	if (dstX != NULL)
		(*dstX) = m_windowWidth;
	if (dstY != NULL)
		(*dstY) = m_windowHeight;
}

void Game::renderAll()
{
	// draw stuff
	for (auto& object : m_Objects)
	{
		object->Render();
	}
}

void Game::renderPresent()
{
	SDL_RenderPresent(m_Renderer);
}

void Game::linkObject(GameObject* p_Obj)
{
	m_Objects.push_back(p_Obj);
}

void Game::unlinkObject(GameObject* p_Obj)
{
	
}