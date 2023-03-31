#pragma once
#include <vector>
#include <string>
#include "GameObject.h"

// a UI object is an element, or collection of elements, that can be rendered
// and is interactable (buttons, sliders, text, etc.)
class UI : public GameObject
{
public:
	
	UI(int* rect, const char* p_TexturePath, const std::vector<UI>& p_Children)
	{
		w = rect[2];
		h = rect[3];
		init(rect[0], rect[1], p_TexturePath, p_Children);
	}
	UI(int px, int py, int pw, int ph, const char* p_TexturePath, const std::vector<UI>& p_Children)
	{
		w = pw;
		h = ph;
		init(px, py, p_TexturePath, p_Children);
	}

	// exists as de facto constructor so copy pasting isn't necessary for inherited classes
	void init(int px, int py, const char* p_TexturePath, const std::vector<UI>& p_Children)
	{
		x = px;
		y = py;
		m_Texture = Renderer::loadTexture(p_TexturePath);
		SDLVERIFY(m_Texture);

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

class Slider : public UI
{

};