#include "LoadGhost.h"
#include "CsvLoader.h"

//���[�h
void LoadGhost::Load(Player* player)
{
	auto Size = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/ghost.csv", "Quantity")));

	ghosts.resize(Size);

	/**/
	Pos.resize(Size);
	Rot.resize(Size);
	Scl.resize(Size);
	/**/

	Name.resize(Size);


	//������
	for (auto i = 0; i < ghosts.size(); i++) {
		ghosts[i] = new Ghost();
		ghosts[i]->Initialize();
		ghosts[i]->SetPlayer(player);
	}


	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/chara/ghost.csv", Pos, "POSITION");
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/chara/ghost.csv", Rot, "ROTATION");
	LoadCSV::LoadCsvParam_XMFLOAT3("Resources/csv/chara/ghost.csv", Scl, "SCALE");

	////���f���ύX�ɑΉ�
	//for (auto i = 0; i < ghosts.size(); i++) {
	//
	//	//ghosts[i]->SetPosition(Pos[i]);
	//	ghosts[i]->SetRotation(Rot[i]);
	//	ghosts[i]->SetScale(Scl[i]);
	//}
}

//������
void LoadGhost::Initialize()
{
	//Load();
}

//�X�V
void LoadGhost::Update()
{
	for (auto i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->Update();
	}

	Collide();
}

//�`��
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
	/*ImGui::Begin("b");
	ImGui::Text("a:%d", a);
	ImGui::End();*/
}
//�����蔻��(�S�[�X�g)
void LoadGhost::Collide() {
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