#pragma once
#include "ObjCommon.h"
//�n�[�g�N���X
class Heart :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Heart();

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
	void HeartJump();

	void HeartSet();
private://�X�e�[�g
	static void (Heart::* stateTable[])();
private:
	//�p�[�e�B�N��
	void Particle();
	//�����蔻��(�v���C���[)
	bool PlayerCollision();
public://getter setter
private:
	//�����t���O
	bool m_Alive = false;
	//��������
	int m_AliveTimer = 0;
private:
	enum HeartState {
		HEART_JUMP,
		HEART_SET,
	}_heartState;

	XMFLOAT3 m_AddPower = {};
};
