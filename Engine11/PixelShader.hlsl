struct OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 normal : NORMAL;
};

struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame: register(b0)
{
	Light light;
};

cbuffer cbPerObject: register(b1)
{
	float4x4 WVP;
	float4x4 World;

	float4 difColor;
	bool hasTexture;
};

SamplerState ObjSamplerState : register(s0);
Texture2D ObjTexture: register(t0);

float4 main(OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	float4 diffuse = difColor;
	if(hasTexture==true)
		diffuse = ObjTexture.Sample(ObjSamplerState, input.uv);

	float3 finalColor = float3(0.0f, 0.0f, 0.0f);
	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

	return float4(finalColor,diffuse.a);
}