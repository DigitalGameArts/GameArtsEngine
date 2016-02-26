
#include "GAEngine.h"

//Window Height and Width
const GLsizei	windowWidth = 1280;
const GLsizei	windowHeight = 720;

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

//Lights
Light*			mainLight = NULL;

//Functions
GLvoid			drawMainScene(SDL_Window *window);
GLboolean		viewportNavigation(GLvoid);

//Textures
unsigned int	crateTexture;

//Shaders
Shader			*lambertShader_static;
Shader			*lambertShader_dynamic;


//Meshes->Primitives
meshLoader		*teapodModel;
meshLoader		*cylinderModel;
meshLoader		*sphereModel;
meshLoader		*cubeModel;


int main(int argc, char **argv)
{

	//Initializes console Position
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd, 1350, 0, 500, 1000, TRUE);
	std::cout << "GameArtsEngine" << std::endl;


	//Initializes SDL and gets everything set up 
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("GameArtsEngine", 40, 40, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	//Initializes our Glew
	glewInit();

	//Initializes our Engine
	iGAEngine->initGL(windowWidth, windowHeight);



										/**************************************ALL THE ONE TIME SCENE SETUP GOES HERE*******************************************/

	
	//Creates a basic Light for our scene --> will eventually be moved to the Engine itself
	mainLight = new Light(LIGHT_POINT);
	mainLight->setDiffuse(2, 2, 2, 1);
	mainLight->setExponent(10);
	mainLight->setPosition(-5,25,5);

	
	//Load Textures
	crateTexture = iGAEngine->loadTexture("Assets/Textures/crate_texture.tga");

	//Load Models->Primitives
	teapodModel = new meshLoader("Assets/Models/Primitives/teapod.dae");
	cylinderModel = new meshLoader("Assets/Models/Primitives/cylinder.dae");
	sphereModel = new meshLoader("Assets/Models/Primitives/sphere.dae");
	cubeModel = new meshLoader("Assets/Models/Primitives/cube.dae");

	//-----------------------------------------------------------------------------------------SHADERSETUP--------------------------------------------
	
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader){

		cout << endl << "SHADER LOG: -------------------------------------------------------- BEGIN" << endl;

		lambertShader_static = new Shader("Color.vert", "Color.frag");
		lambertShader_static->useShader();
		lambertShader_static->setAttribute1i("cloudTexture", 0);
		lambertShader_static->setAttribute4f("Ambient", 1, 1, 1, 1.0);
		lambertShader_static->setAttribute4f("Diffuse", 1, 1, 1, 1.0);
		lambertShader_static->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);


		lambertShader_dynamic = new Shader("lambert_dynamic.vert", "lambert_dynamic.frag");
		lambertShader_dynamic->useShader();
		lambertShader_dynamic->setAttribute4f("Ambient", 0.3, 0.3, 0.3, 1.0);
		lambertShader_dynamic->setAttribute4f("Diffuse", 1, 1, 1, 1.0);
		lambertShader_dynamic->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);
		
		cout << endl << "SHADER LOG: ---------------------------------------------------------- END" << endl;

	}
	else{
		cout << endl << "GLSL not supported" << endl;
	}

	//-----------------------------------------------------------------------------------------SHADERSETUP--------------------------------------------




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

		drawMainScene(window); // calls stuff that needs to update every frame

		//Event loop
		while (SDL_PollEvent(&event)){

			if (event.type == SDL_QUIT)
				isRunning = false;
		}


		if (viewportNavigation())
			isRunning = false;
	}

	delete teapodModel;
	delete lambertShader_static;
	delete lambertShader_dynamic;

	GAEngine::Unitialize();


	SDL_Quit();

	return 1;

}

//draws Items that need to updated every frame.
GLvoid drawMainScene(SDL_Window *window){

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


	//Draws Geometry
	
	lambertShader_dynamic->useShader();
	glPushMatrix();
	glTranslatef(0, 0, 6);
	teapodModel->draw(lambertShader_dynamic->getShaderProgram());
	glPopMatrix();
	lambertShader_dynamic->disableShaders();

	lambertShader_dynamic->useShader();
	glPushMatrix();
	glTranslatef(0, 0, 2);
	cylinderModel->draw(lambertShader_dynamic->getShaderProgram());
	glPopMatrix();
	lambertShader_dynamic->disableShaders();

	lambertShader_dynamic->useShader();
	glPushMatrix();
	glTranslatef(0, 2, -2);
	sphereModel->draw(lambertShader_dynamic->getShaderProgram());
	glPopMatrix();
	lambertShader_dynamic->disableShaders();

	lambertShader_dynamic->useShader();
	glPushMatrix();
	glTranslatef(0, 0, -6);
	cubeModel->draw(lambertShader_dynamic->getShaderProgram());
	glPopMatrix();
	lambertShader_dynamic->disableShaders();


	//iGAEngine->drawCube(0, 1, -5, crateTexture);
	iGAEngine->drawPlane(0, 0, 0, 20, 20, 20);

																		//3D//
	/***************************************************************************************************************************************/
																		//2D//

	glDisable(GL_LIGHTING);

	iGAEngine->setOrtho(windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


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