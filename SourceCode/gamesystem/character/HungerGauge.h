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

	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
private:
	float m_NowHunger = {};//»ÝÌQìQ[W
	float m_HungerMax = {};//QìQ[WÌÅå(Ï»·é)
};
