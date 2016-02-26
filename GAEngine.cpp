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

	//drawText(5, 5, "FPS: %.2f", fps);


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
GLvoid GAEngine::drawCube(float x, float y, float z, unsigned int texture){

	glEnable(GL_TEXTURE_2D);

	// checks if there is a texture to bind
	if (texture != NULL)
		glBindTexture(GL_TEXTURE_2D, texture);
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
	glDisable(GL_TEXTURE_2D);

}

unsigned int GAEngine::loadTexture(const char* filename){

	unsigned int id;
	glGenTextures(1, &id);
	SDL_Surface* img = IMG_Load(filename);


	if (img == NULL){
		cout << endl << "The image: " << filename << " could not be loaded" << endl;
		return -1;
	}

	SDL_PixelFormat form = { SDL_PIXELFORMAT_UNKNOWN, 0, 32, 4, 0, 0, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff };
	SDL_Surface* convertedImg = SDL_ConvertSurface(img, &form, SDL_SWSURFACE);

	if (convertedImg == NULL){
		cout << endl << "The conversion of the Image: " << filename << " failed" << endl;
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedImg->w, convertedImg->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedImg->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(img);
	SDL_FreeSurface(convertedImg);
	return id;
}
