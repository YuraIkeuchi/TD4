#include "HungerGauge.h"
#include "imgui.h"
#include"WinApp.h"
#include "CsvLoader.h"
#include "Helper.h"
float HungerGauge::m_Hungervalue = 5.0f;
HungerGauge* HungerGauge::GetInstance()
{
	static HungerGauge instance;

	return &instance;
}

//������
bool HungerGauge::Initialize() {
	m_CatchCount = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Catch")));
	m_NowHunger = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Now")));
	m_HungerMax = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Max")));
	return true;
}

//�X�V
void HungerGauge::Update() {
	//���鑬�x�����߂Ă���
	m_SubHunger = (m_CatchCount * 200.0f) / 1000.0f;
	float l_Limit = 50.0f;
	//��肸�Ō������Ă���
	m_NowHunger -= 0.01f;
	//�Q��Q�[�W�̍ő吔�����܂��Ă���
	Helper::GetInstance()->FloatClamp(m_NowHunger, 0.0f, m_HungerMax);
	m_NowHunger = min(m_NowHunger, m_HungerMax);
	m_HungerMax = min(m_HungerMax, l_Limit);
}

//ImGui
void HungerGauge::ImGuiDraw() {
	ImGui::Begin("Hunger");
	ImGui::Text("Sub:%f", m_SubHunger);
	ImGui::Text("Max:%f", m_HungerMax);
	ImGui::Text("Now:%f", m_NowHunger);
	ImGui::End();
}