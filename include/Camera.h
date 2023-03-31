#pragma once
#include <SDL_rect.h>

class Camera {
public:
	Camera(const Camera&) = delete;

	static Camera* const Get();

	static void Move(int px, int py);
	static void setPositon(int px, int py);
	static void setRange(const SDL_Rect&);

	static void getPosition(int*, int*);
	static const int& getX() { return x; }
	static const int& getY() { return y; }

private:
	Camera()
	{

	}
	static Camera* m_GlobalCamera;

	static int x, y;
	static int m_minX, m_maxX;
	static int m_minY, m_maxY;

	static void clampCamera();

	static bool clamp(int value, int min, int max) 
	{
		if (value < min)
			value = min;
		if (value > max)
			value = max;
		return value;
	}

};