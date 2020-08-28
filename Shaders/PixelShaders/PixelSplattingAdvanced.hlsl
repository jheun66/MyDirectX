#include "PixelHeader.hlsli"

Texture2D secondMap : register(t10);
Texture2D secondSpecularMap : register(t11);
Texture2D secondNormalMap : register(t12);

Texture2D thirdMap : register(t20);
Texture2D thirdSpecularMap : register(t21);
Texture2D thirdNormalMap : register(t22);

Texture2D fourthMap : register(t30);
Texture2D fourthSpecularMap : register(t31);
Texture2D fourthNormalMap : register(t32);

Texture2D fifthMap : register(t40);
Texture2D fifthSpecularMap : register(t41);
Texture2D fifthNormalMap : register(t42);

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


float4 PS(PixelInput input) : SV_Target
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    float4 second = secondMap.Sample(samp, input.uv);
    float4 third = thirdMap.Sample(samp, input.uv);
    float4 fourth = fourthMap.Sample(samp, input.uv);
    float4 fifth = fifthMap.Sample(samp, input.uv);
    
    albedo = lerp(albedo, second, input.alpha.r);
    albedo = lerp(albedo, third, input.alpha.g);
    albedo = lerp(albedo, fourth, input.alpha.b);
    albedo = lerp(albedo, fifth, input.alpha.a);
    
    float3 light = normalize(lightDir);

    float3 viewDir = normalize(input.viewDir);
    
                                                
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal); 
    float3 N = normalize(input.normal);
    
    float3x3 TBN = float3x3(T, B, N);
   
    float4 normalMapping = normalMap.Sample(samp, input.uv);
    float4 secondNormalMapping = secondNormalMap.Sample(samp, input.uv);
    float4 thirdNormalMapping = thirdNormalMap.Sample(samp, input.uv);
    float4 fourthNormalMapping = fourthNormalMap.Sample(samp, input.uv);
    float4 fifthNormalMapping = fifthNormalMap.Sample(samp, input.uv);
    
    normalMapping = lerp(normalMapping, secondNormalMapping, input.alpha.r);
    normalMapping = lerp(normalMapping, thirdNormalMapping, input.alpha.g);
    normalMapping = lerp(normalMapping, fourthNormalMapping, input.alpha.b);
    normalMapping = lerp(normalMapping, fifthNormalMapping, input.alpha.a);
    
        // color 범위 0 ~ 1 을 벡터의 -1 ~ 1 로 바꿔주기 위해
    float3 normal = normalMapping.xyz * 2.0f - 1.0f;
    normal = normalize(mul(normal, TBN));
    
    
    float diffuse = saturate(dot(normal, -light));
    
    float4 specular = 0;
    if (diffuse > 0)
    {
        float3 viewDir = normalize(input.viewDir);
        
        // Blinn Phong Shading
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(-halfWay, normal));
        
        float4 specualrIntensity = specularMap.Sample(samp, input.uv);
        specualrIntensity = lerp(specualrIntensity, secondSpecularMap.Sample(samp, input.uv), input.alpha.r);
        specualrIntensity = lerp(specualrIntensity, thirdSpecularMap.Sample(samp, input.uv), input.alpha.g);
        specualrIntensity = lerp(specualrIntensity, fourthSpecularMap.Sample(samp, input.uv), input.alpha.b);
        specualrIntensity = lerp(specualrIntensity, fifthSpecularMap.Sample(samp, input.uv), input.alpha.a);
        
        specular = pow(specular, specExp) * specualrIntensity;
    }
   
    
    return albedo * (diffuse + ambient) + specular + float4(input.brushColor, 1);
}