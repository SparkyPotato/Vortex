Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

float4 main(float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	float4 texColor = tex.Sample(samplerState, texcoord);
	return color * texColor;
}