#include "Painter.hpp"

#include "MeshCMS.hpp"

MeshCMS::MeshCMS(cms::Mesh & cms_mesh) : translation_vector{ 0.0f }, _cms_mesh(cms_mesh)
{
	vertices_count = _cms_mesh.vertexCount();
	triangles_count = _cms_mesh.indexCount();
	setup_mesh();
}

MeshCMS::~MeshCMS()
{
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void MeshCMS::paint(Painter & p) const
{
	p.paint(*this);
}

void MeshCMS::setup_mesh()
{
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, _cms_mesh.vertexCount() * sizeof(float) * 3u, &_cms_mesh.getVertices()[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &this->VBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, _cms_mesh.normalCount() * sizeof(float) * 3u, &_cms_mesh.getNormals()[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//hm? czy nie tylko ficzery?
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _cms_mesh.indexCount(), &_cms_mesh.getIndices()[0], GL_STATIC_DRAW);

	// Vertex Positions & Normals
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_normals);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(1);
	//glVertexAttribDivisor(1, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
