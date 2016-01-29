#ifndef GA_ENGINE_H
#define GA_ENGINE_H
#define iGAEngine GAEngine::getEngine()


//#include "SDL.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>

#include <list>
#include <string>

#include <GL/GL.h>
#include <GL/GLU.h>


#include "Texture.h"
#include "Light.h"


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
	GLvoid drawCube(float x = 0.0f, float y = 0.0f, float z = 0.0f, Texture *texture = nullptr);

	GLvoid displayFPS(SDL_Window *window);




private:
	GLuint fontBase;

	Texture *fontTexture;


};

#endif