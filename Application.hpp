#pragma once
#include <memory>

#include "Skybox.hpp"
#include "Painter.hpp"

class Mesh;

class Application
{
public:
	float minValue;

	Application();
	~Application();
	void paint();
	void tick();

	void generateBlobMesh();
	void regenerateTriangles();

	Painter& getPainter() { return _painter; }

private:
	float Potential(glm::vec3 p);
	float Potential2(glm::vec3 p);

	const int nX = 40;
	const int nY = 40;
	const int nZ = 40;
	glm::vec4* mcPoints;

	Painter _painter;
	Skybox _skybox;
	std::shared_ptr<Mesh> _mesh;
};
