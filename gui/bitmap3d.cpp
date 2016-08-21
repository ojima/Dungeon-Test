#include "../include.hpp"

#define PI 3.141592653589793

Bitmap3D::Bitmap3D(int width, int height) : Bitmap(width, height) {
	zBuffer = new double[width * height];
	zBufferWall = new double[width];
}

Bitmap3D::~Bitmap3D() {
	delete zBuffer;
	delete zBufferWall;
}

void Bitmap3D::Render(Game* game) {
	for (int i = 0; i < width; i++) zBufferWall[i] = 0;
	for (int i = 0; i < width * height; i++) zBuffer[i] = 10000;
	
	rot = game->player->rot;
	rSin = sin(rot);
	rCos = cos(rot);
	
	xCam = game->player->x - 0.3 * rSin;
	yCam = game->player->z - 0.3 * rCos;
	zCam = -0.2 + sin(game->player->bobPhase * 0.4) * 0.02 * game->player->bob;
	
	xCenter = width / 2.0;
	yCenter = height / 3.0;
	fov = height;
	
	Level* level = game->level;
	
	int xo = floor(xCam);
	int zo = floor(yCam);
	int r = 6;
	
	for (int xb = xo - r; xb <= xo + r; xb++) {
		for (int zb = zo - r; zb <= zo + r; zb++) {
			Block* c = level->getBlock(xb, zb);
			Block* e = level->getBlock(xb + 1, zb);
			Block* s = level->getBlock(xb, zb + 1);
			
			if (c == nullptr) continue;
			
			if (DoorBlock* d = dynamic_cast<DoorBlock*>(c)) {
				double rr = 1.0 / 8.0;
				double openness = 1.0 - d->openness * 7.0 / 8.0;
				
				if (e->renderSolid) {
					RenderWall(xb + openness, zb + 0.5 - rr, xb, zb + 0.5 - rr, c->tex, (c->col & 0xfefefe) >> 1, 0, openness);
					RenderWall(xb, zb + 0.5 + rr, xb + openness, zb + 0.5 + rr, c->tex, (c->col & 0xfefefe) >> 1, openness, 0);
					RenderWall(xb + openness, zb + 0.5 + rr, xb + openness, zb + 0.5 - rr, c->tex, c->col, 0.5 - rr, 0.5 + rr);
				} else {
					RenderWall(xb + 0.5 - rr, zb, xb + 0.5 - rr, zb + openness, c->tex, c->col, openness, 0);
					RenderWall(xb + 0.5 + rr, zb + openness, xb + 0.5 + rr, zb, c->tex, c->col, 0, openness);
					RenderWall(xb + 0.5 - rr, zb + openness, xb + 0.5 + rr, zb + openness, c->tex, (c->col & 0xfefefe) >> 1, 0.5 - rr, 0.5 + rr);
				}
			}
			
			if (c->renderSolid) {
				if (!e->renderSolid) {
					RenderWall(xb + 1, zb + 1, xb + 1, zb, c->tex, c->col);
				}
				
				if (!s->renderSolid) {
					RenderWall(xb, zb + 1, xb + 1, zb + 1, c->tex, (c->col & 0xfefefe) >> 1);
				}
			} else {
				if (e->renderSolid) {
					RenderWall(xb + 1, zb, xb + 1, zb + 1, e->tex, e->col);
				}
				
				if (s->renderSolid) {
					RenderWall(xb + 1, zb + 1, xb, zb + 1, s->tex, (s->col & 0xfefefe) >> 1);
				}
			}
		}
	}
	
	RenderFloor(level);
	
	for (Emitter* emitter : level->GetEmitters()) {
		emitter->Render(this);
	}
}

void Bitmap3D::PostProcess(Level* level) {
	for (int i = 0; i < width * height; i++) {
		double zl = zBuffer[i];
		if (zl < 0) {
			// render sky
			//int xx = (int) (floor(i % width - rot * SkyTex->width / (2 * PI))) & 0x1ff;
			//int yy = i / width;
			//pixels[i] = SkyTex->pixels[xx + yy * SkyTex->width];
		} else {
			int xp = (i % width);
			int yp = (i / width) * 14;
			double xx = ((i % width - width / 2.0) / width);
			
			int brightness = (int) (level->fog - zl * 6 * (xx * xx * 2 + 1));
			brightness = (brightness + ((xp + yp) & 3) * 4) >> 4 << 4;
			
			if (brightness < 0) brightness = 0;
			if (brightness > 255) brightness = 255;
			
			unsigned int col = pixels[i];
			unsigned int r = (col >> 16) & 0xff;
			unsigned int g = (col >> 8) & 0xff;
			unsigned int b = (col) & 0xff;
			
			r = (r * brightness) / 255;
			g = (g * brightness) / 255;
			b = (b * brightness) / 255;
			
			pixels[i] = (r << 16) | (g << 8) | (b);
		}
	}
}

void Bitmap3D::RenderFloor(Level* level) {
	for (int y = 0; y < height; y++) {
		double yd = ((y + 0.5) - yCenter) / fov;
		bool flr = true;
		
		double zd = (4 - zCam * 8) / yd;
		if (yd < 0) {
			flr = false;
			zd = -(4 + zCam * 8) / yd;
		}
		
		for (int x = 0; x < width; x++) {
			if (zBuffer[x + y * width] <= zd) continue;
			
			double xd = (xCenter - x) / fov;
			xd *= zd;
			
			double xx = xd * rCos + zd * rSin + (xCam + 0.5) * 8;
			double yy = zd * rCos - xd * rSin + (yCam + 0.5) * 8;
			
			int xPix = (int) (xx * 4);
			int yPix = (int) (yy * 4);
			int xTile = xPix >> 5;
			int yTile = yPix >> 5;
			
			Block* block = level->getBlock(xTile, yTile);
			
			if (block == nullptr) {
				zBuffer[x + y * width] = zd;
				pixels[x + y * width] = 0xff000000;
				continue;
			}
			
			int col = block->floorCol;
			int tex = block->floorTex;
			if (!flr) {
				col = block->ceilCol;
				tex = block->ceilTex;
			}
			
			if (tex < 0) {
				zBuffer[x + y * width] = -1;
			} else {
				int xSrc = (xPix & 0x1F) + (tex % 8) * 32;
				int ySrc = (yPix & 0x1F) + (tex / 8) * 32;
				
				zBuffer[x + y * width] = zd;
				unsigned int pix = MixColors(FloorTex->pixels[xSrc + ySrc * FloorTex->width], col);
				pixels[x + y * width] = pix;
			}
		}
	}
}

void Bitmap3D::RenderWall(double x1, double y1, double x2, double y2, int tex, int color) {
	RenderWall(x1, y1, x2, y2, tex, color, 0.0, 1.0);
}

void Bitmap3D::RenderWall(double x1, double y1, double x2, double y2, int tex, int color, double xt1, double xt2) {
	double xc1 = ((x1 - 0.5) - xCam) * 2.0;
	double yc1 = ((y1 - 0.5) - yCam) * 2.0;
	
	double xc2 = ((x2 - 0.5) - xCam) * 2.0;
	double yc2 = ((y2 - 0.5) - yCam) * 2.0;
	
	double xx1 = xc1 * rCos - yc1 * rSin;
	double zz1 = yc1 * rCos + xc1 * rSin;
	
	double xx2 = xc2 * rCos - yc2 * rSin;
	double zz2 = yc2 * rCos + xc2 * rSin;
	
	double u = ((-0.5) - zCam) * 2.0;
	double l = ((+0.5) - zCam) * 2.0;
	
	xt1 *= 16.0;
	xt2 *= 16.0;
	double zClip = 0.2;
	
	if (zz1 < zClip && zz2 < zClip) return;
	
	if (zz1 < zClip) {
		double p = (zClip - zz1) / (zz2 - zz1);
		zz1 = zz1 + (zz2 - zz1) * p;
		xx1 = xx1 + (xx2 - xx1) * p;
		xt1 = xt1 + (xt2 - xt1) * p;
	}
	
	if (zz2 < zClip) {
		double p = (zClip - zz1) / (zz2 - zz1);
		zz2 = zz1 + (zz2 - zz1) * p;
		xx2 = xx1 + (xx2 - xx1) * p;
		xt2 = xt1 + (xt2 - xt1) * p;
	}
	
	double xPix1 = xCenter - (xx1 / zz1 * fov);
	double xPix2 = xCenter - (xx2 / zz2 * fov);
	
	if (xPix1 >= xPix2) return;
	int xp1 = (int) ceil(xPix1);
	int xp2 = (int) ceil(xPix2);
	if (xp1 < 0) xp1 = 0;
	if (xp2 > width) xp2 = width;
	
	double yPixel11 = (u / zz1 * fov + yCenter);
	double yPixel12 = (l / zz1 * fov + yCenter);
	double yPixel21 = (u / zz2 * fov + yCenter);
	double yPixel22 = (l / zz2 * fov + yCenter);
	
	double iz1 = 1.0 / zz1;
	double iz2 = 1.0 / zz2;
	double iza = iz2 - iz1;
	
	double ixt1 = xt1 * iz1;
	double ixta = xt2 * iz2 - ixt1;
	double iw = 1.0 / (xPix2 - xPix1);
	
	for (int x = xp1; x < xp2; x++) {
		double pr = (x - xPix1) * iw;
		double iz = iz1 + iza * pr;
		
		if (zBufferWall[x] > iz) continue;
		zBufferWall[x] = iz;
		int xPix = (int) (2.0 * (ixt1 + ixta * pr) / iz);
		
		double yPix1 = yPixel11 + (yPixel21 - yPixel11) * pr - 0.5;
		double yPix2 = yPixel12 + (yPixel22 - yPixel12) * pr;
		
		int yp1 = (int) ceil(yPix1);
		int yp2 = (int) ceil(yPix2);
		if (yp1 < 0) yp1 = 0;
		if (yp2 > height) yp2 = height;
		
		double ih = 1.0 / (yPix2 - yPix1);
		for (int y = yp1; y < yp2; y++) {
			double pry = (y - yPix1) * ih;
			int yPix = (int) (32 * pry);
			
			int xSrc = xPix + (tex % 8) * 32;
			int ySrc = yPix + (tex / 8) * 32;
			unsigned int pix = MixColors(WallTex->pixels[xSrc + ySrc * WallTex->width], color);
			
			pixels[x + y * width] = pix;
			zBuffer[x + y * width] = 1.0 / iz * 4.0;
		}
	}
}

void Bitmap3D::RenderSprite(double x, double y, double z, int tex, int color) {
	double xc = (x - xCam) * 2.0 - rSin * 0.2;
	double yc = (y - yCam) * 2.0 - rCos * 0.2;
	double zc = (z - zCam) * 2.0;
	
	double xx = xc * rCos - yc * rSin;
	double yy = yc * rCos + xc * rSin;
	double zz = zc;
	
	if (yy < 0.1) return;
	
	double xPix = xCenter - (xx / yy * fov);
	double yPix = (zz / yy * fov) + yCenter;
	
	double xPix1 = xPix - height / zz;
	double xPix2 = xPix + height / zz;
	
	double yPix1 = yPix - height / zz;
	double yPix2 = yPix + height / zz;
	
	int xp1 = (int) ceil(xPix1);
	int xp2 = (int) ceil(xPix2);
	int yp1 = (int) ceil(yPix1);
	int yp2 = (int) ceil(yPix2);
	
	if (xp1 < 0) xp1 = 0;
	if (xp2 > width) xp2 = width;
	if (yp1 < 0) yp1 = 0;
	if (yp2 > height) yp2 = height;
	
	zz *= 4;
	
	for (int xp = xp1; xp < xp2; xp++) {
		double xpr = (xp - xPix1) / (xPix2 - xPix1);
		int xt = (int) (xpr * 32);
		
		for (int yp = yp1; yp < yp2; yp++) {
			double ypr = (yp - yPix1) / (yPix2 - yPix1);
			int yt = (int) (ypr * 32);
			
			if (zBuffer[xp + yp * width] > zz) {
				int xTex = xt + (tex % 8) * 32;
				int yTex = yt + (tex / 8) * 32;
				
				unsigned int pix = SpriteTex->pixels[xTex + yTex * SpriteTex->width];
				if (pix & 0xff000000) {
					pixels[xp + yp * width] = MixColors(pix, color);
					zBuffer[xp + yp * width] = zz;
				}
			}
		}
	}
}
