#pragma once
#include "ObjCommon.h"
enum CDState {
	CD_BIRTH,
	CD_STAY,
	CD_CATCH,
	CD_THROW,
	CD_DEATH
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
	virtual void CatchCD() {};
	virtual void ThrowCD() {};
	virtual void DeathCD() {};
public:
	//gettersetter
	const int& GetCDState() { return m_CDState; }

	void SetCDState(const int CDState) { m_CDState = CDState; }
protected:
	static void (InterCD::* stateTable[])();
private:
protected:
	int m_CDState = CD_BIRTH;

	//�㏸�x
	float m_AddPower = 0.0f;
	//�d�͉����x
	float m_Gravity = 0.02f;
};