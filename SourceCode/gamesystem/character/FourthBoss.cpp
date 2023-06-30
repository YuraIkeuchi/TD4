#include "FourthBoss.h"
#include <any>
#include <random>
#include "Collision.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include "Player.h"
#include <Easing.h>
#include "Helper.h"
void (FourthBoss::* FourthBoss::stateTable[])() = {
	&FourthBoss::WaitUpdate,//要素0
	&FourthBoss::MoveUpdate, //要素1
	&FourthBoss::ControlUpdate,
	&FourthBoss::EnemySpawnUpdate,
	&FourthBoss::SubGaugeUpdate,
	&FourthBoss::UltimateUpdate,
	&FourthBoss::ExplosionUpdate,
};


//生成
FourthBoss::FourthBoss() {
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

	for (unique_ptr<TutorialEnemy>& enemy : Fourthenemys) {
		enemy = make_unique<TutorialEnemy>();
		enemy->Initialize();
		//enemy->SetIsWeak(true);
		enemy->Update();
	}
}

bool FourthBoss::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Scale = { 2.0f,2.0f,2.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Rotation.y = -90.f;
	//m_Position.x = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Fourth/Fourthboss.csv", "pos")));
	m_Magnification = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Fourth/Fourthboss.csv", "Magnification")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Fourth/Fourthboss.csv", "hp1")));
	m_BirthTarget = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/boss/Fourth/Fourthboss.csv", "HeartTarget")));
	m_MaxHp = m_HP;

	ActionTimer = 0;

	m_Radius = 5.0f;
	return true;
}

void FourthBoss::SkipInitialize() {

}

//行動
void FourthBoss::Action() {
	if (m_HP < 0.1f) return;
	if (IsPinch()) { isStrong = true; }
	for (int i = 0; i < kPhotoSpotMax; i++) {
		photoSpot[i]->Update();
	}
	for (unique_ptr<TutorialEnemy>& enemy : Fourthenemys) {
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
void FourthBoss::AppearAction() {


	Obj_SetParam();

}
//ポーズ
void FourthBoss::Pause() {


}

void FourthBoss::ImGui_Origin() {
	ImGui::Begin("BOSS");
	ImGui::SliderInt("Action", &ActionTimer, 0, 1000);
	ImGui::SliderInt("ActionA", &stage_move, 0, 1000);
	ImGui::SliderInt("ActionB", &stage_move_count, 0, 1000);
	ImGui::SliderInt("ActionC", &stage_move_max, 0, 1000);

	ImGui::SliderFloat("m_Position.x", &m_Position.x, 0.0f, 360.0f);
	ImGui::SliderFloat("m_Position.y", &m_Position.y, 0.0f, 360.0f);
	ImGui::SliderFloat("m_Position.z", &m_Position.z, 0.0f, 360.0f);
	ImGui::SliderFloat("Limit", &m_Limit, 0.0f, 360.0f);

	switch (phase) {
	case FourthBoss::commandState::WaitCommand:
		ImGui::Text("WAIT");
		break;
	case FourthBoss::commandState::MoveCommand:
		ImGui::Text("MOVE");
		break;
	case FourthBoss::commandState::ControlCommand:
		ImGui::Text("CONTROL");
		break;
	case FourthBoss::commandState::EnemySpawn:
		ImGui::Text("ENEMYSPAWN");
		break;
	case FourthBoss::commandState::SubGauge:
		ImGui::Text("SUBGAUGE");
		break;
	case FourthBoss::commandState::Ultimate:
		ImGui::Text("ULTIMATE");
		break;
	case FourthBoss::commandState::Explosion:
		ImGui::Text("Explosion");
		break;

	default:
		assert(0);
		break;
	}



	ImGui::End();
}

void FourthBoss::EffecttexDraw(DirectXCommon* dxCommon) {
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
void FourthBoss::Draw(DirectXCommon* dxCommon) {
	//
	for (unique_ptr<TutorialEnemy>& enemy : Fourthenemys) {
		enemy->Draw(dxCommon);
	}
	Obj_Draw();
	EffecttexDraw(dxCommon);
}

void FourthBoss::SelectAction() {
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_RandAction(0, 100);
	int l_case = l_RandAction(mt);
	if (l_case < 10) {
		ChangePos2Random();
		isInstruction = FourthBossInst::None;
		phase = commandState::MoveCommand;
	} else if (l_case < 30) {
		if (!EnemysIsActiveCheck()) {
			isSearch = true;
			isInstruction = FourthBossInst::ChangeGhost;
			phase = commandState::EnemySpawn;
		} else {
			ChangePos2Random();
			isInstruction = FourthBossInst::None;
			phase = commandState::MoveCommand;
		}
	} else if (l_case < 60) {
		if (isReferCheck) {
			isSearch = true;
			isInstruction = FourthBossInst::None;
			phase = commandState::ControlCommand;
		} else {
			ChangePos2Random();
			isInstruction = FourthBossInst::None;
			phase = commandState::MoveCommand;
		}
	} else if (l_case < 90) {
		isInstruction = FourthBossInst::None;
		phase = commandState::SubGauge;
	} else if (l_case < 95) {
		isHyperSearch = true;
		isInstruction = FourthBossInst::None;
		limitHp = m_HP * 0.7f;
		phase = commandState::Ultimate;
		stage_move = ActionTimerMax[(size_t)phase] / 3;
	} else if (l_case > 100) {
		assert(0);
	}
}

void FourthBoss::WaitUpdate() {
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)phase]) {
		SelectAction();
		ActionTimer = 0;
	}
}

void FourthBoss::MoveUpdate() {
	ActionTimer++;
	photoSpot[moveSpawn]->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	if (ActionTimer >= ActionTimerMax[(size_t)phase] && !isShutter) {
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

void FourthBoss::ControlUpdate() {
	if (isMiss) {
		isInstruction = FourthBossInst::None;
		phase = commandState::SubGauge;
		isMiss = false;
		return;
	}
	if (isSearch) { return; }
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)phase] && !isShutter) {
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		isInstruction = FourthBossInst::StopGhost;
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			phase = commandState::WaitCommand;
		}
	}
}

void FourthBoss::EnemySpawnUpdate() {
	if (isMiss) {
		isInstruction = FourthBossInst::None;
		phase = commandState::SubGauge;
		isMiss = false;
		return;
	}
	if (isSearch) { return; }
	if (isInstruction != FourthBossInst::SpawnEnemy) { return; }
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)phase] && !isShutter) {
		int num = 3;
		if (isStrong) {
			num = 5;
		}
		for (int i = 0; i < num; i++) {
			Fourthenemys[i]->SetPosition(jackPos[i]);
			Fourthenemys[i]->Born();
			Fourthenemys[i]->SetIsActive(true);
		}
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			phase = commandState::WaitCommand;
		}
	}
	if (isInstruction == FourthBossInst::FinishMove) {
		phase = commandState::WaitCommand;
	}
}

void FourthBoss::SubGaugeUpdate() {
	ActionTimer++;
	if (ActionTimer >= ActionTimerMax[(size_t)phase] && !isShutter) {
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

void FourthBoss::UltimateUpdate() {
	if (isHyperSearch) { return; }
	if (limitHp >= m_HP && !isShutter && feedTimer == 0.0f) {
		m_HP = limitHp;
		phase = commandState::Explosion;
		return;
	}
	ParticleEmitter::GetInstance()->CameraEffect(80, spotPos[1], 4.0f, 0.0f, { 0.8f,0.5f,0.4f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
	ParticleEmitter::GetInstance()->CameraEffect(80, spotPos[2], 4.0f, 0.0f, { 0.8f,0.5f,0.4f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });
	ParticleEmitter::GetInstance()->CameraEffect(80, spotPos[4], 4.0f, 0.0f, { 0.8f,0.5f,0.4f,1.0f }, { 1.0f,1.0f,1.0f,1.0f });

	ActionTimer++;

	if (stage_move_count < stage_move_max) {

		if (ActionTimer >= stage_move * stage_move_count) {
			if (!isShutter) {
				ChangePos2Rand();
				isShutter = true;
				stage_move_count++;
			}
		}
		if (!isShutter) { return; }
		if (ShutterEffect()) {
			m_Position = spotPos[moveSpawn];
			if (ShutterFeed()) {
				ShutterReset();
			}
		}
		return;
	}
	if (ActionTimer >= ActionTimerMax[(size_t)phase] && !isShutter) {
		isShutter = true;
	}
	if (!isShutter) { return; }
	if (ShutterEffect()) {
		isInstruction = FourthBossInst::AllSummon;
		if (ShutterFeed()) {
			ShutterReset();
			ActionTimer = 0;
			phase = commandState::WaitCommand;
		}
	}
}

void FourthBoss::ExplosionUpdate() {
	m_Rotation.z =  30.0f;

	float l_AddSize = 2.5f;
	const float RandScale = 3.0f;
	float s_scale = 0.3f * l_AddSize;
	float e_scale = (4.0f + (float)rand() / RAND_MAX * RandScale - RandScale / 2.0f) * l_AddSize;

	//色
	const float RandRed = 0.2f;
	const float red = 0.2f + (float)rand() / RAND_MAX * RandRed;
	const XMFLOAT4 s_color = { 0.9f, red, 0.1f, 1.0f }; //濃い赤
	const XMFLOAT4 e_color = { 0, 0, 0, 1.0f }; //無色

	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Randlife(10, 40);
	int l_Life = int(l_Randlife(mt));
	ParticleEmitter::GetInstance()->Explosion(l_Life, m_Position, l_AddSize, s_scale, e_scale, s_color, e_color);

}

bool FourthBoss::ShutterEffect() {
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

bool FourthBoss::ShutterFeed() {
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

void FourthBoss::ShutterReset() {
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

bool FourthBoss::EnemysIsActiveCheck() {
	for (unique_ptr<TutorialEnemy>& enemy : Fourthenemys) {
		if (!enemy) { continue; }
		if (enemy->GetIsActive()) { return true; }
	}
	return false;
}

void FourthBoss::ChangePos2Random() {
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

void FourthBoss::ChangePos2Rand() {
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Rand(0, 2);
	int lol = l_Rand(mt);
	if (lol == 0) {
		moveSpawn = 1;
	} else if (lol == 1) {
		moveSpawn = 2;
	} else if (lol == 2) {
		moveSpawn = 4;
	}
	nowSpawn = moveSpawn;
}

bool FourthBoss::IsPinch() {
	return 	m_HP < (m_MaxHp * 0.66f);
}