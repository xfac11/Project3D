struct GSInput
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMAL;
	float3 PositionWS : POSITIONWS;
};

struct GSOutput
{
	float4 PosCS : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 NormalWS : NORMALWS;
	float3 PositionWS : POSITIONWS;
};

cbuffer CB_PER_FRAME : register(b0)
{
	float4x4 world;//world
	float4x4 view;//view
	float4x4 proj;//proj
};
[maxvertexcount(3)]
void GSmain(triangle GSInput input[3], inout TriangleStream<GSOutput> theOutput)
{
	GSOutput output;
	float3 pos0 = mul(float4(input[0].PositionWS, 1.0f), world).xyz;
	float3 pos1 = mul(float4(input[1].PositionWS, 1.0f), world).xyz;
	float3 pos2 = mul(float4(input[2].PositionWS, 1.0f), world).xyz;
	float3 u = pos1 - pos0;
	float3 v = pos2 - pos0;; //pos of 3 == 0
	float3 normal = cross(u, v);
	normal = normalize(normal);
	//normal = mul(world,float4(normal, 1.0f)).xyz;
	//float o1 = input[2].pos;
	//float o3 = input[0].pos;

	
		for (int i = 0; i < 3; i++)
		{
			output.PosCS = input[i].PosCS;
			//output.PosCS = mul(input[i].PosCS, proj);
			//output.PositionWS = mul(float4(input[i].PositionWS.xyz, 1.0f), world).xyz;
			output.PositionWS = input[i].PositionWS;
			output.NormalWS = input[i].NormalWS;
			//output.NormalWS = normal;
			output.Tex = input[i].Tex;
			theOutput.Append(output);
		}	
}

