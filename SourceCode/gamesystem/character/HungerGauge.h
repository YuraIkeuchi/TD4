#pragma once

//QìQ[WðÇ·éNX
class HungerGauge {
public:
	static HungerGauge* GetInstance();
	bool Initialize();//ú»
	void Update();
	/// <summary>
	/// ImGui`æ
	/// </summary>
	void ImGuiDraw();
public:
	//gettersetter
	float GetNowHunger() { return   m_NowHunger; }
	float GetHungerMax() { return   m_HungerMax; }
	float GetCatchCount() { return   m_CatchCount; }

	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
	void SetHungerMax(float m_HungerMax) { this->m_HungerMax = m_HungerMax; }
	void SetCatchCount(float m_CatchCount) { this->m_CatchCount = m_CatchCount; }
private:
	float m_NowHunger = {};//»ÝÌQìQ[W
	float m_HungerMax = {};//QìQ[WÌÅå(Ï»·é)
	float m_CatchCount = {};//S[XgÌ
};
