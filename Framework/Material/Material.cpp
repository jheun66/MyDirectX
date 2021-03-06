#include "framework.h"

Material::Material()
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr),
	vertexShader(nullptr), pixelShader(nullptr)
{
	buffer = new MaterialBuffer();
}

// VertexShader와 PixelShader가 앞 단어만 다를 경우
Material::Material(wstring file)
	:diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	buffer = new MaterialBuffer();

	vertexShader = Shader::AddVS(L"Vertex" + file);
	pixelShader = Shader::AddPS(L"Pixel" + file);
}

Material::Material(wstring vsFile, wstring psFile)
	:diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr)
{
	buffer = new MaterialBuffer();

	vertexShader = Shader::AddVS(vsFile);
	pixelShader = Shader::AddPS(psFile);
}

Material::Material(VertexShader* vertexShader, PixelShader* pixelShader)
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr),
	vertexShader(vertexShader), pixelShader(pixelShader)
{
	buffer = new MaterialBuffer();
}

Material::~Material()
{
	delete buffer;
}

// diffuse, specular, normal 순으로 텍스처 slot에 넣어주어야함(hlsl에서)
void Material::Set()
{
	// 오류나면 slot 번호 체크하기!!
	buffer->SetPSBuffer(1);

	// 해당 맵을 가지고 있으면 hasMap 1로 변경
	if (diffuseMap != nullptr)
	{
		buffer->data.hasMap[0] = 1;
		diffuseMap->PSSet(0);
	}

	if (specularMap != nullptr)
	{
		buffer->data.hasMap[1] = 1;
		specularMap->PSSet(1);
	}


	if (normalMap != nullptr)
	{
		buffer->data.hasMap[2] = 1;
		normalMap->PSSet(2);
	}

	vertexShader->Set();
	pixelShader->Set();
}

void Material::SetShader(wstring file)
{
	vertexShader = Shader::AddVS(L"Vertex" + file);
	pixelShader = Shader::AddPS(L"Pixel" + file);
}

void Material::SetShader(wstring vsFile, wstring psFile)
{
	vertexShader = Shader::AddVS(vsFile);
	pixelShader = Shader::AddPS(psFile);
}
