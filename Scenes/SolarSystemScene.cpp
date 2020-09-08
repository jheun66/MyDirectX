#include "Framework.h"
#include "SolarSystemScene.h"

SolarSystemScene::SolarSystemScene()
{
    sun = new Orb();
    earth = new Orb();
    moon = new Orb();
    mercury = new Orb();
    venus = new Orb();
    mars = new Orb();
    jupiter = new Orb();


    sun->SetMaterial(L"Shader");
   

    earth->GetMaterial()->SetDiffuseMap(L"Earth/earthmap1k.jpg");
    earth->SetPivot(&(sun->position));
    earth->SetDistance(20);
    earth->SetRevSpeed(20);

    moon->GetMaterial()->SetDiffuseMap(L"Moon/Download-Moon-Texture-Nasa_.jpg");
    moon->SetPivot(&(earth->position));
    moon->SetDistance(3);
    moon->SetRevSpeed(80);
    moon->scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

    mercury->GetMaterial()->SetDiffuseMap(L"2k_mercury.jpg");
    mercury->SetPivot(&(sun->position));
    mercury->SetDistance(7);
    mercury->SetRevSpeed(20);
    mercury->scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

    venus->GetMaterial()->SetDiffuseMap(L"2k_venus_surface.jpg");
    venus->SetPivot(&(sun->position));
    venus->SetDistance(13);
    venus->SetRevSpeed(30);
    venus->scale = XMFLOAT3(0.7f, 0.7f, 0.7f);

    mars->GetMaterial()->SetDiffuseMap(L"2k_mars.jpg");
    mars->SetPivot(&(sun->position));
    mars->SetDistance(40);
    mars->SetRevSpeed(40);

    jupiter->GetMaterial()->SetDiffuseMap(L"2k_jupiter.jpg");
    jupiter->SetPivot(&(sun->position));
    jupiter->SetDistance(53);
    jupiter->SetRevSpeed(10);
    jupiter->scale = XMFLOAT3(2.5f, 2.5f, 2.5f);

    lightBuffer = new PointLightBuffer();
    colorBuffer = new ColorBuffer();
    colorBuffer->data.color = { 1,0,0,0 };
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
    delete colorBuffer;
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
    lightBuffer->SetVSBuffer(3);
    {
        colorBuffer->SetPSBuffer(0);
        sun->Render();
    }
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
}
