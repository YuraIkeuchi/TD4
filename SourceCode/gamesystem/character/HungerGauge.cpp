#include "HungerGauge.h"
#include "imgui.h"
#include"WinApp.h"
#include "CsvLoader.h"
HungerGauge* HungerGauge::GetInstance()
{
	static HungerGauge instance;

	return &instance;
}

//������
bool HungerGauge::Initialize() {
	m_NowHunger = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Now")));
	m_HungerMax = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Max")));
	return true;
}

//�X�V
void HungerGauge::Update() {
	float l_SubHunger = 0.001f;
	//��肸�Ō������Ă���
	m_NowHunger -= l_SubHunger;
	//�Q��Q�[�W�̍ő吔�����܂��Ă���
	m_NowHunger = min(m_NowHunger, m_HungerMax);
}

//ImGui
void HungerGauge::ImGuiDraw() {
	ImGui::Begin("Hunger");
	ImGui::Text("Now:%f", m_NowHunger);
	ImGui::Text("Max:%f", m_HungerMax);
	ImGui::Text(" m_CatchCount:%f", m_CatchCount);
	ImGui::End();
}