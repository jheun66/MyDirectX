#include "Framework.h"

Camera::Camera(string tag)
    :Transform(tag)
{
	CreatePerspective();
}

Camera::~Camera()
{
	delete viewBuffer;
	delete projectionBuffer;
}


void Camera::Update()
{
    XMFLOAT3 tmpPos = { 0, 0, 0 };

    GetCursorPos(&cursorPos);
    // 마우스 왼쪽 시점의 위치 저장
    if (Mouse::Get()->Down(1))
    {
        isDrag = true;
        // 이 경우 마우스가 화면 밖을 벗어갈때 문제
        //dragedPosition = Mouse::Get()->GetPosition();
        
        dragedPosition.x = cursorPos.x;
        dragedPosition.y = cursorPos.y;
    
    }

    if (Mouse::Get()->Up(1))
    {
        isDrag = false;
    }

    if (isDrag)
    {
        // 이 경우 마우스가 화면 밖을 벗어갈때 문제
        //XMVECTOR tmp = Mouse::Get()->GetPosition();
        //XMVECTOR diff = tmp - dragedPosition;
        //float x = XMVectorGetX(diff);
        //float y = XMVectorGetY(diff);

        float x = cursorPos.x - dragedPosition.x;
        float y = cursorPos.y - dragedPosition.y;

        rotation.y += x * Time::Delta();
        rotation.x += y * Time::Delta();

        //dragedPosition = tmp;
        dragedPosition.x = cursorPos.x;
        dragedPosition.y = cursorPos.y;

        if (Keyboard::Get()->Press(VK_LSHIFT))
        {
            if (Keyboard::Get()->Press('W'))
            {
                tmpPos.z += 20 * Time::Delta();
            }
            else if (Keyboard::Get()->Press('S'))
            {
                tmpPos.z -= 20 * Time::Delta();
            }

            if (Keyboard::Get()->Press('A'))
            {
                tmpPos.x -= 20 * Time::Delta();
            }
            else if (Keyboard::Get()->Press('D'))
            {
                tmpPos.x += 20 * Time::Delta();
            }

            if (Keyboard::Get()->Press('Q'))
            {
                tmpPos.y -= 20 * Time::Delta();
            }
            else if (Keyboard::Get()->Press('E'))
            {
                tmpPos.y += 20 * Time::Delta();
            }
        }
        else
        {
            if (Keyboard::Get()->Press('W'))
            {
                tmpPos.z += 10 * Time::Delta();
            }
            else if (Keyboard::Get()->Press('S'))
            {
                tmpPos.z -= 10 * Time::Delta();
            }

            if (Keyboard::Get()->Press('A'))
            {
                tmpPos.x -= 10 * Time::Delta();
            }
            else if (Keyboard::Get()->Press('D'))
            {
                tmpPos.x += 10 * Time::Delta();
            }

            if (Keyboard::Get()->Press('Q'))
            {
                tmpPos.y -= 10 * Time::Delta();
            }
            else if (Keyboard::Get()->Press('E'))
            {
                tmpPos.y += 10 * Time::Delta();
            }
        }
    }

    XMVECTOR tmpVec = XMVectorSet(tmpPos.x, tmpPos.y, tmpPos.z, 0);
    XMMATRIX R = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    tmpVec = XMVector4Transform(tmpVec, R);

    tmpPos.x = XMVectorGetX(tmpVec);
    tmpPos.y = XMVectorGetY(tmpVec);
    tmpPos.z = XMVectorGetZ(tmpVec);

    position.x += tmpPos.x;
    position.y += tmpPos.y;
    position.z += tmpPos.z;

    // Transform Update
    UpdateWorld();

    CreatePerspective();
}

void Camera::PostRender()
{
    ImGui::Begin("Camera", 0, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::SliderFloat3("CameraPosition", (float*)&position, -50, 50);
        ImGui::SliderFloat3("CameraRotation", (float*)&rotation, -50, 50);
        ImGui::Text("CursorPos :%f, %f", (float)cursorPos.x, (float)cursorPos.y);
        ImGui::Text("CursorPos With Window : %f, %f", XMVectorGetX(Mouse::Get()->GetPosition()), XMVectorGetY(Mouse::Get()->GetPosition()));
    }
    ImGui::End();
}


void Camera::CreatePerspective()
{
    viewBuffer = new MatrixBuffer();
    projectionBuffer = new MatrixBuffer();
    
    //// eye, focus, up 벡터를 이용한 방법
    //XMVECTOR eye = XMVectorSet(128, 50, -2, 0);
    //XMVECTOR focus = XMVectorSet(128, 0, 128, 0);
    //XMVECTOR up = XMVectorSet(0, 1, 0, 0);

    //XMMATRIX view = XMMatrixLookAtLH(eye, focus, up);


    XMMATRIX view = XMMatrixInverse(nullptr, world);

    viewBuffer->Set(view);

    XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV2,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(projection);
}
