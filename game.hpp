#ifndef GAME_HPP
#define GAME_HPP

class Display;
class Emitter;
class Level;
class Menu;
class Player;

#include <vector>

class Game {
		friend class Display;
	public:
		Game();
		~Game();
		
		void Tick(const unsigned char* keys);
		
		void NewGame();
		
	public:
		Level* level;
		Player* player;
		Menu* menu;
		
		unsigned char volume, musicvolume;
		bool retromode;
};

#endif // GAME_HPP
