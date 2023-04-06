#include "LoadSceneActor.h"
#include "Audio.h"
#include "input.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include <random>
//������
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	BaseInitialize(dxCommon);
}
//�X�V
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	Input* input = Input::GetInstance();
	lightgroup->Update();
}
//�`��
void LoadSceneActor::Draw(DirectXCommon* dxCommon) {

	//�|�X�g�G�t�F�N�g�������邩
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		GameDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		SpriteDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		GameDraw(dxCommon);
		SpriteDraw();
		dxCommon->PostDraw();
	}
}
//�w�i�`��
void LoadSceneActor::ModelDraw(DirectXCommon* dxCommon) {
}
//�O�ʕ`��
void LoadSceneActor::SpriteDraw() {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
}
//��̕`��ɃX�v���C�g�Ȃǂ�������
void LoadSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//�X�v���C�g�̕`��
	ModelDraw(dxCommon);
}
//ImGui�̕`��
void LoadSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//���
void LoadSceneActor::Finalize() {
}