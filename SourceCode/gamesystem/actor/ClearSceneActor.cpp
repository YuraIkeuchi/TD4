#include "ClearSceneActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"

//������
void ClearSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);

	//�I�[�f�B�I
	Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());

	helper = make_unique< Helper>();

	//�^�C�g��
	IKESprite* ClearSprite_;
	ClearSprite_ = IKESprite::Create(ImageManager::GAMECLEAR, { 0.0f,0.0f });
	ClearSprite.reset(ClearSprite_);
}
//�X�V
void ClearSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->Button_X)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
		Audio::GetInstance()->StopWave(3);
	}
	lightgroup->Update();
	//�ۉe
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ 0.0f,0.0f,0.0f }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

}
//�`��
void ClearSceneActor::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		GameDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw();
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		GameDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//�w�i
void ClearSceneActor::ModelDraw(DirectXCommon* dxCommon) {

}
//�O�ʕ`��
void ClearSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	ClearSprite->Draw();
	IKESprite::PostDraw();
}
//��̕`��ɃX�v���C�g�Ȃǂ�������
void ClearSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//�X�v���C�g�̕`��
	ModelDraw(dxCommon);
}
//ImGui�`��
void ClearSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//���
void ClearSceneActor::Finalize() {
}
