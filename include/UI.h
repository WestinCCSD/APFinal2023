#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "GameObject.h"

// a UI object is an element, or collection of elements, that can be rendered
// and is interactable (buttons, sliders, text, etc.)
class UI : public GameObject
{
public:

	// exists as de facto constructor so copy pasting isn't necessary for inherited classes
	void init(int px, int py, int pw, int ph, const char* p_TexturePath, const std::vector<UI>& p_Children)
	{
		w = pw;
		h = ph;
		x = px;
		y = py;
		m_Texture = Renderer::loadTexture(p_TexturePath);

		// there is probably a better way of doing this...
		// but for such a small project it's no big deal
		m_Children.reserve(p_Children.size());
		for (int i = 0; i < p_Children.size(); i++)
		{
			m_Children.emplace_back(p_Children[i]);
		}
	}

	// DO NOT override this for inherited UI classes! Override UIHandle and
	// UIRender instead!
	void Render() override
	{
		for (auto& child : m_Children)
		{
			child.Render();
		}

		UIRender();

	}

	void Handle(float p_Delta) override
	{
		for (auto& child : m_Children)
		{
			child.Handle(p_Delta);
		}

		UIHandle(p_Delta);

	}

	void setFamilyPosition(int px, int py)
	{
		for (auto& child : m_Children)
		{
			child.setFamilyPosition(px, py);
		}

		x = px;
		y = py;

	}
	
	void moveFamily(int px, int py)
	{
		for (auto& child : m_Children)
		{
			child.moveFamily(px, py);
		}

		x += px;
		y += py;
	}

	const std::string& getName() { return m_Name; }

	UI& getChild(int index)
	{
		return m_Children[index];
	}

	// finds the first child with a matching name
	// returns NULL if child is not found
	// todo: finish this so it works
	UI* getChild(std::string p_Name)
	{
		UI* ret = NULL;

		for (auto& child : m_Children)
		{
			ret = child.getChild(p_Name);
			if (child.getName() == p_Name)
			{
				ret = &child;
			}
		}

		return ret;
	}

	// functions specifically for this ui element
	virtual void UIHandle(float p_Delta)
	{

	}

	// functions specifically for this ui element
	virtual void UIRender()
	{
		SDL_Rect rect = TORECT;
		SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_Texture, NULL, &rect));
	}

protected:
	std::vector<UI> m_Children; // children UI, which may have their own children
	std::string m_Name; // should be specific for each UI instance
	// used for searching/organizing elements and to easily identify said elements

};

namespace UITypes
{
	class Button : public UI
	{
	public:
		bool clickCheck()
		{
			int lx = 0, ly = 0;
			auto mask = SDL_GetMouseState(&lx, &ly);
			if (mask == 1)
			{
				if (!m_mouseDown)
				{
					SDL_Point point{ lx, ly };
					SDL_Rect rect = TORECT;
					m_mouseDown = true;
					return SDL_PointInRect(&point, &rect);
				}
			}
			else
			{
				m_mouseDown = false;
			}
			return false;
		}
		
		// can be overriden for children classes, but make sure to call "clickCheck"
		// (though this is not reccomended)
		void UIHandle(float) override
		{
			if (clickCheck())
			{
				onClick();
			}
		}

		void forceClick(int px, int py)
		{
			SDL_Point point{ px, py };
			SDL_Rect rect = TORECT;

			if (SDL_PointInRect(&point, &rect))
			{
				onClick();
			}

		}

		virtual void onClick() 
		{
			std::cout << "Button clicked!\n";
		}

	protected:
		static bool m_mouseDown;

	};

	class Slider : public Button
	{
	public:
		

		virtual void valueChanged() {}


	protected:
		int m_SliderX, m_SliderY;
		float m_Progress = 0.f;

		SDL_Texture* m_SliderSprite;

	};
}
