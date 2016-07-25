#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

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
	bool exportOBJ(const std::string& i_fName) const;

	void set_translation(glm::vec3 tv) { translation_vector = tv; }

	TRIANGLE * triangles;
	int triangleCount;

	Vertex* vertices;
	int verticesCount;

	glm::vec3 translation_vector;

private:
	// Initializes all the buffer objects/arrays
	void setupMesh();

	GLuint VAO, VBO;
	GLuint _normalmap, _texture;
};


