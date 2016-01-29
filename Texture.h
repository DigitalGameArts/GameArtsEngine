#ifndef TEXTURE_H
#define TEXTURE_H



#include "SDL.h"

#include <windows.h>
#include <stdio.h>

#include <GL/GL.h>
#include <GL/GLU.h>

#include <string>
#include <vector>
#include <fstream>

using std::ifstream;
using std::string;
using std::vector;


struct TGA_Header
{
	GLubyte ID_Length;
	GLubyte ColorMapType;
	GLubyte ImageType;
	GLubyte colorMapSpecifications[5];
	GLshort xOrigin;
	GLshort yOrigin;
	GLshort ImageWidth;
	GLshort ImageHeight;
	GLubyte PixelDepth;


};

class Texture{

	//Functions
public:
	Texture(string filename, string name = "");
	~Texture();


public:
	unsigned char	*imageData;
	unsigned int	bpp;
	unsigned int	width;
	unsigned int	height;
	unsigned int	texID;

	string name;

	static vector<Texture *> textures;

private:
	bool loadTGA(string filename);
	bool createTexture(unsigned char *imageData, int width, int height, int type);




};

#endif // !TEXTURE_H
