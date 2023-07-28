#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include"Font.h"
#include "Helper.h"
/// タイトルシーン
class TitleSceneActor : public BaseActor {
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

	void SceneSelect();
	void FrontDraw();
	void TitleMove();//タイトルの文字の動き
private:
	static const int TITLE_MAX = 3;//タイトルパーツの最大数
private://メンバ変数
	array<unique_ptr<IKESprite>, TITLE_MAX> TitlePartsSprite;
	//スプライト
	unique_ptr<IKESprite> TitleSprite;
	unique_ptr<IKESprite> TitleWordSprite;
	//タイトルのパーツに必要なもの
	array<float, 3> m_Angle;
	array<float, 3> m_Angle2;
	array<XMFLOAT2, TITLE_MAX> m_PartsPos;
	array<XMFLOAT2, TITLE_MAX> m_PartsSize;
	XMFLOAT2 size = { 708.0f,354.0f };
	XMFLOAT2 pos = { 200.0f,150.0f };
	float frame = 0.0f;

	bool m_Change = false;
	
	enum SelectType {
		NORMAL_SCENE,
		SELECT_SCENE,
		SELECT_AREA,
	}_SelectType;
};