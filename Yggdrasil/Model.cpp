#include "Model.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>



Model::Model()
{

}

Model::~Model()
{
}

void Model::Draw()
{
	for (std::list<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->Draw(m_materials);
	}
}

void Model::Load(const char* file_name)
{
	texture_pathname = file_name;
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene);
		LoadMeshes(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		App->editor->AddLog(("Error loading "+(std::string)file_name+ aiGetErrorString()).c_str());
	}
}

void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	m_materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			m_materials.push_back(App->texture->LoadTextureFromFile(file.data,texture_pathname));
		}
	}
}

void Model::LoadMeshes(aiMesh** i_meshes, int i_numMeshes)
{
	//load all the meshes, take the the max and min vertices then made a bouncingbox
	float3 min = float3(math::floatMax);
	float3 max = float3(math::floatMin);

	for (int i = 0; i < i_numMeshes; ++i) {
		Mesh* mesh = Mesh::Load(i_meshes[i]);
		meshes.push_back(mesh);

		aiVector3D* vertices = i_meshes[i]->mVertices;
		unsigned int numVertices = i_meshes[i]->mNumVertices;
		for (int j = 0; j < numVertices; ++j) {
			aiVector3D vert = vertices[j];
			//verify if the value is the max or min , if yes put in in the max or min vector
			max = float3(
				math::Max(max.x, vert.x),
				math::Max(max.y, vert.y),
				math::Max(max.z, vert.z));
			min = float3(
				math::Min(min.x, vert.x),
				math::Min(min.y, vert.y),
				math::Min(min.z, vert.z));
		}
	}
		//create de AABB to have the center later for Focus+orite on it
		aabb = new AABB(min, max);
}

const AABB* Model::GetAABB() {
	return aabb;
}
const aiScene* Model::GetScene() {
	return scene;
}