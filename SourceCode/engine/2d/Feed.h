#pragma once
#include"IKESprite.h"
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
class Feed
{
public:
	Feed();
private:
	std::unique_ptr<IKESprite>FeedTex;
	float m_Alpha;
	bool m_FeedFlag;

	XMFLOAT3 Color_RGB;
	int FeedInCounter;
private:
	enum class FeedPhase
	{
		NON,
		FEEDIN,
		FEEDOUT,
		END
	}_phase;
public:
	enum class FeedType
	{
		BLACK,
		WHITE
	}_color;

	//type:フェードの色(白か黒) feedspeed:フェードのスピード feedf:フェード開始フラグ
	void FeedIn(FeedType type,float feedspeed,bool &feedf);
	void Draw();

	void SetIni() { _phase = FeedPhase::NON; }
	bool GetFeedEnd();

	float GetAlpha() { return m_Alpha; }
};

