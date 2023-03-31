#pragma once
#include <SDL_image.h>

// Renderer singleton for use with loading in textures
class Renderer {
public:

    // deleting copy constructor
    Renderer(const Renderer& obj) = delete;

    static void setRenderer(SDL_Renderer*);
   
    static SDL_Renderer* getRenderer() { return m_GlobalRenderer; }
    static Renderer* const get();    
    static SDL_Texture* loadTexture(const char* p_Path);

private:

    static Renderer* m_Renderer;
    static SDL_Renderer* m_GlobalRenderer;

    // Default constructor
    Renderer() {

    }


};