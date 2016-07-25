#include <chrono>

#include "Mesh.hpp"
#include "MeshCMS.hpp"
#include "Application.hpp"

#include "mesh.h"
#include "algcms.h"
#include "isosurface_t.h"

int ADDRESS_SIZE = c::maxOctreeRes;

Application::Application(void) : minValue(2.0f)
{
	MC_meshes.reserve(c::no_models);
	CMS_meshes.reserve(c::no_models);
	CMS_mesh_painters.reserve(c::no_models);

	generateMeshes(_if);
}

Application::~Application(void)
{
	// uropolinum
	std::vector<std::string> model_names;
	model_names = { "sphere" , "cube", "cone", "heart", "linkedTorii" };
	for(int model_idx = 0; model_idx < c::no_models; ++model_idx)
	{
		auto model_name = model_names[model_idx];
		CMS_meshes[model_idx]->exportOBJ(model_name + "_cms.obj");
		MC_meshes[model_idx]->exportOBJ(model_name + "_mc.obj");
	}
}

void Application::paint(void)
{
	//_mesh->paint(_painter);
	//_cms_mesh_painter->paint(_painter);

	for(int model_idx = 0; model_idx < c::no_models; ++model_idx)
	{
		MC_meshes[model_idx]->paint(_painter);
		CMS_mesh_painters[model_idx]->paint(_painter);
	}

	// Draw skybox last
	_skybox.paint(_painter);
}


void Application::tick(void)
{
	//regenerateTriangles();
}

void Application::generateMeshes(IsoFunction isofunc)
{
	using namespace c;

	using Clock = std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::seconds;
	using std::chrono::milliseconds;
	typedef std::chrono::duration<float> fsec;
	//using namespace std::literals::chrono_literals;
	
	for(int model_idx = 0; model_idx < c::no_models; ++model_idx)
	{
		isofunc.set_function(model_idx);
		// MC ---------------------------------------------------------------------
		//initialize data to be passed to Marching Cubes
		mcPoints = new glm::vec4[(nX + 1)*(nY + 1)*(nZ + 1)];
		glm::vec3 stepSize(c::xrange / static_cast<float>(c::nX), c::yrange / static_cast<float>(c::nY), c::zrange / static_cast<float>(c::nZ));

		for(int i = 0; i < nX + 1; i++)
		{
			for(int j = 0; j < nY + 1; j++)
			{
				for(int k = 0; k < nZ + 1; k++)
				{
					glm::vec4 vert(xmin + static_cast<float>(i) *stepSize.x, ymin + static_cast<float>(j) *stepSize.y, zmin + static_cast<float>(k) *stepSize.z, 0.0f);
					//vert.w = Potential2((glm::vec3) vert);
					vert.w = isofunc(vert.x, vert.y, vert.z);
					mcPoints[i*(nY + 1)*(nZ + 1) + j*(nZ + 1) + k] = vert;
				}
			}
		}

		int triangleCount = 0;
		TRIANGLE * Triangles;
		auto mc_start = Clock::now();
		Triangles = MarchingCubes(nX, nY, nZ, xrange / nX, yrange / nY, zrange / nZ, minValue, mcPoints, triangleCount);
		auto mc_end = Clock::now();
		auto mc_time = duration_cast<fsec>(mc_end - mc_start);

		std::cout << "mc time [" << model_idx << "]: " << mc_time.count() << " s.\n";

		delete[] mcPoints;

		auto mc_mesh = make_unique<Mesh>(Triangles, triangleCount);
		mc_mesh->set_translation(glm::vec3(0.0f, 0.0f, 20.0f*(model_idx - 2)));
		MC_meshes.emplace_back(std::move(mc_mesh));

		// CMS --------------------------------------------------------------------
		iso = make_unique<cms::Isosurface_t<IsoFunction>>(&isofunc);

		_cms = make_unique<cms::AlgCMS>(iso.get(), c::container, c::minOctreeRes, c::maxOctreeRes);
		_cms->setComplexSurfThresh(c::complexSurfaceThreshold);
		auto cms_mesh = make_unique<cms::Mesh>();

		auto cms_start = Clock::now();
		_cms->extractSurface(*cms_mesh);
		auto cms_end = Clock::now();

		auto cms_time = duration_cast<fsec>(cms_end - cms_start);

		std::cout << "cms time [" << model_idx << "]: " << cms_time.count() << " s.\n";

		auto cms_mesh_painter = make_unique<MeshCMS>(*cms_mesh);
		cms_mesh_painter->set_translation(glm::vec3(8.0f, 0.0f, 20.0f*(model_idx - 2)));

		CMS_meshes.emplace_back(std::move(cms_mesh));
		CMS_mesh_painters.emplace_back(std::move(cms_mesh_painter));
	}
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
	//int triangleCount = 0;
	//TRIANGLE * Triangles;
	//Triangles = MarchingCubes(c::nX, c::nY, c::nZ, c::xrange / c::nX, c::yrange / c::nY, c::zrange / c::nZ, minValue, mcPoints, triangleCount);

	//_mesh = make_shared<Mesh>(Triangles, triangleCount);
	generateMeshes(_if);
}

