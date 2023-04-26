#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "JsonLoader.h"

#include <array>
/// タイトルシーン
class LoadSceneActor : public BaseActor {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void SpriteDraw();
private:
	void CreateStage();
	void IntroUpdate()override;
	void MainUpdate()override;
	void FinishUpdate()override;


private://メンバ変数
	int m_LoadTimer = 0;
	const int LoadTimerMax = 60;

	JsonData* m_JsonData = nullptr;
	std::list<std::unique_ptr<IKEObject3d>> grounds;
	std::map<std::string, std::unique_ptr<IKEModel>> models;

	std::unique_ptr<IKEModel> modelSkydome = nullptr;
	std::unique_ptr<IKEModel> modelGround = nullptr;
	std::unique_ptr<IKEModel> modelFighter = nullptr;
	std::unique_ptr<IKEModel> modelSphere = nullptr;
	std::unique_ptr<IKEModel> modelPine = nullptr;

	//Loading
	enum {
		text_L=0,
		text_O,
		text_A,
		text_D,
		text_I,
		text_N,
		text_G,
		SpriteMax
	};


	std::array<std::unique_ptr<IKESprite>, SpriteMax> m_Sprites = {};
	std::array<XMFLOAT2, SpriteMax> m_SpritesPos = {};
	std::array<float, SpriteMax> m_SpritesAngle = {};

};