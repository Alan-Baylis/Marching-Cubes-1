#pragma once
#include <GL/glew.h>

#include "mesh.h"

class Painter;

class MeshCMS
{
public:
	MeshCMS(cms::Mesh & cms_mesh);
	~MeshCMS();

	void paint(Painter& p) const;

	GLuint getVAO() const { return VAO; }

	unsigned int vertices_count;
	unsigned int triangles_count;

private:
	void setup_mesh();

	cms::Mesh & _cms_mesh;
	GLuint VAO, VBO, EBO, VBO_normals;
};