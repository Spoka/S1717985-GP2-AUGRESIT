#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "ObjLoader.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& position, const glm::vec2& textureCoord)
	{
		this->position = position;
		this->textureCoord = textureCoord;
		this->normal = normal;
	}

	glm::vec3* GetPosition() { return &position; }
	glm::vec2* GetTextureCoord() { return &textureCoord; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 position;
	glm::vec2 textureCoord;
	glm::vec3 normal;
};


class Mesh
{
public:
	Mesh();
	~Mesh();


	void draw();
	void initialise(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void loadModel(const std::string& filename);
	void initialiseModel(const IndexedModel& model);

private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};