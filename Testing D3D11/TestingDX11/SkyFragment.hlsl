struct PS_IN
{
	float4 Pos : SV_POSITION;
	//float3 Color : COLOR;
	float3 Tex : TEXCOORD;
};

TextureCube Tex : register(t0);
SamplerState SampSt :register(s0);


float4 PS_main(PS_IN input) : SV_Target
{
	//return float4(Tex.Sample(SampSt, input.Tex).xyz,1.0f);
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}