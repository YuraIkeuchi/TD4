#include "EditorSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
//初期化
void EditorSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	OpenBrowser* openbrowser_;
	openbrowser_ = new OpenBrowser();
	openbrowser.reset(openbrowser_);
	dxCommon->SetFullScreen(false);
	//共通の初期化
	BaseInitialize(dxCommon);

	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	ParticleEmitter::GetInstance()->AllDelete();

	PlayPostEffect = true;
}
//更新
void EditorSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
}
//描画
void EditorSceneActor::Draw(DirectXCommon* dxCommon)
{
	//描画方法
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		postEffect->ImGuiDraw();
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		ImGuiDraw(dxCommon);
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//解放
void EditorSceneActor::Finalize()
{
}
//モデルの描画
void EditorSceneActor::ModelDraw(DirectXCommon* dxCommon) {
#pragma region 3Dオブジェクト描画
	//背景は先に描画する

}
//後ろの描画
void EditorSceneActor::BackDraw(DirectXCommon* dxCommon)
{
}
//ポストエフェクトがかからない
void EditorSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
#pragma endregion
}
//IMGuiの描画
void EditorSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
}