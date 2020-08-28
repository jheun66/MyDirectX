#include "VertexHeader.hlsli"

cbuffer Brush : register(b3)
{
    int type;
    float3 location;
    
    float range;
    float3 color;
}

struct VertexInput
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
    float3 brushColor : COLOR;
    float4 alpha : ALPHA;
};


float3 BrushColor(float3 pos)
{
    if (type == 1)
    {
        float x = pos.x - location.x;
        float z = pos.z - location.z;
        
        float dist = sqrt(x * x + z * z);
        
        if (dist <= range)
            return color;
    }
    else if (type == 2)
    {
        if (location.x - range <= pos.x
            && location.x + range >= pos.x 
            && location.z - range <= pos.z
            && location.z + range >= pos.z)
            return color;
    }
    return float3(0, 0, 0);
}


PixelInput VS(VertexInput input)
{
    PixelInput output;
	
    output.pos = mul(input.pos, world);
    
    float3 camPos = invView._41_42_43;
    output.viewDir = normalize(output.pos.xyz - camPos);
    
    output.brushColor = BrushColor(output.pos.xyz);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    
    output.alpha = input.alpha;
    
    return output;
}