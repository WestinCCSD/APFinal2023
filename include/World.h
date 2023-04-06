#pragma once
#include <iostream>
#include <vector>
#include "Tile.h"

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
	}

	void Handle(float p_Delta) override
	{
		for (auto& tile : m_Tilemap)
		{
			tile.Handle(p_Delta);
		}
	}

	void onClick(int px, int py);

private:
	std::vector<Tile> m_Tilemap;

	uint8_t m_CellLength;
	uint16_t m_SizeX, m_SizeY, m_Total;

	inline bool isEven(const uint64_t& p_Value)
	{
		return ((p_Value % 2) == 0);
	}

};