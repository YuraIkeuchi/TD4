#pragma once

enum SeceneCategory {
	kTutorialStage=0,
	kFirstStage,
	kSecondStage
};


class SceneSave {
public:
	static SceneSave* GetInstance();

	void AllReset();

	void SetClearFlag(SeceneCategory sceneCategory,const bool flag);

	bool GetClearFlag(SeceneCategory sceneCategory);

private:
	static bool m_TutorialStageClear;
	static bool m_FirstStageClear;
	static bool m_SecondStageClear;
};

