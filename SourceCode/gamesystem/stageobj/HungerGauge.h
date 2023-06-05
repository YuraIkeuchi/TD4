#pragma once
#include<array>
//�Q��Q�[�W���Ǘ�����N���X
class HungerGauge {
public:
	static HungerGauge* GetInstance();
	bool Initialize();//������
	void Update();
	/// <summary>
	/// ImGui�`��
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

	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
	void SetHungerMax(float m_HungerMax) { this->m_HungerMax = m_HungerMax; }
	void SetCatchCount(int m_CatchCount) { this->m_CatchCount = m_CatchCount; }
	void SetSubVelocity(float m_SubVelocity) { this->m_SubVelocity = m_SubVelocity; }
public:
	static float m_Hungervalue;//���Z��]�Z�̒l
private:
	static const int SUB_MAX = 5;
private:
	float m_NowHunger = {};//���݂̋Q��Q�[�W
	float m_HungerMax = {};//�Q��Q�[�W�̍ő吔(�ω�����)
	int m_CatchCount = {};//�S�[�X�g�̐�
	float m_SubVelocity = 1.0f;//����Q��Q�[�W(�`���[�W���̂݌���j
	std::array<float,SUB_MAX> m_SubHunger;//�����
	bool carriedFood = false;
};