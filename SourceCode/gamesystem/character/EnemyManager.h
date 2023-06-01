﻿#pragma once
#include "Player.h"
#include "FirstBoss.h"
#include "SecondBoss.h"
#include "NormalEnemy.h"
#include "TutorialEnemy.h"
#include<vector>
#include<memory>
//敵の管理系クラス
class EnemyManager {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyManager(const std::string& sceneName);
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//登場シーン
	void AppearUpdate();
	//バトル更新
	void BattleUpdate();
	//撃破
	void DeadUpdate();
	//チュートリアル用の更新
	void TutorialUpdate(int pattern);
	//描画
	void Draw(DirectXCommon* dxCommon);
	void SpriteDraw(DirectXCommon* dxCommon);
	//チュートリアル用の描画
	void TutorialDraw(DirectXCommon* dxCommon);
	//ImGuiの描画
	void ImGuiDraw();
	//チュートリアルのエネミーが全撃破
	bool AllDeadEnemy();
	//一瞬のフラグ管理
	void FinishCheck();
	//ハート生成の終了
	void FinishHeart();
	//ボスの向き
	void DirSet(int Dir);
public://getter setter
	//敵関係getで取る
	InterBoss* GetBoss() { return boss.get(); }
	const XMFLOAT3& GetEnemyPosition() { return boss.get()->GetPosition(); }
	const bool GetEnemyCheck() { return boss.get()->GetCheck(); }
	const bool GetEnemyFinishAppear() { return boss.get()->GetFinishAppear(); }

	const bool GetBirthHeart() { return boss.get()->GetBirthHeart(); }
	//チュートリアル関係のgetset
	InterEnemy* GetEnemy(const int num) { return tutorialenemy[num].get(); }
	std::vector<InterEnemy*>GetBulEnemy() { return bulletenemy; }
private:
	Player* player = Player::GetInstance();
	//ボス(初期化によってステージごとのボスに変更)
	unique_ptr<InterBoss> boss;
	//ファーストステージ用
	static const int firstEnemyMax = 3;
	std::vector<InterEnemy*>bulletenemy;
	//チュートリアル用
	static const int tutorialEnemyMax = 6;
	std::array<unique_ptr<InterEnemy>, tutorialEnemyMax>tutorialenemy;
	//シーンネーム
	std::string m_SceneName;
public:
	//敵の死亡処理
	bool BossDestroy();
	void SetDeadThrow(bool f) { boss->SetThrowUpdateF(f); }
};