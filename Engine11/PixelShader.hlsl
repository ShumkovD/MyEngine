struct OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

float4 main(OUTPUT output) : SV_TARGET
{
	return output.color;
}