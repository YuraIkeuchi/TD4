#include "SceneSave.h"
#include <cassert>
#include "imgui.h"
#include "Helper.h"
bool SceneSave::m_StageClear[(size_t)kMaxStage] = { };
bool SceneSave::m_StageLose[(size_t)kMaxStage] = { };

SceneSave* SceneSave::GetInstance() {
	static SceneSave instance;
	return &instance;
}

void SceneSave::AllReset() {
	for (int i = 0; i < kMaxStage; i++) {
		m_StageClear[(size_t)i] = false;
	}

	for (int i = 0; i < kMaxStage; i++) {
		m_StageLose[(size_t)i] = false;
	}
}

void SceneSave::SetClearFlag(SeceneCategory sceneCategory, const bool flag) {
	m_StageClear[(size_t)sceneCategory] = flag;
}

bool SceneSave::GetClearFlag(SeceneCategory sceneCategory) {
	return m_StageClear[(size_t)sceneCategory];
}

void SceneSave::SetLoseFlag(SeceneCategory sceneCategory, const bool flag) {
	m_StageLose[(size_t)sceneCategory] = flag;
}

bool SceneSave::GetLoseFlag(SeceneCategory sceneCategory) {
	return m_StageLose[(size_t)sceneCategory];
}

//全ステージクリア
void SceneSave::AllClear() {
	m_StageClear[kTutorialStage] = true;
	bool temp[kMaxStage - 1] = {};
	for (auto i = 0; i < kMaxStage - 1; i++){
		temp[i] = m_StageClear[(size_t)i];
	}
	if (Helper::GetInstance()->All_OfF(temp, kMaxStage - 1)) {
		m_AllClear = true;
	}
	else {
		m_AllClear = false;
	}
}

void SceneSave::ImGuiDraw() {
	ImGui::Begin("Save");
	ImGui::Text("All:%d", m_AllClear);
	for (auto i = 0; i < kMaxStage; i++) {
		ImGui::Text("Clear[%d]:%d", i, m_StageClear[(size_t)i]);
	}
	ImGui::End();
}