struct PS_IN
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS;
	float3 PositionWS : POSITIONWS;
};
struct PS_OUT
{
	//float3 Color : COLOR;
	//float4 DiffuseAlbedo : DIFFUSE;
	//float4 SpecularAlbedo : SPECULAR;
	float4 Normal : SV_Target0;
	float4 TexColor : SV_Target1;
	float4 Pos : SV_Target2;
};
cbuffer CB_PER_FRAME : register(b0)
{
	float3 SpecularAlbedo;
	float SpecularPower;
};
cbuffer CB_PER_FRAME : register(b1)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
	float3 camPos;
	float padding;
};

Texture2D Tex : register(t0);
SamplerState SampSt :register(s0);
PS_OUT PS_main(PS_IN input)
{
	//Fix properties in the shaders. Entrypoint and shader type
	PS_OUT output;
	//float3 diffuseAlbedo = Tex.Sample(SampSt, input.Tex).rgb;
	//float3 normalWS = normalize(input.NormalWS);

	//output.Normal = float4(input.NormalWS, 1.0f);//Normal in worldspace
	//output.Pos = float4(input.PositionWS, 1.0f);
	////output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	////output.SpecularAlbedo = float4(SpecularAlbedo, SpecularPower);
	//output.TexColor = Tex.Sample(SampSt, input.Tex);//float4(input.ThePoint, 1.0f);//World space position

	output.TexColor = Tex.Sample(SampSt, input.Tex);
	//output.TexColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

	//  [-1...1] to [0...1]
	float x = (0.5f*input.NormalWS.x) + 0.5f;
	float y = (0.5f*input.NormalWS.y) + 0.5f;
	float z = (0.5f*input.NormalWS.z) + 0.5f;
	//float xP = (0.5f*input.PositionWS.x) + 0.5f;
	//float yP = (0.5f*input.PositionWS.y) + 0.5f;
	//float zP = (0.5f*input.PositionWS.z) + 0.5f;
	float xP = input.PositionWS.x;
	float yP = input.PositionWS.y;
	float zP = input.PositionWS.z;
	float3 norScale = float3(x, y, z);
	output.Normal = float4(norScale, 1.0f);
	output.Pos = float4(xP, yP, zP, 1.0f);
	//output.Pos = mul(world,float4(input.PositionWS, 1.0f));

	return output;
}