struct PixelIn
{
	float4 pos		: SV_POSITION;
	float3 tex		: TEXTURE;
	float4 color	: COLOR;
};

Texture2D tImage0;
SamplerState sampleImage0;
Texture2D tClut0;
SamplerState sampleClut0;

float4 main(PixelIn pIn) : SV_TARGET
{
	float4 color = pIn.color;
	if (pIn.tex.z < 0.50)
	{
		// Use palette W texture
		float colorIndex = tImage0.Sample(sampleImage0, pIn.tex.xy).r;
		color *= tClut0.Sample(sampleClut0, float2(colorIndex, pIn.tex.z * 2.0));
	}
	else if (pIn.tex.z < 1.0)
	{
		// Only texture
		color *= tImage0.Sample(sampleImage0, pIn.tex.xy);
	}
	else
	{
		// Do not use texture
	}
	return color;
}