// Inside

cbuffer HeightBrush : register(b10)
{
    int type;
    float3 location;
    
    float range;
    uint outputSize;
    float2 dummy;
}

struct InputDesc
{
    uint index;
    float3 pos;
};
StructuredBuffer<InputDesc> input;

struct OutputDesc
{
    uint inside;
    float distance;
};
RWStructuredBuffer<OutputDesc> output;


void Inside(uint index)
{
    if (type == 1)
    {
        float x = input[index].pos.x - location.x;
        float z = input[index].pos.z - location.z;
        
        float dist = sqrt(x * x + z * z);
        
        if (dist <= range)
        {
            output[index].inside = 1;
        }
        else
        {
            output[index].inside = 0;
        }
        output[index].distance = dist;
        return;
    }
    else if (type == 2)
    {
        if (location.x - range <= input[index].pos.x
            && location.x + range >= input[index].pos.x 
            && location.z - range <= input[index].pos.z
            && location.z + range >= input[index].pos.z)
        {
            output[index].inside = 1;
        }
        else
        {
            output[index].inside = 0;
        }
        output[index].distance = 0;
        return;
    }
}

// 개수는 임의로
[numthreads(32, 32, 1)]
void CS(uint3 groupID : SV_GroupID, uint groupIndex : SV_GroupIndex)
{
    uint index = groupID.x * 32 * 32 + groupIndex;
    
    if (outputSize > index)
        Inside(index);
}