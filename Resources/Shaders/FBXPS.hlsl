#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
	float4 target0 : SV_TARGET0;
};
PSOutput main(VSOutput input)
{
	PSOutput output;
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);
	// Lambert����
	float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	float l_Disolve = Disolve;
	float blackarea = Disolve + 0.3f;
	float a = shadecolor.r * 0.3 + shadecolor.g * 0.3 + shadecolor.b * 0.3;

	if (ChangeColor) {
		if (a < blackarea) {
			texcolor.r = texcolor.r - Addcolor.r;
			texcolor.g = texcolor.g - Addcolor.g;
			texcolor.b = texcolor.b - Addcolor.b;
		}
	}

	//���񂾂������
	if (a > l_Disolve) {
		shadecolor.a = 1.0f;
	}
	else {
		shadecolor.a = 0.0f;
	}


	// �A�e�ƃe�N�X�`���̐F������
	if (ShadeSet) {
		output.target0 = shadecolor * texcolor * color;
	}
	else {
		output.target0 = texcolor * color;
	}
	return output;
}