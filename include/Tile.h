#pragma once
#include "GameObject.h"
#include "Country.h"
#include "RNG.h"

// for convenience
#define R m_ColorMod.r
#define G m_ColorMod.g
#define B m_ColorMod.b
#define A m_ColorMod.a

namespace TerrainTextures {
	static SDL_Texture* terrainTextures[] =
	{
		NULL,
		NULL,
		NULL
	};
}

enum TerrainType
{
	Flat,
	Forest,
	Hills,
	Mountains,
};

class Tile : public GameObject
{
public:
	Tile(int px, int py, int l, int id)
		: m_ID(id)
	{
		auto number = rng.randRange(0, 100);
		if (number >= 95)
		{
			m_TerrainType = TerrainType::Mountains;
		}
		else if (number >= 90)
		{
			m_TerrainType = TerrainType::Hills;
		}
		else if (number >= 70)
		{
			m_TerrainType = TerrainType::Forest;
		}

		x = px;
		y = py;
		w = l;
		h = l;
		Tile::setTexture(Renderer::loadTexture("assets/art/tileblank.png"));
		SDLVERIFY(m_TileTexture);

		const auto& country = Countries::getCountry(m_Country);
		setColor(country.getBorderColor());

	}

	void Render() override
	{
		// base rendering + color modulation
		auto rect = TORECT;
		TRANSCAM(rect); // transforms rect to account for camera movement

		SDLCall(SDL_SetTextureColorMod(m_TileTexture, R, G, B));
		SDLCall(SDL_SetTextureAlphaMod(m_TileTexture, A));
		
		SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_TileTexture, NULL, &rect));
		if (m_TerrainType != 0)
			SDL_RenderCopy(Renderer::getRenderer(), TerrainTextures::terrainTextures[m_TerrainType - 1], NULL, &rect); // terrain type is subtracted by 1 because terrain texture 1 = terrain type 0 :)

		SDLCall(SDL_SetTextureColorMod(m_TileTexture, 255, 255, 255));
		SDLCall(SDL_SetTextureAlphaMod(m_TileTexture, 255));
	}


	int distanceTo(int px, int py)
	{
		// normalize vectors
		// ax/y means absolute x/y
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
			Clicked();
	}

	void changeOwner(uint8_t p_Country)
	{
		auto& country = Countries::getCountry(p_Country);
		m_Country = country.getCountryTag();
		if (m_SelectedTile == this) 
		{
			selectColor();
		}
		else
		{
			resetColor();
		}
	}

	static void setMultLength(uint16_t p_MultLength)
	{
		m_MultLength = p_MultLength;
	}

	static void setTexture(SDL_Texture* p_Texture)
	{
		m_TileTexture = p_Texture;
	}

	void resetColor()
	{
		auto& country = Countries::getCountry(m_Country);
		setColor(country.getBorderColor());
	}

	void selectColor()
	{
		const auto& country = Countries::getCountry(m_Country);
		SDL_Color color = country.getBorderColor();
		// strange looking stuff here
		// these clamps are to prevent overflows
		color.r = clamp<int>(0, 255, int(color.r) * 1.8f);
		color.g = clamp<int>(0, 255, int(color.g) * 1.8f);
		color.b = clamp<int>(0, 255, int(color.b) * 1.8f);
		setColor(color);
	}

	virtual void Clicked()
	{
		if (m_SelectedTile != NULL)
		{
			if (m_SelectedTile == this)
			{
				changeOwner(1);
				resetColor();
				m_SelectedTile = NULL;
			}
			else 
			{
				m_SelectedTile->resetColor();
				m_SelectedTile = this;
				changeOwner(1);
				selectColor();
			}
		}
		else
		{
			m_SelectedTile = this;
			changeOwner(1);
			selectColor();
		}

	}

	static void setRadius(uint16_t p_Radius) { m_Radius = p_Radius; }
	static uint16_t getRadius() { return m_Radius; }
	const SDL_Color& getColor() { return m_ColorMod; }

private:
	SDL_Color m_ColorMod{255, 255, 255, 255};
	int m_ID; // special id for this tile bc pointer checking is ineffective

	static SDL_Texture* m_TileTexture;
	static uint16_t m_MultLength;
	static uint16_t m_Radius; // pixel radius of each tile
	static Tile* m_SelectedTile;
	static RNG rng;

	// game vars

	// default population = 500
	int m_Population = 500;
	// duh
	TerrainType m_TerrainType = TerrainType::Flat;
	uint8_t m_Country = 0; // 0 = Unowned


};