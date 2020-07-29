struct VSOut
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float4 col : COLOR)
{
	VSOut output;
	
	output.position = float4(pos, 1.f);
	output.color = col;
	
	return output;
}