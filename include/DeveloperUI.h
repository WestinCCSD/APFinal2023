#pragma once
#include "UI.h"

class DevUI : public UI
{
public:
	DevUI(int px, int py, int pw, int ph);
	DevUI(const DevUI&) = delete; // don't want it

};