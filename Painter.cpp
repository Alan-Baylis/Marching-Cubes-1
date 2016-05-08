// GL Includes
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.hpp"
#include "MeshCMS.hpp"
#include "Skybox.hpp"
#include "Painter.hpp"

Painter::Painter() : shader(Shader("./shaders/normal_mapping_plane.vert", "./shaders/normal_mapping_plane.frag")),
					draw_normals_shader("./shaders/draw_normals.vert", "./shaders/draw_normals.geom", "./shaders/draw_normals.frag"),
					cms_shader("./shaders/default.vert", "./shaders/default.frag"),
					camera(Camera(glm::vec3(0.0f, 0.0f, 0.0f))), _wireframeMode(false)
{

}

void Painter::flipWireframeTrisMode(void)
{
	if(_wireframeMode == true)
	{
		_wireframeMode = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		_wireframeMode = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void Painter::paint(const Mesh& msh)
{
	const auto& VAO = msh.getVAO();
	const auto& texture = msh.getTexture();
	const auto& normalmap = msh.getNormalmap();
	{
		shader.Use();

		// Create transformations
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		projection = glm::perspective(camera.Zoom, 800.0f / 600.0f, 0.1f, 1000.0f);

		// Get their uniform location
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		GLint projLoc = glGetUniformLocation(shader.Program, "projection");
		GLint cameraPosLoc = glGetUniformLocation(shader.Program, "viewPos");

		// Pass them to the shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(cameraPosLoc, 1, &camera.Position[0]);

		glBindVertexArray(VAO);

		glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::getSkyboxTexture());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader.Program, "diffuseMap"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalmap);
		glUniform1i(glGetUniformLocation(shader.Program, "normalMap"), 1);

		glDrawArrays(GL_TRIANGLES, 0, msh.verticesCount);
		glBindVertexArray(0);
	}


	//draw_normals part
	/*{
		draw_normals_shader.Use();

		// Create transformations
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		projection = glm::perspective(camera.Zoom, 800.0f / 600.0f, 0.1f, 1000.0f);

		// Get their uniform location
		GLint viewLoc = glGetUniformLocation(draw_normals_shader.Program, "view");
		GLint modelLoc = glGetUniformLocation(draw_normals_shader.Program, "model");
		GLint projLoc = glGetUniformLocation(draw_normals_shader.Program, "projection");
		// Pass them to the shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, msh.verticesCount);
		glBindVertexArray(0);
	}*/
}

void Painter::paint(const MeshCMS & msh)
{
	const auto& VAO = msh.getVAO();
	//const auto& texture = msh.getTexture();
	//const auto& normalmap = msh.getNormalmap();
	{
		cms_shader.Use();

		// Create transformations
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 projection;

		view = camera.GetViewMatrix();
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, -8.0f));
		projection = glm::perspective(camera.Zoom, 800.0f / 600.0f, 0.1f, 1000.0f);

		// Get their uniform location
		GLint viewLoc = glGetUniformLocation(cms_shader.Program, "view");
		GLint modelLoc = glGetUniformLocation(cms_shader.Program, "model");
		GLint projLoc = glGetUniformLocation(cms_shader.Program, "projection");
		GLint cameraPosLoc = glGetUniformLocation(cms_shader.Program, "viewPos");

		// Pass them to the shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(cameraPosLoc, 1, &camera.Position[0]);

		glBindVertexArray(VAO);

		//glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::getSkyboxTexture());

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture);
		//glUniform1i(glGetUniformLocation(cms_shader.Program, "diffuseMap"), 0);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, normalmap);
		//glUniform1i(glGetUniformLocation(cms_shader.Program, "normalMap"), 1);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, msh.triangles_count, GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(0);
	}

	//draw_normals part
	{
	draw_normals_shader.Use();

	// Create transformations
	glm::mat4 view;
	glm::mat4 model;
	glm::mat4 projection;

	view = camera.GetViewMatrix();
	model = glm::translate(model, glm::vec3(6.0f, 0.0f, -8.0f));
	projection = glm::perspective(camera.Zoom, 800.0f / 600.0f, 0.1f, 1000.0f);

	// Get their uniform location
	GLint viewLoc = glGetUniformLocation(draw_normals_shader.Program, "view");
	GLint modelLoc = glGetUniformLocation(draw_normals_shader.Program, "model");
	GLint projLoc = glGetUniformLocation(draw_normals_shader.Program, "projection");
	// Pass them to the shaders
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, msh.triangles_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	}
}


void Painter::paint(const Skybox& sb)
{
	const auto& VAO = sb.getVAO();
	const auto& cubemapTexture = sb.getTexture();
	const auto& sshader = sb.getShader();

	sshader.Use();

	// Create transformations
	glm::mat4 view;
	glm::mat4 projection;

	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
	projection = glm::perspective(camera.Zoom, 800.0f / 600.0f, 0.1f, 1000.0f);

	// Get their uniform location
	GLint viewLoc = glGetUniformLocation(sshader.Program, "view");
	GLint projLoc = glGetUniformLocation(sshader.Program, "projection");
	// Pass them to the shaders
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Draw skybox first
	glDepthFunc(GL_LEQUAL); // Change depth function so depth test passes when values are equal to depth buffer's content

	// skybox cube
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(sshader.Program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); // Set depth function back to default
}

