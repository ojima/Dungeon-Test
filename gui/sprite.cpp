#include "sprite.hpp"

Sprite::Sprite(double x, double y, double z, int tex, int col) {
	this->x = x;
	this->y = y;
	this->z = z;
	
	this->tex = tex;
	this->col = col;
	this->remove = false;
}

Sprite::~Sprite() {
}

void Sprite::Tick() {
}
