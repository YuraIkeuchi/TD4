#include "ThirdBoss.h"
#include <any>
#include <random>

#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Shake.h"
#include "Player.h"
#include <Easing.h>
#include <TutorialEnemy.h>

void (ThirdBoss::* ThirdBoss::stateTable[])() = {
	&ThirdBoss::WaitUpdate,//要素0
	&ThirdBoss::MoveUpdate, //要素1
	&ThirdBoss::ControlUpdate,
	&ThirdBoss::EnemySpawnUpdate,
	&ThirdBoss::SubGaugeUpdate,
	&ThirdBoss::UltimateUpdate,

};


//生成
ThirdBoss::ThirdBoss() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::MobUsa);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	for (int i = 0; i < kPhotoSpotMax; i++) {
		IKETexture* photoSpot_ = IKETexture::Create(ImageManager::PHOTOSPOT, spotPos[i], { 1.5f,1.5f,1.5f }, { 1,1,1,1 });
		photoSpot_->SetRotation({ 90.0f,0.0f,0.0f });
		photoSpot_->TextureCreate();
		photoSpot[i].reset(photoSpot_);
	}

	photo[Photo_In] = IKESprite::Create(ImageManager::PHOTO_IN, { 0,0 });
	photo[Photo_Out_Top] = IKESprite::Create(ImageManager::PHOTO_OUT, { 0,-360 });
	photo[Photo_Out_Under] = IKESprite::Create(ImageManager::PHOTO_OUT, { 0,1080 });
	for (int i = Photo_Out_Top; i <= Photo_Out_Under; i++) {
		photo[i]->SetSize({ 1280,360 });
	}

	for (unique_ptr<TutorialEnemy>& enemy : Thirdenemys) {
		enemy = make_unique<TutorialEnemy>();
		enemy->Initialize();
		//enemy->SetIsWeak(true);
		enemy->Update();
	}
}

bool ThirdBoss::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 2.0f,2.0f,2.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = -90.f;
	//m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "pos")));
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/first/firstboss.csv", "HeartTarget")));
	m_MaxHp = m_HP;

	ActionTimer = 0;

	m_Radius = 5.0f;
	return true;
}

void ThirdBoss::SkipInitialize() {

}

//行動
void ThirdBoss::Action() {
	if (m_HP < 0.1f) return;
	if (IsPinch()) { isStrong = true; }
	for (int i = 0; i < kPhotoSpotMax; i++) {
		photoSpot[i]->Update();
	}
	for (unique_ptr<TutorialEnemy>& enemy : Thirdenemys) {
		enemy->Update();
	}
	(this->*stateTable[(size_t)phase])();
	/*^^^^当たり判定^^^^*/
	//弾とボスの当たり判定
	vector<InterBullet*> _playerBulA = Player::GetInstance()->GetBulllet_attack();
	CollideBul(_playerBulA, Type::CIRCLE);
	ColPlayer();
	//OBJのステータスのセット
	Obj_SetParam();
	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -55.0f, 65.0f);
	Helper::GetInstance()->Clamp(m_Position.z, -60.0f, 60.0f);
}
//ポーズ
void ThirdBoss::Pause() {


}

void ThirdBoss::ImGui_Origin() {
	ImGui::Begin("BOSS");
	ImGui::SliderInt("Action", &ActionTimer, 0, 100);
	ImGui::SliderFloat("m_Position.x", &m_Position.x, 0.0f, 360.0f);
	ImGui::SliderFloat("m_Position.y", &m_Position.y, 0.0f, 360.0f);
	ImGui::SliderFloat("m_Position.z", &m_Position.z, 0.0f, 360.0f);
	ImGui::End();
}

void ThirdBoss::EffecttexDraw(DirectXCommon* dxCommon) {
	//if (m_HP < 0.1f)return;
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (int i = 0; i < kPhotoSpotMax; i++) {
		photoSpot[i]->Draw();
	}
	IKETexture::PostDraw();
	IKESprite::PreDraw();
	if (phase == commandState::MoveCommand && ActionTimer > 60 && shutterTime < 1.0f) {
		photo[Photo_In]->Draw();
	}
	for (int i = Photo_Out_Top; i <= Photo_Out_Under; i++) {
		photo[i]->Draw();
	}
	IKESprite::PostDraw();
}
//描画
void ThirdBoss::Draw(DirectXCommon* dxCommon) {
	//
	for (unique_ptr<TutorialEnemy>& enemy : Thirdenemys) {
		enemy->Draw(dxCommon);
	}
	Obj_Draw();
	EffecttexDraw(dxCommon);
}

void ThirdBoss::SelectAction() {
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandAction(0, 100);
	int l_case = l_RandAction(mt);
	if (l_case < 10) {
		ChangePos2Random();
		isInstruction = ThirdBossInst::None;
		phase = commandState::MoveCommand;
	} else if (l_case < 30) {
		isSearch = true;
		isInstruction = ThirdBossInst::ChangeGhost;
		phase = commandState::EnemySpawn;
	} else if (l_case < 60) {
		if (isReferCheck) {
			isSearch = true;
			isInstruction = ThirdBossInst::None;
			phase = commandState::ControlCommand;
		} else {
			ChangePos2Random();
			isInstruction = ThirdBossInst::None;
			phase = commandState::MoveCommand;
		}
	} else if (l_case < 90) {
		isInstruction = ThirdBossInst::None;
		phase = commandState::SubGauge;
	} else if (l_case < 95) {
		isHyperSearch = true;
		isInstruction = ThirdBossInst::None;
		phase = commandState::Ultimate;
	} else if (l_case > 100) {
		assert(0);
	}
}

void ThirdBoss::WaitUpdate() {
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)commandState::WaitCommand]) {
		SelectAction();

		ActionTimer = 0;
	}
}

void ThirdBoss::MoveUpdate() {
	ActionTimer++;
	photoSpot[moveSpawn]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	if (ActionTimer >= ActionTimerMax[(size_t)commandState::MoveCommand]) {
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		m_Position = spotPos[moveSpawn];
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			phase = commandState::WaitCommand;
		}
	}
}

void ThirdBoss::ControlUpdate() {
	if (isSearch) { return; }
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)commandState::ControlCommand]) {
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		isInstruction = ThirdBossInst::StopGhost;
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			m_Limit = 20.0f;
			phase = commandState::WaitCommand;
		}
	}
}

void ThirdBoss::EnemySpawnUpdate() {
	if (isInstruction != ThirdBossInst::SpawnEnemy) { return; }
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)commandState::ControlCommand] && !isShutter) {
		int num = 3;
		if (isStrong) {
			num = 5;
		}
		for (int i = 0; i < num; i++) {
			Thirdenemys[i]->SetPosition(jackPos[i]);
			Thirdenemys[i]->SetIsActive(true);
		}
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			m_Limit = 20.0f;
			phase = commandState::WaitCommand;
		}
	}
	if (isInstruction == ThirdBossInst::FinishMove) {
		phase = commandState::WaitCommand;
	}
}

void ThirdBoss::SubGaugeUpdate() {
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)commandState::ControlCommand] && !isShutter) {
		isShutter = true;
		m_Check = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			phase = commandState::WaitCommand;
		}
	}
}

void ThirdBoss::UltimateUpdate() {
	if (isHyperSearch) { return; }
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)commandState::ControlCommand]) {
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		isInstruction = ThirdBossInst::AllSummon;
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			m_Limit = 20.0f;
			phase = commandState::WaitCommand;
		}
	}




}

bool ThirdBoss::ShutterEffect() {
	shutterTime += 1.0f / shutterTimeMax;
	shutterTime = clamp(shutterTime, 0.0f, 1.0f);

	shutterHight[0] = Ease(Out, Quad, shutterTime, -360, 0);
	shutterHight[1] = Ease(Out, Quad, shutterTime, 1080, 360);

	photo[Photo_Out_Top]->SetPosition({ 0,shutterHight[0] });
	photo[Photo_Out_Under]->SetPosition({ 0,shutterHight[1] });
	if (shutterTime == 1.0f) {
		return true;
	} else {
		return false;
	}
}

bool ThirdBoss::ShutterFeed() {
	feedTimer += 1.0f / feedTimeMax;
	float color = Ease(Out, Linear, feedTimer, 1.0f, 0.0f);
	photoSpot[moveSpawn]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	photo[Photo_Out_Top]->SetColor({ 1,1,1, color });
	photo[Photo_Out_Under]->SetColor({ 1,1,1,color });
	feedTimer = clamp(feedTimer, 0.0f, 1.0f);

	if (feedTimer == 1.0f) {
		return true;
	} else {
		return false;
	}
}

void ThirdBoss::ShutterReset() {
	isShutter = false;
	shutterHight[0] = -360.0f;
	shutterHight[1] = 1080.0f;
	photo[Photo_Out_Top]->SetPosition({ 0,shutterHight[0] });
	photo[Photo_Out_Under]->SetPosition({ 0,shutterHight[1] });
	photo[Photo_Out_Top]->SetColor({ 1,1,1,1 });
	photo[Photo_Out_Under]->SetColor({ 1,1,1,1 });
	shutterTime = 0.0f;
	feedTimer = 0.0f;
}

void ThirdBoss::ChangePos2Random() {
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Rand(0, 4);
	moveSpawn = l_Rand(mt);
	if (moveSpawn == nowSpawn) {
		moveSpawn++;
		if (moveSpawn > 4) {
			moveSpawn = 0;
		}
	}
	nowSpawn = moveSpawn;
}

bool ThirdBoss::IsPinch() {
	return 	m_HP < (m_MaxHp * 0.66f);
}