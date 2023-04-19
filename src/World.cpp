#include "World.h"
#include "ProvinceUI.h"

// defining static vars here bc I'm very lazy and can't be bothered
// to create an ENTIRE cpp file for tiles
SDL_Texture* Tile::m_TileTexture = NULL;
SDL_Texture* Tile::m_BuildingTexture = NULL;
uint16_t Tile::m_MultLength = 0; 
uint16_t Tile::m_Radius = 0;
Tile* Tile::m_SelectedTile = NULL;
Mix_Chunk* Tile::m_TileClick = NULL;
Mix_Chunk* Tile::m_ClaimSound = NULL;
RNG Tile::rng = RNG();

World::World(uint16_t p_SizeX, uint16_t p_SizeY, uint16_t p_CellLength)
	: m_SizeX(p_SizeX), m_SizeY(p_SizeY), m_Total(p_SizeX * p_SizeY), m_CellLength(p_CellLength), 
	m_TickTimer(TimerComponent<World>((*this))), m_Calendar(Calendar<World>((*this)))
{
	const uint16_t multLength = m_CellLength / 14; // get the constant length at which this sprite is scaled up by.
	Tile::setMultLength(multLength);
	Tile::setRadius(10 * multLength); // unused for now

	// do some more tile stuff idk
	TerrainTextures::terrainTextures[0] = Renderer::loadTexture("assets/art/terrainforest.png");
	TerrainTextures::terrainTextures[1] = Renderer::loadTexture("assets/art/terrainhills.png");
	TerrainTextures::terrainTextures[2] = Renderer::loadTexture("assets/art/terrainmountains.png");

	SDLVERIFY(TerrainTextures::terrainTextures[0]);
	SDLVERIFY(TerrainTextures::terrainTextures[1]);
	SDLVERIFY(TerrainTextures::terrainTextures[2]);

	m_Tilemap.reserve(m_Total);
	int id = 0;
	for (int y = 0; y < m_SizeY; y++)
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
			Country& country = Countries::getCountry(0);
			country.addTile(&m_Tilemap[id]);

		}
	}

	Tile::m_TileClick = Mix_LoadWAV("assets/sfx/province_own_click.wav");
	Tile::m_ClaimSound = Mix_LoadWAV("assets/sfx/province_core.wav");

	m_TickTimer.Start(-1);


	std::string string = "";
	for (int i = 0; i < 22; i++)
	{
		string = "assets/mus/song (";
		string += std::to_string(i + 1);
		string += ").mp3";
		std::cout << string << "\n";
		m_Music[i] = Mix_LoadMUS(string.c_str());
		string.clear();
	}

	int pos[2] = { 10, 110 };
	m_MusicSlider = UITypes::Slider();
	m_MusicSlider.init(100, 0, 48, 48, "assets/art/defaultslider.png", {}, pos);
	m_VolumeIcon = UI();
	m_VolumeIcon.init(158, 0, 48, 48, "assets/art/volumeicon.png", {}, NULL);

	rng.Randomize();

}

void World::Handle(float p_Delta)
{
	m_MusicSlider.Handle(0.f);

	for (auto& tile : m_Tilemap)
	{
		tile.Handle(p_Delta);
	}
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(m_Music[rng.randRange(0, 22)], 0);
	}
	Mix_Volume(-1, m_MusicSlider.getProgress() * 128);
	Mix_VolumeMusic(m_MusicSlider.getProgress() * 128);
}

void World::onClick(int px, int py)
{
	auto rect = ProvinceUI::getRect();
	SDL_Point point = { px, py };

	if (!SDL_PointInRect(&point, &rect) || !ProvinceUI::isVisible())
	{
		px += Camera::getX();
		py += Camera::getY();

		for (auto& tile : m_Tilemap)
		{
			tile.onClick(px, py);
		}
	}
}

uint32_t World::onTimeout(Timer*)
{
	m_Calendar.dayTick();
	return -1;
}

void World::Pause()
{
	m_Calendar.Pause();
	if (m_Paused)
	{
		m_TickTimer.Start(-1);
		m_Paused = false;
	}
	else
	{
		m_Paused = true;
		m_TickTimer.Stop();
	}
}

void World::dayTick()
{
	for (auto& object : m_Tilemap)
	{
		object.dayTick();
	}
}

void World::weekTick()
{
	Countries::Tick();
	for (auto& object : m_Tilemap)
	{
		object.Tick();
	}
}

void World::yearTick()
{
	std::cout << "A year has passed!\n";
}