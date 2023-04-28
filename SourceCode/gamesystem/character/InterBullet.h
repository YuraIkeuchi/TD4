#pragma once
#include "ObjCommon.h"
using namespace std;         //  名前空間指定

//弾の基底クラス
class InterBullet :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	
	//描画
	void Draw(DirectXCommon* dxCommon);

	virtual void Action() = 0;//弾特有の処理
	//ImGui既定
	void ImGuiDraw();
	//特有のImGui
	virtual void ImGui_Origin() = 0;
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }
	const int& GetBulletType() { return m_BulletType; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetAngle(const XMFLOAT2& Angle) { m_Angle = Angle; }
	void SetBulletType(const int BulletType) { m_BulletType = BulletType; }

public:
	XMFLOAT2 m_Angle = {};//弾の角度
	float m_AddSpeed = {};//加速度
	int m_BulletType = {};//弾の種類
	int m_Timer = {};//出現時間
	int m_TargetTimer = {};//出現時間の目標
	bool m_Alive = true;//生存フラグ
};