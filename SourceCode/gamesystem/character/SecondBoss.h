#pragma once
#include"IKESprite.h"
#include "InterBoss.h"
class SecondBoss :
	public InterBoss {
public:
	SecondBoss();
	bool Initialize() override;//������

	void Pause() override;//�|�[�Y
private:
	void Action() override;//�s��

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	//��{�ړ�
	void Move();
private:
	//�_���[�W�H���������
	void DamAction();

	//���̈ʒu��
	void RemovePos();

	//��퓬��
	void NoBattleMove();

public:
#pragma region �U��
	struct Attack
	{
	private:
		bool AttackFlag;
	public:
		void Action();
	public:
		void SetAttackF(bool flag) { AttackFlag = flag; }
		bool GetAttackF() { return AttackFlag; }
	};

	Attack _attack;
#pragma endregion

private:
	//�v���C���[���m
	bool SearchPlayer;
	unique_ptr<IKETexture>impact1;
private:
	//�摜�g���̂ł����
	std::array<float, 2>texalpha;
	std::array<XMFLOAT3, 2>texscl;
private:
	//��_���[�W����
	bool Recv;
	//�F�ς���悤
	bool DamColSetF;

	//�ҋ@����
	int StayCount;
	//�ҋ@�t���O
	bool StayF;

	//�����������Ɉړ�����悤
	XMVECTOR m_move = { 0.f,0.f, 0.1f, 0.0f };

	XMMATRIX m_matRot;
private:
	//�U��
	bool NormalAttackF;
	//�O���W
	XMFLOAT3 OldPos;

	float EaseT_BatStart;
	//�퓬�J�n
	bool BattleStartF;
	//�G���J�E���g
	bool EncF;
private:
	void CollideBul(vector<InterBullet*>bullet);

	inline void IsOldPos()
	{
		m_Position = OldPos;
	}
};