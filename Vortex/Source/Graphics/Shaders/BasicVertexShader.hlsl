struct VSOut
{
	float4 color : COLOR;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : POSITION, float4 col : COLOR)
{
	VSOut output;
	
    output.position = pos;
	output.color = col;
	
	return output;
}