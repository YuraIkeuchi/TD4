#include "LoadStageObj.h"
#include "CsvLoader.h"
#include "HungerGauge.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"
EnemyManager* LoadStageObj::m_EnemyManager = nullptr;
//ゴーストのロード
void LoadStageObj::GhostLoad() {
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/ghost/ghost.csv", "Quantity")));

	ghosts.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);

}
//食べ物ロード
void LoadStageObj::FoodLoad(const std::string& sceneName) {
	size_t Size;
	if (sceneName == "FIRSTSTAGE") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage1")));
	} else if (sceneName == "SECONDSTAGE") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage2")));
	} else if (sceneName == "TUTORIAL") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage1")));
	} else {
		assert(0);
	}

	foods.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
}
//すべてロード
void LoadStageObj::AllLoad(const std::string& sceneName) {
	m_SceneName = sceneName;
	//ゴースト関係
	GhostLoad();
	//初期化
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i] = new Ghost();
		ghosts[i]->Initialize();
	}
	//食べ物関係
	FoodLoad(sceneName);

	//初期化
	for (auto i = 0; i < foods.size(); i++) {
		foods[i] = new Food();
		foods[i]->Initialize();
	}
}
//ライトセット(食べ物に使う)
void LoadStageObj::LightSet(LightGroup* light) {
	lightgroup = light;
	for (auto i = 0; i < foods.size(); i++) {
		lightgroup->SetCircleShadowActive(i + 2, true);
	}
}
//初期化
void LoadStageObj::Initialize() {
	//Load();
}
void LoadStageObj::TutorialUpdate() {

	CommonUpdate();
}
//更新(ステージ1)
void LoadStageObj::FirstUpdate() {
	//更新
	CommonUpdate();
}

//更新
void LoadStageObj::SecondUpdate() {
	//更新
	CommonUpdate();
	//こっから特有の処理
	//食料の削除(このステージのみ)
	for (int i = 0; i < foods.size(); i++) {
		if (foods[i] == nullptr) {
			continue;
		}

		if (!foods[i]->GetAlive()) {
			foods.erase(cbegin(foods) + i);
		}
	}

	//食料生成
	if (m_EnemyManager->GetEnemyCheck()) {
		Food* newFood;
		newFood = new Food();
		newFood->Initialize();
		newFood->SetPosition({ m_EnemyManager->GetEnemyPosition().x,0.0f,m_EnemyManager->GetEnemyPosition().z });
		newFood->SetLimit(true);
		foods.push_back(newFood);
		m_EnemyManager->FinishCheck();

		for (int i = 0; i < foods.size(); i++) {
			foods[i]->SetLightSet(true);
		}
	}
}
void LoadStageObj::ThirdUpdate() {
	//更新
	CommonUpdate();
	ThirdBossAction();
}
//描画
void LoadStageObj::Draw(DirectXCommon* dxCommon) {
	//ゴースト
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->Draw(dxCommon);
	}
	//
	//食べ物
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->Draw(dxCommon);
	}

	//ハート
	for (auto i = 0; i < hearts.size(); i++) {
		hearts[i]->Draw(dxCommon);
	}
	//
}
//ImGui
void LoadStageObj::ImGuiDraw() {
	//ゴースト
	//ghosts[0]->ImGuiDraw();
	/*ImGui::Begin("Heart");
	ImGui::Text("m_Division:%f", m_Division);
	ImGui::End();*/
	//m_EnemyManager->ImGuiDraw();
}
//当たり判定(ゴースト)
void LoadStageObj::Collide() {
	for (auto i = 0; i < ghosts.size(); ++i) {
		for (auto j = 0; j < ghosts.size(); ++j) {
			XMFLOAT3 ghostpos = ghosts[i]->GetPosition();
			XMFLOAT3 ghostpos2 = ghosts[j]->GetPosition();
			if ((i == j)) { continue; }
			if ((!ghosts[i]->GetAlive()) || (!ghosts[j]->GetAlive())) { continue; }
			if ((!ghosts[i]->GetFollow()) || (!ghosts[j]->GetFollow())) { continue; }
			if (Collision::SphereCollision(ghostpos, 1.5f, ghostpos2, 1.5f)) {
				ghosts[i]->GhostCollision(ghostpos2);
				ghosts[j]->GhostCollision(ghostpos);
			}
		}
	}
}
//食料の検索
void LoadStageObj::SearchFood() {
	for (auto i = 0; i < ghosts.size(); i++) {
		XMFLOAT3 l_ghostpos = ghosts[i]->GetPosition();
		for (auto j = 0; j < foods.size(); j++) {
			if (!ghosts[i]->GetAlive()) { continue; }
			if (!ghosts[i]->GetCatch()) { continue; }
			if (ghosts[i]->GetFollow()) { continue; }
			XMFLOAT3 l_foodpos = foods[j]->GetPosition();
			float l_dir = Helper::GetInstance()->ChechLength(l_ghostpos, l_foodpos);
			if ((!ghosts[i]->GetSearch()) && (foods[j]->GetAlive()) && (!foods[j]->GetLockOn()) && (!foods[j]->GetIsCarried())) {
				if (l_dir < ghosts[i]->GetLimit()) {
					ghosts[i]->StartSearch(l_foodpos);
					foods[j]->SetLockOn(true);
				} else {
					ghosts[i]->SetLimit(ghosts[i]->GetLimit() + 3.0f);
				}
			}
		}
	}
}
//食料とゴーストの当たり判定
void LoadStageObj::CollideFood() {
	float l_Radius = 1.5f;
	for (auto i = 0; i < ghosts.size(); i++) {
		XMFLOAT3 l_ghostpos = ghosts[i]->GetPosition();
		for (auto j = 0; j < foods.size(); j++) {
			XMFLOAT3 l_foodpos = foods[j]->GetPosition();
			if (ghosts[i]->GetFollow()) { continue; }
			float l_dir = Helper::GetInstance()->ChechLength(l_ghostpos, l_foodpos);
			if ((ghosts[i]->GetSearch()) && (l_dir < l_Radius)) {
				ghosts[i]->EndSearch();
				foods[j]->CarryStart(ghosts[i]);
			}
		}
	}
}
//ゴーストが消える
void LoadStageObj::VanishGhost() {
	int l_TargetCatchCount = HungerGauge::GetInstance()->GetCatchCount() - 1;
	float l_Value = HungerGauge::m_Hungervalue;
	//除算をする
	m_Division = HungerGauge::GetInstance()->GetNowHunger() / 5.0f;
	for (auto i = 0; i < ghosts.size(); ++i) {
		if (!ghosts[i]->GetAlive()) { continue; }
		if (!ghosts[i]->GetCatch()) { continue; }
		if (!ghosts[i]->GetFollow()) { continue; }
		//特定の値を下回ったら
		if (m_Division <= l_TargetCatchCount) {
			m_Vanish = true;
		}

		//for分抜ける
		if (m_Vanish) {
			ghosts[i]->SetAlive(false);
			HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() - 1);
			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() - l_Value);
			m_Vanish = false;
			break;
		}
	}

	//ゴースト薄くする版
	//int l_TargetCatchCount = HungerGauge::GetInstance()->GetCatchCount() - 1;
	//float l_Value = HungerGauge::m_Hungervalue;
	////除算をする
	//m_Division = HungerGauge::GetInstance()->GetNowHunger() / 5.0f;
	//m_Alpha = m_Division - ((float)HungerGauge::GetInstance()->GetCatchCount() - 1.0f);
	//for (auto i = 0; i < ghosts.size(); ++i) {
	//	if (!ghosts[i]->GetAlive()) { continue; }
	//	if (!ghosts[i]->GetCatch()) { continue; }
	//	if (!ghosts[i]->GetFollow()) { continue; }

	//	m_Vanish = true;

	//	//for分抜ける
	//	if (m_Vanish) {
	//		ghosts[i]->SetColor({ ghosts[i]->GetColor().x,ghosts[i]->GetColor().y, ghosts[i]->GetColor().z, m_Alpha });
	//		if (ghosts[i]->GetColor().w <= 0.1f) {
	//			ghosts[i]->SetAlive(false);
	//			HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() - 1);
	//			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() - l_Value);
	//		}
	//		m_Vanish = false;
	//		break;
	//	}
	//}
}
//共通の更新
void LoadStageObj::CommonUpdate() {
	//ゴースト
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->Update();
	}

	//
	//食べ物
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->Update();
		/*	if (m_SceneName == "FIRSTSTAGE") {
				if (foods[i]->GetAlive() && foods[i] != nullptr && !m_EnemyManager->BossDestroy()) {
					lightgroup->SetCircleShadowDir(i + 2, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
					lightgroup->SetCircleShadowCasterPos(i + 2, XMFLOAT3({ foods[i]->GetPosition().x, foods[i]->GetPosition().y, foods[i]->GetPosition().z }));
					lightgroup->SetCircleShadowAtten(i + 2, XMFLOAT3(circleShadowAtten));
					lightgroup->SetCircleShadowFactorAngle(i + 2, XMFLOAT2(circleShadowFactorAngle));
				}
				else {
					lightgroup->SetCircleShadowActive(i + 2, false);
				}
			}
			else if (m_SceneName == "TUTORIAL") {
				if (foods[i]->GetAlive() && foods[i] != nullptr) {
					lightgroup->SetCircleShadowDir(i + 2, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
					lightgroup->SetCircleShadowCasterPos(i + 2, XMFLOAT3({ foods[i]->GetPosition().x, foods[i]->GetPosition().y, foods[i]->GetPosition().z }));
					lightgroup->SetCircleShadowAtten(i + 2, XMFLOAT3(circleShadowAtten));
					lightgroup->SetCircleShadowFactorAngle(i + 2, XMFLOAT2(circleShadowFactorAngle));
				}
				else {
					lightgroup->SetCircleShadowActive(i + 2, false);
				}
			}*/
	}

	//ハート
	for (auto i = 0; i < hearts.size(); i++) {
		hearts[i]->Update();
		/*if (m_SceneName == "FIRSTSTAGE") {
			if (hearts[i]->GetAlive() && hearts[i] != nullptr && !m_EnemyManager->BossDestroy()) {
				lightgroup->SetCircleShadowDir(((int)hearts.size() + 1) + 12, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
				lightgroup->SetCircleShadowCasterPos(((int)hearts.size() + 1) + 12, XMFLOAT3({ hearts[i]->GetPosition().x, hearts[i]->GetPosition().y, hearts[i]->GetPosition().z }));
				lightgroup->SetCircleShadowAtten(((int)hearts.size() + 1) + 12, XMFLOAT3(circleShadowAtten));
				lightgroup->SetCircleShadowFactorAngle(((int)hearts.size() + 1) + 12, XMFLOAT2(circleShadowFactorAngle));
			}
			else {
				lightgroup->SetCircleShadowActive(((int)hearts.size() + 1) + 12, false);
				lightgroup->SetCircleShadowCasterPos(((int)hearts.size() + 1) + 12, XMFLOAT3({1000.0f,0.0f,1000.0f}));
			}
		}
		else if(m_SceneName == "SECONDSTAGE") {
			if (hearts[i]->GetAlive() && hearts[i] != nullptr && !m_EnemyManager->BossDestroy()) {
				lightgroup->SetCircleShadowDir(i + 2, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
				lightgroup->SetCircleShadowCasterPos(i + 2, XMFLOAT3({ hearts[i]->GetPosition().x, hearts[i]->GetPosition().y, hearts[i]->GetPosition().z }));
				lightgroup->SetCircleShadowAtten(i + 2, XMFLOAT3(circleShadowAtten));
				lightgroup->SetCircleShadowFactorAngle(i + 2, XMFLOAT2(circleShadowFactorAngle));
			}
			else {
				lightgroup->SetCircleShadowActive(i + 2, false);
			}
		}*/
	}

	//食料の削除(このステージのみ)
	for (int i = 0; i < hearts.size(); i++) {
		if (hearts[i] == nullptr) {
			continue;
		}

		if (!hearts[i]->GetAlive()) {
			hearts.erase(cbegin(hearts) + i);
		}
	}
	//
	//当たり判定
	Collide();
	//食べ物の当たり判定
	CollideFood();
	//食べ物の検索
	SearchFood();
	//ゴーストが消える
	VanishGhost();
	//ハートの生成
	BirthHeart();
	//ライト生成
	LightReturn();
}
//ハートの生成
void LoadStageObj::BirthHeart() {
	if (!m_EnemyManager->GetBoss()) { return; }
	if (m_EnemyManager->GetBirthHeart()) {
		Heart* newHeart;
		newHeart = new Heart();
		newHeart->Initialize();
		newHeart->SetPosition({ m_EnemyManager->GetEnemyPosition().x,0.0f,m_EnemyManager->GetEnemyPosition().z });
		hearts.push_back(newHeart);
		m_EnemyManager->FinishHeart();
		/*for (int i = 0; i < hearts.size(); i++) {
			if (m_SceneName == "FIRSTSTAGE") {
				lightgroup->SetCircleShadowActive(((int)hearts.size() + 1) + 12, true);
			}
			else if (m_SceneName == "SECONDSTAGE") {
				lightgroup->SetCircleShadowActive(i + 2, true);
			}
		}*/
	}
}

void LoadStageObj::LightReturn() {
	/*for (auto i = 0; i < foods.size(); i++) {
		if (foods[i]->GetLightSet()) {
			lightgroup->SetCircleShadowActive(i + 2, true);
			foods[i]->SetLightSet(false);
		}
	}*/
}

void LoadStageObj::ThirdBossAction() {
	LockVerseGhost();
	NonVerseGhost();
	CheckReferGhost();
}

void LoadStageObj::LockVerseGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (!boss->GetSearch()) { return; }
	if (boss->GetStrong()) {
		kStopGhorstMax = 5;
	} else {
		kStopGhorstMax = 3;
	}
	int  nowStopGhorst = 0;
	while (nowStopGhorst < kStopGhorstMax) {
		for (auto i = 0; i < ghosts.size(); i++) {
			if (ghosts[i]->GetIsRefer()) { continue; }
			//キャラステート変える際に気をつけてください
			if (ghosts[i]->GetStateInst() == 2) { continue; }
			XMFLOAT3 difPos = ghosts[i]->GetPosition();
			float dif = Helper::GetInstance()->ChechLength(difPos, boss->GetPosition());
			if (boss->GetLimit() > dif) {
				stopGhosts[nowStopGhorst] = ghosts[i];
				ghosts[i]->SetIsRefer(true);
				nowStopGhorst++;
				if (nowStopGhorst >= kStopGhorstMax) {
					break;
				}
			}
		}
		boss->SetLimit(boss->GetLimit() + 5.0f);
	}
	boss->SetSearch(false);
}


void LoadStageObj::NonVerseGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	if (boss->GetInstruction() != InterBoss::ThirdBossInst::StopGhost) { return; }
	int m_GhostPos = 0;
	for (Ghost*& ghost : stopGhosts) {
		if (!ghost) { continue; }
		ghost->SetColor({ 1,0,1,1 });
		//ghost->SetScale({ 0.0f,0.0f,0.0f });
		ghost->SetIsPostionCheck(true);
		ghost->SetIsVerse(false);
		ghost->SetAlive(false);
		boss->SetJackPos(m_GhostPos, ghost->GetPosition());
		m_GhostPos++;
	}
	boss->SetInstruction(InterBoss::ThirdBossInst::SpawnEnemy);
}

bool LoadStageObj::CheckReferGhost() {
	InterBoss* boss = m_EnemyManager->GetBoss();
	int checkNum = 0;
	for (int i = 0; i < kStopGhorstMax; i++) {
		if (!stopGhosts[i]) { continue; }
		if (!stopGhosts[i]->GetIsRefer()) { continue; }
		checkNum++;
	}
	if (checkNum == 0) {
		boss->SetIsReferCheck(true);
		return false;
	} else {
		boss->SetIsReferCheck(false);
		return true;
	}
}
