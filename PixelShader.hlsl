#include "C:\Users\Josipa\Desktop\faks\8. semestar\Trodimenzionalne simulacije\labovi\Vjezba04\Engine\Lighting\HLSL\Directional Light.hlsli"

Texture2D Texture : register(t0);
Texture2D ProjectionTexture : register(t1);

SamplerState Sampler : register(s0);

//cbuffer MaterialBuffer : register(b0)
//{
//    Material material;
//};

//cbuffer CameraPosition : register(b1)
//{
//    float3 Eye;
//};

//cbuffer DirectionalLightBuffer : register(b2)
//{
//    DirectionalLight directional;
//};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD0;
	float4 viewPos : TEXCOORD1;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	// Interpolating normal can unnormalize it, so normalize it
	//input.norm = normalize(input.norm);

	//float3 tocamera = normalize(Eye - input.pos.xyz);

	// Start with a sum of zero. 
	/*float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 a, d, s;

	ComputeDirectionalLight(material, directional, input.norm, tocamera, a, d, s);
	ambient += a;
	diffuse += d;
	specular += s;

	float4 color = ambient + diffuse + specular;*/

	// Common to take alpha from diffuse material.
	/*color.a = material.Diffuse.a;

	float magnitude = 0.8f * saturate(dot(input.norm, -directional.Direction)) + 0.5f;
	color = color * magnitude;*/
	//color = saturate(color);

	//Tutorial
	/*float4 color = directional.Ambient;
	float3 lightDir = -directional.Direction;
	float lightIntensity = saturate(dot(input.norm, lightDir));
	if(lightIntensity > 0.0f)
	{
		color += (directional.Diffuse * lightIntensity);
	}
	color = saturate(color);*/

	//float4 textureColor = Texture.Sample(Sampler, input.tex);
	/*color = color * textureColor;

	return color;*/

	//Basic Texturing
	float3 direction = normalize(float3(-10, -10, 1));
    float4 textureColor = Texture.Sample(Sampler, input.tex);
	float magnitude = 0.8f * saturate(dot(input.norm, -direction)) + 0.5f;

	float4 color = textureColor * magnitude;

	float4 projectionColor;
	float2 projectTexCoord;
	projectTexCoord.x = input.viewPos.x / input.viewPos.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.viewPos.y / input.viewPos.w / 2.0f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y) && input.viewPos.w >= 0.0f)
	{
		projectionColor = ProjectionTexture.Sample(Sampler, projectTexCoord);

		color = projectionColor;
	}

	return color;
}