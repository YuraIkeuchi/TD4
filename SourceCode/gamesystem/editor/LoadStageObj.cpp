#include "LoadStageObj.h"
#include "CsvLoader.h"
#include "HungerGauge.h"
#include "Collision.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"

EnemyManager* LoadStageObj::enemy = nullptr;
//ゴーストのロード
void LoadStageObj::GhostLoad() {
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/ghost.csv", "Quantity")));

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
	}
	else if (sceneName == "SECONDSTAGE") {
		Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Stage2")));
	}
	else {
		assert(0);
	}

	foods.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
}
//すべてロード
void LoadStageObj::AllLoad(const std::string& sceneName)
{
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
//初期化
void LoadStageObj::Initialize()
{
	//Load();
}
void LoadStageObj::TutorialUpdate() {

	CommonUpdate();
}
//更新(ステージ1)
void LoadStageObj::FirstUpdate()
{
	//更新
	CommonUpdate();
}

//更新
void LoadStageObj::SecondUpdate()
{
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
	if (enemy->GetEnemyCheck()) {
		Food* newFood;
		newFood = new Food();
		newFood->Initialize();
		newFood->SetPosition({ enemy->GetEnemyPosition().x,0.0f,enemy->GetEnemyPosition().z});
		newFood->SetLimit(true);
		foods.push_back(newFood);
		enemy->FinishCheck();
	}
}
//描画
void LoadStageObj::Draw(DirectXCommon* dxCommon)
{	
	//ゴースト
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Draw(dxCommon);
	}
	//
	//食べ物
	for (auto i = 0; i < foods.size(); i++)
	{
		foods[i]->Draw(dxCommon);
	}
	//
}
//ImGui
void LoadStageObj::ImGuiDraw() {
	////ゴースト
	//for (auto i = 0; i < ghosts.size(); i++) {
	//	ghosts[i]->ImGuiDraw();
	//}
	////
	////食べ物
	//for (auto i = 0; i < foods.size(); i++) {
	//	foods[i]->ImGuiDraw();
	//}
	//
	enemy->ImGuiDraw();
}
//当たり判定(ゴースト)
void LoadStageObj::Collide() {
	for (auto i = 0; i < ghosts.size(); ++i) {
		for (auto j = 0; j < ghosts.size(); ++j) {
			XMFLOAT3 ghostpos = ghosts[i]->GetPosition();
			XMFLOAT3 ghostpos2 = ghosts[j]->GetPosition();
			if ((i == j)) { continue; }
			if((!ghosts[i]->GetAlive()) || (!ghosts[j]->GetAlive())) { continue; }
			if ((!ghosts[i]->GetFollow()) || (!ghosts[j]->GetFollow())) { continue; }
			if (Collision::SphereCollision(ghostpos,1.5f,ghostpos2,1.5f)) {
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
			if ((!ghosts[i]->GetSearch()) && (foods[j]->GetAlive()) && (!foods[j]->GetLockOn())&&(!foods[j]->GetIsCarried())) {
				if (l_dir < ghosts[i]->GetLimit()) {
					ghosts[i]->StartSearch(l_foodpos);
					foods[j]->SetLockOn(true);
				}
				else {
					ghosts[i]->SetLimit(ghosts[i]->GetLimit() + 1.0f);
				}
			}
		}
	}
}
//食料とゴーストの当たり判定
void LoadStageObj::CollideFood() {
	float l_Radius = 1.0f;
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
	float l_TargetCatchCount = HungerGauge::GetInstance()->GetCatchCount() - 1.0f;
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
			HungerGauge::GetInstance()->SetCatchCount(HungerGauge::GetInstance()->GetCatchCount() - 1.0f);
			HungerGauge::GetInstance()->SetHungerMax(HungerGauge::GetInstance()->GetHungerMax() - l_Value);
			m_Vanish = false;
			break;
		}
	}
}
//共通の更新
void LoadStageObj::CommonUpdate() {
	//ゴースト
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Update();
	}

	//
	//食べ物
	for (auto i = 0; i < foods.size(); i++)
	{
		foods[i]->Update();
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
}