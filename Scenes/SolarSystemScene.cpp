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
}

void SolarSystemScene::Update()
{
    XMVECTOR sunPosVec = XMLoadFloat3(&sun->position);
    XMVECTOR earthPosVec = XMLoadFloat3(&earth->position);
    XMVECTOR moonPosVec = XMLoadFloat3(&moon->position);
    XMVECTOR mercuryPosVec = XMLoadFloat3(&mercury->position);
    XMVECTOR venusPosVec = XMLoadFloat3(&venus->position);
    XMVECTOR marsPosVec = XMLoadFloat3(&mars->position);
    XMVECTOR jupiterPosVec = XMLoadFloat3(&jupiter->position);

    XMFLOAT3 dif1, dif2, dif3, dif4, dif5, dif6;
    XMStoreFloat3(&dif1, sunPosVec - earthPosVec);
    XMStoreFloat3(&dif2, sunPosVec - moonPosVec);
    XMStoreFloat3(&dif3, sunPosVec - mercuryPosVec);
    XMStoreFloat3(&dif4, sunPosVec - venusPosVec);
    XMStoreFloat3(&dif5, sunPosVec - marsPosVec);
    XMStoreFloat3(&dif6, sunPosVec - jupiterPosVec);

    earth->SetLight(dif1);
    moon->SetLight(dif2);
    mercury->SetLight(dif3);
    venus->SetLight(dif4);
    mars->SetLight(dif5);
    jupiter->SetLight(dif6);

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
    sun->PostRender();
    earth->PostRender();
    moon->PostRender();

    mercury->PostRender();
    venus->PostRender();
    mars->PostRender();
    jupiter->PostRender();
}
