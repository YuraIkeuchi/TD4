#pragma once

enum SeceneCategory {
	kTutorialStage=0,
	kFirstStage,
	kSecondStage,
	kThirdStage,
	kFourthStage,
	kFiveStage,
	kSixStage,
	kSevenStage,
	kMaxStage
};


class SceneSave {
public:
	static SceneSave* GetInstance();

	void AllReset();

	void SetClearFlag(SeceneCategory sceneCategory,const bool flag);

	bool GetClearFlag(SeceneCategory sceneCategory);

	void SetLoseFlag(SeceneCategory sceneCategory, const bool flag);

	bool GetLoseFlag(SeceneCategory sceneCategory);

	void ImGuiDraw();
private:

	static bool m_StageClear[(size_t)kMaxStage];

	static bool m_StageLose[(size_t)kMaxStage];
};

