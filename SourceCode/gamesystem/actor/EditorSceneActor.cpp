#include "EditorSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ModelManager.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
#include "TouchableObject.h"
#include "../../../LoadBox.h"

//初期化
void EditorSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{
	OpenBrowser* openbrowser_;
	openbrowser_ = new OpenBrowser();
	openbrowser.reset(openbrowser_);
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);

	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/NewToneMapPS.hlsl");

	ParticleEmitter::GetInstance()->AllDelete();

	placeObj.reset(new PlaceBox());
	placeObj->Initialize();

	ground.reset(TouchableObject::Create(ModelManager::GetInstance()->GetModel(ModelManager::Ground)));
	ground->Initialize();
	ground->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::Ground));
	ground->SetRotation({ 0.f,0.f,0.f });
	ground->SetScale({ 1.f,1.f,1.f });
	PlayPostEffect = true;

	load = new LoadBox();
	load->Initialize();
}
//更新
void EditorSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup)
{

	lightgroup->Update();
	//camerawork->SetTarget({ camera->GetEye().x,0.0f,camera->GetEye().z });
	//camera->SetTarget({ camera->GetEye().x,camera->GetEye().y - 10.f,camera->GetEye().z + 20.f, });
	camerawork->EditorCamera();
	camerawork->Update(camera);

	ground->SetColor({ 0.5f,0.5f,0.5f,1.0f });
	ground->Update();
	//音楽の音量が変わる
	placeObj->Update();

	load->Update();
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
	IKEObject3d::PreDraw();
	///load->Draw();
	ground->Draw();
	placeObj->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//後ろの描画
void EditorSceneActor::BackDraw(DirectXCommon* dxCommon)
{
	ModelDraw(dxCommon);
}
//ポストエフェクトがかからない
void EditorSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	IKESprite::PostDraw();
#pragma endregion
}
//IMGuiの描画
void EditorSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {

	placeObj->ImGui_Draw();
	camerawork->ImGuiDraw();

	ImGui::Begin("LoadObject");

	ImGui::SetWindowPos(ImVec2(1000, 0));
	ImGui::SetWindowSize(ImVec2(300, 200));
	ImGui::Text("pos  %f", load->GetBox(1)->GetPosition().x);
	if(ImGui::Button("Load", ImVec2(100, 50)))
	{
		if(item[0])
		placeObj->FileWriting();
	}
	
	ImGui::Checkbox("WoodBox", &item[0]);
	ImGui::Checkbox("ACube", &item[1]);

	placeObj->SetLoad(item[0]);

	ImGui::End();

	ImGui::Begin("ScneChange");

	if(ImGui::Button("PlayScene",ImVec2(100,50)))
	{
		
	}

	ImGui::End();

}

