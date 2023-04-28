#pragma once
#include "ObjCommon.h"
#include "Player.h"
//�S�[�X�g�N���X
class Ghost :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Ghost();

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
private://�X�e�[�g
	static void (Ghost::* stateTable[])();
private:
	//�p�[�e�B�N��
	void Particle();
	//�����蔻��(�e)
	bool BulletCollision();
	//�����蔻��(�v���C���[)
	bool PlayerCollision();
	//�H�����Y
	void BirthGhost();
	//�����Ȃ����
	void None();
	//�Ǐ]
	void Follow();
	//�T��
	void Search();

public://getter setter
	void SetPlayer(Player* player) { this->player.reset(player); }
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//�����t���O
	bool m_Catch = true;//�ߊl�t���O
	int m_Timer = 0;
	XMFLOAT3 m_BasePos = {};
private:
	//�L�����̏��
	enum CharaState
	{
		STATE_NONE,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState;

	//�~�^��(��)
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 5.0f;

private:
	XMFLOAT3 m_OldPos = {};
	bool m_Follow = false;
};
