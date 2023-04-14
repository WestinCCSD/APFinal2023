#include "Tooltip.h"
#include "Tile.h"
#pragma once

template <class T>
class ClaimButton : public UITypes::Button
{
public:
	ClaimButton(T& p_Parent)
		: m_Parent(p_Parent)
	{}

	virtual void onClick() override
	{
		m_Parent.claimClicked();
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
	

	void claimClicked();

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
	static ClaimButton<ProvinceUI>* m_ClaimButton;
	static UITypes::ProgressBar* m_ClaimProgress;
	static Tooltip* m_Tooltip;

};