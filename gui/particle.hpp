#ifndef PARTICLE_HPP
#define PARTICLE_HPP

class Bitmap3D;
class Emitter;
class Particle;

#include <vector>

class Particle {
		friend class Emitter;
	public:
		Particle(double x, double y, double z);
		~Particle();
		
		void Tick();
		
	protected:
		double x, y, z;
		double xa, ya, za;
		double mass;
		int life;
		int tex;
		unsigned int col;
};

enum EmitterType {
	EMITTER_BURST,
	EMITTER_STREAM,
	EMITTER_RAIN,
};

class Emitter {
		friend class Particle;
	public:
		Emitter(EmitterType type, double x, double y, double z, int tex, unsigned int col);
		~Emitter();
		
	public:
		void Emit();
		void EndEmit();
		void Tick();
		void Render(Bitmap3D* target);
		
	protected:
		EmitterType type;
		std::vector<Particle*> particles;
		
		double x, y, z;
		double dx, dy, dz;
		
		double vx, vy, vz;
		double dvx, dvy, dvz;
		
		int tex;
		unsigned int col;
		
		unsigned int count;
		int life, dlife;
		double mass, dmass;
		
		bool emit, emitted;
		
	public:
		inline void SetPosSpread(double dx, double dy, double dz) {
			this->dx = dx;
			this->dy = dy;
			this->dz = dz;
		}
		
		inline void SetVelInitial(double vx, double vy, double vz) {
			this->vx = vx;
			this->vy = vy;
			this->vz = vz;
		}
		
		inline void SetVelSpread(double dvx, double dvy, double dvz) {
			this->dvx = dvx;
			this->dvy = dvy;
			this->dvz = dvz;
		}
		
		inline void SetMassSpread(double mass, double dmass) {
			this->mass = mass;
			this->dmass = dmass;
		}
		
		void SetCountSpread(int mcount, int dcount);
		
		inline void SetLifespanSpread(int life, int dlife) {
			this->life = life;
			this->dlife = dlife;
		}
		
		inline bool ToRemove() {return emitted && !particles.size();}
};

#endif // PARTICLE_HPP
