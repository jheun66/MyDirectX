#include "Framework.h"

#include "Cube.h"
#include "Sphere.h"

#include "Robot.h"

Robot::Robot()
{
    Create();
}

Robot::~Robot()
{
	delete head;
	delete body;
	delete leftArm;
	delete rightArm;
	delete leftLeg;
	delete rightLeg;
}

void Robot::Update()
{
    static float rot = 0;
    if (KeyPress(VK_RIGHT))
    {
        isMoving = true;
        position.x += 0.01f;
        rot += 0.001f;
        leftArm->rotation.y = (float)sin(rot);
        rightArm->rotation.y = (float)sin(rot);
        leftLeg->rotation.x = (float)sin(rot);
        rightLeg->rotation.x = -(float)sin(rot);
    }
    else if (KeyPress(VK_LEFT))
    {
        isMoving = true;
        position.x -= 0.01f;
        rot += 0.001f;
        leftArm->rotation.y = (float)sin(rot);
        rightArm->rotation.y = (float)sin(rot);
        leftLeg->rotation.x = (float)sin(rot);
        rightLeg->rotation.x = -(float)sin(rot);
    }

    if (KeyPress(VK_UP))
    {
        isMoving = true;
        position.z += 0.01f;
        rot += 0.001f;
        leftArm->rotation.y = (float)sin(rot);
        rightArm->rotation.y = (float)sin(rot);
        leftLeg->rotation.x = (float)sin(rot);
        rightLeg->rotation.x = -(float)sin(rot);
    }
    else if (KeyPress(VK_DOWN))
    {
        isMoving = true;
        position.z -= 0.01f;
        rot += 0.001f;
        leftArm->rotation.y = (float)sin(rot);
        rightArm->rotation.y = (float)sin(rot);
        leftLeg->rotation.x = (float)sin(rot);
        rightLeg->rotation.x = -(float)sin(rot);
    }

    if (KeyPress('A'))
    {
        isAttack = true;
        rot += 0.001f;
        leftArm->rotation.y = (float)sin(rot);
        rightArm->rotation.y = (float)sin(rot);
        leftArm->rotation.z = rot;
        rightArm->rotation.z = rot;
    }
    else if (Keyboard::Get()->Up('A'))
    {
        isAttack = false;
    }


    if(Keyboard::Get()->Up(VK_RIGHT) || Keyboard::Get()->Up(VK_LEFT) || Keyboard::Get()->Up(VK_UP) || Keyboard::Get()->Up(VK_DOWN))
    {
        isMoving = false;
    }

    if (!isMoving && !isAttack)
    {
        leftArm->rotation = { 0, 0, -0.5f };
        rightArm->rotation = { 0, 0, 0.5f };
        leftLeg->rotation = { 0, 0, 0 };
        rightLeg->rotation = { 0, 0, 0 };
    }

    head->Update();
    body->Update();
    leftArm->Update();
    rightArm->Update();
    leftLeg->Update();
    rightLeg->Update();

    UpdateWorld();
}

void Robot::Render()
{
    head->Render();
    body->Render();
    leftArm->Render();
    rightArm->Render();
    leftLeg->Render();
    rightLeg->Render();
}

void Robot::Create()
{
    head = new Sphere();
    body = new Cube();
    leftArm = new Cube();
    rightArm = new Cube();
    leftLeg = new Cube();
    rightLeg = new Cube();

    head->SetColor({ 1,1,1,1 });
    body->SetColor({ 1,0,0,1 });
    leftArm->SetColor({ 0,1,0,1 });
    rightArm->SetColor({ 0,1,0,1 });
    leftLeg->SetColor({ 0,0,1,1 });
    rightLeg->SetColor({ 0,0,1,1 });

    head->position.y = 4;
    head->scale = XMFLOAT3(1.5f, 1.5f, 1.5f);

    body->scale = XMFLOAT3(3, 3, 1);

    leftArm->position.x = 4.5f;
    leftArm->position.y = 2;
    leftArm->SetPivot({ -1, 0, 0 });
    leftArm->scale = XMFLOAT3(3, 1, 1);
    leftArm->rotation = {0, 0, -0.5f };

    rightArm->position.x = -4.5f;
    rightArm->position.y = 2;
    rightArm->SetPivot({ 1, 0, 0 });
    rightArm->scale = XMFLOAT3(3, 1, 1);
    rightArm->rotation = { 0, 0, 0.5f };

    leftLeg->position.x = 2;
    leftLeg->position.y = -4;
    leftLeg->SetPivot({ 0, 1, 0 });
    leftLeg->scale = XMFLOAT3(1, 3, 1);

    rightLeg->position.x = -2;
    rightLeg->position.y = -4;
    rightLeg->SetPivot({ 0, 1, 0 });
    rightLeg->scale = XMFLOAT3(1, 3, 1);

    head->SetParent(this->GetWorld());
    body->SetParent(this->GetWorld());
    leftArm->SetParent(this->GetWorld());
    rightArm->SetParent(this->GetWorld());
    leftLeg->SetParent(this->GetWorld());
    rightLeg->SetParent(this->GetWorld());

}
