
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
unsigned int	defaultTexture;
unsigned int	cratenormalTexture;

//Shaders
Shader			*lambertShader_static;
Shader			*diffuseShader;
Shader			*normalShader;
Shader			*normalSpecShader;

//Meshes->Primitives
meshLoader		*teapodModel;
meshLoader		*cylinderModel;
meshLoader		*sphereModel;
meshLoader		*cubeModel;


//Meshes->GameModels
meshLoader		*bremerWall;


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
	defaultTexture = iGAEngine->loadTexture("Assets/Textures/ModelgridTexture.jpg");

	//Load Models->Primitives ------------------------ENABLE IF YOU NEED PRIMITIVES---------------------------------
	/*teapodModel = new meshLoader("Assets/Models/Primitives/teapod.dae");
	cylinderModel = new meshLoader("Assets/Models/Primitives/cylinder.dae");
	sphereModel = new meshLoader("Assets/Models/Primitives/sphere.dae");
	cubeModel = new meshLoader("Assets/Models/Primitives/cube.dae");*/
	

	//Load Models->GameModels
	bremerWall = new meshLoader("Assets/Models/test/bremerWall.dae");
	//-----------------------------------------------------------------------------------------SHADERSETUP--------------------------------------------
	
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader){

		cout << endl << "SHADER LOG: -------------------------------------------------------- BEGIN" << endl;
		
		// Simple Normal Shader without SpecMap
		normalShader = new Shader("normal.vert", "normal.frag");
		normalShader->useShader();
		normalShader->setAttribute1i("diffuseMap0", 0);
		normalShader->setAttribute1i("normalMap1", 1);
		normalShader->setAttribute1f("SpecularPower", 2.0f);
		normalShader->setAttribute4f("Specular", 0.5, 0.5, 0.5, 1.0);
		normalShader->setAttribute4f("Ambient", 0.3, 0.3, 0.3, 1.0);
		normalShader->setAttribute4f("Diffuse", 1, 1, 1, 1.0);
		normalShader->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);

		// Normal Shader with SpecMap
		normalSpecShader = new Shader("normalSpec.vert", "normalSpec.frag");
		normalSpecShader->useShader();
		normalSpecShader->setAttribute1i("diffuseMap0", 0);
		normalSpecShader->setAttribute1i("normalMap1", 1);
		normalSpecShader->setAttribute1i("specularMap2", 2);
		normalSpecShader->setAttribute1f("SpecularPower", 2.0f);
		normalSpecShader->setAttribute4f("Specular", 0.5, 0.5, 0.5, 1.0);
		normalSpecShader->setAttribute4f("Ambient", 0.3, 0.3, 0.3, 1.0);
		normalSpecShader->setAttribute4f("Diffuse", 1, 1, 1, 1.0);
		normalSpecShader->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);

		// Simple LambertShader with DiffuseMap
		diffuseShader = new Shader("lambertDiffuse.vert", "lambertDiffuse.frag");
		diffuseShader->useShader();
		diffuseShader->setAttribute4f("Ambient", 0.3, 0.3, 0.3, 1.0);
		diffuseShader->setAttribute4f("Diffuse", 1, 1, 1, 1.0);
		diffuseShader->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);
		
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
	delete diffuseShader;
	delete normalShader;
	delete normalSpecShader;

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
	normalSpecShader->useShader();
	glPushMatrix();
	glTranslatef(0, 0, 0);
	bremerWall->draw(normalSpecShader->getShaderProgram());
	glPopMatrix();
	normalSpecShader->disableShaders();

	//iGAEngine->drawCube(0, 1, 0, NULL);
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