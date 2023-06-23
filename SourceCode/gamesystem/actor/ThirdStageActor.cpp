#include "ThirdStageActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include <HungerGauge.h>
#include "BackObj.h"
#include "Menu.h"
const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMVECTOR kPink{ 0.9f,0.6f,0.8f,1.f };

//初期化
void ThirdStageActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoopWave(AUDIO_BATTLE, VolumManager::GetInstance()->GetBGMVolum()+1.0f);
	//ポストエフェクト
	PlayPostEffect = true;

	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();
	
	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

	backScreen_ = IKESprite::Create(ImageManager::PLAY, { 0,0 });
	backScreen_->SetSize({ 1280.0f,720.0f });

	//各クラス
	//プレイヤー
	Player::GetInstance()->InitState({ 0.0f,5.0f,-70.0f });

	Player::GetInstance()->SetCanShot(false);
	Player::GetInstance()->MoveStop(true);
	//敵
	enemymanager = std::make_unique<EnemyManager>("THIRDSTAGE");
	//カメラ
	camerawork->Update(camera);
	camerawork->SetBoss(enemymanager->GetBoss());
	camerawork->SetCameraState(CAMERA_BOSSAPPEAR);
	camerawork->SetSceneName("THIRDSTAGE");
	camerawork->SplineSet();
	//UI
	ui = std::make_unique<UI>();
	ui->Initialize();
	ui->SetBoss(enemymanager->GetBoss());
	//背景
	BackObj::GetInstance()->Initialize();
	//ステージOBJ
	loadobj = std::make_unique<LoadStageObj>();
	loadobj->AllLoad("FIRSTSTAGE");
	loadobj->LightSet(lightgroup);
	LoadStageObj::SetEnemyManager(enemymanager.get());

	text_ = make_unique<BossText>();
	text_->Initialize(dxCommon);
	text_->SelectText(TextManager::Name_First::VIEWBOSS);

	messagewindow_ = make_unique<MessageWindow>();
	messagewindow_->Initialize();
	messagewindow_->Display();

	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);

	Menu::GetIns()->Init();
}
//更新
void ThirdStageActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	Input* input = Input::GetInstance();

	//プレイヤー
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z}));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	//ボス
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));


	if (enemymanager->BossDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		SceneSave::GetInstance()->SetClearFlag(kFirstStage, true);
		if(camerawork->GetCameraState()==CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		{
			PlayPostEffect = false;
		}
	}
	
	if (camerawork->GetAppearEndF()) {
		Player::GetInstance()->MoveStop(false);
		Player::GetInstance()->SetCanShot(true);
		camerawork->SetCameraState(CAMERA_NORMAL);
		//enemymanager->SkipInitialize();
		//各クラス更新
		BackObj::GetInstance()->Update();
		enemymanager->BattleUpdate();
	}

	if (PlayerDestroy()) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene("GAMEOVER", SceneChanger::NonReverse);
	}
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();


	if (enemymanager->BossDestroy())
	{
		Player::GetInstance()->DeathUpdate();
	}
	else
	{
		Player::GetInstance()->Update();
	}
	//enemymanager->BattleUpdate();
	ColEnemy(enemymanager->GetBulEnemy());
	loadobj->ThirdUpdate();
	ParticleEmitter::GetInstance()->Update();
	if (input->TriggerKey(DIK_X)) {
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		SceneManager::GetInstance()->ChangeScene("SECONDSTAGE");

	}
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
			Player::GetInstance()->InitState({ 0.0f,0.0f,-5.0f });

			enemymanager->SetDeadThrow(false);
			enemymanager->DeadUpdate();
			camerawork->SetCameraState(CAMERA_BOSSDEAD_AFTER_FIRST);
		}


		if (camerawork->GetEndDeath()) {
			sceneChanger_->ChangeStart();
			sceneChanger_->ChangeScene("GAMECLEAR", SceneChanger::ReverseType::NonReverse);
		}
	}
	else
	{
		//if (camerawork->FinishAppear()) {
			//m_SceneState = SceneState::MainState;
		//	camerawork->SetCameraState(CAMERA_NORMAL);
		//}
	}
	if (PlayerDestroy()) {
		std::string str = "GAMEOVER";
		Audio::GetInstance()->StopWave(AUDIO_BATTLE);
		sceneChanger_->ChangeStart();
		sceneChanger_->ChangeScene(str, SceneChanger::Reverse);
	}

	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos,camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);
	
	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
	sceneChanger_->Update();

	//if(_Tscne!=TextScene::ENDTEXT)
	//text_->Display();

	messagewindow_->Update(girl_color_, sutopon_color_);

	camerawork->Update(camera);
	lightgroup->Update();

	Menu::GetIns()->Upda();
	postEffect->SetCloseRad(Menu::GetIns()->GetCloseIconRad());
}
//描画
void ThirdStageActor::Draw(DirectXCommon* dxCommon) {
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
void ThirdStageActor::Finalize() {
}
//後ろの描画
void ThirdStageActor::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	backScreen_->Draw();
	IKESprite::PostDraw();

	IKEObject3d::PreDraw();
	BackObj::GetInstance()->Draw(dxCommon);

	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
		if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
			ParticleEmitter::GetInstance()->BackDrawAll();
		}
	}
	loadobj->Draw(dxCommon);

	ParticleEmitter::GetInstance()->DeathDrawAll();
	////各クラスの描画
	if (camerawork->GetAppearEndF()) {
		Player::GetInstance()->Draw(dxCommon);
	}
	enemymanager->Draw(dxCommon);
	
	IKEObject3d::PostDraw();
}
//ポストエフェクトがかからない
void ThirdStageActor::FrontDraw(DirectXCommon* dxCommon) {
	//パーティクル描画
	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSAPPEAR &&
		camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST) {
		ParticleEmitter::GetInstance()->FlontDrawAll();
	}
	//ParticleEmitter::GetInstance()->DeathDrawAll();

	if(camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE &&camerawork->GetCameraState()!=CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
	ui->Draw();

	//完全に前に書くスプライト
	if (_Tscne != TextScene::ENDTEXT){
		IKESprite::PreDraw();
		messagewindow_->Draw();
		IKESprite::PostDraw();

		if (messagewindow_->DisplayCheck()) {
				text_->SpriteDraw(dxCommon);
			}
	}


	sceneChanger_->Draw();	//完全に前に書くスプライト
	//if (camerawork->GetAppearType() == APPEAR_SEVEN || camerawork->GetAppearType() == APPEAR_EIGHT) {
	if (_Tscne != TextScene::ENDTEXT)
	
	//}
	IKESprite::PreDraw();
	//blackwindow->Draw();
	Menu::GetIns()->Draw();
	camerawork->feedDraw();
	IKESprite::PostDraw();
}
//IMGuiの描画
void ThirdStageActor::ImGuiDraw(DirectXCommon* dxCommon) {
	Player::GetInstance()->ImGuiDraw();
	enemymanager->ImGuiDraw();
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}


void ThirdStageActor::ColEnemy(std::vector<InterEnemy*> enelist)
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