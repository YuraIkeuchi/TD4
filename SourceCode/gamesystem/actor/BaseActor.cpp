#include "BaseActor.h"
#include "VariableCommon.h"
#include "Player.h"
bool BaseActor::s_GameLoop = false;

void (BaseActor::* BaseActor::stateTable[])() = {
	&BaseActor::IntroUpdate,//�v�f0
	&BaseActor::MainUpdate, //�v�f1
	&BaseActor::FinishUpdate,
};
//���ʏ�����
void BaseActor::BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye, XMFLOAT3 target) {
	//�|�X�g�G�t�F�N�g�̏�����
	//(����)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	//�|�X�g�G�t�F�N�g�̃t�@�C���w��
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	//�J����
	camerawork = make_unique<CameraWork>(eye, target);

	//�v���C���[�̒e��S�폜����(�V�[���ׂ��ŕ`�悳���Ȃ�����)
	Player::GetInstance()->DeleteBullet();
}

//�v���C���[�����񂾂�
bool BaseActor::PlayerDestroy() {
	if (Player::GetInstance()->GetHP() <= 0.0f) {
		return true;
	}
	else {
		return false;
	}

	return false;
}