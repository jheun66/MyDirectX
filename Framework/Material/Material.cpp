#include "framework.h"

// VertexShader와 PixelShader가 앞 단어만 다를 경우
Material::Material(wstring file)
	:diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	vertexShader = Shader::AddVS(L"Vertex" + file);
	pixelShader = Shader::AddPS(L"Pixel" + file);
}

Material::Material(wstring vsFile, wstring psFile)
	:diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	vertexShader = Shader::AddVS(vsFile);
	pixelShader = Shader::AddPS(psFile);
}

Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr),
	vertexShader(vertexShader), pixelShader(pixelShader)
{
}

Material::~Material()
{
}

// diffuse, specular, normal 순으로 텍스처 slot에 넣어주어야함(hlsl에서)
void Material::Set()
{
	if (diffuseMap != nullptr)
		diffuseMap->PSSet(0);

	if (specularMap != nullptr)
		specularMap->PSSet(1);

	if (normalMap != nullptr)
		normalMap->PSSet(2);

	vertexShader->Set();
	pixelShader->Set();
}
