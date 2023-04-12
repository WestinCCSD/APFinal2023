#include <iostream>
#include <SDL.h>
#include "../include/Debug.h"

bool Debug::SDLLogCall(const char* function, const char* file, int line, uint8_t error_num, uint8_t exception)
{
	{
		if (error_num != exception)
		{
			std::cout << "[SDL Error] (" << SDL_GetError() << "): " << function << " " << file << " : " << line << "\n";
			return false;
		}
		return true;
	}
}

bool Debug::SDLVerify(bool value, const char* file, int line) 
{
	if (!value)
	{
		std::cout << "[SDL Error] (Resource failed validity check!): " << file << " : " << line << " More info is displayed below if possible \n";
		std::cout << SDL_GetError() << "\n";
	}

	return value;
}
