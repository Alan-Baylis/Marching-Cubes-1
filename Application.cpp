#include "Mesh.hpp"
#include "MeshCMS.hpp"
#include "Application.hpp"

#include "mesh.h"
#include "algcms.h"
#include "isosurface_t.h"

int ADDRESS_SIZE = c::maxOctreeRes;

Application::Application(void) : minValue(1.8f)
{
	iso = make_unique<cms::Isosurface_t<IsoFunction>>(&_if);
	
	_cms = make_unique<cms::AlgCMS>(iso.get(), c::container, c::minOctreeRes, c::maxOctreeRes);
	_cms->setComplexSurfThresh(c::complexSurfaceThreshold);
	_cms_mesh = make_unique<cms::Mesh>();

	generateBlobMesh();
}

Application::~Application(void)
{
	_cms_mesh->exportOBJ("test_cms.obj");

	delete[] mcPoints;
}

void Application::paint(void)
{
	_mesh->paint(_painter);
	_cms_mesh_painter->paint(_painter);

	// Draw skybox last
	_skybox.paint(_painter);
}


void Application::tick(void)
{
	//regenerateTriangles();
}

void Application::generateBlobMesh(void)
{
	using namespace c;
	
	//initialize data to be passed to Marching Cubes
	mcPoints = new glm::vec4[(nX + 1)*(nY + 1)*(nZ + 1)];
	glm::vec3 stepSize(c::xrange / static_cast<float>(c::nX), c::yrange / static_cast<float>(c::nY), c::zrange / static_cast<float>(c::nZ));

	for (int i = 0; i < nX + 1; i++)
	{
		for (int j = 0; j < nY + 1; j++)
		{
			for (int k = 0; k < nZ + 1; k++)
			{
				glm::vec4 vert(xmin + static_cast<float>(i)*stepSize.x, ymin + static_cast<float>(j)*stepSize.y, zmin + static_cast<float>(k)*stepSize.z, 0.0f);
				//vert.w = Potential2((glm::vec3) vert);
				vert.w = _if(vert.x, vert.y, vert.z);
				mcPoints[i*(nY + 1)*(nZ + 1) + j*(nZ + 1) + k] = vert;
			}
		}
	}

	int triangleCount = 0;
	TRIANGLE * Triangles;
	Triangles = MarchingCubes(nX, nY, nZ, xrange / nX, yrange / nY, zrange / nZ, minValue, mcPoints, triangleCount);

	_mesh = make_shared<Mesh>(Triangles, triangleCount);

	_cms->extractSurface(*_cms_mesh);

	_cms_mesh_painter = make_unique<MeshCMS>(*_cms_mesh);
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
	glm::vec3 dp1 = glm::vec3(0.0, 4.0, 0.0) - p;
	glm::vec3 dp2 = glm::vec3(0.0, 4.0, 0.0) - p;
	glm::vec3 dp3 = glm::vec3(0.0, 4.0, 0.0) - p;
	glm::vec3 dp4 = glm::vec3(0.0, 4.0, 0.0) - p;
	return 1 / glm::length(dp1) + 1 / glm::length(dp2) + 1 / glm::length(dp3) + 1 / glm::length(dp4) + 1;
}

void Application::regenerateTriangles(void)
{
	int triangleCount = 0;
	TRIANGLE * Triangles;
	Triangles = MarchingCubes(c::nX, c::nY, c::nZ, c::xrange / c::nX, c::yrange / c::nY, c::zrange / c::nZ, minValue, mcPoints, triangleCount);

	_mesh = make_shared<Mesh>(Triangles, triangleCount);
}

