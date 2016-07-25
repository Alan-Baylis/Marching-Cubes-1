#include <SOIL.h>

#include "Painter.hpp"
#include "Mesh.hpp"

Mesh::Mesh(TRIANGLE * triangles, unsigned int tc) : triangleCount(tc), translation_vector{0.0f}
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

#include <stdlib.h> //realpath
#include <sys/types.h>
#include <sys/stat.h> //mkdir
#include <direct.h> //VS _mkdir

#if defined(_WIN32) || defined(_WIN64)
// Copied from linux libc sys/stat.h:
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

bool Mesh::exportOBJ(const std::string& i_fName) const
{
	// Add output directory posix path to file name
	std::string outputDir = "./output/";
	std::string fullPath = "";

	// Set the fullPath
	fullPath = outputDir + i_fName;

	// If the output folder doesn't exist try to create one
	// if creating it was unsuccessful then modify fullPath so it just outputs to the build root
	struct stat sb;
	if(stat(outputDir.c_str(), &sb) != 0 || !S_ISDIR(sb.st_mode)) //dir doesn't exist
	{
		if(_mkdir(outputDir.c_str()) == -1)
		{
			std::cout << "Directory '" << outputDir << "' could not be created! \n" <<
				"The file will not be outputed in the root of the project build dir." <<
				std::endl;

			fullPath = i_fName;
		}
	}

	// Open the stream and parse
	std::fstream fileOut;
	fileOut.open(fullPath.c_str(), std::ios::out);

	if(fileOut.is_open())
	{
		fileOut << "# CMS Isosurface extraction." << std::endl;
		fileOut << "# George Rassovsky ~ goro.rassovsky@gmail.com \n" << std::endl;

		for(unsigned int i = 0; i<verticesCount; i += 1)
		{
			fileOut << "v " << vertices[i].v.x << " " << vertices[i].v.y << " " << vertices[i].v.z << std::endl;
		}

		// Write vertex normals
		for(unsigned int i = 0; i<verticesCount; i += 1)
		{
			fileOut << "vn " << vertices[i].n.x << " " << vertices[i].n.y << " " << vertices[i].n.z << std::endl;
		}

		// Write the face info
		for(unsigned f = 0; f<triangleCount; f += 1)
		{
			auto find_idx = [&](int xyz, int face)
			{
				int i = 0;
				for(; i < verticesCount; ++i)
				{
					if(triangles[face].p[xyz] == vertices[i].v)
						break;
				}
				return i;
			};
			fileOut << "f " << find_idx(0, f) + 1 << " " << find_idx(1, f) + 1 << " " << find_idx(2, f) + 1<< std::endl;
		}
		char * resolved = 0; // todo -- error handle realpath
		std::cout << "\nExported .py script path: not-realpath \t" << fullPath.c_str() << std::endl; // << realpath(fullPath.c_str(), resolved) << std::endl;
		return true;
	}
	else
	{
		std::cout << "File : " << i_fName << " Not founds " << std::endl;
		return false;
	}
}