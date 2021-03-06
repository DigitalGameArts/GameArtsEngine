#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "meshObject.h"
#include "SDL/SDL.h" 
#include "SDL_IMAGE/SDL_image.h"


enum{
	DEFAULTTEXTURE = 1,
};

class meshLoader{
	vector<meshObject*> meshes;

	void recursiveProcess(aiNode* node, const aiScene* scene);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	//unsigned int loadTexture(const char* filename);
	unsigned int loadTexture(const char* filename, bool generate = false);

public:
	meshLoader(const char* filename);
	~meshLoader();

	void draw(unsigned int programId);

	vector<meshObject*>& getMeshes();

};

#endif