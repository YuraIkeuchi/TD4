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
class FourthBoss :
	public InterBoss {
public:
	FourthBoss();

	bool Initialize() override;//������

	void SkipInitialize() override;//�X�L�b�v���̏�����

	void Pause() override;//�|�[�Y

	void Action() override;//�s��

	void AppearAction() override;//�{�X�o��̌ŗL�̏���

	void DeadAction() {};//�{�X���j�̌ŗL�̏���

	void DeadAction_Throw() {};//�{�X���j�̌ŗL�̏��� �X���[

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
	//������
	void Throw();
	//�s���I���
	void EndMove();
	//CSV�ǂݍ��݌n
	void CSVLoad();
	//�m�[�c�̐���
	void BirthNote(const std::string& BarrageName);

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
		STATE_THROW,
		STATE_END
	};

	//��~����
	int m_StopTimer = 0;
	//�ǂ̍s���ɂ��邩
	int m_MoveState = {};

	//�֐��|�C���^
	static void(FourthBoss::* stateTable[])();
	
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
	//CD�𓊂���ۂ̃^�C�}�[
	int m_ThrowTimer = {};

	//��������
	enum ThrowState {
		THROW_SET,
		THROW_NOW,
		THROW_END
	};

	int m_ThrowState = THROW_SET;

	//�s���I���̐�
	int m_EndCount = {};
	//�L���b�`����CD�̐�
	int m_CatchCount = {};
	//�{�X���v���C���[���瓦���鎞��
	int m_EndTimer = {};
	//�����鑬��
	float m_FollowSpeed = -0.2f;

	//���̓I�Ɏg��
	float m_Angle = 0.0f;
	float m_Angle2 = 0.0f;
	//��_�̋���
	float m_Length = {};
};
