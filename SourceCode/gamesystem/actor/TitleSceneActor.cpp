#include "TitleSceneActor.h"
#include "input.h"
#include "ImageManager.h"
#include "ParticleEmitter.h"
#include "Menu.h"
#include "SelectScene.h"
#include "TItleObj.h"
//初期化
void TitleSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_TITLE, VolumManager::GetInstance()->GetBGMVolum()+2.0f);

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();
	sceneChanger_->ChangeFeedStart();
	//タイトル
	TitleSprite = IKESprite::Create(ImageManager::TITLE, { 0.0f,0.0f });
	TitleWordSprite= IKESprite::Create(ImageManager::TITLEWORD, pos);
	TitleWordSprite->SetSize(size);
	TitleWordSprite->SetScale(0.3f);

	for (auto i = 0; i < TitlePartsSprite.size(); i++) {
		TitlePartsSprite[i] = IKESprite::Create(ImageManager::TITLEWORD, {});
		m_PartsSize[i] = { 100.0f,100.0f };
		m_PartsPos[i] = { 1000.0f,(100.0f * i) + 350.0f };
		TitlePartsSprite[i]->SetScale(0.5f);
	}
	PlayPostEffect = true;
	menu = make_unique<Menu>();
	menu->Initialize();

	camerawork->SetCameraState(CAMERA_TITLE);
	camerawork->Update(camera);

	//各クラス
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ 0.0f,-2.0f,-30.0f });
	TitleObj::GetInstance()->Initialize();
}
//更新
void TitleSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	
	frame += 0.01f;
	TitleWordSprite->SetPosition({pos.x+(sinf(frame*5.0f) * 25.0f), pos.y + (sinf(frame) *30.0f)});

	if (sceneChanger_->GetEasingStart()) {
		string str = "LOAD";
		sceneChanger_->ChangeSceneExtra(str, SceneChanger::NonReverse);
	}
	sceneChanger_->Update();
	lightgroup->Update();
	ParticleEmitter::GetInstance()->FireEffect(100, { 0.0f,23.0f,0.0f }, 5.0f, 0.0f, { 1.0f,0.5f,0.0f,0.5f }, { 1.0f,0.5f,0.0f,0.5f });
	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	postEffect->SetCloseRad(SelectScene::GetIns()->GetCloseIconRad());
	if (SelectScene::GetIns()->GetPedestal() != nullptr) {
		if (SelectScene::GetIns()->GetCloseScl() < 10000.f)
			SelectScene::GetIns()->Upda();
		else
			SelectScene::GetIns()->ResetParama();
	}

	Player::GetInstance()->TitleUpdate();
	TitleObj::GetInstance()->Update();
	menu->Update();
	camerawork->Update(camera);

	//セレクト
	SceneSelect();
}
//描画
void TitleSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw();
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw();
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}

//前面描画
void TitleSceneActor::FrontDraw() {
	IKESprite::PreDraw();
	TitleWordSprite->Draw();
	//TitlePartsSprite[2]->Draw();
	for (int i = 0; i < 2; i++) {
		TitlePartsSprite[i]->Draw();
	}
	SelectScene::GetIns()->Draw_Sprite();
	IKESprite::PostDraw();
	menu->Draw();
	sceneChanger_->Draw();
}
//背面描画
void TitleSceneActor::BackDraw(DirectXCommon* dxCommon)
{
	IKESprite::PreDraw();
	TitleSprite->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	Player::GetInstance()->Draw(dxCommon);
	TitleObj::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ImGui描画
void TitleSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	ImGui::Begin("Title");
	ImGui::Text("%d",(int)_SelectType);
	ImGui::End();
}
//解放
void TitleSceneActor::Finalize() {

}

void TitleSceneActor::SceneSelect() {
	Input* input = Input::GetInstance();

	if (!m_Change) {
		if (_SelectType == NORMAL_SCENE && (input->TiltPushStick(Input::L_DOWN)) || (input->TriggerButton(Input::DOWN))) {
			_SelectType = SELECT_SCENE;
		}
		else if (_SelectType == SELECT_SCENE && (input->TiltPushStick(Input::L_UP)) || (input->TriggerButton(Input::UP))) {
			_SelectType = NORMAL_SCENE;
		}
	}

	if ((input->TriggerButton(input->B) &&
		!sceneChanger_->GetEasingStart())) {
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button_Decide.wav", VolumManager::GetInstance()->GetSEVolum());
		sceneChanger_->ChangeStart();
		Audio::GetInstance()->StopWave(AUDIO_TITLE);

		if (_SelectType == NORMAL_SCENE) {
			s_Skip = false;
		}
		else {
			s_Skip = true;
		}

		m_Change = true;
	}

	TitleMove();
}
//パーツの動き(タイトル)
void TitleSceneActor::TitleMove() {
	if (_SelectType == NORMAL_SCENE) {
		m_Angle[NORMAL_SCENE] += 2.0f;
		m_Angle2[NORMAL_SCENE] = m_Angle[NORMAL_SCENE] * (3.14f / 180.0f);
		//選択時座標が上下に動く
		m_PartsPos[SELECT_AREA] = m_PartsPos[NORMAL_SCENE];
		m_Angle[SELECT_SCENE] = 0.0f;
		//選択時サイズも少し変わる
		m_PartsSize[NORMAL_SCENE] = { (sin(m_Angle2[NORMAL_SCENE]) * 16.0f) + (100.0f),
			(sin(m_Angle2[NORMAL_SCENE]) * 16.0f) + (100.0f) };
		m_PartsSize[SELECT_AREA] = m_PartsSize[NORMAL_SCENE];
		m_PartsSize[SELECT_SCENE] = { 100.0f,100.0f };
	}
	else {
		m_Angle[SELECT_SCENE] += 2.0f;
		m_Angle2[SELECT_SCENE] = m_Angle[SELECT_SCENE] * (3.14f / 180.0f);
		//選択時座標が上下に動く
		m_PartsPos[SELECT_AREA] = m_PartsPos[SELECT_SCENE];
		m_Angle[NORMAL_SCENE] = 0.0f;
		//選択時サイズも少し変わる
		m_PartsSize[SELECT_SCENE] = { (sin(m_Angle2[SELECT_SCENE]) * 16.0f) + (100.0f),
			(sin(m_Angle2[SELECT_SCENE]) * 16.0f - 16.0f) + (100.0f) };
		m_PartsSize[SELECT_AREA] = m_PartsSize[SELECT_SCENE];
		m_PartsSize[NORMAL_SCENE] = { 100.0f,100.0f };
	}

	for (auto i = 0; i < TitlePartsSprite.size(); i++) {
		TitlePartsSprite[i]->SetPosition(m_PartsPos[i]);
		TitlePartsSprite[i]->SetSize(m_PartsSize[i]);
	}
}