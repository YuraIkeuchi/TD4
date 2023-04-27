#pragma once

//‹Q‰ìƒQ[ƒW‚ğŠÇ—‚·‚éƒNƒ‰ƒX
class HungerGauge {
public:
	static HungerGauge* GetInstance();
	bool Initialize();//‰Šú‰»
	void Update();
	/// <summary>
	/// ImGui•`‰æ
	/// </summary>
	void ImGuiDraw();
public:
	//gettersetter
	float GetNowHunger() { return   m_NowHunger; }

	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
private:
	float m_NowHunger = {};//Œ»İ‚Ì‹Q‰ìƒQ[ƒW
	float m_HungerMax = {};//‹Q‰ìƒQ[ƒW‚ÌÅ‘å”(•Ï‰»‚·‚é)
};
