#include "Framework.h"

// defalut disfuseMape으로 star를 사용
ParticleSystem::ParticleSystem()
	:Particle(L"Textures/Landscape/star.png", MAX_COUNT)
{
    material->SetShader(L"Particle");
    geometryShader = Shader::AddGS(L"GeometryParticle");

    //shape = new Shape();

    Create();
    CreateCompute();
}

ParticleSystem::~ParticleSystem()
{
    delete timeBuffer;
    delete structuredBuffer;
   // delete shape;
}

void ParticleSystem::Create()
{
    vertices = new VertexType[MAX_COUNT];
    vertexBuffer = new VertexBuffer(vertices, sizeof(VertexParticle), MAX_COUNT);
}

void ParticleSystem::Update()
{
    accumulatedTime += Time::Delta();
    rateTime += Time::Delta();


    if (accumulatedTime > duration)
        Stop();

    if (isPlay || looping)
    {
        EmitParticle();
    }
    ComputeParticle();
    UpdateWorld();
}

void ParticleSystem::Render()
{
	// buffer 추가해주기

	Particle::Render();
}

void ParticleSystem::PostRender()
{
    ImGui::Begin("ParticleSystem");
	{
        if (ImGui::CollapsingHeader("Particle System"))
        {
            ImGui::SliderFloat("Duration", &duration, 0.0f, 1000.0f);
            ImGui::Separator();
            ImGui::Checkbox("Looping", &looping);
            ImGui::Separator();
            ImGui::SliderFloat("Lifetime", &lifeTime, 0.0f, 15.0f);
            ImGui::Separator();
            ImGui::SliderFloat("Gravity Modifier", &timeBuffer->data.gravity, -100.0f, 100.0f);
            ImGui::Separator();
            /*ImGui::SliderFloat("Start Delay", &pb->data.startDelay, 0.0f, 1000.0f);
            ImGui::Separator();
            ImGui::SliderFloat3("Start Speed", (float*)&pb->data.startSpeed, -100.0f, 100.0f);
            ImGui::Separator();
            ImGui::SliderFloat3("Start Size", (float*)&pb->data.startSize, -100.0f, 100.0f);
            ImGui::Separator();
            ImGui::SliderFloat3("Start Rotation", (float*)&pb->data.startRotation, -XM_PI, XM_PI);
            ImGui::Separator();
            
            ImGui::SliderFloat("Flip Lotation", &pb->data.flipRotation, 0.0f, 1.0f);
            ImGui::Separator();*/
        }

        if (ImGui::CollapsingHeader("Emission"))
        {
            ImGui::SliderFloat("Rate Over Time", &rateOverTime, 0.0f, 500.0f);
            ImGui::Separator();
        }

        //if (ImGui::CollapsingHeader("Shape"))
        //{
        //    const char* items[] = { "Corn", "Sphere", "Box", "SemiSphere"};
        //    ImGui::Combo("Shape", &shape->shapeType, items, IM_ARRAYSIZE(items));

        //    // item에 따라 tree로 분리하기 
        //    ImGui::SliderFloat("Radius", &shape->radius, 0.0f, 100.0f);
        //    ImGui::Separator();
        //    ImGui::SliderFloat("Angle", &shape->angle, 0.0f, 50.0f);
        //    ImGui::Separator();
        //}

        if (ImGui::CollapsingHeader("Material"))
        {
            igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".cpp", ImVec4(0, 0, 1, 0.9));
            igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".h", ImVec4(0, 1, 0, 0.9));
            igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".jpg", ImVec4(1, 1, 0, 0.9));
            igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".png", ImVec4(1, 0, 1, 0.9));
            const char* filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
            if (ImGui::Button("ChangeMaterial"))
            {
                igfd::ImGuiFileDialog::Instance()->OpenDialog("ChangeMaterial", "Choose a File", filters, ".");
            }
            if (igfd::ImGuiFileDialog::Instance()->FileDialog("ChangeMaterial"))
            {
                if (igfd::ImGuiFileDialog::Instance()->IsOk == true)
                {
                    string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
                    string filePath = igfd::ImGuiFileDialog::Instance()->GetCurrentPath();

                    wstring path;
                    path = path.assign(filePathName.begin(), filePathName.end());
                    ChangeDiffuseMap(path);
                }
                igfd::ImGuiFileDialog::Instance()->CloseDialog("ChangeMaterial");
            }
            ImGui::Separator();
        }

        ImGui::SliderFloat3("position", (float*)&position, -100.0f, 100.0f);

        if (ImGui::Button("Play"))
        {
            Play(position);
        }

	}
	ImGui::End();

}

void ParticleSystem::Play(Vector3 position)
{
    accumulatedTime = 0.0f;
    Particle::Play(position);
}

void ParticleSystem::ChangeDiffuseMap(wstring diffuseFile)
{
	material->SetDiffuseMap(diffuseFile);
}

void ParticleSystem::CreateCompute()
{
    computeShader = Shader::AddCS(L"Particle");

    if (structuredBuffer)
    {
        delete structuredBuffer;
        structuredBuffer = nullptr;
    }

    structuredBuffer = new StructuredBuffer(vertices, sizeof(VertexType), MAX_COUNT,
        sizeof(VertexType), MAX_COUNT);

    timeBuffer = new TimeBuffer();
}

void ParticleSystem::ComputeParticle()
{
    computeShader->Set();
    timeBuffer->data.delta = Time::Delta();
    timeBuffer->SetCSBuffer(10);

    if (structuredBuffer)
    {
        delete structuredBuffer;
        structuredBuffer = nullptr;
    }

    // vertices 계속 받아줘야함
    structuredBuffer = new StructuredBuffer(vertices, sizeof(VertexType), MAX_COUNT,
        sizeof(VertexType), MAX_COUNT);


    DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
    DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

    // ceil 올림				1024 쓰레드 개수
    UINT x = ceil((float)MAX_COUNT / 1024.0f);
    DC->Dispatch(x, 1, 1);

    structuredBuffer->Copy(vertices, sizeof(VertexType) * MAX_COUNT);

    vertexBuffer->Update(vertices, MAX_COUNT);

    return;
}

void ParticleSystem::EmitParticle()
{
    if (vertices[emitIndex].isActive == false)
    {
        // 한개 방출
        if (rateTime > (1 / rateOverTime))
        {
            // 초기화
            rateTime = 0.0f;

            vertices[emitIndex].isActive = 1;
            vertices[emitIndex].position = position;    // transform의 position
            vertices[emitIndex].lifeTime = lifeTime;

            float size = GameMath::Random(0.1f, 0.5f);
            vertices[emitIndex].size = XMFLOAT2(size, size);

            float radius = GameMath::Random(5.0f, 10.0f);

            Vector3 velocity = Vector3(0, 0, radius);

            XMFLOAT3 rot;
            rot.x = GameMath::Random(0.0f, XM_2PI);
            rot.y = GameMath::Random(0.0f, XM_2PI);
            rot.z = GameMath::Random(0.0f, XM_2PI);

            XMMATRIX matRot = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

            XMStoreFloat3(&vertices[emitIndex].velocity, XMVector3TransformCoord(velocity.data, matRot));
            emitIndex = (emitIndex + 1) % MAX_COUNT;
            vertexBuffer->Update(vertices, MAX_COUNT);
        }
    }
}