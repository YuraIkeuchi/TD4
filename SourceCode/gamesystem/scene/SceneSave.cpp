#include "SceneSave.h"
#include <cassert>
#include "imgui.h"
bool SceneSave::m_FirstStageClear = false;
bool SceneSave::m_SecondStageClear = false;
bool SceneSave::m_TutorialStageClear = false;
bool SceneSave::m_ThirdStageClear = false;
bool SceneSave::m_FourthStageClear = false;
bool SceneSave::m_FiveStageClear = false;
bool SceneSave::m_SixStageClear = false;
bool SceneSave::m_SevenStageClear = false;

SceneSave* SceneSave::GetInstance() {
	static SceneSave instance;
	return &instance;
}

void SceneSave::AllReset() {
	m_FirstStageClear = false;
	m_SecondStageClear = false;
	m_TutorialStageClear = false;
	m_ThirdStageClear = false;
	m_FourthStageClear = false;
	m_FiveStageClear = false;
	m_SixStageClear = false;
	m_SevenStageClear = false;

}

void SceneSave::SetClearFlag(SeceneCategory sceneCategory, const bool flag) {
	switch (sceneCategory) {
	case kTutorialStage:
		m_TutorialStageClear = flag;
		break;
	case kFirstStage:
		m_FirstStageClear = flag;
		break;
	case kSecondStage:
		m_SecondStageClear = flag;
		break;
	case kThirdStage:
		m_ThirdStageClear = flag;
		break;
	case kFourthStage:
		m_FourthStageClear = flag;
		break;
	case kFiveStage:
		m_FiveStageClear = flag;
		break;
	case kSixStage:
		m_SixStageClear = flag;
		break;
	case kSevenStage:
		m_SevenStageClear = flag;
		break;
	default:
		assert(0);
		break;
	}
}

bool SceneSave::GetClearFlag(SeceneCategory sceneCategory) {
	switch (sceneCategory) {
	case kTutorialStage:
		return m_TutorialStageClear;
		break;
	case kFirstStage:
		return m_FirstStageClear;
		break;
	case kSecondStage:
		return m_SecondStageClear;
		break;
	case kThirdStage:
		return m_ThirdStageClear;
		break;
	case kFourthStage:
		return m_FourthStageClear;
		break;
	case kFiveStage:
		return m_FiveStageClear;
		break;
	case kSixStage:
		return m_SixStageClear;
		break;
	case kSevenStage:
		return m_SevenStageClear;
		break;
	default:
		assert(0);
		return false;
		break;
	}
}

void SceneSave::ImGuiDraw() {
	ImGui::Begin("Save");
	ImGui::Text("Tutorial:%d", m_TutorialStageClear);
	ImGui::Text("FIrst:%d", m_FirstStageClear);
	ImGui::Text("Second:%d", m_SecondStageClear);
}