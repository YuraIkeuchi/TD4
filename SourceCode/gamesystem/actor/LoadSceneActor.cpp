#include "LoadSceneActor.h"
#include "Audio.h"
#include "input.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include <random>
//初期化
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	BaseInitialize(dxCommon);
}
//更新
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	Input* input = Input::GetInstance();
	lightgroup->Update();
}
//描画
void LoadSceneActor::Draw(DirectXCommon* dxCommon) {

	//ポストエフェクトをかけるか
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
//背景描画
void LoadSceneActor::ModelDraw(DirectXCommon* dxCommon) {
}
//前面描画
void LoadSceneActor::SpriteDraw() {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
}
//上の描画にスプライトなども混ぜた
void LoadSceneActor::GameDraw(DirectXCommon* dxCommon)
{
	//スプライトの描画
	ModelDraw(dxCommon);
}
//ImGuiの描画
void LoadSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void LoadSceneActor::Finalize() {
}