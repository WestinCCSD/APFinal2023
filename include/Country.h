#include <vector>
#include <string>
#include <SDL_rect.h>
#pragma once

class Country
{
public:
	Country(const SDL_Color& p_BorderColor, const std::string& p_CountryName, uint8_t p_CountryTag)
		: m_BorderColor(p_BorderColor), m_CountryName(p_CountryName), m_CountryTag(p_CountryTag)
	{}
	Country(const Country& p_Country)
		: m_BorderColor(p_Country.m_BorderColor), m_CountryName(p_Country.m_CountryName), m_CountryTag(p_Country.m_CountryTag)
	{}

	void setBorderColor(const SDL_Color& p_BorderColor) { m_BorderColor = p_BorderColor; }
	void setCountryName(const std::string p_CountryName) { m_CountryName = p_CountryName; }
	void setCountryTag(uint8_t p_CountryTag) { m_CountryTag = p_CountryTag; }


	const SDL_Color& getBorderColor() const { return m_BorderColor; }
	const std::string& getCountryName() const { return m_CountryName; }
	uint8_t getCountryTag() const { return m_CountryTag; }


private:
	SDL_Color m_BorderColor; // yep
	std::string m_CountryName; // name for this country (might not even use this tbh)
	uint8_t m_CountryTag; // unique id for this country

};


// singleton class for countries
class Countries
{
public:
	// delete copy constructor
	Countries(const Countries&) = delete;

	// second parameter is whether or not to automatically assigned
	static void addCountry(const Country&, bool);


	static const Countries& get();
	static Country& getCountry(uint8_t);



private:
	// singleton jargon
	// private constructor so no more than ONE Countries instance may exist
	Countries();
	static Countries* m_Countries;

	static std::vector<Country> m_Data;

};