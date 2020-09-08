#include "framework.h"

WarHawk::WarHawk()
	:Model("WarHawk/WarHawk"), moveSpeed(20.0f), forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	SetShader(L"Model");
	settingBuffer = new MSBuffer();
	settingBuffer->data.option[0] = 0;
	settingBuffer->data.option[1] = 0;

    this->rotation= XMFLOAT3(0, XM_PI, 0);
	this->scale = XMFLOAT3(0.01f, 0.01f, 0.01f);

    // 초기화
    GetCursorPos(&mOldPos);

    matRotation = XMMatrixIdentity();

    origin = position;
}

WarHawk::~WarHawk()
{
	delete settingBuffer;
}

void WarHawk::Render()
{
	settingBuffer->SetPSBuffer(10);
	__super::Render();
}

void WarHawk::Update(float altitude)
{
    Move(altitude);
    Rotate();
	__super::Update();
}

void WarHawk::PostRender()
{
    ImGui::Begin("WarHawk", 0, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::SliderFloat3("Position", (float*)&position, -50, 50);
        ImGui::SliderFloat3("Rotation", (float*)&rotation, -XM_2PI, XM_2PI);
    }
    ImGui::End();
}

void WarHawk::Move(float altitude)
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
        origin += forward * moveSpeed * Time::Delta();
    if (KEY_PRESS('S'))
        origin -= forward * moveSpeed * Time::Delta();
    if (KEY_PRESS('A'))
        origin -= right * moveSpeed * Time::Delta();
    if (KEY_PRESS('D'))
        origin += right * moveSpeed * Time::Delta();
    if (KEY_PRESS('Q'))
        origin -= up * moveSpeed * Time::Delta();
    if (KEY_PRESS('E'))
        origin += up * moveSpeed * Time::Delta();

    position = origin + XMFLOAT3(0, altitude, 0);
}

void WarHawk::Rotate()
{
    // 모니터의 좌표를 구함(마우스가 화면 밖으로 나가도 상관 x)
    GetCursorPos(&mCurPos);
    Vector3 curVec = { (float)mCurPos.x, (float)mCurPos.y, 0 };
    Vector3 oldVec = { (float)mOldPos.x, (float)mOldPos.y, 0 };

    Vector3 dif = curVec - oldVec;

    if (Mouse::Get()->Press(1))
    {
        //rotation.x += dif.y * 1.0f * Time::Delta();
        rotation.y += dif.x * 1.0f * Time::Delta();

        //XMMATRIX rotX = XMMatrixRotationX(rotation.x);
        XMMATRIX rotY = XMMatrixRotationY(rotation.y);

        matRotation = rotY;

        right = XMVector3TransformNormal({ 1, 0, 0 }, matRotation);
        up = XMVector3TransformNormal({ 0, 1, 0 }, matRotation);
        forward = XMVector3TransformNormal({ 0, 0, 1 }, matRotation);
    }

    mOldPos = mCurPos;
}
