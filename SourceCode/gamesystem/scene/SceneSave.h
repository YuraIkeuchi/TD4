#pragma once

enum SeceneCategory {
	kTutorialStage=0,
	kFirstStage,
	kSecondStage,
	kThirdStage,
	kFourthStage,
	kFiveStage,
	kSixStage,
	kSevenStage
};


class SceneSave {
public:
	static SceneSave* GetInstance();

	void AllReset();

	void SetClearFlag(SeceneCategory sceneCategory,const bool flag);

	bool GetClearFlag(SeceneCategory sceneCategory);

	void ImGuiDraw();
private:
	static bool m_TutorialStageClear;
	static bool m_FirstStageClear;
	static bool m_SecondStageClear;
	static bool m_ThirdStageClear;
	static bool m_FourthStageClear;
	static bool m_FiveStageClear;
	static bool m_SixStageClear;
	static bool m_SevenStageClear;
};

