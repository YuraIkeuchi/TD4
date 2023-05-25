#pragma once
#include "Player.h"
#include "FirstBoss.h"
#include "SecondBoss.h"

#include "NormalEnemy.h"
#include "TutorialEnemy.h"

#include<array>
#include<vector>

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
	//更新
	void Update();
	//チュートリアル用の更新
	void TutorialUpdate(int pattern);
	//描画
	void Draw(DirectXCommon* dxCommon);
	//チュートリアル用の描画
	void TutorialDraw(DirectXCommon* dxCommon);
	//ImGuiの描画
	void ImGuiDraw();
	//チュートリアルのエネミーが全撃破
	bool AllDeadEnemy();
	//ボスの撃破
	void FinishCheck();
public://getter setter
	//敵関係getで取る
	InterBoss* GetBoss() { return enemy.get(); }
	const XMFLOAT3& GetEnemyPosition() { return enemy.get()->GetPosition(); }
	const bool GetEnemyCheck() { return enemy.get()->GetCheck(); }
	//チュートリアル関係のgetset
	InterEnemy* GetEnemy(const int num) { return tutorialenemy[num].get(); }
	std::vector<InterEnemy*>GetBulEnemy() { return bulletenemy; }
private:
	Player* player = Player::GetInstance();
	//ボス(初期化によってステージごとのボスに変更)
	unique_ptr<InterBoss> enemy;
	//ファーストステージ用
	static const int firstEnemyMax = 3;
	std::vector<InterEnemy*>bulletenemy;
	//チュートリアル用
	static const int tutorialEnemyMax = 6;
	std::array<unique_ptr<InterEnemy>, tutorialEnemyMax>tutorialenemy;
	//シーンネーム
	std::string SceneName;
public:
	//敵の死亡処理
	bool BossDestroy();
	//シーンネームの取得
	void SetSceneName(std::string name) { SceneName = name; }
	
};