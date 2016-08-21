#include "../../include.hpp"

Block::Block() {
	tex = -1;
	col = -1;
	
	floorTex = -1;
	floorCol = -1;
	
	ceilTex = -1;
	ceilCol = -1;
	
	id = -1;
	
	isSolid = false;
	renderSolid = false;
}

Block::~Block() {
}

void Block::tick() {
}

bool Block::use() {
	return false;
}

bool Block::blocks(Entity* entity) {
	return isSolid;
}

SolidBlock::SolidBlock() : Block() {
	isSolid = true;
	renderSolid = true;
}

SolidBlock::~SolidBlock() {
}

BrokenBlock::BrokenBlock() : SolidBlock() {
	tex = 1;
}

BrokenBlock::~BrokenBlock() {
}
	
bool BrokenBlock::use() {
	this->tex = -1;
	this->col = -1;
	this->isSolid = false;
	this->renderSolid = false;
	
	Emitter* emitter = new Emitter(EMITTER_BURST, x, y, -0.5, 0, col);
	emitter->SetPosSpread(0.3, 0.3, 0.2);
	emitter->SetVelSpread(0.1, 0.1, 0.2);
	emitter->SetMassSpread(1.0, 0.2);
	emitter->SetCountSpread(30, 10);
	emitter->SetLifespanSpread(60, 10);
	emitter->Emit();
	
	level->AddEmitter(emitter);
	
	return true;
}
