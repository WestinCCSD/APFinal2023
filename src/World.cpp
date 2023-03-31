#include "World.h"

SDL_Texture* Tile::m_TileTexture = NULL;
uint16_t Tile::m_MultLength = 0; // defined here bc I'm very lazy and can't be bothered
uint16_t Tile::m_Radius = 0;
// to create an ENTIRE cpp file for tiles

World::World(uint16_t p_SizeX, uint16_t p_SizeY, uint16_t p_CellLength)
	: m_SizeX(p_SizeX), m_SizeY(p_SizeY), m_Total(p_SizeX * p_SizeY), m_CellLength(p_CellLength)
{
	const uint16_t multLength = m_CellLength / 14; // get the constant length at which this 
	Tile::setMultLength(multLength);
	Tile::setRadius(10 * multLength);
	// sprite is scaled up by.
	m_Tilemap.reserve(m_Total);
	for (int y = 0; y < m_SizeY; y++)
	{
		for (int x = 0; x < m_SizeX; x++)
		{
			if (isEven(y))
			{
				m_Tilemap.emplace_back(x * m_CellLength, (y * m_CellLength) - (y == 0 ? 0 : y * 4 * multLength), m_CellLength);
			}
			else
			{
				m_Tilemap.emplace_back((x * m_CellLength) + 7 * multLength, (y * m_CellLength) - (y == 0 ? 0 : y * 4 * multLength), m_CellLength); // creates offset to create a more interesting grid
			}
		}
	}
}

void World::onClick(int px, int py)
{
	px += Camera::getX();
	py += Camera::getY();
	for (auto& tile : m_Tilemap)
	{
		tile.onClick(px, py);
	}
}