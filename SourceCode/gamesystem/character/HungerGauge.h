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

	void SetNowHunger(float m_NowHunger) { this->m_NowHunger = m_NowHunger; }
private:
	float m_NowHunger = {};//���݂̋Q��Q�[�W
	float m_HungerMax = {};//�Q��Q�[�W�̍ő吔(�ω�����)
};
