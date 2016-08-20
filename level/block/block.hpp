#ifndef BLOCK_HPP
#define BLOCK_HPP

class Entity;
class Level;

#include <vector>

class Block {
	public:
		Block();
		virtual ~Block();
		
		virtual bool blocks(Entity* entity);
		
		virtual void tick();
		virtual bool use();
		
	public:
		Level* level;
		int x, y;
		int id;
		int tex, col;
		int floorTex, floorCol;
		int ceilTex, ceilCol;
		bool isSolid, renderSolid;
		std::vector<Entity*> entities;
	
	public:
		void AddEntity(Entity* entity) {
			if (std::find(entities.begin(), entities.end(), entity) == entities.end()) {
				entities.push_back(entity);
			}
		}
};

class SolidBlock : public Block {
	public:
		SolidBlock();
		virtual ~SolidBlock();
};

class BrokenBlock : public SolidBlock {
	public:
		BrokenBlock();
		~BrokenBlock();
	
		bool use();
};

#endif // BLOCK_HPP
