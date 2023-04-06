#include "Game.h"
#include "Renderer.h"
#include "Debug.h"
#include "World.h"
#include "DeveloperUI.h"

int main(int argc, char** args) {
	Game game = Game();
	const Countries& country = Countries::get();
	World world(50, 50, 70);

	int sliderData[2] = 
	{
		500, 501
	};

	Countries::addCountry(Country({ 255, 0, 0, 255 }, "Player", 0), true);

	UITypes::Slider slider;
	slider.init(0, 640, 96, 96, "assets/art/defaultslider.png", {}, sliderData);

	game.linkObject(&slider);

	while (game.isRunning())
	{
		game.renderClear();
		game.handleEvents(&world);
		world.Render();
		game.renderAll();
	}

	return 0;
}