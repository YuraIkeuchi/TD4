#pragma once
#include<DirectXMath.h>
#include<memory>
#include<string>
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
	void TutorialUpdate()override;
	//更新
	void FirstUpdate()override;
	//更新
	void SecondUpdate()override;
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
public:
	static void SetEnemyManager(EnemyManager* boss) { LoadStageObj::boss = boss; }
protected:
	static EnemyManager* boss;

private:
	//当たり判定
	void Collide();
private:
	LightGroup* lightgroup = nullptr;
	std::vector<Ghost*> ghosts;
	std::vector<Food*> foods;
	std::vector<Heart*> hearts;
	//ゴーストを消す処理
	float m_VanishCount = 0.0f;
	bool m_Vanish = false;

	float m_Division = 0.0f;

	//丸影
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,2,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.8f };

	string m_SceneName;

	int m_HeartCount = 2;
};

