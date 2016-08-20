#include "../include.hpp"

Bitmap::Bitmap(int width, int height) : width(width), height(height) {
	this->pixels = new unsigned int[width * height];
}

Bitmap::~Bitmap() {
	delete this->pixels;
}

void Bitmap::Draw(Bitmap* bmp, int xOffs, int yOffs) {
	for (int x = 0; x < bmp->width; x++) {
		int xPix = x + xOffs;
		if (xPix < 0 || xPix >= width) continue;
		
		for (int y = 0; y < bmp->height; y++) {
			int yPix = y + yOffs;
			if (yPix < 0 || yPix >= height) continue;
			
			unsigned int src = bmp->pixels[x + y * bmp->width];
			pixels[xPix + yPix * width] = src;
		}
	}
}

void Bitmap::Draw(Bitmap* bmp, int xOffs, int yOffs, int xo, int yo, int w, int h, unsigned int col) {
	for (int x = 0; x < w; x++) {
		int xPix = x + xOffs;
		if (xPix < 0 || xPix >= width) continue;
		
		for (int y = 0; y < h; y++) {
			int yPix = y + yOffs;
			if (yPix < 0 || yPix >= height) continue;
			
			unsigned int src = bmp->pixels[(x + xo) + (y + yo) * bmp->width];
			if (src & 0xff000000)
				pixels[xPix + yPix * width] = MixColors(src, col);
		}
	}
}

void Bitmap::FlipDraw(Bitmap* bmp, int xOffs, int yOffs) {
	for (int x = 0; x < bmp->width; x++) {
		int xPix = xOffs + bmp->width - x - 1;
		if (xPix < 0 || xPix >= width) continue;
		
		for (int y = 0; y < bmp->height; y++) {
			int yPix = y + yOffs;
			if (yPix < 0 || yPix >= height) continue;
			
			unsigned int src = bmp->pixels[x + y * bmp->width];
			pixels[xPix + yPix * width] = src;
		}
	}
}

void Bitmap::Fill(int x1, int y1, int x2, int y2, unsigned int col) {
	for (int x = x1; x < x2; x++) {
		if (x < 0 || x >= width) continue;
		for (int y = y1; y < y2; y++) {
			if (y < 0 || y >= height) continue;
			
			pixels[x + y * width] = col;
		}
	}
}

void Bitmap::RenderText(std::string text, int x, int y, unsigned int col) {
	static std::string font = std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ.,!?\"'/\\<>()[]{}abcdefghijklmnopqrstuvwxyz_               0123456789+-=*:;ÖÅÄå                      ");
	
	for (unsigned int i = 0; i < text.size(); i++) {
		std::string::size_type c = font.find(text[i]);
		if (c == std::string::npos) continue;
		
		int xx = c % 42;
		int yy = c / 42;
		Draw(FontTex, x + i * 6, y, xx * 6, yy * 8, 5, 8, col);
	}
}
