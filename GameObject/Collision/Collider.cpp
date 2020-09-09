#include "Framework.h"
#include "Collider.h"

Collider::Collider()
{
    material = new Material(L"Shader");
    material->GetBuffer()->data.diffuse = { 0,1,0,1 };
}

Collider::~Collider()
{
    delete material;
    delete mesh;
}

bool Collider::IsCollision(Collider* collider)
{
    //enum���� ã�°� �����..

    // ���� ������ ��ȯ �ȸ����� ���� ��ȯ
    BoxCollider* coll = dynamic_cast<BoxCollider*>(collider);

    if (coll != nullptr)
        return IsBoxCollision(coll);

    return IsSphereCollision(static_cast<SphereCollider*>(collider));
}

void Collider::Render()
{
    UpdateWorld();

    RasterizerState fillMode[2];
    fillMode[1].FillMode(D3D11_FILL_WIREFRAME);
    if (dynamic_cast<BoxCollider*>(this) != nullptr)
    {
        fillMode[1].SetState();
        mesh->Set(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    else
    {
        mesh->Set(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    }

    worldBuffer->SetVSBuffer(0);
    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
    fillMode[0].SetState();
}
