#include "Player.h"

Player::Player()
{
	uint8_t tag;
	Countries::addCountry(Country({ 80, 80, 200, 255 }, "Player", 0, 0.f), true, &tag);

	m_CountryID = tag;
	m_Country = &Countries::getCountry(tag);

	// treasury
	{
		m_TreausuryBase = new UI;
		m_TreausuryBase->init(165, 0, 306, 72, "assets/art/treasuryhud.png", {}, NULL);
		m_MoneyLabel = new UITypes::Label;
		m_MoneyLabel->init(0, 22, 0, 0, OFFICIALFONT, {}, "0");
		m_MoneyLabel->setCenter(165 + 170, -1);
	}
	// food
	{
		m_ResourceHud1Base = new UI;
		m_ResourceHud1Base->init(480, 0, 360, 72, "assets/art/stockpilehud.png", {}, NULL);
		m_ResourceHud1Icon = new UI;
		m_ResourceHud1Icon->init(498, 16, 42, 42, "assets/art/stockpilefoodicon.png", {}, NULL);
		m_ResourceHud1Text = new UITypes::Label;
		m_ResourceHud1Text->init(0, 22, 0, 0, OFFICIALFONT, {}, "0(0)");
		m_ResourceHud1Text->setCenter(690, -1);
	}

}

void Player::Handle(float)
{
	// treasury jargon
	{
		std::string string = std::to_string(m_Country->getMoney());
		m_MoneyLabel->setText(string + "G");
	}
	// food stockpile jargon
	{
		auto stockpile = m_Country->getFoodStockpile();
		uint32_t stockpilecapped = clamp<uint32_t>(0, 1000, stockpile);
		m_Country->setFoodStockpile(stockpilecapped);

		int dif = int(stockpilecapped) - int(m_PrevStockpile);
		if (dif != 0)
		{

			std::string string = std::to_string(stockpilecapped);


			if (dif > 0)
			{
				m_ResourceHud1Text->setColor({ 40, 220, 40, 255 });
			}
			else
			{
				m_ResourceHud1Text->setColor({ 220, 40, 40, 255 });
			}

			string += " (";
			string += std::to_string(dif);
			string += ")";
			m_ResourceHud1Text->setText(string);

			if (stockpilecapped == 1000)
			{
				m_ResourceHud1Text->setColor({ 251, 242, 54, 255 });
				string = std::to_string(stockpilecapped) + "(0)";
				m_ResourceHud1Text->setText(string);
			}

		}

		m_PrevStockpile = stockpilecapped;
	}

}

void Player::Render()
{
	m_ResourceHud1Base->UIRender();
	m_ResourceHud1Icon->UIRender();
	m_ResourceHud1Text->UIRender();
	m_TreausuryBase->UIRender();
	m_MoneyLabel->UIRender();
}