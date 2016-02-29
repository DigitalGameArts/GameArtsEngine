#include "meshObject.h"


meshObject::meshObject(vector<vertexData>* vd, vector<unsigned int>* id, vector<textureData>* td){
	data = *vd;
	indices = *id;
	
	if (td)
		textures = *td;



	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(vertexData), &data[0], GL_STATIC_DRAW);


	glGenBuffers(1, &IND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

meshObject::~meshObject(){
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IND);
}

void meshObject::draw(unsigned int programId){

	int vertex = glGetAttribLocation(programId, "vertex"); //0
	int normal = glGetAttribLocation(programId, "normal"); //1
	int tangent = glGetAttribLocation(programId, "tangent"); //2
	int color = glGetAttribLocation(programId, "color"); //3
	int UV = glGetAttribLocation(programId, "UV"); //4




	// Binds Textures to the shader according to its Type
	string diffuseString = "diffuseMap";
	string normalString = "normalMap";
	string specularString = "specularMap";

	if (textures.size() > 0){
		for (int i = 0; i < textures.size(); i++){
			
			if (textures[i].type == 0) // 0 indicates that its a diffuse map -> PlEASE CHANGE TO ENUM
			{

				//glActiveTexture(GL_TEXTURE0+i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				glUniform1i(glGetUniformLocation(programId, (diffuseString + (char)(i + '0')).c_str()), i);


			}
			if (textures[i].type == 1) // 1 indicates that its a normal map -> PlEASE CHANGE TO ENUM 
			{
				//glActiveTexture(GL_TEXTURE0+i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				glUniform1i(glGetUniformLocation(programId, (normalString + (char)(i + '0')).c_str()), i);

			}
			if (textures[i].type == 2) // 1 indicates that its a normal map -> PlEASE CHANGE TO ENUM 
			{
				//glActiveTexture(GL_TEXTURE0+i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				glUniform1i(glGetUniformLocation(programId, (specularString + (char)(i + '0')).c_str()), i);

			}
	
			//cout << textures[i].id << endl;
			//cout << (str + (char)(i + '0')).c_str() << endl;
		}
	}
	else{
		glActiveTexture(NULL);
		glDisable(GL_TEXTURE_2D);
	}
	


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IND);


    glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(3*sizeof(float)));

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(6 * sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(9 * sizeof(float)));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(12 * sizeof(float)));


	/*glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);*/

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	
}