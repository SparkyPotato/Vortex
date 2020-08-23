struct PSIn
{
    linear float4 position : SV_POSITION;
    linear float4 color : COLOR;
    linear float2 texcoord : TEXCOORD;
    noperspective float size : SIZE;
    noperspective float edgeDistance : EDGE_DISTANCE;
};

float4 main(PSIn input) : SV_TARGET
{
    float4 ret = input.color;
			
    float d = abs(input.edgeDistance) / input.size;
    d = smoothstep(1.0, 1.0 - (2.f / input.size), d);
    ret.a *= d;
		
    return ret;
}
