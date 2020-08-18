#include "framework.h"
#include "Sphere.h"
#include "Star.h"

Star::Star()
{    
    vertexShader = new VertexShader(L"Shaders/VertexShaders/VertexShader.hlsl");
    pixelShader = new PixelShader(L"Shaders/PixelShaders/PixelShader.hlsl");

    colorBuffer = new ColorBuffer();
}

Star::~Star()
{   
    delete vertexShader;
    delete pixelShader;
    delete colorBuffer;
}

void Star::Render()
{
    colorBuffer->SetBufferToPS(0);

    vertexShader->Set();
    pixelShader->Set();

    __super::Render();
}

void Star::PostRender()
{
    ImGui::Begin("Star", 0, ImGuiWindowFlags_AlwaysAutoResize);
    {
        ImGui::SliderFloat("Rotation Speed", &rotSpeed, -2, 2);
    }
    ImGui::End();
}
