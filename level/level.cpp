#include "../include.hpp"

Level::Level() {
	fog = 300;
	xSpawn = -1;
	ySpawn = -1;
}

Level::~Level() {
}

void Level::Init(Game* game, std::string name, std::string file) {
	this->game = game;
	player = game->player;
	
	Bitmap* bitmap = LoadBitmap(file);
	width = bitmap->width;
	height = bitmap->height;
	blocks = new Block*[width * height];
	
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			unsigned int col = bitmap->pixels[x + y * width];
			int id = 0xff - ((bitmap->pixels[x + y * width] >> 24) & 0xff);
			
			Block* block = createBlock(col);
			block->id = id;
			block->x = x;
			block->y = y;
			block->level = this;
			
			if (block->tex == -1) block->tex = wallTex;
			if (block->floorTex == -1) block->floorTex = floorTex;
			if (block->ceilTex == -1) block->ceilTex = ceilTex;
			if (block->col == -1) block->col = wallCol;
			if (block->floorCol == -1) block->floorCol = floorCol;
			if (block->ceilCol == -1) block->ceilCol = ceilCol;
			
			blocks[x + y * width] = block;
		}
	}
	
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Block* block = getBlock(x, y);
			decorateBlock(block, bitmap->pixels[x + y * width]);
		}
	}
	
	std::cout << "Initialized level " << name << " size " << width << "x" << height << std::endl;
}

void Level::Tick() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			blocks[x + y * width]->tick();
		}
	}
	
	for (Emitter*& emitter : emitters) {
		emitter->Tick();
		
		if (emitter->ToRemove()) {
			RemoveEmitter(emitter);
		}
	}
}

Block* Level::createBlock(unsigned int col) {
	col = col & 0xffffff;
	if (col == 0x000000) return new SolidBlock();
	if (col == 0x000080) return new BrokenBlock();
	if (col == 0xff0000) return new DoorBlock();
	return new Block();
}

void Level::decorateBlock(Block* block, unsigned int col) {
	col = col & 0xffffff;
	if (col == 0xffff00) {
		xSpawn = block->x;
		ySpawn = block->y;
	}
}

Block* Level::getBlock(int x, int y) {
	if (blocks == nullptr) return nullptr;
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return nullptr;
	}
	
	return blocks[x + y * width];
}

void Level::AddEntity(Entity* entity) {
	if (std::find(entities.begin(), entities.end(), entity) == entities.end()) {
		entity->level = this;
		entities.push_back(entity);
	}
}

void Level::AddEmitter(Emitter* e) {
	if (std::find(emitters.begin(), emitters.end(), e) == emitters.end()) {
		emitters.push_back(e);
	}
}

void Level::RemoveEmitter(Emitter* e) {
	std::vector<Emitter*>::const_iterator it = std::find(emitters.begin(), emitters.end(), e);
	if (it != emitters.end()) {
		emitters.erase(it);
		
		delete e;
		
		std::cout << "An emitter died out :(" << std::endl;
	}
}

bool Level::ContainsBlockingEntity(double x1, double z1, double x2, double z2) {
	int xc = (int) floor((x1 + x2) / 2);
	int zc = (int) floor((z1 + z2) / 2);
	double r = 2.0;
	for (int x = xc - r; x <= xc + r; x++) {
		for (int z = zc - r; z <= zc + r; z++) {
			Block* block = getBlock(x, z);
			for (Entity* e : block->entities) {
				if (e->isInside(x1, z1, x2, z2)) return true;
			}
		}
	}
	
	return false;
}
