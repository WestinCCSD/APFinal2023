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

    // note: does not actually change dimensions of the window.
    static void setWindowProperties(int w, int h);
    // returns an array of two elements
    static void getWindowProperties(int*, int*);

private:

    static Renderer* m_Renderer;
    static SDL_Renderer* m_GlobalRenderer;

    int m_WindowWidth=0, m_WindowHeight=0;

    // Default constructor
    Renderer() {

    }


};