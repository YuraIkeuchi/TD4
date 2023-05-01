#pragma once
#include "ObjCommon.h"
#include "Player.h"
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
private:
	//�p�[�e�B�N��
	void Particle();
	//�����蔻��
	bool Collision();
	//�H�����Y
	void BirthFood();
public://getter setter
	void SetPlayer(Player* player) { this->player.reset(player); }
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetLockOn() { return m_LockOn; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetLockOn(const bool LockOn) { m_LockOn = LockOn; }
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//�����t���O
	bool m_LockOn = false;
	int m_Timer = 0;
};
