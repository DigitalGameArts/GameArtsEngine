#include "GAEngine.h"
const int fontsize = 20;
const int fontSpace = 19;


//INIT ENGINE
GAEngine::GAEngine(){


}
GAEngine::~GAEngine(){

}
GLvoid GAEngine::Unitialize(GLvoid){
	delete GAEngine::getEngine();
}
GAEngine *GAEngine::getEngine(GLvoid){

	static GAEngine *engine = new GAEngine();

	return engine;
}
GLvoid GAEngine::Inititalize(GLint width, GLint height){

	Light::Initialize();

	fontTexture = new Texture("Assets/Textures/font.tga");


	buildTextureFont();
}


//Sets ViewPortModes
GLvoid GAEngine::setOrtho(GLsizei width, GLsizei height){
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0, width, height, 0);
}
GLvoid GAEngine::setPerspective(GLsizei width, GLsizei height){

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 200.0f);
}

//Initialize OpenGL in respect to Engine
GLvoid GAEngine::initGL(GLsizei width, GLsizei height){

	Inititalize(width, height);

	setPerspective(width, height);

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);


	glEnable(GL_LIGHTING);

}

//TEXT
GLvoid GAEngine::buildTextureFont(GLvoid){

	fontBase = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, fontTexture->texID);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < 256; i++){

		float cx = (float)(i % 16) / 16.0f;
		float cy = (float)(i / 16) / 16.0f;

		glNewList(fontBase + i, GL_COMPILE);

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(cx, 1 - cy - 0.0625);	glVertex2i(0, fontsize);
		glTexCoord2f(cx + 0.0625, 1 - cy - 0.0625);	    glVertex2i(fontsize, fontsize);
		glTexCoord2f(cx + 0.0625, 1 - cy);			glVertex2i(fontsize, 0);
		glTexCoord2f(cx, 1 - cy);			glVertex2i(0, 0);
		glEnd();

		glTranslated(fontSpace - 3, 0, 0);

		glEndList();
	}

}
GLvoid GAEngine::drawText(GLint x, GLint y, const char *in_text, ...){

	char text[256];

	va_list ap;

	va_start(ap, in_text);
	vsprintf_s(text, in_text, ap);
	va_end(ap);

	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, fontTexture->texID);


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();

	glTranslated(x, y - 5, 0);
	glListBase(fontBase - 32);
	glCallLists((GLsizei)strlen(text), GL_BYTE, text);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);


}
GLuint GAEngine::getTextWidth(const char *text){

	return GLuint((strlen(text) + 1) * fontSpace);
}
GLuint GAEngine::getTextHeight(const char *text){


	return (fontsize);
}

//Displays FPS
GLvoid GAEngine::displayFPS(SDL_Window *window){
	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat fps = 0.0f;

	int newTime = SDL_GetTicks();

	if (newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime) * 1000.0f;

		fps = (fps + newFPS) / 2.0f;

		lastTime = newTime;

		loops = 0;

	}

	drawText(5, 5, "FPS: %.2f", fps);


	loops++;
}

//CREATE PLANE OBJECT
GLvoid GAEngine::drawPlane(float x, float y, float z, float width, float height, int divisions){


	float incX = width / (float)divisions;
	float incY = height / (float)divisions;

	glColor3f(0, 0, 0);
	glNormal3f(0, 1, 0);

	for (float i = -width / 2; i < width / 2; i += incX){
		for (float j = -height / 2; j < height / 2; j += incY){


			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(i + incX, 0, j + incY);
			glVertex3f(i, 0, j + incY);
			glVertex3f(i + incX, 0, j);
			glVertex3f(i, 0, j);
			glEnd();

		}
	}



}

//CREATE CUBE OBJECT
GLvoid GAEngine::drawCube(float x, float y, float z, Texture *texture){

	glEnable(GL_TEXTURE_2D);

	// checks if there is a texture to bind
	if (texture != nullptr)
		glBindTexture(GL_TEXTURE_2D, texture->texID);
	else{
		glDisable(GL_TEXTURE_2D);
	}

	glPushMatrix();
	glTranslatef(x, y, z);


	glBegin(GL_QUADS);

	//TopFace
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	//BottomFace
	glNormal3f(0, -1, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	//FrontFace
	glNormal3f(0, 0, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);


	//BackFace
	glNormal3f(0, 0, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	//LeftFace
	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);


	//RightFace
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glEnd();


	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);

}
