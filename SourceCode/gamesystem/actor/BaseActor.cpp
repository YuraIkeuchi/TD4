#include "BaseActor.h"
#include "VariableCommon.h"

//���ʏ�����
void BaseActor::BaseInitialize(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�̏�����
	//(����)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");

	camerawork = make_unique<CameraWork>();
}
