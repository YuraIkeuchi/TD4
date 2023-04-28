#include "LoadStageObj.h"
#include "CsvLoader.h"
//�S�[�X�g�̃��[�h
void LoadStageObj::GhostLoad() {
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/ghost.csv", "Quantity")));

	ghosts.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);

}
//�H�ו����[�h
void LoadStageObj::FoodLoad() {
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Quantity")));

	foods.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
}
//���ׂă��[�h
void LoadStageObj::AllLoad(Player* player)
{
	//�S�[�X�g�֌W
	GhostLoad();
	//������
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i] = new Ghost();
		ghosts[i]->Initialize();
		ghosts[i]->SetPlayer(player);
	}
	//�H�ו��֌W
	FoodLoad();

	//������
	for (auto i = 0; i < foods.size(); i++) {
		foods[i] = new Food();
		foods[i]->Initialize();
		foods[i]->SetPlayer(player);
	}
}

//������
void LoadStageObj::Initialize()
{
	//Load();
}

//�X�V
void LoadStageObj::Update()
{
	//�S�[�X�g
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Update();
	}
	Collide();
	//
	//�H�ו�
	for (auto i = 0; i < foods.size(); i++)
	{
		foods[i]->Update();
	}
	//
}

//�`��
void LoadStageObj::Draw(DirectXCommon* dxCommon)
{	
	//�S�[�X�g
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Draw(dxCommon);
	}
	//
	//�H�ו�
	for (auto i = 0; i < foods.size(); i++)
	{
		foods[i]->Draw(dxCommon);
	}
	//
}
//ImGui
void LoadStageObj::ImGuiDraw() {
	//�S�[�X�g
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i]->ImGuiDraw();
	}
	//
	//�H�ו�
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->ImGuiDraw();
	}
	//
}
//�����蔻��(�S�[�X�g)
void LoadStageObj::Collide() {
	for (auto i = 0; i < ghosts.size(); i++) {
		for (auto j = 0; j < ghosts.size(); j++) {
			if ((i != j)) {
				XMFLOAT3 m_OldPos = ghosts[i]->GetPosition();
				XMFLOAT3 m_OldPos2 = ghosts[j]->GetPosition();
				float l_DisX = ghosts[i]->GetPosition().x - ghosts[j]->GetPosition().x;
				float l_DisZ = ghosts[i]->GetPosition().z - ghosts[j]->GetPosition().z;
				//2�̋������v�Z
				float m_Distance = sqrtf(l_DisX * l_DisX + l_DisZ * l_DisZ);
				if (m_Distance < 1.0f) {
					ghosts[i]->Setma(true);
					ghosts[j]->Setma(true);
				}
			}
		}
	}
}