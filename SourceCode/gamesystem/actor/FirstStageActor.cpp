#include "FirstStageActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"

#include <HungerGauge.h>
#include "BackObj.h"
//初期化

void FirstStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum());
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();
	
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	//各クラス
	Player::GetInstance()->InitState({ 0.0f,0.0f,-70.0f });
	camerawork->Update(camera);
	
	enemymanager = std::make_unique<EnemyManager>("FIRSTSTAGE");
	
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("FIRSTSTAGE");
	ui = std::make_unique<UI>();
	ui->Initialize();
	ui->SetBoss(enemymanager->GetBoss());

	BackObj::GetInstance()->Initialize();
	
	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	LoadStageObj::SetEnemyManager(enemymanager.get());

	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::Name_First::VIEWBOSS);

	lightgroup->SetCircleShadowActive(0, true);
}
//更新
void FirstStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {

	
	Input* input = Input::GetInstance();

	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, Player::GetInstance()->GetPosition().y, Player::GetInstance()->GetPosition().z}));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	if (input->TriggerKey(DIK_X)) {
		Audio::GetInstance()->StopWave(1);
		SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");

	}

	if (enemymanager->BossDestroy()) {
		Audio::GetInstance()->StopWave(1);
	//	sceneChanger_->ChangeStart();
	//	sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::NonReverse);

	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(1);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::NonReverse);
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();
	//各クラス更新
	BackObj::GetInstance()->Update();
	Player::GetInstance()->Update();
	enemymanager->BattleUpdate();
	ColEnemy(enemymanager->GetBulEnemy());
	loadobj->FirstUpdate();
	ParticleEmitter::GetInstance()->Update();
	//カメラワークのセット
	if (enemymanager->BossDestroy())
	{
		//フェード前
		if (!camerawork->GetFeedEnd()) {
			enemymanager->SetDeadThrow(true);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_BEFORE);
		}
		//フェード後
		else
		{
			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_FIRST);
		}
	}
	else
	{
		if (camerawork->FinishAppear()) {
			//m_SceneState = SceneState::MainState;
		//	camerawork->SetCameraState(CAMERA_NORMAL);
		}
	}
	
	camerawork->Update(camera);
	lightgroup->Update();
}
//描画
void FirstStageActor::Draw(DirectXCommon* dxCommon) {
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
	} else {
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
void FirstStageActor::Finalize() {
}
//後ろの描画
void FirstStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	////各クラスの描画
	Player::GetInstance()->Draw(dxCommon);
	loadobj->Draw(dxCommon);
	BackObj::GetInstance()->Draw(dxCommon);
	
	enemymanager->Draw(dxCommon);
	
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void FirstStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	ParticleEmitter::GetInstance()->FlontDrawAll();

	ui->Draw();;
	sceneChanger_->Draw();	//完全に前に書くスプライト
	//if (camerawork->GetAppearType() == APPEAR_SEVEN || camerawork->GetAppearType() == APPEAR_EIGHT) {
		text_->SpriteDraw(dxCommon);
	//}
	//IKESprite::PreDraw();
	//blackwindow->Draw();
	camerawork->feedDraw();
}
//IMGuiの描画
void FirstStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	//loadobj->ImGuiDraw();
}


void FirstStageActor::ColEnemy(std::vector<InterEnemy*> enelist)
{
	for (auto i = 0; i < enelist.size(); ++i) {
		for (auto j = 0; j < enelist.size(); ++j) {
			XMFLOAT3 ghostpos = enelist[i]->GetPosition();
			XMFLOAT3 ghostpos2 = enelist[j]->GetPosition();
			if ((i == j)) { continue; }
			if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
				enelist[i]->EnemyColNormal(ghostpos2);
				enelist[j]->EnemyColNormal(ghostpos);
			}
		}
	}
}