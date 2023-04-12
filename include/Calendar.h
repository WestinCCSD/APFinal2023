#include "UI.h"
#include <sstream>
#pragma once

template<class T>
class Calendar : public UI
{
public:

	Calendar(T& p_World)
		: m_World(p_World)
	{

		m_DayBlank = Renderer::loadTexture("assets/art/dayui.png");
		m_DayOver = Renderer::loadTexture("assets/art/dayover.png");
		SDLVERIFY(m_DayBlank);
		SDLVERIFY(m_DayOver);

		m_Pause = Renderer::loadTexture("assets/art/pause.png");
		m_Play = Renderer::loadTexture("assets/art/play.png");
		SDLVERIFY(m_Pause);
		SDLVERIFY(m_Play);

		int totalwidth = 22 * 5;

		int ww;
		int wh;

		Renderer::getWindowProperties(&ww, &wh);

		for (int i = 0; i < 5; i++)
		{
			m_DayUI[i] = UI();
			m_DayUI[i].init((i * 22) + ww - totalwidth - 60, 33, 22, 22, "assets/art/dayui.png", {}, NULL);
			//m_DayUI[i].init(0, 33, 22, 22, "assets/art/dayui.png", {}, NULL);
			// we immediately free texture
			m_DayUI[i].freeTexture();
			// then pass in the new one
			m_DayUI[i].setTexture(m_DayBlank);
		}

		m_Date.init(0, 11, 0, 0, OFFICIALFONT, {}, "Day 0");
		m_Date.setCenter(ww - (totalwidth / 2) - 60, -1);
		m_Date.setFont(m_Date.loadFont(OFFICIALFONT, 18));

		m_PauseIcon.init(ww - totalwidth - 22 - 60, 33, 22, 22, "assets/art/play.png", {}, NULL);
		m_PauseIcon.freeTexture();
		m_PauseIcon.setTexture(m_Play);

	}

	void Pause()
	{
		if (m_Paused)
		{
			m_PauseIcon.setTexture(m_Play);
			m_Paused = false;
		}
		else
		{
			m_PauseIcon.setTexture(m_Pause);
			m_Paused = true;
		}
	}

	void dayTick()
	{
		m_Day++;
		m_DayTotal++;

		for (int i = 0; i < m_Day; i++)
		{
			m_DayUI[i].setTexture(m_DayOver);
		}

		if (m_Day >= 6)
		{
			m_Day = 0;
			weekTick();
		}

		updateDate();
		m_World.dayTick();

	}

	void weekTick()
	{
		m_Week++;

		for (int i = 0; i < 5; i++)
		{
			m_DayUI[i].setTexture(m_DayBlank);
		}

		if (m_Week >= 52)
		{
			m_Week = 0;
			yearTick();
		}

		m_World.weekTick();

		m_PauseIcon.setTexture((m_Paused ? m_Pause : m_Play));

	}

	void yearTick()
	{
		m_Year++;
		m_World.yearTick();
	}

	void UIRender() override
	{
		if (m_Visible)
		{
			for (int i = 0; i < 5; i++)
			{
				m_DayUI[i].UIRender();
			}
			m_PauseIcon.UIRender();
			m_Date.UIRender();
		}
	}

private:
	bool m_Paused;
	uint8_t m_Day{ 0 }; // day 1 ~ 5
	uint32_t m_Week{ 0 }; // week 1 ~ 52
	uint32_t m_Year{ 0 }; // year 1 ~ infinity

	uint32_t m_DayTotal{ 0 };

	UI m_DayUI[5];
	UI m_PauseIcon;
	UITypes::Label m_Date;

	T& m_World;

	SDL_Texture* m_DayBlank;
	SDL_Texture* m_DayOver;

	SDL_Texture* m_Pause;
	SDL_Texture* m_Play;

	// used for joining string
	std::stringstream m_SS;

	void updateDate()
	{
		using namespace std;
		
		string string = "";

		if (m_Year)
		{
			string += "Year ";
			string += to_string(m_Year);
			string += ", ";
		}
		if (m_Week)
		{
			string += "Week ";
			string += to_string(m_Week);
			string += ", ";
		}
		
		string += "Day ";
		string += std::to_string(m_Day);

		m_Date.queueSetText(string);
	}

};
