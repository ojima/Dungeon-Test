#include "../include.hpp"

Display::Display(int width, int height) : Bitmap(width, height) {
	test = new Bitmap(64, 64);
	for (int i = 0; i < 64 * 64; i++) {
		test->pixels[i] = rand() * ((rand() % 5) / 4);
	}
	
	viewport = new Bitmap3D(width, height);
}

Display::~Display() {
	delete test;
}

void Display::Render(Game* game) {
	Fill(0, 0, width, height, 0x000000);
	
	if (game->menu == nullptr) {
		Level* level = game->level;
		if (level == nullptr) return;
		viewport->Render(game);
		viewport->PostProcess(game->level);
	
		Draw(viewport, 0, 0);
	} else {
		game->menu->Render(game, this);
	}
}
