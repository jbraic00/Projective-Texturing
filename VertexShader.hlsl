cbuffer WorldTransforms : register(b0)
{
    matrix World;
    matrix Dlrow;
};

cbuffer ViewTransforms : register(b1)
{
    matrix View;
};

cbuffer ProjectionTransforms : register(b2)
{
    matrix Projection;
};

cbuffer ViewTransforms : register(b3)
{	
	matrix VPView;
};

cbuffer ProjectionTransforms : register(b4)
{
	matrix VPProjection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
    float3 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD0;
};

// Per-pixel color data passed through the pixel shader.
struct VertexShaderOutput
{
    float4 pos : SV_POSITION;
    float3 norm : NORMAL;
    float2 tex : TEXCOORD0;
	float4 viewPos : TEXCOORD1;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    float4 pos = float4(input.pos, 1.0f);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    output.pos = pos;

	//position from the view point of where we are projection the texture from
	pos = float4(input.pos, 1.0f);
	pos = mul(pos, World);
	pos = mul(pos, VPView);
	pos = mul(pos, VPProjection);
	output.viewPos = pos;

    // Pass through the texture coordinate without modification
    output.tex = input.tex;

    // Transform the normal into world space to allow world-space lighting
    float4 norm = float4(normalize(input.norm), 0.0f);
    norm = mul(norm, Dlrow);
    output.norm = normalize(norm.xyz);

    return output;
}