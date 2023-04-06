#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "GameObject.h"
#include "SDL_ttf.h"

// a UI object is an element, or collection of elements, that can be rendered
// and is interactable (buttons, sliders, text, etc.)
class UI : public GameObject
{
public:

	// exists as de facto constructor so copy pasting isn't necessary for inherited classes
	// data can be anything, usually used for additional parameters in child classes
	virtual void init(int px, int py, int pw, int ph, const char* p_TexturePath, const std::vector<UI>& p_Children, void* p_Data)
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
		virtual bool clickCheck()
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
		// data should contain an array of 2 ints, one for leftmost position of slider
		// one for rightmost position of slider. These points are in global space;
		// they are NOT relative to the slider's position!
		virtual void init(int px, int py, int pw, int ph, const char* p_TexturePath, const std::vector<UI>& p_Children, void* p_Data) override
		{
			w = pw;
			h = ph;
			x = px;
			y = py;
			m_Texture = Renderer::loadTexture(p_TexturePath);

			int* sliderParams = (int*)p_Data; // cast p_Data into int pointer
			m_SliderX1 = sliderParams[0];
			m_SliderX2 = sliderParams[1];

			setSliderSprite("assets/art/defaultsliderback.png");

			// there is probably a better way of doing this...
			// but for such a small project it's no big deal
			m_Children.reserve(p_Children.size());
			for (int i = 0; i < p_Children.size(); i++)
			{
				m_Children.emplace_back(p_Children[i]);
			}
			clampPostion();
		}

		virtual void UIHandle(float) override
		{
			bool prev = m_mouseDown;
			m_mouseDown = false;
			if (clickCheck())
			{
				int prevx = x;
				SDL_GetMouseState(&x, NULL);
				x -= (w / 2); // adjust position
				clampPostion();
				if (x != prevx)
				{
					int absx = x - m_SliderX1, abssldrx = m_SliderX2 - m_SliderX1;
					// ^^ normalize slider positions to calculate progress
					m_Progress = float(absx) / float(abssldrx);
					valueChanged();
				}
			}
			m_mouseDown = prev;

		}

		virtual void UIRender() override
		{
			SDL_Rect rect = TORECT;
			SDL_Rect sliderrect = { m_SliderX1, y, (m_SliderX2 - m_SliderX1) + w, h };

			SDL_RenderCopy(Renderer::getRenderer(), m_SliderSprite, NULL, &sliderrect);
			SDL_RenderCopy(Renderer::getRenderer(), m_Texture, NULL, &rect);
		}

		virtual void valueChanged() 
		{
			std::cout << m_Progress << "\n";
		}

		void setSliderSprite(const char* p_Path)
		{
			m_SliderSprite = Renderer::loadTexture(p_Path);
		}

		// set the bounds of the slider
		void setSliderBounds(int px1, int px2)
		{
			m_SliderX1 = px1;
			m_SliderX2 = px2;
		}

	protected:
		void clampPostion()
		{
			x = clamp<int>(m_SliderX1, m_SliderX2, x);
		}

		// the left most point & right most point of this slider
		int m_SliderX1, m_SliderX2;
		float m_Progress = 0.f;

		SDL_Texture* m_SliderSprite;

	};

	class Label : public UI
	{
	public:



	protected:



	};

}
