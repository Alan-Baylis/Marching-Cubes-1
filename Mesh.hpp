#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "MarchingCubes.h"

class Painter;

struct Vertex
{
	glm::vec3 v;
	glm::vec3 n;
};

class Mesh
{
public:
	Mesh(TRIANGLE * triangles, unsigned int triangleCount);
	~Mesh();

	void paint(Painter& p) const;

	GLuint getVAO() const { return VAO; }
	GLuint getNormalmap() const { return _normalmap; }
	GLuint getTexture() const { return _texture; }
	void loadTextures();

	TRIANGLE * triangles;
	int triangleCount;

	Vertex* vertices;
	int verticesCount;

private:
	// Initializes all the buffer objects/arrays
	void setupMesh();

	GLuint VAO, VBO;
	GLuint _normalmap, _texture;
};


