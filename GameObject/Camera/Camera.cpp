#include "Framework.h"

Camera::Camera(string tag)
    :Transform(tag), FOV(XM_PIDIV2), 
    forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0),
    moveSpeed(20.0f), rotSpeed(1.0f)
{
    // 초기화
    GetCursorPos(&oldPos);

    matRotation = XMMatrixIdentity();
    matView = XMMatrixIdentity();
    matPerspective = XMMatrixIdentity();
    
    viewBuffer = new ViewBuffer();

    CreateView();
	CreatePerspective();
}

Camera::~Camera()
{
	delete viewBuffer;
	delete projectionBuffer;
}

void Camera::Rotation()
{
    // 모니터의 좌표를 구함(마우스가 화면 밖으로 나가도 상관 x)
    GetCursorPos(&curPos);
    Vector3 curVec = { (float)curPos.x, (float)curPos.y, 0 };
    Vector3 oldVec = { (float)oldPos.x, (float)oldPos.y, 0 };

    Vector3 dif = curVec - oldVec;

    if (Mouse::Get()->Press(1))
    {
        rotation.x += dif.y * rotSpeed * Time::Delta();
        rotation.y += dif.x * rotSpeed * Time::Delta();

        XMMATRIX rotX = XMMatrixRotationX(rotation.x);
        XMMATRIX rotY = XMMatrixRotationY(rotation.y);

        matRotation = rotX * rotY;

        right = XMVector3TransformNormal({ 1, 0, 0 }, matRotation);
        up = XMVector3TransformNormal({ 0, 1, 0 }, matRotation);
        forward = XMVector3TransformNormal({ 0, 0, 1 }, matRotation);
    }

    oldPos = curPos;
}

void Camera::Move()
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
            position += forward * moveSpeed * Time::Delta();
        if (KEY_PRESS('S'))
            position -= forward * moveSpeed * Time::Delta();
        if (KEY_PRESS('A'))
            position -= right * moveSpeed * Time::Delta();
        if (KEY_PRESS('D'))
            position += right * moveSpeed * Time::Delta();
        if (KEY_PRESS('Q'))
            position -= up * moveSpeed * Time::Delta();
        if (KEY_PRESS('E'))
            position += up * moveSpeed * Time::Delta();
    }
}


void Camera::Update()
{
    Rotation();
    Move();

    UpdateWorld();

    CreateView();
    CreatePerspective();
}

void Camera::PostRender()
{
    ImGui::Begin("Camera", 0, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::SliderFloat3("CameraPosition", (float*)&position, -50, 50);
        ImGui::SliderFloat3("CameraRotation", (float*)&rotation, -XM_2PI, XM_2PI);
        ImGui::Text("CursorPos :%f, %f", (float)curPos.x, (float)curPos.y);
        ImGui::Text("CursorPos With Window : %f, %f", XMVectorGetX(Mouse::Get()->GetPosition()), XMVectorGetY(Mouse::Get()->GetPosition()));
    }
    ImGui::End();
}


void Camera::CreateView()
{
    //// eye, focus, up 벡터를 이용한 방법
    //Vector3 focus = position + forward;
	//matView = XMMatrixLookAtLH(position, focus, up);
	
    // 월드의 역행렬을 이용해서도 구할 수 있다.
    matView = XMMatrixInverse(nullptr, world);

    viewBuffer->Set(matView);
}

Ray Camera::ScreenPointToRay(Vector3 pos)
{
    XMFLOAT2 screenSize(WIN_WIDTH, WIN_HEIGHT);

    XMFLOAT2 point;
    point.x = ((2 * pos.x) / screenSize.x) - 1.0f;
    point.y = (((2 * pos.y) / screenSize.y) - 1.0f) * -1.0f;

    XMFLOAT4X4 temp;
    XMStoreFloat4x4(&temp, matPerspective);

    point.x /= temp._11;
    point.y /= temp._22;

    Ray ray;
    ray.position = position;

    XMMATRIX invView = XMMatrixInverse(nullptr, matView);

    Vector3 tempPos(point.x, point.y, 1.0f);

    ray.direction = XMVector3TransformNormal(tempPos, invView);
    ray.direction.Normalize();

    return ray;
}


void Camera::CreatePerspective()
{
    projectionBuffer = new MatrixBuffer();
 
    matPerspective = XMMatrixPerspectiveFovLH(FOV,
        WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(matPerspective);
}
