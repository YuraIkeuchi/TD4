#include "EndRollActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"
#include "BackObj.h"
//������
void EndRollActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	//���̃V�[�������Z�s�A�J���[����
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");
	camerawork->SetEye({ 0,0,-20 });
	camerawork->SetTarget({ 0,0,0 });
	camerawork->SetCameraState(CAMERA_NORMAL);
	camerawork->DefUpda(camera);
	//�I�[�f�B�I
	//Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	//Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());
	//�V�[���`�F���W���[
	PlayPostEffect = false;
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();
	m_SceneState = SceneState::IntroState;
	
	endobj = make_unique<EndRollObj>();
	endobj->Initialize();
	BackObj::GetInstance()->Initialize();
}
//�X�V
void EndRollActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	BackObj::GetInstance()->Update();
	endobj->Update();
	sceneChanger_->Update();
}
//�`��
void EndRollActor::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		dxCommon->PreDraw();
		//postEffect->Draw(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw();
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
	else {
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//�O�ʕ`��
void EndRollActor::FrontDraw() {
	sceneChanger_->Draw();
}
void EndRollActor::IntroUpdate(DebugCamera* camera) {
	camerawork->DefUpda(camera);

}
void EndRollActor::MainUpdate(DebugCamera* camera) {

}
void EndRollActor::FinishUpdate(DebugCamera* camera) {
	//sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	//sceneChanger_->Update();
}

//�w��
void EndRollActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	//ClearSprite->Draw();
	IKESprite::PostDraw();

	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	endobj->Draw(dxCommon);
	IKEObject3d::PostDraw();
	ParticleEmitter::GetInstance()->FlontDrawAll();
	IKESprite::PreDraw();
	IKESprite::PostDraw();

}
//ImGui�`��
void EndRollActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//���
void EndRollActor::Finalize() {
}
