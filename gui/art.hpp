#ifndef ART_HPP
#define ART_HPP

class Bitmap;

#include <string>

Bitmap* LoadBitmap(std::string filename);
void LoadTextures();

unsigned int MixColors(unsigned int col1, unsigned int col2);
unsigned int MixColors(unsigned int col1, unsigned int col2, double v);

unsigned int ColorTo8Bit(unsigned int col);
unsigned int ColorTo12Bit(unsigned int col);
unsigned int ColorTo16Bit(unsigned int col);

extern Bitmap* FloorTex;
extern Bitmap* WallTex;
extern Bitmap* FontTex;
extern Bitmap* SpriteTex;

#endif // ART_HPP
