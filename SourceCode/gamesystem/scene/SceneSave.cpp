#include "SceneSave.h"
#include <cassert>
#include "imgui.h"

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

void SceneSave::ImGuiDraw() {
}