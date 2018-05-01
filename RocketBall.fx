//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Texturing Diffuse / Sampler State registers
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
Texture2D txSpecular : register(t1);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 DiffuseMaterial; // Diffuse material
	float4 DiffuseLight; // Diffuse light
	float4 AmbientMaterial; // Ambient material
	float4 AmbientLight; // Ambient light
	float4 SpecularMaterial; // Specular material
	float4 SpecularLight; // Specular light
	float SpecularPower; // Specular power
	float3 EyePosW; // Camera position in world space
	float3 LightVecW; // Light vector in world space
}

//------------------------------------------------------------------------------------
// Structs
//------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 NormalW : NORMAL;
	float3 PosW : POSITION;
	float2 Tex : TEXCOORD0;
};

//------------------------------------------------------------------------------------
// Vertex Shader
//------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, World);
	output.PosW = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = Tex;

	// Convert from local space to world space 
	// W component of vector is 0 as vectors cannot be translated
	float3 normalW = mul(float4(NormalL, 0.0f), World).xyz;
	output.NormalW = normalize(normalW);

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input) : SV_Target
{
	float3 toEye = normalize(EyePosW - input.PosW.xyz);
	float3 normW = normalize(input.NormalW);

	float3 lightVec = normalize(LightVecW);

	float3 r = reflect(-lightVec, normW);
	float4 textureColour = txDiffuse.Sample(samLinear, input.Tex);
	float4 textureSpecular = txSpecular.Sample(samLinear, input.Tex);

	// Compute amounts
	float diffuseAmount = max(dot(lightVec, normW), 0.0f);
	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);

	if (diffuseAmount <= 0.0f)
		specularAmount = 0.0f;

	// Compute diffuse, ambient & specular
	float3 diffuse = diffuseAmount * (DiffuseMaterial * DiffuseLight).rgb;
	float3 ambient = (AmbientMaterial * AmbientLight).rgb;
	float3 specular = specularAmount * (SpecularMaterial * SpecularLight).rgb;

	// Return color & alpha values
	float3 color = (textureColour * (diffuse + ambient)) + (textureSpecular * specular);
	float alpha = textureColour.a;
	clip(alpha - 0.25f);
	
	return float4(color, alpha);
}