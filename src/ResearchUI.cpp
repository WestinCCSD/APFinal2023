#include "ResearchUI.h"

ResearchUI::ResearchUI()
{
	this->init(0, 0, 360, 396, "assets/art/researchuibase.png", {}, NULL);
	m_ResearchButton.init(200, 200, 120, 120, "assets/art/researchbutton.png", {}, NULL);
}


void ResearchUI::Render()
{
	UIRender();

	m_ResearchButton.UIRender();

}

void ResearchUI::Handle(float d)
{
	std::cout << "Bleh\n";
	UIHandle(d);
}

void ResearchUI::UIHandle(float)
{
	//m_ResearchButton.UIHandle(0.f);
}