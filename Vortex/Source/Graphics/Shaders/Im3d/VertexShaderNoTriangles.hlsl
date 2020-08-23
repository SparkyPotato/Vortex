cbuffer ConstantBuffer : register(b0)
{
	float4x4 viewProjectionMatrix;
};

struct VSOut
{
	linear float4 position : SV_POSITION;
	linear float4 color : COLOR;
	linear float2 texcoord : TEXCOORD;
	noperspective float size : SIZE;
	noperspective float edgeDistance : EDGE_DISTANCE;
};

struct VSIn
{
	float4 positionSize : POSITIONSIZE;
	float4 color : COLOR;
};

VSOut main(VSIn input)
{
	VSOut output;
	
	float4 inPos = float4(input.positionSize.xyz, 1.f);
	float inSize = max(input.positionSize.w, 2.f);
	
	output.position = mul(inPos, viewProjectionMatrix);
	output.color = input.color.abgr;
	output.color.a *= smoothstep(0.f, 1.f, input.positionSize.w / 2.f);
	output.size = inSize;
	
	return output;
}
