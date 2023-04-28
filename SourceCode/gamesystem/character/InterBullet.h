#pragma once
#include "ObjCommon.h"
using namespace std;         //  ���O��Ԏw��

//�e�̊��N���X
class InterBullet :
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

	virtual void Action() = 0;//�e���L�̏���
	//ImGui����
	void ImGuiDraw();
	//���L��ImGui
	virtual void ImGui_Origin() = 0;
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const int& GetBulletType() { return m_BulletType; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetBulletType(const int BulletType) { m_BulletType = BulletType; }

public:
	XMFLOAT2 m_Angle = {};//�e�̊p�x
	float m_AddSpeed = {};//�����x
	int m_BulletType = {};//�e�̎��
	int m_Timer = {};//�o������
	int m_TargetTimer = {};//�o�����Ԃ̖ڕW
	bool m_Alive = true;//�����t���O
};