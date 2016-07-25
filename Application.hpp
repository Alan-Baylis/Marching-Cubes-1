#pragma once
#include <memory>
#include <algorithm>
#include <vector>

#include "range.h"
#include "Skybox.hpp"
#include "Painter.hpp"

class Mesh;
class MeshCMS;

namespace cms
{
	class AlgCMS;
	template <typename T> class Isosurface_t;
	class Mesh;
}

class IsoFunction
{
	float sphereFunction(float x, float y, float z) const
	{
		return (x*x + y*y + z*z) - 2.f;
	}

	float cubeFunction(float x, float y, float z) const
	{
		return (std::max(fabs(x) - 2.f, std::max((fabs(y) - 2.f), fabs(z) - 2.f)));
	}

	float coneFunction(float x, float y, float z) const
	{
		return ((((x*x) + (y*y)) / 0.1f) - ((z - 1.f)*(z - 1.f)));
	}

	float antiTankFunction(float x, float y, float z) const
	{
		return ((x*x)*(y*y) + (x*x)*(z*z) + (y*y)*(z*z) - 0.01f);
	}

	float heartFunction(float x, float y, float z) const
	{
		return (powf((x*x + y*y + 2.f*(z*z) - 0.8f), 3.0) - y*y*y * (-0.02f*z*z + x*x));
	}

	float torusFunction(float x, float y, float z) const
	{
		float R = 0.45f; float r = 0.2f; float x0 = x - 0.25f;
		return (((x0*x0 + y*y + z*z + R*R - r*r)*(x0*x0 + y*y + z*z + R*R - r*r)) - (4.f*R*R)*(z*z + x0*x0));
	}

	float doubleTorusFunction(float x, float y, float z) const
	{
		return -(0.01f - x*x*x*x + 2.f*x*x*x*x*x*x - x*x*x*x*x*x*x*x + 2.f*x*x*y*y - 2.f*x*x*x*x*y*y - y*y*y*y - z*z);
	}

	float linkedToriiFunction(float x, float y, float z) const
	{
		float R = 0.9f; float r = 0.3f; float x0 = x - 0.5f; float x1 = x + 0.5f;
		return ((((x0*x0 + y*y + z*z + R*R - r*r)*(x0*x0 + y*y + z*z + R*R - r*r)) - (4.f*R*R)*(z*z + x0*x0)) *
			(((x1*x1 + y*y + z*z + R*R - r*r)*(x1*x1 + y*y + z*z + R*R - r*r)) - (4.f*R*R)*(x1*x1 + y*y)));
	}

	float(IsoFunction::*_func)(float x, float y, float z) const;

public:
	void set_function(unsigned int isofunc_idx)
	{
		switch(isofunc_idx)
		{
		case 0:
			_func = &IsoFunction::sphereFunction;
			break;
		case 1:
			_func = &IsoFunction::cubeFunction;
			break;
		case 2:
			_func = &IsoFunction::coneFunction;
			break;
		case 3:
			_func = &IsoFunction::heartFunction;
			break;
		case 4:
			_func = &IsoFunction::linkedToriiFunction;
			break;
		default:
			_func = &IsoFunction::sphereFunction;
			break;
		}
	}

	/** The overloaded function call operator **/
	float operator()(float x, float y, float z) const
	{
		return (this->*_func)(x, y, z);
	}

};

namespace c
{
	// CMS
	auto const boxSize = 16.0f;
	auto const minOctreeRes = 2u;
	auto const maxOctreeRes = 6u;
	auto const complexSurfaceThreshold = 0.95f;
	auto const halfSize = (c::boxSize / 2.0f);

	const cms::Range container[3] =
	{
		cms::Range(-halfSize,halfSize),
		cms::Range(-halfSize,halfSize),
		cms::Range(-halfSize,halfSize)
	};

	// MC
	auto const nX = static_cast<int>(pow(2, maxOctreeRes));
	auto const nY = nX;
	auto const nZ = nX;

	// boundary values for Marching Cubes
	auto const xmin = container[0].m_lower;
	auto const xmax = container[0].m_upper;
	auto const ymin = container[1].m_lower;
	auto const ymax = container[1].m_upper;
	auto const zmin = container[2].m_lower;
	auto const zmax = container[2].m_upper;

	auto const xrange = container[0].m_upper - container[0].m_lower;
	auto const yrange = container[1].m_upper - container[1].m_lower;
	auto const zrange = container[2].m_upper - container[2].m_lower;

	// other consts
	auto const no_models = 5u;
}

class Application
{
public:
	float minValue;

	Application();
	~Application();
	void paint();
	void tick();

	void generateMeshes(IsoFunction isofunc);
	void regenerateTriangles();

	Painter& getPainter() { return _painter; }

private:
	// MC ----------------------------------------
	float Potential(glm::vec3 p);
	float Potential2(glm::vec3 p);

	glm::vec4* mcPoints;
	std::vector<std::unique_ptr<Mesh>> MC_meshes;
	std::shared_ptr<Mesh> _mesh;
	// -------------------------------------------

	// CMS ---------------------------------------
	IsoFunction _if;
	std::unique_ptr<cms::Isosurface_t<IsoFunction> > iso;
	std::unique_ptr<cms::AlgCMS> _cms;
	std::unique_ptr<cms::Mesh> _cms_mesh;
	std::unique_ptr<MeshCMS> _cms_mesh_painter;
	std::vector<std::unique_ptr<cms::Mesh>> CMS_meshes;
	std::vector<std::unique_ptr<MeshCMS>> CMS_mesh_painters;
	// -------------------------------------------

	Painter _painter;
	Skybox _skybox;
};
