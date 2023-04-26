#include "LoadSceneActor.h"
#include "SceneManager.h"
#include "ImageManager.h"
#include"Easing.h"
#include "Helper.h"

//初期化
void LoadSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	BaseInitialize(dxCommon, { 0,10,200 }, { 0,0,-200 });
	if (!s_GameLoop) {
		SceneManager::GetInstance()->SetLoad(true);
	}
	CreateStage();




	for (int i = 0; i < SpriteMax; i++) {
		m_SpritesPos[i].x = (i + 1) * 100.0f;
		m_SpritesPos[i].y = 630.0f;
		m_Sprites[i] = IKESprite::Create(ImageManager::LOADING, m_SpritesPos[i]);
		m_Sprites[i]->SetAnchorPoint({ 0.5f,0.5f });
		m_Sprites[i]->SetSize({ 96.0f, 96.0f });
	}
}
//更新
void LoadSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	lightgroup->Update();
	camerawork->LoadActorUpdate(camera);

	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])();

	for (std::unique_ptr<IKEObject3d>& obj : grounds) {
		obj->Update();
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
	} else {
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
	for (std::unique_ptr<IKESprite>& sprite : m_Sprites) {
		sprite->Draw();
	}

	IKESprite::PostDraw();
}
void LoadSceneActor::CreateStage() {
	m_JsonData = JsonLoader::LoadFile("Introduction");


	// モデル読み込み
	modelSkydome = make_unique<IKEModel>();
	modelSkydome->Initialize("skydome", false);
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
		std::unique_ptr<IKEObject3d> newObj = make_unique<IKEObject3d>();
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

void LoadSceneActor::IntroUpdate() {

	m_SceneState = SceneState::MainState;
}
void LoadSceneActor::MainUpdate() {

	//上下運動の中心
	constexpr XMFLOAT2 CenterPos = { 100.f,630.0f };
	//加速度
	constexpr float AddMovingVal = 4.0f;
	//最初の文字だけずっと動かす
	m_SpritesAngle[0] += AddMovingVal;

	//文字の間隔
	constexpr float WordsInter = 100.f;
	//縦軸の間隔
	const float space = 30.0f;
	for (int i = 0; i < SpriteMax; i++) {
		if (i != 0 && m_SpritesAngle[i - 1] > AddMovingVal * 5.0f) {
			m_SpritesAngle[i] += AddMovingVal;
		}
		m_SpritesPos[i].x = CenterPos.x + static_cast<float>(i) * WordsInter;
		m_SpritesPos[i].y = CenterPos.y + sinf(m_SpritesAngle[i] * PI / PI_180) * space;
		m_Sprites[i]->SetPosition(m_SpritesPos[i]);
	}


	if (!SceneManager::GetInstance()->GetLoad()) {
		m_SceneState = SceneState::FinishState;
	}
}
void LoadSceneActor::FinishUpdate() {
	m_LoadTimer++;

	float frame = (float)m_LoadTimer / (float)LoadTimerMax;
	for (int i = 0; i < SpriteMax; i++) {
		m_SpritesPos[i].x = (i + 1) * 100.0f;
		m_SpritesPos[i].y = 630.0f;
		m_Sprites[i]->SetPosition(m_SpritesPos[i]);

		float rot = Ease(In, Quad, frame, 0, PI_360);
		m_Sprites[i]->SetRotation(rot);
	}

	//一定時間でシーンが変わる
	if (m_LoadTimer >= LoadTimerMax) {
		SceneManager::GetInstance()->ChangeScene("GAMESCENE");
	}
}
//背面描画
void LoadSceneActor::BackDraw(DirectXCommon* dxCommon) {
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