#pragma once
#include "ObjCommon.h"
//�w�i�̊�Ȃǋ��ʔw�i�̃N���X
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
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetBulletType(const int BulletType) { m_BulletType = BulletType; }
private:
	XMFLOAT2 m_Angle = {};
	float m_AddSpeed = {};
	int m_BulletType = {};
};
