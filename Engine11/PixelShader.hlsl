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


SamplerState ObjSamplerState : register(s0);
Texture2D ObjTexture: register(t0);

float4 main(OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.uv);
	float3 finalColor;
	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

	clip(diffuse.a - .25);
	return float4(finalColor,diffuse.a);
}