#pragma once
#include "GameObject.h"
// for convenience
#define R m_ColorMod.r
#define G m_ColorMod.g
#define B m_ColorMod.b
#define A m_ColorMod.a

class Tile : public GameObject
{
public:
	Tile(int px, int py, int l) 
	{
		x = px;
		y = py;
		w = l;
		h = l;
		Tile::setTexture(Renderer::loadTexture("assets/art/tileblank.png"));
		SDLVERIFY(m_TileTexture);
	}

	void Render() override
	{
		// base rendering + color modulation
		auto rect = TORECT;
		TRANSCAM(rect);
		SDLCall(SDL_SetTextureColorMod(m_TileTexture, R, G, B));
		SDLCall(SDL_SetTextureAlphaMod(m_TileTexture, A));
		SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_TileTexture, NULL, &rect));
		SDLCall(SDL_SetTextureColorMod(m_TileTexture, 255, 255, 255));
		SDLCall(SDL_SetTextureAlphaMod(m_TileTexture, 255));
	}


	int distanceTo(int px, int py)
	{
		// normalize vectors
		// ax/y : absolute x/y
		int ax = x - px, ay = y - py;
		ax = abs(ax);
		ay = abs(ay);
		// pythagorean theorem
		ax *= ax;
		ay *= ay;
		// a^2 + b^2 = c^2
		return sqrt(ax + ay);
	}

	void setColor(const SDL_Color& color) 
	{
		m_ColorMod.r = color.r;
		m_ColorMod.g = color.g;
		m_ColorMod.b = color.b;
		m_ColorMod.a = color.a;
	}

	void onClick(int px, int py)
	{
		SDL_Point point{ px, py };
		SDL_Rect rect = TORECT;
		rect.w -= m_MultLength * 4;
		rect.h -= m_MultLength * 4;
		rect.x += m_MultLength * 4;
		rect.y += m_MultLength * 4;
		if (SDL_PointInRect(&point, &rect))
			setColor({255, 0, 0, 255});
	}

	static void setMultLength(uint16_t p_MultLength)
	{
		m_MultLength = p_MultLength;
	}

	static void setTexture(SDL_Texture* p_Texture)
	{
		m_TileTexture = p_Texture;
	}

	static void setRadius(uint16_t p_Radius) { m_Radius = p_Radius; }
	static uint16_t getRadius() { return m_Radius; }
	const SDL_Color& getColor() { return m_ColorMod; }

private:
	SDL_Color m_ColorMod{255, 255, 255, 255};

	static SDL_Texture* m_TileTexture;
	static uint16_t m_MultLength;

	static uint16_t m_Radius; // pixel radius of each tile

};