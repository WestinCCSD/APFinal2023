#pragma once
#include <SDL_mixer.h>
#include <vector>
#include <deque>
#include <SDL_rect.h>
#include <string>
#include "Resource.h"
#include "Timer.h"

class Country
{
public:
	Country(const SDL_Color& p_BorderColor, const std::string& p_CountryName, uint8_t p_CountryTag, float p_Bonus)
		: m_BorderColor(p_BorderColor), m_CountryName(p_CountryName), m_CountryTag(p_CountryTag), m_PopulationBonus(p_Bonus), m_SoundTimer(TimerComponent<Country>((*this)))
	{
		initStockpile();
		m_GainedProvince = Mix_LoadWAV("assets/sfx/province_core.wav");
	}
	Country(const Country& p_Country)
		: m_BorderColor(p_Country.m_BorderColor), m_CountryName(p_Country.m_CountryName), m_CountryTag(p_Country.m_CountryTag), m_PopulationBonus(p_Country.m_PopulationBonus),
		  m_ManpowerPercent(p_Country.m_ManpowerPercent), m_Tiles(p_Country.m_Tiles), m_SoundTimer(TimerComponent<Country>((*this)))
	{
		initStockpile();
		m_GainedProvince = Mix_LoadWAV("assets/sfx/province_core.wav");
	}

	void setBorderColor(const SDL_Color& p_BorderColor) { m_BorderColor = p_BorderColor; }
	void setCountryName(const std::string p_CountryName) { m_CountryName = p_CountryName; }
	void setCountryTag(uint8_t p_CountryTag) { m_CountryTag = p_CountryTag; }
	void setPopulationBonus(float p_Bonus) { m_PopulationBonus = p_Bonus; }
	void setFoodStockpile(uint32_t p_Stockpile) { m_FoodStockpile = p_Stockpile; }
	void setMoney(int p_Money) { m_Money = p_Money; }
	void addMoney(int p_Money) { m_Money += p_Money; }

	void addTile(void* p_Tile);
	void removeTile(void* p_Tile);
	inline void addClaim()
	{
		m_ClaimCount++;
		m_ClaimSpeed = (10.f / float(m_ClaimCount));
	}
	inline void removeClaim()
	{
		m_ClaimCount--;
		if (m_ClaimCount > 0)
			m_ClaimSpeed = (10.f / float(m_ClaimCount));
		else
			m_ClaimSpeed = 10.f;
	}
	float getClaimSpeed() { return m_ClaimSpeed; }

	const SDL_Color& getBorderColor() const { return m_BorderColor; }
	const std::string& getCountryName() const { return m_CountryName; }
	uint8_t getCountryTag() const { return m_CountryTag; }
	uint32_t getFoodStockpile() { return m_FoodStockpile; }
	float getPopulationBonus() { return m_PopulationBonus; }
	int getMoney() { return m_Money; }

	float m_PopulationBonus;

	void calculateYield();

	uint32_t onTimeout(Timer* timer) { return 0; }

private:
	SDL_Color m_BorderColor; // yep
	std::string m_CountryName; // name for this country (might not even use this tbh)
	uint8_t m_CountryTag; // unique id for this country

	// game vars

	int m_Money{ 0 }; // current money in the treasury of this country
	uint32_t m_Manpower = 0; // men ready to be recruited into active service
	float m_ManpowerPercent = 0.01f; // % of population that can be recruited
	// class we use to store resource/yield pairs
	struct ResourceStorage : Resource
	{
		ResourceStorage(const Resource& p_Resource, float m_BaseYield)
		{
			this->m_Name = p_Resource.m_Name;
			this->m_Value = p_Resource.m_Value;

		}
	};

	std::vector<ResourceStorage> m_Stockpile;
	std::deque<void*> m_Tiles; // deque of pointers to tiles
	
	uint8_t m_ClaimCount{ 0 }; // count of how many tiles this country is attempting to claim
	float m_ClaimSpeed{ 1.f }; // daily claim rate

	uint32_t m_FoodStockpile{ 0 };

	void initStockpile();

	TimerComponent<Country> m_SoundTimer;
	static Mix_Chunk* m_GainedProvince;

public:
	// 0 : iron
	// 1 : gems
	// (each country has a dedicated food storage)
	Resource** getStockpile() 
	{
		Resource* ret[2];
		ret[0] = &m_Stockpile[0];
		ret[1] = &m_Stockpile[1];
		return ret;
	}

};


// singleton class for countries
class Countries
{
public:
	// delete copy constructor
	Countries(const Countries&) = delete;

	// second parameter is whether or not to automatically assign country id
	// third parameter is to store the auto-assigned tag of the country, NULL works fine
	static void addCountry(const Country&, bool, uint8_t*);

	static const Countries& get();
	static Country& getCountry(uint8_t);
	static uint16_t getCountryCount() { return m_Data.size(); }
	
	static void Tick();

private:
	// singleton jargon
	// private constructor so no more than ONE Countries instance may exist
	Countries();
	static Countries* m_Countries;

	static std::vector<Country> m_Data;

};