#include "LoadSceneActor.h"
#include "SceneManager.h"
#include "ImageManager.h"
//初期化
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	BaseInitialize(dxCommon, { 0,10,200 }, {0,0,-200});
	if (!s_GameLoop) {
		SceneManager::GetInstance()->SetLoad(true);
	}
	CreateStage();

}
//更新
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();
	lightgroup->Update();
	camerawork->LoadActorUpdate(camera);
	for (std::unique_ptr<IKEObject3d>& obj : grounds) {
		obj->Update();
	}

	m_LoadTimer++;

	//一定時間でシーンが変わる
	if (m_LoadTimer >= 200 && !SceneManager::GetInstance()->GetLoad()) {
		SceneManager::GetInstance()->ChangeScene("GAMESCENE");
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
void LoadSceneActor::CreateStage() {
	m_JsonData = JsonLoader::LoadFile("Introduction");


	// モデル読み込み
	modelSkydome = make_unique<IKEModel>();
	modelSkydome->Initialize("skydome",false);
	modelGround = make_unique<IKEModel>();
	modelGround->Initialize("ground", false);
	modelFighter = make_unique<IKEModel>();
	modelFighter->Initialize("House", false);
	modelPine = make_unique<IKEModel>();
	modelPine->Initialize("Pine", false);

	models.insert(std::make_pair("skydome", std::move(modelSkydome)));
	models.insert(std::make_pair("ground", std::move(modelGround)));
	models.insert(std::make_pair("House", std::move(modelFighter)));
	models.insert(std::make_pair("Pine", std::move(modelPine)));


	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : m_JsonData->objects) {
		// ファイル名から登録済みモデルを検索
		IKEModel* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second.get();
		}

		// モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<IKEObject3d> newObj=make_unique<IKEObject3d>();
		newObj->Initialize();
		newObj->SetModel(model);

		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObj->SetPosition(pos);

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObj->SetRotation(rot);

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObj->SetScale(scale);

		// 配列に登録
		grounds.push_back(std::move(newObj));
	}

}
//背面描画
void LoadSceneActor::BackDraw(DirectXCommon* dxCommon)
{
	for (std::unique_ptr<IKEObject3d>& obj : grounds) {
		obj->Draw();
	}

}
//ImGuiの描画
void LoadSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Load");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(200, 200));
	ImGui::SliderInt("LoadTimer", &m_LoadTimer, 0, 200);
	
	ImGui::End();
	camerawork->ImGuiDraw();
}
//解放
void LoadSceneActor::Finalize() {
}