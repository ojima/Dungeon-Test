#include "../include.hpp"

Entity::Entity() {
}

Entity::~Entity() {
}

void Entity::Tick() {
}

void Entity::Move() {
	int xSteps = (int) abs(xa * 100) + 1;
	for (int i = xSteps; i > 0; i--) {
		double xxa = xa;
		if (isFree(x + xxa * i / xSteps, z)) {
			x += xxa * i / xSteps;
			break;
		} else {
			xa = 0;
		}
	}
	
	int zSteps = (int) abs(za * 100) + 1;
	for (int i = zSteps; i > 0; i--) {
		double zza = za;
		if (isFree(x, z + zza * i / zSteps)) {
			z += zza * i / zSteps;
			break;
		} else {
			za = 0;
		}
	}
}

bool Entity::contains(double xx, double zz) {
	if (x + r <= xx) return false;
	if (x - r >= xx) return false;
	if (z + r <= zz) return false;
	if (z - r >= zz) return false;
	return true;
}

bool Entity::use(Entity* source) {
	if (typeid(source) == typeid(Player*)) return true;
	return false;
}

bool Entity::isFree(double xx, double yy) {
	int x1 = (int) floor(xx + 0.5 - r);
	int x2 = (int) floor(xx + 0.5 + r);
	int y1 = (int) floor(yy + 0.5 - r);
	int y2 = (int) floor(yy + 0.5 + r);
	
	if (level->getBlock(x1, y1)->blocks(this)) return false;
	if (level->getBlock(x2, y1)->blocks(this)) return false;
	if (level->getBlock(x1, y2)->blocks(this)) return false;
	if (level->getBlock(x2, y2)->blocks(this)) return false;
	
	return true;
}

bool Entity::isInside(double x1, double y1, double x2, double y2) {
	if (x + r <= x1) return false;
	if (x - r >= x2) return false;
	if (z + r <= y1) return false;
	if (z - r >= y2) return false;
	return true;
}

double Entity::getFriction() {
	return 0.6;
}
