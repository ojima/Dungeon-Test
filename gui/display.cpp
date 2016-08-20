#include "../include.hpp"

Display::Display(int width, int height) : Bitmap(width, height) {
	viewport = new Bitmap3D(width, height);
}

Display::~Display() {
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
