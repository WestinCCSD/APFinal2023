#include "Country.h"

// define countries :D
Countries* Countries::m_Countries = NULL;
std::vector<Country> Countries::m_Data;

Countries::Countries() 
{
	addCountry
		({
			{200, 200, 225, 255},
			"Unowned",
			0
		}, false);
}

const Countries& Countries::get()
{
	if (m_Countries == NULL)
	{
		m_Countries = new Countries();
	}
	return (*m_Countries);
}

void Countries::addCountry(const Country& p_Country, bool p_AutoTag)
{
	m_Data.reserve(m_Data.size() + 1);
	m_Data.emplace_back(p_Country);
	// ^ this looks strange but what I'm doing is simply avoiding as many copy operations as possible
	if (p_AutoTag)
		m_Data[m_Data.size() - 1].setCountryTag(m_Data.size() - 1);
}

Country& Countries::getCountry(uint8_t p_Index)
{
	return m_Data[p_Index];
}