#include "../include.hpp"

Player::Player() {
	r = 0.3;
	bob = 0.0;
	useTime = 0;
}

Player::~Player() {
}

void Player::Tick(bool up, bool down, bool left, bool right, bool turnLeft, bool turnRight) {
	if (useTime > 0) useTime--;	
	
	double rotSpeed = 0.05;
	double walkSpeed = 0.03 * getWalkSpeed();
	
	if (turnLeft) rota += rotSpeed;
	if (turnRight) rota -= rotSpeed;
	
	double xm = 0, zm = 0;
	if (up) zm--;
	if (down) zm++;
	if (left) xm--;
	if (right) xm++;
	
	double dd = xm * xm + zm * zm;
	if (dd > 0) dd = sqrt(dd);
	else dd = 1;
	
	xm /= dd;
	zm /= dd;
	
	bob = bob * 0.6 + sqrt(xm * xm + zm * zm);
	turnBob = turnBob * 0.8 + rota;
	bobPhase = bobPhase + sqrt(xm * xm + zm * zm) * getWalkSpeed();
	
	xa -= walkSpeed * (xm * cos(rot) + zm * sin(rot));
	za -= walkSpeed * (zm * cos(rot) - xm * sin(rot));
	
	Move();
	
	double f = getFriction();
	xa *= f;
	za *= f;
	rot += rota;
	rota *= 0.4;
}

void Player::Activate() {
	if (useTime > 0) return;
	
	useTime = 10;
	
	double range = 2.0;
	double xa = range * sin(rot);
	double za = range * cos(rot);
	
	/*
	int xc = (int) (x + 0.5);
	int zc = (int) (z + 0.5);
	
	int rr = (int) (range + 1.0);
	*/ // TODO: add surrounding enemies to list of possible hits
	std::vector<Entity*> possibleHits;
	
	int steps = 100;
	for (int i = 0; i < steps; i++) {
		double xx = x + xa * i / steps;
		double zz = z + za * i / steps;
		
		for (Entity* e : possibleHits) {
			if (e->contains(xx, zz)) {
				if (e->use(this)) {
					return;
				}
			}
		}
		
		int xt = (int) (xx + 0.5);
		int zt = (int) (zz + 0.5);
		if (xt != (int) (x + 0.5) || zt != (int) (z + 0.5)) {
			Block* block = level->getBlock(xt, zt);
			if (block != nullptr) {
				if (block->use()) return;
				if (block->blocks(this)) return;
			}
		}
	}
}

double Player::getWalkSpeed() {
	return 1.0;
}
