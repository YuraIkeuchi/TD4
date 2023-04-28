#include "TitleSceneActor.h"
#include "Audio.h"
#include "input.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
//������
void TitleSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	//�I�[�f�B�I
	Audio::GetInstance()->LoadSound(0, "Resources/Sound/BGM/ruinsBGM.wav");
	Audio::GetInstance()->LoopWave(0, VolumManager::GetInstance()->GetBGMVolum());

	helper = make_unique< Helper>();

	//�^�C�g��
	TitleSprite = IKESprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
}
//�X�V
void TitleSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->B)||input->Pushkey(DIK_SPACE)) {
		if (!s_GameLoop) {
			SceneManager::GetInstance()->ChangeScene("LOAD");
		}
		else {
			SceneManager::GetInstance()->ChangeScene("EDITORSCENE");
		}
		Audio::GetInstance()->StopWave(0);
	}


	lightgroup->Update();
	ParticleEmitter::GetInstance()->FireEffect(100, { 0.0f,23.0f,0.0f }, 5.0f, 0.0f, { 1.0f,0.5f,0.0f,0.5f }, { 1.0f,0.5f,0.0f,0.5f });
	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();
}
//�`��
void TitleSceneActor::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}

//�O�ʕ`��
void TitleSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	TitleSprite->Draw();
	IKESprite::PostDraw();
}
//�w�ʕ`��
void TitleSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ImGui�`��
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//���
void TitleSceneActor::Finalize() {
}
