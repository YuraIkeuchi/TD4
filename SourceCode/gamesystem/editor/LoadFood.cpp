#include "LoadFood.h"
#include "CsvLoader.h"

//���[�h
void LoadFood::Load()
{
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/food/food.csv", "Quantity")));

	foods.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
	/**/

	Name.resize(Size);


	//������
	for (auto i = 0; i < foods.size(); i++) {
		foods[i] = new Food();
		foods[i]->Initialize();
	}


	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/food/food.csv", Pos, "POSITION");
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/food/food.csv", Rot, "ROTATION");
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/food/food.csv", Scl, "SCALE");

	//���f���ύX�ɑΉ�
	for (auto i = 0; i < foods.size(); i++) {
	
		//foods[i]->SetPosition(Pos[i]);
		foods[i]->SetRotation(Rot[i]);
		foods[i]->SetScale(Scl[i]);
	}
}

//������
void LoadFood::Initialize()
{
	Load();
}

//�X�V
void LoadFood::Update()
{
	for (auto i = 0; i < foods.size(); i++)
	{

		foods[i]->Update();
	}
}

//�`��
void LoadFood::Draw(DirectXCommon* dxCommon)
{//tes
	for (auto i = 0; i < foods.size(); i++)
	{
		foods[i]->Draw(dxCommon);
	}

}
//ImGui
void LoadFood::ImGuiDraw() {
	for (auto i = 0; i < foods.size(); i++) {
		foods[i]->ImGuiDraw();
	}
}