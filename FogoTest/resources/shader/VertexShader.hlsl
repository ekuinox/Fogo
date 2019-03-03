#include "Header.hlsli"

VS_OUT main(VS_IN vsin)
{
	VS_OUT output;

	output.pos = mul(vsin.pos, _WVP);
	output.normal = vsin.normal;
	output.uv = vsin.uv;

	return output;
}
