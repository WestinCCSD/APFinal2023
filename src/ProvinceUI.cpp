#include <sstream>
#include "ProvinceUI.h"
																							  
											
ProvinceUI* ProvinceUI::m_ProvinceUI = NULL;
Tile* ProvinceUI::m_SelectedTile = NULL;	
																	
UITypes::Label* ProvinceUI::m_PopulationLabel = NULL;
UITypes::Label* ProvinceUI::m_PopulationText = NULL;
UITypes::Label* ProvinceUI::m_YieldLabel = NULL;
UITypes::Label* ProvinceUI::m_YieldName = NULL;
UITypes::Label* ProvinceUI::m_TileNameLabel = NULL;	
ComponentButton<ProvinceUI>* ProvinceUI::m_ClaimButton = NULL;
UITypes::ProgressBar* ProvinceUI::m_ClaimProgress = NULL;
ComponentButton<ProvinceUI>* ProvinceUI::m_DevelopButton = NULL;
Tooltip* ProvinceUI::m_Tooltip = NULL;

RNG ProvinceUI::rng{};

ProvinceUI::ProvinceUI()											
{			
	int wh=0;
	Renderer::getWindowProperties(NULL, &wh);
	baseInit(48, wh - 400, 384, 288, {});								
																	
	m_PopulationLabel = new UITypes::Label();						
	m_PopulationLabel->init(0, y + 159, 0, 0, OFFICIALFONT, {}, "0.0");
	m_PopulationLabel->setCenter(155, -1); // center of box on sprite

	m_PopulationText = new UITypes::Label();
	m_PopulationText->init(0, y + 159, 0, 0, OFFICIALFONT, {}, "Population");
	m_PopulationText->setCenter(325, -1);

	m_YieldLabel = new UITypes::Label();
	m_YieldLabel->init(0, y + 225, 0, 0, OFFICIALFONT, {}, "0");
	m_YieldLabel->setCenter(155, -1);

	m_YieldName = new UITypes::Label();
	m_YieldName->init(0, y + 225, 0, 0, OFFICIALFONT, {}, "Resource Name");
	m_YieldName->setCenter(325, -1);

	m_TileNameLabel = new UITypes::Label();
	m_TileNameLabel->init(0, y + 29, 0, 0, OFFICIALFONT, {}, "Tile ");
	m_TileNameLabel->setCenter(245, -1);

	m_ClaimButton = new ComponentButton<ProvinceUI>((*this));
	m_ClaimButton->init(314, y + 101, 60, 30, "assets/art/claimbutton.png", {}, NULL);
	m_ClaimProgress = new UITypes::ProgressBar();
	m_ClaimProgress->init(314, y + 110, 48, 12, "assets/art/progressbartop.png", {}, "assets/art/progressbarbottom.png");
	m_ClaimProgress->Hide();

	m_DevelopButton = new ComponentButton<ProvinceUI>((*this));
	m_DevelopButton->init(314, y + 101, 60, 30, "assets/art/developbutton.png", {}, NULL);

	m_Texture = Renderer::loadTexture("assets/art/povinceUI.png");	
	SDLVERIFY(m_Texture);											

	m_ProvinceUI->rng.Randomize();

}																	

ProvinceUI::~ProvinceUI()
{
	delete m_PopulationLabel;
	delete m_TileNameLabel;
}

const ProvinceUI& ProvinceUI::get()									
{																							  
	if (m_ProvinceUI == NULL)																  
	{
		m_ProvinceUI = new ProvinceUI();
	}
	return (*m_ProvinceUI);
}

void ProvinceUI::ProvinceUIHandle()
{
	if (m_ProvinceUI == NULL)
		m_ProvinceUI = new ProvinceUI();

	m_ProvinceUI->Handle(0.f);

	if (m_ProvinceUI->m_Visible)
	{
		m_ProvinceUI->     UIRender();
		m_PopulationLabel->UIRender();
		m_TileNameLabel->  UIRender();
		m_ClaimButton->    UIRender();
		m_YieldLabel->     UIRender();
		m_YieldName->      UIRender();
		m_PopulationText-> UIRender();
		m_ClaimProgress->  UIRender();
		m_DevelopButton->  UIRender();
	}
	UpdateInfo();
}

void ProvinceUI::UpdateInfo()
{
	if (Tile::getSelectedTile() != 0)
	{
		m_ProvinceUI->Show();
		m_TileNameLabel->Show();

		auto stats = Tile::getSelectedTile()->getStats();

		{
			float pop = float(float((stats[0])) / 1000);
			// ugly code here
			// basically just cleaning up population number
			std::stringstream ss;
			ss << std::to_string(pop);
			std::string string = ss.str().erase(ss.str().size() - 4);
			std::stringstream ss2;
			ss2 << string << "k";

			m_PopulationLabel->queueSetText(ss2.str());

			if (Tile::getSelectedTile()->isHungry())
				m_PopulationText->setColor({ 255, 50, 50, 255 });
			else
				m_PopulationText->setColor({ 255, 255, 255, 255 });

			std::stringstream ss3;
			int id = stats[3];
			ss3 << "Tile " << std::to_string(id);
			m_TileNameLabel->queueSetText(ss3.str());
		}
		{
			int yield = stats[4];
			
			if (yield < 1000)
			{
				m_YieldLabel->queueSetText(std::to_string(yield));
			}
			else
			{
				float yieldf = float(float((stats[4])) / 1000);
				// ugly code here
				// basically just cleaning up yield number
				std::stringstream ss;
				ss << std::to_string(yieldf);
				std::string string = ss.str().erase(ss.str().size() - 4);
				std::stringstream ss2;
				ss2 << string << "k";

				m_YieldLabel->queueSetText(ss2.str());
			}

			const auto& res = Tile::getSelectedTile()->getResource();
			m_YieldName->setText(res.m_Name);

		}

		// progress bar & claim button
		{
			bool claim = Tile::getSelectedTile()->isClaimed();
			bool owned = Tile::getSelectedTile()->isOwned();
			bool developing = Tile::getSelectedTile()->isDeveloping();
			if (claim)
			{
				m_DevelopButton->Hide();
				m_ClaimButton->Hide();
				m_ClaimProgress->Show();
				m_ClaimProgress->setProgress(Tile::getSelectedTile()->getClaimProgress());

			}
			else
			{
				m_DevelopButton->Hide();
				m_ClaimButton->Show();
				m_ClaimProgress->Hide();
			}
			if (owned)
			{
				m_ClaimButton->Hide();

				if (Tile::getSelectedTile()->getOwner() == 1) // tile belongs to player and isn't being developed
				{
					if (developing)
					{
						m_ClaimProgress->Show();
						m_ClaimProgress->setProgress(Tile::getSelectedTile()->getDevelopProgress());
					}
					else
					{
						m_ClaimProgress->Hide();
						m_DevelopButton->Show();
					}
				}

			}
		}

	}
	else
	{
		m_ProvinceUI->Hide();
		m_TileNameLabel->Hide();
	}

}

void ProvinceUI::Handle(float)
{
	m_DevelopButton->UIHandle(0.f);
	m_ClaimButton->UIHandle(0.f);
}

void ProvinceUI::onClick(void* p_Button)
{
	if (p_Button == m_ClaimButton)
	{
		if (Tile::getSelectedTile() != NULL)
		{
			if (!Tile::getSelectedTile()->isClaimed()) // the claim button is unavailable if the tile is being claimed anyway, so this is just a precaution
				Tile::getSelectedTile()->createClaim(1);
			// claim this tile on the behalf of the player
		}
	}
	if (p_Button == m_DevelopButton)
	{
		Tile* tile = Tile::getSelectedTile();
		auto& country = Countries::getCountry(tile->getCountry());
		// todo: check to make sure this tile can develop
		tile->develop();
	}
}

SDL_Rect ProvinceUI::getRect()
{
	SDL_Rect rect = { m_ProvinceUI->x, m_ProvinceUI->y, m_ProvinceUI->w, m_ProvinceUI->h };
	return rect;
}