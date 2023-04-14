#pragma once
#include <SDL_mixer.h>
#include "GameObject.h"
#include "Country.h"
#include "RNG.h"
#include "Resource.h"

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

		m_Population /= (m_TerrainType + 1);
		calculateYield();

		m_ClaimProgress *= (m_TerrainType + 1);
		m_ClaimBase = m_ClaimProgress;

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
	void Handle(float) override
	{
		if (m_QueuedCountry != 0 && m_QueuedCountry != m_Country)
		{
			changeOwner(m_QueuedCountry);
		}
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

	void queueChangeOwner(uint8_t p_Country)
	{
		m_QueuedCountry = p_Country;
	}
	void changeOwner(uint8_t p_Country)
	{
		{
			auto& formercountry = Countries::getCountry(m_Country);
			formercountry.removeTile(this);
		}

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

		country.addTile(this);

	}
	void createClaim(uint8_t p_Country)
	{
		m_CountryClaim = p_Country;
		m_Claimed = true;
	}
	bool isClaimed() { return m_Claimed; }

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
		auto country = Countries::getCountry(m_Country);
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
				resetColor();
				m_SelectedTile = NULL;
			}
			else
			{
				m_SelectedTile->resetColor();
				m_SelectedTile = this;
				selectColor();
				Mix_PlayChannel(-1, m_TileClick, 0);
			}
		}
		else
		{
			m_SelectedTile = this;
			selectColor();
			Mix_PlayChannel(-1, m_TileClick, 0);
		}
	}

	// get stats related to this country
	// 0 : population
	// 1 : terrain type
	// 2 : country id
	// 3 : tile id
	// 4 : resource info
	// 5 : resource yield
	std::vector<int> getStats()
	{
		std::vector<int> gameStats =
		{
			m_Population,
			m_TerrainType,
			m_Country,
			m_ID,
			int(m_ResourceYield)
		};
		return gameStats;
	}

	uint32_t getYield() { return m_ResourceYield; }

	void dayTick()
	{
		if (m_CountryClaim != 0 && m_CountryClaim != m_Country)
		{
			m_ClaimProgress--;
			if (m_ClaimProgress == 0)
			{
				queueChangeOwner(m_CountryClaim);
			}
		}
	}

	// functions similar to handle,
	// however this is only called during world updates
	// (like 1 turn in hoi4 or civ5)
	void Tick()
	{
		calculateYield();
		calculateNeeds();
	}

	float getClaimProgress() { return (float(m_ClaimProgress) / float(m_ClaimBase)); }

	bool isHungry() { return m_Hungry; }
	int getPopulation() { return m_Population; }
	const Resource& getResource() { return (*m_Resource); }
	uint16_t getCIndex() { return m_Index; } // get country index
	
	void setCIndex(uint16_t p_Index) { m_Index = p_Index; }
	static void setRadius(uint16_t p_Radius) { m_Radius = p_Radius; }
	static uint16_t getRadius() { return m_Radius; }
	const SDL_Color& getColor() { return m_ColorMod; }
	static Tile* getSelectedTile() { return m_SelectedTile; }
	static Mix_Chunk* m_TileClick;
	static Mix_Chunk* m_ClaimSound;

private:
	SDL_Color m_ColorMod{255, 255, 255, 255};
	int m_ID; // special id for this tile bc pointer checking is ineffective
	uint16_t m_Index; // used for locating this tile in a vector/deque


	static SDL_Texture* m_TileTexture;
	static uint16_t m_MultLength;
	static uint16_t m_Radius; // pixel radius of each tile
	static Tile* m_SelectedTile;
	static RNG rng;

	// game vars

	// default population = 500
	int m_Population = 500;
	bool m_Hungry = false;
	// duh
	TerrainType m_TerrainType = TerrainType::Flat;
	uint8_t m_Country = 0; // 0 = Unowned
	uint8_t m_CountryClaim = 0; // 0 = Unowned
	uint8_t m_QueuedCountry = 0;
	bool m_Claimed = false;
	int m_ClaimBase{ 0 };
	int m_ClaimProgress{ 80 }; // time in days to claim this province
	Resource* m_Resource = NULL; // the resource this tile yields
	uint32_t m_ResourceYield = 0; // weekly resource yield


	// increase population by this formula
	// where p = current population; t = terrain type
	// 
	// 5 + (p * 0% ~ 15%)
	// ------------------
	//			t
	// we use current population to create a sort of population ramp
	void increasePopulation(float p_Modifier)
	{
		const int base = 5;
		float percent = (rng.randFloat() * 0.05) + 0.01f; // random number between 0.01 ~ 0.05
		auto& country = Countries::getCountry(m_Country);
		float bonus = country.getPopulationBonus();
		if (bonus < 0.f)
		{
			bonus = 0.f;
			percent = 0.f;
		}
		int increment = floor(base + ( (double(percent) + double(bonus)) * m_Population));
		increment /= int(m_TerrainType) + 2; 
		m_Population += (increment * p_Modifier);

	}
	void decreasePopulation(int p_Deficit)
	{
		m_Population -= p_Deficit * 2;
	}
	void calculateYield()
	{
		if (m_Resource == NULL)
		{
			m_Resource = new Resource(ResourceTypes[rng.randRange(0, 4)]);
		}

		m_ResourceYield = floor(m_Population * 0.08);

		m_ResourceYield = clamp<int>(0, 20000, m_ResourceYield);
		m_ResourceYield /= m_TerrainType + 2;

	}
	void calculateNeeds()
	{
		int food = 60; // base food output
		int pop = floor(float(m_Population - 100) * 0.08f);

		if (m_Resource->m_Category == ResourceCategory::Food)
		{
			food += m_ResourceYield;
		}

		auto& country = Countries::getCountry(m_Country);

		if (food >= pop)
		{
			// enough food :D (likely there's surplus)
			increasePopulation(1.f);
			m_Hungry = false;

			if (m_Resource->m_Category == ResourceCategory::Food && country.getCountryTag() != 0)
			{
				auto stockpile = country.getFoodStockpile();
				uint32_t surplus = food - pop;
				country.setFoodStockpile(stockpile + surplus);
			}

		}
		else
		{
			// not enough food :'(
			// try to get food from stockpile, if there's no food... hungers :(
			int deficit = pop - food;
			auto stockpile = country.getFoodStockpile();

			if (stockpile >= deficit)
			{
				// there's enough food in stockpile :D
				country.setFoodStockpile(stockpile - deficit);
				increasePopulation(0.5f);
				m_Hungry = false;
			}
			else
			{
				// no food... hungers :(
				decreasePopulation(deficit);
				m_Hungry = true;
			}
			

		}

	}

};