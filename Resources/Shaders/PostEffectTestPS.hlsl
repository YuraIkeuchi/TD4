#include"PostEffectTest.hlsli"
Texture2D<float4>tex0:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4>tex1:register(t1);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);
float4 main(VSOutput input) : SV_TARGET
{
    // float4 texcolor = tex0.Sample(smp,input.uv);
    ////// //default
    //////// return float4(texcolor.rgb, 1);
    // //�ڂ���
    // // 2.5�e�N�Z�������炷���߂�UV�l�����߂�
    // float offsetU = 3.5f / 1280.0f;
    // float offsetV = 3.5f / 720.0f;

    // // ��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(offsetU, 0.0f));

    // // ��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, 0.0f));

    // // ��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(0.0f, offsetV));

    // // ��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(0.0f, -offsetV));

    // // ��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(offsetU, offsetV));

    // // ��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(offsetU, -offsetV));

    // // ��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, offsetV));

    // // ��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O����
    // texcolor += tex0.Sample(smp, input.uv + float2(-offsetU, -offsetV));

    // // ��e�N�Z���ƋߖT8�e�N�Z���̕��ςȂ̂�9�ŏ��Z����
    // texcolor /= 9.0f;

    // return float4(texcolor.rgb,1);

    // //�����܂ł��ڂ���
    // //�{�[�_�[��
     float4 colortex0 = tex0.Sample(smp,input.uv);
     float4 colortex1 = tex0.Sample(smp, input.uv);

     //�F���]
     float4 color = float4(1.0f - colortex1.rgb, 1);
     if (fmod(input.uv.y, 0.1f) < 0.05f) {
         // 2.5�e�N�Z�������炷���߂�UV�l�����߂�
    float offsetU = 3.5f / 1280.0f;
    float offsetV = 3.5f / 720.0f;

    // ��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(offsetU, 0.0f));

    // ��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(-offsetU, 0.0f));

    // ��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(0.0f, offsetV));

    // ��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(0.0f, -offsetV));

    // ��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(offsetU, offsetV));

    // ��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(offsetU, -offsetV));

    // ��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(-offsetU, offsetV));

    // ��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O����
    color += tex0.Sample(smp, input.uv + float2(-offsetU, -offsetV));

    // ��e�N�Z���ƋߖT8�e�N�Z���̕��ςȂ̂�9�ŏ��Z����
    color /= 9.0f;
    }

    return float4(color.rgb, 1);
    //return float4(1.0f - texcolor.rgb, 1);
    //�F�Â��Ȃ���
    //return float4(1-texcolor.rgb, 1);
    //�F���邭�Ȃ��� �ڂ��I���
     //return float4(texcolor.rgb*2, 1);
    //return tex.Sample(smp,input.uv) * color;
//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}