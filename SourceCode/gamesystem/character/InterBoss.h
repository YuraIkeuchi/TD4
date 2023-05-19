﻿#pragma once
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "InterBullet.h"
#include "ObjCommon.h"
#include <array>       // ヘッダファイルインクルード

#include "InterEnemy.h"
using namespace std;         //  名前空間指定

//ボスの基底クラス
class InterBoss :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	bool GetIsAlive() { return isAlive; }
	//gettersetter
public:

	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	/// <summary>
	/// ポーズのとき
	/// </summary>
	virtual void Pause() = 0;
	//描画
	void Draw(DirectXCommon* dxCommon);

	virtual void Action() = 0;//ボス特有の処理

	virtual void ImGui_Origin() = 0;//ボスそれぞれのImGui

	void ImGuiDraw();//ImGuiの描画

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;

public:
	void SummonEnemyInit(InterEnemy* enemy);

	void SummonEnemyUpda(std::vector<InterEnemy*> enemy);

	void SummonEnemyDraw(std::vector<InterEnemy*> enemy, DirectXCommon* dxcomn);

	void EndSummon(std::vector<InterEnemy*> enemy);
protected:
	bool SummonF;
	bool SummobnStop;
public://gettersetter
	void SetHP(float hp) { m_HP = hp; };
	float GetHP() { return m_HP; }

	void SetCheck(bool Check) { m_Check = Check; };
	bool GetCheck() { return m_Check; }

private:
protected:
	int ActionTimer;
	int ActionDamage;
	int ActionCool;

	bool isAlive;
	float m_HP = {};

	bool m_Check = false;
private:

	enum class ActionList
	{
		NON,
		NORMAL,
		MOB_A,
		MOB_B,
		FRONT
	}_action;

	struct DefaultParam
	{
		float Cool;
		int Damage;
		std::vector<float>ParSize;
		bool Shake;
	};
protected:
	//弾との当たり判定
	void CollideBul(vector<InterBullet*>bullet);
	void isRespawn(std::vector<InterEnemy*> enemy);
	bool ResF;
public:
	bool Recv;
	int NextActionInteval;
};

