#include "SDL.h"
#define main SDL_main

#include "GAEngine.h"

//Window Height and Width
const GLsizei	windowWidth = 1600;
const GLsizei	windowHeight = 900;

//Navigational variables
MouseState		mouseState;
GLfloat			rotateX = 45.0f;
GLfloat			rotateY = 45.0f;
GLfloat			zoom = -15.0f;
const float		speed = 5.0f;
bool			navigating = false;
const float		rotateX_speed = 0.5f;
const float		rotateY_speed = 0.5f;
const float		zoom_speed = 0.01f;

//Textures
Texture*		cratetexture = NULL;

//Lights
Light*			mainLight = NULL;

//Functions
GLvoid			drawScene(SDL_Window *window);
GLboolean		viewportNavigation(GLvoid);


int main(int argc, char **argv)
{

	//Initializes SDL and gets everything set up 
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("GameArtsEngine", 40, 40, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	//Initializes our Engine
	iGAEngine->initGL(windowWidth, windowHeight);



										/**************************************ALL THE ONE TIME SCENE SETUP GOES HERE*******************************************/

	//Creates a basic Light for our scene --> will eventually be moved to the Engine itself
	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(2, 2, 2, 1);
	mainLight->setExponent(10);
	mainLight->setPosition(-5,25,5);

	//Imports a new Texture 
	cratetexture = new Texture("Assets/Textures/crate_texture.tga", "Surface texture");

										/**************************************ALL THE ONE TIME SCENE SETUP GOES HERE*******************************************/

	
	// Main Engine Loop
	bool isRunning = true;
	SDL_Event  event;

	while (isRunning )
	{
		//Gets states of the Mouse and stroes them in the struct
		SDL_GetMouseState(&mouseState.x, &mouseState.y);
		mouseState.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
		mouseState.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
		mouseState.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);

		drawScene(window); // calls stuff that needs to update every frame

		//Event loop
		while (SDL_PollEvent(&event)){

			if (event.type == SDL_QUIT)
				isRunning = false;
		}


		if (viewportNavigation())
			isRunning = false;
	}


	GAEngine::Unitialize();
	SDL_Quit();

	return 1;

}

//draws Items that need to updated every frame.
GLvoid drawScene(SDL_Window *window){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	iGAEngine->setPerspective(windowWidth, windowHeight);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_BLEND);

	glTranslatef(0, 0, zoom);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);


	//updates all lights in the scene!
	for (int i = 0; i < (int)Light::lights.size(); i++)
	{
		Light::lights[i]->updateLight();
	}


	// Draws Geometry
	iGAEngine->drawCube(0, 1, 0, cratetexture);
	iGAEngine->drawPlane(0, 0, 0, 10, 10, 10);

																		//3D//
	/***************************************************************************************************************************************/
																		//2D//

	glDisable(GL_LIGHTING);

	iGAEngine->setOrtho(windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//Displays FPS and Engine name
	iGAEngine->displayFPS(window);
	iGAEngine->drawText(windowWidth - (iGAEngine->getTextWidth("GameArtsEngine")) + 10, 7, "GameArtsEngine");

	//Renders and Swaps Buffers
	glFlush();
	SDL_GL_SwapWindow(window);
}

//Viewport Navigation
GLboolean viewportNavigation(GLvoid){

	static int lastX = -1;
	static int lastY = -1;
	if (lastX == -1 && lastY == -1)
	{
		lastX = mouseState.x;
		lastY = mouseState.y;
	}


	int changeX = lastX - mouseState.x;
	int changeY = lastY - mouseState.y;

	lastX = mouseState.x;
	lastY = mouseState.y;

	if (mouseState.LeftButtonDown)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		rotateX -= (float)changeY * rotateX_speed;
		rotateY -= (float)changeX * rotateY_speed;

		navigating = true;
	}

	else if (mouseState.RightButtonDown){
		SDL_SetRelativeMouseMode(SDL_TRUE);
		zoom -= (float)changeX * zoom_speed;

		navigating = true;

	}

	else{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		navigating = false;
	}

	return false;
}