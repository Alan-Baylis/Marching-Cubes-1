#include <SOIL.h>

#include "Painter.hpp"
#include "Mesh.hpp"

Mesh::Mesh(TRIANGLE * triangles, unsigned int tc) : triangleCount(tc)
{
	this->triangles = triangles;

	this->verticesCount = triangleCount * 3;
	vertices = new Vertex[verticesCount];

	for(int i = 0; i < triangleCount; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			vertices[i * 3 + j].v = triangles[i].p[j];
			vertices[i * 3 + j].n = triangles[i].norm[j];
		}
	}

	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->setupMesh();
}

Mesh::~Mesh(void)
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	delete[] vertices;
	delete[] triangles;
}

void Mesh::paint(Painter& p) const
{
	p.paint(*this);
}

void Mesh::setupMesh(void)
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// glVertexAttribPointer(location = 0, vec3 = 3, dataType = GL_FLOAT, normalise = GL_FALSE, strideSize = sizeof(struct), dataBegin = 0)
	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	// Normal vector position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, n));

	glBindVertexArray(0);

	loadTextures();
}

void Mesh::loadTextures(void)
{
	// Load and create a texture 
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	{
		int width, height;
		unsigned char* image = SOIL_load_image("textures/brickwall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	// Load and create a texture 
	glGenTextures(1, &_normalmap);
	glBindTexture(GL_TEXTURE_2D, _normalmap); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	{
		int width, height;
		unsigned char* image = SOIL_load_image("textures/oilrush1_normal.png", &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	
}

