#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;


#include <GL/glew.h>


class Shader{
public:
	Shader(string vertexFile, string fragFile);

	void useShader(void);

	static void disableShaders(void);
	void setAttribute1i(string name, int x);
	void setAttribute1f(string name, float x);
	void setAttribute2f(string name, float x, float y);
	void setAttribute3f(string name, float x, float y, float z);
	void setAttribute4f(string name, float x, float y, float z , float w);

	GLhandleARB getShaderProgram();

private:
	char* readFile(const char *fn);
private:
	
	GLhandleARB vertexShader;
	GLhandleARB fragmentShader;

	GLhandleARB ShaderProgram;
};



#endif