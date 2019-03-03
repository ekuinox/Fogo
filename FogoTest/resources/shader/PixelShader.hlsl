#include "Header.hlsli"

Texture2D<float4> tex0 : register(t0);
SamplerState samp0 : register(s0);

float4 main(VS_OUT vsin) : SV_TARGET
{
	return tex0.Sample(samp0, vsin.uv);
}
