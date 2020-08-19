#include "Framework.h"
#include "SolarSystemScene.h"

SolarSystemScene::SolarSystemScene()
{
    sun = new Star();
    earth = new Planet(L"Earth/earthmap1k.jpg", "earth");
    moon = new Planet(L"Moon/Download-Moon-Texture-Nasa_.jpg", "moon");

    mercury = new Planet(L"2k_mercury.jpg", "mercury");
    venus = new Planet(L"2k_venus_surface.jpg", "venus");
    mars = new Planet(L"2k_mars.jpg", "mars");
    jupiter = new Planet(L"2k_jupiter.jpg", "jupiter");


    sun->SetColor({ 1,0,0,1 });

    earth->position.x = 20;
    earth->SetPivot(&(sun->position));
    earth->revSpeed = 20.0f;

    moon->position.x = 23;
    moon->SetPivot(&(earth->position));
    moon->revSpeed = 80.0f;
    moon->scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

    mercury->position.x = 7;
    mercury->SetPivot(&(sun->position));
    mercury->revSpeed = 20.0f;
    mercury->scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

    venus->position.x = 13;
    venus->SetPivot(&(sun->position));
    venus->revSpeed = 30.0f;
    venus->scale = XMFLOAT3(0.7f, 0.7f, 0.7f);

    mars->position.x = 40;
    mars->SetPivot(&(sun->position));
    mars->revSpeed = 40.0f;

    jupiter->position.x = 53;
    jupiter->SetPivot(&(sun->position));
    jupiter->revSpeed = 80.0f;
    jupiter->scale = XMFLOAT3(2.5f, 2.5f, 2.5f);

    lightBuffer = new PointLightBuffer();
}

SolarSystemScene::~SolarSystemScene()
{
    delete sun;
    delete earth;
    delete moon;

    delete mercury;
    delete venus;
    delete mars;
    delete jupiter;

    delete lightBuffer;
}

void SolarSystemScene::Update()
{

    sun->Update();
    earth->Update();
    moon->Update();
    mercury->Update();
    venus->Update();
    mars->Update();
    jupiter->Update();
}

void SolarSystemScene::PreRender()
{
}

void SolarSystemScene::Render()
{
    lightBuffer->SetBufferToVS(3);

    sun->Render();
    earth->Render();
    moon->Render();
    mercury->Render();
    venus->Render();
    mars->Render();
    jupiter->Render();

}

void SolarSystemScene::PostRender()
{
    ImGui::SliderFloat3("LightPos", (float*)&lightBuffer->data.position, -100, 100);


    sun->PostRender();
    earth->PostRender();
    moon->PostRender();

    mercury->PostRender();
    venus->PostRender();
    mars->PostRender();
    jupiter->PostRender();
}
