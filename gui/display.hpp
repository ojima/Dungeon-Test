#ifndef DISPLAY_HPP
#define DISPLAY_HPP

class Bitmap3D;
class Game;

#include <string>
#include "bitmap.hpp"

class Display : public Bitmap {
	private:
		Bitmap* test;
		Bitmap3D* viewport;
		
	public:
		Display(int width, int height);
		~Display();
		
		void Render(Game* game);
};

#endif // DISPLAY_HPP
