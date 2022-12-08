#pragma once
#include "GL/glew.h"

#include "Mesh.h"
#include "assimp/scene.h"


#include <string>
#include <list>
#include <vector>
#include "Geometry/AABB.h"

using namespace std;

class Model
{

public:
	Model();
	~Model();

	void Draw();
	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);
	void LoadMeshes(aiMesh** i_meshes, int i_numMeshes);
	const AABB* GetAABB();
	const aiScene* GetScene();

private:

	const char* texture_pathname;
	vector<GLuint> m_materials;
	list<Mesh*> meshes;
	AABB* aabb;
	const aiScene* scene;
};

