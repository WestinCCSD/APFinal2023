#pragma once
#include "Debug.h"
#include "Renderer.h"
#include "Camera.h"

// for convenience
#define TORECT SDL_Rect{x, y, w, h}
// quickly transforms an object by the camera's position
#define TRANSCAM(r) r.x -= Camera::getX();\
					   rect.y -= Camera::getY()
// base class for everything that renders and has logic
class GameObject {
public:
	virtual void Render() {
		auto rect = TORECT;
		TRANSCAM(rect);
		SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_Texture, NULL, &rect));
	}

	virtual void Handle(float p_Delta) {}

protected:

	int x, y, w, h;
	SDL_Texture* m_Texture;

};