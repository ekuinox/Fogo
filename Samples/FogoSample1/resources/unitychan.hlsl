//--------------------------------------------------------------------------------------
// lesson06.fx
//--------------------------------------------------------------------------------------
Texture2D g_Tex:register(t0);				// テクスチャ
SamplerState g_SamplerLinear:register(s0);	// サンプラー

//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 LightDirection;
	float3 CameraPos;
}

cbuffer ConstantBufferBoneMatrix : register(b1)
{
	matrix BoneMatrix[400];
};

cbuffer ConstantBufferMaterial : register(b2)
{
	float4		diffusematerial;
	float4		specularmaterial;
	float4		ambientmaterial;
	float4		power;
};

//--------------------------------------------------------------------------------------
// 構造体定義
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 Tex   : TEXCOORD;
	float4  diffuse : TEXCOORD1;
	float4  specular : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(
	float4 Pos : POSITION, 
	float4 Color : COLOR,
	float3 Normal : NORMAL,
	float2 Tex : TEXCOORD,
	float4 BoneIndex : BONEINDEX,
	float4 BoneWeight : BONEWEIGHT
) {
	// 頂点ブレンドの処理
	float4x4 comb = (float4x4)0;
	for (int i = 0; i < 4; i++)
	{
		comb += BoneMatrix[BoneIndex[i]] * BoneWeight[i];
	}

    VS_OUTPUT output = (VS_OUTPUT)0;
	float4 wpos;

	output.Pos = mul(Pos, comb);
	
	output.Pos = mul( output.Pos, World );
	wpos = output.Pos;
	
	output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
	output.Tex = Tex;
	float3 N;
	float3 L;
	N = normalize(Normal);
	L = normalize(LightDirection);
	float d = max(0.0, dot(L, N));
	output.diffuse = diffusematerial*d;

	float3 V;
	V = wpos.xyz - CameraPos;

	float3 H;
	H = normalize(L + V);

	float s = max(0.0f, dot(H, N));

	s = pow(s, 30);

	output.specular = specularmaterial*s;

	return output;
}

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 PSMain( VS_OUTPUT input ) : SV_Target
{
	float4 texcol = g_Tex.Sample(g_SamplerLinear, input.Tex);

	float4 outcolor= texcol;
	//outcolor.rgb = outcolor.rgb*input.diffuse.rgb+input.specular.rgb;
	outcolor.rgb = texcol.rgb*input.diffuse.rgb+input.specular.rgb;
	return outcolor;
}