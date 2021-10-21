struct OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

SamplerState ObjSamplerState : register(s0);
Texture2D ObjTexture: register(t0);

float4 main(OUTPUT input) : SV_TARGET
{
	return ObjTexture.Sample(ObjSamplerState, input.uv);
}