#include "Framework.h"

Quad::Quad()
{
	material = new Material(L"UV");
	material->SetDiffuseMap(L"sana.jpeg");

	CreateData();
	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

Quad::~Quad()
{
	delete mesh;
	delete material;
}

void Quad::Update()
{
	UpdateWorld();
}

void Quad::Render()
{
	mesh->Set();

	SetWorldBuffer();

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Quad::CreateData()
{
	vertices.resize(4);
	vertices[0].position = { -0.5f,0.5f,0 };
	vertices[1].position = { 0.5f,0.5f,0 };
	vertices[2].position = { -0.5f,-0.5f,0 };
	vertices[3].position = { 0.5f,-0.5f,0 };

	vertices[0].uv = { 0,0 };
	vertices[1].uv = { 1,0 };
	vertices[2].uv = { 0,1 };
	vertices[3].uv = { 1,1 };

	indices = {
		0,1,2,
		2,1,3
	};

}
