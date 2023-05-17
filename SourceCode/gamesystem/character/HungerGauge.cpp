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

//初期化
bool HungerGauge::Initialize() {
	m_CatchCount = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Catch")));
	m_NowHunger = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Now")));
	m_HungerMax = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/hungergauge.csv", "Max")));
	return true;
}

//更新
void HungerGauge::Update() {
	//減る速度を求めている
	float l_SubHunger = (m_CatchCount * (m_Hungervalue * m_SubVelocity)) / 1000;
	float l_Limit = 50.0f;
	//一定ずつで減少していく
	m_NowHunger -= l_SubHunger;
	//飢餓ゲージの最大数が決まっている
	Helper::GetInstance()->FloatClamp(m_NowHunger, 0.0f, m_HungerMax);
	m_NowHunger = min(m_NowHunger, m_HungerMax);
	m_HungerMax = min(m_HungerMax, l_Limit);
}

//ImGui
void HungerGauge::ImGuiDraw() {
	ImGui::Begin("Hunger");
	ImGui::SliderFloat("Now", &m_NowHunger, 0.0f, 50.0f);
	ImGui::SliderFloat("Max", &m_HungerMax, 0.0f, 50.0f);
	//ImGui::SliderFloat("Percent", &(m_NowHunger / m_HungerMax), 0.0f, 50.0f);
	ImGui::Text("m_SubVelocity:%f", m_SubVelocity);
	ImGui::Text("CatchCount:%f", m_CatchCount);
	ImGui::End();
}

float HungerGauge::GetPercentage() {
	float temp= m_NowHunger / m_HungerMax;
	Helper::GetInstance()->FloatClamp(temp,0.0f,1.0f);
	return temp;
}

void HungerGauge::AddNowHunger(float m_NowHunger) {
	float temp = m_NowHunger;
	temp = min(m_NowHunger, m_HungerMax);
	this->m_NowHunger = temp;
}
