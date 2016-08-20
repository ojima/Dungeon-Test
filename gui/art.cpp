#include "../include.hpp"

Bitmap* FloorTex = nullptr;
Bitmap* SkyTex = nullptr;
Bitmap* WallTex = nullptr;
Bitmap* FontTex = nullptr;
Bitmap* SpriteTex = nullptr;

Bitmap* LoadBitmap(std::string filename) {
	SDL_Surface *opt = nullptr, *load = IMG_Load(filename.c_str());
	if (load != nullptr) {
		static SDL_PixelFormat format;
		format.BitsPerPixel = 32;
		format.BytesPerPixel = 4;
		format.Rmask = 0x00ff0000;
		format.Gmask = 0x0000ff00;
		format.Bmask = 0x000000ff;
		format.Amask = 0xff000000;
		
		opt = SDL_ConvertSurface(load, &format, 0);
		SDL_FreeSurface(load);
		
		if (opt != nullptr) {
			SDL_SetColorKey(opt, SDL_TRUE, SDL_MapRGB(&format, 0xFF, 0x00, 0xFF));
		}
	}
	
	if (opt == nullptr) return nullptr;
	
	Bitmap* bmp = new Bitmap(opt->w, opt->h);
	for (int i = 0; i < opt->w * opt->h; i++) {
		bmp->pixels[i] = ((unsigned int*) opt->pixels)[i];
		if (bmp->pixels[i] == 0xffff00ff) {
			bmp->pixels[i] = 0x00000000;
		}
	}
	
	SDL_FreeSurface(opt);
	
	std::cout << "Loaded a " << bmp->width << "x" << bmp->height
		<< " bitmap from file " << filename << std::endl;
	
	return bmp;
}

void LoadTextures() {
	FloorTex = LoadBitmap("res/tex/floors.png");
	WallTex = LoadBitmap("res/tex/walls.png");
	FontTex = LoadBitmap("res/tex/font.png");
	SpriteTex = LoadBitmap("res/tex/sprites.png");
}

unsigned int MixColors(unsigned int col1, unsigned int col2) {
	unsigned int a1 = (col1 >> 24) & 0xff;
	unsigned int r1 = (col1 >> 16) & 0xff;
	unsigned int g1 = (col1 >> 8) & 0xff;
	unsigned int b1 = (col1) & 0xff;
	
	unsigned int a2 = (col2 >> 24) & 0xff;
	unsigned int r2 = (col2 >> 16) & 0xff;
	unsigned int g2 = (col2 >> 8) & 0xff;
	unsigned int b2 = (col2) & 0xff;
	
	unsigned int a3 = (a1 + a2) / 2;
	unsigned int r3 = (r1 + r2) / 2;
	unsigned int g3 = (g1 + g2) / 2;
	unsigned int b3 = (b1 + b2) / 2;
	
	return (a3 << 24) | (r3 << 16) | (g3 << 8) | (b3);
}

unsigned int MixColors(unsigned int col1, unsigned int col2, double v) {
	unsigned int r1 = (col1 >> 16) & 0xff;
	unsigned int g1 = (col1 >> 8) & 0xff;
	unsigned int b1 = (col1) & 0xff;
	
	unsigned int r2 = (col2 >> 16) & 0xff;
	unsigned int g2 = (col2 >> 8) & 0xff;
	unsigned int b2 = (col2) & 0xff;
	
	unsigned int r3 = r1 * v + r2 * (1.0 - v);
	unsigned int g3 = g1 * v + g2 * (1.0 - v);
	unsigned int b3 = b1 * v + b2 * (1.0 - v);
	
	return (0xff << 24) | (r3 << 16) | (g3 << 8) | (b3);
}

unsigned int ColorTo8Bit(unsigned int col) {
	unsigned int r = (col >> 16) & 0xff;
	unsigned int g = (col >> 8) & 0xff;
	unsigned int b = col & 0xff;
	
	r = (r >> 5) << 5;
	g = (g >> 5) << 5;
	b = (b >> 6) << 6;
	
	return (0xff << 24) | (r << 16) | (g << 8) | (b);
}

unsigned int ColorTo12Bit(unsigned int col) {
	unsigned int r = (col >> 16) & 0xff;
	unsigned int g = (col >> 8) & 0xff;
	unsigned int b = col & 0xff;
	
	r = (r >> 4) << 4;
	g = (g >> 4) << 4;
	b = (b >> 4) << 4;
	
	return (0xff << 24) | (r << 16) | (g << 8) | (b);
}

unsigned int ColorTo16Bit(unsigned int col) {
	unsigned int r = (col >> 16) & 0xff;
	unsigned int g = (col >> 8) & 0xff;
	unsigned int b = col & 0xff;
	
	r = (r >> 3) << 3;
	g = (g >> 2) << 2;
	b = (b >> 3) << 3;
	
	return (0xff << 24) | (r << 16) | (g << 8) | (b);
}
