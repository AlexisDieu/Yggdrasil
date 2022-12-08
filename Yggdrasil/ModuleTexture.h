#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex-oct2022/DirectXTex/DirectXTex.h"
#include "GL/glew.h"

#include <string>

using namespace std;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	bool Start();
	bool CleanUp();
	DirectX::ScratchImage LoadImageFromFile(string imagepath, const char* modelTexturepath);
	HRESULT LoadTestPath(string imagepath, DirectX::ScratchImage& image);
	GLuint LoadTextureFromFile(string imagepath, const char* modelTexturepath);
	void LoadInformationFromFile(const DirectX::ScratchImage& i_image, 
		GLsizei& width, GLsizei& height,
		GLint& internalFormat,
		GLenum& format, GLenum& type);

	std::string GetDirectory(const std::string& fname);

	DirectX::ScratchImage GetImage();

private:

	unsigned char* data;

	string imagepath;
	const char* fbxTexturepath;

	friend class ModuleEditor;
};