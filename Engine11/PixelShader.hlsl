struct OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

SamplerState ObjSamplerState : register(s1);
Texture2D ObjTexture: register(t0);

float4 main(OUTPUT input) : SV_TARGET
{
	float4 color = ObjTexture.Sample(ObjSamplerState, input.uv);
	clip(color.a - .25);
	return color;
}