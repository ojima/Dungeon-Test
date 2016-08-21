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
	
	double xm = 0, ym = 0;
	if (up) ym--;
	if (down) ym++;
	if (left) xm--;
	if (right) xm++;
	
	double dd = xm * xm + ym * ym;
	if (dd > 0) dd = sqrt(dd);
	else dd = 1;
	
	xm /= dd;
	ym /= dd;
	
	bob = bob * 0.6 + sqrt(xm * xm + ym * ym);
	turnBob = turnBob * 0.8 + rota;
	bobPhase = bobPhase + sqrt(xm * xm + ym * ym) * getWalkSpeed();
	
	xa -= walkSpeed * (xm * cos(rot) + ym * sin(rot));
	ya -= walkSpeed * (ym * cos(rot) - xm * sin(rot));
	
	Move();
	
	double f = getFriction();
	xa *= f;
	ya *= f;
	rot += rota;
	rota *= 0.4;
}

void Player::Activate() {
	if (useTime > 0) return;
	
	useTime = 10;
	
	double range = 2.0;
	double xa = range * sin(rot);
	double ya = range * cos(rot);
	
	/*
	int xc = (int) (x + 0.5);
	int zc = (int) (z + 0.5);
	
	int rr = (int) (range + 1.0);
	*/ // TODO: add surrounding enemies to list of possible hits
	std::vector<Entity*> possibleHits;
	
	int steps = 100;
	for (int i = 0; i < steps; i++) {
		double xx = x + xa * i / steps;
		double yy = y + ya * i / steps;
		
		for (Entity* e : possibleHits) {
			if (e->contains(xx, yy)) {
				if (e->use(this)) {
					return;
				}
			}
		}
		
		int xt = (int) (xx + 0.5);
		int yt = (int) (yy + 0.5);
		if (xt != (int) (x + 0.5) || yt != (int) (y + 0.5)) {
			Block* block = level->getBlock(xt, yt);
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
