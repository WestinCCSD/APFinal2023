#include "Game.h"
#include "World.h"
#include "ProvinceUI.h"
#include "Player.h"
#include "ResearchUI.h"

int main(int argc, char** args) {
	Game game = Game();
	const Countries& countries = Countries::get();
	World world(50, 50, 70);
	game.linkObject(&world);
	Player player;
	game.linkObject(&player);
	ResearchUI researchui;
	game.linkObject(&researchui);


	while (game.isRunning())
	{
		game.renderClear();
		game.handleEvents(&world);
		game.renderAll();
		ProvinceUI::ProvinceUIHandle();
		researchui.Render();
		game.renderPresent();
	}

	return 0;
}