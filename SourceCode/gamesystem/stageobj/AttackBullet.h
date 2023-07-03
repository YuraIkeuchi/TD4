#pragma once
#include "InterBullet.h"
//言霊クラス
class AttackBullet :
	public InterBullet {
public:
	AttackBullet();

	bool Initialize() override;//初期化

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGui_Origin() override;
private:
	//弾の動く処理
	void Action() override;

private:
	//ふつうの弾
	void NormalShot();
	//ボスが吸収したときの動き
	void AbsorptionShot();
private:
	static const int POWER_MAX = 4;
private:
	//イージング後の位置
	XMFLOAT3 m_AfterPos = {};
	//円運動
	float m_CircleScale = 30.0f;
	float m_CircleSpeed = {};

	vector<float>m_Power;
};
