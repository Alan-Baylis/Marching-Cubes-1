#include "Mesh.hpp"
#include "Application.hpp"


Application::Application(void) : minValue(1.8f)
{
	generateBlobMesh();
}

Application::~Application(void)
{
	delete[] mcPoints;
}

void Application::paint(void)
{
	_mesh->paint(_painter);

	// Draw skybox last
	_skybox.paint(_painter);
}


void Application::tick(void)
{
	//regenerateTriangles();
}

void Application::generateBlobMesh(void)
{
	//boundary values for Marching Cubes
	const float MINX = -4.0f;
	const float MAXX = 5.0f;
	const float MINY = -8.0f;
	const float MAXY = 8.0f;//8.0f
	const float MINZ = -8.0f;
	const float MAXZ = 8.0f;
	
	//initialize data to be passed to Marching Cubes
	mcPoints = new glm::vec4[(nX + 1)*(nY + 1)*(nZ + 1)];
	glm::vec3 stepSize((MAXX - MINX) / nX, (MAXY - MINY) / nY, (MAXZ - MINZ) / nZ);
	for(int i = 0; i < nX + 1; i++)
		for(int j = 0; j < nY + 1; j++)
			for(int k = 0; k < nZ + 1; k++)
			{
				//glm::vec4 vert(MINX + i*stepSize.x, MINY + j*stepSize.y, MINZ + k*stepSize.z, 0);
				glm::vec4 vert(MINX + i*stepSize.x, MINY + j*stepSize.y, MINZ + k*stepSize.z, 0);
				vert.w = Potential2((glm::vec3) vert);
				//vert.w = -vert.y;// Potential((glm::vec3) vert);
				//vert.w += 0.5*sin(vert.x)*cos(vert.z);
				mcPoints[i*(nY + 1)*(nZ + 1) + j*(nZ + 1) + k] = vert;
			}

	int triangleCount = 0;
	TRIANGLE * Triangles;
	Triangles = MarchingCubes(nX, nY, nZ, (MAXX - MINX) / nX, (MAXY - MINY) / nY, (MAXZ - MINZ) / nZ, minValue, mcPoints, triangleCount);

	_mesh = make_shared<Mesh>(Triangles, triangleCount);
}


float Application::Potential(glm::vec3 p)
{
	glm::vec3 dp1 = glm::vec3(0.0, -2.0, 0.0) - p;
	glm::vec3 dp2 = glm::vec3(0.0, 2.0, 0.0) - p;
	glm::vec3 dp3 = glm::vec3(2.0, 2.0, 0.0) - p;
	glm::vec3 dp4 = glm::vec3(0.0, 0.0, 4.0) - p;
	glm::vec3 dp5 = glm::vec3(-0.5, 3.1, -1.0) - p;
	glm::vec3 dp6 = glm::vec3(0.0, 0.0, -4.0) - p;
	return 1 / glm::length(dp1) + 1 / glm::length(dp2) + 1 / glm::length(dp3) + 1 / glm::length(dp4) + 1 / glm::length(dp5) + 1 / glm::length(dp6);
}

float Application::Potential2(glm::vec3 p)
{
	glm::vec3 dp1 = glm::vec3(0.0, 3.0, 0.0) - p;
	glm::vec3 dp2 = glm::vec3(0.0, 3.0, 0.0) - p;
	glm::vec3 dp3 = glm::vec3(0.0, 3.0, 0.0) - p;
	glm::vec3 dp4 = glm::vec3(0.0, 3.0, 0.0) - p;
	return 1 / glm::length(dp1) + 1 / glm::length(dp2) + 1 / glm::length(dp3) + 1 / glm::length(dp4) + 1;
}



void Application::regenerateTriangles(void)
{
	//boundary values for Marching Cubes
	const float MINX = -4.0f;
	const float MAXX = 5.0f;
	const float MINY = -8.0f;
	const float MAXY = 0.0f;//8.0f
	const float MINZ = -8.0f;
	const float MAXZ = 8.0f;

	int triangleCount = 0;
	TRIANGLE * Triangles;
	Triangles = MarchingCubes(nX, nY, nZ, (MAXX - MINX) / nX, (MAXY - MINY) / nY, (MAXZ - MINZ) / nZ, minValue, mcPoints, triangleCount);

	_mesh = make_shared<Mesh>(Triangles, triangleCount);
}

