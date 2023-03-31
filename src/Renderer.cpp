#include "../include/Renderer.h"
#include "../include/Debug.h"

Renderer* Renderer::m_Renderer = NULL;
SDL_Renderer* Renderer::m_GlobalRenderer = NULL;

void Renderer::setRenderer(SDL_Renderer* p_Renderer)
{
	m_GlobalRenderer = p_Renderer;
}

Renderer* const Renderer::get()
{
	if (m_Renderer == NULL)
	{
		m_Renderer = new Renderer();
	}
	
	return m_Renderer;

}

SDL_Texture* Renderer::loadTexture(const char* p_Path)
{
	// SDLVERIFY should be used
	return IMG_LoadTexture(m_GlobalRenderer, p_Path);
}

