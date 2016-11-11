#include "include.hpp"

Game::Game() {
	player = nullptr;
	level = nullptr;
	menu = nullptr;
	
	volume = 100;
	musicvolume = 80;
	retromode = false;
	
	isRunning = true;
}

Game::~Game() {
}

void Game::Tick(const unsigned char* keys) {
	bool up = keys[SDL_SCANCODE_UP] | keys[SDL_SCANCODE_W];
	bool down = keys[SDL_SCANCODE_DOWN] | keys[SDL_SCANCODE_S];
	
	bool left = keys[SDL_SCANCODE_A];
	bool right = keys[SDL_SCANCODE_D];
	
	bool turnLeft = keys[SDL_SCANCODE_LEFT] | keys[SDL_SCANCODE_Q];
	bool turnRight = keys[SDL_SCANCODE_RIGHT] | keys[SDL_SCANCODE_E];
	
	bool use = keys[SDL_SCANCODE_SPACE];
	bool enter = keys[SDL_SCANCODE_RETURN];
	
	if (menu == nullptr) {
		player->Tick(up, down, left, right, turnLeft, turnRight);
		if (use) {
			player->Activate();
		}
		
		level->Tick();
	} else {
		menu->Tick(this, up, down, left, right, use | enter);
	}
}

void Game::NewGame() {
	level = new Level();
	level->Init(this, "The Prison", "res/level/Prison.png");
	
	player = new Player();
	level->AddEntity(player);
	
	player->x = level->xSpawn + 0.5;
	player->y = level->ySpawn + 0.5;
	player->rot = 0.5;
}

void Game::Quit() {
	isRunning = false;
}
