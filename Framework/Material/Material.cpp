#include "framework.h"

Material::Material()
	: diffuseMap(nullptr), specularMap(nullptr), normalMap(nullptr),
	vertexShader(nullptr), pixelShader(nullptr)
{
	buffer = new MaterialBuffer();
}

// VertexShader�� PixelShader�� �� �ܾ �ٸ� ���
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

// diffuse, specular, normal ������ �ؽ�ó slot�� �־��־����(hlsl����)
void Material::Set()
{
	// �������� slot ��ȣ üũ�ϱ�!!
	buffer->SetBufferToPS(1);

	if (diffuseMap != nullptr)
		diffuseMap->PSSet(0);

	if (specularMap != nullptr)
		specularMap->PSSet(1);

	if (normalMap != nullptr)
		normalMap->PSSet(2);

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
