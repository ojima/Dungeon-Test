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
		void Quit();
		
	public:
		Level* level;
		Player* player;
		Menu* menu;
		
		unsigned char volume, musicvolume;
		bool retromode;
		bool isRunning;
};

#endif // GAME_HPP
