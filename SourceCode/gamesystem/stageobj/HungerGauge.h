#pragma once
#include<array>
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
	int GetCatchCount() { return   m_CatchCount; }
	bool GetFirstCarry() { return   carriedFood; }
	void ResetFirstCarry() { carriedFood = false; }

	float GetPercentage();

	void AddNowHunger(float m_NowHunger);

	void RecoveryNowHunger(float m_NowHunger);

	void SetIsStop(bool isStop) { this->isStop= isStop; }
	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
	void SetHungerMax(float m_HungerMax) { this->m_HungerMax = m_HungerMax; }
	void SetCatchCount(int m_CatchCount) { this->m_CatchCount = m_CatchCount; }
	void SetSubVelocity(float m_SubVelocity) { this->m_SubVelocity = m_SubVelocity; }
public:
	static float m_Hungervalue;//ÁZâ]ZÌl
private:
	static const int SUB_MAX = 5;
private:
	float m_NowHunger = {};//»ÝÌQìQ[W
	float m_HungerMax = {};//QìQ[WÌÅå(Ï»·é)
	int m_CatchCount = {};//S[XgÌ
	float m_SubVelocity = 1.0f;//¸éQìQ[W(`[WÌÝ¸éj
	std::array<float,SUB_MAX> m_SubHunger;//¸éÊ
	bool carriedFood = false;
	bool isStop = false;
};
