// Particle

cbuffer Time : register(b10)
{
    float delta;
    float gravity;
    uint outputSize;    // 1000개
}

struct InputDesc
{
    float3 position;
    float2 size;
    float3 velocity;
    uint isActive;
    float lifeTime;
};
StructuredBuffer<InputDesc> input;

struct OutputDesc
{
    float3 position;
    float2 size;
    float3 velocity;
    uint isActive;
    float lifeTime;
};
RWStructuredBuffer<OutputDesc> output;


void AddTime(uint index)
{
    output[index] = input[index];

    if (input[index].isActive == 1)
    {
        output[index].lifeTime = input[index].lifeTime - delta;
        if (output[index].lifeTime < 0)
        {
            output[index].isActive = 0;
        }
        else
        {   
            output[index].velocity.y -= gravity * delta;
            output[index].position = input[index].position + output[index].velocity * delta;
        }
    }
    return;
}

// 개수는 임의로
[numthreads(32, 32, 1)]
void CS(uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = groupID.x * 32 * 32 + groupIndex;
    
    if (outputSize > index)
        AddTime(index);
}