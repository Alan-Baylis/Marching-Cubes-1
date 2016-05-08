#pragma once
#include <memory>

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

public:
	/** The overloaded function call operator **/
	float operator()(float x, float y, float z) const
	{
		return sphereFunction(x, y, z);
	}

};

namespace c
{
	// CMS
	auto const boxSize = 16.0f;
	auto const minOctreeRes = 2u;
	auto const maxOctreeRes = 5u;
	auto const complexSurfaceThreshold = 0.85f;
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
}

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
	// MC ----------------------------------------
	float Potential(glm::vec3 p);
	float Potential2(glm::vec3 p);

	glm::vec4* mcPoints;
	std::shared_ptr<Mesh> _mesh;
	// -------------------------------------------

	// CMS ---------------------------------------
	IsoFunction _if;
	std::unique_ptr<cms::Isosurface_t<IsoFunction> > iso;
	std::unique_ptr<cms::AlgCMS> _cms;
	std::unique_ptr<cms::Mesh> _cms_mesh;
	std::unique_ptr<MeshCMS> _cms_mesh_painter;
	// -------------------------------------------

	Painter _painter;
	Skybox _skybox;
};
