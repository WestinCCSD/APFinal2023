#pragma once
#include <string>

enum ResourceCategory
{
	Food,
	Valuable,
	Material
};

// simple class for different resource types
struct Resource
{
	Resource()
	{
		m_Name = "N/A";
		m_Value = 1;
	}
	Resource(const std::string& p_Name, uint8_t p_Value, ResourceCategory p_Category)
		: m_Name(p_Name), m_Value(p_Value), m_Category(p_Category)
	{}

	std::string m_Name; // name of resource
	ResourceCategory m_Category;
	uint16_t m_Value; // how many of these equate to 1 gold

};

// 0 : iron
// 1 : gems
// 2 : livestock
// 3 : produce
static Resource ResourceTypes[4] =
{
	Resource("Iron", 40, ResourceCategory::Material),
	Resource("Gems", 20, ResourceCategory::Valuable),
	Resource("Livestock", 60, ResourceCategory::Food),
	Resource("Produce", 60, ResourceCategory::Food),
};
