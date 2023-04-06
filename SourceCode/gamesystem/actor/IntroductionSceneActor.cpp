#include "IntroductionSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include <Easing.h>
#include "ImageManager.h"
#include "VariableCommon.h"
#include "imgui.h"
//������
void IntroductionSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//���ʂ̏�����
	BaseInitialize(dxCommon);

	//���C�g
	lightgroup->SetDirLightActive(0, true);
	lightgroup->SetDirLightActive(1, true);
	lightgroup->SetDirLightActive(2, true);
	lightgroup->SetPointLightActive(0, false);
	lightgroup->SetPointLightActive(1, false);

	//�|�X�g�G�t�F�N�g�̃t�@�C���w��
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/SepiaPS.hlsl");

	//�I�[�f�B�I
	Audio::GetInstance()->LoadSound(2, "Resources/Sound/BGM/8bo8k-1eq6w.wav");
	Audio::GetInstance()->LoopWave(2, VolumManager::GetInstance()->GetBGMVolum());

	helper = make_unique< Helper>();
}
//�X�V
void IntroductionSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	lightgroup->Update();
	//�ۉe
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ 0.0f,0.0f,0.0f }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
}
//�`��
void IntroductionSceneActor::Draw(DirectXCommon* dxCommon) {

	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		GameDraw(dxCommon);
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
		ImGuiDraw(dxCommon);
		GameDraw(dxCommon);
		FrontDraw();
		dxCommon->PostDraw();
	}
}
//�w�i
void IntroductionSceneActor::ModelDraw(DirectXCommon* dxCommon) {
}
//�O�ʕ`��
void IntroductionSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
}
//��̕`��ɃX�v���C�g�Ȃǂ�������
void IntroductionSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//�X�v���C�g�̕`��
	ModelDraw(dxCommon);
}
//ImGui�`��
void IntroductionSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//���
void IntroductionSceneActor::Finalize() {
}
