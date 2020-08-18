#include "framework.h"

Grid::Grid(wstring texturePath, wstring heightMapPath)
	:width(10), height(10), texture(nullptr), heightMap(nullptr)
{
	vertexShader = new VertexShader(L"Shaders/VertexShaders/VertexDiffuse.hlsl");
	pixelShader = new PixelShader(L"Shaders/PixelShaders/PixelDiffuse.hlsl");
	
	vertexShader2 = new VertexShader(L"Shaders/VertexShaders/VertexColorShader.hlsl");
	pixelShader2 = new PixelShader(L"Shaders/PixelShaders/PixelColorShader.hlsl");


	if (texturePath != L"")
	{
		texture = Texture::Add(texturePath);
	}
	if (heightMapPath != L"")
	{
		heightMap = Texture::Add(heightMapPath);
	}

	if (heightMap != nullptr)
	{
		width = heightMap->GetWidth() - 1;
		height = heightMap->GetHeight() - 1;
	}
	else if (texture != nullptr)
	{
		width = texture->GetWidth() - 1;
		height = texture->GetHeight() - 1;
	}

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

	delete vertexShader2;
	delete pixelShader2;

	delete vertexBuffer;
	delete indexBuffer;

	delete rasterizerState[0];
	delete rasterizerState[1];

	delete lightBuffer;
}

void Grid::CreateData()
{
	if (heightMap == nullptr)
	{
		for (UINT z = 0; z <= height; z++)
		{
			for (UINT x = 0; x <= width; x++)
			{
				VertexUVNormal vertex;
				vertex.position = XMFLOAT3(x, 0, z);
				vertex.uv = XMFLOAT2(x / (float)width, 1.0f - (z / (float)height));

				vertices.emplace_back(vertex);
			}
		}
	}
	else
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

	vertexBuffer2 = new VertexBuffer(pivotVertices.data(), sizeof(VertexColor), pivotVertices.size());
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

		VertexColor vc, vc2;
		vc.position.x = (vertices[index0].position.x + vertices[index1].position.x + vertices[index2].position.x) / 3;
		vc.position.y = (vertices[index0].position.y + vertices[index1].position.y + vertices[index2].position.y) / 3;
		vc.position.z = (vertices[index0].position.z + vertices[index1].position.z + vertices[index2].position.z) / 3;
		vc.color = { 1,0,0,1 };

		vc2.position.x = vc.position.x + XMVectorGetX(normal);
		vc2.position.y = vc.position.y + XMVectorGetY(normal);
		vc2.position.z = vc.position.z + XMVectorGetZ(normal);
		vc2.color = vc.color;

		pivotVertices.emplace_back(vc);
		pivotVertices.emplace_back(vc2);
	}
	
}

void Grid::Render()
{
	if(viewFrame)
		rasterizerState[1]->SetState();
	else
		rasterizerState[0]->SetState();

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetBufferToVS(0);
	lightBuffer->SetBufferToVS(3);

	if(texture != nullptr)
		texture->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);

	if (viewNormal)
	{
		vertexBuffer2->Set();
		IASetPT(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		vertexShader2->Set();
		pixelShader2->Set();

		DC->Draw(pivotVertices.size(), 0);
	}
}

void Grid::PostRender()
{
	ImGui::Begin("Grid", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{		
		ImGui::SliderFloat3("LightPos", (float*)&lightBuffer->data.position, -100, 100);
		ImGui::Checkbox("Frame", &viewFrame);
		ImGui::Checkbox("Normal", &viewNormal);
	}
	ImGui::End();
}
