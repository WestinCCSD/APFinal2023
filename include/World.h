#pragma once
#include <iostream>
#include <vector>
#include <SDL_mixer.h>
#include "Tile.h"
#include "Timer.h"
#include "Calendar.h"

// stores data for the world
class World : public GameObject {
public:
	World(uint16_t p_SizeX, uint16_t p_SizeY, uint16_t p_CellLength);

	void Render() override
	{
		for (auto& tile : m_Tilemap) 
		{
			tile.Render();
		}
		m_Calendar.Render();
		m_MusicSlider.Render();
		m_VolumeIcon.Render();
	}

	void Handle(float p_Delta) override;

	void onClick(int px, int py);

	void Pause();
	void dayTick();
	void weekTick();
	void yearTick();
	uint32_t onTimeout(Timer*);

private:
	std::vector<Tile> m_Tilemap;

	uint8_t m_CellLength;
	uint16_t m_SizeX, m_SizeY, m_Total;

	bool m_Paused = false;

	TimerComponent<World> m_TickTimer;
	Calendar<World> m_Calendar;

	UITypes::Slider m_MusicSlider;
	UI m_VolumeIcon;

	inline bool isEven(const uint64_t& p_Value)
	{
		return ((p_Value % 2) == 0);
	}
	
	RNG rng{};
	Mix_Music* m_Music[22];

	

};