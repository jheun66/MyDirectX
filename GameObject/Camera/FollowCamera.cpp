#include "Framework.h"

FollowCamera::FollowCamera()
	:Camera(), distance(5), height(3), offset(0, 0, 20),
	moveDamping(5), rotDamping(0), destPos(0, 0, 0), destRot(0),
	rotY(0), rotSpeed(0.001f), zoomSpeed(0.1f),
	target(nullptr), isFP(false)
{
	matRotation = XMMatrixIdentity();
}

FollowCamera::~FollowCamera()
{
}

// World Matrix�δ� �ϱ� ����, LookAtLH ���
void FollowCamera::Update()
{
	if (target == nullptr)
		return;

	if (KEY_DOWN('F'))
	{
		isFP = !isFP;
	}

	if (isFP)
	{
		FPMove();
	}
	else
	{
		Move();
	}

	CreatePerspective();
}

void FollowCamera::Move()
{
	if (rotDamping > 0.0f)
	{
		if (target->rotation.y != destRot)
		{
			destRot = GameMath::Lerp(destRot, target->rotation.y, rotDamping * Time::Delta());
		}

		matRotation = XMMatrixRotationY(destRot);
	}
	else
	{
		MouseControl();

		matRotation = XMMatrixRotationY(rotY);
	}

	Vector3 forward = XMVector3TransformNormal(GameMath::kForward, matRotation);
	destPos = forward * -distance;

	// ī�޶� ��ġ
	destPos += target->WorldPos();
	
	// ī�޶� ����
	destPos.y += height;

	position = XMVectorLerp(position.data, destPos.data, moveDamping * Time::Delta());

	// ��Ȯ�� ĳ���͸� ���°� �ƴ϶� �߰� Offset�� ���
	Vector3 tempOffset = XMVector3TransformCoord(offset.data, matRotation);
	// updateworld�� ������. up���ͷ� (0,1,0) �ᵵ ���� 
	matView = XMMatrixLookAtLH(position.data, (target->WorldPos() + tempOffset).data, GameMath::kUp);

	viewBuffer->Set(matView);
}

void FollowCamera::FPMove()
{
	Vector3 tempPos = Vector3(0, 0, 0.2f);

	destRot = target->rotation.y;
	matRotation = XMMatrixRotationY(destRot);

	destPos = XMVector3TransformCoord(tempPos.data, matRotation);

	// ī�޶� ��ġ
	destPos += target->WorldPos();

	// ī�޶� ����
	destPos.y += 1.5f;

	position = destPos;

	// ��Ȯ�� ĳ���͸� ���°� �ƴ϶� �߰� Offset�� ���
	Vector3 tempOffset = XMVector3TransformCoord(offset.data, matRotation);
	// updateworld�� ������. up���ͷ� (0,1,0) �ᵵ ���� 
	matView = XMMatrixLookAtLH(position.data, (target->WorldPos() + tempOffset).data, GameMath::kUp);

	viewBuffer->Set(matView);
}

void FollowCamera::MouseControl()
{
	Vector3 value = Mouse::Get()->GetMoveValue();
	if (Mouse::Get()->Press(1))
	{
		rotY += value.x * rotSpeed;
	}

	distance += value.z * zoomSpeed;
	height += value.z * zoomSpeed;
}

void FollowCamera::PostRender()
{
	Camera::PostRender();
	ImGui::Begin("FollowCamera", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::SliderFloat("CamDistance", &distance, -10, 100);
		ImGui::SliderFloat("CamHeight", &height, 1, 100);
		ImGui::SliderFloat("CamMoveDamping", &moveDamping, 0, 30);
		ImGui::SliderFloat("CamRotDamping", &rotDamping, 0, 30);
		ImGui::SliderFloat3("CamOffset", (float*)&offset, -20.0f, 20.0f);
	}
	ImGui::End();
}
