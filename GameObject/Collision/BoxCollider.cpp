#include "Framework.h"

BoxCollider::BoxCollider(Vector3 minBox, Vector3 maxBox)
    :minBox(minBox), maxBox(maxBox)
{
    CreateMesh();
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::IsCollision(IN Ray ray, OUT Contact* contact)
{
    float minDistance = FLT_MAX;
    Vector3 hitPoint;
    for (UINT i = 0; i < indices.size(); i += 3)
    {
        Vector3 p[3];
        for (UINT j = 0; j < 3; j++)
        {
            p[j] = XMVector3TransformCoord(Vector3(vertices[indices[i + j]].position), world);
        }
        float distance;
        if (TriangleTests::Intersects(ray.position.data, ray.direction.data, p[0].data, p[1].data, p[2].data, distance))
        {
            if (distance < minDistance)
            {
                minDistance = distance;
                hitPoint = ray.position + ray.direction * distance;
            }
             
        }
    }
    if (minDistance != FLT_MAX)
    {
        contact->distance = minDistance;
        contact->hitPoint = hitPoint;
        return true;
    }
    else
        return false;
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    Obb box1 = GetObb();
    Obb box2 = collider->GetObb();

    Vector3 D = box2.position - box1.position;

    for (UINT i = 0; i < 3; i++)
    {
        if (SeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (SeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (box1.axis[i] == box2.axis[j]) return true;
        }
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (SeperateAxis(D, box1.axis[i].Cross(box2.axis[j]), box1, box2)) return false;
        }
    }

    return true;
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    Obb box = GetObb();

    Vector3 D = collider->WorldPos() - box.position;

    for (UINT i = 0; i < 3; i++)
    {
        if (SeperateAxis(D, box.axis[i], box, collider->GetRadius())) return false;
    }

    if (SeperateAxis(D, (box.axis[0] + box.axis[1]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[0] - box.axis[2]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[1] + box.axis[2]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[1] - box.axis[2]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[2] + box.axis[0]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[2] - box.axis[0]).Normal(), box, collider->GetRadius())) return false;

    if (SeperateAxis(D, (box.axis[0] + box.axis[1] + box.axis[2]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[1] + box.axis[2] - box.axis[0]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[0] + box.axis[2] - box.axis[1]).Normal(), box, collider->GetRadius())) return false;
    if (SeperateAxis(D, (box.axis[0] + box.axis[1] - box.axis[2]).Normal(), box, collider->GetRadius())) return false;

    return true;
}

Obb BoxCollider::GetObb()
{
    Obb obb;
    obb.position = WorldPos();

    obb.axis[0] = Right();
    obb.axis[1] = Up();
    obb.axis[2] = Forward();

    obb.halfSize = (maxBox - minBox) * 0.5f;
    obb.halfSize *= scale;

    return obb;
}

Vector3 BoxCollider::GetMin()
{
    return XMVector2TransformCoord(minBox.data, world);
}

Vector3 BoxCollider::GetMax()
{
    return XMVector2TransformCoord(maxBox.data, world);
}

void BoxCollider::CreateMesh()
{
    vertices.emplace_back(minBox.x, minBox.y, minBox.z);
    vertices.emplace_back(minBox.x, maxBox.y, minBox.z);
    vertices.emplace_back(maxBox.x, maxBox.y, minBox.z);
    vertices.emplace_back(maxBox.x, minBox.y, minBox.z);

    vertices.emplace_back(minBox.x, minBox.y, maxBox.z);
    vertices.emplace_back(minBox.x, maxBox.y, maxBox.z);
    vertices.emplace_back(maxBox.x, maxBox.y, maxBox.z);
    vertices.emplace_back(maxBox.x, minBox.y, maxBox.z);

    indices = {
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

    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
}

bool BoxCollider::SeperateAxis(Vector3 position, Vector3 axis, Obb box1, Obb box2)
{
    float distance = abs(position.Dot(axis));

    float a = 0.0f;
    float b = 0.0f;

    a += abs((box1.axis[0] * box1.halfSize.x).Dot(axis));
    a += abs((box1.axis[1] * box1.halfSize.y).Dot(axis));
    a += abs((box1.axis[2] * box1.halfSize.z).Dot(axis));

    b += abs((box2.axis[0] * box2.halfSize.x).Dot(axis));
    b += abs((box2.axis[1] * box2.halfSize.y).Dot(axis));
    b += abs((box2.axis[2] * box2.halfSize.z).Dot(axis));

    return distance > a + b;
}

bool BoxCollider::SeperateAxis(Vector3 position, Vector3 axis, Obb box1, float radius)
{
    float distance = abs(position.Dot(axis));

    float a = 0.0f;

    a += abs((box1.axis[0] * box1.halfSize.x).Dot(axis));
    a += abs((box1.axis[1] * box1.halfSize.y).Dot(axis));
    a += abs((box1.axis[2] * box1.halfSize.z).Dot(axis));

    return distance > a + radius;
}
