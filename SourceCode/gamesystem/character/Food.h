#pragma once
#include "ObjCommon.h"
#include <Ghost.h>
//�H�ו��̃N���X
class Food :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Food();

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


	void CarryStart(Ghost* ghost);
private:
	//�p�[�e�B�N��
	void Particle();
	//�����蔻��
	bool Collision();
	//�H�����Y
	void BirthFood();
	//�H���폜
	void DeleteFood();

	bool CarriedGhost();
public://getter setter
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetLockOn() { return m_LockOn; }
	const bool& GetIsCarried() { return m_IsCarried; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetLockOn(const bool LockOn) { m_LockOn = LockOn; }
	void SetLimit(const bool Limit) { m_Limit = Limit; }
	void SetIsCarried(const bool IsCarried) { m_IsCarried = IsCarried; }

private:
	bool m_Alive = true;//�����t���O
	bool m_LockOn = false;
	bool m_IsCarried = false;
	int m_Timer = 0;
	
	Ghost* m_ghost = nullptr;
	//�������Ԃ𒴂������ǂ���
	bool m_Limit = false;
	//�폜�܂ł̎���
	int m_DeleteTimer = 0;
};
