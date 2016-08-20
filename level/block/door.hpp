#ifndef DOOR_HPP
#define DOOR_HPP

class Entity;

#include "block.hpp"

class DoorBlock : public SolidBlock {
	public:
		DoorBlock();
		~DoorBlock();
		
		bool use();
		void tick();
		
		bool blocks(Entity* entity);
		
	public:
		bool open;
		double openness;
};

#endif // DOOR_HPP
