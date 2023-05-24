#pragma once
#include "DirectXCommon.h"
#include "ObjCommon.h"
//敵基底
class InterEnemy :
	public ObjCommon {
protected:
	// DirectX::を省略
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
	/// 初期化
	/// </summary>
	virtual bool Initialize()override;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();
	
	virtual void Action() = 0;//敵の行動

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)override;

	virtual void ImGuiDraw() = 0;

	virtual void Appearance() = 0;
protected: //静的メンバ変数
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