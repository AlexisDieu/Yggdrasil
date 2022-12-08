#pragma once
#include <assimp/mesh.h>
#include <vector>
#include "GL/glew.h"


class Mesh
{
public:
	Mesh();
	~Mesh();
	static Mesh* Load(const aiMesh* i_mesh);
	void Draw(const std::vector<GLuint>& model_textures);
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	int GetNumVertices();
	int GetNumTriangles();
private:

	unsigned int vbo,ebo,vao;

	int material_index, num_vertices, num_triangles;
	friend class Model;

};

