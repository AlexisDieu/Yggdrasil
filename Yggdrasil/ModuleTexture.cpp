#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "Application.h"
#include "ModuleEditor.h"

using namespace std;

ModuleTexture::ModuleTexture()
{

}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
    return true;
}

bool ModuleTexture::Start()
{


    return true;
}

bool ModuleTexture::CleanUp()
{

    return true;
}

DirectX::ScratchImage ModuleTexture::LoadImageFromFile(string imagepath,const char* fbxTexturepath)
{
    std::string directory = GetDirectory(fbxTexturepath);
    std::string textureDir("Texture/");

    DirectX::ScratchImage image = DirectX::ScratchImage();
    DirectX::ScratchImage resImage = DirectX::ScratchImage();

            std::string fileName = imagepath;
            directory.append(fileName);
            textureDir.append(fileName);

            if ((!FAILED(LoadTestPath(imagepath, image))))
            {
                App->editor->AddLog(("Loading texture :" + imagepath).c_str());
            }
            else if ((!FAILED(LoadTestPath(directory.c_str(), image))))
            {
                App->editor->AddLog(("Loading texture :" + directory).c_str());
            }
            else if ((!FAILED(LoadTestPath(textureDir.c_str(), image))))
            {
                App->editor->AddLog(("Loading texture :" + textureDir).c_str());
            }
    DirectX::FlipRotate(image.GetImages(),image.GetImageCount(),image.GetMetadata(),DirectX::TEX_FR_FLIP_VERTICAL,resImage);

    return resImage;
}

HRESULT ModuleTexture::LoadTestPath(string imagepath, DirectX::ScratchImage& image)
{
    HRESULT error;

    wstring imagepaths = wstring(imagepath.begin(), imagepath.end());
    const wchar_t* w_imagepath = imagepaths.c_str();;

    error = DirectX::LoadFromDDSFile(w_imagepath, DirectX::DDS_FLAGS_NONE, nullptr, image);

    if (FAILED(error))
       error = DirectX::LoadFromTGAFile(w_imagepath, DirectX::TGA_FLAGS_NONE, nullptr, image);

    if (FAILED(error))
       error =  DirectX::LoadFromWICFile(w_imagepath, DirectX::WIC_FLAGS_NONE, nullptr, image);

    return error;
}

GLuint ModuleTexture::LoadTextureFromFile(string imagepath,const char* fbxTexturepath)
{
    GLuint texture;
    App->editor->AddLog(("Model Path: "+(std::string)fbxTexturepath + "").c_str());
    DirectX::ScratchImage image = LoadImageFromFile(imagepath, fbxTexturepath);

    GLsizei width, height;
    GLenum format, type;
    GLint internalFormat;
    LoadInformationFromFile(image, width, height, internalFormat, format, type);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image.GetMetadata().width, image.GetMetadata().height, 0, format, type, image.GetPixels());

    glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

void ModuleTexture::LoadInformationFromFile(const DirectX::ScratchImage& i_image,
    GLsizei& width,
    GLsizei& height,
    GLint& internalFormat,
    GLenum& format,
    GLenum& type)
{
    DirectX::TexMetadata metadata = i_image.GetMetadata();
    switch (metadata.format)
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        internalFormat = GL_RGBA8;
        format = GL_RGBA;
        type = GL_UNSIGNED_BYTE;
        break;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
        internalFormat = GL_RGBA8;
        format = GL_BGRA;
        type = GL_UNSIGNED_BYTE;
        break;
    case DXGI_FORMAT_B5G6R5_UNORM:
        internalFormat = GL_RGB8;
        format = GL_BGR;
        type = GL_UNSIGNED_BYTE;
        break;
    default:
        assert(false && "Unsupported format");
    }
}

std::string ModuleTexture::GetDirectory(const std::string& fname)
{
    size_t pos = fname.find_last_of("\\/");
    return (std::string::npos == pos)
        ? ""
        : fname.substr(0, pos + 1);
}
