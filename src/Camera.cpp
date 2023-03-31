#include "Camera.h"

Camera* Camera::m_GlobalCamera = 0;
int Camera::x = 0;
int Camera::y = 0;
int Camera::m_minX = 0;
int Camera::m_maxX = 0;
int Camera::m_minY = 0;
int Camera::m_maxY = 0;

Camera* const Camera::Get() {
	if (m_GlobalCamera == 0) {
		m_GlobalCamera = new Camera;
	}
	return m_GlobalCamera;
}

void Camera::Move(int px, int py)
{
	x += px;
	y += py;
	//clampCamera();
}

void Camera::clampCamera() 
{
	x = clamp(x, m_minX, m_maxX);
	y = clamp(y, m_minY, m_maxY);
}

void Camera::getPosition(int* a, int* b) 
{
	(*a) = x;
	(*b) = y;
}

void Camera::setPositon(int a, int b)
{
	x = a;
	y = b;
}

void Camera::setRange(const SDL_Rect& rect)
{
	m_minX = rect.x;
	m_maxX = rect.w;
	m_minY = rect.y;
	m_maxY = rect.h;
}
