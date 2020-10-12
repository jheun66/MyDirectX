#include "VertexHeader.hlsli"

struct VertexInput
{
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 tangent : Tangent;
    float4 indices : BlendIndices;
    float4 weights : BlendWeights;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : Position0;
    float3 camPos : Position1;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    matrix boneWorld = SkinWorld(0, input.indices, input.weights);
    boneWorld = mul(boneWorld, world);

    output.pos = mul(input.pos, boneWorld);
    
    output.worldPos = output.pos.xyz;
    output.camPos = invView._41_42_43;
        
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.uv = input.uv;

    output.normal = mul(input.normal, (float3x3) boneWorld);
    output.tangent = mul(input.tangent, (float3x3) boneWorld);
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}