#ifndef SPRITE_HPP
#define SPRITE_HPP

class Sprite {
	public:
		Sprite(double x, double y, double z, int tex, int col);
		virtual ~Sprite();
		
		void Tick();
		
	public:
		double x, y, z;
		int tex, col;
		bool remove;
};

#endif // SPRITE_HPP
