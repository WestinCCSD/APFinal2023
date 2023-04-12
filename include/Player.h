#pragma once
#include "UI.h"
#include "Country.h"

class Player : public GameObject
{
public:
	Player();

	void Handle(float) override;
	// although player itself does not draw anything, it does have its own UI
	void Render() override;

private:
	Country* m_Country;
	uint8_t m_CountryID;

	UI* m_ResourceHud1Base;
	UI* m_ResourceHud1Icon;
	UITypes::Label* m_ResourceHud1Text;

	UI* m_TreausuryBase;
	UITypes::Label* m_MoneyLabel;

	uint32_t m_PrevStockpile{ 0 };

};