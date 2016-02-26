#ifndef MESH_H
#define MESH_H

#include <windows.h>
#include <iostream>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

#include "Vector3.h"

#include <vector>
#include <string>

using namespace std;


struct vertexData{
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	Vector3 color;
	float U, V;
};

struct textureData{
	unsigned int id;
	unsigned int type;
};


class meshObject{
	vector<vertexData> data;
	vector<textureData> textures;
	vector<unsigned int> indices;
	unsigned int VBO;
	unsigned int IND;

public:
	meshObject(vector<vertexData>* vd, vector<unsigned int>* id, vector<textureData>* td = nullptr);
	~meshObject();

	void draw(unsigned int programId);

};


#endif