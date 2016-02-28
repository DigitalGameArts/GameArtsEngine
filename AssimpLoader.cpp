#include "AssimpLoader.h"



meshLoader::meshLoader(const char* filename){
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, 
														aiProcess_GenSmoothNormals | 
														aiProcess_Triangulate | 
														aiProcess_CalcTangentSpace | 
														aiProcess_FlipUVs);

	/*if (scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		cout << "The model file: " << filename << " was not successfully opened" << endl;
		return;
	}*/


	if (!scene){
		cout << "The model file: " << filename << " was not successfully opened" << endl;
		return;
	}
	

	cout << endl << endl << filename << endl;
	recursiveProcess(scene->mRootNode, scene);

}


meshLoader::~meshLoader(){
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
}

void meshLoader::recursiveProcess(aiNode* node, const aiScene* scene){

	//process
	for (int i = 0; i < node->mNumMeshes; i++){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}


	//recursion
	for (int i = 0; i < node->mNumChildren; i++){

		recursiveProcess(node->mChildren[i], scene);
	}

}


void meshLoader::processMesh(aiMesh* mesh, const aiScene* scene){

	vector<vertexData> data;
	vector<unsigned int> indices;
	vector<textureData> textures;
	
	aiColor4D col;
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &col);
	Vector3 defaultColor(col.r, col.g, col.b);


	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		vertexData tmp;
		Vector3 tmpVec;

		//position
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		tmp.position = tmpVec;


		//normals
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		
		tmp.normal = tmpVec;


		if (mesh->mTangents){
			tmpVec.x = mesh->mTangents[i].x;
			tmpVec.y = mesh->mTangents[i].y;
			tmpVec.z = mesh->mTangents[i].z;	

		
		}
		else{
			tmpVec.x = 1.0;
			tmpVec.y = tmpVec.z = 0;
		}
		tmp.tangent = tmpVec;

		if (mesh->mColors[0]){
			
			tmpVec.x = mesh->mColors[0][i].r;
			tmpVec.y = mesh->mColors[0][i].g;
			tmpVec.z = mesh->mColors[0][i].b;

		
		}
		else{
			tmpVec = defaultColor;
		}
		tmp.color = tmpVec;

		if (mesh->mTextureCoords[0]){
			tmpVec.x = mesh->mTextureCoords[0][i].x;
			tmpVec.y = mesh->mTextureCoords[0][i].y;
		}
		else{
			tmpVec.x = tmpVec.y = tmpVec.z = 0.0;
		}
		tmp.U = tmpVec.x;
		tmp.V = tmpVec.y;


		data.push_back(tmp);
	}


	for (int i = 0; i < mesh->mNumFaces; i++){
		
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++){

			indices.push_back(face.mIndices[j]);
		}
	}

	//aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	cout << "Amount of Diffuse Maps: " << mat->GetTextureCount(aiTextureType_DIFFUSE) << endl;
	cout << "Amount of Normal Maps: " << mat->GetTextureCount(aiTextureType_OPACITY) << endl; // For now we need to use opacity...internal max collada issue!
	cout << "Amount of Specular Maps: " << mat->GetTextureCount(aiTextureType_SPECULAR) << endl;

	cout << "total amount of maps: " << (mat->GetTextureCount(aiTextureType_DIFFUSE) + mat->GetTextureCount(aiTextureType_OPACITY) + mat->GetTextureCount(aiTextureType_SPECULAR) ) << endl;

	if ((mat->GetTextureCount(aiTextureType_DIFFUSE) + mat->GetTextureCount(aiTextureType_OPACITY) + mat->GetTextureCount(aiTextureType_SPECULAR)) > 0){
		
		for (int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); i++){
			
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
			
			textureData tmp;
			tmp.id = loadTexture(str.C_Str(),true);
			tmp.type = 0;
			textures.push_back(tmp);

		}

		for (int i = 0; i < mat->GetTextureCount(aiTextureType_OPACITY); i++){  // again its actually looking for the normal maps not opacity
			aiString str;
			mat->GetTexture(aiTextureType_OPACITY, i, &str);

			textureData tmp;
			tmp.id = loadTexture(str.C_Str(),true);
			tmp.type = 1;
			textures.push_back(tmp);

		}

		//SpecComponent
		for (int i = 0; i < mat->GetTextureCount(aiTextureType_SPECULAR); i++){  // again its actually looking for the normal maps not opacity
			aiString str;
			mat->GetTexture(aiTextureType_SPECULAR, i, &str);

			textureData tmp;
			tmp.id = loadTexture(str.C_Str(),true);
			tmp.type = 2;
			textures.push_back(tmp);

		}



	}
	else{
		textureData tmp;
		tmp.id = DEFAULTTEXTURE;
		textures.push_back(tmp);
	}
	

	meshes.push_back(new meshObject(&data, &indices, &textures));

	
}



void meshLoader::draw(unsigned int programId){
	for (int i = 0; i < meshes.size(); i++){
		meshes[i]->draw(programId);
	}
		
}


/*unsigned int meshLoader::loadTexture(const char* filename){

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

	cout << "TextureID: " << id << endl;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedImg->w, convertedImg->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedImg->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(img);
	SDL_FreeSurface(convertedImg);
	
	return id; 

}*/

unsigned int meshLoader::loadTexture(const char* filename, bool generate){

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

	cout << "TextureID: " << id << endl;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedImg->w, convertedImg->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedImg->pixels);

	if (generate)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*if (generate)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, convertedImg->w, convertedImg->h, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedImg->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, convertedImg->w, convertedImg->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, convertedImg->pixels);*/
	

	if (generate)
		glGenerateMipmap(GL_TEXTURE_2D);
	
	
	SDL_FreeSurface(img);
	SDL_FreeSurface(convertedImg);

	return id;

}



vector<meshObject*>& meshLoader::getMeshes(){
	return meshes;
}