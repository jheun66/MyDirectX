#include "Framework.h"

FreeCamera::FreeCamera()
    :Camera(), moveSpeed(20.0f), rotSpeed(3.0f)
{
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Move()
{
    if (KEY_PRESS(VK_RBUTTON))
    {
        if (KEY_PRESS(VK_LSHIFT))
        {
            moveSpeed = 40.0f;
        }
        else
        {
            moveSpeed = 20.0f;
        }

        if (KEY_PRESS('W'))
            position += Forward() * moveSpeed * Time::Delta();
        if (KEY_PRESS('S'))
            position -= Forward() * moveSpeed * Time::Delta();
        if (KEY_PRESS('A'))
            position -= Right() * moveSpeed * Time::Delta();
        if (KEY_PRESS('D'))
            position += Right() * moveSpeed * Time::Delta();
        if (KEY_PRESS('Q'))
            position -= Up() * moveSpeed * Time::Delta();
        if (KEY_PRESS('E'))
            position += Up() * moveSpeed * Time::Delta();
    }

    position -= 0.1f * Forward() * XMVectorGetZ(Mouse::Get()->GetMoveValue());
}

void FreeCamera::Rotation()
{
    Vector3 dif = Mouse::Get()->GetMoveValue();

    if (Mouse::Get()->Press(1))
    {
        rotation.x += dif.y * rotSpeed * Time::Delta();
        rotation.y += dif.x * rotSpeed * Time::Delta();
    }
}

void FreeCamera::Update()
{
    Move();
    Rotation();

    Camera::Update();
}

void FreeCamera::PostRender()
{
    ImGui::Begin("FreeCamera", 0, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::SliderFloat("CamMoveSpeed", &moveSpeed, 1, 50);
        ImGui::SliderFloat("CamRotSpeed", &rotSpeed, 2.0f, 5.0f);
    }
    ImGui::End();
}
