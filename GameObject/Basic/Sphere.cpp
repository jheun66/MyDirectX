#include "Framework.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	:radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	material = new Material();

	CreateMesh();

	fillMode[0] = new RasterizerState();
	fillMode[1] = new RasterizerState();
}

Sphere::Sphere(wstring shaderFile, float radius, UINT sliceCount, UINT stackCount)
	:radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	material = new Material(shaderFile);

	CreateMesh();

	fillMode[0] = new RasterizerState();
	fillMode[1] = new RasterizerState();
}

Sphere::~Sphere()
{
	delete material;
	delete mesh;

	delete fillMode[0];
	delete fillMode[1];
}

void Sphere::Update()
{
	UpdateWorld();
}

void Sphere::Render()
{
	mesh->Set();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	SetWorldBuffer();

	material->Set();

	fillMode[1]->SetState();
	DC->DrawIndexed(indices.size(), 0, 0);
	fillMode[0]->SetState();

}

void Sphere::CreateMesh()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;
		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;
			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.position = Vector3(vertex.normal) * radius;

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = (float)i / stackCount;
			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.emplace_back((sliceCount + 1) * i + j);
			indices.emplace_back((sliceCount + 1) * i + j + 1);
			indices.emplace_back((sliceCount + 1) * (i + 1) + j);

			indices.emplace_back((sliceCount + 1) * (i + 1) + j);
			indices.emplace_back((sliceCount + 1) * i + j + 1);
			indices.emplace_back((sliceCount + 1) * (i + 1) + j + 1);
		}
	}

	CreateTangent();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(), indices.data(), indices.size());
}

void Sphere::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		XMFLOAT2 uv0 = vertex0.uv;
		XMFLOAT2 uv1 = vertex1.uv;
		XMFLOAT2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		vertices[index0].tangent = tangent + vertices[index0].tangent;
		vertices[index1].tangent = tangent + vertices[index1].tangent;
		vertices[index2].tangent = tangent + vertices[index2].tangent;

	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();

		vertex.tangent = temp;
	}


}

//#include "Framework.h"
//#include "Sphere.h"
//
//Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
//    :radius(radius), sliceCount(sliceCount), stackCount(stackCount)
//{
//    material = new Material(L"NormalMapping");
//    // 기본 텍스처처럼 사용
//    material->SetDiffuseMap(L"LandScape/Fieldstone_DM.tga");
//    material->SetSpecularMap(L"LandScape/Fieldstone_SM.tga");
//    material->SetNormalMap(L"LandScape/Fieldstone_NM.tga");
//
//    CreateData();
//
//    mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
//        indices.data(), indices.size());
//
//    fillMode[0] = new RasterizerState();
//    fillMode[1] = new RasterizerState();
//    fillMode[1]->FillMode(D3D11_FILL_WIREFRAME);
//}
//
//Sphere::~Sphere()
//{
//    delete fillMode[0];
//    delete fillMode[1];
//
//    delete mesh;
//    delete material;
//}
//
//void Sphere::Update()
//{
//    UpdateWorld();
//}
//
//void Sphere::Render()
//{
//    mesh->Set();
//
//    SetWorldBuffer();
//
//    material->Set();
//
//    //fillMode[1]->SetState();
//    DC->DrawIndexed(indices.size(), 0, 0);
//    //fillMode[0]->SetState();
//}
//
//void Sphere::CreateData()
//{
//    int stack = stackCount;
//    int sector = sliceCount;
//
//    // y 축(스택)은 반원만 하면 됨
//    float stackStep = (float)XM_PI / stack;
//    float sectorStep = (float)XM_2PI / sector;
//
//    float radius = 1.0f;
//
//    for (int i = 0; i <= stack; i++)
//    {
//        float pi = XM_PIDIV2 - stackStep * i;
//        for (int j = 0; j <= sector; j++)
//        {
//            float theta = sectorStep * j;
//
//            VertexType vertex;
//
//            vertex.normal.x = (radius * cos(pi)) * cos(theta);
//            vertex.normal.y = (radius * sin(pi));
//            vertex.normal.z = (radius * cos(pi)) * sin(theta);
//
//            // Float3로는 바로 못 곱해서
//            vertex.position = Vector3(vertex.normal) * radius;
//            
//            vertex.uv = XMFLOAT2(j / (float)sector, i / (float)stack);
//
//            vertices.emplace_back(vertex);
//        }
//    }
//    
//    for (int i = 0; i < stack; i++)
//    {
//        int k1 = i * (sector + 1);
//        int k2 = k1 + sector + 1;
//
//        for (int j = 0; j < sector; j++, k1++, k2++)
//        {
//            if (i != 0)
//            {
//                indices.push_back(k1);
//                indices.push_back(k1 + 1);
//                indices.push_back(k2);
//            }
//
//            if (i != stack - 1)
//            {
//                indices.push_back(k1 + 1);
//                indices.push_back(k2 + 1);
//                indices.push_back(k2);
//            }
//        }
//    }
//
//    CreateTangent();
//}
//
//void Sphere::CreateTangent()
//{
//    for (UINT i = 0; i < indices.size() / 3; i++)
//    {
//        UINT index0 = indices[i * 3 + 0];
//        UINT index1 = indices[i * 3 + 1];
//        UINT index2 = indices[i * 3 + 2];
//
//        VertexType vertex0 = vertices[index0];
//        VertexType vertex1 = vertices[index1];
//        VertexType vertex2 = vertices[index2];
//
//        Vector3 p0 = vertex0.position;
//        Vector3 p1 = vertex1.position;
//        Vector3 p2 = vertex2.position;
//
//        XMFLOAT2 uv0 = vertex0.uv;
//        XMFLOAT2 uv1 = vertex1.uv;
//        XMFLOAT2 uv2 = vertex2.uv;
//
//        Vector3 e0 = p1 - p0;
//        Vector3 e1 = p2 - p0;
//
//        float u0 = uv1.x - uv0.x;
//        float u1 = uv2.x - uv0.x;
//        float v0 = uv1.y - uv0.y;
//        float v1 = uv2.y - uv0.y;
//
//        float d = 1.0f / (u0 * v1 - v0 * u1);
//
//        Vector3 tangent;
//        tangent = (v1 * e0 - v0 * e1) * d;
//
//        vertices[index0].tangent = tangent + vertices[index0].tangent;
//        vertices[index1].tangent = tangent + vertices[index1].tangent;
//        vertices[index2].tangent = tangent + vertices[index2].tangent;
//
//    }
//
//    for (VertexType& vertex : vertices)
//    {
//        Vector3 t = vertex.tangent;
//        Vector3 n = vertex.normal;
//
//        Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();
//
//        vertex.tangent = temp;
//    }
//}
