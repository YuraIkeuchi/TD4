#pragma once
#include "BaseScene.h"
#include "FourthStageActor.h"
//ゲームプレイシーン
class FourthStage : public BaseScene
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//開放
	void Finalize() override;
};