#include "LoadStageObj.h"
#include "CsvLoader.h"
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
void LoadStageObj::FoodLoad() {
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Quantity")));

	foods.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
}
//すべてロード
void LoadStageObj::AllLoad(Player* player)
{
	//ゴースト関係
	GhostLoad();
	//初期化
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i] = new Ghost();
		ghosts[i]->Initialize();
		ghosts[i]->SetPlayer(player);
	}
	//食べ物関係
	FoodLoad();

	//初期化
	for (auto i = 0; i < foods.size(); i++) {
		foods[i] = new Food();
		foods[i]->Initialize();
		foods[i]->SetPlayer(player);
	}
}

//初期化
void LoadStageObj::Initialize()
{
	//Load();
}

//更新
void LoadStageObj::Update()
{
	//ゴースト
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Update();
	}
	Collide();
	//
	//食べ物
	for (auto i = 0; i < foods.size(); i++)
	{
		foods[i]->Update();
	}
	//
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
	//ゴースト
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->ImGuiDraw();
	}
	//
	//食べ物
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->ImGuiDraw();
	}
	//
}
//当たり判定(ゴースト)
void LoadStageObj::Collide() {
	for (auto i = 0; i < ghosts.size(); i++) {
		for (auto j = 0; j < ghosts.size(); j++) {
			if ((i != j)) {
				XMFLOAT3 m_OldPos = ghosts[i]->GetPosition();
				XMFLOAT3 m_OldPos2 = ghosts[j]->GetPosition();
				float l_DisX = ghosts[i]->GetPosition().x - ghosts[j]->GetPosition().x;
				float l_DisZ = ghosts[i]->GetPosition().z - ghosts[j]->GetPosition().z;
				//2つの距離を計算
				float m_Distance = sqrtf(l_DisX * l_DisX + l_DisZ * l_DisZ);
				if (m_Distance < 1.0f) {
					ghosts[i]->Setma(true);
					ghosts[j]->Setma(true);
				}
			}
		}
	}
}