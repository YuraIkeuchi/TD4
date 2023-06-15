#pragma once
#include "InterBoss.h"
#include "Shake.h"
#include "Note.h"
#include "AttackNote.h"
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
	//�����̑I��
	void Choice();
	//�ӂ��̒e��
	void NormalBarrage();
	//���݂̒e
	void AlterBarrage();
	//�����_���̏��
	void RandomBarrage();
	//CSV�ǂݍ��݌n
	void CSVLoad();
	//�m�[�c�̐���
	void BirthNote(const std::string& BarrageName);

private:
	static const int BULLET_NUM = 4;
private:
	//�e�N���X
	unique_ptr<Note> note;
	vector<AttackNote*> attacknotes;//�{��̃X�^���v
	//�L�����̏��
	enum CharaState
	{
		STATE_CHOICE,
		STATE_NORMAL,
		STATE_ALTER,
		STATE_RANDOM,
	}_charaState;

	//��~����
	int m_StopTimer = 0;
	//�ǂ̍s���ɂ��邩
	int m_MoveState = {};

	//�֐��|�C���^
	static void(FourthBoss::* stateTable[])();
	

	//CSV�n
	int m_ChoiceInterval = {};

	//�C�[�W���O��̈ʒu
	XMFLOAT3 m_AfterPos = {};
	//X�����̉�]
	XMFLOAT3 m_AfterRot = { 0.0f,0.0f,0.0f };
	float m_Frame = {};

	enum BarrageState {
		BARRA_SET,
		BARRA_BIRTH,
		BARRA_END,
	};

	int m_BarraState = {};

	int m_RotCount = 0;
	int m_RotTimer = 0;
};
