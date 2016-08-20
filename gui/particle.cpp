#include "../include.hpp"

static double randDouble() {
	return 1.0 * std::rand() / RAND_MAX;
}

static double gaussian() {
	static bool haveNext = false;
	static double next = 0.0;
	
	if (haveNext) {
		haveNext = false;
		return next;
	} else {
		double v1, v2, s;
		do {
			v1 = 2.0 * randDouble() - 1.0;
			v2 = 2.0 * randDouble() - 1.0;
			s = v1 * v1 + v2 * v2;
		} while (s >= 1 || s == 0);
		
		double m = sqrt(-2.0 * std::log(s) / s);
		next = v2 * m;
		haveNext = true;
		return v1 * m;
	}
}

Particle::Particle(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
	
	this->xa = 0.0;
	this->ya = 0.0;
	this->za = 0.0;
	
	this->life = 60;
	
	this->mass = 1.0;
	
	this->tex = 2;
	this->col = 0x555555;
}

Particle::~Particle() {
}
		
void Particle::Tick() {
	if (life > 0) life--;
	
	x += xa;
	y += ya;
	z += za;
	
	ya -= 0.1 * mass;
	if (y < 0.0) {
		y = 0.0;
		xa *= 0.8;
		za *= 0.8;
	}
}

Emitter::Emitter(EmitterType type, double x, double y, double z, int tex, unsigned int col) {
	this->type = type;
	
	this->x = x;
	this->y = y;
	this->z = z;
	
	this->tex = tex;
	this->col = col;
	
	this->dx = 0.0;
	this->dy = 0.0;
	this->dz = 0.0;
	
	this->count = 0;
	
	this->life = 60;
	this->dlife = 60;
	
	this->mass = 1.0;
	this->dmass = 0.5;
	
	this->emit = false;
	this->emitted = false;
}

Emitter::~Emitter() {
	while (!particles.empty()) {
		Particle* temp = particles.back();
		particles.pop_back();
		delete temp;
	}
}

void Emitter::Emit() {
	emit = true;
	emitted = true;
	
	std::cout << "Poof! I shall emit " << count << " particles :)" << std::endl;
}

void Emitter::EndEmit() {
	emit = false;
}

void Emitter::Tick() {
	while (emit && particles.size() < count) {
		double xx = x + dx * gaussian();
		double yy = y + dy * gaussian();
		double zz = z + dz * gaussian();
		
		Particle* particle = new Particle(xx, yy, zz);
		
		particle->tex = tex;
		particle->col = col;
		
		particle->mass = mass + dmass * gaussian();
		particle->life = life + dlife * gaussian();
		
		particles.push_back(particle);
	}
	
	if (type == EMITTER_BURST) emit = false;
	
	for (std::vector<Particle*>::const_iterator i = particles.begin(); i != particles.end(); i++) {
		Particle* particle = *i;
		particle->Tick();
		
		if (!particle->life) {
			particles.erase(i);
			delete particle;
			i--;
		}
	}
}

void Emitter::Render(Bitmap3D* target) {
	for (Particle*& particle : particles) {
		target->RenderSprite(particle->x, particle->y, particle->z, particle->tex, particle->col);
	}
}

void Emitter::SetCountSpread(int mcount, int dcount) {
	int tcount = -1;
	do {
		tcount = mcount + dcount * gaussian();
	} while (tcount < 0);
	
	count = (unsigned int) tcount;
}
