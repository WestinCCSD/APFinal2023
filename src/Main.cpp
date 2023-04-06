#include "Game.h"
#include "Renderer.h"
#include "Debug.h"
#include "World.h"
#include "DeveloperUI.h"

int main(int argc, char** args) {
	Game game = Game();
	const Countries& country = Countries::get();
	World world(50, 50, 70);

	UITypes::Button button;
	button.init(0, 0, 50, 50, "assets/art/testbutton.png", {});
	
	game.linkObject(&button);

	while (game.isRunning())
	{
		game.renderClear();
		game.handleEvents(&world);
		world.Render();
		button.Render();
		game.renderAll();
	}

	return 0;
}