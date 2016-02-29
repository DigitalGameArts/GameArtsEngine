#include "Shader.h"


Shader::Shader(string vertexFile, string fragFile){

	vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	const char *vv = readFile(vertexFile.c_str());
	const char *ff = readFile(fragFile.c_str());


	if (vv != NULL && ff != NULL)
	{
		glShaderSourceARB(vertexShader, 1, &vv, NULL);
		glShaderSourceARB(fragmentShader, 1, &ff, NULL);

		delete[] vv;
		delete[] ff;

		glCompileShaderARB(vertexShader);
		glCompileShaderARB(fragmentShader);

		char errorv[1000];
		glGetShaderInfoLog(vertexShader, 1000, NULL, errorv);
		
		if (errorv[0] != NULL && errorv[1] != NULL) cout << endl << vertexFile << " Status: " << errorv << endl;
		else cout << endl << vertexFile << " Status: loaded successfully! " << endl;

		char errorf[1000];
		glGetShaderInfoLog(fragmentShader, 1000, NULL, errorf);
		if (errorf[0] != NULL && errorf[1] != NULL) cout << endl << fragFile << " Status: " << errorf << endl;
		else cout << endl << fragFile << " Status: loaded successfully! " << endl;


		ShaderProgram = glCreateProgramObjectARB();
		glAttachObjectARB(ShaderProgram, vertexShader);
		glAttachObjectARB(ShaderProgram, fragmentShader);

		glLinkProgramARB(ShaderProgram);

	}


}





void Shader::useShader(void){
	glUseProgramObjectARB(NULL);
	glUseProgramObjectARB(ShaderProgram);
}

GLhandleARB Shader::getShaderProgram(){
	return ShaderProgram;
}




void Shader::disableShaders(void){
	glUseProgramObjectARB(NULL);
}
void Shader::setAttribute1i(string name, int x){
	glUniform1iARB(glGetUniformLocationARB(ShaderProgram, name.data()), x);
}
void Shader::setAttribute1f(string name, float x){
	glUniform1fARB(glGetUniformLocationARB(ShaderProgram, name.data()), x);
}
void Shader::setAttribute2f(string name, float x, float y){
	glUniform2fARB(glGetUniformLocationARB(ShaderProgram, name.data()), x,y);
}
void Shader::setAttribute3f(string name, float x, float y, float z){
	glUniform3fARB(glGetUniformLocationARB(ShaderProgram, name.data()), x,y,z);
}
void Shader::setAttribute4f(string name, float x, float y, float z, float w){
	glUniform4fARB(glGetUniformLocationARB(ShaderProgram, name.data()), x, y,z,w);
}


char* Shader::readFile(const char *fn){

	int length;
	char *buffer;

	ifstream file;

	file.open(fn, ios::binary);

	if (!file.is_open()){
		cout << "fail";
		return NULL;
	}


	file.seekg(0, ios::end);
	length = file.tellg();

	file.seekg(0, ios::beg);



	buffer = new char[length + 1];

	file.read(buffer, (GLuint)length);

	file.close();
	buffer[length] = '\0';

	//cout << buffer;

	return buffer;
}
