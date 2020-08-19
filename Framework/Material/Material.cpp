#include "framework.h"

Material::Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	texture = nullptr;
}

Material::~Material()
{
}

void Material::Add(wstring vertexShaderPath, wstring pixelShaderPath, wstring texturePath)
{
	vertexShader = Shader::AddVS(vertexShaderPath);
	pixelShader = Shader::AddPS(pixelShaderPath);

	texture = Texture::Add(texturePath);
}

void Material::Set(UINT textureSlot)
{
	vertexShader->Set();
	pixelShader->Set();
	texture->PSSet(textureSlot);
}
