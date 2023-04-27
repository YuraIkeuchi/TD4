#pragma once
#include "ObjCommon.h"
//����N���X
class Bullet :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Bullet();

	bool Initialize() override;//������
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGuiDraw();
private:
	//�e�̓�������
	void Move();
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetBulletType(const int BulletType) { m_BulletType = BulletType; }
private:
	XMFLOAT2 m_Angle = {};//�e�̊p�x
	float m_AddSpeed = {};//�����x
	int m_BulletType = {};//�e�̎��
	int m_Timer = {};//�o������
	int m_TargetTimer = {};//�o�����Ԃ̖ڕW
	bool m_Alive = true;//�����t���O
};
