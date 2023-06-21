#include "FourthStageActor.h"
#include "Audio.h"
#include"Easing.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <algorithm>
#include "BackObj.h"
#include "Menu.h"
//初期化
void FourthStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);

	//ポストエフェクト
	PlayPostEffect = true;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,5.0f,-5.0f });

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });
	//シーンチェンジャー
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	enemymanager = std::make_unique<EnemyManager>("FOURTHSTAGE");
	//enemymanager->Initialize(dxCommon);
	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::ANGER_TALK);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("FOURTHSTAGE");
	camerawork->SplineSet();
	camerawork->Update(camera);
	ui = std::make_unique<UI>();
	ui->Initialize();

	Menu::GetIns()->Init();
	ui->SetBoss(enemymanager->GetBoss());
	BackObj::GetInstance()->Initialize();

	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	m_SceneState = SceneState::IntroState;

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	//丸影のためのやつ
	lightgroup->SetDirLightActive(0, false);
	lightgroup->SetDirLightActive(1, false);
	lightgroup->SetDirLightActive(2, false);
	for (int i = 0; i < SPOT_NUM; i++) {
		lightgroup->SetSpotLightActive(i, true);
	}

	spotLightPos[0] = { 20,  10, 60 };
	spotLightPos[1] = { 20,  10,  0 };
	spotLightPos[2] = { -20, 10, 60 };
	spotLightPos[3] = { -20, 10,  0 };

	spotLightDir[0] = { -2, -1, -2 };
	spotLightDir[1] = { -2, -1, 2 };
	spotLightDir[2] = { 2, -1, -2 };
	spotLightDir[3] = { 2, -1, 2 };
}
//更新
void FourthStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	const float l_AddAngle = 5.0f;
	//関数ポインタで状態管理
	(this->*stateTable[static_cast<size_t>(m_SceneState)])(camera);
	sceneChanger_->Update();

	//プレイヤー
	if (enemymanager->BossDestroy() && camerawork->GetFeedEnd()) {
		//SceneSave::GetInstance()->SetClearFlag(kFourthStage, true);
		lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z }));
		lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	}
	else {//ボス撃破ムービーの後は丸影消す
		lightgroup->SetCircleShadowActive(0, false);
	}

	//ボス
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 	0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));
	
	//sin波によって上下に動く
	m_Angle += l_AddAngle;
	m_Angle2 = m_Angle * (3.14f / 180.0f);
	m_AddPos = (sin(m_Angle2) * 10.0f);
	//for (int i = 0; i < 4; i++) {
	//	spotLightPos[i].x += m_AddPos;
	//	spotLightPos[i].z += m_AddPos;
	//}
	///スポットライト
	for (int i = 0; i < SPOT_NUM; i++) {
		lightgroup->SetSpotLightDir(i, XMVECTOR({ spotLightDir[i].x,spotLightDir[i].y,spotLightDir[i].z,0}));
		lightgroup->SetSpotLightPos(i, spotLightPos[i]);
		lightgroup->SetSpotLightColor(i, XMFLOAT3(spotLightColor));
		lightgroup->SetSpotLightAtten(i, XMFLOAT3(spotLightAtten));
		lightgroup->SetSpotLightFactorAngle(i, XMFLOAT2(spotLightFactorAngle));
	}
	lightgroup->Update();
	Menu::GetIns()->Upda();
	postEffect->SetCloseRad(Menu::GetIns()->GetCloseIconRad());
}
//描画
void FourthStageActor::Draw(DirectXCommon* dxCommon) {
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
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//解放
void FourthStageActor::Finalize() {
}
//後ろの描画
void FourthStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);
	//パーティクル描画
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		if (!enemymanager->BossDestroy()) {
			ParticleEmitter::GetInstance()->BackDrawAll();
		}
	}
	////各クラスの描画
	if (!camerawork->GetFeedEnd()) {
		Player::GetInstance()->Draw(dxCommon);
		loadobj->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void FourthStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	if (!camerawork->GetFeedEnd() && m_SceneState == SceneState::MainState) {
		ParticleEmitter::GetInstance()->FlontDrawAll();
	}

	ParticleEmitter::GetInstance()->DeathDrawAll();
	//完全に前に書くスプライト
	IKESprite::PreDraw();
	if (m_SceneState == SceneState::MainState && !camerawork->GetFeedEnd()) {
		ui->Draw();
	}
	if (m_SceneState == SceneState::IntroState) {
		if ((camerawork->GetAppearType() == APPEAR_SEVEN) || (camerawork->GetAppearType() == APPEAR_EIGHT)) {
			text_->SpriteDraw(dxCommon);
		}
	}
	IKESprite::PostDraw();
	sceneChanger_->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
}
//IMGuiの描画
void FourthStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Light");
	ImGui::SliderFloat("LightX0", &spotLightDir[0].x, -3, 3);
	ImGui::SliderFloat("LightZ0", &spotLightDir[0].z, -3, 3);
	ImGui::SliderFloat("LightX1", &spotLightDir[1].x, -3, 3);
	ImGui::SliderFloat("LightZ1", &spotLightDir[1].z, -3, 3);
	ImGui::SliderFloat("LightX2", &spotLightDir[2].x, -3, 3);
	ImGui::SliderFloat("LightZ2", &spotLightDir[2].z, -3, 3);
	ImGui::SliderFloat("LightX3", &spotLightDir[3].x, -3, 3);
	ImGui::SliderFloat("LightZ3", &spotLightDir[3].z, -3, 3);
	ImGui::End();
	//Player::GetInstance()->ImGuiDraw();
	//loadobj->ImGuiDraw();
	camerawork->ImGuiDraw();
	//enemymanager->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}
//登場シーン
void FourthStageActor::IntroUpdate(DebugCamera* camera) {

	//最後までテキストを見た
	if (enemymanager->GetEnemyFinishAppear()) {
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
	}

	//演出スキップ
	if (Input::GetInstance()->TriggerButton(Input::A)) {
		camerawork->SetCameraSkip(true);
	}

	if (camerawork->GetAppearEndF()) {
		m_SceneState = SceneState::MainState;
		camerawork->SetCameraState(CAMERA_NORMAL);
		enemymanager->SkipInitialize();
	}

	//各クラス更新
	BackObj::GetInstance()->Update();
	ParticleEmitter::GetInstance()->Update();
	Player::GetInstance()->AppearUpdate();
	enemymanager->AppearUpdate();

	camerawork->Update(camera);
}
//バトルシーン
void FourthStageActor::MainUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
	ui->Update();
	//カメラワークのセット
	if (enemymanager->BossDestroy())
	{
		Audio::GetInstance()->StopWave(1);
		//フェード前
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//フェード後
		else
		{
			PlayPostEffect = false;
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });
			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_FOURTH);
		}

		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);

		}

		Player::GetInstance()->DeathUpdate();
	}
	else
	{
		Player::GetInstance()->Update();
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(1);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::Reverse);
	}

	//音楽の音量が変わる
	VolumManager::GetInstance()->Update();

	//各クラス更新
	BackObj::GetInstance()->Update();


	enemymanager->BattleUpdate();
	loadobj->FourthUpdate();
	ParticleEmitter::GetInstance()->Update();

	camerawork->Update(camera);

	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
}
//撃破シーン
void FourthStageActor::FinishUpdate(DebugCamera* camera) {
	Input* input = Input::GetInstance();
}