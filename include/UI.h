#pragma once
#include <iostream>
#include <deque>
#include <string>
#include "GameObject.h"
#include "SDL_ttf.h"

#define OFFICIALFONT "assets/fonts/AGaramondPro-Regular.ttf"

// a UI object is an element, or collection of elements, that can be rendered
// and is interactable (buttons, sliders, text, etc.)
class UI : public GameObject
{
public:

	// exists as de facto constructor so copy pasting isn't necessary for inherited classes
	// data can be anything, usually used for additional parameters in child classes
	virtual void init(int px, int py, int pw, int ph, const char* p_TexturePath, const std::deque<UI*> p_Children, const void* p_Data)
	{
		m_Texture = Renderer::loadTexture(p_TexturePath);
		baseInit(px, py, pw, ph, p_Children);
	}

	// DO NOT override this for inherited UI classes! Override UIHandle and
	// UIRender instead!
	void Render() override
	{
		if (m_Visible)
		{
			for (auto& child : m_Children)
			{
				child->Render();
			}

			UIRender();
		}
	}

	void Handle(float p_Delta) override
	{
		for (auto& child : m_Children)
		{
			child->Handle(p_Delta);
		}

		UIHandle(p_Delta);

	}

	void setFamilyPosition(int px, int py)
	{
		for (auto& child : m_Children)
		{
			child->setFamilyPosition(px, py);
		}

		x = px;
		y = py;

	}
	
	void moveFamily(int px, int py)
	{
		for (auto& child : m_Children)
		{
			child->moveFamily(px, py);
		}

		x += px;
		y += py;
	}

	const std::string& getName() { return m_Name; }

	UI* getChild(int index)
	{
		return m_Children[index];
	}

	virtual void UIHandle(float) {}

	// functions specifically for this ui element
	virtual void UIRender()
	{
		if (m_Visible)
		{
			SDL_Rect rect = TORECT;
			SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_Texture, NULL, &rect));
		}
	}

	void Show() { m_Visible = true; }
	void Hide() { m_Visible = false; }
	void toggleVisibility() { m_Visible = !m_Visible; }

protected:
	std::deque<UI*> m_Children; // children UI, which may have their own children
	std::string m_Name; // should be specific for each UI instance
	// used for searching/organizing elements and to easily identify said elements

	bool m_Visible = true;

	// used as base init, does not load in textures!
	void baseInit(int px, int py, int pw, int ph, std::deque<UI*> p_Children)
	{
		w = pw;
		h = ph;
		x = px;
		y = py;

		// there is probably a better way of doing this...
		// but for such a small project it's no big deal
		for (int i = 0; i < p_Children.size(); i++)
		{
			m_Children.push_back(p_Children[i]);
		}
	}

};

namespace UITypes
{


	// standard issue button. override click function to change logic
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

	// behaves like button, but follows the mouse along a certain range of the x axis
	class Slider : public Button
	{
	public:
		// data should contain an array of 2 ints, one for leftmost position of slider
		// one for rightmost position of slider. These points are in global space;
		// they are NOT relative to the slider's position!
		virtual void init(int px, int py, int pw, int ph, const char* p_TexturePath, const std::deque<UI*> p_Children, const void* p_Data) override
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
			for (int i = 0; i < p_Children.size(); i++)
			{
				m_Children.push_back(p_Children[i]);
			}
			clampPostion();
		}

		float getProgress() { return m_Progress; }

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

		virtual void valueChanged() {}

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
		float m_Progress = 1.f;

		SDL_Texture* m_SliderSprite;

	};

	// standard text label, supports multiple fonts, colors, centering, and per-frame logic
	class Label : public UI
	{
	public:
		~Label()
		{
			TTF_CloseFont(m_Font);
		}

		// for labels
		// most params stay the same, data is used for starting text
		virtual void init(int px, int py, int pw, int ph, const char* p_FontPath, const std::deque<UI*> p_Children, const void* p_Data) override
		{
			baseInit(px, py, pw, ph, p_Children);

			if (p_Data != NULL)
			{
				const char* text = (const char*)p_Data;
				m_Text = text;
			}

			setFont(loadFont(p_FontPath, 32));
			regenerateText();
		}

		virtual void UIRender() override
		{
			if (m_Visible)
			{
				SDL_Rect rect = { x, y, m_RenderWidth, m_RenderHeight };
				if (m_CenterX != -1)
					rect.x = m_CenterX - (m_RenderWidth / 2);
				if (m_CenterY != -1)
					rect.y = m_CenterY - (m_RenderWidth / 2);
				SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_Texture, NULL, &rect));

				if (!m_QueuedText.empty())
				{
					setText(m_QueuedText);
					m_QueuedText.clear();
				}

			}
		}

		void setCenter(int px, int py)
		{
			m_CenterX = px;
			m_CenterY = py;
		}

		void setText(const std::string& p_Text)
		{
			m_Text = p_Text;
			regenerateText();
		}

		void queueSetText(const std::string& p_Text)
		{
			m_QueuedText = p_Text;
		}

		void setFont(TTF_Font* p_Font)
		{
			m_Font = p_Font;
			regenerateText();
		}

		void setColor(const SDL_Color& p_FG)
		{
			m_FGColor = p_FG;
			regenerateText();
		}

		TTF_Font* loadFont(const char* p_Path, int p_Size)
		{
			m_Size = p_Size;
			TTF_Font* font = TTF_OpenFont(p_Path, p_Size);
			SDLVERIFY(font);
			return font;
		}

		const TTF_Font* const getFont() { return m_Font; }
		const std::string& getText() { return m_Text; }

	protected:
		// DON'T TOUCH THIS unless there's a good reason to! The data in this
		// rect will automatically change when the font or text is changed!
		int m_RenderWidth{ 0 };
		int m_RenderHeight{ 0 };
		// if this value is not x this label will be centered around this point
		int m_CenterX = -1;
		int m_CenterY = -1;
		TTF_Font* m_Font;
		std::string m_Text;
		int m_Size;
		SDL_Color m_FGColor{255, 255, 255, 255};
		
		std::string m_QueuedText{};

		void regenerateText()
		{
			SDL_Surface* surface = TTF_RenderText_Blended(m_Font, m_Text.c_str(), m_FGColor);
			SDLVERIFY(surface);
			SDL_DestroyTexture(m_Texture); // free previous texture
			m_Texture = SDL_CreateTextureFromSurface(Renderer::getRenderer(), surface);
			SDLVERIFY(m_Texture);
			regenerateProperties();
			SDL_FreeSurface(surface);
		}

		void regenerateProperties()
		{
			TTF_SizeText(m_Font, m_Text.c_str(), &m_RenderWidth, &m_RenderHeight);
		}

	};

	// behaves like button, reveals another UI element upon interaction
	// is probably gonna get removed
	class Dropdown : public Button
	{
	public:
		void showPartner()
		{
			m_PartnerUI->Show();
		}
		void hidePartner()
		{
			m_PartnerUI->Hide();
		}
		void togglePartner()
		{
			m_PartnerUI->toggleVisibility();
		}

		virtual void onClick() override
		{
			togglePartner();
		}

	protected:
		// UI to show/hide
		UI* m_PartnerUI;


	};

	class ProgressBar : public UI
	{
	public:
		virtual void init(int px, int py, int pw, int ph, const char* p_TexturePath, const std::deque<UI*> p_Children, const void* p_BottomTexturePath) override
		{
			baseInit(px, py, pw, ph, {});

			m_BaseWidth = pw;
			m_BaseHeight = ph;

			m_Texture = Renderer::loadTexture(p_TexturePath);
			const char* path = (const char*)(p_BottomTexturePath);
			m_BottomTexture = Renderer::loadTexture(path);

		}

		void setTextureBaseSize(int bw, int bh)
		{
			m_BaseWidth = bw;
			m_BaseHeight = bh;
		}
		void setProgress(float p_Progress) { m_Progress = p_Progress; m_Progress = clamp<float>(0.f, 1.f, m_Progress); }
		float getProgress() { return m_Progress; }

		void UIRender() override
		{
			if (m_Visible)
			{
				SDL_Rect toprectsrc =
				{
					0.f,
					0.f,
					ceil(float(m_BaseWidth) * m_Progress),
					m_BaseHeight
				};
				SDL_Rect toprectdst =
				{
					x,
					y,
					ceil(float(w) * m_Progress),
					h
				};

				std::cout << float(w) * m_Progress << "\n";

				SDL_Rect rect = TORECT;

				SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_BottomTexture, NULL, &rect));
				SDLCall(SDL_RenderCopy(Renderer::getRenderer(), m_Texture, &toprectsrc, &toprectdst));
			}

		}



	protected:
		SDL_Texture* m_BottomTexture;
		float m_Progress = 0.f;

		int m_BaseWidth;
		int m_BaseHeight;

	};


}
