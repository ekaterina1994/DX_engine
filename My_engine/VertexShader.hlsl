struct VS_INPUT
{
	//float4 pos : POSITION;
	//float4 color: COLOR;
	uint idx : SV_VertexID;
};

struct VS_OUTPUT
{
	float4 pos: SV_POSITION;
	float4 color: COLOR;
};
/*
cbuffer ConstantBuffer : register(b0)
{
	float4x4 wvpMat;
};
*/

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
//	output.pos = mul(input.pos, wvpMat);
	float2 pos = float2((float)(input.idx & 1), (float)(input.idx >> 1)) * 2.0 - 1.0;
	output.pos = float4(sign(pos), 0.0, 1.0);
	output.color = 1.0;// input.color;
	return output;
}