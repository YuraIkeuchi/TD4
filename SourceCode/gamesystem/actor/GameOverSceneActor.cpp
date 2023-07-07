#include "GameOverSceneActor.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "Audio.h"

//������
void GameOverSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);

	//�I�[�f�B�I
	//Audio::GetInstance()->LoadSound(3, "Resources/Sound/BGM/jto3s-8fzcz.wav");
	//Audio::GetInstance()->LoopWave(3, VolumManager::GetInstance()->GetBGMVolum());
	//�V�[���`�F���W���[
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	//�^�C�g��
	ClearSprite = IKESprite::Create(ImageManager::GAMEOVER, { 0.0f,0.0f });
	ClearSprite->SetSize({1280.0f,720.0f});
}
//�X�V
void GameOverSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	if (input->TriggerButton(input->B) && !sceneChanger_->GetEasingStart()) {
		sceneChanger_->ChangeStart();
		str = NextStageName();
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Voice_Retry.wav", VolumManager::GetInstance()->GetSEVolum());
		//Audio::GetInstance()->StopWave(AUDIO_LOAD);
	}
	if (input->TriggerButton(input->A) && !sceneChanger_->GetEasingStart()) {
		sceneChanger_->ChangeStart();
		str = "TITLE";
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Over.wav", VolumManager::GetInstance()->GetSEVolum());
		//Audio::GetInstance()->StopWave(AUDIO_LOAD);
	}

	sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	sceneChanger_->Update();

	lightgroup->Update();
	//�ۉe
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ 0.0f,0.0f,0.0f }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

}
//�`��
void GameOverSceneActor::Draw(DirectXCommon* dxCommon) {
	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
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
		BackDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//�O�ʕ`��
void GameOverSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	ClearSprite->Draw();
	IKESprite::PostDraw();
	sceneChanger_->Draw();
}
string GameOverSceneActor::NextStageName() {
	string str = "";
	for (int i = 0; i < kMaxStage; i++) {
		if (!SceneSave::GetInstance()->GetLoseFlag((SeceneCategory)i)) { continue; }
		switch (i) {
		case 1:
			str = "FIRSTSTAGE";
			break;
		case 2:
			str = "SECONDSTAGE";
			break;
		case 3:
			str = "THIRDSTAGE";
			break;
		case 4:
			str = "FOURTHSTAGE";
			break;
		case 5:
			str = "FIVESTAGE";
			break;
		case 6:
			str = "SIXSTAGE";
			break;
		case 7:
			str = "SEVENSTAGE";
			break;
		default:
			break;
		}
	}
	return str;
}
//�w��
void GameOverSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ImGui�`��
void GameOverSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//SceneSave::GetInstance()->ImGuiDraw();
}
//���
void GameOverSceneActor::Finalize() {
}
