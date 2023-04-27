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
	//�����蔻��
	bool Collision();
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
	int m_Timer = 0;
	int m_Type = 0;
private:
	//�L�����̏��
	enum CharaState
	{
		STATE_NONE,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState;
};
