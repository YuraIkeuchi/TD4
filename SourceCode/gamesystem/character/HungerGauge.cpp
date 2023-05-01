#include "HungerGauge.h"
#include "imgui.h"
#include"WinApp.h"
#include "CsvLoader.h"
HungerGauge* HungerGauge::GetInstance()
{
	static HungerGauge instance;

	return &instance;
}

//初期化
bool HungerGauge::Initialize() {
	m_NowHunger = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Now")));
	m_HungerMax = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Max")));
	return true;
}

//更新
void HungerGauge::Update() {
	float l_SubHunger = 0.001f;
	//一定ずつで減少していく
	m_NowHunger -= l_SubHunger;
	//飢餓ゲージの最大数が決まっている
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