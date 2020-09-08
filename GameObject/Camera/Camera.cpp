#include "Framework.h"

Camera* Camera::instance = nullptr;

Camera::Camera()
    :Transform(), FOV(XM_PIDIV4)
{
    // 초기화
    GetCursorPos(&oldPos);

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

void Camera::Create()
{
    instance = new FreeCamera();
}

void Camera::Update()
{
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
