#include <time.h>
#include <cstdlib>

#pragma once
#define RNG RandomNumberGenerator

class RandomNumberGenerator
{
public:
	RandomNumberGenerator()
	{
		Randomize();
	}

	int getRandomNumber()
	{
		return rand();
	}

	// returns a random number between min and max, or 0 if max is less than or equal to min
	int randRange(int min, int max)
	{
		if (min < max)
		{
			max -= min; // goofy business!
			auto ret = rand() % max;
			ret += min;
			return ret;
		}
		else
		{
			return 0;
		}
	}

	// p_Digits how many the float should have
	// anything higher than 6 and the float will not have enough memory to store
	float randFloat()
	{
		float ret = rand() % 100;
		ret /= 100;
		return ret;
	}

	void Randomize()
	{
		srand(time(NULL));
	}

	bool Roll(float p_SuccessChance)
	{
		return (randFloat() <= p_SuccessChance);
	}

};
