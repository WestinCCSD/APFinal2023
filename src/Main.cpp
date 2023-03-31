#include "Game.h"
#include "Renderer.h"
#include "Debug.h"

#include "World.h"
#include "UI.h"

int main(int argc, char** args) {
	Game game = Game();

	World world(50, 50, 70);

	while (game.is_running())
	{
		game.renderClear();
		game.handleEvents(&world);
		world.Render();
		game.renderAll();
	}

	return 0;
}