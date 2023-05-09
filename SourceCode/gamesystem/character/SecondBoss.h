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

public:
#pragma region �U��
	struct Attack
	{
	private:
		bool AttackFlag;
	public:
		void Action();
	};

	Attack _charge;
#pragma endregion

private:
	//�v���C���[���m
	bool SearchPlayer;
	unique_ptr<IKETexture>impact1;


	unique_ptr<IKESprite>SearchPlayerTex;
	float SearchtexAlpha;
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
	//�_���[�W�H���������
	void DamAction();

	//���̈ʒu��
	void RemovePos();

	//��퓬��
	void NoBattleMove();

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
};