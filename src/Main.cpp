#include "Game.h"
#include "World.h"
#include "ProvinceUI.h"
#include "Player.h"
#include "ResearchUI.h"

int main(int argc, char** args) {
	Game game = Game();
	const Countries& countries = Countries::get();
	World world(50, 50, 70);
	Player player;
	game.linkObject(&world);
	game.linkObject(&player);

	while (game.isRunning())
	{
		game.renderClear();
		game.handleEvents(&world);
		game.renderAll();
		ProvinceUI::ProvinceUIHandle();
		game.renderPresent();
	}

	return 0;
}