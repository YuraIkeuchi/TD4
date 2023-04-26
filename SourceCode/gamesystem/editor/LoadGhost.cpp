#include "LoadGhost.h"
#include "CsvLoader.h"

//ロード
void LoadGhost::Load(Player* player)
{
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/ghost/ghost.csv", "Quantity")));

	ghosts.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
	/**/

	Name.resize(Size);


	//初期化
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i] = new Ghost();
		ghosts[i]->Initialize();
		ghosts[i]->SetPlayer(player);
	}


	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/ghost/ghost.csv", Pos, "POSITION");
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/ghost/ghost.csv", Rot, "ROTATION");
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/ghost/ghost.csv", Scl, "SCALE");

	////モデル変更に対応
	//for (auto i = 0; i < ghosts.size(); i++) {
	//
	//	//ghosts[i]->SetPosition(Pos[i]);
	//	ghosts[i]->SetRotation(Rot[i]);
	//	ghosts[i]->SetScale(Scl[i]);
	//}
}

//初期化
void LoadGhost::Initialize()
{
	//Load();
}

//更新
void LoadGhost::Update()
{
	for (auto i = 0; i < ghosts.size(); i++)
	{

		ghosts[i]->Update();
	}
}

//描画
void LoadGhost::Draw(DirectXCommon* dxCommon)
{//tes
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Draw(dxCommon);
	}

}
//ImGui
void LoadGhost::ImGuiDraw() {
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->ImGuiDraw();
	}
}