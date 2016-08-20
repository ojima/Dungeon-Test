#ifndef LEVEL_HPP
#define LEVEL_HPP

class Block;
class Entity;
class Game;
class Player;

#include <string>

class Level
{
	public:
		Level();
		virtual ~Level();
		
		void Init(Game* game, std::string name, std::string file);
		void Tick();
		
	protected:
		Game* game;
		Player* player;
		
	public:
		int width, height;
		int xSpawn, ySpawn;
		int fog;
		
	protected:
		Block** blocks;
		int wallCol = 0xB3CEE2;
		int floorCol = 0x9CA09B;
		int ceilCol = 0x9CA09B;
		
		int wallTex = 0;
		int floorTex = 0;
		int ceilTex = 0;
		
		std::vector<Entity*> entities;
		
		Block* createBlock(unsigned int col);
		void decorateBlock(Block* block, unsigned int col);
		
		std::vector<Emitter*> emitters;
		
	public:
		void AddEntity(Entity* entity);
		bool ContainsBlockingEntity(double x1, double z1, double x2, double z2);
		
		void AddEmitter(Emitter* emitter);
		void RemoveEmitter(Emitter* emitter);
		
		inline std::vector<Emitter*> GetEmitters() {return this->emitters;}
	
		Block* getBlock(int x, int y);
};

#endif // LEVEL_HPP
