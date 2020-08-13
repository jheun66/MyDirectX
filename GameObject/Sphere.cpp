#include "Framework.h"
#include "Sphere.h"

Sphere::Sphere()
{
    vertexShader = new VertexShader(L"Shaders/VertexShaders/VertexShader.hlsl");
    pixelShader = new PixelShader(L"Shaders/PixelShaders/PixelShader.hlsl");

    Create();
    colorBuffer = new ColorBuffer();
}

Sphere::~Sphere()
{
    delete vertexShader;
    delete pixelShader;
    delete colorBuffer;
    delete vertexBuffer;
    delete indexBuffer;
}

void Sphere::Update()
{
    UpdateWorld();
}

void Sphere::Render()
{
    vertexBuffer->Set();
    indexBuffer->Set();
    IASetPT();

    SetWorldBuffer();
    colorBuffer->SetBufferToPS(0);

    vertexShader->Set();
    pixelShader->Set();

    DC->DrawIndexed(indexCnt, 0, 0);
}

void Sphere::Create()
{
    vector<Vertex> vertices;

    int stack = 50;
    int sector = 50;

    // y 축(스택)은 반원만 하면 됨
    float stackStep = (float)XM_PI / stack;
    float sectorStep = (float)XM_2PI / sector;

    float radius = 1.0f;

    for (int i = 0; i <= stack; i++)
    {
        for (int j = 0; j <= sector; j++)
        {
            float pi = XM_PIDIV2 - stackStep * i;
            float theta = sectorStep * j;

            Vertex vt;

            vt.position.x = (radius * XMScalarCos(pi)) * XMScalarCos(theta);
            vt.position.y = (radius * XMScalarSin(pi));
            vt.position.z = (radius * XMScalarCos(pi)) * XMScalarSin(theta);

            vertices.push_back(vt);
        }
    }

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());

    vector<UINT> indices;
    for (int i = 0; i < stack; i++)
    {
        int k1 = i * (sector + 1);
        int k2 = k1 + sector + 1;

        for (int j = 0; j < sector; j++, k1++, k2++)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k1 + 1);
                indices.push_back(k2);
            }

            if (i != stack - 1)
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2 + 1);
                indices.push_back(k2);
            }
        }
    }
    indexCnt = indices.size();
    indexBuffer = new IndexBuffer(indices.data(), indexCnt);
}
