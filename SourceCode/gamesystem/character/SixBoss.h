#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "BarrangeCD.h"
#include "ConfuCD.h"
#include "LineCD.h"
#include "DebuffCD.h"
#include "AttackNote.h"
#include "DamageArea.h"
#include "ConfuEffect.h"
#include "NoteEffect.h"
class SixBoss :
	public InterBoss {
public:
	SixBoss();

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
	//�C���^�[�o��
	void InterValMove();
	//�����̑I��
	void Choice();
	//�_���[�W�G���A�̃Z�b�g
	void LineSet();
	//�v���C���[�̃f�o�t
	void Debuff();
	//����
	void Confu();
	//�e��
	void Barrage();
	//�s���I���
	void EndMove();
	//CSV�ǂݍ��݌n
	void CSVLoad();
	//�m�[�c�̐���
	void BirthNote(const std::string& BarrageName);
	//���񂾂Ƃ��̃p�[�e�B�N��
	void DeathParticle();
private:
	static const int BULLET_NUM = 4;
	static const int CD_NUM = 4;
private:
	//�e�N���X
	array<unique_ptr<InterCD>, CD_NUM> cd;
	vector<AttackNote*> attacknotes;//�{��̃X�^���v
	unique_ptr<DamageArea> damagearea;//�_���[�W�G���A
	unique_ptr<ConfuEffect> confueffect;
	unique_ptr<NoteEffect> noteeffect;
	//�L�����̏��
	enum CharaState
	{
		STATE_INTER,
		STATE_CHOICE,
		STATE_LINE,
		STATE_DEBUFF,
		STATE_CONFU,
		STATE_BARRA,
		STATE_END
	};

	//��~����
	int m_StopTimer = 0;
	//�ǂ̍s���ɂ��邩
	int m_MoveState = {};

	//�֐��|�C���^
	static void(SixBoss::* stateTable[])();

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

	enum CDType {
		CD_LINE,
		CD_DEBUFF,
		CD_CONFU,
		CD_BARRA,
	};
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
