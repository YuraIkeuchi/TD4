#pragma once
#include "InterBoss.h"
#include "JoyStamp.h"
#include "AngerStamp.h"
#include "ShockWave.h"
#include "Predict.h"
#include "Collision.h"
#include "Shake.h"
#include "ConfuEffect.h"
#include "NoteEffect.h"

class Spline;

class FiveBoss : 
	public InterBoss
{
public:
	FiveBoss();

	bool Initialize() override;//������

	void SkipInitialize() override;//�X�L�b�v���̏�����

	void Pause() override;//�|�[�Y

	void Action() override;//�s��

	void AppearAction() override;//�{�X�o��̌ŗL�̏���

	void DeadAction() override;//�{�X���j�̌ŗL�̏���

	void DeadAction_Throw() override;//�{�X���j�̌ŗL�̏��� �X���[

	void ImGui_Origin() override;//�{�X���ꂼ���ImGui

	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:

private:
	//CSV�ǂݍ��݌n
	void CSVLoad();
	//���񂾂Ƃ��̃p�[�e�B�N��
	void DeathParticle();
private:
	static const int CD_NUM = 4;
private:
	//�e�N���X
	unique_ptr<ConfuEffect> confueffect;
	unique_ptr<NoteEffect> noteeffect;
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_CHOICE,
		STATE_ROCKON,
		STATE_RAND,
		STATE_HIT,
		STATE_END
	};

	//��~����
	int m_StopTimer = 0;
	//�ǂ̍s���ɂ��邩
	int m_MoveState = {};


	int _charaState = STATE_INTER;

	//CSV�n
	int m_ChoiceInterval = {};

	//�C�[�W���O��̈ʒu
	XMFLOAT3 m_AfterPos = {};
	//X�����̉�]
	XMFLOAT3 m_AfterRot = { 0.0f,0.0f,0.0f };
	float m_Frame = {};

	int m_RotCount = 0;
	int m_RotTimer = 0;

	enum AreaState {
		AREA_SET,
		AREA_STOP,
		AREA_END,
	};

	int m_AreaState = AREA_SET;
	float SplineSpeed = false;

	//�����̃C���^�[�o��
	int m_MoveInterVal = {};
	//�s���I���̐�
	int m_EndCount = {};
	//�L���b�`����CD�̐�
	int m_CatchCount = {};
	//�{�X���v���C���[���瓦���鎞��
	int m_EndTimer = {};

	//���̓I�Ɏg��
	float m_Angle = 0.0f;
	float m_Angle2 = 0.0f;
	//��_�̋���
	float m_Length = {};

	//�~�^��
	float m_CircleScale = 30.0f;
	float m_CircleSpeed = {};

	//�e���̎��
	int m_BarraRand = {};

	int m_AttackRand = {};

	//CSV�n
	//�e�C���^�[�o���⃊�~�b�g����
	vector<int>m_Limit;

	enum LimitState {
		LIMIT_BASE,
		LIMIT_CONFU,
		LIMIT_STRONG_CONFU,
		LIMIT_BARRA,
	};

	//�ړ���
	float m_FollowSpeed = {};
};

