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

	// �ش� ���� ������ ������ hasMap 1�� ����
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
