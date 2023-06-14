#pragma once
#include "ObjCommon.h"
#include "InterBullet.h"
#include "Collision.h"
#include "CollisionPrimitive.h"
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
	void Particle();
	//�����蔻��(�v���C���[)
	bool PlayerCollision();
	//�H�����Y
	void BirthGhost();
	//�����Ȃ����
	void None();
	//���܂����
	void Spawm();
	//�Ǐ]
	void Follow();
	//�T��
	void Search();
	//�H�ו����^��
	void CarryFood();
	bool CollideBullet(vector<InterBullet*>bullet);
public:
	//�S�[�X�g���m�̓����蔻��
	void GhostCollision(const XMFLOAT3& pos);
public://getter setter
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const bool& GetCatch() { return m_Catch; }
	const bool& GetFollow() { return m_Follow; }
	const bool& GetSearch() { return m_Search; }
	const bool& GetIsVerse() { return isVerse; }
	const float& GetLimit() { return m_Limit; }
	void SetIsVerse(const bool isVerse) { this->isVerse = isVerse; }
	void SetCatch(const bool Catch) { m_Catch = Catch; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetLimit(const float Limit) { m_Limit = Limit; }
private:
	bool m_Alive = true;//�����t���O
	bool m_Catch = false;//�ߊl�t���O
	bool isVerse = true;//���X�|�[���t���O
	int m_ResPornTimer = 0;//�����̎���
	XMFLOAT3 m_FollowPos = {};//�Ǐ]��
	XMFLOAT3 m_OBBScale = {};//OBB�p�̑傫��
private:
	//�L�����̏��
	enum CharaState {
		STATE_NONE,
		STATE_SPAWN,
		STATE_FOLLOW,
		STATE_SEARCH,
	}_charaState = CharaState::STATE_NONE;

private:
	IKEModel* model_follow = nullptr;
	IKEModel* model_seach = nullptr;

	XMFLOAT3 m_OldPos = {};
	bool m_Follow = false;
	//�Ǐ]���
	enum FollowState {
		Follow_NO,
		Follow_START,
		Follow_END,
	}_followState = FollowState::Follow_NO;
private://�T��
	bool m_Search = false;
	XMFLOAT3 m_SearchPos = {};

	float noneTimer = 0.0f;

	float m_SpawnTimer = 0.f;
	float kSpawnTimerMax = 60.f;

	//�T�����
	enum SearchState {
		SEARCH_NO,
		SEARCH_START,
		SEARCH_END,
	}_searchState = SearchState::SEARCH_NO;

	int m_SearchTimer = 0;
private:
	//�T��������͈̂̔�
	float m_Limit = {};

	bool m_Hit = false;

	OBB m_OBB1 = {}, m_OBB2 = {};
};
