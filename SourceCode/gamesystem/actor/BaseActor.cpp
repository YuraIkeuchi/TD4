#include "BaseActor.h"
#include "VariableCommon.h"
bool BaseActor::s_GameLoop = false;
//���ʏ�����
void BaseActor::BaseInitialize(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�̏�����
	//(����)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	//�|�X�g�G�t�F�N�g�̃t�@�C���w��
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	camerawork = make_unique<CameraWork>();
}
