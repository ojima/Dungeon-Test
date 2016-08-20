#ifndef BITMAP3D_HPP
#define BITMAP3D_HPP

class Game;
class Level;

#include "bitmap.hpp"

class Bitmap3D : public Bitmap {
	public:
		Bitmap3D(int width, int height);
		~Bitmap3D();
		
		void Render(Game* game);
		void PostProcess(Level* level);
		
	private:
		void RenderFloor(Level* level);
		void RenderWall(double x1, double y1, double x2, double y2, int tex, int color);
		void RenderWall(double x1, double y1, double x2, double y2, int tex, int color, double xt1, double xt2);
		
	public:
		void RenderSprite(double x, double y, double z, int tex, int color);
		
	private:
		double* zBuffer;
		double* zBufferWall;
		double xCam, yCam, zCam;
		double rCos, rSin, rot;
		double xCenter, yCenter, fov;
};

#endif // BITMAP3D_HPP
