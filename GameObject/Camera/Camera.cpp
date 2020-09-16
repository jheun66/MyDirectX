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
    // 기본 카메라를 FreeCamera로
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
        ImGui::Text("CameraPosition : %.2f, %.2f, %.2f", position.x, position.y, position.z);
        ImGui::Text("CameraRotation : %.2f, %.2f, %.2f", rotation.x, rotation.y, rotation.z);
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

void Camera::ChangeCameraMode(bool isFollowCamera)
{
    delete instance;

    if (isFollowCamera)
        instance = new FollowCamera();
    else
        instance = new FreeCamera();
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
