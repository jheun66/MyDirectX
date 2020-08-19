#include "Framework.h"
#include "Plane.h"

Plane::Plane(UINT _row, UINT _col)
	:row(_row), col(_col)
{
	vertexShader = Shader::AddVS(L"VertexUV");
	pixelShader = Shader::AddPS(L"PixelUV");

	Create();

	ScratchImage image;
	LoadFromWICFile(L"Textures/sana.jpeg", WIC_FLAGS_NONE, nullptr, image);

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);


	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FillMode(D3D11_FILL_WIREFRAME);
}

Plane::~Plane()
{
	delete vertexBuffer;
	delete indexBuffer;

	srv->Release();

	delete rasterizerState[1];
	delete rasterizerState[0];
}

void Plane::Update()
{
	
	UpdateWorld();
}

void Plane::Render()
{
	// TODO : keyboard ют╥б
	if (ImGui::Button("Frame", ImVec2(100, 100)))
	{
		isFrame = !isFrame;
	}

	if(isFrame)
		rasterizerState[1]->SetState();

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	SetWorldBuffer();

	DC->PSSetShaderResources(0, 1, &srv);

	vertexShader->Set();
	pixelShader->Set();

	DC->DrawIndexed(indexCount, 0, 0);

	rasterizerState[0]->SetState();
}

void Plane::Create()
{
	vector<VertexUV> vertices;
	float y = (float)col / 2;
	float x = -1 * ((float)row / 2);
	for (int i = 0; i < col + 1; i++)
	{
		for (int j = 0; j < row + 1; j++)
		{
			VertexUV v;
			v.position = { x, y ,0 };
			v.uv = { j / (float)row, i / (float)col };
			vertices.push_back(v);
			x += 1;
		}
		x = -1 * ((float)row / 2);
		y -= 1;
	}

	int index = 0;
	vector<UINT> indices;
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			index = i * (col+1) + j;
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(row + 1 + index + 1);
			indices.push_back(row + 1 + index + 1);
			indices.push_back(row + index + 1);
			indices.push_back(index);
		}
	}
	
	indexCount = indices.size();
	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}
