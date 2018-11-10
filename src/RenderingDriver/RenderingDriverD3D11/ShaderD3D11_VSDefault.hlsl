cbuffer MatrixBuffer : register(b0)
{
	matrix Matrix;
	matrix dummy;
};

struct VertexIn
{
	float4 pos		: POSITION;
	float3 tex		: TEXTURE;
	float4 color	: COLOR;
};
struct VertexOut
{
	float4 pos		: SV_POSITION;
	float3 tex		: TEXTURE;
	float4 color	: COLOR;
};

VertexOut main(const VertexIn vIn)
{
	VertexOut vOut;
	//vOut.pos = mul(Matrix, vIn.pos);
	vOut.pos = vIn.pos;
	vOut.tex = vIn.tex;
	vOut.color = vIn.color;
	return vOut;
}