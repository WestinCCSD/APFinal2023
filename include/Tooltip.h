#pragma once
#include "UI.h"

class Tooltip : public UITypes::Label
{
public:
	Tooltip(const std::string& p_Text)
	{
		m_Text = p_Text;

		init(0, 0, 0, 0, OFFICIALFONT, {}, m_Text.c_str());
	}

	void UIHandle(float) override
	{
		SDL_GetMouseState(&x, &y);
	}
private:



};