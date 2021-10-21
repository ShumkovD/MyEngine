struct OUTPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

OUTPUT main(float4 pos : POSITION, float4 color : COLOR)
{
	OUTPUT output;
	output.pos = pos;
	output.color = color;


	return output;
}