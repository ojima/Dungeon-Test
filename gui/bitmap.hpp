#ifndef BITMAP_HPP
#define BITMAP_HPP

class Bitmap {
	public:
		const int width, height;
		unsigned int* pixels;
		
	public:
		Bitmap(int width, int height);
		virtual ~Bitmap();
		
		void Draw(Bitmap* bmp, int xOffs, int yOffs);
		void Draw(Bitmap* bmp, int xOffs, int yOffs, int xo, int yo, int w, int h, unsigned int col);
		
		void FlipDraw(Bitmap* bmp, int xOffs, int yOffs);
		
		void Fill(int x1, int y1, int x2, int y2, unsigned int col);
		
		void RenderText(std::string text, int x, int y, unsigned int col);
};

#endif // BITMAP_HPP
