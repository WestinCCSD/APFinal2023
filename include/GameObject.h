#pragma once
#include "Debug.h"
#include "Renderer.h"
#include "Camera.h"

// for convenience
#define TORECT SDL_Rect{x, y, w, h}
// quickly transforms an object by the camera's position
#define TRANSCAM(r) r.x -= Camera::getX();\
					   r.y -= Camera::getY()
// base class for everything that renders and has logic
class GameObject {
public:
	virtual void Render() {
		auto rect = TORECT;
		TRANSCAM(rect);
		SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_Texture, NULL, &rect));
	}

	virtual void Handle(float p_Delta) {}

	void setIndex(uint8_t p_Index)
	{
		m_Index = p_Index;
	}

	uint8_t getIndex() { return m_Index; }

	template<typename T>
	T clamp(T min, T max, T value)
	{
		if (value < min)
			value = min;
		if (value > max)
			value = max;
		return value;
	}

protected:

	int x, y, w, h;
	SDL_Texture* m_Texture;
	uint8_t m_Index;
};