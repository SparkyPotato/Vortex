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
		
    return ret;
}
