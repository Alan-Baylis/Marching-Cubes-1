#pragma once
#include "Shader.hpp"
#include "Camera.hpp"

class Mesh;
class MeshCMS;
class Skybox;

class Painter
{
public:
	Painter();

	void paint(const Mesh& msh);
	void paint(const MeshCMS& msh);
	void paint(const Skybox& sb);

	Camera& getCamera() { return camera; }
	void flipWireframeTrisMode();


private:
	Shader shader;
	Shader draw_normals_shader;
	Shader cms_shader;
	Camera camera;
	bool _wireframeMode;
};
