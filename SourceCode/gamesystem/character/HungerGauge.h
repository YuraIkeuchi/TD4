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
	void SetSubVelocity(float m_SubVelocity) { this->m_SubVelocity = m_SubVelocity; }
public:
	static float m_Hungervalue;//ÁZâ]ZÌl
private:
	float m_NowHunger = {};//»ÝÌQìQ[W
	float m_HungerMax = {};//QìQ[WÌÅå(Ï»·é)
	float m_CatchCount = {};//S[XgÌ
	float m_SubVelocity = 1.0f;//¸éQìQ[W(`[WÌÝ¸éj
};
