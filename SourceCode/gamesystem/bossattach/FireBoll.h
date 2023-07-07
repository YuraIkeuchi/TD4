#pragma once
#include "ObjCommon.h"
//�΂̋ʃN���X
class FireBoll :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	FireBoll();
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

	bool Collide();	//�����蔻��

	void Move();

	void BirthParticle();
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetCircleSpeed(const float CircleSpeed) { m_CircleSpeed = CircleSpeed; }
private:
	bool m_Alive = 0.0f;
	int m_MoveTimer = {};
	float m_CircleSpeed = {};
	float m_CircleScale = {};
	float m_Damage = {};
	float m_Frame = {};
	XMFLOAT3 m_TargetPos = {};
	enum MoveState {
		MOVE_SET,
		MOVE_PLAY,
		MOVE_OMEN,
		MOVE_ATTACK,
		MOVE_END,
	}_MoveState;
};