#ifndef ENTITY_HPP
#define ENTITY_HPP

class Level;

class Entity {
	public:
		Entity();
		virtual ~Entity();
		
		void Tick();
	
	public:
		double x, y, rot;
		double xa, ya, rota;
		double r = 0.4;
		Level* level;
		
		bool contains(double xx, double yy);
		bool use(Entity* source);
		
	protected:
		void Move();
		
	public:
		bool isFree(double xx, double yy);
		bool isInside(double x1, double y1, double x2, double y2);
	
	protected:
		double getFriction();
};

#endif // ENTITY_HPP
