#pragma once
#include"BaseScene.h"
#include"FiveStageActor.h"

class FiveStage:public BaseScene
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon)override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//解放
	void Finalize()override;
};

