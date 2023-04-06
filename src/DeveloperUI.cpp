#include "DeveloperUI.h"

DevUI::DevUI(int px, int py, int pw, int ph)
{
	init(px, py, pw, ph, "assets/art/devuibase.png", {}, NULL);
	SDLVERIFY(m_Texture);
}