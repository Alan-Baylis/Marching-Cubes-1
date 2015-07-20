#pragma once
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.hpp"

class Painter;

class Skybox
{
public:
	Skybox();
	~Skybox();
	void paint(Painter& p) const;

	GLuint getVAO() const { return VAO; }
	GLuint getTexture() const { return _texture; }
	static GLuint getSkyboxTexture() { return _texture; }
	const Shader& getShader() const { return _shader; }

private:
	// Initializes all the buffer objects/arrays
	void setupVAO();
	void loadSkyboxTextures();
	GLuint loadCubemap(std::vector<const GLchar*>& faces);

	GLuint VAO, VBO;
	static GLuint _texture;
	Shader _shader;
	static GLfloat skyboxVertices[];
};
