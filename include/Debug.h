#pragma once


namespace Debug
{
	// I would make these static but c++ doesn't like that (it also didn't work making these functions inline)

	// function, file, and line are all predetermined macroes so generously provided by c++!
	// exception is used to override what the correct return value is (e.g. SDL_Init should return 0 but IMG_Init should return the flags passed into it)
	bool SDLLogCall(const char* function, const char* file, int line, uint8_t error_num, uint8_t exception);
	
	bool SDLVerify(bool value, const char* file, int line);

}

// assertions for debugging
#define ASSERT(x) if (!(x)) __debugbreak();


#define SDLCall(x) ASSERT(Debug::SDLLogCall(#x, __FILE__, __LINE__, x, NULL));
#define EXCPTCall(x, exception) ASSERT(Debug::SDLLogCall(#x, __FILE__, __LINE__, x, exception)); // used when specific error codes are needed
#define SDLVERIFY(x) ASSERT( Debug::SDLVerify((x != NULL), __FILE__, __LINE__) ); // for use with textures, windows, renderers, ect. 
	