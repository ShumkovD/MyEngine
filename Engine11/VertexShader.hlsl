cbuffer cbPerObject:register(b0)
{
	float4x4 WVP;
	float4x4 World;

	float4 difColor;
	bool hasTexture;
	float3 textureScale;
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
	output.uv.x = (uv.x - 0.5) / textureScale.x + 0.5;
	output.uv.y = (uv.y - 0.5) / textureScale.y +0.5;
	return output;
}