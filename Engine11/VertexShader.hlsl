cbuffer cbPerObject
{
	float4x4 WVP;
};

struct OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
};

OUTPUT main(float4 pos : POSITION, float2 uv : TEXCOORD) 
{
	OUTPUT output;
	output.pos = mul(pos, WVP);
	output.uv = uv;


	return output;
}