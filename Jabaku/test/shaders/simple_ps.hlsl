cbuffer GLOBAL_CONSTANTS : register(b0) {
	float4 g_color;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color + g_color;
}