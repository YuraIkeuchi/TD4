#pragma once
#include "DirectXCommon.h"
#include "ObjCommon.h"
//�G���
class InterEnemy :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://getter setter

protected:

public:
	//virtual ~InterEnemy() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual bool Initialize()override;
	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	void Update();
	
	virtual void Action() = 0;//�G�̍s��

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)override;

	virtual void ImGuiDraw() = 0;

	virtual void Appearance() = 0;
protected: //�ÓI�����o�ϐ�
	bool isAlive;
	bool isStop = false;
	int HP;
	bool ShotF;
	int ShotCount;
public:
	void SetShotF(bool f) { ShotF = f; }
	void SetIsStop(bool isStop) {this->isStop = isStop; }

	bool GetShotF() { return ShotF; }
	int GetShotCount() { return ShotCount; }

	bool GetisAlive() { return isAlive; }

	float GEtAlpha() { return m_Color.w; }

	void SetAlive(bool f) { isAlive = f; }

	void SetColor(XMFLOAT4 color) { m_Color = color; }

	void SetHP(int HP) { this->HP = HP; }
protected:
	void ColPlayer();
	void OnCollision();
	void DeathAction();
protected:
	bool canRot;

	bool Rush;
public:
	void SetcanRotandRush(bool f) { canRot = f; Rush = f; }
};