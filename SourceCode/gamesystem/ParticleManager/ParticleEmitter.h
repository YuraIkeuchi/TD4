#pragma once
#include "ParticleManager.h"

using namespace std;         //  名前空間指定
/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //シングルトン化
	//コンストラクタを隠蔽
	ParticleEmitter() = default;
	//デストラクタを隠蔽
	~ParticleEmitter() = default;
public: //メンバ関数
	static ParticleEmitter* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void PlayerDrawAll();
	void FlontDrawAll();
	void FireDrawAll();
	void SmokeDrawAll();
	//エフェクト
	void FireEffect(int life,XMFLOAT3 l_pos,float startscale,float endscale,XMFLOAT4 startcolor,XMFLOAT4 endcolor);
	//ヒール
	void HealEffect(int life, XMFLOAT3 l_pos, float startscale, float endscale, XMFLOAT4 startcolor, XMFLOAT4 endcolor);
	//足元の煙
	void HootEffect(int life, XMFLOAT3 l_pos, float startscale, float endscale, XMFLOAT4 startcolor, XMFLOAT4 endcolor);
	//看板の煙
	void BoardEffect(int life, XMFLOAT3 l_pos, float startscale, float endscale, XMFLOAT4 startcolor, XMFLOAT4 endcolor);
	//死んだ時のエフェクト
	void DeathEffect(int life, XMFLOAT3 l_pos, float startscale, float endscale, XMFLOAT4 startcolor, XMFLOAT4 endcolor,float velocity);
	//削除
	void AllDelete();

private: //メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

private: //メンバ変数
	//パーティクルマネージャー(丸)
	unique_ptr<ParticleManager> circleParticle;
	//パーティクルマネージャー(ヒール)
	unique_ptr<ParticleManager> healParticle;
	//死んだ時のパーティクル
	unique_ptr<ParticleManager> deathParticle;
	//煙
	unique_ptr<ParticleManager> smokeParticle;
	//煙
	unique_ptr<ParticleManager> boardParticle;
};
