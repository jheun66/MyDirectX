#include "Framework.h"
#include "SphereScene.h"

SphereScene::SphereScene()
{
    sun = new Star();
    earth = new Star();
    moon = new Star();

    sun->SetColor({ 1,0,0,1 });
    earth->SetColor({ 0,1,0,1 });
    moon->SetColor({ 0,0,1,1 });

    earth->position.x = 5;
    earth->SetParent(sun->GetWorld());

    moon->position.x = 4;
    moon->SetParent(earth->GetWorld());
}

SphereScene::~SphereScene()
{
    delete sun;
    delete earth;
    delete moon;
}

void SphereScene::Update()
{
    sun->Update();
    earth->Update();
    moon->Update();
}

void SphereScene::PreRender()
{
}

void SphereScene::Render()
{
    sun->Render();
    earth->Render();
    moon->Render();
}

void SphereScene::PostRender()
{
}
