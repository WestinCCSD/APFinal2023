#include "Tooltip.h"
#include "Tile.h"
#pragma once

template <class T>
class ComponentButton : public UITypes::Button
{
public:
	ComponentButton(T& p_Parent)
		: m_Parent(p_Parent)
	{}

	virtual void onClick() override
	{
		m_Parent.onClick(this);
	}

private:
	T& m_Parent;

};

// singleton for provinceui class
class ProvinceUI : public UI
{
public:
	ProvinceUI(const ProvinceUI&) = delete;
	static const ProvinceUI& get();
	// special handling function for this class only
	static void ProvinceUIHandle();
	void Handle(float) override;
	

	void onClick(void*);

	static SDL_Rect getRect();
	static bool isVisible() { return m_ProvinceUI->m_Visible; }

private:
	ProvinceUI();
	~ProvinceUI();
	static void UpdateInfo();

	static ProvinceUI* m_ProvinceUI;
	static Tile* m_SelectedTile;

	static UITypes::Label* m_PopulationLabel;
	static UITypes::Label* m_PopulationText;
	static UITypes::Label* m_YieldLabel;
	static UITypes::Label* m_YieldName;
	static UITypes::Label* m_TileNameLabel;
	static ComponentButton<ProvinceUI>* m_ClaimButton;
	static UITypes::ProgressBar* m_ClaimProgress;
	static ComponentButton<ProvinceUI>* m_DevelopButton;
	static Tooltip* m_Tooltip;

	static RNG rng;

};