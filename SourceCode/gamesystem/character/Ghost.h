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
	//�T���̂͂���
	void StartSearch(const XMFLOAT3& pos);
	//�T���̏I���
	void EndSearch();

private://�X�e�[�g
	static void (Ghost::* stateTable[])();
private:
	//�p�[�e�B�N��
	void Particle();
	//�����蔻��(�e)
	bool BulletCollision();
	//�����蔻��(�v���C���[)
	bool PlayerCollision();
	//�S�[�X�g���m�̓����蔻��
	bool GhostCollision(const XMFLOAT3& pos);
	//�H�����Y
	void BirthGhost();
	//�����Ȃ����
	void None();
	//�Ǐ]
	void Follow();
	//�T��
	void Search();
	//�H�ו����^��
	void CarryFood();
public://getter setter
	void SetPlayer(Player* player) { this->player.reset(player); }
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetCatch() { return m_Catch; }
	const bool& GetSearch() { return m_Search; }
	const float& GetLimit() { return m_Limit; }
	void SetCatch(const bool Catch) { m_Catch = Catch; }
	void Setma(const bool ma) { m_ma = ma; }
	void SetLimit(const float Limit) { m_Limit = Limit; }
private:
	unique_ptr<Player> player;
	bool m_Alive = true;//�����t���O
	bool m_Catch =false;//�ߊl�t���O
	int m_Timer = 0;
	XMFLOAT3 m_BasePos = {};
	bool m_ma = false;
private:
	//�L�����̏��
	enum CharaState
	{
		STATE_NONE,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState;

private:
	XMFLOAT3 m_OldPos = {};
	bool m_Follow = false;
private://�T��
	bool m_Search = false;
	XMFLOAT3 m_SearchPos = {};

	//�T�����
	enum SearchState {
		SEARCH_NO,
		SEARCH_START,
		SEARCH_END,
	}_searchState;

private:
	float m_Limit = {};
};
