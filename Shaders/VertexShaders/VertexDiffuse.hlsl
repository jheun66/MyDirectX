cbuffer W : register(b0)
{
    matrix world;
}

cbuffer V : register(b1)
{
    matrix view;
}
cbuffer P : register(b2)
{
    matrix projection;
}

cbuffer Light : register(b3)
{
    float3 lightPosition;
}


struct VertexInput
{
	//          ½Ã¸àÆ½ ³×ÀÓ
    float4 pos : Position;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float diffuse : DIFFUSE;
};


PixelInput VS(VertexInput input)
{
    PixelInput output;
    
    
    output.pos = mul(input.pos, world);
   
    float3 light = normalize(input.pos.xyz - lightPosition);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    input.normal = normalize(input.normal);
    input.normal = mul(input.normal, (float3x3) world);
    output.diffuse = saturate(dot(input.normal, -light.xyz));
    
    output.uv = input.uv;
    
    return output;
}