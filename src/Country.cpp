#include <iostream>
#include "Country.h"
#include "Tile.h"

// define countries :D
Countries* Countries::m_Countries = NULL;
std::vector<Country> Countries::m_Data;
Mix_Chunk* Country::m_GainedProvince = Mix_LoadWAV("assets/sfx/province_core.wav");

void Country::initStockpile()
{
	m_Stockpile.reserve(4);

	for (int i = 0; i < 4; i++)
	{
		m_Stockpile.emplace_back(ResourceStorage(ResourceTypes[i], 20.f));
	}

}

Countries::Countries() 
{
	addCountry
		({
			{200, 200, 225, 255},
			"Unowned",
			0,
			100.f
		}, false, NULL);
	m_Data[0].setPopulationBonus(-1.f);
}

const Countries& Countries::get()
{
	if (m_Countries == NULL)
	{
		m_Countries = new Countries();
	}
	return (*m_Countries);
}

void Countries::addCountry(const Country& p_Country, bool p_AutoTag, uint8_t* p_TgtTag)
{
	m_Data.reserve(m_Data.size() + 1);
	m_Data.emplace_back(p_Country);
	// ^ this looks strange but what I'm doing is simply avoiding as many copy operations as possible
	if (p_AutoTag)
	{
		m_Data[m_Data.size() - 1].setCountryTag(m_Data.size() - 1);
		if (p_TgtTag != NULL)
			(*p_TgtTag) = m_Data.size() - 1;
	}

}

Country& Countries::getCountry(uint8_t p_Index)
{
	return m_Data[p_Index];
}

void Countries::Tick()
{
	for (auto& country : m_Data)
	{
		if (country.getCountryTag() != 0)
			country.calculateYield();
	}
}

void Country::calculateYield()
{
	// 0 : iron
	// 1 : gems
	// (food is calculated playerside)
	uint32_t yield1 = 0;
	uint32_t yield2 = 0;

	for (auto& tile : m_Tiles)
	{
		Tile* tileobj = (Tile*)(tile);
		const auto& resource = tileobj->getResource();
		if (resource.m_Category == ResourceCategory::Valuable)
		{
			yield1 += tileobj->getYield();
		}
		else if (resource.m_Category == ResourceCategory::Material)
		{
			yield2 += tileobj->getYield();
		}
	}

	yield1 /= floor(ResourceTypes[1].m_Value);
	yield2/= floor(ResourceTypes[0].m_Value);

	m_Money += yield1 + yield2;

}

void Country::addTile(void* p_Tile)
{
	Tile* tile = (Tile*)(p_Tile);
	tile->setCIndex(m_Tiles.size());
	m_Tiles.push_back(p_Tile);
	if (m_CountryName == "Player")
		Mix_PlayChannel(-1, m_GainedProvince, 0);
}

void Country::removeTile(void* p_Tile)
{
	Tile* tile = (Tile*)(p_Tile);
	// every other tile belonging to this country must be adjusted
	for (int i = tile->getCIndex(); i < m_Tiles.size(); i++)
	{
		Tile* atile = (Tile*)(m_Tiles[i]);
		atile->setCIndex(atile->getCIndex() - 1);
	}

	m_Tiles.erase(m_Tiles.begin() + tile->getCIndex());
}

bool Country::canDevelop(void* p_Tile)
{
	Tile* tile = (Tile*)(p_Tile);

	return (m_Money >= (tile->getTerrain() + 1) * 10);

}
