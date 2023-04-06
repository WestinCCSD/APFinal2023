#include "World.h"

// defining static vars here bc I'm very lazy and can't be bothered
// to create an ENTIRE cpp file for tiles
SDL_Texture* Tile::m_TileTexture = NULL;
uint16_t Tile::m_MultLength = 0; 
uint16_t Tile::m_Radius = 0;
Tile* Tile::m_SelectedTile = NULL;
RNG Tile::rng = RNG();

World::World(uint16_t p_SizeX, uint16_t p_SizeY, uint16_t p_CellLength)
	: m_SizeX(p_SizeX), m_SizeY(p_SizeY), m_Total(p_SizeX * p_SizeY), m_CellLength(p_CellLength)
{
	const uint16_t multLength = m_CellLength / 14; // get the constant length at which this sprite is scaled up by.
	Tile::setMultLength(multLength);
	Tile::setRadius(10 * multLength); // unused for now


	// do some more tile shit idk
	TerrainTextures::terrainTextures[0] = Renderer::loadTexture("assets/art/terrainforest.png");
	TerrainTextures::terrainTextures[1] = Renderer::loadTexture("assets/art/terrainhills.png");
	TerrainTextures::terrainTextures[2] = Renderer::loadTexture("assets/art/terrainmountains.png");

	SDLVERIFY(TerrainTextures::terrainTextures[0]);
	SDLVERIFY(TerrainTextures::terrainTextures[1]);
	SDLVERIFY(TerrainTextures::terrainTextures[2]);

	m_Tilemap.reserve(m_Total);
	int id = 0;
	for (int y = 0; y < m_SizeY; y++, id++)
	{
		for (int x = 0; x < m_SizeX; x++, id++)
		{
			if (isEven(y))
			{
				m_Tilemap.emplace_back(x * m_CellLength, (y * m_CellLength) - (y == 0 ? 0 : y * 4 * multLength), m_CellLength, id);
			}
			else
			{
				m_Tilemap.emplace_back((x * m_CellLength) + 7 * multLength, (y * m_CellLength) - (y == 0 ? 0 : y * 4 * multLength), m_CellLength, id); // creates offset to create a more interesting grid
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