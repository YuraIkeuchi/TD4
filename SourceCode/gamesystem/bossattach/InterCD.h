#pragma once
#include "ObjCommon.h"
enum CDState {
	CD_BIRTH,
	CD_STAY,
	CD_THROUGH,
	CD_CATCH,
	CD_THROW,
	CD_DEATH,
	CD_RESPORN,
};
using namespace std;         //  ���O��Ԏw��
//�����N���X
class InterCD :
public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//������
	virtual bool Initialize() = 0;
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);

	//�`��(�ŗL��)
	virtual void Origin_Draw(DirectXCommon* dxCommon) = 0;

	void ImGuiDraw();//ImGui�̕`��

protected:

	virtual void Action() = 0;//�{�X���L�̏���

	virtual void ImGui_Origin() = 0;//�{�X���ꂼ���ImGui

protected:
	//�����o�֐�
	virtual void BirthCD() {};
	virtual void StayCD() {};
	virtual void ThroughCD() {};
	virtual void CatchCD() {};
	virtual void ThrowCD() {};
	virtual void DeathCD() {};
	virtual void ResPornCD() {};
public:
	//gettersetter
	const int& GetCDState() { return m_CDState; }

	void SetCDState(const int CDState) { m_CDState = CDState; }

	void SetCatchPos(const XMFLOAT3 CatchPos) { m_CatchPos = CatchPos; }
protected:
	static void (InterCD::* stateTable[])();
private:
protected:
	//CD�̏��
	int m_CDState = CD_BIRTH;

	//�㏸�x
	float m_AddPower = 0.0f;
	//�d�͉����x
	float m_Gravity = 0.02f;
	//�L���b�`������̃|�W�V����
	XMFLOAT3 m_CatchPos = {};
	//������Ԃ̎���
	int m_ThrowTimer = {};
	double m_SpeedX = 0.0f;
	double m_SpeedZ = 0.0f;

	//���X�|�[������
	int m_ResPornTimer = {};
};