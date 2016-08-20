#include "../../include.hpp"

static const double openlimit = 7.0 / 8.0;

DoorBlock::DoorBlock() : SolidBlock() {
	open = false;
	openness = 0.0;
	tex = 4;
	renderSolid = false;
	isSolid = true;
}

DoorBlock::~DoorBlock() {
}

bool DoorBlock::use() {
	open = !open;
	return true;
}

void DoorBlock::tick() {
	if (open) {
		openness += 0.2;
		
		if (openness > 1.0) {
			openness = 1.0;
		}
	} else {
		openness -= 0.2;
		
		if (openness < 0.0) {
			openness = 0.0;
		}
	}
	
	if (openness < openlimit && !open && !isSolid) {
		if (level->ContainsBlockingEntity(x - 0.5, y - 0.5, x + 0.5, y + 0.5)) {
			openness = openlimit;
			return;
		}
	}
	
	isSolid = (openness < openlimit);
}

bool DoorBlock::blocks(Entity* entity) {
	if (openness >= openlimit) {
		if (dynamic_cast<Player*>(entity)) return isSolid;
	}
	
	return true;
}
