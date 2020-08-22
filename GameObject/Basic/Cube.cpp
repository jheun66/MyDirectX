#include "Framework.h"

Cube::Cube()
{
    material = new Material(L"Shader");

    CreateData();

    mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
        indices.data(), indices.size());

    colorBuffer = new ColorBuffer();
}

Cube::~Cube()
{
    delete colorBuffer;
    delete mesh;
    delete material;
}

void Cube::Update()
{
    UpdateWorld();
}

void Cube::Render()
{
    mesh->Set();

    SetWorldBuffer();

    material->Set();
    colorBuffer->SetBufferToPS(0);

    DC->DrawIndexed(indices.size(), 0, 0);
}

void Cube::CreateData()
{
    vertices.resize(8);
    vertices[0].position = { 0, 0, 0 };
    vertices[1].position = { 0, 1, 0 };
    vertices[2].position = { 1, 1, 0 };
    vertices[3].position = { 1, 0, 0 };

    vertices[4].position = { 0, 0, 1 };
    vertices[5].position = { 0, 1, 1 };
    vertices[6].position = { 1, 1, 1 };
    vertices[7].position = { 1, 0, 1 };

    indices =
    {
        // f
        0,1,2,
        0,2,3,

        // u
        1,5,6,
        1,6,2,

        // r
        3,2,6,
        3,6,7,

        // l
        0,5,1,
        0,4,5,

        // d
        0,7,4,
        0,3,7,

        // b
        4,6,5,
        4,7,6,
    };
}
