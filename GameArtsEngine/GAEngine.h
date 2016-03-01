#ifndef GA_ENGINE_H
#define GA_ENGINE_H
#define iGAEngine GAEngine::getEngine()

#include <windows.h>
#include <GL/glew.h>
#include <iostream>
using namespace std;

#include "SDL/SDL.h"
#include "SDL_IMAGE/SDL_image.h"
#define main SDL_main


#include <stdio.h>
#include <math.h>
#include <list>
#include <string>

#include <GL/glew.h>
#include <GL/GLU.h>

#include "Light.h"
#include "Shader.h"
#include "AssimpLoader.h"

struct MouseState {
	int LeftButtonDown;
	int RightButtonDown;
	int MiddleButtonDown;

	int x;
	int y;


	MouseState(){
		int LeftButtonDown = 0;
		int RightButtonDown = 0;
		int MiddleButtonDown = 0;
	}
};

class GAEngine{
public:
	GAEngine();
	~GAEngine();


	static GLvoid Unitialize(GLvoid);
	static GAEngine *getEngine(GLvoid);

	GLvoid Inititalize(GLint width, GLint height);

	GLvoid setOrtho(GLsizei width, GLsizei height);
	GLvoid setPerspective(GLsizei width, GLsizei height);

	GLvoid initGL(GLsizei width, GLsizei height);

	GLvoid buildTextureFont(GLvoid);
	GLvoid drawText(GLint x, GLint y, const char *text, ...);

	GLuint getTextWidth(const char *text);
	GLuint getTextHeight(const char *text);

	GLvoid drawPlane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float width = 10.0f, float height = 10.0f, int divisions = 10);
	GLvoid drawCube(float x = 0.0f, float y = 0.0f, float z = 0.0f, GLhandleARB p = NULL, unsigned int = NULL, unsigned int = NULL);

	GLvoid displayFPS(SDL_Window *window);

	unsigned int loadTexture(const char* filename);



private:
	GLuint fontBase;


};

#endif