#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../Models/RawModel.h"
#include <vector>
#include <string>

class Loader{
public:
	Loader();
	virtual ~Loader();

	//RawModel LoadToVAO(float positions[], int count);
	RawModel LoadToVAO(float vertices[], int indices[], float texCoords[], int vertCount, int indCount, int texCount);

	GLuint	 LoadTexture(const std::string& fileName);

	inline void UnbindVAO(){ glBindVertexArray(0); }
private:
	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> m_textures;
	GLuint CreateVAO();
	void StoreDataInAttributeList(GLuint attribNumber, int size,
		float data[],int& count);
	void BindIndicesBuffer(int indices[], int& count);
};

Loader::Loader(){
	vaos.clear();
	vbos.clear();
	m_textures.clear();
	//clear() cant release stored memory, just clear the data.
	//need to check the data one by one and delete them ,
	//is a better choice, check this part later.
	//reference page:http://www.cnblogs.com/summerRQ/articles/2407974.html
}

Loader:: ~Loader(){
	while (vbos.size() > 0)	{
		glDeleteBuffers(1, &vbos.back());
		vbos.pop_back();
	}
	while (vaos.size() > 0){
		glDeleteVertexArrays(1, &vaos.back());
		vaos.pop_back();
	}

	while (m_textures.size() > 0){
		glDeleteTextures(1, &m_textures.back());
		m_textures.pop_back();
	}
}

RawModel Loader::LoadToVAO(float vertices[], int indices[], float texCoords[], int vertCount, int indCount, int texCount){


	//create a new VAO
	GLuint vaoID = CreateVAO();
	BindIndicesBuffer(indices, indCount);

	//store the data in an attribute list
	StoreDataInAttributeList(0, 3,vertices, vertCount);
	StoreDataInAttributeList(1, 2, texCoords, texCount);
	UnbindVAO();
	return RawModel(vaoID, vertCount);

}
GLuint Loader::LoadTexture(const std::string& fileName){
	GLuint texture;
	int width, height, numComponents;

	//Load image data
	stbi_uc* imageData = stbi_load(
		("../res/textures/" + fileName + ".png").c_str(),
		&width, &height, &numComponents, 4);

	if (imageData == NULL){
		std::cerr << "Error:texture loading failed for" << fileName << std::endl;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// How OpenGL will fill an area that's to big or to small
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Store the OpenGL texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	// Store the buffer in the list
	m_textures.push_back(texture);

	// Unload image data
	stbi_image_free(imageData);

	return texture;
}

GLuint Loader::CreateVAO(){
	GLuint vaoID;
	
	//createa a new vao
	glGenVertexArrays(1, &vaoID);

	//store the vao in the list
	vaos.push_back(vaoID);

	//bind the vao to use it
	glBindVertexArray(vaoID);
	return vaoID;
}

void Loader::StoreDataInAttributeList(GLuint attribNumber,int size,
	float data[], int& count){

	GLuint vboID;
	//create a new buffer
	glGenBuffers(1, &vboID);
	//store the buffer in the list
	vbos.push_back(vboID);
	//bind the buffer to use it.
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	//store the data in the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(data)* count, data, GL_STATIC_DRAW);

	//tell opengl how and where to store this vbo in the vao
	glVertexAttribPointer(attribNumber, size, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Loader::BindIndicesBuffer(int indices[], int& count){
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*count, indices, GL_STATIC_DRAW);
}