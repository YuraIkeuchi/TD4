#include "LoadSceneActor.h"
#include "SceneManager.h"
#include "ImageManager.h"
//初期化
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	BaseInitialize(dxCommon);
	if (!s_GameLoop) {
		SceneManager::GetInstance()->SetLoad(true);
	}
}
//更新
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	lightgroup->Update();

	m_LoadTimer++;

	//一定時間でシーンが変わる
	if (m_LoadTimer >= 200 && !SceneManager::GetInstance()->GetLoad()) {
		SceneManager::GetInstance()->ChangeScene("EDITORSCENE");
	}
}
//描画
void LoadSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		SpriteDraw();
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
		SpriteDraw();
		dxCommon->PostDraw();
	}
}
//前面描画
void LoadSceneActor::SpriteDraw() {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
}
//背面描画
void LoadSceneActor::BackDraw(DirectXCommon* dxCommon)
{

}
//ImGuiの描画
void LoadSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Load");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(1280, 720));
	ImGui::SliderInt("LoadTimer", &m_LoadTimer, 0, 200);
	ImGui::End();
}
//解放
void LoadSceneActor::Finalize() {
}