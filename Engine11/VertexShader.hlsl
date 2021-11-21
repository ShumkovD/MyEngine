cbuffer cbPerObject:register(b0)
{
	float4x4 WVP;
	float4x4 World;
};


struct OUTPUT
{
	float4 pos : SV_POSITION;
	float2 uv :  TEXCOORD0;
	float3 normal : NORMAL;
};

OUTPUT main(float4 pos : POSITION, float2 uv : TEXCOORD, float3 normal: NORMAL) 
{
	OUTPUT output;
	output.pos = mul(pos, WVP);
	output.normal = mul(normal, World);
	output.uv = uv;


	return output;
}