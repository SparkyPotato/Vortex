struct VSOut
{
	float4 color : COLOR;
    float4 normal : NORMAL;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : POSITION, float4 col : COLOR, float4 norm : NORMAL)
{
	VSOut output;
	
    output.position = pos;
	output.color = col;
    output.normal = norm;
	
	return output;
}