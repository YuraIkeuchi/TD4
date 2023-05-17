#pragma once

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
	float GetCatchCount() { return   m_CatchCount; }

	float GetPercentage();

	void AddNowHunger(float m_NowHunger);

	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
	void SetHungerMax(float m_HungerMax) { this->m_HungerMax = m_HungerMax; }
	void SetCatchCount(float m_CatchCount) { this->m_CatchCount = m_CatchCount; }
	void SetSubVelocity(float m_SubVelocity) { this->m_SubVelocity = m_SubVelocity; }
public:
	static float m_Hungervalue;//���Z��]�Z�̒l
private:
	float m_NowHunger = {};//���݂̋Q��Q�[�W
	float m_HungerMax = {};//�Q��Q�[�W�̍ő吔(�ω�����)
	float m_CatchCount = {};//�S�[�X�g�̐�
	float m_SubVelocity = 1.0f;//����Q��Q�[�W(�`���[�W���̂݌���j
	float m_SubHunger = 0.0f;//�����
};
