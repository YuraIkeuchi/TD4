#pragma once
#include "Player.h"
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "ObjCommon.h"
#include <array>       // ヘッダファイルインクルード
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
	void SetPlayer(Player* player) { this->player.reset(player); }

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

private:
protected:
	//各クラス
	unique_ptr<Player> player = nullptr;
};

