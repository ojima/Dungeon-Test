#include "../include.hpp"

Menu::Menu() {
}

Menu::~Menu() {
}
		
void Menu::Render(Game* game, Bitmap* target) {
}

void Menu::Tick(Game* game, bool up, bool down, bool left, bool right, bool enter) {
}

TitleMenu::TitleMenu() {
	selected = 0;
	submenu = 0;
}

TitleMenu::~TitleMenu() {
}

void TitleMenu::Render(Game* game, Bitmap* target) {
	static std::vector<std::string> selections = { "New Game", "Load Game", "Options", "Credits", "Exit" };
	static std::vector<std::string> options = { "Sound", "Music", "Retro Mode", "Return" };
	
	switch (submenu) {
		case 0:
			for (unsigned i = 0; i < selections.size(); i++) {
				if (i == selected) {
					target->RenderText("> " + selections[i] + " <", 2, 2 + 10 * i, 0xffffff);
				} else {
					target->RenderText(selections[i], 2, 2 + 10 * i, 0xa0a0a0);
				}
			}
		break;
	
		case 1:
			for (unsigned i = 0; i < options.size(); i++) {
				std::string text = options[i];
				if (i == 0) text = text + ": " + std::to_string(game->volume);
				if (i == 1) text = text + ": " + std::to_string(game->musicvolume);
				if (i == 2) {
					if (game->retromode) text = text + ": On";
					else text = text + ": Off";
				}
				
				if (i == selected) {
					target->RenderText("> " + text + " <", 2, 2 + 10 * i, 0xffffff);
				} else {
					target->RenderText(text, 2, 2 + 10 * i, 0xa0a0a0);
				}
			}
		break;
	}
	
	target->RenderText("Copyright (c) 2015-2016 - Ojima", 2, target->height - 10, 0x303030);
}

void TitleMenu::Tick(Game* game, bool up, bool down, bool left, bool right, bool enter) {
	static int timeout = 0;	
	if (timeout > 0) timeout--;
	if (!enter && !up && !down && !left && !right) timeout = 0;
	
	// Selection adjustment
	unsigned int maxOption = 5;
	
	if (up && !timeout) {
		if (selected == 0) selected = maxOption - 1;
		else selected--;
		
		timeout = 20;
	} else if (down && !timeout) {
		if (selected == maxOption - 1) selected = 0;
		else selected++;
		
		timeout = 20;
	}
	
	// Volume adjustment
	if (submenu == 1) {
		if (selected == 0) {
			if (left && game->volume > 0) game->volume--;
			if (right && game->volume < 100) game->volume++;
		}
		
		if (selected == 1) {
			if (left && game->musicvolume > 0) game->musicvolume--;
			if (right && game->musicvolume < 100) game->musicvolume++;
		}
	}
	
	// Execute selection
	if (enter && !timeout) {
		switch (submenu) {
			case 0:
				// Main menu
				switch (selected) {
					case 0:
						game->menu = nullptr;
						game->NewGame();
						delete this;
					break;
					
					case 2:
						selected = 0;
						submenu = 1;
					break;
				}
			break;
			
			case 1:
				// Options menu
				if (selected == 2) game->retromode = !game->retromode;
				if (selected == 3) {
					submenu = 0;
					selected = 0;
				}
			break;
		}
		
		timeout = 20;
	}
}
