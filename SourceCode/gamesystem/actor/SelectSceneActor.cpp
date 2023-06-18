#include "SelectSceneActor.h"
#include "SelectSceneActor.h"
#include "Audio.h"
#include "SceneManager.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include <HungerGauge.h>
#include "BackObj.h"
#include"SelectScene.h"

const XMVECTOR kSkyBlue{ 0.f,1.f,1.f,1.f };
const XMVECTOR kPink{ 0.9f,0.6f,0.8f,1.f };

//初期化
void SelectSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	dxCommon->SetFullScreen(true);
	//共通の初期化
	BaseInitialize(dxCommon);
	//オーディオ
	Audio::GetInstance()->LoadSound(1, "Resources/Sound/BGM/BGM_boss.wav");
	Audio::GetInstance()->LoopWave(1, VolumManager::GetInstance()->GetBGMVolum() + 1.0f);
	//ポストエフェクト
	PlayPostEffect = false;
	//パーティクル全削除
	ParticleEmitter::GetInstance()->AllDelete();

	SelectScene::GetIns()->Init();

	sceneChanger_ = make_unique<SceneChanger>();
	sceneChanger_->Initialize();

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
	camerawork->SetSceneName("FIRSTSTAGE");
	//UI
	ui = std::make_unique<UI>();
	ui->Initialize();
	ui->SetBoss(enemymanager->GetBoss());
	//背景
	
	lightgroup->SetCircleShadowActive(0, true);
	lightgroup->SetCircleShadowActive(1, true);
}
//更新
void SelectSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {


	const int IntervalTextC = 200;

	Input* input = Input::GetInstance();

	//プレイヤー
	lightgroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.0f, Player::GetInstance()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	//ボス
	lightgroup->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
	lightgroup->SetCircleShadowCasterPos(1, XMFLOAT3({ enemymanager->GetBoss()->GetPosition().x, 0.0f, 	enemymanager->GetBoss()->GetPosition().z }));
	lightgroup->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
	lightgroup->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));

	
	//音楽の音量が変わる
	Audio::GetInstance()->VolumChange(0, VolumManager::GetInstance()->GetBGMVolum());
	VolumManager::GetInstance()->Update();
	ui->Update();


	XMFLOAT3 Position = enemymanager->GetBoss()->GetPosition();
	XMVECTOR tex2DPos = { Position.x, Position.y, Position.z };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewMatrix(), false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetProjectionMatrix(), true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, camera->GetViewPort(), false);

	postEffect->SetRadCenter(XMFLOAT2(tex2DPos.m128_f32[0], tex2DPos.m128_f32[1]));
	postEffect->SetRadPower(camerawork->GetEffectPower());
	sceneChanger_->Update();

	SelectScene::GetIns()->Upda();
	camerawork->SetEye({ SelectScene::GetIns()->GetPedestalPos().x,
						SelectScene::GetIns()->GetPedestalPos().y + 20.f,
						SelectScene::GetIns()->GetPedestalPos().z - 50.f
		});
	camerawork->SetTarget(SelectScene::GetIns()->GetPedestalPos());

	camerawork->DefUpda(camera);

	lightgroup->Update();
}
//描画
void SelectSceneActor::Draw(DirectXCommon* dxCommon) {
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
void SelectSceneActor::Finalize() {
}
//後ろの描画
void SelectSceneActor::BackDraw(DirectXCommon* dxCommon) {
	
	SelectScene::GetIns()->Draw_Obj(dxCommon);
}
//ポストエフェクトがかからない
void SelectSceneActor::FrontDraw(DirectXCommon* dxCommon) {


	//ParticleEmitter::GetInstance()->DeathDrawAll();

	if (camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_BEFORE && camerawork->GetCameraState() != CameraState::CAMERA_BOSSDEAD_AFTER_FIRST)
		ui->Draw();

	sceneChanger_->Draw();	//完全に前に書くスプライト
	//if (camerawork->GetAppearType() == APPEAR_SEVEN || camerawork->GetAppearType() == APPEAR_EIGHT) {
	IKESprite::PreDraw();
	//blackwindow->Draw();
	SelectScene::GetIns()->Draw_Sprite();
	camerawork->feedDraw();
	IKESprite::PostDraw();
}
//IMGuiの描画
void SelectSceneActor::ImGuiDraw(DirectXCommon* dxCommon) {
	//loadobj->ImGuiDraw();
	//SceneSave::GetInstance()->ImGuiDraw();
}

