#pragma once
#include <DirectXMath.h>
#include <memory>
#include <string>
#include "Ghost.h"
#include "Food.h"
#include "Heart.h"
#include "LoadManager.h"
#include "EnemyManager.h"
#include "LightGroup.h"
using namespace DirectX;
class LoadStageObj :public LoadManager
{

public:
	//ゴーストロード
	void GhostLoad();
	//食べ物ロード
	void FoodLoad(const std::string& sceneName);
	//すべてのロード
	void AllLoad(const std::string& sceneName);
	//初期化
	void Initialize()override;
	//更新
	void TutorialUpdate();
	//更新
	void FirstUpdate();
	//更新
	void SecondUpdate();
	//更新
	void ThirdUpdate();
	//更新
	void FourthUpdate();

	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
	//ライトのセット
	void LightSet(LightGroup* light);
	//ライトの更新
	void LightUpdate();
private:
	//食料の検索
	void SearchFood();
	//食料とゴーストの当たり判定
	void CollideFood();
	//ゴーストが消える
	void VanishGhost();
	//共通の更新
	void CommonUpdate();
	//ハートの生成
	void BirthHeart();
	//ライトのサイド生成
	void LightReturn();
	//サードボスに関する関数をまとめています
	void ThirdBossAction();
	//ゴーストをエネミーに変換します
	void ChangeGhost2Enemy();
	//ゴーストをエネミーに変換します
	void ChangeGhost2Hyper();
	//近場のゴーストをロックします
	void LockVerseGhost();
	//多くのゴーストをロックします
	void LockAllGhost();

	//ゴーストを削除します。
	void NonVerseGhost();
	//参照されているゴーストが存在するかチェックします。
	bool CheckReferGhost();
	//手に入れているゴーストを三体ほど消します
	void SubHunger();
public:
	static void SetEnemyManager(EnemyManager* m_EnemyManager) { LoadStageObj::m_EnemyManager = m_EnemyManager; }
protected:
	static EnemyManager* m_EnemyManager;
private:
	//当たり判定
	void Collide();
private:
	LightGroup* lightgroup = nullptr;
	std::vector<Ghost*> ghosts;
	//
	static const int kStopGhostMax = 10;
	array<Ghost*, kStopGhostMax> stopGhosts;
	std::vector<Food*> foods;
	std::vector<Heart*> hearts;
	//ゴーストを消す処理
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;

	float m_Division = 0.0f;

	int kStopGhorstMax = 3;

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.8f };

	string m_SceneName;

	float m_Alpha = {};

	//飢餓ゲージを減らす
	bool m_SubHunger = false;
	float m_Frame = 0.0f;

	float m_LimitHunger = {};
};

