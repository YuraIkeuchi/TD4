#include "SceneSave.h"
#include <cassert>
bool SceneSave::m_FirstStageClear = false;
bool SceneSave::m_SecondStageClear = false;
bool SceneSave::m_TutorialStageClear = false;

SceneSave* SceneSave::GetInstance() {
	static SceneSave instance;
	return &instance;
}

void SceneSave::AllReset() {
	m_FirstStageClear = false;
	m_SecondStageClear = false;
	m_TutorialStageClear = false;
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
	default:
		assert(0);
		return false;
		break;
	}
}
