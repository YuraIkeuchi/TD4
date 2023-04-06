#include "VolumManager.h"
#include "imgui.h"
//Ã“Iƒƒ“ƒo•Ï”‚ÌŽÀ‘Ô
float VolumManager::s_SEVolum = 0.05f;
float VolumManager::s_BGMVolum = 0.03f;

VolumManager* VolumManager::GetInstance()
{
	static VolumManager instance;

	return &instance;
}

void VolumManager::Initialize() {

}

void VolumManager::Update() {
	//‰¹—Ê‚Í“Á’è‚Ü‚Å‚µ‚©“®‚©‚È‚¢
	if (s_SEVolum > 1.0f) {
		s_SEVolum = 1.0f;
	}
	else if (s_SEVolum < 0.0f) {
		s_SEVolum = 0.0f;
	}

	if (s_BGMVolum > 1.0f) {
		s_BGMVolum = 1.0f;
	}
	else if (s_BGMVolum < 0.0f) {
		s_BGMVolum = 0.0f;
	}
}

void VolumManager::Draw() {
	/*ImGui::Begin("Volum");
	ImGui::SetWindowPos(ImVec2(1000, 50));
	ImGui::SetWindowSize(ImVec2(280, 300));
	ImGui::SliderFloat("s_BGMVolum", &s_BGMVolum, 360, -360);
	ImGui::SliderFloat("s_SEVolum", &s_SEVolum, 360, -360);
	ImGui::End();*/
}

void VolumManager::Finalize() {

}