#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity {
	public:
		Player();
		~Player();
		
		void Tick(bool up, bool down, bool left, bool right, bool turnLeft, bool turnRight);
		void Activate();
		
	public:
		double bob, bobPhase, turnBob;
		
	private:
		int useTime;
	
		double getWalkSpeed();
};

#endif // PLAYER_HPP
