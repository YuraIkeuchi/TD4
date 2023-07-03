#pragma once
#include "InterBullet.h"
//����N���X
class AttackBullet :
	public InterBullet {
public:
	AttackBullet();

	bool Initialize() override;//������

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGui_Origin() override;
private:
	//�e�̓�������
	void Action() override;

private:
	//�ӂ��̒e
	void NormalShot();
	//�{�X���z�������Ƃ��̓���
	void AbsorptionShot();
private:
	static const int POWER_MAX = 4;
private:
	//�C�[�W���O��̈ʒu
	XMFLOAT3 m_AfterPos = {};
	//�~�^��
	float m_CircleScale = 30.0f;
	float m_CircleSpeed = {};

	vector<float>m_Power;
};
