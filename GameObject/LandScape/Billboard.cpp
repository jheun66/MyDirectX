#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(wstring diffuseFile)
{
	material = new Material(L"UV");
	material->SetDiffuseMap(diffuseFile);

	CreateMesh();
}

Billboard::~Billboard()
{
	delete material;
	delete mesh;
}

void Billboard::Update()
{
	//Vector3 rot = CAMERA->GetRot();
	//rotation.y = rot.y;
	//rotation = rot;

	Vector3 dir = position - Camera::Get()->position;

	rotation.y = atan2(dir.x, dir.z);

	UpdateWorld();
}

void Billboard::Render()
{
	SetWorldBuffer(0);
	mesh->Set();
	material->Set();

	DC->DrawIndexed(6, 0, 0);
}

void Billboard::CreateMesh()
{
	VertexUV vertices[4];
	vertices[0].position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[1].position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[2].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertices[3].position = XMFLOAT3(0.5f, -0.5f, 0.0f);

	vertices[0].uv = XMFLOAT2(0, 0);
	vertices[1].uv = XMFLOAT2(1, 0);
	vertices[2].uv = XMFLOAT2(0, 1);
	vertices[3].uv = XMFLOAT2(1, 1);


	UINT indices[] = {
		0,1,2,
		1,3,2
	};

	mesh = new Mesh(vertices, sizeof(VertexUV), 4, indices, 6);
}
