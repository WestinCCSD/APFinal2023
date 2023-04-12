#pragma once
#include "UI.h"

class ResearchButton : public UITypes::Button
{
public:
	void onClick() override
	{
		std::cout << "Research clicked!\n";
	}

private:



};

class ResearchUI : public UI
{
public:
	ResearchUI();

	void Render() override;
	void Handle(float) override;
	void UIHandle(float) override;

private:
	ResearchButton m_ResearchButton;

};