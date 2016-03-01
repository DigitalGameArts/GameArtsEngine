#ifndef LIGHT_H
#define LIGHT_H

#include "SDL/SDL.h"

#include <windows.h>
#include <stdio.h>

#include <GL/GL.h>
#include <GL/GLU.h>

#include <string>
#include <vector>
#include <fstream>

using std::vector;

enum LIGHT_TYPE{
	LIGHT_SPOT,
	LIGHT_POINT,
	LIGHT_DIRECTIONAL

};


class Light{

public:
	static void Initialize(void);
	Light(LIGHT_TYPE lightType);
	~Light();


	void setDiffuse(float r, float g, float b, float a);
	void setAmbient(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);

	void setLightType(LIGHT_TYPE lighType);

	void setPosition(float x, float y, float z);

	void setSpotDirection(float x, float y, float z);
	void setCutoff(float value);
	void setExponent(float value);

	void setAttenuation(float constant, float linear, float quadratic);

	int getLightNum(void);
	void updateLight(void);

	void Visible(bool = true);
	static int numLights;
	static vector<int> avaiableLights;
	static vector<Light *> lights;

private:
	GLfloat position[4];
	GLfloat diffuse[4];
	GLfloat ambient[4];
	GLfloat specular[4];
	GLfloat spotDirection[4];

	float cutoff;
	float exponent;

	bool visible;
	int lightType;


	int lightNum;
};

#endif // !LIGHT_H
