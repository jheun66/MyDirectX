#include "framework.h"

Grid::Grid()
{
	vertexShader = new VertexShader(L"Shaders/VertexShaders/VertexDiffuse.hlsl");
	pixelShader = new PixelShader(L"Shaders/PixelShaders/PixelDiffuse.hlsl");

	texture = Texture::Add(L"Landscape/Dirt2.png");
	heightMap = Texture::Add(L"HeightMaps/ColorMap256.png");

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	CreateData();

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);

	worldBuffer = new MatrixBuffer();
	lightBuffer = new LightBuffer();
}

Grid::~Grid()
{
	delete vertexShader;
	delete pixelShader;

	delete vertexBuffer;
	delete indexBuffer;

	delete rasterizerState[0];
	delete rasterizerState[1];

	delete lightBuffer;
}

void Grid::CreateData()
{
	vector<XMFLOAT4> pixels = heightMap->ReadPixels();


	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexUVNormal vertex;
			vertex.position = XMFLOAT3(x, 0, z);
			vertex.uv = XMFLOAT2(x / (float)width, 1.0f - (z / (float)height));

			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * 20.0f;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x);
			indices.emplace_back((width + 1) * (z + 1) + x + 1);
			indices.emplace_back((width + 1) * z + x + 1);
		}
	}

	CreateNormal();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUVNormal), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

}

void Grid::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		XMVECTOR v0 = XMLoadFloat3(&vertices[index0].position);
		XMVECTOR v1 = XMLoadFloat3(&vertices[index1].position);
		XMVECTOR v2 = XMLoadFloat3(&vertices[index2].position);

		XMVECTOR A = v1 - v0;
		XMVECTOR B = v2 - v0;

		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(A, B));

		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}
}

void Grid::Render()
{
	//rasterizerState[1]->SetState();

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetBufferToVS(0);
	lightBuffer->SetBufferToVS(3);

	texture->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);

	rasterizerState[0]->SetState();
}

void Grid::PostRender()
{
	ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->data.direction, -1, 1);
}
