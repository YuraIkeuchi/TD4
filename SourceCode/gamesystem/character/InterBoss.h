#pragma once
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "InterBullet.h"
#include "ObjCommon.h"
#include "BreakEffect.h"
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
	enum ThirdBossInst {
		None = 0,
		StopGhost,
		ChangeGhost,
		SpawnEnemy,
		FinishMove,

	};
public:
	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	//ボス登場シーンの更新
	void AppearUpdate();
	//ボス撃破シーンの更新
	void DeadUpdate();

	virtual void SkipInitialize() = 0;
	/// <summary>
	/// ポーズのとき
	/// </summary>
	virtual void Pause() = 0;
	//描画
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();//ImGuiの描画

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;

protected:

	virtual void Action() = 0;//ボス特有の処理

	virtual void AppearAction() = 0;//ボス登場の固有の処理

	virtual void DeadAction() = 0;//ボス撃破の固有の処理

	virtual void DeadAction_Throw() = 0;//ボス撃破の固有の処理 スロー

	virtual void ImGui_Origin() = 0;//ボスそれぞれのImGui

private:
	void BirthEffect();
public:
	void SummonEnemyUpda(std::vector<InterEnemy*> boss);
	void SummonEnemyDraw(std::vector<InterEnemy*> boss, DirectXCommon* dxcomn);
	void EndSummon(std::vector<InterEnemy*> boss);
	void isRespawn(std::vector<InterEnemy*> boss);

	void SpawnSetEnemy(vector<unique_ptr<InterEnemy>> enemys);

protected:
	bool SummonF;
	bool SummobnStop;
	bool SearchF;// = Collision::GetLength(m_Position, l_player) < 25.f;
private:
	XMFLOAT3 EffectFirstPos;
public://gettersetter
	bool GetIsAlive() { return isAlive; }

	void SetHP(float hp) { m_HP = hp; };
	float GetHP() { return m_HP; }

	void SetLimit(float limit) { m_Limit = limit; };
	float GetLimit() { return m_Limit; }

	void SetStrong(bool strong) { isStrong = strong; };
	bool GetStrong() { return isStrong; }

	void SetSearch(bool isSearch) { this->isSearch = isSearch; };
	bool GetSearch() { return isSearch; }

	void SetInstruction(int isInstruction) { this->isInstruction = isInstruction; };
	int GetInstruction() { return isInstruction; }

	float HpPercent();

	void SetCheck(bool Check) { m_Check = Check; };
	bool GetCheck() { return m_Check; }

	void SetFinishApp(bool FinishApp) { m_FinishApp = FinishApp; };

	bool GetFinishAppear() { return m_FinishAppear; }

	bool GetBirthHeart() { return m_BirthHeart; }
	void SetBirthHeart(bool HeartBirth) { m_BirthHeart = HeartBirth; };

	void SetDirEmo(int DirEmo) { m_DirEmo = DirEmo; };

	void SetJackPos(const int num, XMFLOAT3 pos) { jackPos[num] = pos; };

	void SetIsReferCheck(bool isReferCheck) { this->isReferCheck = isReferCheck; };
	bool GetIsReferCheck() { return isReferCheck; }

private:
	std::string SceneName;
	vector<InterEffect*> effects;
protected:
	//ダメージ食らったとの色変換
	float ColChangeEaseT;
	int ActionTimer;

	int m_CheckTimer = {};

	int ActionDamage;
	int ActionCool;

	bool isAlive = true;
	float m_HP = {};
	float m_MaxHp = {};
	float m_Limit = 20.0f;
	bool isStrong = false;
	//ゴーストを5たいサーチ
	bool isSearch = false;
	//ゴーストを削除しスポーンを止めます。
	int isInstruction = 0;
	//ゴーストの状態がSearch状態かしれます
	bool isReferCheck = false;
	bool m_Check = false;
	XMFLOAT3 m_OBBScale = {};

	//弾とボスの当たり判定に使う大きさ
	float m_Radius = 0.0f;

	//登場シーンが終わったかどうか
	bool m_FinishAppear = false;

	//どっちの顔を向けているか
	int m_DirEmo = {};
	bool m_FinishApp = false;

	int m_DeathTimer = 0;

	bool m_BirthHeart = false;

	//ダメージ倍率
	float m_Magnification = {};

	//
	array<XMFLOAT3, 5> jackPos = {};
private:

	enum class ActionList {
		NON,
		NORMAL,
		MOB_A,
		MOB_B,
		FRONT
	}_action;

	struct DefaultParam {
		float Cool;
		int Damage;
		std::vector<float>ParSize;
		bool Shake;
	};
protected:
	//弾との当たり判定
	enum class Type {
		CIRCLE,
		SPHERE
	};
	void CollideBul(vector<InterBullet*>bullet, Type type = Type::SPHERE);
	bool EndSummonRepos;
	bool ResF;
public:
	bool Recv;
	int NextActionInteval;
	bool IdleRota;

	bool GetIdleRotaF() { return IdleRota; }
protected:
	bool DeathSceneF;
	void DeathAction();
	void DeathAction_Throw();
	bool ThrowUpdateF;

public:
	bool GetDeathAction() { return DeathSceneF; }
	void SetThrowUpdateF(bool f) { ThrowUpdateF = f; }

	int m_BirthTarget = {};
};

