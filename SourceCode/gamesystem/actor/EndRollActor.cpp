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
	camerawork->SetEye({ 0,10,0 });
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
	BackObj::GetInstance()->Initialize();
	endobj = make_unique<EndRollObj>();
	endobj->Initialize();
}
//�X�V
void EndRollActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	//�e�N���X�X�V
	BackObj::GetInstance()->Update();
	endobj->Update();
	camerawork->DefUpda(camera);
	sceneChanger_->Update();

	lightgroup->Update();

	if (Input::GetInstance()->TriggerButton(Input::A)) {

		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);
	}
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

}
void EndRollActor::MainUpdate(DebugCamera* camera) {
	
}
void EndRollActor::FinishUpdate(DebugCamera* camera) {

}

//�w��
void EndRollActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	endobj->Draw(dxCommon);
	BackObj::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ImGui�`��
void EndRollActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//SceneSave::GetInstance()->ImGuiDraw();
	ImGui::Begin("End");
	ImGui::Text("a");
	ImGui::End();
}
//���
void EndRollActor::Finalize() {
}
